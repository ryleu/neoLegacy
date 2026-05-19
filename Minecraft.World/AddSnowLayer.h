#pragma once
#include "Layer.h"

class AddSnowLayer : public Layer
{
public:
    AddSnowLayer(int32_t seed, int64_t seedMixup, shared_ptr<Layer> parent);
    virtual intArray getArea(int xo, int yo, int w, int h);
};