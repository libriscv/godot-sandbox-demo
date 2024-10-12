#!/bin/bash
set -e

#CXX="ccache riscv64-unknown-elf-g++"
#CC="ccache riscv64-unknown-elf-gcc"
CC="ccache riscv64-linux-gnu-gcc-12"
CXX="ccache riscv64-linux-gnu-g++-12"

mkdir -p .build
pushd .build
cmake .. -G Ninja -DSANDBOX_RISCV_EXT_C=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain.cmake
time ninja
popd
