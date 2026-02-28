#ifdef MUJOCO_MIRI_SUPPORT

#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <new>

#include <mujoco/mjexport.h>

extern "C" {
  void* mju_malloc(size_t size);
  void mju_free(void* ptr);

  // set by setup_miri_bump_allocator
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
  //------------------------------ Miri Bump Allocator -----------------------------------------------

  static unsigned char* miri_bump_buffer = 0;
  static size_t miri_bump_size = 0;
  static size_t miri_bump_offset = 0;

  static void* miri_bump_malloc(size_t size) {
      if (!miri_bump_buffer) return NULL;
      if (miri_bump_offset + size > miri_bump_size) {
          printf("MIRI_LOG: miri_bump_malloc(%zu) FAILED (offset %zu, size %zu)\n", size, miri_bump_offset, miri_bump_size);
          return NULL;
      }
      void* ptr = miri_bump_buffer + miri_bump_offset;
      miri_bump_offset += size;
      // Align to 64
      if (miri_bump_offset % 64 != 0) {
          miri_bump_offset += 64 - (miri_bump_offset % 64);
      }
      return ptr;
  }

  static void miri_bump_free(void* ptr) {
      // no-op for bump allocator
  }

  // Set up the bump allocator and hook into mju_user_malloc
  MJAPI void setup_miri_bump_allocator(unsigned char* buffer, size_t size) {
      printf("MIRI_LOG: setup_miri_bump_allocator(%p, %zu)\n", buffer, size);
      miri_bump_buffer = buffer;
      miri_bump_size = size;
      miri_bump_offset = 0;
      
      mju_user_malloc = miri_bump_malloc;
      mju_user_free = miri_bump_free;
  }
}

#endif
