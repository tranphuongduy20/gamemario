#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  15
#define BRICK_BBOX_HEIGHT 15

class CBrick : public CGameObject
{
public:
	int width, height;
public:
	CBrick(int width, int height);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};