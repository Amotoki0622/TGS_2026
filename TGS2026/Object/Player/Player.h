// =============================================
// Playerクラス
// プレイヤーの座標・状態・描画を管理するクラス
// =============================================

#pragma once
#include "DxLib.h"
#include "../../Utility/Vector2D.h"
#include "../Wall/Wall.h"
#include "../GameObject.h"
#include "../../Utility/EffectManager/EffectManager.h"

#include <vector>

class Wall;
class EffectManager;

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


    // 現在の状態（Normal / Shadow）を返す
    State GetState() const {
        return state; 
    }

    // 手数用テスト
    int tekazu;

    void AddEffectFromObject(float x, float y) {
        // プレイヤー自身が持っている effectManager にそのまま横流しする
        effectManager.AddEffect(x, y, EffectType::Smoke, "Resource/Images/GameMain/smoke.png", 0.6f, 0.0f, false);
    }

private:
    // 位置情報
    int x;
    int y;

    // 見た目
    int radius;
    int color;

    // 画像関連
    static const int IMAGE_NUM = 3; // 分割数（例：4枚）
    int images[IMAGE_NUM];          // 分割画像を格納
    int images2[IMAGE_NUM];          // 分割画像を格納
    int currentImage;               // 現在表示している画像

    // 音源関連
    int freq;
    int moveSE;          // 移動音
    int changeStateSE;   // 状態切り替え音（Shadow化など）


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
    int motionTimer = 0; // 移動アニメーション維持用

    // =========================================
    // エフェクト用変数
    // =========================================
    EffectManager effectManager; // プレイヤー専用のエフェクトマネージャ
    int lastPx = -1;             // 1フレーム前のX座標
    int lastPy = -1;             // 1フレーム前のY座標

public:
    void Initialize();
    void Update(const float& delta_second);
    void Draw() const;

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

    // 座標取得
    void GetLocation(int& outX, int& outY) const;
    void SetPosition(float x, float y);  //プレイヤーの座標設定
    float GetTekazu();

    void Move(const std::vector<GameObject*>& objects);

private:
    void ChangeState();

    // =========================
    // アニメーション更新
    // =========================
    void UpdateAnimation(float delta_second);
};