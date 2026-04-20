#pragma once
#include "../Player/Player.h"
//ゴールクラス
class Goal
{
private:
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
};

