#include "Goal.h"
#include "DxLib.h"

// プレイヤーをセット
void Goal::SetPlayer(Player* p)
{
    player = p;
}

void Goal::Initialize()
{

}

void Goal::Update()
{

}

void Goal::Draw() const
{
	DrawBox(240, 530, 320, 610, GetColor(255, 255, 255), true);

    // =========================
// プレイヤー座標表示
// =========================
    if (player != nullptr)
    {
        int px, py;
        player->GetLocation(px, py);

        char text[64];
        sprintf_s(text, "Player : (%d , %d)", px, py);

        DrawString(10, 50, text, GetColor(255, 255, 255));
    }
}

void Goal::Finalize()
{

}