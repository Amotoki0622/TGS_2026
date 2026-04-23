#include "StageManager.h"

// インストラクタ
StageManager::StageManager()
	: m_currentLevel(0), m_currentMoveLimit(0), m_pCurrentData(nullptr)
{

}

StageManager::~StageManager()
{
}

// 初期化処理
void StageManager::Initialize()
{
	// 全ステージデータをロード
	m_allStages = StageLoader::LoadMapList("Resource/Map/.csv");		// 格納するファイルの指定
}

// 指定したレベルのステージを開始する
void StageManager::LoadLevel(int levelIndex)
{
	// ステージの総数を取得する
	int totalStage = (int)m_allStages.size();

	// ステージが0未満ではないか
	// ステージが総ステージ数を超えていないか
	if (levelIndex < 0 || levelIndex >= totalStage)
	{
		// 存在しないステージだから、処理を中断する
		return;
	}

	// 現在のステージ番号を記録する
	m_currentLevel = levelIndex;
	// 「今使うデータ」だけにピンを立てる
	m_pCurrentData = &m_allStages[m_currentLevel];

	// ステージの詳細(ステージ配置)をロード
	StageLoader::LoadMapCSV(*m_pCurrentData);

	// 手数制限をセット
	m_currentMoveLimit = m_pCurrentData->moveLimit;


	// マップデータに基づいて、実際にゲームオブジェクトを生成する
	CreateStageObject();
}

void StageManager::CreateStageObject()
{
	if (!m_pCurrentData) return;

	for (int y = 0; y < m_pCurrentData->height; ++y) {
		for (int x = 0; x < (int)m_pCurrentData->map[y].size(); ++x) {
			char mode = m_pCurrentData->map[y][x];

			// 座標計算する処理

			// modeに応じた生成処理
			switch (mode)
			{
			default:
				break;
			}
		}
	}
}