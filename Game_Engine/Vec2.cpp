#include "Vec2.h"

namespace sa {

	Vec2::Vec2()
		:x(0.f), y(0.f) { }

	Vec2::Vec2(float xin, float yin)
		: x(xin), y(yin) { }

	Vec2 Vec2::operator+(const Vec2& rhs)    const { return Vec2(x + rhs.x, y + rhs.y); }

	Vec2 Vec2::operator-(const Vec2& rhs)    const { return Vec2(x - rhs.x, y - rhs.y); }

	Vec2 Vec2::operator*(const float scalar) const { return Vec2(scalar * x, scalar * y); }

	Vec2 Vec2::operator/(const float scalar) const { return Vec2(x / scalar, y / scalar); }

	void Vec2::operator+=(const Vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}

	void Vec2::operator-=(const Vec2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}

	void Vec2::operator*=(const float scalar)
	{
		x *= scalar;
		y *= scalar;
	}

	void Vec2::operator/=(const float scalar)
	{
		x /= scalar;
		y /= scalar;
	}

	float Vec2::length()
	{
		return std::sqrt(x * x + y * y);
	}

	float Vec2::length_sq()
	{
		return (x * x + y * y);
	}

	void Vec2::normalize()
	{
		float length = std::sqrt(x * x + y * y);

		if (length > 0.0f)
		{
			x /= length;
			y /= length;
		}
	}
}
