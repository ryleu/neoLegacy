#pragma once

#include "Layer.h"

class DeepOceanLayer : public Layer
{
public:
	DeepOceanLayer(int32_t seed, int64_t seedMixup, shared_ptr<Layer> parent);
	virtual ~DeepOceanLayer() {}
	virtual intArray getArea(int xo, int yo, int w, int h) override;
};
