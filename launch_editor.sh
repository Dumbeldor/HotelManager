#! /bin/bash
cd `dirname $0`/game
if [ ! -x "../godotengine/bin/godot.x11.tools.64.llvm" ]; then
	echo "Godot engine is not compiled. Please compile it with LLVM support"
	exit 1
fi
../godotengine/bin/godot.x11.tools.64.llvm -e
