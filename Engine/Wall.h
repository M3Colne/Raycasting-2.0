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
	void Draw(Graphics& gfx, Color c)
	{
		gfx.DrawLine(start, end, c);
	}
};