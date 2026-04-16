#include "SceneBase.h"

#include "DxLib.h"

SceneBase::SceneBase() : background_image(NULL)
{

}

SceneBase::~SceneBase()
{
	this->Finalize();
}

void SceneBase::Initialize()
{

}

eSceneType SceneBase::Update()
{
	// objectsの更新処理をまとめて呼ぶ
	for (GameObject* obj : objects)
	{
		obj->Update();
	}

	return GetNowSceneType();
}

void SceneBase::Draw() const
{
	DrawGraph(0, 0, background_image, TRUE);

	// objectsの描画処理をまとめて呼ぶ
	for (GameObject* obj : objects)
	{
		obj->Draw();
	}
}

void SceneBase::Finalize()
{
	// 要素が空かどうか調べる
	if (objects.empty())
	{
		return;
	}

	// objectsの終了時処理をまとめて呼ぶ
	for (GameObject* obj : objects)
	{
		obj->Finalize();
		delete obj;
	}

	// 要素を全て削除
	objects.clear();
}