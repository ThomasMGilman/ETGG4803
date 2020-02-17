using System;
using System.Collections.Generic;
using System.Text;

class DijkstraAStarPathFinder
{
    static Dictionary<Tuple<int, int>, Path> shortestPathFromStart;
    static List<Tuple<int, int>> orderChecked;
    DominoNode start;
    DominoNode end;

    bool usingAStar;

    struct Path
    {
        public int cost;
        public List<DominoNode> path;
    }

    public DijkstraAStarPathFinder(in DominoNode s, in DominoNode e, bool aStarCheck = false)
    {
        shortestPathFromStart = new Dictionary<Tuple<int, int>, Path>();
        orderChecked = new List<Tuple<int, int>>();
        this.start = s;
        this.end = e;
        this.usingAStar = aStarCheck;
    }

    public void printPathToNode(Tuple<int, int> dest)
    {
        foreach (DominoNode dominoNode in shortestPathFromStart[dest].path)
            Console.WriteLine("\t\t[" + dominoNode.getPlaceInMaze().Item1.ToString() + "," + dominoNode.getPlaceInMaze().Item2.ToString() + "]");
        Console.WriteLine("\tCost: " + shortestPathFromStart[dest].cost + "\n");
    }

    public void printOutPaths()
    {
        Console.WriteLine("Paths:");
        foreach (KeyValuePair<Tuple<int, int>, Path> NodePath in shortestPathFromStart)
        {
            Console.WriteLine("\tTo " + NodePath.Key + ":");
            printPathToNode(NodePath.Key);
        }
    }

    /// <summary>
    /// Copy path to get to neighbor into newPath and append new end point
    /// </summary>
    /// <param name="currentDominoNode"></param>
    /// <param name="newPath"></param>
    /// <param name="end"></param>
    public void copyPath(in DominoNode currentDominoNode, out List<DominoNode> newPath, in DominoNode end)
    {
        newPath = new List<DominoNode>();
        foreach (DominoNode dominoNode in shortestPathFromStart[currentDominoNode.getPlaceInMaze()].path)
            newPath.Add(dominoNode);
        newPath.Add(end);
    }

    public void traverse(in List<List<DominoNode>> maze)
    {
        HashSet<DominoNode> Visited = new HashSet<DominoNode>();
        List<DominoNode> toVisit = new List<DominoNode>();

        this.start.costToGetToFromStart = 0;
        start.cost = 0;
        toVisit.Add(start);
        shortestPathFromStart.Add(start.getPlaceInMaze(), new Path() { cost = start.costToGetToFromStart, path = new List<DominoNode> { start } });

        while (toVisit.Count > 0)
        {
            toVisit.Sort();
            DominoNode currentDominoNode = toVisit[0];
            orderChecked.Add(currentDominoNode.getPlaceInMaze());
            Visited.Add(currentDominoNode);

            if (currentDominoNode == end)
                break;

            foreach (DominoNode neighboringDomino in currentDominoNode.connections)
            {
                int costToGetTo = shortestPathFromStart[currentDominoNode.getPlaceInMaze()].cost + 1;

                // Neightbor already visited before, 
                // check if path to it from this city is cheaper then current path
                if (Visited.Contains(neighboringDomino) || shortestPathFromStart.ContainsKey(neighboringDomino.getPlaceInMaze()))
                {
                    // New Path is cheaper to this city
                    if (shortestPathFromStart[neighboringDomino.getPlaceInMaze()].cost > costToGetTo)
                    {
                        neighboringDomino.cost = costToGetTo;

                        List<DominoNode> pathToCity;
                        copyPath(in currentDominoNode, out pathToCity, in neighboringDomino);
                        shortestPathFromStart[neighboringDomino.getPlaceInMaze()] = new Path() { cost = costToGetTo, path = pathToCity };
                    }
                }
                // Otherwise add city to toVisit to have its neighbors checked
                else
                {
                    neighboringDomino.cost = costToGetTo + (usingAStar ? (int)neighboringDomino.getHeuristic() : 0);
                    toVisit.Add(neighboringDomino);

                    List<DominoNode> pathToCity;
                    copyPath(in currentDominoNode, out pathToCity, in neighboringDomino);
                    if (shortestPathFromStart.ContainsKey(neighboringDomino.getPlaceInMaze()))
                    {
                        printOutPaths();
                        throw new Exception("Trying to add CityPath that is already added!!");
                    }
                    else
                        shortestPathFromStart.Add(neighboringDomino.getPlaceInMaze(), new Path() { cost = costToGetTo, path = pathToCity });
                }
            }
            toVisit.RemoveAt(0);
        }
    }
}
