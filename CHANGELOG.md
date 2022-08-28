# Changelog

## 1.0.0

- Initial release

## 1.0.1

- Improved readability of ReadAcquire

## 1.0.2

- Switched to hierarchical CMake setup with tests conditionally pulled in if the library is not pulled in as a dependency

## 1.1.0

- Added optional cacheline alignment enabling use on hosted environments and embedded environments that do not manually invalidate cache or disable instance caching with the MPU/MMU in a lock free manner

## 1.1.1

- Switched to using C11 standard alignas specifier instead of manually adding padding to the structure for cacheline alignment
