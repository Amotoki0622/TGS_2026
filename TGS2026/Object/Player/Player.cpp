#include "Player.h"
#include "../Block/Block.h"
#include "../Goal/Goal.h"
#include "../Key/Key.h"
#include "../Warp/Warp.h"
#include "../Trap/Cam/Cam.h"
#include "../Trap/SpikeTrap/SpikeTrap.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/EffectManager/EffectManager.h"
#include "../../Stage/StageData.h"

#include <typeinfo>
#include <iostream>

// =========================
// 初期化処理
// =========================
void Player::Initialize()
{
    radius = 64;
    speed = 128;

    state = State::Normal;
    currentImage = 0;

    collisionWidth = radius;
    collisionHeight = radius;

    revers = TRUE;

    canMove = true;
    hasKey = false;

    step_se = LoadSoundMem("Resource/Sounds/SE/object/player/player_step.mp3");
    ChangeVolumeSoundMem(110, step_se);

    // 変身音の読み込み・設定
    changeStateSE = LoadSoundMem("Resource/Sounds/SE/object/player/change.mp3");
    freq = GetFrequencySoundMem(changeStateSE); // 元の周波数を取得
    ChangeVolumeSoundMem(70, changeStateSE);
    SetFrequencySoundMem((int)(freq * 2.0f), changeStateSE); // 倍速

    kickImage = LoadGraph(
        "Resource/Images/Player/player_kick.png"
    );

    LoadDivGraph(
        "Resource/Images/Player/player_idol.png",
        2,      // 2分割
        2, 1,   // 横2 縦1
        621,    // 1枚の幅
        670,   // 1枚の高さ
        idleImages
    );

    // シャドウ状態の画像読み込み
    int result = LoadDivGraph(
        "Resource/Images/Player/shadow2.png",
        2, 2, 1, 768, 1024, images2
    );

    if (result == -1) {
        printfDx("画像読み込み失敗\n");
    }
}

// =========================
// 更新処理
// =========================
void Player::Update(const float& delta_second)
{
    ChangeState();
    UpdateAnimation(delta_second);
    effectManager.Update(delta_second);
}

// =========================
// 移動処理
// =========================
void Player::Move(const std::vector<GameObject*>& objects) 
{
    InputManager* input = InputManager::GetInstance();

    if (state == State::Normal)
    {
        // Bボタン（蹴る）でブロックを動かす
        if (input->GetButtonInputState(XINPUT_BUTTON_B) == eInputState::ePress ||
            input->GetKeyInputState(KEY_INPUT_SPACE) == eInputState::ePress)
        {
            isKicking = true;
            kickTimer = 0.3f;  // キック画像の描画しかん

            // --- 1. 蹴る方向（ベクトル）を決定する ---
            float kickX = 0.0f;
            float kickY = 0.0f;

            if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::eHold ||
                input->GetKeyInputState(KEY_INPUT_LEFT) == eInputState::eHold) {
                kickX = -chipSize;
            }
            else if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::eHold ||
                input->GetKeyInputState(KEY_INPUT_RIGHT) == eInputState::eHold) {
                kickX = chipSize;
            }
            else if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_UP) == eInputState::eHold ||
                input->GetKeyInputState(KEY_INPUT_UP) == eInputState::eHold) {
                kickY = -chipSize;
            }
            else if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::eHold ||
                input->GetKeyInputState(KEY_INPUT_DOWN) == eInputState::eHold) {
                kickY = chipSize;
            }
            else {
                // 現在の向きを基準に蹴る
                if (dir == Direction::Left)  kickX = -chipSize;
                if (dir == Direction::Right) kickX = chipSize;
                if (dir == Direction::Up)    kickY = -chipSize;
                if (dir == Direction::Down)  kickY = chipSize;
            }

            // --- 2. 判定を出す位置（check座標）を計算 ---
            float halfSize = chipSize / 2.0f;
            float checkX = (float)x + (kickX != 0 ? (kickX > 0 ? halfSize : -halfSize) : 0.0f);
            float checkY = (float)y + (kickY != 0 ? (kickY > 0 ? halfSize : -halfSize) : 0.0f);


            // --- 3. 衝突判定とPushの実行 ---
            for (auto& obj : objects) {
                if (obj == this) continue;

                float hitSize = chipSize * 0.9f;
                if (obj->IsHit((int)checkX, (int)checkY, hitSize, hitSize))
                {
                    Block* targetBlock = dynamic_cast<Block*>(obj);
                    if (targetBlock != nullptr)
                    {
                        targetBlock->Push(kickX, kickY, objects);
                        return;     // 成功したら終了
                    }
                }
            }
        }
    }

    int moveX = 0;
    int moveY = 0;

    // =========================
    // プレイヤー入力
    // =========================
    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::ePress ||
        input->GetKeyInputState(KEY_INPUT_LEFT) == eInputState::ePress)
    {
        moveX = -speed;
        revers = FALSE;
        dir = Direction::Left;
    }
    else if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::ePress ||
        input->GetKeyInputState(KEY_INPUT_RIGHT) == eInputState::ePress)
    {
        moveX = speed;
        revers = TRUE;
        dir = Direction::Right;
    }
    else if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_UP) == eInputState::ePress ||
        input->GetKeyInputState(KEY_INPUT_UP) == eInputState::ePress)
    {
        moveY = -speed;
        dir = Direction::Up;
    }
    else if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::ePress ||
        input->GetKeyInputState(KEY_INPUT_DOWN) == eInputState::ePress)
    {
        moveY = speed;
        dir = Direction::Down;
    }

    // 入力がなければ何もしない
    if (moveX == 0 && moveY == 0)
    {
        return;
    }

    // =========================
    // 次の座標を計算
    // =========================
    int nextX = x + moveX;
    int nextY = y + moveY;

    // =========================
    // 画面外チェック
    // =========================
    if (nextX < radius || nextX > 1280 - radius ||
        nextY < radius || nextY > 720 - radius)
    {
        return;
    }

    // =========================
    // オブジェクトとの当たり判定
    // =========================
    canMove = true;

    for (const auto& obj : objects)
    {
        if (obj == this) continue;

        if (obj->IsHit(nextX, nextY, collisionWidth, collisionHeight))
        {
            auto keyObj = dynamic_cast<Key*>(obj);
            if (keyObj != nullptr)
            {
                if (state == State::Normal)
                {
                    if (hasKey) continue;
                    continue; // 鍵の上は通り抜け可能
                }
            }

            auto goalObj = dynamic_cast<Goal*>(obj);
            if (goalObj != nullptr)
            {
                // 鍵を持っていて、通常状態のとき
                if (hasKey && state == State::Normal)
                {
                    // ゴールがまだ閉まっている（檻付き）なら開ける
                    if (!goalObj->IsOpen())
                    {
                        goalObj->Open();   // ゴールの檻を開ける（画像が変わる）
                        canMove = false;   // 手前で足を止める
                        break;
                    }
                    else
                    {
                        // すでにゴールが開いているなら、移動を許可してループを抜ける！
                        canMove = true;    // 進むことを確定させる
                        break;             // 他の判定に邪魔されないようにここでループを抜ける
                    }
                }
                else
                {
                    // 鍵がない、または影状態ならただの壁
                    canMove = false;
                    break;
                }
            }

            auto WarpObj = dynamic_cast<Warp*>(obj);
            if (WarpObj != nullptr)
            {
                if (this->state == State::Shadow) continue; // 影なら通過（ワープ可能）
                else { canMove = false; break; }
            }

            auto spikeObj = dynamic_cast<SpikeTrap*>(obj);
            if (spikeObj != nullptr) continue;

            auto camObj = dynamic_cast<Cam*>(obj);
            if (camObj != nullptr)
            {
                canMove = false;
                break;
            }
            canMove = false;
            break;
        }
    }

    // =========================
    // 移動確定処理
    // =========================
    if (canMove)
    {
        float oldX = (float)x;
        float oldY = (float)y;

        float spawnX = oldX;
        float spawnY = oldY;
        float angle = 0.0f;
        bool isReversedX = false;

        std::string effectImagePath = "Resource/Images/Effect/Smoke3.png";

        if (moveX != 0)
        {
            spawnY = oldY + 20.0f;
            if (moveX < 0) isReversedX = false;
            else isReversedX = true;
            effectImagePath = "Resource/Images/Effect/Smoke.png";
        }
        else if (moveY != 0)
        {
            if (moveY < 0) {
                spawnY = oldY + 10.0f;
                angle = 0.0f;
            }
            else {
                spawnY = oldY - 10.0f;
                angle = 0.0f;
            }
            effectImagePath = "Resource/Images/Effect/Smoke3.png";
        }

        // 待機アニメ解除
        idleTimer = 0.0f;
        isIdleAnimation = false;
        idleFrame = 0;

        // 座標更新と手数減算
        x = nextX;
        y = nextY;

        canWarp = true;
        tekazu--;

        PlaySoundMem(step_se, DX_PLAYTYPE_BACK);

        // 移動煙エフェクト発生
        effectManager.AddEffect(spawnX, spawnY, EffectType::Smoke, effectImagePath, 0.2f, angle, isReversedX);
    }
}

// =========================
// 状態切り替え
// =========================
void Player::ChangeState()
{
    InputManager* input = InputManager::GetInstance();

    if (input->GetButtonInputState(XINPUT_BUTTON_LEFT_SHOULDER) == eInputState::ePress ||
        input->GetButtonInputState(XINPUT_BUTTON_RIGHT_SHOULDER) == eInputState::ePress ||
        input->GetKeyInputState(KEY_INPUT_RETURN) == eInputState::ePress)
    {
        PlaySoundMem(changeStateSE, DX_PLAYTYPE_BACK);
        state = (state == State::Normal) ? State::Shadow : State::Normal;
        tekazu--;
    }
}

// =========================
// アニメーション処理
// =========================
void Player::UpdateAnimation(float delta_second)
{
    InputManager* input = InputManager::GetInstance();

    idleTimer += delta_second;

    if (idleTimer >= 1.0f)
    {
        isIdleAnimation = true;
    }

    if (isIdleAnimation)
    {
        static float frameTimer = 0.0f;

        frameTimer += delta_second;

        if (frameTimer >= 0.4f)
        {
            idleFrame = (idleFrame + 1) % 2;
            frameTimer = 0.0f;
        }
    }

    if (isKicking)
    {
        kickTimer -= delta_second;

        if (kickTimer <= 0.0f)
        {
            kickTimer = 0.0f;
            isKicking = false;
        }
    }

    if (state == State::Normal)
    {
        bool isPressed = (input->GetButtonInputState(XINPUT_BUTTON_B) == eInputState::ePress ||
            input->GetKeyInputState(KEY_INPUT_SPACE) == eInputState::ePress);

        if (isPressed && actionTimer <= 0.0f)
        {
            actionTimer = 0.25f; // 蹴り発動
            tekazu--;
        }

        if (actionTimer > 0.0f)
        {
            currentImage = 1;
            actionTimer -= delta_second;
        }
        else
        {
            currentImage = 0;
            actionTimer = 0.0f;
        }
    }
}

void Player::SetChipSize(float size)
{
    chipSize = size;
    speed = (int)size;
    collisionWidth = size * 0.9f;
    collisionHeight = size * 0.9f;
    radius = (int)(size / 2.0f);

    drawScale = 0.2f * (size / 128.0f);
}

// =========================
// 描画処理
// =========================
void Player::Draw() const
{
    if (state == State::Normal)
    {
        int handle = idleImages[0];

        // キック最優先
        if (isKicking)
        {
            handle = kickImage;
        }
        // 待機
        else if (isIdleAnimation)
        {
            handle = idleImages[idleFrame];
        }

        DrawRotaGraph(
            x,
            y,
            drawScale,
            0.0,
            handle,
            TRUE,
            revers
        );
    }
    else
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
        if (images2[currentImage] != -1)
        {
            float shadowScale = drawScale * 0.85f;
            DrawRotaGraph(x, y, shadowScale, 0.0, images2[currentImage], TRUE, revers);
        }
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    // プレイヤーの向きを示す三角の描画
    if (state == State::Normal) // 通常状態のときだけ出す場合
    {
        float offset = chipSize * 0.8f; // プレイヤー中心からの距離
        float size = 24.0f;             // 三角形のサイズ

        int rColor = GetColor(255, 0, 0);

        // 三角形の3つの頂点座標
        int x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0;

        switch (dir)
        {
        case Direction::Left:
            x1 = x - offset;          y1 = y;
            x2 = x - offset + size;   y2 = y - size / 2;
            x3 = x - offset + size;   y3 = y + size / 2;
            break;
        case Direction::Right:
            x1 = x + offset;          y1 = y;
            x2 = x + offset - size;   y2 = y - size / 2;
            x3 = x + offset - size;   y3 = y + size / 2;
            break;
        case Direction::Up:
            x1 = x;                   y1 = y - offset;
            x2 = x - size / 2;        y2 = y - offset + size;
            x3 = x + size / 2;        y3 = y - offset + size;
            break;
        case Direction::Down:
            x1 = x;                   y1 = y + offset;
            x2 = x - size / 2;        y2 = y + offset - size;
            x3 = x + size / 2;        y3 = y + offset - size;
            break;
        }

        // 塗りつぶしの三角形を描画（アンチエイリアス版を使うときれいです）
        DrawTriangleAA((float)x1, (float)y1, (float)x2, (float)y2, (float)x3, (float)y3, rColor, TRUE);
    }

    effectManager.Draw();
}

// =========================
// 各種ゲッター・セッター
// =========================
Vector2D Player::GetCollisionPos() const { return Vector2D{ (float)x, (float)y }; }
float Player::GetCollisionWidth() const { return collisionWidth; }
float Player::GetCollisionHeight() const { return collisionHeight; }

void Player::GetLocation(int& outX, int& outY) const
{
    outX = x;
    outY = y;
}

void Player::SetPosition(float newX, float newY)
{
    x = newX;
    y = newY;
}

void Player::SetTekazu(int maxLimit) { tekazu = maxLimit; }
int Player::GetTekazu() const { return tekazu; }