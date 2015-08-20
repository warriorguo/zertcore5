/*
 * geometric.h
 *
 *  Created on: 2013-5-14
 *      Author: Andrew Guo
 */

/*
 * geometric.h
 *
 *  Update on: 2015-7-19
 *      Author: Andrew Guo
 *
 *  Add serialization
 */

#ifndef ZERTCORE_SUIT_LOCATION_GEOMETRIC_H_
#define ZERTCORE_SUIT_LOCATION_GEOMETRIC_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore {

/**
 *
 * Point2d<T>
 *
 */
template <typename T>
struct Point2d : Unserializable<Point2d<T> >, Serializable<Point2d<T> >
{
	typedef T								value_type;
	typedef Point2d<T>						self;
	typedef self&							reference;
	typedef const self&						const_reference;

	T x, y;

	Point2d():x(0), y(0)					{;}
	Point2d(T px, T py) : x(px), y(py)		{;}
	Point2d(const_reference p) : x(p.x), y(p.y)
											{;}

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["x"] & x;
		archiver["y"] & y;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		archiver["x"] & x;
		archiver["y"] & y;

		return x > 0 || y > 0;
	}

	reference operator = (const_reference cp) {
		x = cp.x; y = cp.y;
		return *this;
	}
	reference operator += (const_reference cp) {
		x += cp.x; y += cp.y;
		return *this;
	}
	reference operator -= (const_reference cp) {
		x -= cp.x; y -= cp.y;
		return *this;
	}

	bool operator == (const_reference cp) const {
		return x == cp.x && y == cp.y;
	}
	bool operator != (const_reference cp) const {
		return x != cp.x || y != cp.y;
	}
	T distance(const_reference p) const {
		return sqrt((double)distance2s(p));
	}
	T distance2s(const_reference p) const {
		return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);
	}
};

template <typename T>
Point2d<T> operator+ (typename Point2d<T>::const_reference p1, typename Point2d<T>::const_reference p2) {
	Point2d<T> p(p1); p += p2;
	return p;
}

template <typename T>
Point2d<T> operator- (typename Point2d<T>::const_reference p1, typename Point2d<T>::const_reference p2) {
	Point2d<T> p(p1); p -= p2;
	return p;
}

template <typename T>
struct Point2dLess : std::binary_function <Point2d<T>, Point2d<T>, bool>
{
	bool operator() (const Point2d<T>& x, const Point2d<T>& y) const {
		static Point2d<T> center;
		return x.distance2s(center) < y.distance2s(center);
	}
};

/**
 *
 * Point3d<T>
 *
 */
template <typename T>
struct Point3d : Unserializable<Point3d<T> >, Serializable<Point3d<T> >
{
	typedef T								value_type;
	typedef Point3d<T>						self;
	typedef self&							reference;
	typedef const self&						const_reference;

	T x, y, z;

	Point3d():x(0), y(0), z(0)				{;}
	Point3d(T px, T py, T pz) : x(px), y(py), z(pz)
											{;}
	Point3d(const_reference p) : x(p.x), y(p.y), z(p.z)
											{;}

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["x"] & x;
		archiver["y"] & y;
		archiver["z"] & z;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		archiver["x"] & x;
		archiver["y"] & y;
		archiver["z"] & z;

		return x > 0 || y > 0 || z > 0;
	}

	reference operator = (const_reference cp) {
		x = cp.x; y = cp.y; z = cp.z;
		return *this;
	}
	reference operator = (typename Point2d<T>::const_reference cp) {
		x = cp.x; y = cp.y; z = 0;
		return *this;
	}
	reference operator += (const_reference cp) {
		x += cp.x; y += cp.y; z += cp.z;
		return *this;
	}
	reference operator -= (const_reference cp) {
		x -= cp.x; y -= cp.y; z -= cp.z;
		return *this;
	}
	reference operator += (typename Point2d<T>::const_reference cp) {
		x += cp.x; y += cp.y;
		return *this;
	}
	reference operator -= (typename Point2d<T>::const_reference cp) {
		x -= cp.x; y -= cp.y;
		return *this;
	}
	bool operator == (const_reference cp) const {
		return x == cp.x && y == cp.y && z == cp.z;
	}
	bool operator != (const_reference cp) const {
		return x != cp.x || y != cp.y || z != cp.z;
	}
	T distance(const_reference p) {
		return sqrt(distance2s(p));
	}
	T distance2s(const_reference p) {
		return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y) + (p.z - z) * (p.z - z);
	}
};

template <typename T>
Point3d<T> operator+ (typename Point3d<T>::const_reference p1, typename Point3d<T>::const_reference p2) {
	Point3d<T> p(p1); p += p2;
	return p;
}

template <typename T>
Point3d<T> operator- (typename Point3d<T>::const_reference p1, typename Point3d<T>::const_reference p2) {
	Point3d<T> p(p1); p -= p2;
	return p;
}

template <typename T>
struct Point3dLess : std::binary_function <Point3d<T>, Point3d<T>, bool>
{
	bool operator() (const Point3d<T>& x, const Point3d<T>& y) const {
		static Point3d<T> center;
		return x.distance2s(center) < y.distance2s(center);
	}
};

template <typename T>
struct Circle;
/**
 *
 * Rectangle<T>
 *
 */
template <typename T>
struct Rectangle : Unserializable<Rectangle<T> >, Serializable<Rectangle<T> >
{
	typedef T								value_type;
	typedef Rectangle<T>					self;
	typedef self&							reference;
	typedef const self&						const_reference;

	T x, y, width, height;

	Rectangle():x(0), y(0), width(0), height(0)
											{;}
	Rectangle(T px, T py, T pw, T ph):
		x(px), y(py), width(pw), height(ph)	{;}

	Rectangle(typename Point2d<T>::const_reference point):
		x(point.x), y(point.y), width(0), height(0)
											{;}
	Rectangle(typename Point2d<T>::const_reference b,
			typename Point2d<T>::const_reference e):
		x(b.x), y(b.y), width(e.x - b.x), height(e.y - b.y)
											{;}
	Rectangle(const_reference cr):
		x(cr.x), y(cr.y), width(cr.width), height(cr.height)
											{;}

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["x"] & x;
		archiver["y"] & y;
		archiver["w"] & width;
		archiver["h"] & height;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		archiver["x"] & x;
		archiver["y"] & y;
		archiver["w"] & width;
		archiver["h"] & height;

		return x > 0 || y > 0 || width > 0 || height > 0;
	}

	reference operator = (const_reference cr) {
		x = cr.x; y = cr.y; width = cr.width; height = cr.height;
		return *this;
	}
	bool operator == (const_reference cr) const {
		return x == cr.x && y == cr.y && width == cr.width && height == cr.height;
	}
	bool operator != (const_reference cr) const {
		return x != cr.x || y != cr.y || width != cr.width || height != cr.height;
	}
	bool inside(typename Point2d<T>::const_reference cr) const {
		return x <= cr.x && y <= cr.y && (x + width) > cr.x && (y + height) > cr.y;
	}
	bool intersect(const_reference cr) const {
		return (abs(cr.x - x) < (cr.width + width)) && (abs(cr.y - y) < (cr.height + height));
	}
	bool intersect(const Circle<T>& circle) const;
	reference move(typename Point2d<T>::const_reference cr) {
		x += cr.x; y += cr.y;
		return *this;
	}
	reference enlarge(typename Point2d<T>::const_reference cr) {
		width += cr.x; height += cr.y;
		return *this;
	}
};

/**
 * Circle<T>
 */
template <typename T>
struct Circle : Unserializable<Circle<T> >, Serializable<Circle<T> >
{
	typedef T								value_type;
	typedef Circle<T>						self;
	typedef self&							reference;
	typedef const self&						const_reference;

	Point2d<T> center;
	T r;

	Circle() : center(), r(0)				{;}
	Circle(typename Point2d<T>::const_reference point, const T& radial) :
		center(point), r(radial)			{;}
	Circle(const_reference cr) : center(cr.center), r(cr.r)	{;}

	template <class Archiver>
	void serialize(Archiver& archiver) const {
		archiver["c"] & center;
		archiver["r"] & r;
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		archiver["c"] & center;
		archiver["r"] & r;

		return r > 0;
	}

	reference operator = (const_reference cr) {
		center = cr.center; r = cr.r;
		return *this;
	}
	bool operator == (const_reference cr) const {
		return r == cr.r && center == cr.center;
	}
	bool operator != (const_reference cr) const {
		return r != cr.r || center != cr.center;
	}
	reference move(typename Point2d<T>::const_reference cr) {
		center += cr;
		return *this;
	}
	reference enlarge(const T& offset) {
		r += offset;
		return *this;
	}
	bool inside(typename Point2d<T>::const_reference cr) const {
		return center.distance2s(cr) <= r * r;
	}
	bool intersect(const_reference cr) const {
		return center.distance2s(cr) <= (cr.r + r) * (cr.r + r);
	}
	bool intersect(typename Rectangle<T>::const_reference cr) const {
		if (cr.inside(center)) return true;
		if (inside(Point2d<T>(cr.x, cr.y))) return true;
		if (inside(Point2d<T>(cr.x + cr.width, cr.y))) return true;
		if (inside(Point2d<T>(cr.x, cr.y + cr.height))) return true;
		if (inside(Point2d<T>(cr.x + cr.width, cr.y + cr.height))) return true;
		return false;
	}
};

template <typename T>
bool Rectangle<T>::intersect(const Circle<T>& circle) const {
	return circle.intersect(*this);
}

}


#endif /* SUIT_LOCATION_GEOMETRIC_H_ */
