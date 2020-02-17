using System;
using System.Collections.Generic;

class MainClass {
  public static void Main (string[] args) {
      FileReader fileReader = new FileReader();//"dominos.txt");
      DominoParser mazeParser = new DominoParser(fileReader.getFileLines());
      List<List<DominoNode>> maze = mazeParser.getDominoMaze();
      Console.WriteLine("Parsed File!");
      DijkstraAStarPathFinder pathFinder = new DijkstraAStarPathFinder(mazeParser.startNode,mazeParser.endNode, true);
      pathFinder.traverse(maze);
      pathFinder.printPathToNode(mazeParser.end);
  }
}