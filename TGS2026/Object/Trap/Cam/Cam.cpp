#include "Cam.h"

// コンストラクタ
Cam::Cam(float x, float y, float angle, float range, float fov)
    : TrapObject(x, y, TrapType::Camera) // 親のコンストラクタ呼び出し
{
    this->range = range;
    this->baseAngle = angle; // 基準
    this->angle = angle;     // 現在
    this->fov = fov;

    this->currentSwingAngle = 0.0f;
    this->swingAngle = 0.8f;  // 振れ幅（約45度）
    this->swingSpeed = 0.4f;  // ここを小さくするとゆっくり動く
    this->direction = 1;
    this->isPausing = false;
    this->pauseTimer = 0.0f;
}

// 引数に delta_second を追加しましょう
void Cam::Update(const Player& player, float delta_second) {
    detected = false;

    detected = false;

    if (isPausing) {
        // --- 待機中の処理 ---
        pauseTimer += delta_second;
        if (pauseTimer >= 1.0f) { // 1.0f = 1秒待機（ここを調整）
            isPausing = false;
            pauseTimer = 0.0f;
            direction *= -1; // 向きを反転させる
        }
    }
    else {
        // --- 移動中の処理 ---
        // swingSpeed を 0.5f くらいにするとゆっくりになります
        currentSwingAngle += swingSpeed * delta_second * direction;

        // 端まで到達したかチェック
        if (fabsf(currentSwingAngle) >= swingAngle) {
            isPausing = true;  // 停止モードへ
            // 行き過ぎを補正
            currentSwingAngle = (direction > 0) ? swingAngle : -swingAngle;
        }
    }

    // 最終的な angle を決定
    angle = baseAngle + currentSwingAngle;


    // 影状態なら検知処理を行わない
    if (player.GetState() == Player::State::Shadow) return;

    int px, py;
    player.GetLocation(px, py);

    // 1. 距離判定
    float dist = sqrtf(powf((float)px - x, 2) + powf((float)py - y, 2));
    if (dist < range) {
        // 2. 角度判定（ここで使う angle は、上で計算された最新の角度になります）
        float targetAngle = atan2f((float)py - y, (float)px - x);
        float diffAngle = targetAngle - angle;

        // 角度の差を -PI ～ PI の範囲に補正
        while (diffAngle > DX_PI_F)  diffAngle -= DX_PI_F * 2;
        while (diffAngle < -DX_PI_F) diffAngle += DX_PI_F * 2;

        if (fabsf(diffAngle) < fov / 2.0f) {
            detected = true;
        }
    }
}

void Cam::Draw() const {
    unsigned int color = detected ? GetColor(255, 0, 0) : GetColor(255, 255, 0);

    // カメラ本体（青い円で代用）
    DrawCircle((int)x, (int)y, 10, GetColor(0, 0, 255), TRUE);

    // 視界の可視化（扇形）
    // 多数の線を描画して範囲を埋める
    for (float a = angle - fov / 2; a <= angle + fov / 2; a += 0.05f) {
        DrawLine((int)x, (int)y, (int)(x + cosf(a) * range), (int)(y + sinf(a) * range), color);
    }
}