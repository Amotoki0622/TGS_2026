#pragma once

#include "../Utility/Vector2D.h"
#include "../Object/GameObject.h"
#include <vector>

enum class eSceneType
{
	eTitle,
	eInGame,
	eResult,
	eRanking,
	eEnd,
};

class SceneBase
{
protected:
	std::vector<GameObject*> objects; // 動的配列
	int background_image;             // 背景画像データ

public:
	SceneBase();
	virtual ~SceneBase();

public:
	// 初期化処理
	virtual void Initialize();
	// 更新処理
	virtual eSceneType Update();
	// 描画処理
	virtual void Draw() const;
	// 終了時処理
	virtual void Finalize();

public:
	// 現在のシーン情報を返す
	virtual eSceneType  GetNowSceneType() const = 0;

protected:
	// クラステンプレート(テンプレートの実装はヘッダーファイル内に書く)
	template <class T>
	T* CreateObject(const Vector2D& location)
	{
		T* new_instance = new T();

		GameObject* new_object = dynamic_cast<GameObject*>(new_instance);

		if (new_object == nullptr)
		{
			delete new_instance;
			throw("ゲームオブジェクトが生成できませんでした\n");
		}

		new_object->SetLocation(location);

		new_object->Initialize();

		objects.push_back(new_object);

		return new_instance;
	}

	void DestroyObject(GameObject* target);
};