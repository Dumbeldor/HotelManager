#! /bin/sh

BINPATH="../godotengine/bin/godot.x11.tools.64.llvm"

cd `dirname $0`/game
if [ ! -x ${BINPATH} ]; then
	echo "Godot engine is not compiled. Please compile it with LLVM support"
	exit 1
fi

LLDB_DEBUG=0
while getopts ":d" opt; do
	case $opt in
		d)
			LLDB_DEBUG=1
			;;
		*)
		;;
	esac
done

if [ ${LLDB_DEBUG} -gt 0 ]; then
	lldb ${BINPATH}
else
	${BINPATH}
fi
