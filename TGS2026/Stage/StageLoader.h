#pragma once

#include "StageData.h"
#include <vector>
#include <string>

class StageLoader
{
public:
	// ステージマップ.csvを読み込む処理
	static std::vector<StageData> LoadMapList(const std::string& filePath);

	// マップファイルを読み込み、StageDataにマップ配置を書き込む処理

	static void LoadMapCSV(StageData& data);
};

