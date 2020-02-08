using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CityCreator : MonoBehaviour
{
    public GameObject CityPrefab;
    public Dictionary<string, City> map;
    public Dictionary<string, Path> shortestPathFromStart;
    private City start, end;

    public string CityTextFileName = "connections";
    public bool usingAStar = false;

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

        public City(string name, in GameObject prefab, int heuristic = 0, int costToGetTo = System.Int32.MaxValue)
        {
            this.cityName = name;
            this.heuristic = heuristic;
            this.costToGetToFromStart = costToGetTo;
            this.connections = new SortedList<int, City>();
            this.cityObj = Instantiate(prefab, Vector3.zero, prefab.transform.rotation);
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

    // Start is called before the first frame update
    void Start()
    {
        //Create a empty map and fill it with the Citys provided by the textfile
        map = new Dictionary<string, City>();
        shortestPathFromStart = new Dictionary<string, Path>();

        parseCityData();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    /// <summary>
    /// Parses an Text File that should be placed within the relase or debug folder
    /// </summary>
    public void parseCityData()
    {
        string[] lines = Resources.Load<TextAsset>(CityTextFileName).text.Split('\n');
        if (lines.Length <= 0)
            throw new System.Exception("File: " + CityTextFileName + " is empty!!");

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
                        throw new System.Exception("Bad City Format Exception!!\n\tLine " + i.ToString() + " Got: " + CityPack + " Should Be CITY:HEURISTIC");
                    map.Add(CityPack[0], new City(CityPack[0], in CityPrefab, System.Int32.Parse(CityPack[1])));
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
                        throw new System.Exception("Bad connection between cities Exception!! in: " + CityTextFileName + "\n\tError at line " + i.ToString() + " Got: " + lines[i] + " Expected: CITY:CITY:COST");

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
