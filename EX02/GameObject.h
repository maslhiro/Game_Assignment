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

	DWORD sweptAABB(float vx, float vy, RECT _rectObj, RECT _rectOther, DWORD dt)
	{
		LONG dxEntry, dxExit;
		LONG dyEntry, dyExit;

		//_RPT1(0, "[INFO] VX : %f \n", vx);
		//_RPT1(0, "[INFO] VY : %f \n", vy);


		// kiem tra 2 hinh da  chong len nhau
		//bool check = checkCollision(_rectOther);
		//if (check) return 1000.f;

		//// tìm khoảng cách các cạnh tương ứng
		if (vx > 0.0f)
		{
			dxEntry = _rectOther.left - _rectObj.right;
			dxExit = _rectOther.right - _rectObj.left;
		}
		else
		{
			dxEntry = _rectObj.left - _rectOther.right;
			dxExit = _rectObj.right - _rectOther.left;
		}


		if (vy > 0.0f)
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
		if (vx == 0.0f)
		{
			// đang đứng yên thì bằng vô cực (chia cho  0)
			txEntry = -std::numeric_limits<long>::infinity();
			txExit = std::numeric_limits<long>::infinity();
		}
		else if (vx > 0.0f)
		{
			txEntry = dxEntry / vx;
			txExit = dxExit / vx;
		}
		else if (vx < 0.0f)
		{
			txEntry = dxEntry / -vx;
			txExit = dxExit / -vx;
		}

		if (vy == 0.0f)
		{
			tyEntry = -std::numeric_limits<long>::infinity();
			tyExit = std::numeric_limits<long>::infinity();
		}
		else if (vy > 0.0f)
		{
			tyEntry = dyEntry / vy;
			tyExit = dyExit / vy;
		}
		else if (vy < 0.0f)
		{
			tyEntry = dyEntry / -vy;
			tyExit = dyExit / -vy;
		}

		// thời gian va chạm là thời gian lớn nhất của 2 trục (2 trục phải cùng tiếp xúc thì mới va chạm)
		float entryTime = max(txEntry, tyEntry);
		// thời gian hết va chạm là thời gian của 2 trục, (1 cái ra khỏi là object hết va chạm)
		float exitTime = min(txExit, tyExit);
		//_RPT1(0, "[INFO] ENTRY TIME : %f \n", entryTime);
		//_RPT1(0, "[INFO] TXENTRY : %ld \n", txEntry);
		//_RPT1(0, "[INFO] TYENTRY : %ld \n", tyEntry);
		//_RPT1(0, "[INFO] TXEXIT : %f \n", txExit);
		//_RPT1(0, "[INFO] TYEXIT : %f \n", tyExit);

		// kiểm tra xem có thể va chạm không, mình xét ngược lại cho nhanh
		if (entryTime > exitTime || (txEntry < 0 && tyEntry < 0) || txEntry > dt || tyEntry > dt)
		{
			return dt;
		}

		/*if (txEntry > tyEntry)
		{
			if (dxEntry < 0.0f)
			{
				normalx = 1.0f;
				normaly = 0.0f;
			}
			else
			{
				normalx = -1.0f;
				normaly = 0.0f;
			}
		}
		else
		{
			if (dyEntry < 0.0f)
			{
				normalx = 0.0f;
				normaly = 1.0f;
			}
			else
			{
				normalx = 0.0f;
				normaly = -1.0f;
			}
		}*/


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