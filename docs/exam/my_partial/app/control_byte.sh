if [ &# -ne 1 ]; then
    echo "error: insert (just) one parameter"
fi

if [ ! -x "$(dirname "$0")/recurse.sh" ]; then
    echo "error: 'recurse.sh' does not exist or is not executable." >&2
    exit 1
fi

function increase_digit()
{
    local digit=$1
    if [ $digit -eq 9 ]; then
        digit=0
    else
        digit=$digit + 1
    fi
}

function sorting_all()
{
    local string=$1
    sort string
    #se vor sorta liniile numeric crescător după al doilea câmp delimitat de caracterul "=".
}

"$(dirname "$0")/recurse.sh" "$1" | change_digit | sorting_all
#"$iwashere" "$maindir" | sort -r | uniq | tr -d '/'
x
exit_code=$?
exit $exit_code
