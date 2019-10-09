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

	// xac dinh van toc vx
	float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
	_vx = 1.5f + scale * 0.5f;

	// random huong chuyen dong


	int flag = 1 + rand() % (4);
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
	case idSide::BOTTOM:
	{
		if (_preSide == idSide::LEFT)
		{
			x += _vx;
		}
		else if (_preSide == idSide::RIGHT)
		{
			x -= _vx;
		}

		if (_state == idState::DOWN) {
			if (y < CGame::GetInstance()->getHeight() - _width / 2) {
				y += _vy;
			}
			else {
				_state = idState::UP;
			}
		}
		else if (_state == idState::UP) {
			if (x >= (CGame::GetInstance()->getWidth() - _width / 2 - (_bar2.right - _bar2.left)) && _preSide == idSide::LEFT) {
				_side = idSide::RIGHT;

				_preSide = idSide::BOTTOM;
				_state = idState::DOWN;
			}
			else if (x <= (_width / 2) + _bar1.right && _preSide == idSide::RIGHT) {
				_side = idSide::LEFT;

				_preSide = idSide::BOTTOM;
				_state = idState::DOWN;
			}
			else {
				y -= _vy;

			}
		}
	}
	break;

	case idSide::RIGHT:
	{
		if (_preSide == idSide::BOTTOM)
		{
			y -= _vy;
		}
		else if (_preSide == idSide::TOP)
		{
			y += _vy;
		}

		if (_state == idState::DOWN) {
			//_RPT1(0, "[INFO] CHECK BAR 2 : %d \n", checkCollision(_bar2));
			if (x >= CGame::GetInstance()->getWidth() - _width / 2 - (_bar2.right - _bar2.left)) {
				if (checkCollision(_bar2)) {
					_RPT1(0, "[INFO] %d < Pos BAR 2 %d < %d \n", _bar2.top, y, _bar2.bottom);
					_state = idState::UP;
				}
				// neu ko dung bar thi chay luon
				else {
					// cho qua bong chay qua man hinh moi reset
					if (x <= CGame::GetInstance()->getWidth() + _width / 2)
					{
						x += _vx;
					}
					else {
						// Reset
						_wait = true;
						CGame::GetInstance()->increasePointP1();
					}
				}
			}
			else {
				x += _vx;
			}
		}
		else if (_state == idState::UP) {

			// kiem tra dung ben trai
			if (x < _width / 2 + (_bar1.right)) {
				// giu nguyen preside vi co 2 TH 1 TOP va 1 BOTTOM
				//_preSide = idSide::BOTTOM;
				_state = idState::DOWN;
				_side = idSide::LEFT;

			}
			// dung top
			else if (y < _width / 2 && _preSide == idSide::BOTTOM) {
				_side = idSide::TOP;

				_preSide = idSide::RIGHT;
				_state = idState::DOWN;
			}
			// dung bottom
			else if (y > CGame::GetInstance()->getHeight() - _width / 2 && _preSide == idSide::TOP) {
				_side = idSide::BOTTOM;

				_preSide = idSide::RIGHT;
				_state = idState::DOWN;
			}
			else {
				x -= _vx;

			}
		}
	}
	break;

	case idSide::TOP:
	{
		if (_preSide == idSide::LEFT)
		{
			x += _vx;
		}
		else if (_preSide == idSide::RIGHT)
		{
			x -= _vx;
		}

		if (_state == idState::UP) {
			if (x > _width / 2 - _bar1.right) {
				y -= _vy;
			}
			else {
				_state = idState::DOWN;
			}
		}
		else if (_state == idState::DOWN) {
			if (x < _width / 2 + _bar1.right && _preSide == idSide::RIGHT) {
				_side = idSide::LEFT;

				_preSide = idSide::TOP;
				_state = idState::DOWN;
			}
			else if (x > CGame::GetInstance()->getWidth() - _width / 2 - (_bar2.right - _bar2.left) && _preSide == idSide::LEFT) {
				_side = idSide::RIGHT;

				_preSide = idSide::TOP;
				_state = idState::DOWN;
			}
			else {
				y += _vy;

			}
		}
	}
	break;

	case idSide::LEFT:
	{
		if (_preSide == idSide::BOTTOM)
		{
			y -= _vy;
		}
		else if (_preSide == idSide::TOP)
		{
			y += _vy;
		}

		if (_state == idState::DOWN) {
			if (x <= _width / 2 + _bar1.right) {
				if (checkCollision(_bar1)) {
					_RPT1(0, "[INFO] %d < Pos BAR 1 %d < %d \n", _bar1.top, y, _bar1.bottom);
					_state = idState::UP;
				}
				// neu ko dung bar thi chay luon
				else {
					// cho qua bong chay qua man hinh moi reset
					if (x >= -_width / 2)
					{
						x -= _vx;
					}
					else {
						// Reset
						_wait = true;
						CGame::GetInstance()->increasePointP2();
					}
				}
			}
			else {
				x -= _vx;
			}
		}
		else if (_state == idState::UP) {
			// kiem tra dung ben phai
			if (x >= CGame::GetInstance()->getWidth() - _width / 2 - (_bar2.right - _bar2.left)) {
				//_preSide = idSide::TOP;
				_state = idState::DOWN;
				_side = idSide::RIGHT;

			}
			else if (y < _height / 2 && _preSide == idSide::BOTTOM) {
				_side = idSide::TOP;

				_preSide = idSide::LEFT;
				_state = idState::DOWN;
			}
			else if (y > CGame::GetInstance()->getHeight() - _width / 2 && _preSide == idSide::TOP) {
				_side = idSide::BOTTOM;

				_preSide = idSide::LEFT;
				_state = idState::DOWN;
			}
			else {
				x += _vx;

			}
		}
	}
	break;

	default:
		break;
	}

}
