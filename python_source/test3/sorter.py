with open('bin.txt','r') as bin:
    prefixes = eval(bin.read())

print(str(sorted(prefixes)))
#print(str(sorted(prefixes,key = prefixes.get)))
