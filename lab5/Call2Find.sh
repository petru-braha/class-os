#! /bin/bash

if [ $# -eq 1 ]
then
	n=$1
else
	echo "Insert number"
	read n
fi

if [! files=$(find ~ -maxdepth $n -type f -mtime -28)]
then
	echo "Command find failed" >&2
	exit 1
fi

for file in $files
do
	if [ file "$file" | grep -q "C source" ] then
		filename=$(basename "$file")
		filename_no_ext="${filename%.*}"

		if [ gcc "$file" -o "${file%/*}/$filename_no_ext" ] 
		then
			echo "$file -> Compiled"
		else
			echo "$file -> ERROR"
		fi

	else
		mod_date=$(stat -c %y "$file" | cut -d' ' -f1)
		echo "$mod_date $file"
	fi
done