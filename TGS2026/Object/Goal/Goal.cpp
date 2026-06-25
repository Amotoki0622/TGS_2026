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
    image_count = 0;
    LoadDivGraph("Resource/Images/Gimmick/goal_image.png", 2, 2, 1, 128, 128, goal_image);

    unlock_se = LoadSoundMem("Resource/Sounds/SE/object/key/unlock.mp3");
    goal_se = LoadSoundMem("Resource/Sounds/SE/object/kaidan/goal.m4a");
    ChangeVolumeSoundMem(130, unlock_se);
    ChangeVolumeSoundMem(140, goal_se);

}

// 更新
void Goal::Update(float delta_second)
{

}

void Goal::SetChipSize(float size)
{
    // ★重要：128pxの画像を基準として倍率を求める
    // sizeが128.0fなら 128/128 = 1.0倍（そのまま）
    // sizeが100.0fなら 100/128 = 0.78125倍（自動で一回り小さくなる）
    drawScale = size / 128.0f;
}

// 描画
void Goal::Draw() const
{
    DrawRotaGraph(
        (int)location.x,
        (int)location.y,
        drawScale,
        0.0,
        goal_image[image_count],
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

// 外部（Playerクラスなど）から檻を開けるための関数
void Goal::Open()
{
    // すでに画像が 1（解放状態）なら、処理をスルーする
    if (image_count == 1) {
        StopSoundMem(unlock_se);
        return;
    }
    image_count = 1; // 画像を普通の階段（インデックス1）に変える
    PlaySoundMem(unlock_se, DX_PLAYTYPE_BACK);
}

// 今ゴールが開いているかどうかを返す関数
bool Goal::IsOpen() const
{
    return (image_count == 1);
}

// インゲームシーンがゴールの座標を知るための関数
void Goal::GetLocation(int& outX, int& outY) const
{
    outX = (int)location.x;
    outY = (int)location.y;
}

// プレイヤーがゴールに乗ったときにSEを鳴らす関数
void Goal::PlayGoalSE()
{
    PlaySoundMem(goal_se, DX_PLAYTYPE_BACK);
}