#pragma once
#include "DxLib.h"

class GridManager {
private:
    int startX, startY; // 赤い枠の左上の座標
    int cellSize;       // 1マスのサイズ（例: 64）
    int cols, rows;     // 横に何マス、縦に何マスか
    unsigned int color;

public:
    // コンストラクタで範囲を指定
    GridManager(int x, int y, int size, int c, int r)
        : startX(x), startY(y), cellSize(size), cols(c), rows(r) {
        color = GetColor(150, 150, 150); // グリッドの色
    }

    // グリッド座標からスクリーン座標に変換
    void GetScreenPos(int gridX, int gridY, int& outX, int& outY) const {
        outX = startX + (gridX * cellSize) + (cellSize / 2); // マスの中心を返す
        outY = startY + (gridY * cellSize) + (cellSize / 2);
    }

    // 描画処理
    void Draw() const {
        int width = cols * cellSize;
        int height = rows * cellSize;

        // 縦線
        for (int i = 0; i <= cols; i++) {
            int x = startX + (i * cellSize);
            DrawLine(x, startY, x, startY + height, color);
        }
        // 横線
        for (int j = 0; j <= rows; j++) {
            int y = startY + (j * cellSize);
            DrawLine(startX, y, startX + width, y, color);
        }
    }

    // 指定したグリッド位置が範囲内かチェック
    bool IsWithinBounds(int gx, int gy) const {
        return (gx >= 0 && gx < cols && gy >= 0 && gy < rows);
    }
};