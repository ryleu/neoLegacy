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

    shared_ptr<Layer> islandLayer = make_shared<IslandLayer>(seed32, 1);
    islandLayer = make_shared<FuzzyZoomLayer>(seed32, islandLayer, 0x7D0);
    islandLayer = make_shared<AddIslandLayer>(seed32, islandLayer, 1);
    islandLayer = make_shared<ZoomLayer>(seed32, islandLayer, 0x7D1);
    islandLayer = make_shared<AddIslandLayer>(seed32, islandLayer, 2);
    islandLayer = make_shared<AddIslandLayer>(seed32, islandLayer, 0x32);
    islandLayer = make_shared<AddIslandLayer>(seed32, islandLayer, 0x46);
    islandLayer = make_shared<RemoveTooMuchOceanLayer>(seed32, islandLayer, 2);
    islandLayer = make_shared<AddSnowLayer>(seed32, islandLayer, 2);
    islandLayer = make_shared<AddIslandLayer>(seed32, islandLayer, 3);
    islandLayer = make_shared<AddEdgeLayer>(seed32, islandLayer, 2, 0);
    islandLayer = make_shared<AddEdgeLayer>(seed32, islandLayer, 2, 1);
    islandLayer = make_shared<AddEdgeLayer>(seed32, islandLayer, 3, 2);
    islandLayer = make_shared<ZoomLayer>(seed32, islandLayer, 0x7D2);
    islandLayer = make_shared<ZoomLayer>(seed32, islandLayer, 0x7D3);
    islandLayer = make_shared<AddIslandLayer>(seed32, islandLayer, 4);
    islandLayer = make_shared<DeepOceanLayer>(seed32, islandLayer, 4);

    shared_ptr<Layer> baseLayer = ZoomLayer::zoom(seed32, islandLayer, 0x3E8, 0);

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

    shared_ptr<Layer> riverInit = make_shared<RiverInitLayer>(seed32, baseLayer, 0x64);
    shared_ptr<Layer> hillsNoise = ZoomLayer::zoom(seed32, riverInit, 0x3E8, 2);

    shared_ptr<Layer> riverLayerFinal = ZoomLayer::zoom(seed32, riverInit, 0x3E8, 2);
    riverLayerFinal = ZoomLayer::zoom(seed32, riverLayerFinal, 0x3E8, riverZoomCount);
    riverLayerFinal = make_shared<RiverLayer>(seed32, riverLayerFinal, 1);
    riverLayerFinal = make_shared<SmoothLayer>(seed32, riverLayerFinal, 0x3E8);

    shared_ptr<Layer> biomeLayer = make_shared<BiomeInitLayer>(seed32, baseLayer, 0xC8, levelType, superflatConfig);

    {
    auto testBiome = make_shared<BiomeInitLayer>(seed32, baseLayer, 0xC8, levelType, superflatConfig);
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

    biomeLayer = make_shared<BiomeEdgeLayer>(seed32, biomeLayer, 0x3E8);
    biomeLayer = make_shared<RegionHillsLayer>(seed32, biomeLayer, hillsNoise, 0x3E8);
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

    biomeLayer = make_shared<RareBiomeSpotLayer>(seed32, biomeLayer, 0x3E9);

    for (int i = 0; i < zoomLevel; ++i)
    {
        biomeLayer = make_shared<ZoomLayer>(seed32, biomeLayer, 0x3E8 + i);
        if (i == 0)
        {
            biomeLayer = make_shared<AddIslandLayer>(seed32, biomeLayer, 3);
            biomeLayer = make_shared<AddMushroomIslandLayer>(seed32, biomeLayer, 5);
        }
        if (zoomLevel == 1 || i == 1)
        {
            biomeLayer = make_shared<GrowMushroomIslandLayer>(seed32, biomeLayer, 5);
            biomeLayer = make_shared<ShoreLayer>(seed32, biomeLayer, 0x3E8);
        }
    }

    biomeLayer = make_shared<SmoothLayer>(seed32, biomeLayer, 0x3E8);
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
    shared_ptr<Layer> mixed = make_shared<RiverMixerLayer>(seed32, biomeLayer, riverLayerFinal, 0x64);
    shared_ptr<Layer> voronoi = make_shared<VoronoiZoom>(seed32, mixed, 0xA);

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
Layer::Layer(int64_t s)
{
    int64_t sm = (int64_t)(int32_t)s;
    int64_t a = sm * (0x5851F42D4C957F2DLL * sm + 0x14057B7EF767814FLL) + sm;
    int64_t b = (0x5851F42D4C957F2DLL * a + 0x14057B7EF767814FLL) * a + sm;
    this->seedMixup = b * (0x5851F42D4C957F2DLL * b + 0x14057B7EF767814FLL) + sm;
    this->parent = nullptr;
    this->seed = 0;
    this->rval = 0;
}

void Layer::initRandom(int64_t x, int64_t y)
{

    static bool first = true;
    

    int64_t xi = (int64_t)(int32_t)x;
    int64_t yi = (int64_t)(int32_t)y;

    int64_t v4 = (0x5851F42D4C957F2DLL * seed + 0x14057B7EF767814FLL) * seed + xi;
    uint32_t v5 = 1284865837u * (uint32_t)(v4 * (1284865837LL * (int32_t)v4 - 144211633LL) + yi) - 144211633u;
    int64_t paired = (int64_t)(((uint64_t)v5 << 32) | (uint64_t)v5);
    int64_t v6 = paired * (v4 * (0x5851F42D4C957F2DLL * v4 + 0x14057B7EF767814FLL) + yi) + xi;
    uint32_t lo = 1284865837u * (uint32_t)v6 - 144211633u;
    int64_t paired2 = (int64_t)(((uint64_t)lo << 32) | (uint64_t)lo);
    rval = paired2 * v6 + yi;

  
}

int Layer::nextRandom(int max)
{
    int64_t rval_full = this->rval;  
    uint32_t rval_lo = (uint32_t)rval_full;
    uint32_t rval_hi = (uint32_t)(rval_full >> 32);
    uint32_t seed_lo = (uint32_t)this->seed;

    int result = (int)((int64_t)(rval_full >> 24) % (int64_t)max);
    if (result < 0) result += max;

    uint32_t new_lo = rval_lo * (1284865837u * rval_lo - 144211633u) + seed_lo;

   
    uint64_t paired = ((uint64_t)rval_lo << 32) | (uint64_t)rval_lo;
    uint64_t big = (uint64_t)(int64_t)rval_full * (0x5851F42D4C957F2DULL * paired + 0x14057B7EF767814FULL);
    uint32_t carry = (uint32_t)(((uint64_t)(rval_lo * (1284865837u * rval_lo - 144211633u)) + seed_lo) >> 32);
    uint32_t new_hi = seed_lo + carry + (uint32_t)(big >> 32);  

    this->rval = ((int64_t)new_hi << 32) | (int64_t)new_lo;
    return result;
}

void Layer::init(int64_t worldSeed)
{
    if (parent != nullptr)
        parent->init(worldSeed);

    uint32_t ws = (uint32_t)worldSeed;
    int64_t m   = this->seedMixup;

    uint32_t v12 = ws * (1284865837u * ws - 144211633u);
    int64_t sum1 = (int64_t)(int32_t)v12 + m;           
    uint32_t v14_lo = (uint32_t)sum1 * (1284865837u * (uint32_t)sum1 - 144211633u);
    int64_t sum2 = (int64_t)(int32_t)v14_lo + m;

    this->seed = sum2 * (0x5851F42D4C957F2DLL * sum2 + 0x14057B7EF767814FLL) + m;
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
    } else {
        Biome* biome = Biome::getBiome(biomeIdA);
        Biome* biome2 = Biome::getBiome(biomeIdB);
        if (biome != nullptr && biome2 != nullptr) {
            if (biome != Biome::mesaPlateauF && biome != Biome::mesaPlateau) {
                return biome == biome2 || biome->getBaseBiomeId() == biome2->getBaseBiomeId();
            } else {
                return biome2 == Biome::mesaPlateauF || biome2 == Biome::mesaPlateau;
            }
        } else {
            return false;
        }
    }
}


int Layer::random(int i, int j, int k, int l) {
    int random = nextRandom(4);

    int ret = (random != 2 ? i : k);
    if (random == 3)
        ret = l;
    if (random == 1)
        ret = j;
    return ret;
}

int Layer::random(int i, int j) {
    if (nextRandom(2)) {
        return j;
    }

    return i;
}

int Layer::modeOrRandom(int i, int j, int k, int l) {
    if (j == k && k == l) {
        return j;
    } else if (i == j && i == k) {
        return i;
    } else if (i == j && i == l) {
        return i;
    } else if (i == k && i == l) {
        return i;
    } else if (i == j && k != l) {
        return i;
    } else if (i == k && j != l) {
        return i;
    } else if (i == l && j != k) {
        return i;
    } else if (j == k && i != l) {
        return j;
    } else if (j == l && i != k) {
        return j;
    } else {
        return k == l && i != j ? k : random(i, j, k, l);
    }
}