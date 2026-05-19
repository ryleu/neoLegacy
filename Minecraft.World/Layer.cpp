#include "stdafx.h"
#include "net.minecraft.world.level.newbiome.layer.h"
#include "RiverInitLayer.h"
#include "RareBiomeSpotLayer.h"
#include "net.minecraft.world.level.h"
#include "BiomeOverrideLayer.h"
#include "CustomizableSourceSettings.h"

#ifdef __PSVITA__
#include "../Minecraft.Client/PSVita/PSVitaExtras/libdivide.h"
#include <ImposeContinentsLayer.h>

libdivide::divider<long long> fast_d2(2);
libdivide::divider<long long> fast_d3(3);
libdivide::divider<long long> fast_d4(4);
libdivide::divider<long long> fast_d5(5);
libdivide::divider<long long> fast_d6(6);
libdivide::divider<long long> fast_d7(7);
libdivide::divider<long long> fast_d10(10);
#endif
LayerArray Layer::getDefaultLayers(int64_t seed, LevelType* levelType, void* superflatConfig)
{
    int32_t seed32 = (int32_t)seed;


    shared_ptr<Layer> islandLayer = make_shared<IslandLayer>(seed32, 1LL);
    islandLayer = make_shared<FuzzyZoomLayer>(seed32, 0x7D0LL, islandLayer);
    islandLayer = make_shared<AddIslandLayer>(seed32, 1LL, islandLayer);
    islandLayer = make_shared<ZoomLayer>(seed32, 0x7D1LL, islandLayer);
    islandLayer = make_shared<AddIslandLayer>(seed32, 2LL, islandLayer);
    islandLayer = make_shared<AddIslandLayer>(seed32, 0x32LL, islandLayer);
    islandLayer = make_shared<AddIslandLayer>(seed32, 0x46LL, islandLayer);
    islandLayer = make_shared<RemoveTooMuchOceanLayer>(seed32, 2LL, islandLayer);
    islandLayer = make_shared<AddSnowLayer>(seed32, 2LL, islandLayer);
    islandLayer = make_shared<AddIslandLayer>(seed32, 3LL, islandLayer);
    

    islandLayer = make_shared<AddEdgeLayer>(seed32, 2LL, islandLayer, 0);
    islandLayer = make_shared<AddEdgeLayer>(seed32, 2LL, islandLayer, 1);
    islandLayer = make_shared<AddEdgeLayer>(seed32, 3LL, islandLayer, 2);
    
    islandLayer = make_shared<ZoomLayer>(seed32, 0x7D2LL, islandLayer);
    islandLayer = make_shared<ZoomLayer>(seed32, 0x7D3LL, islandLayer);
    islandLayer = make_shared<AddIslandLayer>(seed32, 4LL, islandLayer);
    islandLayer = make_shared<DeepOceanLayer>(seed32, 4LL, islandLayer);


    shared_ptr<Layer> baseLayer = ZoomLayer::zoom(seed32, 0x3E8LL, islandLayer, 0);

    int zoomLevel = 4;
    int riverZoomCount = 4;

    if (levelType == LevelType::lvl_customized && superflatConfig != nullptr)
    {
        auto settings = CustomizableSourceSettings::Builder::build(
            CustomizableSourceSettings::Builder::fromString(superflatConfig));
        zoomLevel = settings->getBiomeSize();
        riverZoomCount = settings->getRiverSize();
    }
    if (levelType == LevelType::lvl_largeBiomes)
        zoomLevel = 6;


    shared_ptr<Layer> riverInit = make_shared<RiverInitLayer>(seed32, 0x64LL, baseLayer);
    shared_ptr<Layer> hillsNoise = ZoomLayer::zoom(seed32, 0x3E8LL, riverInit, 2);

    shared_ptr<Layer> riverLayerFinal = ZoomLayer::zoom(seed32, 0x3E8LL, riverInit, 2);
    riverLayerFinal = ZoomLayer::zoom(seed32, 0x3E8LL, riverLayerFinal, riverZoomCount);
    riverLayerFinal = make_shared<RiverLayer>(seed32, 1LL, riverLayerFinal);
    riverLayerFinal = make_shared<SmoothLayer>(seed32, 0x3E8LL, riverLayerFinal);


    shared_ptr<Layer> biomeLayer = make_shared<BiomeInitLayer>(seed32, 0xC8LL, baseLayer, levelType, superflatConfig);

    {
        auto testBiome = make_shared<BiomeInitLayer>(seed32, 0xC8LL, baseLayer, levelType, superflatConfig);
        testBiome->init((uint32_t)seed);
        IntCache::releaseAll();
        intArray testArea = testBiome->getArea(0, 0, 20, 20);
        app.DebugPrintf("BiomeInitLayer only seed=%d:\n", seed32);
        for (int z = 0; z < 20; z++) {
            for (int x = 0; x < 20; x++)
                app.DebugPrintf("%3d ", testArea[x + z*20]);
            app.DebugPrintf("\n");
        }
        IntCache::releaseAll();
    }

    biomeLayer = make_shared<BiomeEdgeLayer>(seed32, 0x3E8LL, biomeLayer);
    

    biomeLayer = make_shared<RegionHillsLayer>(seed32, 0x3E8LL, biomeLayer, hillsNoise);
    
    {
        auto testLayer = biomeLayer;
        testLayer->init((uint32_t)seed);
        IntCache::releaseAll();
        intArray testArea = testLayer->getArea(0, 0, 20, 20);
        app.DebugPrintf("After RegionHillsLayer seed=%d:\n", seed32);
        for (int z = 0; z < 20; z++) {
            for (int x = 0; x < 20; x++)
                app.DebugPrintf("%3d ", testArea[x + z*20]);
            app.DebugPrintf("\n");
        }
        IntCache::releaseAll();
    }

    biomeLayer = make_shared<RareBiomeSpotLayer>(seed32, 0x3E9LL, biomeLayer);


    for (int i = 0; i < zoomLevel; ++i)
    {
      
        biomeLayer = make_shared<ZoomLayer>(seed32, (int64_t)(0x3E8 + i), biomeLayer);
        if (i == 0)
        {
            biomeLayer = make_shared<AddIslandLayer>(seed32, 3LL, biomeLayer);
            biomeLayer = make_shared<AddMushroomIslandLayer>(seed32, 5LL, biomeLayer);
        }
        if (zoomLevel == 1 || i == 1)
        {
            biomeLayer = make_shared<GrowMushroomIslandLayer>(seed32, 5LL, biomeLayer);
            biomeLayer = make_shared<ShoreLayer>(seed32, 0x3E8LL, biomeLayer);
        }
    }

    biomeLayer = make_shared<SmoothLayer>(seed32, 0x3E8LL, biomeLayer);
    
    {
        auto testLayer = biomeLayer;
        testLayer->init((uint32_t)seed);
        IntCache::releaseAll();
        intArray testArea = testLayer->getArea(60, 240, 20, 20);
        app.DebugPrintf("After zoom loop seed=%d:\n", seed32);
        for (int z = 0; z < 20; z++) {
            for (int x = 0; x < 20; x++)
                app.DebugPrintf("%3d ", testArea[x + z*20]);
            app.DebugPrintf("\n");
        }
        IntCache::releaseAll();
    }
    

    shared_ptr<Layer> mixed = make_shared<RiverMixerLayer>(seed32, 0x64LL, biomeLayer, riverLayerFinal);
    shared_ptr<Layer> voronoi = make_shared<VoronoiZoom>(seed32, 0xALL, mixed);

    mixed->init((uint32_t)seed);
    voronoi->init((uint32_t)seed);

    IntCache::releaseAll();
    mixed->init((uint32_t)seed);
    intArray testArea = mixed->getArea(0, 200, 60, 60);
    app.DebugPrintf("Biomes 0-60, 200-260 seed=%d:\n", seed32);
    for (int z = 0; z < 60; z++) {
        for (int x = 0; x < 60; x++)
            app.DebugPrintf("%3d ", testArea[x + z*60]);
        app.DebugPrintf("\n");
    }
    IntCache::releaseAll();

    LayerArray result(3, false);
    result[0] = mixed;
    result[1] = voronoi;
    result[2] = mixed;

    return result;
}

Layer::Layer(int32_t seed, int64_t seedMixup)
{

    this->seed = 0;

    this->parent = nullptr;

    this->rval = 0;


    int64_t a = seedMixup * (0x5851F42D4C957F2DLL * seedMixup + 0x14057B7EF767814FLL) + seedMixup;
    int64_t b = (0x5851F42D4C957F2DLL * a + 0x14057B7EF767814FLL) * a + seedMixup;

    this->seedMixup = b * (0x5851F42D4C957F2DLL * b + 0x14057B7EF767814FLL) + seedMixup;
}

void Layer::initRandom(int64_t x, int64_t y)
{

    int64_t xi = (int64_t)(int32_t)x;
    int64_t yi = (int64_t)(int32_t)y;

    int64_t v4 = (0x5851F42D4C957F2DLL * seed + 0x14057B7EF767814FLL) * seed + xi;
    uint32_t v5 = 1284865837u * (uint32_t)(v4 * (1284865837LL * v4 - 144211633LL) + yi) - 144211633u;
    int64_t paired = (int64_t)(((uint64_t)v5 << 32) | (uint64_t)v5);
    int64_t v6 = paired * (v4 * (0x5851F42D4C957F2DLL * v4 + 0x14057B7EF767814FLL) + yi) + xi;
    uint32_t lo = 1284865837u * (uint32_t)v6 - 144211633u;
    int64_t paired2 = (int64_t)(((uint64_t)lo << 32) | (uint64_t)lo);
    rval = paired2 * v6 + yi;

  
}

int Layer::nextRandom(int max) {
    
    int r = (int)((this->rval >> 24) % max);
    if (r < 0) {
        r += max;
    }

    
    this->rval = this->rval * (this->rval * 0x5851F42D4C957F2DLL + 0x14057B7EF767814FLL) + this->seed;

    return r;
}

void Layer::init(int64_t worldSeed)
{
    if (parent != nullptr)
        parent->init(worldSeed);

    int64_t m = this->seedMixup;
    int64_t val = (int64_t)(uint32_t)worldSeed;

    
    uint32_t lo = (uint32_t)val;
    uint64_t paired = ((uint64_t)lo << 32) | (uint64_t)lo;
    val = val * (int64_t)(0x5851F42D4C957F2DULL * paired + 0x14057B7EF767814FULL) + m;

    val = val * (0x5851F42D4C957F2DLL * val + 0x14057B7EF767814FLL) + m;
    val = val * (0x5851F42D4C957F2DLL * val + 0x14057B7EF767814FLL) + m;

    this->seed = val;
    static int count = 0;
    if (count++ < 3)
        app.DebugPrintf("init result: seedMixup=%lld, worldSeed=%lld, seed=%lld\n", 
                        seedMixup, worldSeed, this->seed);
}

bool Layer::isOcean(int biomeId)
{
    return biomeId == Biome::ocean->id ||
           biomeId == Biome::deepOcean->id ||
           biomeId == Biome::frozenOcean->id;
}


bool Layer::isSame(int biomeIdA, int biomeIdB) {
    if (biomeIdA == biomeIdB) {
        return true;
    }
    Biome* biome = Biome::getBiome(biomeIdA);
    Biome* biome2 = Biome::getBiome(biomeIdB);
    if (biome != nullptr && biome2 != nullptr) {
        return biome->isSame(biome2);
    }
    return false;
}


int Layer::random(int i, int j, int k, int l) {
    int r = nextRandom(4);
    if (r == 0) return i;
    if (r == 1) return j;
    if (r == 2) return k;
    return l;
}

int Layer::random(int i, int j) {
    if (nextRandom(2) == 0) {
        return i;
    }
    return j;
}

int Layer::modeOrRandom(int i, int j, int k, int l) {
    if (j == k && k == l) {
        return j;
    }
    if (i == j && j == k) {
        return i;
    }
    if (i == j && j == l) {
        return i;
    }
    if (i == k && k == l) {
        return i;
    }
    if (i == j && k != l) {
        return i;
    }
    if (i == k && j != l) {
        return i;
    }
    if (i == l && j != k) {
        return i;
    }
    if (j == k && i != l) {
        return j;
    }
    if (j == l && i != k) {
        return j;
    }
    if (k == l && i != j) {
        return k;
    }
    
    return random(i, j, k, l);
}