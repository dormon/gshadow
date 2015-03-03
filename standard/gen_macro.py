#!/usr/bin/python
import sys

#test na nespravny pocet argumentu
if len(sys.argv)<2:
	sys.exit()

# pocet zanoreni makra
num=int(sys.argv[1])

#test na nespravnou hloubku zanoreni
if num<1:
	sys.exit()

#dalsi pravidla
for i in range(num-1):
	print "#define LIST_MAP"+str(num-i)+"(f,x,...) f(x) LIST_MAP"+str(num-i-1)+"(f,__VA_ARGS__)"

#posledni pravidlo
print "#define LIST_MAP1"+"(f,x,...) f(x)"



