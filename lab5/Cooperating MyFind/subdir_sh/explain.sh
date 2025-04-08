#!/bin/bash

count_lines()
{
    local file="$1"
    local lines=$(wc -l < "$file")
    echo "$lines"
}

print_first_five_lines() 
{
    local file="$1"
    echo "the first five lines from $file are:"
    head -n 5 "$file"
}

recursive_traverse() 
{
    local directory="$1"
    local total_lines=0

    for file in "$directory"/*; do
        if [ -f "$file" ] 
        then
            
            file_type=$(file -b "$file")
            if [[ "$file_type" == *"C source"* ]]
            then
                lines=$(count_lines "$file")
                echo "$lines $file"
                total_lines=$((total_lines + lines))
            else
                print_first_five_lines "$file" >&2
            fi
        elif [ -d "$file" ]
        then
            recursive_traverse "$file"
        fi
    done

    echo "total nr. of text lines from C programs: $total_lines"
    if [ $total_lines -gt 0 ]
    then
        exit 0
    else
        exit 1
    fi
}

if [ $# -ne 1 -o ! -d "$1" ]
then
    echo "Usage: $0 directory"
    exit 1
fi

start_directory="$1"
recursive_traverse "$start_directory"