if [ $# -ne 1 ]; then
    exit 2
fi

if [ ! -f "$(dirname "$0")/first_and_last_byte" ]; then
    gcc source/first_and_last_byte.c
    if [ $? -ne 0 ]; then
        echo "error: compilation failed." >&2
        exit 3
    fi
fi

path_of_file=$1

function process_directory()
{
    for entry in "$(dirname "$path_of_file")/*" do
        if [ -f "$entry" ]; then 
            ./souce/first_and_last_byte $entry
        elif [ -d "$entry" ]; then
            path_of_file = $entry
            process_directory
        fi
}

if [ -f path_of_file ]; then
    ./souce/first_and_last_byte $path_of_file
elif [ -d path_of_file ]; then 
    process_directory
fi

exit 0