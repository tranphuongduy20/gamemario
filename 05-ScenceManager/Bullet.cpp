//#include "Bullet.h"
//Bullet::Bullet()
//{
//	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BULLET));
//	x = 0;
//	y = 0;
//	alpha = 0;
//	isCollision = 0;
//	isDone = true;
//	damage = 1;
//	timeDelayed = 0;
//	timeDelayMax = BULLET_DELAY;
//}
//
//Bullet::~Bullet()
//{
//}
//
//void Bullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
//{
//}
//
//void Bullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
//{
//	//CGameObject::Update(dt, coObjects);
//	if (isDone == true)
//	{
//		alpha = 0;
//	}
//	else
//	{
//		vy += BULLET_GRAVITY * dt;
//		timeDelayed += dt;
//		CGameObject::Update(dt);
//		if (isTargetTop == false) {
//			vx = BULLET_SPEED * nx;
//			vy = 0;
//		}
//		else {
//			vy = -BULLET_SPEED;
//			vx = 0;
//		}
//#pragma region Xử lý va chạm
//		vector<LPCOLLISIONEVENT> coEvents;
//		vector<LPCOLLISIONEVENT> coEventsResult;
//
//		coEvents.clear();
//
//		CalcPotentialCollisions(colliable_objects, coEvents);
//
//		if (coEvents.size() == 0)
//		{
//			x += dx;
//			y += dy;
//		}
//		else
//		{
//			float min_tx, min_ty, nx = 0, ny;
//			float rdx = 0;
//			float rdy = 0;
//
//			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
//
//			for (UINT i = 0; i < coEventsResult.size(); i++)
//			{
//				LPCOLLISIONEVENT e = coEventsResult[i];
//				if (e->obj->GetType() == EntityType::BRICK)
//				{
//					isCollision = 1;
//					x += min_tx * dx + nx * 0.4f;
//					y += min_ty * dy + ny * 0.4f;
//					vx = 0;
//					vy = 0;
//				}
//			}
//		}
//		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
//#pragma endregion
//	}
//}
//
//void Bullet::Render()
//{
//	RenderBoundingBox();
//	int ani;
//	if (timeDelayed >= timeDelayMax)
//	{
//		isDone = true;
//		timeDelayed = 0;
//	}
//	else
//	{
//		if (isCollision == 0)
//		{
//			animation_set->at(0)->Render(nx, x, y, alpha);
//		}
//		else if (isCollisionEnemies == 1)
//		{
//			isDone = true;
//			timeDelayed = 0;;
//		}
//		else if (isCollision == 1)
//		{
//			ani = SMALL_BULLET_JASON_BANG_ANI;
//			animationSet->at(ani)->OldRender(x, y - DISTANCE_TO_BANG, alpha);
//			if (animationSet->at(ani)->GetFrame() == 3)
//			{
//				isDone = true;
//				timeDelayed = 0;
//			}
//		}
//	}
//}
//
//void Bullet::GetBoundingBox(float& l, float& t, float& r, float& b)
//{
//	l = x;
//	t = y;
//	r = x + BULLET_BBOX_HEIGHT;
//	b = y + BULLET_BBOX_WIDTH;
//
//}