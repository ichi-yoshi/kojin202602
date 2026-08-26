#include "EnemyInfo.h"
#include "Resource.h"

namespace EnemyType
{
    // 通常の敵
    const EnemyData Enemy1 = 
    {
        8.5f,                           // 移動速度
        100.0f,                         // 最大スタミナ
        0.05f,                          // 回復率
        0.01f,                          // 消費率
        { 1000.0f, 0.0f, -300.0f },     // 初期座標
        image::Enemy1                   // 画像パス
    };

    // 足の速い敵
    const EnemyData Enemy2 = 
    {
        14.0f,                          // 移動速度
        80.0f,                          // 最大スタミナ
        0.08f,                          // 回復率
        0.02f,                          // 消費率
        { 1000.0f, 0.0f, -300.0f },     // 初期座標
        image::Enemy2                   // 画像パス
    };
}