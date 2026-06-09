# Example trace obained for the firmware

This directory contains the firmware binaries (`boot-emsa5.elf` and `fw-emsa5.elf`) which have been executed on the EMSA5.
The directory also contains the trace stream after extraction from the UDP frames (`example-trace.bin`) as well as after conversion into CTXP format (`example-trace.ctxp.zst`, compressed with `zstd`).

The following snippet shows the first ten lines of the CTXP trace:
```
HDR:format=accemic//ctxp-txt,ver=1
META:#0="CPU0"
#0:SYNC::0x80 @ 1092300355
#0:BRANCH_TAKEN:0x88:0xa1040000 @ 1092300357
#0:DAQ_DATA::0x42 @ 1092300375
#0:SYNC::0xa1040000 @ 1092300375
#0:CALL:0xa1040034:0xa1042648 @ 1092300914
#0:CALL:0xa1042658:0xa1041000 @ 1092300914
#0:RETURN:0xa1041028:0xa104265c @ 1092300914
#0:CALL:0xa104265c:0xa104102c @ 1092301198
...
```
