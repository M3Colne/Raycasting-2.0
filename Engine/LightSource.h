#pragma once

#include "Graphics.h"
#include "Wall.h"

class LightSource
{
private:
	//Data
	struct LightRay : Wall
	{
		bool initialized = false;
		bool hasIntersected = false;
		float angle;

		void Inhib(Vec2 _rayPos, float a);
		void Draw(Graphics& gfx);
		bool isIntersecting(Wall wall);
		Vec2 IntersectionPoint(Wall wall);
	};
	static constexpr int nRays = 100;
	//Data
public:
	//Data
	Vec2 pos;
	LightRay rays[nRays];
	//Data
private:
	//Functions
	//Functions
public:
	//Functions
	LightSource(Vec2 sourcePos, float angleOfVision, float offSet);
	void UpdateWithScreenEdges(Wall* w, int nW);
	void UpdateWithoutScreenEdges(Wall* w, int nW);
	void Rotate(float offSet);
	void Draw(Graphics& gfx);
	int GetnRays() const;
	//Functions
};