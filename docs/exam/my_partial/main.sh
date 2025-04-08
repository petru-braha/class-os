if [ &# -ne 1 ]; then
    echo "error: insert (just) one parameter"
fi

if [ ! -x "$(dirname "$0")/app/control_byte.sh" ]; then
    echo "error: (the folder 'app' does not exist) or ('control_byte.sh' does not exist or is not executable)." >&2
    exit 1
fi

if [ ! -r "$(dirname "$0")/app/source/first_and_last_byte.c" ]; then
  echo "error: (the folders 'app' or 'source' does not exist) or ('first_and_last_byte.c' does not exist or is not readable)." >&2
  exit 2
fi

if [ ! -d "$1" ] || [ ! -f "$1" ]; then
  echo "error: the file does not exist" >&2
  exit 3
fi

"$(dirname "$0")/app/control_byte.sh" "$1"
exit_code=$?
exit $exit_code