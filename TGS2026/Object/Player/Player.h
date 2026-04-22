#pragma once
#include "DxLib.h"
#include "../../Utility/Vector2D.h"

// =============================================
// Playerクラス
// プレイヤーの座標・状態・描画を管理するクラス
// =============================================
class Player
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


    // 現在の状態（Normal / Shadow）を返す
    State GetState() const {
        return state; 
    }

    // 手数用テスト
    int tekazu;

private:
    // -----------------------------------------
    // 位置情報
    // -----------------------------------------
    int x;
    int y;

    // -----------------------------------------
    // 見た目
    // -----------------------------------------
    int radius;
    int color;

    // =========================
    // 画像関連（追加）
    // =========================
    static const int IMAGE_NUM = 2; // 分割数（例：4枚）
    int images[IMAGE_NUM];          // 分割画像を格納
    int images2[IMAGE_NUM];          // 分割画像を格納
    int currentImage;               // 現在表示している画像

    // -----------------------------------------
    // 移動関連
    // -----------------------------------------
    int speed;

    // -----------------------------------------
    // 状態
    // -----------------------------------------
    State state;

    // =========================================
    // 矩形コリジョン情報
    // =========================================
    float collisionWidth;
    float collisionHeight;

public:
    void Initialize();
    void Update();
    void Draw() const;

    // =========================================
    // コリジョン用取得関数
    // =========================================
    Vector2D GetCollisionPos() const;
    float GetCollisionWidth() const;
    float GetCollisionHeight() const;

    // 座標取得
    void GetLocation(int& outX, int& outY) const;
    void SetPosition(float x, float y);  //プレイヤーの座標設定



private:
    void Move();
    void ChangeState();
    void UpdateColor();

    // =========================
    // アニメーション更新（追加）
    // =========================
    void UpdateAnimation();
};