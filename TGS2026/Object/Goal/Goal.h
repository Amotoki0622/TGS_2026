#include "../Player/Player.h"
#include "../../Utility/Vector2D.h"

#pragma once
//ゴールクラス
class Goal
{
public:
	Vector2D pos;
	float width;
	float height;

private:
	Player player;

public:
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

