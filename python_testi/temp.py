# -*- coding: utf-8 -*-
"""
Spyder Editor

This temporary script file is located here:
/home/uros/.spyder2/.temp.py
"""
import sys
import os
import re

def Find(kaj,kje):
    najdu=re.search(kaj,kje)
    if najdu:
        print(najdu.group())

def foo():
    Find('ece','abeceda je ece beba')
    print("boo")
    
def main():
    foo()
    pass
    
    
if __name__ == "__main__":
    main()