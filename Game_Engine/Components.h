#pragma once

#include "Dependencies.h"

namespace sa {

	class CTransform
	{
	public:
		sa::Vec2 pos{ 0.f, 0.f };
		sa::Vec2 speed{ 0.f, 0.f };
		float angle = 0.f;

		CTransform(const Vec2& p, const Vec2& s, float a)
			:pos(p), speed(s), angle(a)
		{ }
	};

	class CShape
	{
	public:
		sf::CircleShape circle;

		CShape(float radius, int points, const sf::Color& fillcolor , const sf::Color& outlineColor, float thickness)
			:circle(radius, points)
		{
			circle.setOrigin(radius, radius);
			circle.setFillColor(fillcolor);
			circle.setOutlineColor(outlineColor);
			circle.setOutlineThickness(thickness);
		}
	};

	class CCollision
	{
	public:
		float radius = 0.f;

		CCollision(float r)
			:radius(r)
		{ }
	};

	class CScore
	{
	public:
		int score = 0;
		CScore(int s)
			:score(s)  
		{ }
	};

	class CLifespan
	{
	public:
		int remLifeSpan = 0;
		int totalLifeSpan = 0;

		CLifespan(int total)
			:remLifeSpan(total), totalLifeSpan(total)
		{ }
	};

	class CInput
	{
	public:
		bool up    = false;
		bool down  = false;
		bool left  = false;
		bool right = false;
		bool shoot = false;

		CInput() { }
	};
}
