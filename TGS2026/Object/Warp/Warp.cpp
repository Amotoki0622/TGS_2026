#include "Warp.h"
#include "../Player/Player.h"
#include "DxLib.h"

void Warp::SetPlayer(Player* p)
{
	player = p;
}

Warp::Warp()
	: toX(0.0f)
	, toY(0.0f)
	, isWarping(false)
	, player(nullptr)
	,animFrame(0)
	,animTimer(0)
	,angle(0.0f)
{
	/*this->x = 0.0f;
	this->y = 0.0f;
	this->width = 0.0f;
	this->height = 0.0f;*/

	this->location = Vector2D(0.0f, 0.0f);
	this->box_size = Vector2D(0.0f, 0.0f);
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

void Warp::SetPosition(float px, float py)
{
	this->location.x = px;
	this->location.y = py;
}

void Warp::SetSize(float w, float h)
{
	/*this->width = w;
	this->height = h;*/

	this->box_size.x = w;
	this->box_size.y = h;
}

void Warp::SetTargetPosition(float to_x, float to_y)
{
	this->toX = to_x;
	this->toY = to_y;
}

void Warp::Initialize()
{
	LoadDivGraph("Resource/Images/Gimmick/warp.png",3,3,1,512,512,warpImage);
}

void Warp::Update(float delta_second)
{
	//アニメーション
	animTimer++;

	if (animTimer >= 6)
	{
		animTimer = 0;
		animFrame++;

		if (animFrame >= 3)
		{
			animFrame = 0;
		}
	}

	//回転
	angle += 0.05f;

	if (player == nullptr)return;

	int playerX, playerY;
	player->GetLocation(playerX, playerY);

	bool hit = IsHit(playerX, playerY, player->GetCollisionWidth(), player->GetCollisionHeight());

	if (hit && !isWarping)
	{
		player->SetPosition(toX, toY);

		isWarping = true;
	}

	if (!hit)
	{
		isWarping = false;
	}
}

void Warp::Draw() const
{
	/*DrawBox(
		(int)(x - width / 2),
		(int)(y - height / 2),
		(int)(x + width / 2),
		(int)(y + height / 2),
		GetColor(0, 255, 255),
		FALSE
	);*/
	// 水色のワープ入口
	//DrawExtendGraph(
	//	(int)(x - width / 2),
	//	(int)(y - height / 2),
	//	(int)(x + width / 2),
	//	(int)(y + height / 2),
	//	warpImage,
	//	TRUE
	//);
	//DrawBox(
	//	(int)(location.x - box_size.x / 2),
	//	(int)(location.y - box_size.y / 2),
	//	(int)(location.x + box_size.x / 2),
	//	(int)(location.y + box_size.y / 2),
	//	GetColor(0, 255, 255),
	//	FALSE
	//);

	DrawRotaGraph(
		(int)location.x,
		(int)location.y,
		0.5,
		angle,
		warpImage[animFrame],
		TRUE
	);
	/*DrawBox(
		(int)(toX - width / 2),
		(int)(toY - height / 2),
		(int)(toX + width / 2),
		(int)(toY + height / 2),
		GetColor(255, 128, 0),
		FALSE
	);*/

	// オレンジのワープ出口
	//DrawExtendGraph(
	//	(int)(toX - width / 2),
	//	(int)(toY - height / 2),
	//	(int)(toX + width / 2),
	//	(int)(toY + height / 2),
	//	warpImage,
	//	TRUE
	//);
	DrawBox(
		(int)(toX - box_size.x / 2),
		(int)(toY - box_size.y / 2),
		(int)(toX + box_size.x / 2),
		(int)(toY + box_size.y / 2),
		GetColor(255, 128, 0),
		FALSE
	);
}

void Warp::Finalize()
{
	for (int i = 0; i < 3; i++)
	{
		if (warpImage[i] != -1)
		{
			DeleteGraph(warpImage[i]);
			warpImage[i] = -1;
		}
	}
}

bool Warp::IsHit(int nextX, int nextY, float pW, float pH) const
{
	float pLeft = nextX - pW / 2;
	float pRight = nextX + pW / 2;
	float pTop = nextY - pH / 2;
	float pBottom = nextY + pH / 2;

	/*float wLeft = x - width / 2;
	float wRight = x + width / 2;
	float wTop = y - height / 2;
	float wBottom = y + height / 2;*/

	// ワープますの矩形
	float wLeft = location.x - box_size.x / 2;
	float wRight = location.x + box_size.x / 2;
	float wTop = location.y - box_size.y / 2;
	float wBottom = location.y + box_size.y / 2;

	return (
		pLeft < wRight &&
		pRight > wLeft &&
		pTop < wBottom &&
		pBottom > wTop
		);
}