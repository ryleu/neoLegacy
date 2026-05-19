#pragma once

#include "Layer.h"

class LevelType;

class BiomeOverrideLayer : public Layer
{
private:
	static const unsigned int width = 216;
	static const unsigned int height = 216;

	byteArray m_biomeOverride;

public:
	BiomeOverrideLayer(int32_t seed, int64_t seedMixup);
    intArray getArea(int xo, int yo, int w, int h);
};