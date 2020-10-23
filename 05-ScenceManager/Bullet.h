//#pragma once
//#define ANIMATION_SET_BULLET		5
//#define BULLET_BBOX_WIDTH			8
//#define BULLET_BBOX_HEIGHT			9
//
//#define BULLET_SPEED					0.2f
//#define BULLET_GRAVITY					0.0003f
//#define BULLET_DELAY				860
//#include "GameObject.h"
//class Bullet;
//typedef Bullet* LPBULLET;
//class Bullet : public CGameObject
//{
//public:
//	bool isDone;
//	int damage;
//	int isTargetTop;
//	int alpha;
//	int isCollision;
//	int isCollisionEnemies;
//	float timeDelayed, timeDelayMax;
//
//public:
//	Bullet();
//	~Bullet();
//	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
//	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
//	virtual void Render();
//
//	void Fire(int direct, float posX, float posY) { nx= direct; x = posX ; y = posY; alpha = 255; isDone = false; isCollision = 0; isCollisionEnemies = 0; }
//	void ResetDelay() { timeDelayed = 0; }
//	bool GetIsDone() { return isDone; }
//	void SetIsDone(bool isdone) { isDone = isdone; }
//	int GetDamage() { return damage; }
//
//};
