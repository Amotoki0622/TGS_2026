#pragma once
#include "../../Utility/Vector2D.h"
#include "../GameObject.h"

class Player;

//壁クラス
class Wall :public GameObject
{
private:

	Player* player; // プレイヤー参照

	float drawScale = 1.0f; // 描画用の倍率を保存する変数

	int wall_image;


public:
	void SetPlayer(Player* p); // 追加

	Wall();
	Wall(float x, float y, float w, float h);
	~Wall();
	void SetSize(float w, float h);

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

	bool IsHit(int nextX, int nextY, int width, int height) const override;
	//bool IsHit(int nextX, int nextY, float pW, float pH) const;

};

