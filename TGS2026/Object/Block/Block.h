#pragma once
#include "../../Utility/Vector2D.h"
#include "../GameObject.h"

class Player;

//壁クラス
class Block :public GameObject
{
private:
	float x, y;
	float width, height;

	Player* player; // プレイヤー参照


public:
	void SetPlayer(Player* p); // 追加

	Block(float x, float y, float w, float h);
	~Block();

	// 初期化処理
	void Initialize() override;
	//更新処理
	void Update(float delta_second) override;
	//描画処理
	void Draw() const override;
	//終了処理
	void Finalize() override;

	bool IsHit(int nextX, int nextY, float pW, float pH) const;

};

