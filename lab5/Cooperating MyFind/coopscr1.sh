#! /bin/bash

if [ $# -ne 1 ]
then
	echo "Using: #0 <direct>"
	exit 1
fi

dir=$1

if [ ! -d "$dir" ]
then
	echo "Directory $dir doesn't exist" >&2
	exit 1
fi

if [ ! -r "$dir" ]
then
	echo "Directory $dir has not read rights" >&2
	exit 2
fi
