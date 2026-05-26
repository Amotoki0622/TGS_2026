#include "Goal.h"
#include "../Player/Player.h"
#include "DxLib.h"

// プレイヤーをセット
void Goal::SetPlayer(Player* p)
{
    player = p;
}

void Goal::SetSize(float w, float h)
{
    box_size.x = w;
    box_size.y = h;
}

// 初期化
void Goal::Initialize()
{
    x = 1200;
    y = 350;
    isGoal = false;

    width = 70.0f;
    height = 70.0f;

    /*this->x = this->location.x - (width / 2.0f);
    this->y = this->location.y - (height / 2.0f);*/

    isGoal = false;
}

// 更新
void Goal::Update(float delta_second)
{
    if (player == nullptr) return;

    if (CheckCollision())
    {
        isGoal = true;
    }
}

// 描画
void Goal::Draw() const
{
   /* DrawBox(
        (int)(location.x - box_size.x / 2),
        (int)(location.y - box_size.y / 2),
        (int)(location.x + box_size.x / 2),
        (int)(location.y + box_size.y / 2),
        GetColor(255, 255, 255),
        TRUE

        
    );*/

    DrawBox((int)x,(int)y,(int)(x + width),(int)(y + height), GetColor(255, 255, 255),TRUE);

    // プレイヤー座標表示
    if (player != nullptr)
    {
        int px, py;
        player->GetLocation(px, py);

        char text[64];
        sprintf_s(text, "Player : (%d , %d)", px, py);

        DrawString(10, 50, text, GetColor(255, 255, 255));
    }

    // ゴールしたら表示
    if (isGoal)
    {
        DrawString(500, 300, "GOAL!", GetColor(255, 255, 0));
    }

}

// 終了
void Goal::Finalize()
{
}

// =========================
// 当たり判定（中心座標対応）
// =========================
bool Goal::CheckCollision() const
{
    if (player == nullptr) return false;

    // プレイヤー（中心）
    Vector2D pPos = player->GetCollisionPos();
    float pW = player->GetCollisionWidth();
    float pH = player->GetCollisionHeight();

    // プレイヤーを「左上基準」に変換
    float pLeft = pPos.x - pW / 2;
    float pRight = pPos.x + pW / 2;
    float pTop = pPos.y - pH / 2;
    float pBottom = pPos.y + pH / 2;

    // ゴール（左上基準）
    float gLeft = x;
    float gRight = x + width;
    float gTop = y;
    float gBottom = y + height;

    // ゴール側も中心座標（location）とサイズ（box_size）で判定
    //float gLeft = location.x - box_size.x / 2;
    //float gRight = location.x + box_size.x / 2;
    //float gTop = location.y - box_size.y / 2;
    //float gBottom = location.y + box_size.y / 2;

    // AABB判定
    return (
        pLeft < gRight &&
        pRight > gLeft &&
        pTop < gBottom &&
        pBottom > gTop
        );
}

bool Goal::IsGoal() const
{
    return isGoal;
}