using System;
using System.Collections.Generic;
using System.Text;

class MathFunctions
{
    public float Norm(List<float> values, float pow)
    {
        float val = 0;
        foreach (float n in values)
            val += MathF.Pow(n, pow);
        return MathF.Pow(val, 1 / pow);
    }
}