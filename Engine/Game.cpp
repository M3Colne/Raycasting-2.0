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
	cameraPos(Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2),
	camera(cameraPos, 15.0f, offSet, 3)
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

	camera.Update(walls);

	//Player movement
	if (wnd.kbd.KeyIsPressed('W'))
	{
		cameraPos += {0.0f, -vel};
		camera.pos += {0.0f, -vel};
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		cameraPos += {0.0f, vel};
		camera.pos += {0.0f, vel};
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		cameraPos += {-vel, 0.0f};
		camera.pos += {-vel, 0.0f};
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		cameraPos += {vel, 0.0f};
		camera.pos += {vel, 0.0f};
	}
	//Player vision
	if (wnd.kbd.KeyIsPressed('C'))
	{
		offSet -= visionVel;
		camera.Rotate(offSet);
	}
	if (wnd.kbd.KeyIsPressed('V'))
	{
		offSet += visionVel;
		camera.Rotate(offSet);
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

		//Drawing normal
		gfx.DrawLine((walls[i].end + walls[i].start) / 2,
			Vec2((walls[i].end + walls[i].start) / 2) 
			+ Vec2(-(walls[i].end.y - walls[i].start.y), walls[i].end.x - walls[i].start.x).NormalizeTo(30.0f), Colors::Cyan);
	}

	//Player detection rays drawing
	camera.Draw(gfx);

	//Player drawing
	DrawPlayer(cameraPos, playerRadius, 255, 122, 122);
}
