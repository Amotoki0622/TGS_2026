#include "Light.h"

Light::Light(float x, float y, float radius)
    : DetectiveObject(x, y, DetectiveType::Light), radius(radius) {
}

void Light::Update(const Player& player) {
    detected = false;

    if (player.GetState() == Player::State::Normal) return;

    int px, py;
    player.GetLocation(px, py);

    // 円形範囲判定
    float dist = sqrtf(powf(px - x, 2) + powf(py - y, 2));
    if (dist < radius) {
        detected = true;
    }
}

void Light::Draw() const {
    unsigned int color = detected ? GetColor(255, 0, 0) : GetColor(255, 255, 255);

    // 照明本体（白い点）
    DrawCircle((int)x, (int)y, 5, GetColor(255, 255, 255), TRUE);

    // 範囲の可視化（透過円）
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
    DrawCircle((int)x, (int)y, (int)radius, color, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // 範囲の縁
    DrawCircle((int)x, (int)y, (int)radius, color, FALSE);
}