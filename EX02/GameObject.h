#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <limits>

enum eTexture
{
	TEX_0 = 0,
	TEX_1 = 1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,
	TEX_8,
	TEX_9,
	TEX_T
};

enum eDirection
{
	D_LEFT = 1,
	D_RIGHT,
	D_UP,
	D_DOWN
};

class CGameObject
{
protected:
	float x;
	float y;

	float _vx, _vy;

	int _width;
	int _height;

	D3DXVECTOR3 _origin;

	LPDIRECT3DTEXTURE9 texture;
public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void setOrigin(float x, float y) { _origin = D3DXVECTOR3(x, y, 0); }

	int getWidth() { return _width; }
	int getHeight() { return _height; }

	void setVx(float vx) { _vx = vx; }
	void setVy(float vy) { _vy = vy; }
	float getVx() { return _vx; }
	float getVy() { return _vy; }

	float getPosX() { return x; }
	float getPosY() { return y; }

	RECT getBoundingBox();

	CGameObject();
	CGameObject(LPCWSTR texturePath);

	bool checkCollision(RECT);

	RECT getSweptBroadphaseRect(float dx, float dy)
	{
		RECT current = getBoundingBox();

		current.left = dx != 0 ? current.left + dx : current.left;
		current.right = dx != 0 ? current.right + dx : current.right;
		current.top = dy != 0 ? current.top + dy : current.top;
		current.bottom = dy != 0 ? current.bottom + dy : current.bottom;

		return current;
	}

	DWORD sweptAABB(float dx, float dy, RECT _rectOther, DWORD dt, int &result)
	{
		LONG dxEntry, dxExit;
		LONG dyEntry, dyExit;


		RECT _rectObj = getSweptBroadphaseRect(dx, dy);
		//RECT _rectObj = getBoundingBox();
		//_RPT1(0, "[INFO] SWEPT : %d %d %d %d \n", _rectObj.left, _rectObj.top, _rectObj.right, _rectObj.bottom);
		//_RPT1(0, "[INFO] BOX : %d %d %d %d \n", getBoundingBox().left, getBoundingBox().top, getBoundingBox().right, getBoundingBox().bottom);

		//// tìm khoảng cách các cạnh tương ứng
		if (dx > 0.0f)
		{
			dxEntry = _rectOther.left - _rectObj.right;
			dxExit = _rectOther.right - _rectObj.left;
		}
		else
		{
			dxEntry = _rectObj.left - _rectOther.right;
			dxExit = _rectObj.right - _rectOther.left;
		}


		if (dy > 0.0f)
		{
			dyEntry = _rectOther.top - _rectObj.bottom;
			dyExit = _rectOther.bottom - _rectObj.top;
		}
		else
		{
			dyEntry = _rectObj.top - _rectOther.bottom;
			dyExit = _rectObj.bottom - _rectOther.top;
		}


		float txEntry, txExit;
		float tyEntry, tyExit;

		//// tim thoi gian va cham
		if (dx == 0.0f)
		{
			// đang đứng yên thì bằng vô cực (chia cho  0)
			txEntry = -std::numeric_limits<long>::infinity();
			txExit = std::numeric_limits<long>::infinity();
		}
		else
		{
			txEntry = dxEntry / abs(dx);
			txExit = dxExit / abs(dx);
		}

		if (dy == 0.0f)
		{
			tyEntry = -std::numeric_limits<long>::infinity();
			tyExit = std::numeric_limits<long>::infinity();
		}
		else
		{
			tyEntry = dyEntry / abs(dy);
			tyExit = dyExit / abs(dy);
		}

		// thời gian va chạm là thời gian lớn nhất của 2 trục (2 trục phải cùng tiếp xúc thì mới va chạm)
		float entryTime = max(txEntry, tyEntry);
		// thời gian hết va chạm là thời gian của 2 trục, (1 cái ra khỏi là object hết va chạm)
		float exitTime = min(txExit, tyExit);

		// kiểm tra xem có thể va chạm không, mình xét ngược lại cho nhanh
		if (entryTime > exitTime || (txEntry < 0 && tyEntry < 0) || txEntry > dt || tyEntry > dt)
		{
			result = 0;
			return dt;
		}

		// Kiem tra xem time theo chieu x hay y se dung obj truoc
		if (txEntry > tyEntry)
		{
			// va cham theo truc x
			if (dxEntry > 0.0f)
			{
				if (dx != 0.0f)	result = eDirection::D_RIGHT;
			}
			// dx co the am neu object  va cham voi other
			else
			{
				if (dx != 0.0f)	result = eDirection::D_LEFT;
			}
		}
		else
		{
			if (dyEntry > 0.0f)
			{
				if (dy != 0.0f)	result = eDirection::D_UP;
			}
			else
			{
				if (dy != 0.0f)	result = eDirection::D_DOWN;
			}
		}

		//_RPT1(0, "[INFO] ENTRY TIME : %f \n", entryTime);

		return entryTime;
	}

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