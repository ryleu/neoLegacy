#pragma once

#include "Layer.h"

class ZoomLayer : public Layer
{
public:
	ZoomLayer::ZoomLayer(int32_t seed, int64_t seedMixup, shared_ptr<Layer> parent);
	virtual intArray getArea(int xo, int yo, int w, int h);

protected:
	int random(int a, int b);
	int random(int a, int b, int c, int d);

public:
	static shared_ptr<Layer> zoom(int32_t seed, int64_t seedMixup, shared_ptr<Layer> sup, int count);
};