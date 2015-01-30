using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Doungen;

namespace DoungenWrapper
{
    class Program
    {
        static void Main(string[] args)
        {
            ManagedMap map = new ManagedMap(75, 75);
            MapModifier modifier = new MapModifier();
            modifier.seed = 152;
            modifier.roomAttempts = 50;
            modifier.shrinkFactor = 1f;
            map.generate(modifier);
            bool[,] tiles = map.getMap();
            for (int i = 0; i < tiles.GetLength(1); i++)
            {
                for (int j = 0; j < tiles.GetLength(0); j++)
                {
                    Console.Write(tiles[j, i] ? " " : "+");
                }
                Console.WriteLine();
            }
        }
    }
}
