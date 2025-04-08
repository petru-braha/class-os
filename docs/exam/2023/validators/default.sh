if [ "$#" -ne 1 ]; then
    echo "Error: No argument provided." >&2
    exit 2
fi

cd "$(dirname "$0")/validators"

if [ ! -x "validatorASCII" ]; then
    gcc -o validatorASCII validatorASCII.c
    if [ $? -ne 0 ]; then
        echo "Error: Compilation of 'validatorASCII.c' failed." >&2
        exit 3
    fi
fi

./validatorASCII "$1" | tr '-' '>'