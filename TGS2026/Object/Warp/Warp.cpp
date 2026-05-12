#include "Warp.h"
#include "../Player/Player.h"
#include "DxLib.h"

#include<cmath>

void Warp::SetPlayer(Player* p)
{
	player = p;
}

Warp::Warp(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
}

Warp::~Warp()
{
}

void Warp::Initialize()
{
}

void Warp::Update(float delta_second)
{
}

void Warp::Draw() const
{
}

void Warp::Finalize()
{
}

bool Warp::IsHit(int nextX, int nextY, float pW, float pH) const
{
}
//Warp::Warp(int _x, int _y, int _w, int _h, int _Tx, int _Ty, int _Color1, int _Color2, bool _byouga)
//	:x(_x), y(_y), w(_w), h(_h), Tx(_Tx), Ty(_Ty), Color1(_Color1), Color2(Color2), byouga(_byouga)
//{
//}
//
//bool Warp::CheckHit(int px, int py)const
//{
//	return (px > x && px <x + w &&
//		py > y && py < y + h);
//}
//
//void Warp::Draw() const
//{
//	if (!byouga)return;
//
//	DrawBox(x, y, x + w, y + h, Color1, FALSE);
//
//	int size = 200;
//	DrawBox(Tx, Ty,
//		Tx + w, Ty + h,
//		Color2, FALSE);
//}