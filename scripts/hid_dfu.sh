#!/bin/bash

run_dir="OpenDeck"

if [[ $(basename "$(pwd)") != "$run_dir" ]]
then
    echo This script must be run from $run_dir directory!
    exit 1
fi

if [ "$(uname)" == "Darwin" ]; then
    find="gfind"

    if [[ "$(command -v gfind)" == "" ]]
    then
        echo "ERROR: GNU find not installed (gfind)"
        exit 1
    fi

    dfu="bin/dfu/hid_bootloader_loader_mac"
elif [ "$(uname -s)" == "Linux" ]
then
    find="find"
    dfu="bin/dfu/hid_bootloader_loader_linux"
fi

echo "Info: In case of errors please run this script with super-user rights."
echo "Please select the board for which you want to update firmware:"

boards=$($find bin/compiled -type f -not -path "*merged*" -name "*.hex" -printf '%f\n' | cut -f 1 -d '.' | sort)
echo "$boards" | tr " " "\n" | cat -n
printf "Board number: "
read -r board_nr

filename=$(echo "$boards" | head -n "$board_nr" | tail -n 1)
path=$($find -type f -not -path "*merged*" -name "${filename}.hex")
mcu=$(echo "$path" | cut -d / -f6)

"$dfu" "$mcu" "$path"