#pragma once
#include <math.h>

struct Vector2
{
	Vector2() { x = 0; y = 0; }
	Vector2(float inX, float inY) { x = inX; y = inY; }

	float x;
	float y;

	float GetLengthSqr() const { return (x*x + y*y); }
	float GetLength() const { return sqrt(GetLengthSqr()); }
	void Normalize() {
		float mag = GetLength(); 
		if (mag < 0.00001f)
		{
			x = 0; y = 0;
		}
		else
		{
			x /= mag; y /= mag;
		}
	}
	Vector2 GetNormalized() const { 
		float mag = GetLength();
		if (mag < 0.00001f)
		{
			return Vector2(0, 0);
		}
		else
		{
			return Vector2(x / mag, y / mag);
		}
	}


	//Standard arithmetic
	Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
	Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
	Vector2 operator/(const float f) const { return Vector2(x/f, y/f); }
	Vector2 operator*(const float f) const { return Vector2(x*f, y*f); }

	//Assignment operators
	Vector2& operator=(const Vector2& v) { x = v.x; y = v.y; return *this; }
	Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
	Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
	Vector2& operator/=(const float f) { x /= f; y /= f; return *this; }
	Vector2& operator*=(const float f) { x *= f; y *= f; return *this; }

	//Comparison operators
	bool operator==(const Vector2& v) { return (x == v.x && y == v.y); }
	bool operator!=(const Vector2& v) { return (x != v.x || y != v.y); }
};

