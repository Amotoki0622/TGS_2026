#pragma once
#include "../Player/Player.h"
#include "../../Utility/Vector2D.h"
#include "../GameObject.h"

//壁クラス
class Wall :public GameObject
{
private:
	float x, y;
	float width, height;

	Player* player; // プレイヤー参照


public:
	void SetPlayer(Player* p); // 追加

	Wall(float x, float y, float w, float h);
	~Wall();

	// 初期化処理
	void Initialize() override;
	//更新処理
	void Update(float delta_second) override;
	//描画処理
	void Draw() const override;
	//終了処理
	void Finalize() override;

	bool CheckCollision() const;

};

