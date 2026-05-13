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

	// 当たり判定チェック用の関数
	// 仮想関数(virtual)にすることで、子クラスで個別の判定処理を書けるようにします
	virtual bool IsHit(int nextX, int nextY, int width, int height) const {
		// 基本的な矩形（ボックス）同士の当たり判定ロジック
		float halfW = box_size.x * 0.5f;
		float halfH = box_size.y * 0.5f;

		// オブジェクトの上下左右の座標
		float left = location.x - halfW;
		float right = location.x + halfW;
		float top = location.y - halfH;
		float bottom = location.y + halfH;

		// プレイヤーの次の移動先範囲
		float pLeft = nextX - width * 0.5f;
		float pRight = nextX + width * 0.5f;
		float pTop = nextY - height * 0.5f;
		float pBottom = nextY + height * 0.5f;

		// 重なりチェック
		return (left < pRight && right > pLeft && top < pBottom && bottom > pTop);
	}

	// 「動かせるかどうか」のフラグ（デフォルトはfalse、ブロッククラスでtrueにする）
	virtual bool IsMovable() const { return false; }
};