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

#from collections import deque
import time
import math
import threading

finishedStates = []
quickestPatternCount = math.inf
quickestPattern = []
'''
pendingThreads = list()

MAX_NUM_THREADS_ALLOWED = 100
currentThreadsRunning = 0'''
lock = threading.Lock()

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


class Bridge:
    def __init__(self, fromSide, toSide, patternThusFar, count):
        self.timeTaken = count
        self.pattern = list(patternThusFar)     #list of tuples of length 1 or 2
        self.fromSide = list(fromSide)
        self.toSide = list(toSide)

def createNewBridgeState(currentBridgeState, i, j = None):
    if j != None:
        person0 = currentBridgeState.fromSide[i]
        person1 = currentBridgeState.fromSide[j]

        # Copy Data from current bridge class and add changes to the new bridge state
        newFromSide = currentBridgeState.fromSide.copy()
        newFromSide.pop(j)
        newFromSide.pop(i)

        newToSide = currentBridgeState.toSide.copy()
        newToSide.append(person0)
        newToSide.append(person1)

        newBridgeCount = currentBridgeState.timeTaken + (person0 if person0 > person1 else person1)

        newBridgePattern = currentBridgeState.pattern.copy()
        newBridgePattern.append(("crossing:", person0, person1))
        return Bridge(newFromSide, newToSide, newBridgePattern, newBridgeCount)
    else:
        person0 = currentBridgeState.toSide[i]

        newToSide = currentBridgeState.toSide.copy()
        newToSide.remove(i)

        newFromSide = currentBridgeState.fromSide.copy()
        newFromSide.append(person0)

        newBridgeCount = currentBridgeState.timeTaken + person0

        newBridgePattern = currentBridgeState.pattern.copy()
        newBridgePattern.append(("returning:", person0))
        return Bridge(newFromSide, newToSide, newBridgePattern, newBridgeCount)

def printFinishedState(finishedBridgeState):
    lock.acquire()
    print("This Pattern took "+str(finishedBridgeState.timeTaken)+" "+timeValue+" pattern is:")
    for i in range(len(finishedBridgeState.pattern)):
       print("\t" + str(finishedBridgeState.pattern[i]))
    print("\n")
    lock.release()

def checkFinishedState(finishedBridgeState):
    lock.acquire()
    global quickestPatternCount
    global quickestPattern
    if finishedBridgeState.timeTaken < quickestPatternCount:
        quickestPatternCount = finishedBridgeState.timeTaken
        quickestPattern = finishedBridgeState.pattern
    lock.release()

def bridgeCrosser(currentBridgeState, returningTorch):
    '''global currentThreadsRunning

    lock.acquire()
    currentThreadsRunning += 1
    lock.release()'''

    if not returningTorch:
        for i in range(len(currentBridgeState.fromSide)-1):
            for j in range(i+1, len(currentBridgeState.fromSide)):
                person0 = currentBridgeState.fromSide[i]
                person1 = currentBridgeState.fromSide[j]

                # Copy Data from current bridge class and add changes to the new bridge state
                newFromSide = currentBridgeState.fromSide.copy()
                newFromSide.pop(j)
                newFromSide.pop(i)

                newToSide = currentBridgeState.toSide.copy()
                newToSide.append(person0)
                newToSide.append(person1)

                newBridgeCount = currentBridgeState.timeTaken + (person0 if person0 > person1 else person1)

                newBridgePattern = currentBridgeState.pattern.copy()
                newBridgePattern.append(("crossing:", person0, person1))

                newBridgeState = Bridge(newFromSide, newToSide, newBridgePattern, newBridgeCount)
                # This bridge State has an empty from side meaning its finished, add it to the list of finished states
                if len(newBridgeState.fromSide) == 0:
                    finishedStates.append(newBridgeState)
                    checkFinishedState(newBridgeState)
                    #printFinishedState(newBridgeState)
                else:
                    #bridgeCrosser(newBridgeState, True)
                    t = threading.Thread(target=bridgeCrosser, args=(newBridgeState, True)).start()

                    '''lock.acquire()
                    if currentThreadsRunning < MAX_NUM_THREADS_ALLOWED:
                        t.start()
                    else:
                        pendingThreads.append(t)
                    currentThreadsRunning -= 1
                    lock.release()'''

    # Handle Returning the torch across the bridge case
    else:
        for i in range(len(currentBridgeState.toSide)):
            person0 = currentBridgeState.toSide[i]

            newToSide = currentBridgeState.toSide.copy()
            newToSide.pop(i)

            newFromSide = currentBridgeState.fromSide.copy()
            newFromSide.append(person0)

            newBridgeCount = currentBridgeState.timeTaken + person0

            newBridgePattern = currentBridgeState.pattern.copy()
            newBridgePattern.append(("returning:", person0))
            newBridgeState = Bridge(newFromSide, newToSide, newBridgePattern, newBridgeCount)
            #bridgeCrosser(newBridgeState, False)
            t = threading.Thread(target=bridgeCrosser, args=(newBridgeState, False)).start()

            '''lock.acquire()
            if currentThreadsRunning < MAX_NUM_THREADS_ALLOWED:
                t.start()
            else:
                pendingThreads.append(t)
            currentThreadsRunning -= 1
            lock.release()'''




# Sides of bridge in list form
# Sort FromSide
From = list(quickSort([1, 4, 6, 20]))
To = list() #deque([], maxlen=len(From))
timeValue = "Minutes"

# Starting Bridge State
startBridge = Bridge(From, To, [], 0)

# Start Crossing Bridge brute force method
bridgeCrosser(startBridge, False)

while(threading.activeCount() > 1):
    continue


print("Quickest pattern had a time of "+str(quickestPatternCount)+" pattern is:")
for i in range(len(quickestPattern)):
    print("\t"+str(quickestPattern[i]))

#Code below is work in progress for optimal solution
"""
for i in range(len(finishedStates)):
    if finishedStates[i].timeTaken < quickestPatternCount:
        quickestPatternCount = finishedStates[i].timeTaken
        quickestPattern = finishedStates[i].pattern
    print("Pattern "+str(i)+" took "+str(finishedStates[i].timeTaken)+timeValue+" pattern is:")
    for j in range(len(finishedStates.pattern)):
        print("\t"+finishedStates.pattern[i])
"""

"""
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





print("Total time taken to get everyone across is: ", str(timeTaken)+" "+timeValue)"""

