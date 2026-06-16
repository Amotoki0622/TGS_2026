#pragma once
#include "../TrapObject/TrapObject.h"

class Cam : public TrapObject {
private:
    float range;
    float angle;
    float fov;

    float baseAngle;   // 初期設定の向き（基準）
    float swingAngle;  // 左右にどれだけ振るか（例：0.5f）
    float swingSpeed;  // 振るスピード
    float swingTimer;  // 時間経過を記録する用

    float currentSwingAngle; // 現在の振れ角（-swingAngle ～ swingAngle）
    float pauseTimer;        // 停止時間を計る用
    bool isPausing;          // 今止まっているか
    int direction;           // 1 なら右、-1 なら左へ動く
    int cam_image;

    bool IsHit(int nextX, int nextY, int width, int height) const override;

public:
    Cam();
    // 引数の型を float に統一しておくと計算ミスが減ります
    Cam(float x, float y, float angle, float range, float fov);

    // 追加点
    void SetUpCamera(float angle, float range, float fov)
    {
        this->baseAngle = angle;
        this->angle = angle;
        this->range = range;
        this->fov = fov;
    }

    void Initialize();
    void Update(const Player& player, float delta_second) override;
    void Draw() const override;
};