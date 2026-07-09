# Structure and allocation requirements of EFI_FILE_INFO
[Reference: UEFI-Specification 13.5.16. EFI_FILE_INFO](https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-info)

## Structure Layout
```
typedef struct {
  UINT64                         Size;
  UINT64                         FileSize;
  UINT64                         PhysicalSize;
  EFI_TIME                       CreateTime;
  EFI_TIME                       LastAccessTime;
  EFI_TIME                       ModificationTime;
  UINT64                         Attribute;
  CHAR16                         FileName [];
} EFI_FILE_INFO;
```

## Variable-length filename storage

Unlike the fixed-size fields at the beginning of the structure, FileName is declared as a flexible array member:
```
CHAR16 FileName[];
```
This means that the filename is stored directly after the fixed portion of the structure in the same memory allocation. The structure itself therefore has no fixed total size.
The `FileName` field contains a null-terminated UTF-16 string (`CHAR16`). Since filenames can have different lengths, the required buffer size depends on the actual filename returned by the firmware.

## Required buffer sizing strategy

Because the total size of EFI_FILE_INFO depends on the filename length, callers cannot simply allocate:
```
sizeof(EFI_FILE_INFO)
```

Instead, the buffer must be large enough to hold:
- the fixed structure fields
- the complete UTF-16 filename
- the terminating null character

The firmware reports the required buffer size through the `BufferSize` parameter when the supplied buffer is too small.

## Why two-step allocation patterns are necessary

The required size of EFI_FILE_INFO is not known before calling `EFI_FILE_PROTOCOL.GetInfo()`.

The typical workflow is therefore:
1. Call `GetInfo()` with a NULL or intentionally undersized buffer.
2. The firmware returns `EFI_BUFFER_TOO_SMALL` and updates `BufferSize` with required size.
3. Allocate a buffer of the reported size.
4. Call `GetInfo()`  again using the allocated buffer.

This approach is commonly used throughout UEFI for structures whose size depends on runtime data.