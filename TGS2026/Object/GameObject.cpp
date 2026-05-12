#include "GameObject.h"
#include "DxLib.h"

GameObject::GameObject() :location(0.0f), box_size(0.0f)
{
}

GameObject::~GameObject()
{
	this->Finalize();
}

void GameObject::Initialize()
{
}

void GameObject::Update(float delta_second)
{
}

void GameObject::Draw() const
{
	// オブジェクト画像を描画
	DrawRotaGraphF(location.x, location.y, 1.0, 0.0, graphic_images, TRUE, flip_flag);

#if _DEBUG
	// 当たり判定の範囲を描画
	Vector2D a = location - (box_size * 0.5f);
	Vector2D b = a + box_size;
	DrawBoxAA(a.x, a.y, b.x, b.y, GetColor(0, 255, 0), FALSE);
#endif // _DEBUG
}

void GameObject::Finalize()
{
}