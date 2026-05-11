#pragma once
#include "DxLib.h"

class Warp
{
private:
	int x, y;
	int w, h;
	int Tx, Ty;

	int Color1;
	int Color2;

	bool byouga;

public:
	Warp(int _x, int _y, int _w, int _h, int _Tx, int _Ty,
		int _Color1, int _Color2, bool _byouga = true);

	bool CheckHit(int px, int py) const;
	void Draw() const;

	int GetTx()const { return Tx; }
	int GetTy()const { return Ty; }
	bool IsByouga() const { return byouga; }

};