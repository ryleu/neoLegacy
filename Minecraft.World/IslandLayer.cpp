#include "stdafx.h"
#include "net.minecraft.world.level.newbiome.layer.h"

IslandLayer::IslandLayer(int32_t seed, int64_t seedMixup) : Layer(seed, seedMixup)
{
}

intArray IslandLayer::getArea(int xo, int yo, int w, int h)
{
    intArray result = IntCache::allocate(w * h);
    static int callCount = 0;
    bool doLog = (callCount++ == 0);
    
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            initRandom(xo + x, yo + y);
            int r = nextRandom(10);
            int val = (r == 0) ? 1 : 0;
            result[x + y * w] = val;
            
           
        }
    }
    if (xo > -w && xo <= 0 && yo > -h && yo <= 0)
        result[-xo + -yo * w] = 1;
    return result;
}