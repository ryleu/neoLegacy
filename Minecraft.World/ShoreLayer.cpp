#include "stdafx.h"
#include "net.minecraft.world.level.newbiome.layer.h"
#include "net.minecraft.world.level.biome.h"
#include "IntCache.h"

ShoreLayer::ShoreLayer(int32_t seed, int64_t seedMixup, shared_ptr<Layer> parent) : Layer(seed,seedMixup)
{
	this->parent = parent;
}

static bool isJungleCompatible(int id)
{
	if (id == Biome::ocean->id || id == Biome::deepOcean->id)
		return true;

	Biome* b = Biome::getBiome(id);
	if (b != nullptr && b->isSame(Biome::jungle)) 
		return true;
	
	return id == Biome::jungleEdge->id || id == Biome::jungle->id || id == Biome::jungleHills->id ||
	       id == Biome::forest->id || id == Biome::forestHills->id || id == Biome::taiga->id ||
	       (b != nullptr && b->hasSnow());
}

void ShoreLayer::replaceIfNeighborOcean(intArray& b, intArray& result, int x, int y, int w, int old, int replacement)
{
	if (isOcean(old))
	{
		result[x + y * w] = old;
		return;
	}

	int stride = w + 2;
	int north = b[(x + 1) + y * stride];
	int east  = b[(x + 2) + (y + 1) * stride];
	int west  = b[(x + 0) + (y + 1) * stride];
	int south = b[(x + 1) + (y + 2) * stride];

	if (isOcean(north) || isOcean(east) || isOcean(west) || isOcean(south))
	{
		result[x + y * w] = replacement;
	}
	else
	{
		result[x + y * w] = old;
	}
}

intArray ShoreLayer::getArea(int xo, int yo, int w, int h)
{
	intArray b = parent->getArea(xo - 1, yo - 1, w + 2, h + 2);
	intArray result = IntCache::allocate(w * h);

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			initRandom(x + xo, y + yo);
			int old = b[(x + 1) + (y + 1) * (w + 2)];
			int _n = b[(x + 1) + (y + 0) * (w + 2)];
			int _e = b[(x + 2) + (y + 1) * (w + 2)];
			int _w = b[(x + 0) + (y + 1) * (w + 2)];
			int _s = b[(x + 1) + (y + 2) * (w + 2)];

			if (old == Biome::mushroomIsland->id)
			{
				if (_n == Biome::ocean->id || _e == Biome::ocean->id || _w == Biome::ocean->id || _s == Biome::ocean->id)
				{
					result[x + y * w] = Biome::mushroomIslandShore->id;
				}
				else
				{
					result[x + y * w] = old;
				}
			}
			else if (old == Biome::jungle->id || old == Biome::jungleHills->id || old == Biome::jungleEdge->id)
			{
				if (isJungleCompatible(_n) && isJungleCompatible(_e) && isJungleCompatible(_w) && isJungleCompatible(_s))
				{
					if (!isOcean(_n) && !isOcean(_e) && !isOcean(_w) && !isOcean(_s))
					{
						result[x + y * w] = old;
					}
					else
					{
						result[x + y * w] = Biome::jungleEdge->id;
					}
				}
				else
				{
					result[x + y * w] = Biome::beaches->id;
				}
			}
			else if (old == Biome::mesa->id || old == Biome::mesaPlateauF->id || old == Biome::mesaPlateau->id)
			{
				replaceIfNeighborOcean(b, result, x, y, w, old, Biome::mesa->id);
			}
			else if (Biome::getBiome(old) != nullptr && Biome::getBiome(old)->hasSnow())
			{
				replaceIfNeighborOcean(b, result, x, y, w, old, Biome::coldBeach->id);
			}
			else if (old == Biome::extremeHills->id || old == Biome::extremeHills_plus->id)
			{
				replaceIfNeighborOcean(b, result, x, y, w, old, Biome::stoneBeach->id);
			}
			else if (old == Biome::ocean->id || old == Biome::deepOcean->id || old == Biome::river->id || old == Biome::swampland->id)
			{
				result[x + y * w] = old;
			}
			else
			{
				if (isOcean(_n) || isOcean(_e) || isOcean(_w) || isOcean(_s))
				{
					result[x + y * w] = Biome::beaches->id;
				}
				else
				{
					result[x + y * w] = old;
				}
			}
		}
	}

	return result;
}