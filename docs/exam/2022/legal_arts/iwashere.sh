#!/bin/bash
if [ $# -ne 1 ]; then
    echo "choose (just) one parameter - file"
    exit 2
fi

gcc legal_arts/performances/filegraffiti.c -o legal_arts/executable
file_name=$1

function process_file 
{
    local file=$1 
    local user=$(whoami) 
    if [ -f "$file" ]; then 
        ./legal_arts/executable "$file" "$user"
        if [ $? -ne 0 ]; then 
            exit 3
        fi
    fi
}

function process_directory 
{
    local dir=$1
    for entry in "$dir"/*; do
    if [ -d "$entry" ]; then
        "$0" "$entry"
    else
        process_file "$entry"
    fi
    done
}

if [ -d $filename ]; then
    process_directory "$file_name"

else
    process_file "$file_name"
fi

exit 0