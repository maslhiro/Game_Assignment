#pragma once
#include "GameObject.h"
#include "Game.h"

class CBar :
	public CGameObject
{
private:
	float _vy, dy;
	int _id;
public:
	CBar();
	CBar(LPCWSTR texturePath) :CGameObject(texturePath) {
		_vy = 0;
	};

	void setVy(float vy) { _vy = vy; }
	void setId(int id) { _id = id; }

	float getDY() { return dy; }

	void Update(DWORD);

	RECT getBoundingBox();

	void inputHandler_Player01();

	void inputHandler_Player02();

	void inputHandler_Player021();
};

