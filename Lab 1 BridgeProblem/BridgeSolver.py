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

from collections import deque

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


# People and their times
A = 1
B = 2
C = 5
D = 10

timeValue = "Minutes"

# Sides of bridge in list form
# Sort FromSide
From = deque(quickSort([A, B, C, D]))
To = deque([], maxlen=len(From))

# People still on other side of bridge
EvenPass = True
timeTaken = 0
person0 = 0
person1 = 0

if len(From) == 1:
    person0 = From.pop()
    timeTaken += person0
    To.append(person0)
else:
    while len(From) > 0:
        if EvenPass:
            # Fastest and second Fastest pair and travel across, timeTaken is slowest of the two
            person0 = From.popleft()
            person1 = From.popleft()
            To.appendleft(person1)
            To.appendleft(person0)
            timeTaken += person1
        else:
            # Slowest People cross over and fastest person on other side brings flashlight back across
            person1 = From.pop()
            person0 = From.pop()
            To.append(person0)
            To.append(person1)
            timeTaken += person1

        # Fastest person on other side brings flashlight back if people still on other side
        if len(From) > 0:
            if EvenPass:
                person0 = To.popleft()
                From.appendleft(person0)
            else:
                person0 = To.popleft()
                From.insert(1, person0)
            timeTaken += person0
            EvenPass = not EvenPass

print("Total time taken to get everyone across is: ", str(timeTaken)+" "+timeValue)