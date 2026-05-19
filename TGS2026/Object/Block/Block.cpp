#include "Block.h"
#include "../Player/Player.h"
#include "DxLib.h"
#include "../GameObject.h"

#include <cmath>

// プレイヤーをセット
void Block::SetPlayer(Player* p)
{
    player = p;
}

Block::Block(float x, float y, float w, float h)
{
    // 親クラス GameObject が持っている location と box_size に代入する
    this->location.x = x;
    this->location.y = y;
    this->box_size.x = w;
    this->box_size.y = h;
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
        (int)(location.x - box_size.x / 2),
        (int)(location.y - box_size.y / 2),
        (int)(location.x + box_size.x / 2),
        (int)(location.y + box_size.y / 2),
        GetColor(255, 0, 0),
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
bool Block::IsHit(int nextX, int nextY, int width, int height) const
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
    float bLeft = location.x - box_size.x / 2.0f;
    float bRight = location.x + box_size.x / 2.0f;
    float bTop = location.y - box_size.y / 2.0f;
    float bBottom = location.y + box_size.y / 2.0f;

    // =========================================================
    // 3. 衝突判定（AABB方式）
    // すべての条件が重なっている場合のみ true を返します
    // =========================================================
    return (
        pLeft < bRight &&
        pRight > bLeft &&
        pTop < bBottom &&
        pBottom > bTop
        );

}

void Block::Push(float moveX, float moveY, const std::vector<GameObject*>& objects) {
    // 1. 移動先の座標をあらかじめ計算する
    int nextX = (int)(location.x + moveX);
    int nextY = (int)(location.y + moveY);

    // 画面外チェック
    float halfW = box_size.x / 2.0f;
    float halfH = box_size.y / 2.0f;

    if (nextX < halfW || nextX > 1280.0f - halfW ||
        nextY < halfH || nextY > 720.0f - halfH)
    {
        return; // 画面外に出そうなら、ここで処理を中断して動かさない
    }

    // 2. 移動先に他のオブジェクトがないかチェック
    bool canMove = true;
    for (const auto& obj : objects) {
        // 自分自身（このブロック）との判定はスキップ
        if (obj == this) continue;

        // 移動先の座標、自分のサイズ(box_size)を使って衝突判定
        // わずかな計算誤差を防ぐため、サイズを少し（例: 2ピクセル）小さくして判定するのがコツ
        if (obj->IsHit(nextX, nextY, (int)box_size.x - 2, (int)box_size.y - 2)) {
            canMove = false;
            break; // 何か（壁や他のブロック）があったらループを抜ける
        }
    }

    // 3. どこにもぶつからなければ、実際に座標を更新する
    if (canMove) {
        this->location.x += moveX;
        this->location.y += moveY;
    }
}