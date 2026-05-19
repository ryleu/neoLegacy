#pragma once

#include "Layer.h"

class SmoothZoomLayer : public Layer
{
public:
	public:
    SmoothZoomLayer(int32_t seed, int64_t seedMixup, std::shared_ptr<Layer> parent);

	virtual intArray getArea(int xo, int yo, int w, int h);
	static shared_ptr<Layer>zoom(int32_t seed, int64_t seedMixup, shared_ptr<Layer> sup, int count);
};