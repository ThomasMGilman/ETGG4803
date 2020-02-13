using System;
using System.Collections;

class Program
{
    static double normDist(ref ArrayList list, float pow)
    {
        double dst = 0;
        foreach(double val in list)
            dst += Math.Pow(Math.Abs(val), pow);

        return dst;
    }

    static ArrayList list = new ArrayList() { 2.0, 4.0 };

    static void Main(string[] args)
    {
        Console.WriteLine("Distance: "+normDist(ref list, 1000).ToString());
    }
}