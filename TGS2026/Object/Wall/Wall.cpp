#include "Wall.h"
#include "../Player/Player.h"
#include "DxLib.h"

#include <cmath>

// プレイヤーをセット
void Wall::SetPlayer(Player* p)
{
    player = p;
}

Wall::Wall(float x, float y, float w, float h)
{
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;
}

Wall::~Wall()
{

}

// 初期化
void Wall::Initialize()
{
    //x = 700;
    //y = 530;

    //width = 80;
    //height = 80;
}

// 更新
void Wall::Update(float delta_second)
{
    if (player == nullptr) return;

    width = player->GetCollisionWidth();
    height = player->GetCollisionHeight();

    if (!CheckCollision()) return;

    Vector2D pPos = player->GetCollisionPos();
    float pW = player->GetCollisionWidth();
    float pH = player->GetCollisionHeight();

    float pLeft = pPos.x - pW / 2;
    float pRight = pPos.x + pW / 2;
    float pTop = pPos.y - pH / 2;
    float pBottom = pPos.y + pH / 2;

    float wLeft = x - width / 2;
    float wRight = x + width / 2;
    float wTop = y - height / 2;
    float wBottom = y + height / 2;

    // 各方向のめり込み量
    float overlapLeft = pRight - wLeft;
    float overlapRight = wRight - pLeft;
    float overlapTop = pBottom - wTop;
    float overlapBottom = wBottom - pTop;

    // 最小の押し戻し量を探す
    float minOverlap = overlapLeft;
    int direction = 0; // 0:左,1:右,2:上,3:下

    if (overlapRight < minOverlap)
    {
        minOverlap = overlapRight;
        direction = 1;
    }
    if (overlapTop < minOverlap)
    {
        minOverlap = overlapTop;
        direction = 2;
    }
    if (overlapBottom < minOverlap)
    {
        minOverlap = overlapBottom;
        direction = 3;
    }

    // 押し戻し
    switch (direction)
    {
    case 0: // 左から当たった
        player->SetPosition(wLeft - pW / 2, pPos.y);
        break;

    case 1: // 右から当たった
        player->SetPosition(wRight + pW / 2, pPos.y);
        break;

    case 2: // 上から当たった
        player->SetPosition(pPos.x, wTop - pH / 2);
        break;

    case 3: // 下から当たった
        player->SetPosition(pPos.x, wBottom + pH / 2);
        break;
    }
}

// 描画
void Wall::Draw() const
{
    DrawBox(
        (int)(x - width / 2),
        (int)(y - height / 2),
        (int)(x + width / 2),
        (int)(y + height / 2),
        GetColor(0, 0, 255),
        FALSE
    );

}

// 終了
void Wall::Finalize()
{
}

// =========================
// 当たり判定（中心座標対応）
// =========================
bool Wall::CheckCollision() const
{
    if (player == nullptr) return false;

    Vector2D pPos = player->GetCollisionPos();
    float pW = player->GetCollisionWidth();
    float pH = player->GetCollisionHeight();

    float pLeft = pPos.x - pW / 2;
    float pRight = pPos.x + pW / 2;
    float pTop = pPos.y - pH / 2;
    float pBottom = pPos.y + pH / 2;

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
