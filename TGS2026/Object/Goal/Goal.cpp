#include "Goal.h"
#include "../Player/Player.h"
#include "DxLib.h"

// プレイヤーをセット
void Goal::SetPlayer(Player* p)
{
    player = p;
}

Goal::Goal()
{
    this->location.x = 0.0f;
    this->location.y = 0.0f;
    this->box_size.x = 0.0f;
    this->box_size.y = 0.0f;
}

Goal::Goal(float x, float y, float w, float h)
{
    // 親クラス GameObject が持っている location と box_size に代入する
    this->location.x = x;
    this->location.y = y;
    this->box_size.x = w;
    this->box_size.y = h;
}

Goal::~Goal()
{

}

// 初期化
void Goal::Initialize()
{
    goal_image = LoadGraph("Resource/Images/Gimmick/goal.png");
}

// 更新
void Goal::Update(float delta_second)
{

}

// 描画
void Goal::Draw() const
{
    // 1. 画像のサイズ（横幅と縦幅）を取得する
    int img_width, img_height;
    GetGraphSize(goal_image, &img_width, &img_height);

    // 2. 中心(location)から画像のサイズ半分を引き、左上の座標を計算して描画
    DrawGraph(
        (int)(location.x - img_width / 2),
        (int)(location.y - img_height / 2),
        goal_image,
        TRUE
    );

    //DrawBox(
    //    (int)(location.x - box_size.x / 2),
    //    (int)(location.y - box_size.y / 2),
    //    (int)(location.x + box_size.x / 2),
    //    (int)(location.y + box_size.y / 2),
    //    GetColor(255, 255, 255),
    //    FALSE
    //);
}

// 終了
void Goal::Finalize()
{
}

// =========================
// 当たり判定（中心座標）
// =========================
bool Goal::IsHit(int nextX, int nextY, int width, int height) const 
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
    float gLeft = location.x - box_size.x / 2.0f;
    float gRight = location.x + box_size.x / 2.0f;
    float gTop = location.y - box_size.y / 2.0f;
    float gBottom = location.y + box_size.y / 2.0f;

    // =========================================================
    // 3. 衝突判定（AABB方式）
    // すべての条件が重なっている場合のみ true を返します
    // =========================================================
    return (
        pLeft < gRight &&
        pRight > gLeft &&
        pTop < gBottom &&
        pBottom > gTop
        );
}
