#pragma once
#include "../GameObject.h"

class Player;

class Key : public GameObject
{
private:
    Player* targetPlayer; // 追従対象のプレイヤー
    bool isPickedUp;      // 鍵を拾ったかどうかのフラグ
    float angle;          // 演出（上下浮遊、周囲回転）に使用する角度

    float drawScale = 1.0f; // 描画用の倍率を保存する変数

    int key_image;  // 鍵の画像

public:
    Key();
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

    void SetChipSize(float size) override;

    // プレイヤーをセットするための関数
    void SetPlayer(Player* p);

    void SetSize(float w, float h)
    {
        this->box_size.x = w;
        this->box_size.y = h;
    }

};