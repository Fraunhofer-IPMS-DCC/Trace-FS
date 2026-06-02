bindir=$1
trace=$2
tool=../tools/NexRv/NexRv-arm64

$tool -dump $trace > fw-emsa5.dump

#zstd -19 fw-emsa5.dump

#mv fw-emsa5.dump.zst $bindir
