using System;
using System.Collections.Generic;

class Program
{
    static void Main(string[] args)
    {
        FileReader fileReader = new FileReader();//"dominos.txt");
        DominoParser mazeParser = new DominoParser(fileReader.getFileLines());
        List<List<DominoNode>> maze = mazeParser.getDominoMaze();
        Console.WriteLine("Parsed File!");
        DijkstraAStarPathFinder pathFinder = new DijkstraAStarPathFinder(mazeParser.startNode, mazeParser.endNode, true);
        pathFinder.traverse(in maze);
        Console.WriteLine("Path to get from start at: " + mazeParser.start.ToString() + "\n\tto end at: " + mazeParser.end.ToString());
        pathFinder.printPathToNode(mazeParser.end);
    }
}
