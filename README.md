# HotelManager

## License

This game is currently under proprietary license. More details are available
in LICENSE.md

## Changelog

To see the changelog, please look at CHANGELOG.md

## Development

### C++

If methods should be bound to GodotEngine API and are not bindable (example: enums),
you should add a wrapper function suffixed with \_\_api name

## Build

To build this project, please first compile the embedded Godot engine

For Linux platform:

```
cd godotengine
scons platform=x11 \
	tools=yes \
	target=release_debug \
	use_llvm=yes \
	builtin_zlib=yes \
	openssl=yes \
	colored=yes \
	pulseaudio=no bits=64
```

## Launch editor

Just run the compiled editor from godotengine folder

```
./godotengine/bin/godot.x11.opt.tools.64 -e
```
