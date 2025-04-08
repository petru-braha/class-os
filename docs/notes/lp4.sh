# variabilele sunt by default strings
# command: unset variable

# substitutie ${var:start:count}
# $ (command) - ruleaza comanda intr-un nou shell
# ${var%sufix}
# $# == argc
# $@
# $* == word of all parameters
# read -p "zi ba despre n: " n
# echo $n

# declare -i n = integer
# echo `expr 4 + 1` # write spaces
# echo 3 ^ 2 | bc
# if [ 1 == 1 ]
# then echo du-te in oua
# fi
#  if [ 1 == 2 ] ; then # pui ; daca then este pe aceeasi linie

# 1.
if [ $# -ne 1 ]
then
	echo "invalid number of arguments."
fi

let n = $1
let i = 0
let number = 1
while [ i -le n ]
do
	number = $number * 2

done

power_of_two() {
    local k="$1"

    if [ "$k" -eq 0 ]; then
        echo 1
        return
    fi

    local prev_power
    prev_power=$(power_of_two $((k-1)))
    echo $((prev_power * 2))
}

# Check if a command line argument is provided
if [ $# -ge 1 ]; then
    input_value="$1"
    echo "Using command line argument: $input_value"
else
    echo "No command line argument provided." >&2
    echo -n "Please enter a value: " >&2
    read input_value
    echo "Using input: $input_value"
fi

for ((i=0; i<=input_value; i++)); do
    result=$(power_of_two "$i")
    echo "2^$i = $result" >&2
done
