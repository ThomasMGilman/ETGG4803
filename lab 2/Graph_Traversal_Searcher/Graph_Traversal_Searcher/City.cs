using System;
using System.Collections.Generic;
using System.Text;

namespace Graph_Traversal_Searcher
{
    class City
    {
        string cityName;
        List<connection> connections;

        public struct connection{
            public City otherCity;
            public int costOfTravel;

            public static implicit operator connection(Tuple<City, int> path)
            {
                return new connection() { otherCity = path.Item1, costOfTravel = path.Item2};
            }
        };

        City(string name)
        {
            cityName = name;
        }

        void set_Connections(in List<Tuple<City, int>> connections)
        {
            foreach(Tuple<City, int> path in connections){
                connection c = path;
                this.connections.Add(c);
            }
        }
    }
}
