#include "Warp.h"
#include "../Player/Player.h"
#include "DxLib.h"

void Warp::SetPlayer(Player* p)
{
	player = p;
}

Warp::Warp(float x, float y, float w, float h, float toX, float toY)
{
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
	this->toX = toX;
	this->toY = toY;

}

Warp::~Warp()
{
}

void Warp::Initialize()
{
}

void Warp::Update(float delta_second)
{
	//if (player == nullptr)return;

	//if (CheckCollision())
	//{
	//	player->SetLoction();
	//}
}

void Warp::Draw() const
{
	DrawBox(
		(int)x,
		(int)y,
		(int)(x + width),
		(int)(y + height),
		GetColor(0, 255, 255),
		FALSE
	);

	DrawBox(
		(int)toX,
		(int)toY,
		(int)(toX + width),
		(int)(toY + height),
		GetColor(255,128,0),
		FALSE
	);
}

void Warp::Finalize()
{
}

bool Warp::IsHit(int nextX, int nextY, float pW, float pH) const
{
	float pLeft = nextX - pW / 2;
	float pRight = nextX + pW / 2;
	float pTop = nextY - pH / 2;
	float pBottom = nextY + pH / 2;

	float wLeft = x - width / 2;
	float wRight = x + width / 2;
	float wTop = y - height / 2;
	float wBottom = y + height / 2;

	return (
		pLeft < wRight &&
		pRight > wLeft &&
		pTop < wBottom &&
		pBottom > wTop
		);
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