"""
ETGG 4803 Artificial Intelligence Bridge Problem
* N number of people need to get across a bridge that is covered in holes and it is dark
* There is only one flashlight and the people can only travel in pairs of Two
* The people travelling across the bridge can only go as fast as the slowest of the two
* The flashlight cannot be thrown back over the bridge and must be walked back to the other side
* What is the most optimal time to get everyone across the bridge

Example:
    A = 10
    B = 5
    C = 2
    D = 1

    Solution:
        C and D go across.                      Time Taken = 2 min's
        D takes flashlight back across bridge.  Time Taken = 1 min
        A and B go across.                      Time Taken = 10 min's
        C takes flashlight back across bridge.  Time Taken = 2 min's
        C and D go across.                      Time Taken = 2 min's
        All people are across bridge            Totel Time Taken = 17 min's
"""

import math

# quicksort for sorting list given as it may not always be sorted
def quickSort(arrayToSort):
    less = []
    equal = []
    greater = []
    if len(arrayToSort) > 1:
        pivot = arrayToSort[0]
        for i in arrayToSort:
            if i < pivot:
                less.append(i)
            elif i == pivot:
                equal.append(i)
            elif i > pivot:
                greater.append(i)

        return quickSort(less)+equal+quickSort(greater)
    else:
        return arrayToSort

"""
This uses Theorem 1 from Gunter Rote's article "Crossing the Bridge at Night".
The Theorem states:
    The minimum time to cross the bridge is min{C0, C1,...,C[N/2]−1},
with
    Ck = (N − 2 − k)t0 + (2k + 1)t1 + (for(i=2;i<N;i++)ti) - (for(i=0;i<k;i++)tN+1-2i)
    
Should return the min time to get N people across the bridge if the list of times provided is sorted
"""
def findMinSolutionTime(times):
    N = len(times)
    K = int(N/2)
    solutionTimes = list()
    minK = 0
    minTime = math.inf

    print("listLen: "+str(N)+" Methods: "+str(K))
    if N == 0:
        minTime = 0
    elif N == 1:
        minTime = times[0]
    else:
        for k in range(K):
            time = (N - 2 - k) * times[0] + (2 * k + 1) * times[1]
            for i in range(2, N):
                time += times[i]

            for i in range(1, k):
                t = N + 1 - 2*i
                time -= times[t]

            print("Method: " + str(k)+" time: "+str(time))

            if time < minTime:
                minTime = time
                minK = k

    return (minTime, minK)


# Sides of bridge in list form
# Sort FromSide
From = list(quickSort([1, 4, 6, 10]))
To = list() #deque([], maxlen=len(From))
timeValue = "Minutes"

timeAndK = findMinSolutionTime(From)

print("The min time for the order of "+str(From)+" is: "+str(timeAndK[0])+" k is: "+str(timeAndK[1]))