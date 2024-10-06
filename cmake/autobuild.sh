#!/bin/sh

#
# Usage: whenever.sh [pattern] [command]
#
# Executes a command whenever files matching the pattern are closed in write
# mode. "{}" in the command is replaced with the matching filename (via xargs).
# Requires inotifywait from inotify-tools.
#
# For example,
#
#    whenever.sh '\.md$' 'markdown -f $(basename {} .md).html {}'
#
# This runs "markdown -f my-document.html my-document.md" whenever
# my-document.md is saved.
#

set -e -u

FPATH=".."
PATTERN="\.cpp$"
COMMAND="./build.sh"

inotifywait -q --format '%f' -m -r -e close_write $FPATH \
    | grep --line-buffered $PATTERN \
    | xargs -I{} -r sh -c "echo [\$(date -Is)] $COMMAND && $COMMAND"
