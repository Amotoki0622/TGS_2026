#pragma once
#include "Vector2D.h"

// ===============================================
// CollisionManager
// 当たり判定専用クラス（状態を持たない）
// ===============================================
class CollisionManager
{
public:

    // ===========================================
    // 円と円の当たり判定
    // pos = 中心座標
    // radius = 半径
    // ===========================================
    static bool CheckCircleCollision(
        const Vector2D& posA, float radiusA,
        const Vector2D& posB, float radiusB);

    // ===========================================
    // 矩形と矩形の当たり判定（中心基準）
    // pos = 中心座標
    // width / height = 幅と高さ
    // ===========================================
    static bool CheckBoxCollision(
        const Vector2D& posA, float widthA, float heightA,
        const Vector2D& posB, float widthB, float heightB);

    // ===========================================
    // 円と矩形の当たり判定
    // ===========================================
    static bool CheckCircleBoxCollision(
        const Vector2D& circlePos, float radius,
        const Vector2D& boxPos, float boxWidth, float boxHeight);
};
