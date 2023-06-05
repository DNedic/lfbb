# Changelog

## 1.0.0

- Initial release

## 1.0.1

- Improved readability of ReadAcquire

## 1.0.2

- Switched to hierarchical CMake setup with tests conditionally pulled in if the library is not pulled in as a dependency

## 1.1.0

- Added optional cacheline alignment enabling multicore safe use on hosted environments and embedded environments that do not manually invalidate cache or disable data buffer caching with the MPU/MMU in a lock free manner

## 1.1.1

- Switched to using C11 standard alignas specifier instead of manually adding padding to the structure for cacheline alignment

## 1.1.2

- Important documentation fixes regarding multicore vs DMA use

## 1.1.3

- Documentation and configuration header improvements

## 1.2.0

- Switched away from needing a configuration header to defining configuration options by the build system or before library inclusion

## 1.2.1

- Refactored for performance improvements and code quality
- Improved documentation in regards to configuration

## 1.2.2

- Improved performance by using a write_wrapped flag instead of infering write wraps

## 1.3.0

- Set ```LFBB_MULTICORE_HOSTED``` to ```false``` by default. This is a more embedded-friendly configuration which doesn't impact hosted platforms performance significantly if the library is left unconfigured but avoids wasting space on embedded devices. When using the library on hosted platforms passing ```LFBB_MULTICORE_HOSTED``` as ```true``` is now advised for maximum performance.

## 1.3.1

- A critical bug where valid data would be invalidated if the write ended exactly on the end of the buffer was fixed - [@KOLANICH](https://github.com/KOLANICH) in [lfbb_cpp](https://github.com/DNedic/lfbb_cpp)

## 1.3.2

- Performance and code conciseness improvements
