#include "LightSource.h"
#include <assert.h>

void LightSource::LightRay::Inhib(Vec2 _rayPos, float a)
{
	if (!initialized)
	{
		start = _rayPos;
		angle = a;
		end = start + Vec2(float(cos(angle)), float(sin(angle)));
		initialized = true;
	}
}

void LightSource::LightRay::Draw(Graphics& gfx)
{
	gfx.DrawLine(start, end, Colors::White);
}

Vec2 LightSource::LightRay::isIntersecting(Wall wall)
{
	const float x1 = wall.start.x;
	const float y1 = wall.start.y;
	const float x2 = wall.end.x;
	const float y2 = wall.end.y;
	const float x3 = start.x;
	const float y3 = start.y;
	const float x4 = end.x;
	const float y4 = end.y;
	const float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

	if (den == 0.0f) //It means the lines are paralel
	{
		return Vec2(0.0000123f, 0.0000123f);
	}

	const float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
	const float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;

	if (0.0f <= t && t <= 1.0f && u > 0.0f) //Why not u < 1.0f? because it will make the ray to have infinite lenght tbh
	{
		return Vec2(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
	}
	else
	{
		return Vec2(0.0000123f, 0.0000123f);
	}
}

void LightSource::LightRay::GetReflections(std::vector<LightRay>& reflectionsVector, std::vector<Wall> w, unsigned int R)
{
	Vec2 prevRefStart = this->start;
	Vec2 prevRefEnd = this->end;

	while(R != 0)
	{
		LightRay ref;
		//Initializing ref
		{
			Wall theWall = w[this->intersectingWallID];
			Wall Normal((theWall.end + theWall.start)/2, 
						((theWall.end + theWall.start) / 2) + Vec2(-(theWall.end.y - theWall.start.y), theWall.end.x - theWall.start.x));

			//a and b are vectors at the 0,0 origin now
			Vec2 a = Normal.end - Normal.start;
			Vec2 b = prevRefStart - prevRefEnd; //Draw it in MS Paint, you will understand why

			float refAngle = a.GetAngle() - b.GetAngle(a.GetAngle());

			//I'm using this dir vector just so that the ref doesn't start in the wall which can create some problems with the intersections
			Vec2 dir = (prevRefEnd - prevRefStart).Normalize();
			ref.Inhib(prevRefEnd - dir, refAngle);
			if (ref.angle >= 6.2831853f)
			{
				ref.angle -= 6.2831853f;
			}
		}

		//Im using those to find the closest end
		Vec2 recordEnd = ref.end;
		//The biggest possible line in the screen
		float recordLenSquared = Graphics::ScreenWidth * Graphics::ScreenWidth + Graphics::ScreenHeight * Graphics::ScreenHeight;

		for (unsigned int wallIndex = 0; wallIndex < w.size(); wallIndex++)
		{
			//Checking for intersections with the wall
			Vec2 l(ref.isIntersecting(w[wallIndex]));
			if (l != Vec2(0.0000123f, 0.0000123f))
			{
				ref.hasIntersected = true;
				Vec2 possibleNewRecord = l;
				float len = Vec2(possibleNewRecord - ref.start).GetLengthSq();
				if (len < recordLenSquared)
				{
					recordLenSquared = len;
					recordEnd = possibleNewRecord;
					intersectingWallID = wallIndex;
				}
			}
		}

		if (ref.hasIntersected)
		{
			ref.end = recordEnd;
			reflectionsVector.push_back(ref);
			R--;
			prevRefStart = ref.start;
			prevRefEnd = ref.end;
		}
		else
		{
			R = 0;
		}
	}
}

LightSource::LightSource(Vec2 sourcePos, float angleOfVision, float offSet, unsigned int r)
{
	pos = sourcePos;
	nReflections = r;

	//Transforming it in radians
	angleOfVision *= 3.1415926f / 180.0f;
	offSet *= 3.1415926f / 180.0f;

	float angleForRay = offSet;
	for (int i = 0; i < nRays; i++)
	{
		rays[i].Inhib(pos, angleForRay);
		angleForRay += (angleOfVision / nRays);
	}
}

void LightSource::Update(std::vector<Wall> w)
{
	//Clear the reflections
	reflectedRays.clear();

	for (int i = 0; i < nRays; i++)
	{
		if (rays[i].angle >= 6.2831853f)
		{
			rays[i].angle -= 6.2831853f;
		}
		rays[i].hasIntersected = false;
		rays[i].start = pos; //This is if the source is moving
		rays[i].end = pos + Vec2(float(cos(rays[i].angle)), float(sin(rays[i].angle)));

		//Im using those to find the closest end
		Vec2 recordEnd = rays[i].end;
		//The biggest possible line in the screen
		float recordLenSquared = Graphics::ScreenWidth * Graphics::ScreenWidth + Graphics::ScreenHeight * Graphics::ScreenHeight;

		for (unsigned int wallIndex = 0; wallIndex < w.size(); wallIndex++)
		{
			//Checking for intersections with the wall
			Vec2 l(rays[i].isIntersecting(w[wallIndex]));
			if (l != Vec2(0.0000123f, 0.0000123f))
			{
				rays[i].hasIntersected = true;
				Vec2 possibleNewRecord = l;
				float len = Vec2(possibleNewRecord - rays[i].start).GetLengthSq();
				if (len < recordLenSquared)
				{
					recordLenSquared = len;
					recordEnd = possibleNewRecord;
					rays[i].intersectingWallID = wallIndex;
				}
			}
		}

		if (rays[i].hasIntersected)
		{
			rays[i].end = recordEnd;
			rays[i].GetReflections(reflectedRays, w, nReflections);
		}
		else
		{
			rays[i].end = rays[i].start;
		}
	}
}

void LightSource::Rotate(float offSet)
{
	if (offSet >= 360.0f)
	{
		offSet -= 360.0f;
	}
	//Transforming it in radians
	offSet *= 3.1415926f / 180.0f;

	const float p = rays[1].angle - rays[0].angle; //This could be any rays, it just needs to be consecutive
	for (int i = 0; i < nRays; i++)
	{
		rays[i].angle = offSet;
		offSet += p;
	}
}

void LightSource::Draw(Graphics& gfx)
{
	for (int i = 0; i < nRays; i++)
	{
		rays[i].Draw(gfx);
	}
	for (int i = 0; i < reflectedRays.size(); i++)
	{
		reflectedRays[i].Draw(gfx);
	}
}

int LightSource::GetnRays() const
{
	return nRays;
}
