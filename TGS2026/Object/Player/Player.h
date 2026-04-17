#pragma once
#include "DxLib.h"

// ===============================
// Playerクラス
// ===============================
class Player
{
private:
    int x, y;          // 座標
    int radius;        // 半径
    int color;         // 色
    int speed;         // 移動速度

public:
    // 初期化
    void Initialize();

    // 更新
    void Update();

    // 描画
    void Draw() const;
};