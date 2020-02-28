using System;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

class DominoParser : MonoBehaviour
{
    public Vector2 start, end;
    public DominoNode startNode, endNode;
    List<List<DominoNode>> dominoMaze;

    public DominoParser(List<string> dominoMazeFile)
    {
        if (dominoMazeFile.Count == 0 || dominoMazeFile == null)
            throw new Exception("Bad Domino Maze parse call!!! Need to pass a valid DominoMazeFile In the format of List<string>!!! Got empty List!!!");
        
        this.dominoMaze = new List<List<DominoNode>>();
        for(int r = 0; r < dominoMazeFile.Count; r++)
        {
            if (dominoMazeFile[r].Length > 0)
            {
                if (dominoMazeFile[r].StartsWith("#") || dominoMazeFile[r].StartsWith("//")) continue;      // Ignore comment lines
                if (dominoMazeFile[r].StartsWith("start") || dominoMazeFile[r].StartsWith("end"))
                {
                    string[] tmp = dominoMazeFile[r].Trim().Split('=');
                    tmp = tmp[1].Trim().Split(':');
                    Tuple<int, int> location = new Tuple<int, int>(Int32.Parse(tmp[0]), Int32.Parse(tmp[1]));
                    if (dominoMazeFile[r].StartsWith("end"))
                        this.end = new Vector2(location.Item1, location.Item2);
                    else
                        this.start = new Vector2(location.Item1, location.Item2);
                }
                if (dominoMazeFile[r].StartsWith("maze"))
                {
                    r++;
                    int startOfMaze = r;
                    while (!dominoMazeFile[r].StartsWith("}"))
                    {
                        string[] dominoNodes = dominoMazeFile[r].Trim().Split(',');                         // Split nodes that are in format "pip:Orientation pip:Orientation..."
                        List<DominoNode> row = new List<DominoNode>();
                        for(int c = 0; c < dominoNodes.Length; c++)
                        {
                            string[] dominoPack = dominoNodes[c].Trim().Split(':');
                            if (dominoPack.Length < 2)
                                throw new Exception("Bad DominoNode!! need to be in format of 'pip:orientation,'\n\tError at line "+r.ToString()+" Column "+c.ToString());

                            int pip = Int32.Parse(dominoPack[0]);
                            row.Add(new DominoNode(pip, dominoPack[1], new Vector2(c, r - startOfMaze), ((end.x + c) + (end.y - (r - startOfMaze))) ));
                        }
                        this.dominoMaze.Add(row);
                        r++;
                    }
                }
            }
        }

        startNode = this.dominoMaze[(int)start.y][(int)start.x];
        endNode = this.dominoMaze[(int)end.y][(int)end.x];

        int width = this.dominoMaze[0].Count;
        int height = dominoMaze.Count;

        // Set DominoNode Connections
        for(int r = 0; r < dominoMaze.Count; r++)
        {
            for(int c = 0; c < dominoMaze[r].Count; c++)
            {
                DominoNode curNode = dominoMaze[r][c];
                if (r > 0 && curNode.isPipEqual(dominoMaze[r - 1][c].getPip()))             //check domino node above
                    curNode.setConnection(dominoMaze[r - 1][c]);
                if (r < height - 1 && curNode.isPipEqual(dominoMaze[r + 1][c].getPip()))    //check domino node bellow
                    curNode.setConnection(dominoMaze[r + 1][c]);
                if (c < width - 1 && curNode.isPipEqual(dominoMaze[r][c+1].getPip()))       //check domino node to the right
                    curNode.setConnection(dominoMaze[r][c + 1]);
                if (c > 0 && curNode.isPipEqual(dominoMaze[r][c-1].getPip()))               //check domino node to the left
                    curNode.setConnection(dominoMaze[r][c - 1]);

                //Handle Orientation
                switch (curNode.getOrientation())
                {
                    case "u":   curNode.setConnection(dominoMaze[r - 1][c]);
                        break;
                    case "d":   curNode.setConnection(dominoMaze[r + 1][c]);
                        break;
                    case "r":   curNode.setConnection(dominoMaze[r][c + 1]);
                        break;
                    case "l":   curNode.setConnection(dominoMaze[r][c - 1]);
                        break;
                    default:
                        throw new Exception("Bad orientation Exception!!!! Domino node needs to be either l,r,u,d instead got: "+curNode.getOrientation()+"\n\tError at Node: "+curNode.getPlaceInMaze().ToString());
                }
            }
        }
    }

    public List<List<DominoNode>> getDominoMaze() { return this.dominoMaze; }
}
