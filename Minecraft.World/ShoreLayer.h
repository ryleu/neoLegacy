#pragma once
#include "Layer.h"

class ShoreLayer : public Layer
{
public:
    ShoreLayer(int64_t seed, shared_ptr<Layer> parent, int64_t seedMixup);
    virtual intArray getArea(int xo, int yo, int w, int h);
private:
    void replaceIfNeighborOcean(intArray& b, intArray& result, int x, int y, int w, int old, int replacement);
};