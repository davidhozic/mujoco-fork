#ifdef MUJOCO_MIRI_SUPPORT

#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <new>

#include <mujoco/mjexport.h>

extern "C" {
  void* mju_malloc(size_t size);
  void mju_free(void* ptr);

  // set by setup_miri_chunk_allocator
  extern void* (*mju_user_malloc)(size_t);
  extern void (*mju_user_free)(void*);
}

// Global C++ operator new/delete overrides.
// These will catch all allocations within libmujoco.so.

void* operator new(std::size_t size) {
    return mju_malloc(size);
}

void operator delete(void* ptr) noexcept {
    if (ptr) {
        mju_free(ptr);
    }
}

extern "C" {
  //------------------------------ Miri Chunk Allocator ----------------------------------------------

  // Miri cannot hand Rust a reference to memory that this library allocated, so MuJoCo allocates
  // from memory that Rust owns. Each block gets a whole chunk of its own, so Miri knows the bounds
  // of every block separately. One shared pool would put a block overrun inside the same Miri
  // allocation, where nothing reports it.

  #define MIRI_MAX_CHUNKS 32768

  static unsigned char* miri_chunk_base[MIRI_MAX_CHUNKS];
  static size_t miri_chunk_size[MIRI_MAX_CHUNKS];
  static unsigned char* miri_chunk_block[MIRI_MAX_CHUNKS];  // handed-out block, 0 while free
  static size_t miri_chunk_count = 0;

  // Index of the first chunk that holds size bytes. The caller registers the chunks in ascending
  // size order, so this is a binary search over miri_chunk_size.
  static size_t miri_lower_bound(size_t size) {
      size_t lo = 0, hi = miri_chunk_count;
      while (lo < hi) {
          size_t mid = lo + (hi - lo) / 2;
          if (miri_chunk_size[mid] < size) {
              lo = mid + 1;
          } else {
              hi = mid;
          }
      }
      return lo;
  }

  static void* miri_chunk_malloc(size_t size) {
      // mju_malloc pads to 64 on its default path, so keep the same slack and the same alignment.
      size_t padded = size ? ((size + 63) / 64) * 64 : 64;
      // Take the first free chunk at or above the requested size. A class that runs out therefore
      // spills into the next one up, and only an empty pool fails.
      for (size_t i = miri_lower_bound(padded); i < miri_chunk_count; i++) {
          if (miri_chunk_block[i]) {
              continue;
          }
          // Place the block against the end of the chunk. A one-byte overrun then leaves the
          // chunk, which is the boundary Miri checks.
          unsigned char* ptr = miri_chunk_base[i] + miri_chunk_size[i] - padded;
          miri_chunk_block[i] = ptr;
          return ptr;
      }
      printf("MIRI_LOG: miri_chunk_malloc(%zu) FAILED: no free chunk holds %zu bytes\n",
             size, padded);
      return NULL;
  }

  static void miri_chunk_free(void* ptr) {
      for (size_t i = 0; i < miri_chunk_count; i++) {
          if (miri_chunk_block[i] == ptr) {
              miri_chunk_block[i] = 0;
              return;
          }
      }
      // A block from before the hooks were installed. Dropping it leaks, which stays safe.
  }

  // Registers the chunks that every later allocation is carved from, and installs the hooks.
  // Each base must be 64-byte aligned, each size a multiple of 64, and the sizes must ascend.
  MJAPI void setup_miri_chunk_allocator(unsigned char** bases, const size_t* sizes, size_t count) {
      if (count > MIRI_MAX_CHUNKS) {
          printf("MIRI_LOG: setup_miri_chunk_allocator: %zu chunks exceed the %d maximum\n",
                 count, MIRI_MAX_CHUNKS);
          count = MIRI_MAX_CHUNKS;
      }
      for (size_t i = 0; i < count; i++) {
          miri_chunk_base[i] = bases[i];
          miri_chunk_size[i] = sizes[i];
          miri_chunk_block[i] = 0;
      }
      miri_chunk_count = count;

      mju_user_malloc = miri_chunk_malloc;
      mju_user_free = miri_chunk_free;
  }
}

#endif
