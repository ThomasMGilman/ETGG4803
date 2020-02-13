

def normDist(list, pow):
    dist = 0
    for val in range(len(list)):
        dist += abs(list[val])**pow
    return dist**(1/pow)

list = [2, 4]

print(str(normDist(list, 1000)))