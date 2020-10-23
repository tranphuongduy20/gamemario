#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "Brick.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	SetState(MARIO_ANI_SMALL_IDLE);
	start_x = x;
	start_y = y;
	backup_JumpY = 0;
	isFly = false;
	this->x = x;
	this->y = y;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;
	//vx += 0.001 * dt;
	if (isJumping && backup_JumpY - y >= HIGHT_LEVER1 && isJumpHandle == false)
	{
		if (!isPressJump)
			vy = 0;
		isJumpHandle = true;
	}

	if (isRun == true && vx != 0)
	{
		if (abs(vx) < MARIO_VMAX)
			vx += MARIO_SPEEEDUP * nx;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	isWaittingPressBtn = GetTickCount() - startWalkingComplete <= 350;
	if (isWaittingPressBtn) {
		DebugOut(L"\n true - %d", GetTickCount() - startWalkingComplete);
	}
	else DebugOut(L"\n false - %d", GetTickCount() - startWalkingComplete);

		if (isWaittingPressBtn) {
			if (walkingDirection != nx) {
				isWalkingComplete = true;
			}
			else
				isWalkingComplete = false;
		}
		else
			isWalkingComplete = false;

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (isWaitingForAni && animation_set->at(state)->IsOver()) {
		isWaitingForAni = false;
	}

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.1f;

		/*if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;*/


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else if (level == MARIO_LEVEL_SMALL)
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is CBrick 
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);

				if (e->ny != 0)
				{
					if (e->ny == -1)
					{
						isGround = true;
						isJumping = false;
						isFly = false;
						//vy = 0;
					}
					else
						y += dy;
					
				}
				/*if (e->ny < 0)
				{
					isJumping = false;
				}*/
			} // if Brick
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	int ani = -1;
	if (isDie == true) {
		ani = MARIO_ANI_SMALL_DIE;
	}
	else if (level == MARIO_LEVEL_BIG)
	{
		if (isCrouch == true) {
			ani = MARIO_ANI_BIG_CROUCH;
		}
		else if (isWalkingComplete == true) {
			ani = MARIO_ANI_BIG_SKID;
		}
		else
		{
			if (isJumping == false)
			{
				if (vx == 0)
					ani = MARIO_ANI_BIG_IDLE;
				else if (0 < abs(vx) && abs(vx) <= MARIO_VMAX / 2)
					ani = MARIO_ANI_BIG_WALKING;
				else if (MARIO_VMAX / 2 < abs(vx) && abs(vx) <= MARIO_VMAX)
					ani = MARIO_ANI_BIG_WALK_RUN;
				else if (MARIO_VMAX < abs(vx))
					ani = MARIO_ANI_BIG_RUN;
			}
			else
				ani = MARIO_ANI_BIG_JUMP;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (isWalkingComplete == true) {
			ani = MARIO_ANI_SMALL_SKID;
		}
		else {
			if (isJumping == false)
			{
				if (vx == 0)
					ani = MARIO_ANI_SMALL_IDLE;
				else if (0 < abs(vx) && abs(vx) <= MARIO_VMAX / 2)
					ani = MARIO_ANI_SMALL_WALKING;
				else if (MARIO_VMAX / 2 < abs(vx) && abs(vx) <= MARIO_VMAX)
					ani = MARIO_ANI_SMALL_WALK_RUN;
				else if (MARIO_VMAX < abs(vx))
					ani = MARIO_ANI_SMALL_RUN;
			}
			else
				ani = MARIO_ANI_SMALL_JUMP;
		}
		
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
		if (isFly)
			ani = MARIO_ANI_RACCOON_FLY;
		else if (isCrouch) 
			ani = MARIO_ANI_RACCOON_CROUCH;
		else if (isSpin) 
			ani = MARIO_ANI_RACCOON_SPIN;
		else if (isWalkingComplete) 
			ani = MARIO_ANI_RACCOON_SKID;
		else 
		{
			if (isJumping == false)
			{
				if (vx == 0)
				{	
					if (isAttack == false)
						ani = MARIO_ANI_RACCOON_IDLE;
					else ani = MARIO_ANI_RACCOON_SPIN_SINGLE;
				}
				else if (0 < abs(vx) && abs(vx) <= MARIO_VMAX / 2)
				{
					if (isAttack == false)
						ani = MARIO_ANI_RACCOON_WALKING;
					else ani = MARIO_ANI_RACCOON_SPIN_SINGLE;
				}
				else if (MARIO_VMAX / 2 < abs(vx) && abs(vx) <= MARIO_VMAX)
				{
					if (isAttack == false)
						ani = MARIO_ANI_RACCOON_RUN;
					else ani = MARIO_ANI_RACCOON_SPIN_SINGLE;
				}
				else if (MARIO_VMAX < abs(vx))
				{
						ani = MARIO_ANI_RACCOON_SPEEDUP;
				}
			}
			else
			{
				if (vy > 0.03)
					ani = MARIO_ANI_RACCOON_JUMPDOWN;
				else
					ani = MARIO_ANI_RACCOON_JUMPUP;
			}
		}
	}
	else if (level == MARIO_LEVEL_FIRE) 
	{
		if (isCrouch == true) {
			ani = MARIO_ANI_FIRE_CROUCH;
		}
		else if (isWalkingComplete == true) {
			ani = MARIO_ANI_FIRE_SKID;
		}
		else
		{
			if (isJumping == false)
			{
				if (vx == 0)
					ani = MARIO_ANI_FIRE_IDLE;
				else if (0 < abs(vx) && abs(vx) <= MARIO_VMAX / 2)
					ani = MARIO_ANI_FIRE_WALKING;
				else if (MARIO_VMAX / 2 < abs(vx) && abs(vx) <= MARIO_VMAX)
					ani = MARIO_ANI_FIRE_WALK_RUN;
				else if (MARIO_VMAX < abs(vx))
					ani = MARIO_ANI_FIRE_RUN;
			}
			else
				ani = MARIO_ANI_FIRE_JUMP;
		}
	}
	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(ani)->Render(nx, x, y, alpha);
	if (animation_set->at(MARIO_ANI_RACCOON_SPIN_SINGLE)->GetCurrentFrame() == 3)
		isAttack = false;
	DebugOut(L"[vx] %f \n", vx);
	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		if (isCrouch == true)
			return;
		nx = 1;
		if (isRun == true)
			return;
		vx = MARIO_WALKING_SPEED;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isCrouch == true)
			return;
		nx = -1;
		if (isRun == true)
			return;
		vx = -MARIO_WALKING_SPEED;
		break;
	case MARIO_STATE_IDLE:
		isSpin = false;
		isRun = false;
		isPressJump = false;
		isCrouch = false;
		if (vx > 0) {
			vx -= 0.00015 * dt;
			if (vx < 0)
				vx = 0;
		}
		else if (vx < 0) {
			vx += 0.00015 * dt;
			if (vx > 0)
				vx = 0;
		}
		//vx = 0;
		break;
	case MARIO_STATE_JUMP:
		isPressJump = true;
		isCrouch == true;
		if (level == MARIO_LEVEL_RACCOON)
		{
			if (abs(vx) > MARIO_VMAX || isFly == true)
			{
				isFly = true;
				vy = -0.1;
			}
			else
			{
				if (isJumping == true)
				{
					if (vy < 0)
						return;
					else
						vy = -0.001;
				}
				else
				{
					isJumpHandle = false;
					isJumping = true;
					vy = -MARIO_JUMP_SPEED_YMAX;
					backup_JumpY = y;
				}
			}
		}
		else
		{
			if (isJumping == true)
				return;
			else
			{
				isJumpHandle = false;
				isJumping = true;
				vy = -MARIO_JUMP_SPEED_YMAX;
				backup_JumpY = y;
			}
		}
		break;
	case MARIO_STATE_CROUCH:
		isCrouch = true;
		vx = 0;
		break;
	case MARIO_STATE_SPIN:
		isSpin = true;
		break;
	case MARIO_STATE_DIE:
		isDie = true;
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (level == MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
		right = x + MARIO_RACCOON_BBOX_WIDTH;
		bottom = y + MARIO_RACCOON_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		right = x + MARIO_FIRE_BBOX_WIDTH;
		bottom = y + MARIO_FIRE_BBOX_HEIGHT;
	}
}

void CMario::ResetBIG()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CMario::ResetRACCOON()
{
	SetState(MARIO_ANI_RACCOON_IDLE);
	SetLevel(MARIO_LEVEL_RACCOON);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CMario::ResetFIRE()
{
	SetState(MARIO_ANI_FIRE_IDLE);
	SetLevel(MARIO_LEVEL_FIRE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}


