#pragma once
#include "Layer.h"

class ShoreLayer : public Layer
{
public:
    ShoreLayer(int32_t seed,int64_t seedMixup, shared_ptr<Layer> parent );
    virtual intArray getArea(int xo, int yo, int w, int h);
private:
    void replaceIfNeighborOcean(intArray& b, intArray& result, int x, int y, int w, int old, int replacement);
};