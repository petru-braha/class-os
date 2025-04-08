if [ $# -ne 1 ]; then
    exit 5
fi

sdir="$(dirname "$(realpath "$0")")"
iwashere="$sdir/legal_arts/iwashere.sh"
if [ ! -f "$iwashere" ]; then
 echo "Error: iwashere Not found in legal_arts" >&2
 exit 1
fi 

if [ ! -x "$iwashere" ]; then
 chmod +x "$iwashere"
fi

performance="$sdir/legal_arts/performance"
graffiti_c="$performance/graffiti.c"
if [ ! -f "graffiti_c" -o ! -r "$graffiti_c" ]; then
 echo "Error: graffiti.c was not located in 'performance' directory or it is not readable" >&2
 exit 2
fi

graffiti="$performance/graffiti"
if [ ! -f "$graffiti" ]; then 
 gcc "$graffiti_c" -o "graffiti"
 if [ $? -ne 0 ]; then 
  echo "Error: Compilation Error" >&2
  exit 3
 fi
fi

maindir=$1
if [ ! -d "$maindir" -o ! -r "$maindir" ]; then
 echo "Error: Arguement directory not readabl   e" >&2
 exit 4
fi 

"$iwashere" "$maindir" | sort -r | uniq | tr -d '/'
exit 0