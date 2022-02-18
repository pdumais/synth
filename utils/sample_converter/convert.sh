#!/bin/bash

FOLDER=$1
NAME=$2

if [[ -z "$FOLDER" || -z "$NAME" ]]
then
    echo "Usage: $0 <folder> <basename>"
    exit 1
fi

n=0
for i in "$FOLDER"/*.wav
do
    f="${NAME}${n}" 
    echo $f
    ffmpeg -i "$i" -f s16le -ar 16384 -ac 1 ${f}.pcm
    python3 $(dirname $0)/makeheader.py ${f}.pcm > ${f}.h
    n=$((n+1))
done
