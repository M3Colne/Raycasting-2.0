#pragma once

#include "Graphics.h"

struct Wall
{
	Vec2 start;
	Vec2 end;

	Wall() = default;
	Wall(Vec2 a, Vec2 b)
	{
		start = a;
		end = b;
	}
	Wall operator+(const Vec2& os) const
	{
		return Wall(this->start + os, this->end + os);
	}
	Wall& operator+=(const Vec2& os)
	{
		this->start += os;
		this->end += os;

		return *this;
	}
	void Draw(Graphics& gfx, Color c)
	{
		gfx.DrawLine(start, end, c);
	}
};