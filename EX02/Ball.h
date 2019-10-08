#pragma once
#include "GameObject.h"
#include "Game.h"

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
	float _vx, _vy;

	int _state;

	int _side;
	int _preSide;

public:

	void setVx(float vx) { _vx = vx; }
	void setVy(float vy) { _vy = vy; }
	void setState(int state) { _state = state; }

	CBall();
	CBall(LPCWSTR texturePath) :CGameObject(texturePath) {
		_vx = 0;
		_vy = 0;

		// goc phai duoi 
		//_state = idState::DOWN;
		//_side = idSide::TOP;
		//_preSide = idSide::LEFT;

		// goc phai tren
		_state = idState::UP;
		_side = idSide::BOTTOM;
		_preSide = idSide::LEFT;

		// goc trai tren
		//_state = idState::UP;
		//_side = idSide::BOTTOM;
		//_preSide = idSide::RIGHT;

		// goc trai duoi
		//_state = idState::DOWN;
		//_side = idSide::TOP;
		//_preSide = idSide::RIGHT;
	};

	void update(DWORD, RECT, RECT);
};

