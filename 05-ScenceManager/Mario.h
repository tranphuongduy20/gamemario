#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.08f 
#define	MARIO_VMAX				0.15f
#define MARIO_WALKING_ACC		0.00012f 

#define MARIO_JUMP_SPEED_Y		0.2f
#define MARIO_JUMP_SPEED_YMAX	0.2f
#define MARIO_JUMP_Y			0.015f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.0003f
#define MARIO_DIE_DEFLECT_SPEED	 0.2f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_JUMP_MAX		500
#define MARIO_STATE_DIE				400
#define MARIO_STATE_CROUCH			600
#define MARIO_STATE_SKID_LEFT		700
#define MARIO_STATE_SKID_RIGHT		800
#define MARIO_STATE_RUN				900
#define MARIO_STATE_SPIN			1000
#define MARIO_STATE_FLY				1100

#define MARIO_ANI_BIG_IDLE				0
#define MARIO_ANI_BIG_WALKING			1
#define MARIO_ANI_BIG_RUN				2
#define MARIO_ANI_SMALL_IDLE			3
#define MARIO_ANI_SMALL_WALKING			4
#define MARIO_ANI_SMALL_RUN				5
#define MARIO_ANI_SMALL_SKID			6
#define MARIO_ANI_BIG_SKID				7
#define MARIO_ANI_BIG_JUMP				8
#define MARIO_ANI_SMALL_JUMP			9
#define MARIO_ANI_BIG_CROUCH			10
#define MARIO_ANI_RACCOON_IDLE			11
#define MARIO_ANI_RACCOON_WALKING		12
#define MARIO_ANI_RACCOON_RUN			13
#define MARIO_ANI_RACCOON_SKID			14
#define MARIO_ANI_RACCOON_JUMPUP		15
#define MARIO_ANI_RACCOON_CROUCH		16
#define MARIO_ANI_RACCOON_SPIN			17
#define MARIO_ANI_BIG_WALK_RUN			18
#define MARIO_ANI_SMALL_WALK_RUN		19
#define MARIO_ANI_RACCOON_SPIN_SINGLE	20
#define MARIO_ANI_RACCOON_SPEEDUP		21
#define MARIO_ANI_FIRE_IDLE				22
#define MARIO_ANI_FIRE_WALKING			23
#define MARIO_ANI_FIRE_WALK_RUN			24
#define MARIO_ANI_FIRE_RUN				25
#define MARIO_ANI_FIRE_SKID				26
#define MARIO_ANI_FIRE_JUMP				27
#define MARIO_ANI_FIRE_CROUCH			28
#define MARIO_ANI_RACCOON_FLY			29
#define MARIO_ANI_SMALL_DIE				30
#define MARIO_ANI_RACCOON_JUMPDOWN		31
#define MARIO_ANI_RACCOON_FLY_SHEET		32



#define	MARIO_LEVEL_SMALL			51
#define	MARIO_LEVEL_BIG				52
#define	MARIO_LEVEL_RACCOON			53
#define	MARIO_LEVEL_FIRE			54

#define MARIO_SMALL_BBOX_WIDTH		13
#define MARIO_SMALL_BBOX_HEIGHT		15

#define MARIO_BIG_BBOX_WIDTH		15
#define MARIO_BIG_BBOX_HEIGHT		27

#define MARIO_RACCOON_BBOX_WIDTH	18
#define MARIO_RACCOON_BBOX_HEIGHT	27

#define MARIO_FIRE_BBOX_WIDTH		15
#define MARIO_FIRE_BBOX_HEIGHT		27

#define MARIO_UNTOUCHABLE_TIME 5000

#define	MARIO_ACCELERATION		0.005f

#define	MARIO_ACC		0.005f

#define	MARIO_SPEEEDUP		0.002f


#define HIGHT_LEVER1 50

//#define	MARIO_GO		0.015f
//#define	MARIO_GOMAX		0.12f

class CMario : public CGameObject
{
public:
	int level;
	int lastnx;
	int untouchable;
	bool isGround;
	bool isWaitingForAni;
	DWORD untouchable_start;

	bool isJumping;
	bool isJumpHandle;
	float backup_JumpY;
	bool isPressJump;
	bool isCrouch;
	bool isPressCrouch;
	bool isRun;
	bool isSpin;
	bool isFly;
	bool isAttack;
	bool isDie;

	int walkingDirection;
	bool isWaittingPressBtn;
	int startWalkingComplete; 
	bool isWalkingComplete;

	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();


	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void startWalkingDone() { startWalkingComplete = GetTickCount(); }

	void SetPressS(bool press) { isPressJump = press; }
	void SetPressDown(bool pres) { isPressCrouch = pres; }
	void ResetBIG();
	void ResetRACCOON();
	void ResetFIRE();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};