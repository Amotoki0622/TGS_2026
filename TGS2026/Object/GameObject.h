#pragma once

#include "../Utility/Vector2D.h"

class GameObject
{
protected:
	Vector2D location;   // 座標
	Vector2D box_size;   // 当たり判定
	int graphic_images;  // オブジェクト画像
	int flip_flag;       // 反転するか？

public:
	GameObject();
	virtual ~GameObject();

public:
	// 初期化処理
	virtual void Initialize();
	// 更新処理
	virtual void Update(float delta_second);
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
};