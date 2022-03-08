#!/usr/bin/env python3

data = open("17puz49158.txt").read().split('\n')
# print(data)
writefile = open("17clues.txt", "w")

for line in data:
	tmp = ""
	for ch in line:
		if ch == '.':
			tmp += '-'
		else:
			tmp += ch
		tmp += ' '
	writefile.write(tmp+'\n')
