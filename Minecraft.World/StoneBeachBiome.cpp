#include "stdafx.h"
#include "net.minecraft.world.level.tile.h"
#include "BiomeDecorator.h"
#include "StoneBeachBiome.h"

StoneBeachBiome::StoneBeachBiome(int id) : Biome(id)
{

    topMaterial  = static_cast<byte>(Tile::stone_Id);
    material     = static_cast<byte>(Tile::stone_Id);

    depth  = 1;  
    scale  = 1;    

    decorator->treeCount   = -999;
    decorator->deadBushCount = 0;
    decorator->reedsCount    = 0;
    decorator->cactusCount   = 0;
}