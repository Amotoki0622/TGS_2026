#pragma once
#include <vector>
#include "../../Utility/Vector2D.h"
#include "../GameObject.h"
#include "../../Utility/EffectManager/EffectManager.h"

class Player;
class EffectManager;

//壁クラス
class Block :public GameObject
{
private:

	Player* player; // プレイヤー参照
	
    // =========================================
    // エフェクト用変数
    // =========================================
    EffectManager effectManager; // プレイヤー専用のエフェクトマネージャ
    int lastPx = -1;             // 1フレーム前のX座標
    int lastPy = -1;             // 1フレーム前のY座標
	
	float drawScale = 1.0f; // 描画用の倍率を保存する変数

	int block_image;  // ブロックの画像
	int block_se;  // ブロックが蹴られた時のSE

public:
	void SetPlayer(Player* p); // 追加

	Block();

	Block(float x, float y, float w, float h);
	~Block();

	// 初期化処理
	void Initialize() override;
	//更新処理
	void Update(float delta_second) override;
	//描画処理
	void Draw() const override;
	// 画像のサイズ変更処理
	void SetChipSize(float size) override;
	//終了処理
	void Finalize() override;
	// 当たり判定チェック
	bool IsHit(int nextX, int nextY, int width, int height) const override;

	// プレイヤーがキックするとブロックが動く
	void Push(float moveX, float moveY, const std::vector<GameObject*>& objects);

	void SetSize(float w, float h)
	{
		this->box_size.x = w;
		this->box_size.y = h;
	}
};

