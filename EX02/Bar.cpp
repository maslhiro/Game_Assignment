#include "Bar.h"

CBar::CBar()
{
}

void CBar::Update(DWORD dt)
{
	if (_id == 1)	this->inputHandler_Player01();
	else if (_id == 2) this->inputHandler_Player021();
}

RECT CBar::getBoundingBox()
{
	RECT _rect;
	// thanh bar sat trai
	// origin = null
	if (_id == 1)
	{
		_rect.left = x;
		//_rect.left = x + _width;
		_rect.top = y;
		_rect.bottom = y + _height;
		_rect.right = x + _width;
	}
	if (_id == 2)
	{
		_rect.right = x;
		_rect.top = y;
		_rect.bottom = y + _height;
		//_rect.right = x - _width;
		_rect.left = x - _width;
	}
	return _rect;
}


void CBar::inputHandler_Player01()
{
	RECT r = getBoundingBox();
	//_RPT1(0, "[INFO] BOUDING BOX : %d, %d , %d , %d \n", r.left, r.top, r.right, r.bottom);

	// A
	if (CGame::GetInstance()->getKeyDown() == 0x41) {
		if (y < CGame::GetInstance()->getHeight() - _height)
		{
			dy = _vy;
			y += _vy;
		}
	}
	// D
	else if (CGame::GetInstance()->getKeyDown() == 0x44) {
		if (y > 0) {
			dy = -_vy;
			y -= _vy;
		}

	}
}

void CBar::inputHandler_Player02()
{
	//RECT r = getBoundingBox();
	//_RPT1(0, "[INFO] BOUDING BOX : %d, %d , %d , %d \n", r.left, r.top, r.right, r.bottom);
	// A
	if (CGame::GetInstance()->distancePosMouse() > 1) {
		if (y < CGame::GetInstance()->getHeight() - _height)
		{
			dy = _vy;
			y += _vy;
		}
	}
	// D
	else if (CGame::GetInstance()->distancePosMouse() < -1) {
		if (y > 0) {
			dy = -_vy;
			y -= _vy;
		}

	}
}

void CBar::inputHandler_Player021()
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);

	if (cursorPos.y > y + getHeight() / 2) {
		if (y < CGame::GetInstance()->getHeight() - _height)
		{
			dy = _vy;
			y += _vy;
		}
	}
	// D
	else if (cursorPos.y < y + getHeight() / 2) {
		if (y > 0) {
			dy = -_vy;
			y -= _vy;
		}

	}
	else dy = 0;
}
