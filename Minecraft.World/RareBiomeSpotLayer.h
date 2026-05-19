#pragma once
#include "Layer.h"
#include <memory>

class RareBiomeSpotLayer : public Layer
{
public:
	RareBiomeSpotLayer(int32_t seed, int64_t seedMixup, shared_ptr<Layer> parent);
	virtual intArray getArea(int xo, int yo, int w, int h) override;
};
