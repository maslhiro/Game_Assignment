#pragma once
#include "GameObject.h"
#include <limits>

float sweptAABB(float vx, float vy, RECT _rectObj, RECT _rectOther)
{
	float dxEntry, dxExit;
	float dyEntry, dyExit;

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
		dyEntry = _rectOther.bottom - _rectObj.top;
		dyExit = _rectOther.top - _rectObj.bottom;
	}
	else
	{
		dyEntry = _rectObj.bottom - _rectOther.top;
		dyExit = _rectObj.top - _rectOther.bottom;
	}


	float txEntry, txExit;
	float tyEntry, tyExit;

	//// tim thoi gian va cham
	if (vx <= 0.0f)
	{
		// đang đứng yên thì bằng vô cực (chia cho  0)
		txEntry = -std::numeric_limits<float>::infinity();
		txExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		txEntry = dxEntry / vx;
		txExit = dxExit / vx;
	}

	if (vy <= 0.0f)
	{
		tyEntry = -std::numeric_limits<float>::infinity();
		tyExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		tyEntry = dyEntry / vy;
		tyExit = dyExit / vy;
	}

	// thời gian va chạm là thời gian lớn nhất của 2 trục (2 trục phải cùng tiếp xúc thì mới va chạm)
	float entryTime = max(txEntry, tyEntry);
	// thời gian hết va chạm là thời gian của 2 trục, (1 cái ra khỏi là object hết va chạm)
	float exitTime = min(txExit, tyExit);
	//_RPT1(0, "[INFO] ENTRY TIME : %f \n", entryTime);
	//_RPT1(0, "[INFO] TXENTRY : %f \n", txEntry);
	//_RPT1(0, "[INFO] TYENTRY : %f \n", tyEntry);
	//_RPT1(0, "[INFO] TXEXIT : %f \n", txExit);
	//_RPT1(0, "[INFO] TYEXIT : %f \n", tyExit);
	// kiểm tra xem có thể va chạm không, mình xét ngược lại cho nhanh
	if (entryTime > exitTime || (txEntry < 0.0f && tyEntry < 0.0f) || txEntry > 1.0f || tyEntry > 1.0f)
	{
		return 1.0f;
	}

	return entryTime;
}


