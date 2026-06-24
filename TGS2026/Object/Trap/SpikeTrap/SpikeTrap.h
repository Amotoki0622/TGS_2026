#pragma once
#include "../TrapObject/TrapObject.h"
#include <vector>

class Player;


// ダメージエフェクト用の構造体
struct textEffect {
    float x, y;    // 座標
    float timer;   // 残り時間
    float alpha;   // 透明度
};

// 1つの血しぶきの「粒」のデータ
struct BloodParticle {
    float x, y;       // 現在の座標
    float vx, vy;     // 飛び散る速度と方向（ベクトル）
    float size;       // 粒の大きさ
    float timer;      // 残り寿命
    float alpha;      // 透明度
    unsigned int color; // 色（影用と通常用を分けるため）
};

class SpikeTrap : public TrapObject {
private:
    int spikeImage;     // トゲの画像
    float radius;       // 当たり判定の半径

    // フォント
    int font[3];

    // 生成したエフェクトを保存するリスト
    std::vector<textEffect> textEffects;

    // 血の「粒」をたくさん管理するリスト
    std::vector<BloodParticle> bloodParticles;

    int normal_spike_se;  // 棘床を踏んだ時のSE（通常状態）
    int shadow_spike_se;  // 棘床を踏んだ時のSE（影状態）

public:
    SpikeTrap();
    SpikeTrap(float x, float y, float radius);

    // 初期化処理
    void Initialize() override;

    // 毎フレーム、純粋に「今プレイヤーが乗っているか」だけをチェックする
    void Update(const Player& player, float delta_second) override;
    void Draw() const override;

    // 外部からの当たり判定のサイズを設定できる関数
    void SetRadius(float r);
};