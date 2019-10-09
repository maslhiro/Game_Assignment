#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include "Game.h"
#include "GameObject.h"
#include <unordered_map> 

class HubPoint
{
private:
	// Vi tri
	int _x, _y;

	// mang texture
	std::unordered_map<int, LPDIRECT3DTEXTURE9> _listTexture;

	// diem cua 2 player
	int _point01 = 0, _point02 = 0;



public:
	HubPoint();
	~HubPoint();

	void setPos(int x, int y)
	{
		_x = x;
		_y = y;
	}

	void add(int, LPCWSTR);

	void render();

	void update(DWORD);
};

