"""
Lab1B Solution
    by. Thomas Gilman

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

from collections import deque
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
    K = max(0, int(N/2) - 1)
    solutionTimes = list()
    minK = 0
    minTime = math.inf

    print("listLen: "+str(N)+" Methods: "+str(K)+" List: "+str(times))
    if N == 0:
        minTime = 0
    elif N == 1:
        minTime = times[0]
    else:
        for k in range(K + 1):
            t1 = (N - 2 - k) * times[0]
            t2 = (2 * k + 1) * times[1]
            time = t1 + t2
            for i in range(2, N):
                time += times[i]

            for i in range(1, k):
                t = N-1 + 1 - 2*i
                time -= times[t]

            print("\tMethod: " + str(k)+" time: "+str(time))

            if time < minTime:
                minTime = time
                minK = k

    print("Best Method: "+str(minK)+", Time: "+str(minTime)+"\n")
    return (minTime, minK)


# Sides of bridge in list form
# Sort FromSide
BridgePeople = [1, 4, 6, 10]
From = deque(quickSort(BridgePeople))
To = deque([], maxlen=len(From))
order = list()
timeValue = "Minutes"

# Calculate first k
timeAndK = findMinSolutionTime(From)
minTime = timeAndK[0]

person0 = None
person1 = None
while(len(From) > 0):
    # Only 1 person on the From side, Should only call this in the case of a single person crossing bridge
    if(len(From) == 1):
        order.append((From[0], 0, From[0]))
        To.append(From.pop())

    # The Case where there are 2 people left on the From Side or only two people crossing
    elif (len(From) == 2):
        order.append((From[0], From[1], From[1]))
        To.append(From.pop())
        To.appendleft(From.pop())

    # The Case where there are only 3 people crossing the bridge
    elif(len(From) == 3):
        order.append((From[0], From[-1], From[-1]))
        order.append((From[0], 0, From[0]))
        To.append(From.pop())

    # Cases where N >= 4 crossing bridge and using k
    # Case 1
    elif(timeAndK[1] > 0):
        # Move the two quickest people across first and quickest person back
        order.append((From[0], From[1], From[1]))
        order.append((From[0], 0, From[0]))

        # Move the two slowest people across
        order.append((From[-2], From[-1], From[-1]))
        person0 = From.pop()
        person1 = From.pop()
        To.append(person1)
        To.append(person0)

        order.append((From[1], 0, From[1]))
    # Case 2 k = 0
    else:
        order.append((From[0], From[-1], From[-1]))
        order.append((From[0], 0, From[0]))
        order.append((From[0], From[-2], From[-2]))
        order.append((From[0], 0, From[0]))
        person0 = From.pop()
        person1 = From.pop()
        To.append(person1)
        To.append(person0)
    timeAndK = findMinSolutionTime(From)

print("The order to get everyone with times"+str(BridgePeople)+"\n\tacross the bridge with a minimum time of "+str(minTime)+" "+timeValue+" is: ")
for o in order:
    print("\t\t"+str(o[0])+", "+str(o[1])+"\tTime: "+str(o[2]))