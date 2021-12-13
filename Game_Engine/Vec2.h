#pragma once

#include "Dependencies.h"

namespace sa {

	class Vec2
	{
	public:
		float x = 0.f;
		float y = 0.f;

		Vec2();
		Vec2(float xin, float yin);

		Vec2 operator +(const Vec2& rhs)const;
		Vec2 operator -(const Vec2& rhs)const;
		Vec2 operator *(const float scalar)const;
		Vec2 operator /(const float scalar)const;

		void operator += (const Vec2& rhs);
		void operator -= (const Vec2& rhs);
		void operator *= (const float scalar);
		void operator /= (const float scalar);

		float length();
		float length_sq();
		void normalize();

		friend std::ostream& operator<<(std::ostream& stream, const Vec2& vector)
		{
			return stream << vector.x << " " << vector.y;
		}
	};

}

