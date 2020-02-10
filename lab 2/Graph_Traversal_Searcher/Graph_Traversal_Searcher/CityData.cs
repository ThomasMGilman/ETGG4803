using System;
using System.Collections.Generic;
using System.Text;

namespace Graph_Traversal_Searcher
{
    class CityData
    {
        public string dataPack = "# Citys provided by\n" +
"Citys:{\n" +
    "Arad:366\n" +
    "Bucharest:0\n" +
    "Craiova:160\n" +
    "Drobeta:242\n" +
    "Elforie:161\n" +
    "Fagaras:176\n" +
    "Giurgiu:77\n" +
    "Hirsova:151\n" +
    "Iasi:226\n" +
    "Lugoj:244\n" +
    "Mehadia:241\n" +
    "Neamt:234\n" +
    "Oradea:380\n" +
    "Pitesti:100\n" +
    "Rimnicu_Vilcea:193\n" +
    "Sibiu:253\n" +
    "Timisoara:329\n" +
    "Urziceni:80\n" +
    "Vaslui:199\n" +
    "Zerind:374\n" +
"}\n" +
    "Start:Arad\n" +
    "End:Bucharest\n" +
"# City From : City To : Cost to travel between Citys\n" +
"Connections:{\n" +
    "Oradea:Zerind:71\n" +
    "Oradea:Sibiu:151\n" +
    "Zerind:Arad:75\n" +
    "Arad:Sibiu:140\n" +
    "Arad:Timisoara:118\n" +
    "Timisoara:Lugoj:111\n" +
    "Lugoj:Mehadia:70\n" +
    "Mehadia:Drobeta:75\n" +
    "Drobeta:Craiova:120\n" +
    "Sibiu:Rimnicu_Vilcea:80\n" +
    "Sibiu:Fagaras:99\n" +
    "Rimnicu_Vilcea:Craiova:146\n" +
    "Rimnicu_Vilcea:Pitesti:97\n" +
    "Craiova:Pitesti:138\n" +
    "Fagaras:Bucharest:211\n" +
    "Pitesti:Bucharest:101\n" +
    "Bucharest:Giurgiu:90\n" +
    "Bucharest:Urziceni:85\n" +
    "Urziceni:Hirsova:98\n" +
    "Hirsova:Elforie:86\n" +
    "Urziceni:Vaslui:142\n" +
    "Vaslui:Iasi:92\n" +
    "Iasi:Neamt:87\n" +
"}\n";

        public CityData() { }
    }
}
