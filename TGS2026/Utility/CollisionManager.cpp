#include "CollisionManager.h"
#include <cmath>

// ===============================================
// 円と円
// ===============================================
bool CollisionManager::CheckCircleCollision(
    const Vector2D& posA, float radiusA,
    const Vector2D& posB, float radiusB)
{
    float dx = posA.x - posB.x;
    float dy = posA.y - posB.y;

    float distanceSq = dx * dx + dy * dy;
    float radiusSum = radiusA + radiusB;

    return distanceSq <= radiusSum * radiusSum;
}

// ===============================================
// 矩形と矩形（中心基準 AABB）
// ===============================================
bool CollisionManager::CheckBoxCollision(
    const Vector2D& posA, float widthA, float heightA,
    const Vector2D& posB, float widthB, float heightB)
{
    float halfWidthA = widthA * 0.5f;
    float halfHeightA = heightA * 0.5f;

    float halfWidthB = widthB * 0.5f;
    float halfHeightB = heightB * 0.5f;

    // 左右
    if (posA.x + halfWidthA < posB.x - halfWidthB) return false;
    if (posA.x - halfWidthA > posB.x + halfWidthB) return false;

    // 上下
    if (posA.y + halfHeightA < posB.y - halfHeightB) return false;
    if (posA.y - halfHeightA > posB.y + halfHeightB) return false;

    return true;
}

// ===============================================
// 円と矩形
// ===============================================
bool CollisionManager::CheckCircleBoxCollision(
    const Vector2D& circlePos, float radius,
    const Vector2D& boxPos, float boxWidth, float boxHeight)
{
    float halfWidth = boxWidth * 0.5f;
    float halfHeight = boxHeight * 0.5f;

    // 矩形の範囲
    float left = boxPos.x - halfWidth;
    float right = boxPos.x + halfWidth;
    float top = boxPos.y - halfHeight;
    float bottom = boxPos.y + halfHeight;

    // 円の中心を矩形内にクランプ
    float closestX = circlePos.x;
    float closestY = circlePos.y;

    if (circlePos.x < left)   closestX = left;
    if (circlePos.x > right)  closestX = right;
    if (circlePos.y < top)    closestY = top;
    if (circlePos.y > bottom) closestY = bottom;

    // 距離計算
    float dx = circlePos.x - closestX;
    float dy = circlePos.y - closestY;

    return (dx * dx + dy * dy) <= (radius * radius);
}