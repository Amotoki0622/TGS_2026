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

	//’Ç‰Á
	bool isWarping = false;

	int warpImage[3];

	int animFrame;
	int animTimer;

	float angle;

public:
	void SetPlayer(Player* p);

	Warp();

	Warp(float x, float y, float w, float h, float toX, float toY);
	~Warp();

	float GetToX() const { return toX; }
	float GetToY() const { return toY; }

	void SetPosition(float px, float py);
	void SetSize(float w, float h);
	void SetTargetPosition(float to_x, float to_y);

	//‰Šú‰»ˆ—
	void Initialize() override;
	//XVˆ—
	void Update(float delta_second) override;
	//•`‰æˆ—
	void Draw() const override;
	//I—¹ˆ—
	void Finalize() override;

	bool IsHit(int nextX, int nextY, float pW, float pH) const;
};