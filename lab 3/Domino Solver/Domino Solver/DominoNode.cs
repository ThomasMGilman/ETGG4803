using System;
using System.Collections.Generic;
using System.Text;

class DominoNode
{
    string pip;
    string orientation;
    Tuple<int, int> placeInMaze;
    List<DominoNode> connections;
    float heuristic = 0;

    public DominoNode(string pip, string orientation, Tuple<int, int> mazePlacement, float heuristic = 0)
    {
        this.pip            = pip;
        this.orientation    = orientation;
        this.heuristic      = heuristic;
        this.placeInMaze    = mazePlacement;
        connections         = new List<DominoNode>();
    }

    public void setConnection(ref DominoNode node)
    {
        connections.Add(node);
    }

    public string getPip() { return this.pip; }

    public string getOrientation() { return this.orientation; }

    public Tuple<int, int> getPlaceInMaze() { return this.placeInMaze; }

    public float getHeuristic() { return this.heuristic; }

    public bool isVisited(Tuple<int, int> otherLocation) { return this.placeInMaze == otherLocation; }
}
