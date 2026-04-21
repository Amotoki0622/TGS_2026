#include "Wall.h"
#include "../Player/Player.h"
#include "DxLib.h"

#include <cmath>

// プレイヤーをセット
void Wall::SetPlayer(Player* p)
{
    player = p;
}

// 初期化
void Wall::Initialize()
{
    x = 700;
    y = 530;

    width = 80;
    height = 80;
}

// 更新
void Wall::Update(float delta_second)
{
    if (player == nullptr) return;

    if (CheckCollision())
    {
        Vector2D pPos = player->GetCollisionPos();
        float pW = player->GetCollisionWidth();
        float pH = player->GetCollisionHeight();

        // プレイヤー矩形
        float pLeft = pPos.x - pW / 2;
        float pRight = pPos.x + pW / 2;
        float pTop = pPos.y - pH / 2;
        float pBottom = pPos.y + pH / 2;

        // 壁矩形
        float wLeft = x;
        float wRight = x + width;
        float wTop = y;
        float wBottom = y + height;

        // 押し戻し量を計算
        float overlapX1 = wRight - pLeft;   // 左から当たった
        float overlapX2 = pRight - wLeft;   // 右から当たった
        float overlapY1 = wBottom - pTop;   // 上から当たった
        float overlapY2 = pBottom - wTop;   // 下から当たった

        // 最小の押し戻しを選ぶ
        float minX = (overlapX1 < overlapX2) ? overlapX1 : -overlapX2;
        float minY = (overlapY1 < overlapY2) ? overlapY1 : -overlapY2;

        // 小さい方で押し戻す（軸分離）
        if (abs(minX) < abs(minY))
        {
            // X方向
            player->SetPosition(pPos.x + minX, pPos.y);
        }
        else
        {
            // Y方向
            player->SetPosition(pPos.x, pPos.y + minY);
        }
    }
}

// 描画
void Wall::Draw() const
{
    DrawBox(
        (int)x,
        (int)y,
        (int)(x + width),
        (int)(y + height),
        GetColor(0, 0, 255),
        TRUE


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

    // AABB判定
    return (
        pLeft < gRight &&
        pRight > gLeft &&
        pTop < gBottom &&
        pBottom > gTop
        );
}
