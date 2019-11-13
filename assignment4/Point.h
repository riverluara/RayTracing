#pragma once

#ifndef POINT_H
#define POINT_H
#include <math.h>


typedef class Point
{
public:
	float x, y, z;
public:
	Point(float nx, float ny, float nz)
	{
		x = nx;
		y = ny;
		z = nz;
	}
	Point() { ; }
	Point operator-() const
	{
		return Point(-x, -y, -z);
	}
	void operator+=(const Point& second)
	{
		x += second.x;
		y += second.y;
		z += second.z;
	}
	void operator+=(float second)
	{
		x += second;
		y += second;
		z += second;
	}
	void operator-=(const Point& second)
	{
		x -= second.x;
		y -= second.y;
		z -= second.z;
	}
	void operator-=(float second)
	{
		x -= second;
		y -= second;
		z -= second;
	}
	void operator*=(float second)
	{
		x *= second;
		y *= second;
		z *= second;
	}
	void operator/=(float second)
	{
		x /= second;
		y /= second;
		z /= second;
	}
	void operator/=(const Point& b)
	{
		x /= b.x;
		y /= b.y;
		z /= b.z;
	}
	Point operator+(const Point& second) const
	{
		Point tmp = *this;
		tmp += second;
		return tmp;
	}
	Point operator+(float second)const
	{
		Point tmp = *this;
		tmp += second;
		return tmp;
	}
	Point operator-(const Point& second)const
	{
		Point tmp = *this;
		tmp -= second;
		return tmp;
	}
	Point operator-(float second)const
	{
		Point tmp = *this;
		tmp -= second;
		return tmp;
	}
	Point operator*(const Point& second)const
	{
		Point tmp;
		tmp.x = x * second.x;
		tmp.y = y * second.y;
		tmp.z = z * second.z;
		return tmp;
	}
	Point operator*(float second)const
	{
		Point tmp = *this;
		tmp *= second;
		return tmp;
	}
	Point operator/(float second)const
	{
		Point tmp = *this;
		tmp /= second;
		return tmp;
	}
	Point operator/(const Point& b)
	{
		Point tmp = *this;
		tmp /= b;
		return tmp;
	}
	void operator=(float second)
	{
		x = second;
		y = second;
		z = second;
	}
	Point cross(const Point& b) const
	{
		Point tmp;
		tmp.x = y * b.z - z * b.y;
		tmp.y = z * b.x - x * b.z;
		tmp.z = x * b.y - y * b.x;
		return tmp;
	}
	float dot(const Point& b) const
	{
		return x * b.x + y * b.y + z * b.z;
	}
	float abs() const
	{
		return sqrt(x*x + y * y + z * z);
	}
	void normalize()
	{
		*this /= this->abs();
	}
	//bool isSame(const Point& b) const
	//{
	//	return fabs(this->x - b.x) <= EPS && fabs(this->y - b.y) <= EPS && fabs(this->z - b.z) <= EPS;
	//}
	float length(const Point& b)
	{
		return (*this - b).abs();
	}
}Vector3;

#endif