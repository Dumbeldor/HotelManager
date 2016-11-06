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

To perform engine compilation, please read the following link:
https://godot.readthedocs.io/en/stable/reference/compiling_for_x11.html

For Linux platform:

```
cd godotengine
scons platform=x11 tools=yes target=debug openssl=yes colored=yes pulseaudio=no use_llvm=yes
```

## Launch editor

Launch _./game/launch\_editor.sh_

## Launch game

Launch _./game/launch\_game.sh_
