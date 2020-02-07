using System;
using System.Collections.Generic;
using System.IO;

namespace Graph_Traversal_Searcher
{
    
    class Program
    {
        /*
         * Citys Provided by Lab 2 pdf according to Graph and Chart
         * Heuristics are provided as well based on straight line distance to Bucharest
         */
        static string CityTextFileName = "connections.txt";
        static bool usingAStar = false;


        static Dictionary<string, City> map;
        static Dictionary<string, Dictionary<int, List<City>>> shortestPathFromStart;
        static City start;
        static City end;

        static void Main(string[] args)
        {
            //Create a empty map and fill it with the Citys provided by the textfile
            map = new Dictionary<string, City>();
            parseCityData();
            traverse();
        }

        static public void traverse()
        {
            SortedList<int, City> toVisit = new SortedList<int, City>();
            SortedList<int, City> visited = new SortedList<int, City>();

            City currentCity = start;
            while(currentCity != end)
            {
                foreach(KeyValuePair<int, City> connection in currentCity.connections)
                {
                }
            }
        }

        /// <summary>
        /// Parses an Text File that should be placed within the relase or debug folder
        /// </summary>
        static public void parseCityData()
        {
            string path = System.IO.Directory.GetCurrentDirectory() + "\\" +CityTextFileName;
            if (!System.IO.File.Exists(path))
                throw new Exception("File Exception!!! File: \n\t" + path + "\nDoesnt Exist!!");

            string[] lines = System.IO.File.ReadAllLines(@path);
            if (lines.Length <= 0)
                throw new Exception("File: " + path + " is empty!!");

            for(int i = 0; i < lines.Length; i++)
            {
                if (lines[i].StartsWith("#") || lines[i].StartsWith("//")) continue;             // File Comments
                else if (lines[i].StartsWith("Citys"))
                {
                    i++;
                    while(!lines[i].StartsWith("}"))
                    {
                        string[] CityPack = lines[i].Trim().Split(":");
                        if (CityPack.Length < 2)
                            throw new Exception("Bad City Format Exception!!\n\tLine " + i.ToString() + " Got: " + CityPack + " Should Be CITY:HEURISTIC");
                        map.Add(CityPack[0], new City(CityPack[0], Int32.Parse(CityPack[1])));
                        i++;
                    }
                }
                else if(lines[i].StartsWith("Start"))
                {
                    string s = lines[i].Trim().Split(":")[1];
                    if (s.Length == 0)
                        throw new Exception("Bad Start City Exception!!\n\tLine " + i.ToString() + " Need to specify a start city Got: " + lines[i]);
                    if (map.ContainsKey(s))
                        start = map[s];
                    else
                    {
                        debugPrintConnections();
                        throw new Exception("Bad Start City Exception!!\n\tLine " + i.ToString() + " Need to give a valid city, Got: " + s);
                    }
                        
                }
                else if(lines[i].StartsWith("End"))
                {
                    string e = lines[i].Trim().Split(":")[1];
                    if (e.Length == 0)
                        throw new Exception("Bad End City Exception!!\n\tLine " + i.ToString() + " Need to specify a End city Got: " + lines[i]);
                    if (!map.TryGetValue(e, out end))
                    {
                        debugPrintConnections();
                        throw new Exception("Bad End City Exception!!\n\tLine " + i.ToString() + " Need to give a valid city, Got: " + e);
                    }
                }
                else if(lines[i].StartsWith("Connections"))
                {
                    i++;
                    City city0, city1;
                    while (!lines[i].StartsWith("}"))
                    {
                        string[] relation = lines[i].Trim().Split(":");
                        if (relation.Length < 3)
                            throw new Exception("Bad connection between cities Exception!! in: "+CityTextFileName+"\n\tError at line "+i.ToString()+" Got: "+lines[i]+" Expected: CITY:CITY:COST");

                        int cost = Int32.Parse(relation[2]);

                        
                        if(!map.TryGetValue(relation[0], out city0))
                            throw new Exception("Bad connection between cities Exception!!\n\tError at line " + i.ToString() + " Got: " + lines[i] + " Got Invalid City Name: "+relation[0]);
                        if(!map.TryGetValue(relation[1], out city1))
                            throw new Exception("Bad connection between cities Exception!!\n\tError at line " + i.ToString() + " Got: " + lines[i] + " Got Invalid City Name: " + relation[1]);

                        city0.add_Connections(in city1, cost);
                        city1.add_Connections(in city0, cost);
                        i++;
                    }
                }
            }
        }

        static public void debugPrintConnections()
        {
            foreach (KeyValuePair<string, City> city in map)
            {
                city.Value.print_Connections();
            }
        }
    }
}
