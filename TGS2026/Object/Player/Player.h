// =============================================
// Playerクラス
// プレイヤーの座標・状態・描画を管理するクラス
// =============================================

#pragma once
#include "DxLib.h"
#include "../../Utility/Vector2D.h"
#include "../GameObject.h"
#include "../../Utility/EffectManager/EffectManager.h"

#include <vector>

class Player : public GameObject
{
public:
    // -----------------------------------------
    // プレイヤーの状態
    // -----------------------------------------
    enum class State
    {
        Normal,
        Shadow
    };

    enum class Direction {
        Left,
        Right,
        Up,
        Down
    };

private:
    Direction dir = Direction::Right;

public:
    // 現在の状態（Normal / Shadow）を返す
    State GetState() const { return state; }

    // 手数
    int tekazu;

    void AddEffectFromObject(float x, float y) {
        effectManager.AddEffect(x, y, EffectType::Smoke, "Resource/Images/GameMain/smoke.png", 0.6f, 0.0f, false);
    }

private:

    // 位置情報
    int x;
    int y;

    bool canMove;

    // 見た目
    int radius;
    int color;

    // 画像関連
    static const int IMAGE_NUM = 3;
    int images[IMAGE_NUM];
    int images2[IMAGE_NUM];
    int currentImage;
    int idleImages[2];
    int shadowIdleImages[2];
    int kickImage;

    float idleTimer = 0.0f;
    bool isIdleAnimation = false;
    int idleFrame = 0;

    float kickTimer = 0.0f;
    bool isKicking = false;

    // 音源関連
    int freq;
    int moveSE;          // 移動音
    int changeStateSE;   // 状態切り替え音
    int step_se;  // プレイヤーの足音

    // 移動関連
    int speed;

    // 状態
    State state;

    // 矩形コリジョン情報
    float collisionWidth;
    float collisionHeight;

    // 画像反転
    bool revers = TRUE;

    float actionTimer = 0.0f; // アクション画像の維持タイマー
    int motionTimer = 0;       // 移動アニメーション維持用

    // =========================================
    // エフェクト用変数
    // =========================================
    EffectManager effectManager;
    int lastPx = -1;
    int lastPy = -1;

    bool hasKey;         // 鍵フラグ
    bool canWarp = true; // ワープフラグ

public:
    void Initialize();
    void Update(const float& delta_second);
    void Draw() const;
    void Move(const std::vector<GameObject*>& objects);

    // 手数を1減らす関数
    void DecreaseMoveCount() {
        if (tekazu > 0) {
            tekazu--;
        }
    }

    // =========================================
    // コリジョン用取得関数
    // =========================================
    Vector2D GetCollisionPos() const;
    float GetCollisionWidth() const;
    float GetCollisionHeight() const;

    // 座標取得・設定
    void GetLocation(int& outX, int& outY) const;
    void SetPosition(float x, float y);

    // 手数関連の処理
    int GetTekazu() const;
    void SetTekazu(int maxLimit);

private:
    void ChangeState();
    void UpdateAnimation(float delta_second);

public:

    void SetHasKey(bool flag) { hasKey = flag; }

    bool CanWarp() const { return canWarp; }
    void SetCanWarp(bool flag) { canWarp = flag; }

private:
    float chipSize = 128.0f;
    float drawScale = 128.0f;

public:
    void SetChipSize(float size);
};