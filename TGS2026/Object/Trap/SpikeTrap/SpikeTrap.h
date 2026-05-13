#pragma once
#include "../TrapObject/TrapObject.h"

// 前方宣言（Playerの実体は持たず、参照だけ使うために必要）
class Player;

// トゲの状態を定義
enum class SpikeState {
    On,    // トゲが出ている（危険）
    Off    // トゲが出ていない（安全）
};

class SpikeTrap : public TrapObject {
private:
    int openImage;    // 待機中画像
    int closedImage;  // 発動後画像

    float radius;       // 当たり判定の半径
    SpikeState state;   // 現在の状態（On/Off）

    // プレイヤーの動きを監視するための変数
    int lastPlayerX = -1;
    int lastPlayerY = -1;

public:
    // コンストラクタ（座標と判定半径を受け取る）
    SpikeTrap(float x, float y, float radius);

    // 毎フレームの更新処理（ここで移動監視と当たり判定を行う）
    void Update(const Player& player, float delta_second) override;

    // 描画処理（現在の状態を色や文字で表示する）
    void Draw() const override;

    // 状態（On/Off）を反転させる関数
    void ToggleState();
};