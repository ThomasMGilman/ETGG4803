using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using UnityEngine;

public class Main : MonoBehaviour
{
    private FileReader fileReader;
    private DominoParser mazeParser;
    private List<List<DominoNode>> maze;

    private HashSet<Vector2> Visited;
    private SortedList<int, Queue<DominoNode>> toVisit;
    private Dictionary<Vector2, Path> shortestPathFromStart;
    private List<Vector2> orderChecked;
    private DominoNode start;
    private DominoNode end;

    public bool usingAStar;
    private bool reachedEnd = false, printedEnd = false;

    public float waitTime = 2;
    private float timer = 2;
    private Stopwatch stopwatch;

    // Start is called before the first frame update
    void Start()
    {
        this.fileReader = new FileReader();
        this.mazeParser = new DominoParser(fileReader.getFileLines());
        this.maze = mazeParser.getDominoMaze();

        shortestPathFromStart = new Dictionary<Vector2, Path>();
        orderChecked = new List<Vector2>();
        this.start = this.mazeParser.startNode;
        this.end = this.mazeParser.endNode;

        Visited = new HashSet<Vector2>();
        toVisit = new SortedList<int ,Queue<DominoNode>>();

        this.start.costToGetToFromStart = 0;
        start.cost = 0;
        addToVisit(start);
        start.DominoPiece.GetComponent<Renderer>().material.SetColor("_BaseColor", Color.green);
        shortestPathFromStart.Add(start.getPlaceInMaze(), new Path() { cost = start.costToGetToFromStart, path = new List<DominoNode> { start } });

        stopwatch = new Stopwatch();
        stopwatch.Start();
    }

    void addToVisit(in DominoNode val)
    {
        int cost = val.cost;
        if(toVisit.ContainsKey(cost))
            toVisit[cost].Enqueue(val);
        else
        {
            Queue<DominoNode> newQueue = new Queue<DominoNode>();
            newQueue.Enqueue(val);
            toVisit.Add(cost, newQueue);
        }
    }

    DominoNode getToVisit()
    {
        int smallestKey = toVisit.Keys[0];
        DominoNode node = toVisit[smallestKey].Dequeue();

        if (toVisit[smallestKey].Count == 0)
            toVisit.Remove(smallestKey);
        return node;
    }

    // Update is called once per frame
    void Update()
    {
        if (!reachedEnd)
        {
            if (timer <= 0)
            {
                traverse();
                timer = waitTime;
            }
            else
                timer -= Time.deltaTime;
        }
        else
        {
            if (!printedEnd)
                printShortestPathToDestination();
        }
    }

    struct Path
    {
        public int cost;
        public List<DominoNode> path;
    }

    public void printShortestPathToDestination()
    {
        print("Using " + (usingAStar ? "A*" : "Dijkstras") + " Algorithm");
        print("Reached Goal in: " + stopwatch.Elapsed.TotalSeconds.ToString() + "Seconds");
        print("The shortest path To '" + mazeParser.end + "' From '" + mazeParser.start + "'\n\tDistance: " + shortestPathFromStart[end.getPlaceInMaze()].cost.ToString());
        print("\tPath: ");
        int index = 0;
        foreach (DominoNode dominoNode in shortestPathFromStart[end.getPlaceInMaze()].path)
        {
            print("\t\t" + dominoNode.getPlaceInMaze());
            dominoNode.DominoPiece.GetComponent<Renderer>().material.SetColor("_BaseColor", Color.green);
            index++;
        }
        printedEnd = true;
    }

    public void printPathToNode(Vector2 dest)
    {
        foreach (DominoNode dominoNode in shortestPathFromStart[dest].path)
            print("\t\t[" + dominoNode.getPlaceInMaze().x.ToString() + "," + dominoNode.getPlaceInMaze().y.ToString() + "]");
        print("\tCost: " + shortestPathFromStart[dest].cost + "\n");
    }

    public void printOutPaths()
    {
        print("Paths:");
        foreach (KeyValuePair<Vector2, Path> NodePath in shortestPathFromStart)
        {
            print("\tTo " + NodePath.Key + ":");
            printPathToNode(NodePath.Key);
        }
    }

    /// <summary>
    /// Copy path to get to neighbor into newPath and append new end point
    /// </summary>
    /// <param name="currentDominoNode"></param>
    /// <param name="newPath"></param>
    /// <param name="dest"></param>
    public void copyPath(in DominoNode currentDominoNode, out List<DominoNode> newPath, in DominoNode dest)
    {
        newPath = new List<DominoNode>();
        foreach (DominoNode dominoNode in shortestPathFromStart[currentDominoNode.getPlaceInMaze()].path)
            newPath.Add(dominoNode);
        newPath.Add(dest);
    }

    public void traverse()
    {
        if (toVisit.Count > 0)
        {
            DominoNode currentDominoNode = getToVisit();
            orderChecked.Add(currentDominoNode.getPlaceInMaze());
            Visited.Add(currentDominoNode.getPlaceInMaze());

            if (currentDominoNode.getPlaceInMaze() == end.getPlaceInMaze())
            {
                stopwatch.Stop();
                reachedEnd = true;
                return;
            }
            else
            {
                currentDominoNode.DominoPiece.GetComponent<Renderer>().material.SetColor("_BaseColor", Color.green);
                
                foreach (DominoNode neighboringDomino in currentDominoNode.connections)
                {
                    int costToGetTo = shortestPathFromStart[currentDominoNode.getPlaceInMaze()].cost + 1;
                    if(!Visited.Contains(neighboringDomino.getPlaceInMaze()))
                        neighboringDomino.DominoPiece.GetComponent<Renderer>().material.SetColor("_BaseColor", Color.blue);

                    // Neightbor already visited before, 
                    // check if path to it from this city is cheaper then current path
                    if (Visited.Contains(neighboringDomino.getPlaceInMaze()) || shortestPathFromStart.ContainsKey(neighboringDomino.getPlaceInMaze()))
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
                        addToVisit(neighboringDomino);

                        List<DominoNode> pathToCity;
                        copyPath(in currentDominoNode, out pathToCity, in neighboringDomino);
                        if (shortestPathFromStart.ContainsKey(neighboringDomino.getPlaceInMaze()))
                        {
                            printOutPaths();
                            throw new System.Exception("Trying to add CityPath that is already added!!");
                        }
                        else
                            shortestPathFromStart.Add(neighboringDomino.getPlaceInMaze(), new Path() { cost = costToGetTo, path = pathToCity });
                    }
                }
                currentDominoNode.DominoPiece.GetComponent<Renderer>().material.SetColor("_BaseColor", Color.red);
                toVisit[toVisit.Keys[0]].Peek().DominoPiece.GetComponent<Renderer>().material.SetColor("_BaseColor", Color.green);
            }
        }
    }
}
