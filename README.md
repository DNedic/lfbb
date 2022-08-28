# LFBB - Lock Free Bipartite Buffer
![CMake](https://github.com/DNedic/lfbb/actions/workflows/.github/workflows/cmake.yml/badge.svg)

LFBB is a bipartite buffer implementation written in standard C11, suitable for all platforms, from deeply embedded to HPC uses. It is lock-free for single consumer single producer scenarios making it incredibly performant and easy to use.

## What is a bipartite buffer

A bipartite buffer is a variation of the classic ring buffer with the ability to always be able to provide the user with contigous memory regions inside the buffer for writing/reading if there is enough space/data.
[Here](https://www.codeproject.com/Articles/3479/The-Bip-Buffer-The-Circular-Buffer-with-a-Twist) is a nice writeup about the essence of bipartite buffers.

## Why use a bipartite buffer
* To offload transfers to DMA increasing the transfer speed and freeing up CPU time
* To avoid creating intermediate buffers for APIs that require contigous data
* To process data inside the buffer without dequeing it
* For scenarios where operations on data might fail or only some data might be used
* To use stdlib memcpy which is faster than bytewise implementations used in most queues and ring buffers

## Features
* Written in standard C11, compatible with all platforms supporting it
* Lock free thread safe when used in single producer single consumer scenarios
* No dynamic allocation
* MIT Licensed
* Supports CMake FetchContent()

## How to use
Shown here is an example of typical use:
* Consumer thread/interrupt
```c
size_t data_available;
uint8_t *read_location = LFBB_ReadAcquire(&lfbb_adc, &data_available);

if (read_location != NULL) {
  size_t data_used = DoStuffWithData(read_location, data_available);
  LFBB_ReadRelease(&lfbb_adc, data_used);
}
```

* Producer thread/interrupt
```c
if (!write_started) {
  uint8_t *write_location = LFBB_WriteAcquire(&lfbb_adc, sizeof(data));
  if (write_location != NULL) {
    ADC_StartDma(&adc_dma_h, write_location, sizeof(data));
    write_started = true;
  }
} else {
  if (ADC_PollDmaComplete(&adc_dma_h) {
    LFBB_WriteRelease(&lfbb_adc, sizeof(data));
    write_started = false;
  }
}
```

## Configuration
There are multiple ways to ensure the lock-free nature of LFBB on CPUs with caches:
* Invalidate cache manually (for embedded and freestanding)
* Use the MPU/MMU to disable caching of the memory area containing the instance (for embedded and freestanding)
* Set LFBB_CACHELINE_ALIGN and LFBB_CACHELINE_LENGTH in ```lfbb_config.h``` (for all systems)

On embedded systems it is prefferable to use the first two methods to avoid wasting RAM on padding.

## Caveats
* The library does not implement alignment of writes and reads, it is up to the user to only write in factors they want the data to be aligned to, adequately size and align the buffer used
