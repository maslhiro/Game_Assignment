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
	//_vx = 0.25f;
	_vx = 0.125f;
	// random huong chuyen dong
	_vy = 0.1f;
	//_vy = 0.5f;

	dx = _vx;
	dy = _vy;

	int flag = 2;
	//int flag = 1 + rand() % (4);
	switch (flag)
	{
	case 1:
		// goc trai duoi
		_state = idState::DOWN;
		_side = idSide::TOP;
		_preSide = idSide::RIGHT;
		break;
	case 2:
		// goc trai tren
		_state = idState::UP;
		_side = idSide::BOTTOM;
		_preSide = idSide::RIGHT;
		break;
	case 3:
		// goc phai tren
		_state = idState::UP;
		_side = idSide::BOTTOM;
		_preSide = idSide::LEFT;
		break;
	case 4:
		// goc phai duoi 
		_state = idState::DOWN;
		_side = idSide::TOP;
		_preSide = idSide::LEFT;
		break;
	default:
		_state = NULL;
		_side = NULL;
		_preSide = NULL;
		break;
	}


}

void CBall::update(DWORD dt, RECT _bar1, RECT _bar2)
{
	DWORD collisionTime = dt;


	if (_wait)
	{
		_waitTime += dt;
		if (_waitTime >= MAX_WAIT_TIME) {
			_wait = false;
			_waitTime = 0;
			init();
		}
		else {
			x = CGame::GetInstance()->getWidth() / 2;
			y = CGame::GetInstance()->getHeight() / 2;
		}
		return;
	}

	switch (_side)
	{
#pragma region BOTTOM
	case idSide::BOTTOM:
	{
		if (_state == idState::DOWN) {
			if (std::lroundf(y) < CGame::GetInstance()->getHeight() - _width / 2) {
				dy = _vy;
				y += dy * dt;
			}
			else {
				y = CGame::GetInstance()->getHeight() - _width / 2;
				_state = idState::UP;
			}
		}
		else if (_state == idState::UP) {
			//_RPT1(0, "[INFO] X %ld - %ld \n", std::lroundf(x), CGame::GetInstance()->getWidth() - _width / 2 - (_bar2.right - _bar2.left));

			if (std::floorf(x) > ((_width / 2) + _bar1.right) && std::lroundf(x) < (CGame::GetInstance()->getWidth() - _width / 2 - (_bar2.right - _bar2.left)))
			{
				// check swept aabb
				if (_preSide == idSide::LEFT)
				{
					collisionTime = sweptAABB(dx, dy, getBoundingBox(), _bar2, dt, resultCollision);

					if (resultCollision == eDirection::D_RIGHT)
					{
						_side = idSide::RIGHT;
						_preSide = idSide::BOTTOM;
						_state = idState::UP;

						break;
					}
				}
				else if (_preSide == idSide::RIGHT)
				{
					collisionTime = sweptAABB(dx, dy, getBoundingBox(), _bar1, dt, resultCollision);

					if (resultCollision == eDirection::D_LEFT)
					{
						_side = idSide::LEFT;
						_preSide = idSide::BOTTOM;
						_state = idState::UP;

						break;
					}
				}
				_RPT1(0, "[INFO] COLLISION BOTTOM %ld \n", collisionTime);

				_RPT1(0, "[INFO] DIRECTION %d \n", resultCollision);

				dy = -_vy;
				y += dy * collisionTime;
			}
			else
			{

				if (_preSide == idSide::LEFT)
				{
					_side = idSide::RIGHT;
				}
				else if (_preSide == idSide::RIGHT)
				{
					_side = idSide::LEFT;
				}

				_preSide = idSide::BOTTOM;
				_state = idState::DOWN;
				return;

			}
		}

		if (_preSide == idSide::LEFT)
		{
			dx = _vx;
			x += dx * collisionTime;
		}
		else if (_preSide == idSide::RIGHT)
		{
			dx = -_vx;
			x += dx * collisionTime;
		}
	}
	break;
#pragma endregion

#pragma region RIGHT
	case idSide::RIGHT:
	{
		if (_preSide == idSide::BOTTOM)
		{
			dy = -_vy;
			y += dy * dt;
		}
		else if (_preSide == idSide::TOP)
		{
			dy = _vy;
			y += dy * dt;
		}

		if (_state == idState::DOWN) {
			//if (std::lroundf(x) >= CGame::GetInstance()->getWidth() - _width / 2)
			//{
			//	x = CGame::GetInstance()->getWidth() - _width / 2;
			//	_state = idState::UP;
			//}
			//else {
			//	dx = _vx;
			//	x += dx * dt;
			//}

			if (std::lroundf(x) <= CGame::GetInstance()->getWidth() + _width / 2)
			{
				dx = _vx;
				x += dx * dt;
				// check va cham 
				//sweptAABB(dx, dy, getBoundingBox(), _bar2, dt, resultCollision);
				//if (resultCollision == eDirection::D_RIGHT) {
				//if (checkCollision(_bar2)) {
				//	//_RPT1(0, "[INFO] %d < Pos BAR 1 %d < %d \n", _bar1.top, y, _bar1.bottom);
				//	_state = idState::UP;
				//}
				//else {
				//	dx = _vx;
				//	x += dx * dt;
				//}
			}
			else {
				// Reset
				_wait = true;
				CGame::GetInstance()->increasePointP1();
			}
		}
		else if (_state == idState::UP) {
			if (std::floorf(y) <= _height / 2 && _preSide == idSide::BOTTOM) {
				y = _height / 2;

				_side = idSide::TOP;

				_preSide = idSide::RIGHT;
				_state = idState::DOWN;
			}
			else if (std::lroundf(y) >= CGame::GetInstance()->getHeight() - _width / 2 && _preSide == idSide::TOP) {
				y = CGame::GetInstance()->getHeight() - _width / 2;

				_side = idSide::BOTTOM;

				_preSide = idSide::RIGHT;
				_state = idState::DOWN;
			}
			else {

				// check xem co dung canh phai truoc hay ko ?

				if (std::floorf(x) <= _width / 2 + _bar1.right)
				{
					if (_preSide == idSide::TOP) 	_side = idSide::BOTTOM;
					else if (_preSide == idSide::BOTTOM) _side = idSide::TOP;


					x = _width / 2 + _bar1.right;

					_side = idSide::LEFT;
					_state = idState::DOWN;

				}
				//else
				{
					dx = -_vx;
					x += dx * dt;
				}
			}
		}
	}
	break;
#pragma endregion

#pragma region TOP
	case idSide::TOP:
	{

		if (_state == idState::UP) {

			if (std::floorf(x) > ((_width / 2) + _bar1.right) && std::lroundf(x) < (CGame::GetInstance()->getWidth() - _width / 2 - (_bar2.right - _bar2.left)))
			{
				// check swept aabb
				if (_preSide == idSide::LEFT)
				{
					collisionTime = sweptAABB(dx, dy, getBoundingBox(), _bar2, dt, resultCollision);

					if (resultCollision == eDirection::D_RIGHT)
					{
						_side = idSide::RIGHT;
						_preSide = idSide::TOP;
						_state = idState::UP;

						break;
					}
				}
				else if (_preSide == idSide::RIGHT)
				{
					collisionTime = sweptAABB(dx, dy, getBoundingBox(), _bar1, dt, resultCollision);

					if (resultCollision == eDirection::D_LEFT)
					{
						_side = idSide::RIGHT;
						_preSide = idSide::TOP;
						_state = idState::UP;

						break;
					}
				}
				_RPT1(0, "[INFO] COLLISION TOP %ld \n", collisionTime);
				_RPT1(0, "[INFO] DIRECTION %d \n", resultCollision);

				dy = _vy;
				y += dy * collisionTime;
			}
			else
			{

				if (_preSide == idSide::LEFT)
				{
					_side = idSide::RIGHT;
				}
				else if (_preSide == idSide::RIGHT)
				{
					_side = idSide::LEFT;
				}

				_preSide = idSide::TOP;
				_state = idState::DOWN;
				return;

			}
		}
		else if (_state == idState::DOWN) {

			if (std::floorf(y) > _height / 2) {
				dy = _vy;
				y += dy * dt;
			}
			else {
				y = _height / 2;
				_state = idState::UP;
			}

		}

		if (_preSide == idSide::LEFT)
		{
			dx = _vx;
			x += dx * collisionTime;
		}
		else if (_preSide == idSide::RIGHT)
		{
			dx = -_vx;
			x += dx * collisionTime;
		}
	}
	break;
#pragma endregion

#pragma region LEFT
	case idSide::LEFT:
	{
		//_RPT0(0, "[OK LEFT] ====================== \n");
		if (_preSide == idSide::BOTTOM)
		{
			dy = -_vy;
			y += dy * dt;
		}
		else if (_preSide == idSide::TOP)
		{
			dy = _vy;
			y += dy * dt;
		}

		if (_state == idState::DOWN) {
			if (std::floorf(x) >= -_width / 2)
			{

				dx = -_vx;
				x += dx * dt;
				// check va cham 
				//if (checkCollision(_bar1)) {
				//	//_RPT1(0, "[INFO] %d < Pos BAR 1 %d < %d \n", _bar1.top, y, _bar1.bottom);
				//	_state = idState::UP;
				//}
				//else {
				//	dx = -_vx;
				//	x += dx * dt;
				//}
			}
			else {
				// Reset
				_wait = true;
				CGame::GetInstance()->increasePointP2();
			}
		}
		else if (_state == idState::UP) {

			if (std::floorf(y) <= _height / 2 && _preSide == idSide::BOTTOM) {
				y = _height / 2;

				_side = idSide::TOP;

				_preSide = idSide::LEFT;
				_state = idState::DOWN;
			}
			else if (std::lroundf(y) >= CGame::GetInstance()->getHeight() - _width / 2 && _preSide == idSide::TOP) {
				y = CGame::GetInstance()->getHeight() - _width / 2;

				_side = idSide::BOTTOM;

				_preSide = idSide::LEFT;
				_state = idState::DOWN;
			}
			else {
				// check xem co dung canh phai truoc hay ko ?

				if (std::lroundf(x) >= CGame::GetInstance()->getWidth() - _width / 2 - (_bar2.right - _bar2.left))
				{
					if (_preSide == idSide::TOP) 	_side = idSide::BOTTOM;
					else if (_preSide == idSide::BOTTOM) _side = idSide::TOP;
					x = CGame::GetInstance()->getWidth() - _width / 2 - (_bar2.right - _bar2.left);
					_side = idSide::RIGHT;
					_state = idState::DOWN;

				}
				else
				{
					dx = _vx;
					x += dx * dt;
				}
			}
		}
	}
	break;
#pragma endregion

	default:
		break;
	}

}

void CBall::update02(DWORD dt, RECT _bar1, RECT _bar2, float vy1)
{
	if (_wait)
	{
		/*	_waitTime += dt;
			if (_waitTime >= MAX_WAIT_TIME) {
				_wait = false;
				_waitTime = 0;
				init();
			}
			else {
				x = CGame::GetInstance()->getWidth() / 2;
				y = CGame::GetInstance()->getHeight() / 2;
			}*/
		return;
	}

	RECT rectTop;
	rectTop.top = 0;
	rectTop.bottom = 0;
	rectTop.left = 0;
	rectTop.right = CGame::GetInstance()->getWidth();

	if (y < getHeight() / 2 || y > CGame::GetInstance()->getHeight() - getHeight() / 2) {
		dy = -dy;
	}

	if (x < getWidth() / 2 || x > CGame::GetInstance()->getWidth() - getWidth() / 2) {
		dx = -dx;
	}

	x += dx * dt;
	y += dy * dt;
}