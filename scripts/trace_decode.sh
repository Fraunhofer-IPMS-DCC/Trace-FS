bindir=$1
trace=$2
tool=../tools/NexRv/NexRv-arm64

$tool -conv -objd $bindir/fw-emsa5.dis -pcinfo fw-emsa5.pci
$tool -conv -objd $bindir/boot-emsa5.dis -pcinfo boot-emsa5.pci

cat boot-emsa5.pci > foo.pci
cat fw-emsa5.pci >> foo.pci
CTXP_TEXT_TRACEFILE=fw-emsa5.ctxp CEDAR_TRACEFILE=fw-emsa5.trc $tool -deco $trace -pcinfo foo.pci -pcout foo.pco -all > decode.log

#zstd -19 fw-emsa5.trc
zstd -19 fw-emsa5.ctxp

#mv fw-emsa5.trc.zst $bindir
mv fw-emsa5.ctxp.zst $bindir
rm *.pci *.pco #decode.log fw-emsa5.trc fw-emsa5.ctxp
