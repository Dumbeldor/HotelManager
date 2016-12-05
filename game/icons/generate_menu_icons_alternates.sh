#! /bin/sh

if [ ! -d alternates ]; then
	mkdir alternates
fi

# Tile Menu items
for F in $(find . -maxdepth 1 -name 'icon_menu_tiletype_*.png' -type f | sed 's/.png//g'| sed 's/\.\///g'); do
	composite -gravity center layer_menuitem_hover.png ${F}.png alternates/${F}_hover.png
done

# Tile items
for F in $(find . -maxdepth 1 -name 'icon_tile_*.png' -type f | sed 's/\.png//g' | sed 's/\.\///g'); do
	composite -gravity center layer_menuitem_hover.png ${F}.png alternates/${F}_hover.png
done
