import csv
import numpy as np

binary_rows = []



with open('square_opal.txt', 'r') as file:
    mylist = []
    content = file.readlines()
    content = [x.split() for x in content] 

    

to_write = open('2matrix.txt', 'w')
count = 0
for i in content:
    for j in i:
        to_write.write(j + "\n")


to_write.close()
