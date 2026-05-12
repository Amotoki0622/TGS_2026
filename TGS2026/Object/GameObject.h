#pragma once

#include "../Utility/Vector2D.h"
#include "Player/Player.h" 
#include <cmath>

// オブジェクトの種類を定義
enum class ObjectType {
	Camera,         // カメラ
	Light,          // ライト
	LegTrap,        // トラバサミトラップ
	SpikeTrap,      // トゲ床トラップ
	Wall,           // 壁
	WoodenBox,      // 動かせる木箱など
	Player,         // プレイヤー
	None
};

class GameObject
{
protected:
	Vector2D location;   // 座標
	Vector2D box_size;   // 当たり判定
	int graphic_images;  // オブジェクト画像
	int flip_flag;       // 反転するか？

	bool detected;       // 検知・発動フラグ
	ObjectType type;     // オブジェクトの種類

public:
	GameObject(ObjectType type = ObjectType::None);
	virtual ~GameObject();

public:
	// 初期化処理
	virtual void Initialize();
	// 更新処理
	virtual void Update(const Player& player, float delta_second) = 0;;
	// 描画処理
	virtual void Draw() const;
	// 終了時処理
	virtual void Finalize();

public:
	// 座標を返す
	const Vector2D& GetLocation() const
	{
		return location;
	}

	// 座標を設定する
	void SetLocation(const Vector2D& location)
	{
		this->location = location;
	}

	// 当たり判定の範囲を返す
	const Vector2D& GetBoxSize() const
	{
		return box_size;
	}

	// 見つかったか
	bool IsDetected() const 
	{ 
		return detected; 
	}
	
	// タイプ取得
	ObjectType GetType() const 
	{ 
		return type; 
	}
};