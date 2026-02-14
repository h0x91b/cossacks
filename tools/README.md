# gsc_extract

Command-line tool for unpacking `.gsc` resource archives used by Cossacks: Back to War.

## Build

Built automatically by `build.bat` in the repository root. To build manually:

```
cl /nologo /O2 /D_CRT_SECURE_NO_WARNINGS gsc_extract.c /Fe:gsc_extract.exe
```

Requires MSVC (Visual Studio 2022 or compatible).

## Usage

```
gsc_extract.exe <archive.gsc> -l                  List all files
gsc_extract.exe <archive.gsc> -x [-o <outdir>]    Extract all files
gsc_extract.exe <archive.gsc> -x <name> [-o dir]  Extract a single file
gsc_extract.exe <archive.gsc> -i                   Show archive info + file list
gsc_extract.exe <archive.gsc> -v                   Verify FAT entry hashes
```

### Examples

```batch
REM List contents of resources.gsc
gsc_extract.exe Cossacks142\resources.gsc -l

REM Extract everything into an "unpacked" folder
gsc_extract.exe Cossacks142\resources.gsc -x -o unpacked

REM Extract a single file
gsc_extract.exe Cossacks142\resources.gsc -x WEAPON.ADS -o unpacked
```

## GSC archive format

| Section | Size | Description |
|---------|------|-------------|
| Header | 14 B | `descriptor[6]` `version[2]` `key[2]` `entries[4]` |
| FAT | 81 &times; N B | Per-file: `hash[4]` `filename[64]` `offset[4]` `size[4]` `reserved[4]` `flags[1]` |
| Data | variable | Raw file data (no compression) |

Key details:

- **Offsets** in FAT entries are stored bitwise-inverted (`~offset`).
- **Encryption** is optional per-file (`flags=1`): each byte is `NOT`'d then `XOR`'d with a key derived from the header's `key` field.
- **Filenames** are stored uppercase, max 63 characters.
- **Hash** is a sum of 16 big-endian DWORDs over the 64-byte zero-padded uppercase filename.
