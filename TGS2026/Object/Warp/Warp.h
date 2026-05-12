#pragma once
#include "../../Utility/Vector2D.h"
#include "../GameObject.h"

class Player;

class Warp : public GameObject
{
private:
	float x, y;
	float width, height;
	float toX, toY;

	Player* player;

public:
	void SetPlayer(Player* p);

	Warp(float x, float y, float w, float h, float toX, float toY);
	~Warp();

	//‰Šú‰»ˆ—
	void Initialize() override;
	//XVˆ—
	void Update(float delta_second) override;
	//•`‰æˆ—
	void Draw() const override;
	//I—¹ˆ—
	void Finalize() override;

	bool CheckCollision() const;
};
//#include "DxLib.h"
//
//class Warp
//{
//private:
//	int x, y;
//	int w, h;
//	int Tx, Ty;
//
//	int Color1;
//	int Color2;
//
//	bool byouga;
//
//public:
//	Warp(int _x, int _y, int _w, int _h, int _Tx, int _Ty,
//		int _Color1, int _Color2, bool _byouga = true);
//
//	bool CheckHit(int px, int py) const;
//	void Draw() const;
//
//	int GetTx()const { return Tx; }
//	int GetTy()const { return Ty; }
//	bool IsByouga() const { return byouga; }
//
//};