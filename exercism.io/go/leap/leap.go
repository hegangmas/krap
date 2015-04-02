#!/usr/bin/python

def is_leap_year(year):
    if year % 4 == 0:
        if year % 100 == 0 and not year % 400 == 0:
            return false
        return true
    else:
        return false
