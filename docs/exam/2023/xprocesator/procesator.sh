if [[ ! -x "$(dirname "$0")/xprocesator/procesator.sh" ]]; then
  echo "Error: 'procesator.sh' does not exist or is not executable." >&2
  exit 1
fi

if [[ ! -r "$(dirname "$0")/validators/validatorASCII.c" ]]; then
  echo "Error: 'validatorASCII.c' does not exist or is not readable." >&2
  exit 2
fi

if [[ ! -d "$1" ]] || [[ ! -r "$1" ]]; then
  echo "Error: The provided directory does not exist or is not readable." >&2
  exit 3
fi

"$(dirname "$0")/xprocesator/procesator.sh" "$1"
exit_code=$?

exit $exit_code