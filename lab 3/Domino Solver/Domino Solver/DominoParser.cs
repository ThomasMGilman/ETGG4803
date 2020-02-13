using System;
using System.Collections.Generic;
using System.Text;

class DominoParser
{
    public Tuple<int, int> start, end;

    public DominoParser(List<string> dominoMazeFile)
    {
        if (dominoMazeFile.Count == 0 || dominoMazeFile == null)
            throw new Exception("Bad Domino Maze parse call!!! Need to pass a valid DominoMazeFile In the format of List<string>!!! Got empty List!!!");
        
        List<List<string>> dominoMaze = new List<List<string>>();
        for(int r = 0; r < dominoMazeFile.Count; r++)
        {
            if (dominoMazeFile[r].Length > 0)
            {
                if (dominoMazeFile[r].StartsWith('#') || dominoMazeFile[r].StartsWith("//")) continue;      // Ignore comment lines
                if (dominoMazeFile[r].StartsWith("start") || dominoMazeFile[r].StartsWith("end"))
                {
                    string[] tmp = dominoMazeFile[r].Trim().Split("=");
                    tmp = tmp[1].Trim().Split(':');
                    Tuple<int, int> location = new Tuple<int, int>(Int32.Parse(tmp[0]), Int32.Parse(tmp[1]));
                    if (dominoMazeFile[r].StartsWith("end"))
                        this.end = location;
                    else
                        this.start = location;
                }
                if (dominoMazeFile[r].StartsWith("list"))
                {
                    r++;
                    int startOfMaze = r;
                    while (!dominoMazeFile[r].StartsWith("}"))
                    {
                        string[] dominoNodes = dominoMazeFile[r].Trim().Split(',');                         // Split nodes that are in format "pip:Orientation pip:Orientation..."
                        List<string> row = new List<string>();
                        for(int c = 0; c < dominoNodes.Length; c++)
                        {
                            string[] dominoPack = dominoNodes[c].Trim().Split(':');
                            if (dominoPack.Length < 2)
                                throw new Exception("Bad DominoNode!! need to be in format of 'pip:orientation,'\n\tError at line "+r.ToString()+" Column "+c.ToString());


                        }
                        r++;
                    }
                }
            }
        }
        
        int width = dominoMaze[0].Count;
        int height = dominoMaze.Count;
    }
}
