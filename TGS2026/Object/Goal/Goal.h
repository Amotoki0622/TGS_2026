#pragma once
#include "../Player/Player.h"
#include "../../Utility/Vector2D.h"
#include "../GameObject.h"

//ゴールクラス
class Goal :public GameObject
{
private:
	/*float x, y;
	float width, height;*/

	Player* player; // プレイヤー参照

	float drawScale = 1.0f; // 描画用の倍率を保存する変数

	/*int goal_image;*/
	int goal_image[2];
	int image_count;
	int unlock_se;  // 鍵解除時のSE

	bool isOpen;



public:
	void SetPlayer(Player* p); // 追加

	Goal();

	Goal(float x, float y, float w, float h);
	~Goal();

	// 初期化処理
	void Initialize() override;
	//更新処理
	void Update(float delta_second) override;
	//描画処理
	void Draw() const override;
	//終了処理
	void Finalize() override;

	void SetChipSize(float size) override;

	// 当たり判定チェック
	bool IsHit(int nextX, int nextY, int width, int height) const override;

	void SetSize(float w, float h)
	{
		this->box_size.x = w;
		this->box_size.y = h;
	}

	// Goalクラスにこんな関数と変数があると想定します
	void Open();
	bool IsOpen() const;

	void GetLocation(int& outX, int& outY) const;
};

