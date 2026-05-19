#pragma once

#include "Layer.h"

class RiverLayer : public Layer
{
public:
	RiverLayer(int32_t seed, int64_t seedMixup, shared_ptr<Layer> parent);
	intArray getArea(int xo, int yo, int w, int h);
};