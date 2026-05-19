#include "stdafx.h"
#include "net.minecraft.world.level.biome.h"
#include "net.minecraft.world.level.newbiome.layer.h"

RiverLayer::RiverLayer(int32_t seed, int64_t seedMixup, shared_ptr<Layer> parent) : Layer(seed,seedMixup)
{
	this->parent = parent;
}

intArray RiverLayer::getArea(int xo, int yo, int w, int h)
{
    int px = xo - 1;
    int py = yo - 1;
    int pw = w + 2;
    int ph = h + 2;
    intArray p = parent->getArea(px, py, pw, ph);
    intArray result = IntCache::allocate(w * h);
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int c = p[(x + 1) + (y + 1) * pw];
            int n = p[(x + 1) + (y + 0) * pw];
            int e = p[(x + 2) + (y + 1) * pw];
            int s = p[(x + 1) + (y + 2) * pw];
            int w1 = p[(x + 0) + (y + 1) * pw];

            int cc = c >= 2 ? (c & 1) + 2 : c;
            int nc = n >= 2 ? (n & 1) + 2 : n;
            int ec = e >= 2 ? (e & 1) + 2 : e;
            int sc = s >= 2 ? (s & 1) + 2 : s;
            int wc = w1 >= 2 ? (w1 & 1) + 2 : w1;

            if (cc == nc && cc == ec && cc == sc && cc == wc)
                result[x + y * w] = -1;
            else
                result[x + y * w] = Biome::river->id;
        }
    }
    return result;
}