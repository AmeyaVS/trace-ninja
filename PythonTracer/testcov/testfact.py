#!/usr/bin/python

from __future__ import print_function

def fact(n):
    if n == 0:
        return 1
    else:
        return n*fact(n - 1)

def main():
    i = 20
    print("fact(%d): %d" %(i, fact(i)))

if __name__ == '__main__':
    main()
