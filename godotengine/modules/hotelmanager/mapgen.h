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

#pragma once

#include <cstdint>
#include "game_tiles.h"

class PerlinNoise
{
public:
	PerlinNoise(const double &persistence, const double &frequency,
		const double &amplitude, int32_t _octaves, int32_t seed);

	double get_height(const double &x, const double &y) const
	{
		return m_amplitude * total(x, y);
	}

private:
	double total(const double &i, const double &j) const;
	double get_value(const double &x, const double &y) const;
	double interpolate(const double &x, const double &y, const double &a) const;
	double noise(int32_t x, int32_t y) const;

	double m_persistence = 0.0f;
	double m_frequency = 0.0f;
	double m_amplitude = 0.0f;
	int32_t m_octaves = 0;
	int32_t m_seed = 0;
};

class MapGen
{
public:
	MapGen();
	~MapGen();

	GameMapTile get_tile_for_pos(int32_t x, int32_t y);
private:
	PerlinNoise *m_perlin_noise;
};
