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

	int goal_image;

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

	// 当たり判定チェック
	bool IsHit(int nextX, int nextY, int width, int height) const override;

	void SetSize(float w, float h)
	{
		this->box_size.x = w;
		this->box_size.y = h;
	}
};

