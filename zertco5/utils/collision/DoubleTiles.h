/*
 * DoubleTiles.h
 *
 *  Created on: 2015年6月24日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_COLLISION_DOUBLETILES_H_
#define ZERTCORE_UTILS_COLLISION_DOUBLETILES_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace utils {

template <typename T>
struct Rectage
{
	typedef T								value_type;

	value_type					x{0},
								y{0},
								width{0},
								height{0};
};

}}

namespace zertcore { namespace utils {

typedef double								size_type;

namespace details {

typedef Rectage<size_type>					rect_type;
typedef vector<uuid_t>						data_list_type;

class Sensor;
typedef unordered_set<Sensor *>				sensor_set_type;
/**
 * Tile
 */
struct Tile
{
	sensor_set_type			sensor_set;
	data_list_type			data_list;
};
typedef vector<Tile>						tile_map_type;

/**
 * Sensor
 */
class Sensor
{
	vector<Tile*>			tiles;
};

/**
 * Tiles
 */
template <size_type Size>
class Tiles
{
public:


public:
	explicit Tiles(const rect_type& rect);

public:
	bool setupSensor(Sensor& sensor);

private:
	tile_map_type				tile_map_;
};

}

/**
 * DoubleTiles<>
 */
template <double MaxParticle>
class DoubleTiles
{
public:
};

}}


#endif /* UTILS_COLLISION_DOUBLETILES_H_ */
