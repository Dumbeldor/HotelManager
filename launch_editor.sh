#! /bin/bash
cd `dirname $0`/game
if [ ! -x "../godotengine/bin/godot.x11.tools.64.llvm" ]; then
	echo "Godot engine is not compiled. Please compile it with LLVM support"
	exit 1
fi
BINPATH="../godotengine/bin/godot.x11.tools.64.llvm"

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
	${BINPATH} -e
fi

