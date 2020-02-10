using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;

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
        static Dictionary<string, Path> shortestPathFromStart;
        static List<string> orderChecked;
        static City start;
        static City end;
        static Stopwatch timer;
        

        static void Main(string[] args)
        {
            //Create a empty map and fill it with the Citys provided by the textfile
            map = new Dictionary<string, City>();
            shortestPathFromStart = new Dictionary<string, Path>();
            orderChecked = new List<string>();
            parseCityData();

            timer = new Stopwatch();
            timer.Start();
            traverse();
            timer.Stop();

            printShortestPathToDestination();
            //printOutPaths();
        }

        static public void printShortestPathToDestination()
        {
            Console.WriteLine("Using " + (usingAStar ? "A*" : "Dijkstras") + " Algorithm");
            Console.WriteLine("Reached Goal in: " + timer.Elapsed.TotalSeconds.ToString() + "Seconds");
            Console.WriteLine("The shortest path To '" + end.cityName + "' From '" + start.cityName + "'\n\tDistance: " + shortestPathFromStart[end.cityName].cost.ToString());
            Console.WriteLine("\tPath: ");
            foreach (City c in shortestPathFromStart[end.cityName].path)
                Console.WriteLine("\t\t" + c.cityName);

            Console.WriteLine("\n\nChecked Cities and their neighbors in the order of:");
            for(int i = 0; i < orderChecked.Count; i++)
                Console.WriteLine("\t"+i.ToString() +"\t"+ orderChecked[i]);
        }

        static public void printOutPaths()
        {
            Console.WriteLine("Paths:");
            foreach(KeyValuePair<string, Path> cityPath in shortestPathFromStart)
            {
                Console.WriteLine("\tTo "+cityPath.Key+":");
                foreach (City c in cityPath.Value.path)
                    Console.WriteLine("\t\t" + c.cityName);
                Console.WriteLine("\tCost: " + cityPath.Value.cost+"\n");
            }
        }

        /// <summary>
        /// Copy path to get to neighbor into newPath and append new end point
        /// </summary>
        /// <param name="neighboringCity"></param>
        /// <param name="newPath"></param>
        /// <param name="end"></param>
        static public void copyPath(in string neighboringCity, out List<City> newPath, in City end)
        {
            newPath = new List<City>();
            foreach(City c in shortestPathFromStart[neighboringCity].path)
                newPath.Add(c);
            newPath.Add(end);
        }

        /// <summary>
        /// Goes through each Citys connections from the start all the way to the finish and finds the shortest path to the end city
        /// </summary>
        static public void traverse()
        {
            HashSet<City> Visited = new HashSet<City>();
            SortedList<int, City> toVisit = new SortedList<int, City>();
            //Queue<City> toVisit = new Queue<City>();
            toVisit.Add(0, start); //.Enqueue(start);

            shortestPathFromStart.Add(start.cityName, new Path() { cost = start.costToGetToFromStart , path = new List<City> {start} });

            while(toVisit.Count > 0)
            {
                City currentCity = toVisit[toVisit.Keys[0]]; //toVisit.Dequeue();
                orderChecked.Add(currentCity.cityName);
                Visited.Add(currentCity);

                if (currentCity == end)
                    break;

                foreach (KeyValuePair<int, City> connection in currentCity.connections)
                {
                    City neighboringCity = connection.Value;
                    int costToGetTo = shortestPathFromStart[currentCity.cityName].cost + connection.Key;

                    // Neightbor already visited before, 
                    // check if path to it from this city is cheaper then current path
                    if (Visited.Contains(neighboringCity) || shortestPathFromStart.ContainsKey(neighboringCity.cityName))       
                    {
                        // New Path is cheaper to this city
                        if(shortestPathFromStart[neighboringCity.cityName].cost  > costToGetTo)
                        {
                            List<City> pathToCity;
                            copyPath(in currentCity.cityName, out pathToCity, in neighboringCity);
                            shortestPathFromStart[neighboringCity.cityName] = new Path() { cost = costToGetTo, path = pathToCity };
                        }
                    }
                    // Otherwise add city to toVisit to have its neighbors checked
                    else
                    {
                        toVisit.Add(costToGetTo + (usingAStar ? connection.Value.heuristic : 0), neighboringCity);//.Enqueue(neighboringCity);
                        List<City> pathToCity;
                        copyPath(in currentCity.cityName, out pathToCity, in neighboringCity);
                        if(shortestPathFromStart.ContainsKey(neighboringCity.cityName))
                        {
                            printOutPaths();
                            throw new Exception("Trying to add CityPath that is already added!!");
                        }
                        else
                            shortestPathFromStart.Add(neighboringCity.cityName, new Path() { cost = costToGetTo, path = pathToCity});
                    }
                }
                toVisit.RemoveAt(0);
            }
        }

        /// <summary>
        /// Parses an Text File that should be placed within the relase or debug folder
        /// </summary>
        static public void parseCityData(string fileName = null)
        {
            string[] lines;
            if (fileName != null)
            {
                string path = System.IO.Directory.GetCurrentDirectory() + "\\" + fileName;
                if (!System.IO.File.Exists(path))
                    throw new Exception("File Exception!!! File: \n\t" + path + "\nDoesnt Exist!!");

                lines = System.IO.File.ReadAllLines(@path);
                if (lines.Length <= 0)
                    throw new Exception("File: " + path + " is empty!!");
            }
            else
            {
                CityData mapData = new CityData();
                lines = mapData.dataPack.Split('\n');
            }

            for(int i = 0; i < lines.Length; i++)
            {
                lines[i] = lines[i].Trim();
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
                    if (!map.TryGetValue(s, out start))
                    {
                        debugPrintConnections();
                        throw new Exception("Bad Start City Exception!!\n\tLine " + i.ToString() + " Need to give a valid city, Got: " + s);
                    }
                    else
                        start.costToGetToFromStart = 0;  //Cost to get to start from start is 0
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
                            throw new Exception("Bad connection between cities Exception!! in: "+ fileName + "\n\tError at line "+i.ToString()+" Got: "+lines[i]+" Expected: CITY:CITY:COST");

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
