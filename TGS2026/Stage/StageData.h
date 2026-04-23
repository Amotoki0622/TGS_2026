#pragma once

#include <vector>
#include <string>

struct StageData {
	int moveLimit;		// 手数制限

	std::string MapFileName;		// マップ用のCSVパス( 例: "Stage_1.csv")

	int width;			// マップの横幅
	int height;			// マップの縦幅
	std::vector<std::vector<char>> map;		// マップのレイアウト

	// エラー対策
	bool isFileEmpty() const
	{
		return map.empty();			// データが空かどうか確認する処理
	}
};