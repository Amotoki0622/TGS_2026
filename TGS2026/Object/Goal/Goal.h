#pragma once
#include "../Player/Player.h"
#include "../../Utility/Vector2D.h"
#include "../../Utility/CollisionManager.h"

//ゴールクラス
class Goal
{
private:
	Vector2D pos;
	float width;
	float height;

	Player* player; // プレイヤー参照


public:
	void SetPlayer(Player* p); // 追加

	// 初期化処理
	void Initialize();
	//更新処理
	void Update();
	//描画処理
	void Draw() const;
	//終了処理
	void Finalize();

	bool CheckCollision(const Vector2D& playerPos, float playerW, float playerH) const;
};

