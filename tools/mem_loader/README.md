# Mem-Loader

Loads two binary files passed on the command line:
- the ITCM image to `0xA0100000`
- the BRAM image to `0xA1040000`

Firmware must be compiled as binary file (not as an .elf).
This can be done using `riscv64-unknown-elf-objcopy`.


`mem_loader` uses a single configured `32 KiB` window size for both memory
windows. That matches the currently implemented CPU0 ITCM backing RAM, and also
matches the BRAM window currently used by the design.

Files must be at least `4-byte` aligned.
If the file size is only `4-byte` aligned but not `8-byte` aligned, the loader
accepts it and zero-pads the final 4 bytes in memory.

The loader stages the file in normal RAM first and then writes the target through
explicit aligned `32-bit` MMIO stores. This avoids `fread`/`memcpy`/`memset`
directly into the `/dev/mem` mapping, which can cause bus errors on this target.

Compiling and running on Kria KR260
```
gcc -Wall -Wextra -Werror -o mem_loader mem_loader.c
chmod 744 mem_loader
./mem_loader <itcm-binary-file> <bram-binary-file>
```
