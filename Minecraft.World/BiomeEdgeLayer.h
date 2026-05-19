#pragma once
#include "Layer.h"

class BiomeEdgeLayer : public Layer
{
public:
    BiomeEdgeLayer(int32_t seed, int64_t seedMixup, shared_ptr<Layer> parent);
    virtual ~BiomeEdgeLayer() {}
    virtual intArray getArea(int xo, int yo, int w, int h) override;

private:
    static bool isValidTemperatureEdge(int a1biome, int a2biome);
    bool checkEdge(intArray& b, intArray& result, int x, int y, int w, int biome, int target, int replacement);
    bool checkEdgeStrict(intArray& srcArray, intArray& destArray, int x, int z, int width, int centerBiome, unsigned int targetBiome, int replacementBiome);
};