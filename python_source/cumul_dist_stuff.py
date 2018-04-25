''' Create prefixes dictionary 'prefix': [numRandom, cardLength, probWeight]
i.e.
prefixes = {
    '5235**': [2, 8, 100],
    '123456': [0, 8, 1]
}
'''
with open('bin.txt','r') as bin:
    prefixes = eval(bin.read())

############################################################################

def create_prefix_ordered_list(prefixes):
    return sorted(prefixes,key = prefixes.get)
self.prefix_order = create_prefix_ordered_list(prefixes)

def create_cumul_fxn(prefix_order, prefixes, total_prob):
    cumul_prob = 0
    prefix_cumul = {}
    for prefix in prefix_order:
        prefix_cumul[prefix] = cumul_prob
        cumul_prob += float(prefixes[prefix][2]) / total_prob
    return prefix_cumul
self.prefix_cumul = create_cumul_fxn(self.prefix_order, prefixes, self.total_prob)

def cumul(self, m):
    #print m
    prefix = list('******')
    for i in range(6):
        prefix[i] = m[i]
        prefixStr = ''.join(prefix)
        if prefixStr in self.prefixes:
            #last digit is the check dig
            randomDigs = m[6-self.prefixes[prefixStr][0]:-1]
            numRandomDigs = self.prefixes[prefixStr][1] - 7
            prefixCumul = self.prefix_cumul[prefixStr]
            totalCumul = prefixCumul + float(randomDigs)*pow(10,-numRandomDigs) / self.total_prob
            return totalCumul
    print "Invalid credit card"
    return -1
cumul(m)
