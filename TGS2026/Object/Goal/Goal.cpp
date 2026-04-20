#include "Goal.h"
#include "../Player/Player.h"
#include "../../Utility/CollisionManager.h"
#include "DxLib.h"

void Goal::Initialize()
{
	pos = { 280, 570 };   // íÜêSç¿ïW
	width = 80;
	height = 80;
}

void Goal::Update()
{

}

void Goal::Draw() const
{
	DrawBox(240, 530, 320, 610, GetColor(255, 255, 255), true);
	//DrawFormatString(10, 100, GetColor(255, 255, 255), "x: %.2f y: %.2f", player.x, player.y);
}

void Goal::Finalize()
{

}