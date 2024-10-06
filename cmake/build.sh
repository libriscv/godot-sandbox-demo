#!/bin/bash
set -e

CXX="ccache riscv64-unknown-elf-g++"
#CXX="ccache riscv64-linux-gnu-g++-14"

mkdir -p .build
pushd .build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain.cmake
time ninja
popd
