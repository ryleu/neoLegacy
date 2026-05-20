#pragma once
#include "Layer.h"

class AddEdgeLayer : public Layer
{

    
private:
   
    int mode;
    intArray coolWarm(int xo, int yo, int w, int h);
    intArray heatIce(int xo, int yo, int w, int h);
    intArray introduceSpecial(int xo, int yo, int w, int h);


public:
    enum LayerMode {
        MODE_COOL_WARM = 0,
        MODE_HEAT_ICE = 1,
        MODE_SPECIAL = 2
    };
    AddEdgeLayer(int32_t seed,  int64_t seedMixup, std::shared_ptr<Layer> parent,int mode);
    virtual ~AddEdgeLayer() {}
    intArray getArea(int xo, int yo, int w, int h) override;
};