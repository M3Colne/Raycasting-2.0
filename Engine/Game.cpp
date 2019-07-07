/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	playerPos(Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2),
	playerDetection(playerPos, 360.0f, 0.0f, 0)
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	//Walls
	if (wnd.mouse.LeftIsPressed())
	{
		possibleNewWall.start = Vec2(float(wnd.mouse.GetPosX()), float(wnd.mouse.GetPosY()));
		startInit = true;
	}
	else if (wnd.mouse.RightIsPressed() && startInit)
	{
		possibleNewWall.end = Vec2(float(wnd.mouse.GetPosX()), float(wnd.mouse.GetPosY()));
		walls.push_back(possibleNewWall);
		startInit = false;
	}

	playerDetection.UpdateWithoutScreenEdges(walls);

	for (int i = 0; i < playerDetection.GetnRays(); i++)
	{
		if (Vec2(playerDetection.rays[i].end - playerDetection.rays[i].start).GetLength() <= playerRadius)
		{
			color = 255;
			break;
		}
		else
		{
			color = 122;
		}
	}

	//Player movement
	if (wnd.kbd.KeyIsPressed('W'))
	{
		playerPos += {0.0f, -vel};
		playerDetection.pos += {0.0f, -vel};
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		playerPos += {0.0f, vel};
		playerDetection.pos += {0.0f, vel};
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		playerPos += {-vel, 0.0f};
		playerDetection.pos += {-vel, 0.0f};
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		playerPos += {vel, 0.0f};
		playerDetection.pos += {vel, 0.0f};
	}
}

void Game::DrawPlayer(Vec2 pos, float radius, int r, int g, int b)
{
	const float topLeftX = pos.x - radius;
	const float topLeftY = pos.y - radius;
	const float diameter = (radius * 2) + 1;

	for (float y = topLeftY; y < topLeftY + diameter; ++y) {
		for (float x = topLeftX; x < topLeftX + diameter; ++x) {
			const float DistanceSquared = (pos.x - x) * (pos.x - x) + (pos.y - y) * (pos.y - y);

			if (DistanceSquared + 0.5f <= radius * radius) {
				gfx.PutPixel(int(x), int(y), r, g, b);
			}
		}
	}
}

void Game::ComposeFrame()
{
	//Wall drawing
	for (unsigned int i = 0; i < walls.size(); i++)
	{
		walls[i].Draw(gfx, Colors::Magenta);
	}

	//Player detection rays drawing
	playerDetection.Draw(gfx);

	//Player drawing
	DrawPlayer(playerPos, playerRadius, 255, color, 122);
}
