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
    block_image = LoadGraph("Resource/Images/Gimmick/wall.png");
    block_se = LoadSoundMem("Resource/Sounds/SE/object/move_object/move_object.mp3");
    ChangeVolumeSoundMem(120, block_se);
}

// 更新
void Block::Update(float delta_second)
{
    effectManager.Update(delta_second);
}

void Block::SetChipSize(float size)
{
    // ★重要：128pxの画像を基準として倍率を求める
    // sizeが128.0fなら 128/128 = 1.0倍（そのまま）
    // sizeが100.0fなら 100/128 = 0.78125倍（自動で一回り小さくなる）
    drawScale = size / 128.0f;
}

// 描画
void Block::Draw() const
{

    // ブロックから発生した煙エフェクトをここで描画する！
    effectManager.Draw();

    if (block_image != -1)
    {
        DrawRotaGraph(
            (int)location.x,
            (int)location.y,
            drawScale,
            0.0,
            block_image,
            TRUE
        );
    }

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

// =========================================================
// ブロックを押し出す処理
// =========================================================
bool Block::Push(float moveX, float moveY, const std::vector<GameObject*>& objects) {

    // 1. 【floatで計算】移動先の座標を float のまま正確に計算する
    float nextX_f = location.x + moveX;
    float nextY_f = location.y + moveY;

    // 画面外チェック用のサイズ半分
    float halfW = box_size.x / 2.0f;
    float halfH = box_size.y / 2.0f;

    // 画面外に出そうなら、ここで処理を中断して false（移動失敗）を返す
    if (nextX_f < halfW || nextX_f > 1280.0f - halfW ||
        nextY_f < halfH || nextY_f > 720.0f - halfH)
    {
        return false;
    }

    // 2. 移動先に他のオブジェクトがないかチェック
    for (const auto& obj : objects) {
        // 自分自身（このブロック）との判定はスキップ
        if (obj == this) continue;

        // ★ IsHit を呼ぶ瞬間に (int) にキャストして渡す
        // 計算のわずかな誤差を防ぐため、サイズを少し（2ピクセル）小さくして判定
        if (obj->IsHit((int)nextX_f, (int)nextY_f, (int)box_size.x - 2, (int)box_size.y - 2)) {

            // 壁や他の木箱など、ブロックの移動を遮るものがあったので移動失敗！
            return false;
        }
    }

    // 3. 【移動確定】どこにもぶつからなかったので、実際に動かす処理に入る
    // Blockの元の座標を float で保持（エフェクト用）
    float oldX = this->location.x;
    float oldY = this->location.y;

    // エフェクト用の変数準備
    float spawnX = oldX;
    float spawnY = oldY;
    float angle = 0.0f;
    bool isReversedX = false;

    std::string effectImagePath = "Resource/Images/Effect/Smoke3.png"; // デフォルト

    // --- 移動方向別の「正確な位置調整」と「見た目」の割り出し ---
    if (moveX != 0.0f)
    {
        // 【横移動】
        spawnY = oldY + 20.0f; // 横移動のときは一律で少し下にずらす

        if (moveX < 0.0f) {
            isReversedX = false;
        }
        else {
            isReversedX = true;
        }
        effectImagePath = "Resource/Images/Effect/Smoke.png";
    }
    else if (moveY != 0.0f)
    {
        // 【縦移動】
        if (moveY < 0.0f)
        {
            // 上に動くとき
            spawnX = oldX;
            spawnY = oldY + (this->box_size.y / 2.0f) - 20.0f;
        }
        else
        {
            // 下に動くとき
            spawnX = oldX;
            spawnY = oldY - (this->box_size.y / 2.0f) + 20.0f;
        }

        if (moveY < 0.0f)
        {
            spawnY = oldY + 10.0f;
        }
        else
        {
            spawnY = oldY - 10.0f;
        }
        effectImagePath = "Resource/Images/Effect/Smoke3.png";
    }

    // 実際に木箱の座標を float で更新！
    this->location.x = nextX_f;
    this->location.y = nextY_f;

    // 蹴られたときにSEを流す
    PlaySoundMem(block_se, DX_PLAYTYPE_BACK);

    // エフェクトを発生
    effectManager.AddEffect(spawnX, spawnY, EffectType::Smoke, effectImagePath, 0.2f, angle, isReversedX);

    // ★無事に動かせたので true（移動成功）をプレイヤーに返す！
    return true;
}