#!/bin/bash
set -e

usage() {
	echo "Usage: $0 [--api] [-o output] -- [input...]"
	echo "  --api        Instead of compiling, copy the API files to the output."
	echo "   -o output   Compile sources into an output ELF file, including API and inputs."
	echo "  --debug      Compile with debug information."
	echo "  --version    Print the current version of the API."
	echo "   -v          Verbose output."
	echo "  --           Separate options from input files."
	exit 1
}

verbose=false
current_version=5

while [[ "$#" -gt 0 ]]; do
	case $1 in
		--api) cp -r /usr/api $1; exit ;;
		-o) shift; output="$1"; shift; break ;;
		--debug) shift ;;
		--version) shift; echo "$current_version"; exit ;;
		-v) verbose=true; shift ;;
		--) shift; break ;;
		*) usage ;;
	esac
done

if [ -z "$output" ]; then
	usage
fi

# Build the project
cargo +nightly build --release

# Copy the resulting ELF file to the output
cp target/riscv64gc-unknown-linux-gnu/release/rust_program $output
