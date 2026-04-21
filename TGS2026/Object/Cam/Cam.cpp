#include "Cam.h"

Cam::Cam(float x, float y, float angle, float range, float fov)
    : DetectiveObject(x, y, DetectiveType::Camera), angle(angle), range(range), fov(fov) {
}

void Cam::Update(const Player& player) {
    detected = false;

    // 影状態なら検知処理を行わない
    if (player.GetState() == Player::State::Shadow) return;

    int px, py;
    player.GetLocation(px, py);

    // 1. 距離判定
    float dist = sqrtf(powf(px - x, 2) + powf(py - y, 2));
    if (dist < range) {
        // 2. 角度判定
        float targetAngle = atan2f(py - y, px - x);
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