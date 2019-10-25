#include "Ball.h"
#define MAX_WAIT_TIME 2000

CBall::CBall()
{

}

void CBall::init()
{
	srand((int)time(0));

	x = CGame::GetInstance()->getWidth() / 2;
	y = CGame::GetInstance()->getHeight() / 2;

	resultCollision = 0;

	// xac dinh van toc vx
	float scale = rand() / (float)RAND_MAX; /*  [5.0, 7.0] */
	//_vx = 1.5f + scale * 0.5f;
	//_vx = 1.5f;

	// cham
	//_vx = 0.125f;
	//_vy = 0.1f;

	//// nhanh
	_vx = 1.f;
	_vy = 1.2f;

	//_vx = 1.2f;
	// random huong chuyen dong
	//_vy = 2.0f;

	dx = _vx;
	dy = _vy;

}

void CBall::update(DWORD dt, RECT _bar1, RECT _bar2)
{
	DWORD collisionTime = dt;
	resultCollision = 0;

	if (dx > 0)
	{
		if (x >= CGame::GetInstance()->getWidth() - getWidth() / 2)
		{
			dx = -_vx;
		}
		else
		{
			collisionTime = sweptAABB(dx, dy, _bar2, dt, resultCollision);
			//if (collisionTime == 0) _RPT1(0, "[INFO] DIRECTION %d \n", resultCollision);
			if (collisionTime == 0 && resultCollision == D_LEFT)
			{
				dx = -_vx;
			}
		}

	}
	else if (dx < 0)
	{

		if (x <= getWidth() / 2)
		{
			dx = _vx;
		}
		else
		{
			collisionTime = sweptAABB(dx, dy, _bar1, dt, resultCollision);
			if (collisionTime == 0) _RPT1(0, "[INFO] DIRECTION %d \n", resultCollision);

			if (collisionTime == 0 && resultCollision == D_LEFT)
			{
				dx = _vx;
			}

		}

	}

	if (dy > 0)
	{
		if (y > CGame::GetInstance()->getHeight() - getHeight() / 2)
		{
			y = CGame::GetInstance()->getHeight() - getHeight() / 2;
			dy = -_vy;
			return;
		}
	}
	else if (dy < 0)
	{
		if (y < getHeight() / 2)
		{
			y = getHeight() / 2;
			dy = _vy;
			return;
		}
	}

	x += dx * collisionTime;
	y += dy * collisionTime;
}