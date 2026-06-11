#include "Wall.h"
#include "../Player/Player.h"
#include "DxLib.h"

#include <cmath>

// プレイヤーをセット
void Wall::SetPlayer(Player* p)
{
    player = p;
}

Wall::Wall()
    : player(nullptr)
{
    location = Vector2D(2.0f, 2.0f);
    box_size = Vector2D(2.0f, 2.0f);
}

Wall::Wall(float x, float y, float w, float h) {
    // 親クラス GameObject が持っている location と box_size に代入する
    this->location.x = x;
    this->location.y = y;
    this->box_size.x = w;
    this->box_size.y = h;
}

Wall::~Wall()
{

}

void Wall::SetSize(float w, float h)
{
    box_size.x = w;
    box_size.y = h;
}

// 初期化
void Wall::Initialize()
{
    wall_image = LoadGraph("Resource/Images/Gimmick/wall_image_130x125.png");
}

// 更新
void Wall::Update(float delta_second)
{
    
}

void Wall::SetChipSize(float size)
{
    // ★重要：128pxの画像を基準として倍率を求める
    // sizeが128.0fなら 128/128 = 1.0倍（そのまま）
    // sizeが100.0fなら 100/128 = 0.78125倍（自動で一回り小さくなる）
    drawScale = size / 128.0f;
}

// 描画
void Wall::Draw() const
{
    if (wall_image != -1)
    {
        // 引数: (中心X, 中心Y, 拡大率, 回転角度, グラフィックハンドル, 透けフラグ)
        // location.x と location.y（マスの中心座標）をそのまま渡すだけでピッタリ真ん中に描画されます。
        DrawRotaGraph(
            (int)location.x,
            (int)location.y,
            drawScale,
            0.0,
            wall_image,
            TRUE
        );
    }


    DrawBox(
        (int)(location.x - box_size.x / 2),
        (int)(location.y - box_size.y / 2),
        (int)(location.x + box_size.x / 2),
        (int)(location.y + box_size.y / 2),
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
bool Wall::IsHit(int nextX, int nextY, int width, int height) const
{
    // =========================================================
    // 1. プレイヤー（移動先）の四隅を計算
    // 引数で渡された nextX, nextY, width, height を使用します
    // =========================================================
// 判定を0.5ピクセル分だけ内側にする例
    float pLeft = (float)nextX - (float)width / 2.0f + 0.5f;
    float pRight = (float)nextX + (float)width / 2.0f - 0.5f;
    float pTop = (float)nextY - (float)height / 2.0f + 0.5f;
    float pBottom = (float)nextY + (float)height / 2.0f - 0.5f;

    // =========================================================
    // 2. この壁自体の四隅を計算
    // GameObject から継承した location と box_size を使用します
    // =========================================================
    float wLeft = location.x - box_size.x / 2.0f;
    float wRight = location.x + box_size.x / 2.0f;
    float wTop = location.y - box_size.y / 2.0f;
    float wBottom = location.y + box_size.y / 2.0f;

    // =========================================================
    // 3. 衝突判定（AABB方式）
    // すべての条件が重なっている場合のみ true を返します
    // =========================================================
    return (
        pLeft < wRight &&
        pRight > wLeft &&
        pTop < wBottom &&
        pBottom > wTop
        );
}
