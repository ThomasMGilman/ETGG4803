using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Text;
using UnityEngine;

public class DominoNode : MonoBehaviour, IEquatable<DominoNode>, IComparable<DominoNode>
{
    public GameObject DominoPrefab;
    public int costToGetToFromStart = Int32.MaxValue;
    public List<DominoNode> connections;
    public int cost = 0;

    public float spacing = 0.2f;

    public GameObject DominoPiece;

    int pip;
    string orientation;
    Vector2 placeInMaze;
    float heuristic = 0;

    public DominoNode(int pip, string orientation, Vector2 mazePlacement, float heuristic = 0)
    {
        this.pip            = pip;
        this.orientation    = orientation;
        this.heuristic      = heuristic;
        this.placeInMaze    = mazePlacement;
        connections         = new List<DominoNode>();
        if(DominoPrefab == null)
            DominoPrefab = Resources.Load<GameObject>("Prefabs/DominoNode");

        float zPos = 0 + mazePlacement.x * DominoPrefab.transform.localScale.x + mazePlacement.x * spacing;
        float yPos = 0 + DominoPrefab.transform.localScale.y / 2;
        float xPos = 0 + mazePlacement.y * DominoPrefab.transform.localScale.z + mazePlacement.y * spacing;
        Vector3 pos = new Vector3(xPos, yPos, zPos);

        Quaternion rot = DominoPrefab.transform.rotation;
        if (this.orientation == "d")
            rot.eulerAngles = new Vector3(0, -90, 0);
        else if (this.orientation == "u")
            rot.eulerAngles = new Vector3(0, 90, 0);
        else if (this.orientation == "r")
            rot.eulerAngles = new Vector3(0, 180, 0);

        DominoPiece = Instantiate(DominoPrefab, pos, rot);
        DominoPiece.name = "DominoNode [" + mazePlacement.x.ToString() + "," + mazePlacement.y.ToString() + "]";
        for (int c = 0; c < DominoPiece.transform.childCount-1; c++)
            DominoPiece.transform.GetChild(c).gameObject.SetActive((c == pip - 1));
    }

    public void setConnection(in DominoNode node)
    {
        connections.Add(node);
    }

    public int getPip() { return this.pip; }

    public bool isPipEqual(int otherPip) { return this.pip == otherPip; }

    public string getOrientation() { return this.orientation; }

    public Vector2 getPlaceInMaze() { return this.placeInMaze; }

    public float getHeuristic() { return this.heuristic; }

    public void setHeuristic(float newHeuristic) { this.heuristic = newHeuristic; }

    public bool isVisited(Vector2 otherLocation) { return this.placeInMaze == otherLocation; }

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
