#pragma once

#include <Windows.h>
#include <d3dx9.h>

class CGameObject
{
protected:
	float x;
	float y;

	int _width;
	int _height;

	D3DXVECTOR3 _origin;

	LPDIRECT3DTEXTURE9 texture;
public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void setOrigin(float x, float y) { _origin = D3DXVECTOR3(x, y, 0); }

	int getWidth() { return _width; }
	int getHeight() { return _height; }

	RECT getBoundingBox();

	CGameObject();
	CGameObject(LPCWSTR texturePath);

	bool checkCollision(RECT);
	void Update(DWORD dt);
	void Render();

	~CGameObject();
};

class CMario : public CGameObject
{
public:
	CMario(LPCWSTR texturePath) :CGameObject(texturePath) {};
	void Update(DWORD dt);
};