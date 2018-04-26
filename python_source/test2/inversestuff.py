import os
with open('testinverse_table.txt','r') as f:
    table = eval(f.read())

for j in table:
    print("Adding value: "+str(j[0]))
    f = open("inverse_table.txt","a")
    f.write(str(j[0]) + " " + j[1] + "\n")
