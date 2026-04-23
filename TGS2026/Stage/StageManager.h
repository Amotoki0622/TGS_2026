#pragma once

#include "StageData.h"
#include "StageLoader.h"

#include <vector>
class StageManager
{
private:
	std::vector<StageData> m_allStages; // 全ステージデータ
	int m_currentLevel;                 // 現在のステージ番号
	int m_currentMoveLimit;             // 現在の残り手数

public:
	// インストラクタ
	StageManager();
	// デストラクタ
	~StageManager();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Updata();

	// 指定したレベルのステージを開始する
	void LoadLevel(int levelIndex);

	// 現在の手数を取得
	int GetCurrentMoveLimit()const
	{
		return m_currentMoveLimit;
	}
private:
	// 実際にオブジェクトを生成する内部関数
	void CreateStageObject();

	// 現在プレイ中のステージデータを参照(ポインタ)
	StageData* m_pCurrentData;
};

