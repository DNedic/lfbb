# LFBB - Lock Free Bipartite Buffer

LFBB is a bipartite buffer implementation written in standard C11, suitable for all platforms, from deeply embedded to HPC uses. It is lock-free for single consumer single producer scenarios making it incredibly performant and easy to use.

## What is a bipartite buffer

A bipartite buffer is a variation of the classic ring buffer with the ability to always be able to provide the user with contigous memory regions inside the buffer for writing/reading if there is enough space/data.
[Here](https://www.codeproject.com/Articles/3479/The-Bip-Buffer-The-Circular-Buffer-with-a-Twist) is a nice writeup about the essence of bipartite buffers.

## Features
* Written in standard C11, compatible with all platforms supporting it
* Especially suitable for DMA transfers
* Lock free thread safe when used in single producer single consumer scenarios
* No dynamic allocation
* MIT Licensed
* Supports CMake FetchContent()

## Caveats
* The library is still work in progress at the moment, more documentation, tests and examples will be added
* The library does not implement alignment of writes and reads, it is up to the user to only write in factors they want the data to be aligned to, adequately size and align the buffer used
* Cache invalidation must be manually done for DMA transfers
