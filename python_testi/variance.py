import math
grades = [22.0, 20.0, 21.0, 13.0, 41.0]

def print_grades(grades):
    for grade in grades:
        print grade

def grades_sum(grades):
    total = 0
    for grade in grades: 
        total += grade
    return total
    
def grades_average(grades):
    sum_of_grades = grades_sum(grades)
    average = sum_of_grades / len(grades)
    return average

def grades_variance(scores, average):
    vari=0.0
    for v in scores:
        vari+=float(((v**2)-(average**2)))
    return vari/float(len(scores))

ga=grades_average(grades)
va=grades_variance(grades,ga)
def grades_std_deviation(variance):
    return math.sqrt(variance)
#print grades_std_deviation(va)
s=grades_sum(grades)    
print s
a=grades_average(grades)
print a
v=grades_variance(grades,a)
print v
std=grades_std_deviation(v)
print std
