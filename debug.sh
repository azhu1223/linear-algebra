#!/bin/sh

mkdir -p debug

if [ "$#" -gt 0 ]; then
    if [ "$#" -gt 1 ] || [ "$1" = "--help" ]; then
        echo "Usage: ./debug.sh [--full/--help]"
        exit 0
    elif [ "$1" = "--full" ]; then
        for file in ./build/*; do
            [ "$file" = "./debug/_deps" ] && continue
            rm -rf "$file"
        done
    fi
fi

(cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && cmake --build .)
