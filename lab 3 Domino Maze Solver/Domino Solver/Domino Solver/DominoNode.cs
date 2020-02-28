using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Text;

class DominoNode : IEquatable<DominoNode>, IComparable<DominoNode>
{
    public int costToGetToFromStart = Int32.MaxValue;
    public List<DominoNode> connections;
    public int cost = 0;

    int pip;
    string orientation;
    Tuple<int, int> placeInMaze;
    float heuristic = 0;

    public DominoNode(int pip, string orientation, Tuple<int, int> mazePlacement, float heuristic = 0)
    {
        this.pip            = pip;
        this.orientation    = orientation;
        this.heuristic      = heuristic;
        this.placeInMaze    = mazePlacement;
        connections         = new List<DominoNode>();
    }

    public void setConnection(in DominoNode node)
    {
        connections.Add(node);
    }

    public int getPip() { return this.pip; }

    public bool isPipEqual(int otherPip) { return this.pip == otherPip; }

    public string getOrientation() { return this.orientation; }

    public Tuple<int, int> getPlaceInMaze() { return this.placeInMaze; }

    public float getHeuristic() { return this.heuristic; }

    public void setHeuristic(float newHeuristic) { this.heuristic = newHeuristic; }

    public bool isVisited(Tuple<int, int> otherLocation) { return this.placeInMaze == otherLocation; }

    public bool Equals(DominoNode other)
    {
        if (other == null) return false;
        return (this.placeInMaze.Equals(other.placeInMaze));
    }

    public int CompareTo(DominoNode other)
    {
        if (other == null)
            return 1;
        else
            return this.cost.CompareTo(other.cost);
    }
}
