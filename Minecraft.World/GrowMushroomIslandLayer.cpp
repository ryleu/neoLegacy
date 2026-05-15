#include "stdafx.h"
#include "net.minecraft.world.level.newbiome.layer.h"
#include "net.minecraft.world.level.biome.h"


GrowMushroomIslandLayer::GrowMushroomIslandLayer(int64_t seed, shared_ptr<Layer> parent, int64_t seedMixup) : Layer(seedMixup)
{
	this->parent = parent;
}

intArray GrowMushroomIslandLayer::getArea(int xo, int yo, int w, int h)
{
    int px = xo - 1;
    int py = yo - 1;
    int pw = w + 2;
    int ph = h + 2;
    intArray p = parent->getArea(px, py, pw, ph);
    intArray result = IntCache::allocate(w * h);
    int mushroomId = Biome::mushroomIsland->id; 
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            
            int ul = p[(x + 0) + (y + 0) * pw];
            int ur = p[(x + 2) + (y + 0) * pw];
            int dl = p[(x + 0) + (y + 2) * pw];
            int dr = p[(x + 2) + (y + 2) * pw];
            int c  = p[(x + 1) + (y + 1) * pw];
            
            int north = p[(x + 1) + (y + 0) * pw];
            int south = p[(x + 1) + (y + 2) * pw];
            int west  = p[(x + 0) + (y + 1) * pw];
            int east  = p[(x + 2) + (y + 1) * pw];

            if (ul == mushroomId || ur == mushroomId ||
                dl == mushroomId || dr == mushroomId ||
                north == mushroomId || south == mushroomId ||
                west == mushroomId || east == mushroomId)
            {
                result[x + y * w] = mushroomId;
            }
            else
            {
                result[x + y * w] = c;
            }
        }
    }
    return result;
}