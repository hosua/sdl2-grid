#!/bin/bash

# rm -rf build && \
mkdir build
cd build
cmake ..
compiledb make && \
mv compile_commands.json .. && \
./grid-game
