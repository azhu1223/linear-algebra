#!/bin/sh

mkdir -p build

if [ "$#" -gt 0 ]; then
    if [ "$#" -gt 1 ] || [ "$1" = "--help" ]; then
        echo "Usage: ./build.sh [--full/--help]"
        exit 0
    elif [ "$1" = "--full" ]; then
        for file in ./build/*; do
            [ "$file" = "./build/_deps" ] && continue
            rm -rf "$file"
        done
    fi
fi

(cd build && cmake .. && cmake --build .)
