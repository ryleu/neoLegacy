#pragma once

#include "Layer.h"

class ImposeContinentsLayer : public Layer
{
public:
    ImposeContinentsLayer(int32_t seed, int64_t seedMixup, shared_ptr<Layer> parent);
    virtual ~ImposeContinentsLayer();

    virtual intArray getArea(int xo, int yo, int w, int h) override;
};