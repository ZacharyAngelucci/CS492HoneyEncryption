import os
with open('oldbin.txt','r') as bin:
    prefixes = eval(bin.read())

for prefix,val in prefixes.iteritems():
    print("Adding prefix: "+prefix)
    f = open("bin.txt","a")
    f.write(prefix + " " + str(val[0]) + " " + str(val[1]) + " " + str(val[2]) + "\n")
