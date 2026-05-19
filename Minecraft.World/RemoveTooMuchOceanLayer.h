#pragma once

#include "Layer.h"

class RemoveTooMuchOceanLayer : public Layer
{
public:
	RemoveTooMuchOceanLayer(int32_t seed, int64_t seedMixup, shared_ptr<Layer> parent);
	virtual ~RemoveTooMuchOceanLayer() {}
	virtual intArray getArea(int xo, int yo, int w, int h) override;
};
