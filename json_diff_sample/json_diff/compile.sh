#!/bin/bash

# ccache is optional, but recommended

# Create build directory
mkdir -p build
pushd build
AR="riscv64-unknown-elf-ar" CXX="riscv64-unknown-elf-g++" CC="riscv64-unknown-elf-gcc" cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain.cmake
cmake --build .
popd
