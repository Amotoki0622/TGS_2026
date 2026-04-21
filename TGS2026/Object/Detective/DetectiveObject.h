#pragma once
#include "DxLib.h"
#include "../Player/Player.h"
#include <cmath>

// オブジェクトの種類を定義
enum class DetectiveType {
    Camera,
    Light
};

class DetectiveObject {
protected:
    float x, y;
    bool detected;
    DetectiveType type; // 自分のタイプを保持する変数

public:
    DetectiveObject(float x, float y, DetectiveType type)
        : x(x), y(y), detected(false), type(type) {
    }

    virtual ~DetectiveObject() {}

    virtual void Update(const Player& player) = 0;
    virtual void Draw() const = 0;

    bool IsDetected() const { return detected; }
    // タイプを取得する関数を追加
    DetectiveType GetType() const { return type; }
};