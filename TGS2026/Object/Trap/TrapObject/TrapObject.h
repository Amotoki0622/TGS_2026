#pragma once
#include "DxLib.h"
#include "../../Player/Player.h"
#include <cmath>

// オブジェクトの種類を定義
enum class TrapType {
    Camera,
    Light,
    LegTrap
};

class TrapObject {
protected:
    float x, y;
    bool detected;
    TrapType type; // 自分のタイプを保持する変数

public:
    TrapObject(float x, float y, TrapType type)
        : x(x), y(y), detected(false), type(type) {
    }

    virtual ~TrapObject() {}

    virtual void Update(const Player& player, float delta_second) = 0;
    virtual void Draw() const = 0;

    bool IsDetected() const { return detected; }
    // タイプを取得する関数を追加
    TrapType GetType() const { return type; }
};