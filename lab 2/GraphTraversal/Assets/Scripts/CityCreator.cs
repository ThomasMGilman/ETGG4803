using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using UnityEngine;

public class CityCreator : MonoBehaviour
{
    public static GameObject CityPrefab;
    public GameObject cPrefabIn;
    public TextAsset cityPack;

    public bool usingAStar = false;

    [Range(0.0f, 1.0f)]
    public float PositionScaler = 1;

    private Dictionary<string, City> map;
    private Dictionary<string, Path> shortestPathFromStart;
    private HashSet<City> Visited;
    private SortedList<int, City> toVisit;
    private City start, end;

    private bool reachedEnd, printedEnd;

    LineRenderer lineRenderer;
    public float waitTime = 2;
    private float timer = 2;
    private Stopwatch stopwatch;

    // Start is called before the first frame update
    void Start()
    {
        CityPrefab = cPrefabIn;
        map = new Dictionary<string, City>();
        shortestPathFromStart = new Dictionary<string, Path>();
        Visited = new HashSet<City>();
        toVisit = new SortedList<int, City>();//new Queue<City>();
        reachedEnd = false;
        printedEnd = false;

        parseCityData();
        toVisit.Add(0, start);//.Enqueue(start);
        shortestPathFromStart.Add(start.cityName, new Path() { cost = start.costToGetToFromStart, path = new List<City> { start } });

        lineRenderer = this.gameObject.AddComponent<LineRenderer>();

        start.cityObj.GetComponent<Renderer>().material.SetColor("_BaseColor", Color.green);
        stopwatch = new Stopwatch();
        stopwatch.Start();

    }

    // Update is called once per frame
    void Update()
    {
        if(!reachedEnd)
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

    public void printShortestPathToDestination()
    {
        lineRenderer.positionCount = shortestPathFromStart[end.cityName].path.Count;

        print("Using " + (usingAStar ? "A*" : "Dijkstras") + " Algorithm");
        print("Reached Goal in: " + stopwatch.Elapsed.TotalSeconds.ToString() + "Seconds");
        print("The shortest path To '" + end.cityName + "' From '" + start.cityName + "'\n\tDistance: " + shortestPathFromStart[end.cityName].cost.ToString());
        print("\tPath: ");
        int index = 0;
        foreach (City c in shortestPathFromStart[end.cityName].path)
        {
            print("\t\t" + c.cityName);
            lineRenderer.SetPosition(index, c.cityObj.transform.position);
            index++;
        }
        printedEnd = true;
    }

    public void printOutPaths()
    {
        print("Paths:");
        foreach (KeyValuePair<string, Path> cityPath in shortestPathFromStart)
        {
            print("\tTo " + cityPath.Key + ":");
            foreach (City c in cityPath.Value.path)
                print("\t\t" + c.cityName);
            print("\tCost: " + cityPath.Value.cost + "\n");
        }
    }

    /// <summary>
    /// Copy path to get to neighbor into newPath and append new end point
    /// </summary>
    /// <param name="neighboringCity"></param>
    /// <param name="newPath"></param>
    /// <param name="end"></param>
    public void copyPath(in string neighboringCity, out List<City> newPath, in City end)
    {
        newPath = new List<City>();
        foreach (City c in shortestPathFromStart[neighboringCity].path)
            newPath.Add(c);
        newPath.Add(end);
    }

    /// <summary>
    /// Goes through each Citys connections from the start all the way to the finish and finds the shortest path to the end city
    /// </summary>
    public void traverse()
    {
        if(toVisit.Count == 0)
            reachedEnd = true;
        else
        {
            City currentCity = toVisit[toVisit.Keys[0]];
            if (currentCity == end)
            {
                stopwatch.Stop();
                reachedEnd = true;
                return;
            }
            LineRenderer newLine = currentCity.cityObj.AddComponent<LineRenderer>();
            
            newLine.positionCount = currentCity.connections.Count * 2;
            newLine.material = new Material(Shader.Find("Unlit/Texture"));
            newLine.material.color = Color.red;

            Visited.Add(currentCity);
            currentCity.cityObj.GetComponent<Renderer>().material.SetColor("_BaseColor", Color.red);
            int lineIndex = 0;
            foreach (KeyValuePair<int, City> connection in currentCity.connections)
            {
                City neighboringCity = connection.Value;
                int costToGetTo = shortestPathFromStart[currentCity.cityName].cost + connection.Key;

                Vector3 s = currentCity.cityObj.transform.position;
                Vector3 e = neighboringCity.cityObj.transform.position;
                s.y = 10;
                e.y = 10;

                newLine.SetPosition(lineIndex, s);
                newLine.SetPosition(lineIndex + 1, e);
                lineIndex += 2;

                // Neightbor already visited before
                // check if path to it from this city is cheaper then current path
                if (Visited.Contains(neighboringCity) || shortestPathFromStart.ContainsKey(neighboringCity.cityName))
                {
                    // New Path is cheaper to this city
                    if (shortestPathFromStart[neighboringCity.cityName].cost > costToGetTo)
                    {
                        List<City> pathToCity;
                        copyPath(in currentCity.cityName, out pathToCity, in neighboringCity);
                        shortestPathFromStart[neighboringCity.cityName] = new Path() { cost = costToGetTo, path = pathToCity };
                    }
                }
                // Otherwise add city to toVisit to have its neighbors checked
                else
                {
                    toVisit.Add(costToGetTo + (usingAStar ? connection.Value.heuristic : 0), neighboringCity);
                    List<City> pathToCity;
                    copyPath(in currentCity.cityName, out pathToCity, in neighboringCity);
                    if (shortestPathFromStart.ContainsKey(neighboringCity.cityName))
                    {
                        printOutPaths();
                        throw new System.Exception("Trying to add CityPath that is already added!!");
                    }
                    else
                        shortestPathFromStart.Add(neighboringCity.cityName, new Path() { cost = costToGetTo, path = pathToCity });
                }
            }
            toVisit.RemoveAt(0);
            toVisit[toVisit.Keys[0]].cityObj.GetComponent<Renderer>().material.SetColor("_BaseColor", Color.green);
        }
    }

    public struct Path
    {
        public int cost;
        public List<City> path;
    }

    public class City
    {
        public string cityName;
        public SortedList<int, City> connections;
        public int heuristic, costToGetToFromStart;
        public GameObject cityObj;

        public struct connection
        {
            public City otherCity;
            public int costOfTravel;
        };

        public City(string name, int heuristic = 0, int costToGetTo = System.Int32.MaxValue, Transform parent = null)
        {
            this.cityName = name;
            this.heuristic = heuristic;
            this.costToGetToFromStart = costToGetTo;
            this.connections = new SortedList<int, City>();

            this.cityObj = Instantiate(CityPrefab, Vector3.zero, CityPrefab.transform.rotation, parent);
            this.cityObj.transform.name = name;
        }

        public void add_Connections(in City c, int cost)
        {
            connections.Add(cost, c);
        }

        public void print_Connections()
        {
            print("City: " + cityName + " Heuristic: " + heuristic.ToString() + " Connections:");
            foreach (KeyValuePair<int, City> c in connections)
                print("\t" + c.Value.cityName + " Cost: " + c.Key);
        }
    }
    
    /// <summary>
    /// Parses an Text File that should be placed within the relase or debug folder
    /// </summary>
    public void parseCityData()
    {
        if (cityPack == null)
            throw new System.Exception("Need to pass a valid TextAsset with The city information and connection data, Got NULL!!!");

        string[] lines = cityPack.text.Split('\n');
        if (lines.Length <= 0)
            throw new System.Exception("File: " + cityPack.name + " is empty!!");

        for (int i = 0; i < lines.Length; i++)
        {
            if (lines[i].StartsWith("#") || lines[i].StartsWith("//")) continue;             // File Comments
            else if (lines[i].StartsWith("Citys"))
            {
                i++;
                while (!lines[i].StartsWith("}"))
                {
                    string[] CityPack = lines[i].Trim().Split(':');
                    if (CityPack.Length < 2)
                        throw new System.Exception("Bad City Format Exception!!\n\tLine " + i.ToString() + " Got: " + CityPack + " Should Be in the format 'CITY:HEURISTIC'");
                    map.Add(CityPack[0], new City(CityPack[0], System.Int32.Parse(CityPack[1]), System.Int32.MaxValue, this.transform));
                    i++;
                }
            }
            else if (lines[i].StartsWith("Start"))
            {
                string s = lines[i].Trim().Split(':')[1];
                if (s.Length == 0)
                    throw new System.Exception("Bad Start City Exception!!\n\tLine " + i.ToString() + " Need to specify a start city Got: " + lines[i]);
                if (!map.TryGetValue(s, out start))
                {
                    debugPrintConnections();
                    throw new System.Exception("Bad Start City Exception!!\n\tLine " + i.ToString() + " Need to give a valid city, Got: " + s);
                }
                else
                    start.costToGetToFromStart = 0;  //Cost to get to start from start is 0
            }
            else if (lines[i].StartsWith("End"))
            {
                string e = lines[i].Trim().Split(':')[1];
                if (e.Length == 0)
                    throw new System.Exception("Bad End City Exception!!\n\tLine " + i.ToString() + " Need to specify a End city Got: " + lines[i]);
                if (!map.TryGetValue(e, out end))
                {
                    debugPrintConnections();
                    throw new System.Exception("Bad End City Exception!!\n\tLine " + i.ToString() + " Need to give a valid city, Got: " + e);
                }
            }
            else if (lines[i].StartsWith("Connections"))
            {
                i++;
                City city0, city1;
                while (!lines[i].StartsWith("}"))
                {
                    string[] relation = lines[i].Trim().Split(':');
                    if (relation.Length < 3)
                        throw new System.Exception("Bad connection between cities Exception!! in: " + cityPack.name + "\n\tError at line " + i.ToString() + " Got: " + lines[i] + " Expected: CITY:CITY:COST");

                    int cost = System.Int32.Parse(relation[2]);
                    if (!map.TryGetValue(relation[0], out city0))
                        throw new System.Exception("Bad connection between cities Exception!!\n\tError at line " + i.ToString() + " Got: " + lines[i] + " Got Invalid City Name: " + relation[0]);
                    if (!map.TryGetValue(relation[1], out city1))
                        throw new System.Exception("Bad connection between cities Exception!!\n\tError at line " + i.ToString() + " Got: " + lines[i] + " Got Invalid City Name: " + relation[1]);

                    city0.add_Connections(in city1, cost);
                    city1.add_Connections(in city0, cost);
                    i++;
                }
            }
            else if (lines[i].StartsWith("Positions"))
            {
                i++;
                while (!lines[i].StartsWith("}"))
                {
                    string[] CityPack = lines[i].Trim().Split(':');
                    if (CityPack.Length < 4)
                        throw new System.Exception("Bad City Format Exception!!\n\tLine " + i.ToString() + " Got: " + CityPack + " Should Be in the format 'CITY:X:Y:Z'");
                    float x = float.Parse(CityPack[1]);
                    float y = float.Parse(CityPack[2]) + (CityPrefab.transform.localScale.y / 2);
                    float z = float.Parse(CityPack[3]);

                    map[CityPack[0]].cityObj.transform.position = new Vector3(x, y, z) * PositionScaler;
                    i++;
                }
            }
        }
    }

    public void debugPrintConnections()
    {
        foreach (KeyValuePair<string, City> city in map)
        {
            city.Value.print_Connections();
        }
    }
}
