#include "Key.h"
#include "../Player/Player.h"
#include "DxLib.h"
#include <cmath>

Key::Key()
    : targetPlayer(nullptr)
    , isPickedUp(false)
    , angle(0.0f)
{
}

Key::Key(float x, float y, Player* p)
{
    this->location.x = x;
    this->location.y = y;
    Initialize();

    // 当たり判定のサイズ
    // this->box_size.x = 64.0f;
    // this->box_size.y = 64.0f;
    
    this->isPickedUp = false;
    this->targetPlayer = p;
    this->angle = 0.0f; // この角度を全演出に共通して使います
}

Key::~Key()
{
    this->Finalize();
}

void Key::Initialize() 
{
    // isPickedUp = false;
    // angle = 0.0f;

    // 鍵のサイズを設定
    this->box_size.x = 64.0f;
    this->box_size.y = 64.0f;

    key_image = LoadGraph("Resource/Images/Gimmick/Key.png");
}

void Key::Update(float delta_second)
{
    if (targetPlayer == nullptr) return;

    if (!isPickedUp)
    {
        // -------------------------------------------------------------
        // 【状態1】拾われる前：その場でフワフワ（変更なし）
        // -------------------------------------------------------------
        int px, py;
        targetPlayer->GetLocation(px, py);
        float pw = targetPlayer->GetCollisionWidth();
        float ph = targetPlayer->GetCollisionHeight();

        float halfW = box_size.x * 0.5f;
        float halfH = box_size.y * 0.5f;
        float left = location.x - halfW;
        float right = location.x + halfW;
        float top = location.y - halfH;
        float bottom = location.y + halfH;

        float pLeft = (float)px - pw * 0.5f;
        float pRight = (float)px + pw * 0.5f;
        float pTop = (float)py - ph * 0.5f;
        float pBottom = (float)py + ph * 0.5f;

        if (left < pRight && right > pLeft && top < pBottom && bottom > pTop)
        {
            isPickedUp = true; // 重なったら取得！
        }

        // 演出用の角度を更新（拾う前と同じゆったりしたテンポ）
        angle += 2.0f * delta_second;
    }
    else
    {
        // -------------------------------------------------------------
        // 【状態2】拾われた後：しっかり離れてフワフワ浮きながら追従
        // -------------------------------------------------------------
        int px, py;
        targetPlayer->GetLocation(px, py);
        targetPlayer->SetHasKey(true);  // 鍵の取得通知処理

        // 1. 💡 角度は拾う前と全く同じテンポ（2.0f）で更新し続ける！
        // これで、拾う前後でフワフワのリズムがシームレスに繋がります
        angle += 2.0f * delta_second;

        // 2. 💡 向きに合わせて目標地点を左右に切り替える（被り防止）
        float offsetX = -40.0f * drawScale; // 基本はプレイヤーの左（後ろ）
        if (location.x > (float)px)
        {
            // プレイヤーより右にいる（＝左に移動中）なら目標を「右（+50px）」に
            offsetX = 40.0f * drawScale;
        }

        float targetX = (float)px + offsetX;
        float targetY = (float)py + (10.0f * drawScale); // 💡 プレイヤーの背中あたりの高さにする

        // 3. 鍵と目標点の現在の距離を計算
        float dx = targetX - location.x;
        float dy = targetY - location.y;
        float distance = sqrtf(dx * dx + dy * dy);

        // 💡 近づかない最低距離を思い切って「80ピクセル」まで拡大！
        // これでキャラクターとの間にしっかりとした「後ろの距離」が生まれます
        float keepDistance = 80.0f * drawScale;

        if (distance > keepDistance)
        {
            // ストッパーより離れているときだけ、少しゆっくり（0.06f）近づく
            // 💡 数値を落としたことで、プレイヤーが動くと自然と引き離されて遅れてついてきます
            location.x += dx * 0.06f;
            location.y += dy * 0.06f;
        }
    }
}

void Key::SetChipSize(float size)
{
    // 128.0fの時を1.0（基準）として、現在のマスサイズに合わせる倍率を計算
    drawScale = size / 128.0f;

    // 当たり判定のサイズもマスの大きさに連動（128pxのとき64pxなら、常にマスの半分のサイズにする）
    this->box_size.x = size * 0.5f;
    this->box_size.y = size * 0.5f;
}

// -----------------------------------------------------------------
// 【浮遊感の肝】描画関数
// -----------------------------------------------------------------
void Key::Draw() const
{
    unsigned int color = GetColor(255, 220, 0); // 黄色

    if (!isPickedUp)
    {
        // --- 拾われる前 ---
        // ベースとなる位置（location.y）に、サイン波で上下の揺れ（±8.0f）を加える
        float bounceY = location.y + sinf(angle * 2.0f) * (8.0f * drawScale);

        DrawRotaGraph((int)location.x, (int)bounceY, drawScale, 0.0f, key_image, TRUE);
    }
    else
    {
        // --- ⭕ 拾われた後 ---
        // 💡【ここが一番重要！】
        // 追従しているベース位置（location.y）に、拾う前と全く同じ幅（* 8.0f）と速度（angle * 2.0f）
        // のサイン波を上乗せして描画します！！！
        //
        // これにより、しっかり離れた位置に追従しつつ、立ち止まったときも
        // その場で「フワフワ…ゆらゆら…」と上下に揺れ続けてくれます。
        float bounceY = location.y + sinf(angle * 2.0f) * (8.0f * drawScale);

        DrawRotaGraph((int)location.x, (int)bounceY, drawScale, 0.0f, key_image, TRUE);

    }
}

bool Key::IsHit(int nextX, int nextY, int width, int height) const { return false; }

void Key::SetPlayer(Player* p)
{
    targetPlayer = p;
}

void Key::Finalize() {}