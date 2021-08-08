#include "types.h"
#include "x86.h"

uint8_t chunk_bitmap[256] = {0};
uint8_t chunk_buffer[256 * 8 * 64];

uint8_t* chunk_bitmap_p = chunk_bitmap;
uint8_t* chunk_buffer_p = chunk_buffer;

void config_chunk(void* pbitmap, void* pbuffer)
{
  chunk_bitmap_p = pbitmap;
  chunk_buffer_p = pbuffer;
}

void* new_chunk(void)
{
  uint16_t i;

  for (i = 0; i < 256 * 8; i++) {
    if (! btcsr(0, chunk_bitmap_p, i)) {
      btcsr(2, chunk_bitmap_p, i);
      return (void*) (chunk_buffer_p + i * 64);
    }
  }

  return NULL;
}

void free_chunk(void* ptr)
{
  if (ptr != NULL) {
    btcsr(3, chunk_bitmap_p, ((uint32_t) ptr - (uint32_t) chunk_buffer_p) / 64);
  }
}

/*end*/
