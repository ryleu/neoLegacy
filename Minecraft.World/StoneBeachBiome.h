#pragma once
#include "Biome.h"
class StoneBeachBiome : public Biome
{
public:
    StoneBeachBiome(int id);
    virtual bool isFoggy() { return false; }
    virtual bool isNatural() { return true; }
    virtual int getBaseClass() { return 15; }
};