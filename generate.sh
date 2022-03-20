#!/bin/sh

mkdir 17clues

for (( i = 0; i < 1000; i++ )); do
	./build/sudokugen 3 17 $i > ./17clues/prob.txt
	cat ./17clues/problems.txt > ./17clues/tmp.txt
	cat ./17clues/tmp.txt ./17clues/prob.txt > ./17clues/problems.txt
done
