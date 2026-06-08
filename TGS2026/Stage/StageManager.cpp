#include "StageManager.h"
#include "../Object/Player/Player.h"

// 追加した所
#include "../Object/Block/Block.h"
#include "../Object/Wall/Wall.h"
#include "../Object/Goal/Goal.h"
#include "../Object/Trap/Cam/Cam.h"
#include "../Object/Trap/SpikeTrap/SpikeTrap.h"
#include "../Object/Key/Key.h"
#include "../Object/Warp/Warp.h"
#include <DxLib.h>

// インストラクタ
StageManager::StageManager()
	: m_currentLevel(0)
	, m_currentMoveLimit(0)
	, m_pCurrentData(nullptr)
{

}

StageManager::~StageManager()
{
	// もし残ってたら消すため
	ClearObjects();
}

// 初期化処理
void StageManager::Initialize()
{
	// 1. リストを読み込む
	m_allStages = StageLoader::LoadMapList("Resource/Map/StageData.csv");

	// 2. リストが空っぽならここで処理を止める（アクセス違反を防ぐ）
	if (m_allStages.empty())
	{
		// ここで止まる場合は StageData.csv の読み込み自体に失敗しています
		return;
	}

	// 3. 各ステージの中身(Stage1.csvなど)を読み込む
	for (int i = 0; i < (int)m_allStages.size(); i++)
	{
		StageLoader::LoadMapCSV(m_allStages[i]);
	}

	// 4. 準備ができたら最初のレベルをロード
	LoadLevel(0);
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

	if (m_pCurrentData->map.empty())
	{
		return;
	}

	// ステージの詳細(ステージ配置)をロード
	// StageLoader::LoadMapCSV(*m_pCurrentData);＊ワンちゃんいらないかもしれない(要らなかった)

	// 手数制限をセット
	m_currentMoveLimit = m_pCurrentData->moveLimit;


	// マップデータに基づいて、実際にゲームオブジェクトを生成する
	CreateStageObject();				// (オブジェクトを生成するときに使う処理)
}

void StageManager::NextLevel()
{
	int nextLevel = m_currentLevel + 1;

	// 次のステージがあるか確認
	if (nextLevel < (int)m_allStages.size())
	{
		LoadLevel(nextLevel);
	}
	else
	{
		// 全ステージクリアした時の処理(エンディングがあったらここで処理)
	}

}

void StageManager::ClearObjects()
{
	m_Object.clear();
}


// 実際にオブジェクトを生成する内部関数
void StageManager::CreateStageObject()
{
	//if (!m_pCurrentData) return;
	if (m_pCurrentData == nullptr)
	{
		return;
	}

	// 古いデータが有れば一回リセットする
	ClearObjects();

	// ワープフラグリセット
	m_hasWarpSrc = false;
	m_hasWarpDst = false;
	
	Warp* temporaryWarpInstance = nullptr;	// 一時保存用

	// ★デバック用の1マスサイズ
	float CHIP_SIZE = 128.0f;

	// ステージごとに1マスを変更する処理(32.0f)が限界値
	//float CHIP_SIZE = 128.0f - (m_currentLevel * 32.0f);

	if (m_currentLevel >= 2)
	{
		CHIP_SIZE = 100.0f;
	}

	/*if (CHIP_SIZE < 32.0f)
	{
		CHIP_SIZE = 32.0f;
	}*/

	for (int y = 0; y < m_pCurrentData->height; ++y) 
	{
		for (int x = 0; x < (int)m_pCurrentData->map[y].size(); ++x) 
		{
			char mode = m_pCurrentData->map[y][x];

			// 座標計算する処理

			/*マスの中心座標を求めてる*/
			const float start_x = CHIP_SIZE / 2.0f;
			const float start_y = CHIP_SIZE / 2.0f;

			float posX = start_x + (x * CHIP_SIZE);
			float posY = start_y + (y * CHIP_SIZE);
			
			// modeに応じた生成処理	(コメント文の中に詳細を書く)
			switch (mode)
			{
				case 'B':
				{
					m_debug_isB_Detected = true;
					m_debug_bX = posX;
					m_debug_bY = posY;

					// デバック
					//DrawFormatString(20, 320, GetColor(255, 255, 255), "CSV 'B' Detect! X: %f Y: %f", posX, posY);
					//AppLogAdd("★CSVから 'B' を検知しました！ マップデータ位置: [x:%d, y:%d] 計算座標: (X:%f, Y:%f)\n", x, y, posX, posY);

					// 動かせる壁を生成する処理
					Block * new_block = CreateStageObjectInstance<Block>(Vector2D(posX, posY));

					if (new_block != nullptr)
					{
						new_block->SetSize(CHIP_SIZE, CHIP_SIZE);
						new_block->SetChipSize(CHIP_SIZE);
					}
				}	
					break;

				case 'W':
				{
					// 壁を生成する処理
					Wall* new_wall = CreateStageObjectInstance<Wall>(Vector2D(posX, posY));
					if (new_wall != nullptr)
					{
						new_wall->SetSize(CHIP_SIZE, CHIP_SIZE);
						new_wall->SetChipSize(CHIP_SIZE);
					}
				}
					break;

				case 'P':
					// プレイヤーを生成する処理
					m_playerSpawnPos = Vector2D(posX, posY);

				/*	if (m_pPlayer != nullptr)
					{
						m_pPlayer->SetChipSize(CHIP_SIZE);
					}*/

					break;

				case 'C':
				{
					// カメラを生成する処理
					Cam* new_cam = CreateStageObjectInstance<Cam>(Vector2D(posX, posY));

					if (new_cam != nullptr)
					{
						// 2. カメラの初期パラメータをセット
						// 例：下向き（DX_PI_F / 2.0f）、距離350.0f、視野角0.8f
						new_cam->SetUpCamera(DX_PI_F / 2.0f, 350.0f, 0.8f);
					}
				}
					break;

				case 'G':
				{
					// ゴールのサイズの変数を定義(70.0f)に設定
					float GOAL_SIZE = CHIP_SIZE;

					Goal* new_goal = CreateStageObjectInstance<Goal>(Vector2D(posX, posY));

					if (new_goal != nullptr)
					{
						new_goal->SetSize(GOAL_SIZE, GOAL_SIZE);
					}
				}
					break;

				case 'L':
				{
					// ライトを生成する処理
				}
					break;

				case 'T':
				{
					// トラップ(トラばさみのような地面に設置する罠)を生成する処理
					const float TRAP_RADIUS = 100.0f;

					SpikeTrap* new_spike = CreateStageObjectInstance<SpikeTrap>(Vector2D(posX, posY));
					if (new_spike != nullptr)
					{
						// 半径をセット
						new_spike->SetRadius(TRAP_RADIUS);

						new_spike->Initialize();
					}
				}
					break;

				case 'F':
				{

					// 床を生成する処理
				}
					break;

				case 'w':
				{
					// ワープ入口の生成
					temporaryWarpInstance = CreateStageObjectInstance<Warp>(Vector2D(posX, posY));
					if (temporaryWarpInstance != nullptr)
					{
			
						temporaryWarpInstance->SetPosition(posX, posY);
						temporaryWarpInstance->SetSize(CHIP_SIZE, CHIP_SIZE);

						m_warpSrcPos = Vector2D(posX, posY);
						m_hasWarpSrc = true;
					}
				}
					break;

				case 'v':
				{
					m_warpDstPos = Vector2D(posX, posY);
					m_hasWarpDst = true;
				}
					break;

				case 'K':
				{
					// 鍵を生成する処理
					Key* new_key = CreateStageObjectInstance<Key>(Vector2D(posX, posY));
				}
					break;

				default:
					break;
			}
		}
	}

	if (m_hasWarpSrc && m_hasWarpDst && temporaryWarpInstance != nullptr)
	{
		temporaryWarpInstance->SetTargetPosition(m_warpDstPos.x, m_warpDstPos.y);
	}
}

Vector2D StageManager::GetPlayerSpawnPosition() const
{
	return m_playerSpawnPos;
}

void StageManager::DrawDebugInfo() const
{
	// もし 'B' が一度でも読み込まれていたら、画面の左上にずっと文字を出し続ける デバック用
	if (m_debug_isB_Detected == true)
	{
		DrawFormatString(20, 20, GetColor(255, 255, 0), "【DEBUG】CSV 'B' Loaded! Pos:(%f, %f)", m_debug_bX, m_debug_bY);
	}
	else
	{
		DrawFormatString(20, 20, GetColor(255, 0, 0), "【DEBUG】CSV 'B' NOT FOUND...");
	}
}
