# Postal
Modern C++ version of Running With Scissors' game 'Postal'

####Changes made:
* Removed unused files.
* Reorganized files with proper file names.
* Removed `#include` strangeness and now use the standard include practice.
* Replaced `CompilerOptions.h` and localization headers with build argument defines.
* Replaced projects files with a simple QtCreator project file.
* Replaced non-standard types and platform specific types with standard types.
 * `U8`/`UCHAR`/`uint8` -> `uint8_t`
 * `S8`/`char`/`int8` -> `int8_t`
 * `U16`/`USHORT`/`uint16` -> `uint16_t`
 * `S16`/`short`/`int16` -> `int16_t`
 * `U32`/`ULONG`/`uint32` -> `uint32_t`
 * `S32`/`long`/`int32` -> `int32_t`
 * `U64`/`uint64` -> `uint64_t`
 * `S64`/`int64` -> `int64_t`
* Replaced `long*` type being used for arbitrary storage with `Variant`.
* Replaced Localization system with string literal type hashed lookups. (much cleaner)
* Network code is no longer compiled when networking is disabled.
* Replaced some DEEPLY nested condition trees (23 levels!) with simple exception handling.
