#include "Block.h"
#include "../Player/Player.h"
#include "DxLib.h"

#include <cmath>

// プレイヤーをセット
void Block::SetPlayer(Player* p)
{
    player = p;
}

Block::Block(float x, float y, float w, float h)
{
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;
}

Block::~Block()
{

}

// 初期化
void Block::Initialize()
{

}

// 更新
void Block::Update(float delta_second)
{

}

// 描画
void Block::Draw() const
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
void Block::Finalize()
{
}

// =========================
// 当たり判定（中心座標対応）
// =========================
bool Block::IsHit(int nextX, int nextY, float pW, float pH) const
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
