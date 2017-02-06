/*
 * This game is under its authors' proprietary license and is property of:
 *
 * No commercial usage of this program could be done without its authors
 * agreement or the current game directors.
 *
 * This license can change to a free license if the game directors decide it.
 *
 * Copyright:
 *
 * 2016, Loic Blot <loic.blot@unix-experience.fr>
 *
 * All rights reserved
 */

#include <random>
#include <iostream>
#include <algorithm>
#include "mapgen.h"
#include "gamemap.h"

#define NOISE_MAGIC_Y 57

// Interesting links
// https://shanee.io/blog/2015/09/25/procedural-island-generation/
// https://stackoverflow.com/questions/4753055/perlin-noise-generation-for-terrain
// http://libnoise.sourceforge.net/tutorials/tutorial4.html

/**
 *
 * @param persistence
 * @param frequency more frequency is near 0, less the terrain have variations
 * @param amplitude
 * @param octaves
 * @param seed
 */
PerlinNoise::PerlinNoise(const double &persistence, const double &frequency,
	const double &amplitude, int32_t octaves, int32_t seed):
	m_persistence(persistence),
	m_frequency(frequency),
	m_amplitude(amplitude),
	m_octaves(octaves),
	m_seed(seed)
{
}

/**
 *
 * @param i
 * @param j
 * @return
 */
double PerlinNoise::total(const double &i, const double &j) const
{
	// properties of one octave (changing each loop)
	double t = 0.0f;

	for (uint32_t k = 0; k < m_octaves; k++) {
		double freq = k * m_frequency;
		double amplitude = m_persistence * k;
		t += get_value(j * freq + m_seed, i * freq + m_seed) * amplitude;
	}

	return t;
}

/**
 *
 * @param x
 * @param y
 * @return
 */
double PerlinNoise::get_value(const double &x, const double &y) const
{
	int Xint = (int)x;
	int Yint = (int)y;
	double Xfrac = x - Xint;
	double Yfrac = y - Yint;

	//noise values
	double n01 = noise(Xint-1, Yint-1);
	double n02 = noise(Xint+1, Yint-1);
	double n03 = noise(Xint-1, Yint+1);
	double n04 = noise(Xint+1, Yint+1);
	double n05 = noise(Xint-1, Yint);
	double n06 = noise(Xint+1, Yint);
	double n07 = noise(Xint, Yint-1);
	double n08 = noise(Xint, Yint+1);
	double n09 = noise(Xint, Yint);

	double n12 = noise(Xint+2, Yint-1);
	double n14 = noise(Xint+2, Yint+1);
	double n16 = noise(Xint+2, Yint);

	double n23 = noise(Xint-1, Yint+2);
	double n24 = noise(Xint+1, Yint+2);
	double n28 = noise(Xint, Yint+2);

	double n34 = noise(Xint+2, Yint+2);

	// find the noise values of the four corners
	double x0y0 = 0.0625*(n01+n02+n03+n04) + 0.125*(n05+n06+n07+n08) + 0.25*(n09);
	double x1y0 = 0.0625*(n07+n12+n08+n14) + 0.125*(n09+n16+n02+n04) + 0.25*(n06);
	double x0y1 = 0.0625*(n05+n06+n23+n24) + 0.125*(n03+n04+n09+n28) + 0.25*(n08);
	double x1y1 = 0.0625*(n09+n16+n28+n34) + 0.125*(n08+n14+n06+n24) + 0.25*(n04);

	// interpolate between those values according to the x and y fractions
	double v1 = interpolate(x0y0, x1y0, Xfrac); // interpolate in x direction (y)
	double v2 = interpolate(x0y1, x1y1, Xfrac); // interpolate in x direction (y+1)
	double fin = interpolate(v1, v2, Yfrac);  // interpolate in y direction

	return fin;
}

/**
 *
 * @param x
 * @param y
 * @param a
 * @return
 */
double PerlinNoise::interpolate(const double &x, const double &y, const double &a) const
{
	double negA = 1.0 - a;
	double negASqr = negA * negA;
	double fac1 = 3.0 * (negASqr) - 2.0 * (negASqr * negA);
	double aSqr = a * a;
	double fac2 = 3.0 * aSqr - 2.0 * (aSqr * a);

	return x * fac1 + y * fac2; //add the weighted factors
}

/**
 * Generate noise2d at (x,y)
 * @param x
 * @param y
 * @return
 */
double PerlinNoise::noise(int32_t x, int32_t y) const
{
	int n = x + y * NOISE_MAGIC_Y;
	n = (n << 13) ^ n;
	n = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
	return 1.0 - (double(n) / 1073741824.0);
}

MapGen::MapGen()
{
	uint64_t seed = time(NULL);
	std::mt19937 random_gen(seed);

	int octaves = 2;
	float persistence = 0.5f;
	float frequency = 14.0f;
	float amplitude = 0.8f;

	m_perlin_noise = new PerlinNoise(persistence, frequency, amplitude,
		octaves, seed);
}


MapGen::~MapGen()
{
	delete m_perlin_noise;
}

/**
 *
 * @param x
 * @param y
 * @return tile_id to set at (x,y)
 */
uint32_t MapGen::get_tile_for_pos(int32_t x, int32_t y)
{
	// First realign the map center
	float width = WORLD_LIMIT_X * 2;
	float height = WORLD_LIMIT_Y * 2;
	// WORLD_LIMIT_X - WORLD_LIMIT_Y permits to move to center of map
	x += width / 2 - (WORLD_LIMIT_X - WORLD_LIMIT_Y);
	y += height / 2;

	float distanceX = (WORLD_LIMIT_Y - x) * (WORLD_LIMIT_Y - x);
	float distanceY = (WORLD_LIMIT_Y - y) * (WORLD_LIMIT_Y - y);
	float distanceToCenter = sqrt(distanceX + distanceY);
	distanceToCenter = distanceToCenter / WORLD_LIMIT_Y;

	double noise = m_perlin_noise->get_height(x, y) + (1.0f - distanceToCenter);

	if (noise > 0.85f) {
		return 2; // Stone
	}
	else if (noise > 0.25f) {
		return 1; // Grass
	}
	else if (noise > 0.0f) {
		return 11; // Sand
	}
	else {
		return 4; // Water
	}
}
