using System;
using System.Collections.Generic;
using System.Text;

namespace Graph_Traversal_Searcher
{
    class City
    {
        public string cityName;
        public SortedList<int, City> connections;
        public int heuristic, costToGetTo;

        public struct connection{
            public City otherCity;
            public int costOfTravel;
        };

        public City(string name, int heuristic = 0, int costToGetTo = Int32.MaxValue)
        {
            this.cityName = name;
            this.heuristic = heuristic;
            this.costToGetTo = costToGetTo;
            this.connections = new SortedList<int, City>();
        }

        public void add_Connections(in City c, int cost)
        {
            connections.Add(cost,  c);
        }

        public void print_Connections()
        {
            Console.WriteLine("City: " + cityName + " Heuristic: " + heuristic.ToString()+" Connections:");
            foreach(KeyValuePair<int, City> c in connections)
                Console.WriteLine("\t"+c.Value.cityName + " Cost: " + c.Key);
        }
    }
}
