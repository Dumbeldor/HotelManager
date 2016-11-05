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

Before compile, read this link for packages:
https://godot.readthedocs.io/en/stable/reference/compiling_for_x11.html

For Linux platform:

```
cd godotengine
scons platform=x11 tools=yes target=release_debug openssl=yes colored=yes pulseaudio=no
```

## Launch editor

Just run the compiled editor from godotengine folder

```
./godotengine/bin/godot.x11.opt.tools.64 -e
```
