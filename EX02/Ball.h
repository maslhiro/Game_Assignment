#pragma once
#include "GameObject.h"
#include "Game.h"
#include <time.h>
#include <limits>

enum idState {
	DOWN = 1,
	UP,
};

enum idSide {
	TOP = 1,
	BOTTOM,
	RIGHT,
	LEFT,
};

class CBall : public CGameObject
{
private:

	int _state;

	int _side;
	int _preSide;

	// thoi gian cho de ball chuyen dong sau khi reset state
	DWORD _waitTime;
	bool _wait;

	int resultCollision;

	float dx, dy;

public:

	void setState(int state) { _state = state; }

	CBall();
	CBall(LPCWSTR texturePath) :CGameObject(texturePath) {
		_vy = 0;

		_wait = false;

		init();
		// goc phai duoi 
		//_state = idState::DOWN;
		//_side = idSide::TOP;
		//_preSide = idSide::LEFT;

		// goc phai tren
		//_state = idState::UP;
		//_side = idSide::BOTTOM;
		//_preSide = idSide::LEFT;

		// goc trai tren
		//_state = idState::UP;
		//_side = idSide::BOTTOM;
		//_preSide = idSide::RIGHT;

		// goc trai duoi
		//_state = idState::DOWN;
		//_side = idSide::TOP;
		//_preSide = idSide::RIGHT;
	};

	void init();

	void update(DWORD, RECT, RECT);


};

