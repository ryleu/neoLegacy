#pragma once

#include "Layer.h"

class RegionHillsLayer : public Layer
{
private:
    shared_ptr<Layer> riverNoise;

public:
   
    RegionHillsLayer(int32_t seed, int64_t seedMixup, shared_ptr<Layer> parent, shared_ptr<Layer> riverNoise);

    virtual void init(int64_t seed) override;
    virtual intArray getArea(int xo, int yo, int w, int h) override;
};