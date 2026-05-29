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

Block::Block()
{
    this->location.x = 0.0f;
    this->location.y = 0.0f;
    this->box_size.x = 0.0f;
    this->box_size.y = 0.0f;
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
    block_image = LoadGraph("Resource/Images/Gimmick/barrel.png");
}

// 更新
void Block::Update(float delta_second)
{
    effectManager.Update(delta_second);
}

// 描画
void Block::Draw() const
{

    // ブロックから発生した煙エフェクトをここで描画する！
    effectManager.Draw();

    // 1. 画像のサイズ（横幅と縦幅）を取得する
    int img_width, img_height;
    GetGraphSize(block_image, &img_width, &img_height);

    // 2. 中心(location)から画像のサイズ半分を引き、左上の座標を計算して描画
    DrawGraph(
        (int)(location.x - img_width / 2),
        (int)(location.y - img_height / 2),
        block_image,
        TRUE
    );

    //DrawBox(
    //    (int)(location.x - box_size.x / 2),
    //    (int)(location.y - box_size.y / 2),
    //    (int)(location.x + box_size.x / 2),
    //    (int)(location.y + box_size.y / 2),
    //    GetColor(255, 0, 0),
    //    FALSE
    //);
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
    if (canMove)
    {
        // Blockの元の座標を float で取得
        float oldX = this->location.x;
        float oldY = this->location.y;

        // エフェクト用の変数準備
        float spawnX = oldX;
        float spawnY = oldY;
        float angle = 0.0f;
        bool isReversedX = false;

        // 画像パスを保存する変数を1つ用意する
        std::string effectImagePath = "Resource/Images/Effect/Smoke3.png"; // デフォルト

        // --- 移動方向別の「正確な位置調整」と「見た目」の割り出し ---
        if (moveX != 0)
        {
            // 【横移動】
            spawnY = oldY + 20.0f; // 横移動のときは一律で少し下にずらす

            if (moveX < 0) {
                // 左移動：プレイヤーの画像基準が「左移動＝反転なし」なら、煙も合わせる
                isReversedX = false;
            }
            else {
                // 右移動：右を向くので反転させる
                isReversedX = true;
            }

            // 横移動のときは、横専用の煙画像をセット！
            effectImagePath = "Resource/Images/Effect/Smoke.png";
        }
        else if (moveY != 0)
        {
            // 【縦移動】
            if (moveY < 0)
            {
                // 上に動くとき
                spawnX = oldX;
                spawnY = oldY + (this->box_size.y / 2.0f) - 20.0f;
                angle = 0.0f;
            }
            else
            {
                // 下に動くとき
                spawnX = oldX;
                spawnY = oldY - (this->box_size.y / 2.0f) + 20.0f;
                angle = 0.0f;
            }

            // 【縦移動】画像が縦向きに回転するため、上下でずらす方向を変える
            if (moveY < 0)
            {
                // 上移動：一歩前の足元なので、少し「下」にずらして縦向き（270度）にする
                spawnY = oldY + 10.0f;
                angle = 0.0f;
            }
            else
            {
                // 下移動：一歩前の足元なので、少し「上」にずらして縦向き（90度）にする
                spawnY = oldY - 10.0f;
                angle = 0.0f;
            }

            // 縦移動のときは、縦専用の煙画像をセット！
            effectImagePath = "Resource/Images/Effect/Smoke3.png";
        }

        // 実際に木箱の座標を更新
        this->location.x = (float)nextX;
        this->location.y = (float)nextY;

        // 最後の引数に、上で切り替えた「effectImagePath」をそのまま渡す！
        effectManager.AddEffect(spawnX, spawnY, EffectType::Smoke, effectImagePath, 0.2f, angle, isReversedX);
    }
}