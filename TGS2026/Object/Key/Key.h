#pragma once
#include "../GameObject.h"

class Player;

class Key : public GameObject
{
private:
    Player* targetPlayer; // 追従対象のプレイヤー
    bool isPickedUp;      // 鍵を拾ったかどうかのフラグ
    float angle;          // 演出（上下浮遊、周囲回転）に使用する角度

public:
    Key(float x, float y, Player* p);
    ~Key();

    void Initialize() override;
    void Update(float delta_second) override;
    void Draw() const override;
    void Finalize() override;

    // 【重要】プレイヤーの移動システム用。すり抜けるためにオーバーライド
    bool IsHit(int nextX, int nextY, int width, int height) const override;

    // 基底クラス(GameObject)の仕様に合わせるための通り抜けフラグ
    bool IsPassable() const override { return true; }
};