#pragma once

#include "ZoomLayer.h"

class FuzzyZoomLayer : public ZoomLayer
{
public:
	FuzzyZoomLayer(int32_t seed, int64_t seedMixup, shared_ptr<Layer> parent);
    intArray getArea(int xo, int yo, int w, int h);

protected:
	int random(int a, int b);
    int random(int a, int b, int c, int d);

public:
	static std::shared_ptr<Layer> zoom(int32_t seed, int64_t seedMixup, std::shared_ptr<Layer> sup, int count);
};