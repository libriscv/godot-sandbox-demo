# Readme for Json Diff? for Godot Sandbox

Install msys2 dependencies for Windows.

```bash
pacman -Sy mingw-w64-x86_64-riscv64-unknown-elf-gcc ninja cmake git
```

We want to see a compiler that we can use.

```bash
cd json_diff_sample/json_diff
./compile.sh
```

```bash
# Optional for debugging.
riscv64-unknown-elf-readelf.exe -a json_diff.elf
```

## Debugging

Make sure Visual Studio Code settings has C++ intellisense enabled.
