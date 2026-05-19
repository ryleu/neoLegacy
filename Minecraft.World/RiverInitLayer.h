#pragma once
#include "Layer.h"
#include <memory>

class RiverInitLayer : public Layer
{
public:
	RiverInitLayer(int32_t seed, int64_t seedMixup, shared_ptr<Layer> parent);

	intArray getArea(int xo, int yo, int w, int h);
};