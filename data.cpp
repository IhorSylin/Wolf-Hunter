#include "raylib.h"
#include "header.h"
#include "raymath.h"
#include <string>
#include <cstdio>
#include <cmath>

// Player Anim Data
Unit CreatePlayerData(Unit Player, FILE *SaveFile, Texture2D PlayerTexture, int TileSize)
{

    Player.rec.width = PlayerTexture.width / 6;
    Player.rec.height = PlayerTexture.height;
    Player.rec.x = 0;
    Player.rec.y = 0;
    Player.pos.x = 17 * TileSize;
    Player.pos.y = 95 * TileSize;
    Player.size.x = Player.rec.width * 2;
    Player.size.y = Player.rec.height * 2;
    Player.origin.x = Player.size.x / 2;
    Player.origin.y = Player.size.y / 2;
    Player.rotation = 0;
    Player.hp = 200;
    Player.maxHp = 200;
    Player.moveSpeed = 600;
    Player.rightLeft = 1;
    Player.isAlive = true;

    return Player;
}

RectTextureData CreateCursorData(RectTextureData Cursor, Texture2D CursorTexture)
{
    Cursor.source.width = CursorTexture.width;
    Cursor.source.height = CursorTexture.height;
    Cursor.source.x = 0;
    Cursor.source.y = 0;
    Cursor.dest.width = Cursor.source.width * 4;
    Cursor.dest.height = Cursor.source.height * 4;
    Cursor.dest.x = 0;
    Cursor.dest.y = 0;
    Cursor.origin.x = Cursor.dest.width / 2;
    Cursor.origin.y = Cursor.dest.height / 2;
    Cursor.rotation = 0;
    return Cursor;
}

RectTextureData CreateWeaponData(RectTextureData WeaponData, Texture2D WeaponTexture1, Texture2D WeaponTexture2, int WeaponType, int TileSize){
    if(WeaponType == 0){
        WeaponData.source.width = WeaponTexture1.width / 2;
        WeaponData.source.height = WeaponTexture1.height;
    }
    if(WeaponType == 1){
        WeaponData.source.width = WeaponTexture2.width / 7;
        WeaponData.source.height = WeaponTexture2.height;
    }
    WeaponData.source.x = 0;
    WeaponData.source.y = 0;
    WeaponData.dest.width = WeaponData.source.width * 2;
    WeaponData.dest.height = WeaponData.source.height * 2;
    WeaponData.dest.x = 17 * TileSize;
    WeaponData.dest.y = 95 * TileSize + 12;
    WeaponData.origin.x = WeaponData.dest.width / 2;
    WeaponData.origin.y = WeaponData.dest.height / 2;
    WeaponData.rotation = 0;
    return WeaponData;
}

// RectTextureData CreateArrowData(RectTextureData ArrowData, Texture2D ArrowTexture, int TileSize)
// {
//     ArrowData.source.width = ArrowTexture.width;
//     ArrowData.source.height = ArrowTexture.height;
//     ArrowData.source.x = 0;
//     ArrowData.source.y = 0;
//     ArrowData.dest.width = ArrowData.source.width * 2;
//     ArrowData.dest.height = ArrowData.source.height * 2;
//     ArrowData.dest.x = 17 * TileSize;
//     ArrowData.dest.y = 95 * TileSize + 12;
//     ArrowData.origin.x = ArrowData.dest.width / 2;
//     ArrowData.origin.y = ArrowData.dest.height / 2;
//     ArrowData.rotation = 0;
//     return ArrowData;
// }

void GenerateCollisions(int TileType[M][N])
{
    int TileFill2[35][3]{
        {0, 37, 56},
        {0, 49, 55},
        {53, 64, 55},
        {0, 49, 54},
        {53, 64, 54},
        {0, 49, 53},
        {53, 64, 53},
        {0, 49, 52},
        {53, 64, 52},
        {0, 64, 51},
        {0, 64, 50},
        {0, 64, 49},
        {0, 64, 48},
        {0, 64, 47},
        {0, 64, 46},
        {0, 64, 45},
        {0, 64, 44},
        {0, 64, 45},
        {0, 64, 44},
        {0, 64, 43},
        {0, 64, 42},
        {0, 64, 41},
        {0, 64, 40},
        {17, 64, 39},
        {18, 64, 38},
        {19, 64, 37},
        {32, 64, 36}};
    int TileFill1[130][3]{
        {34, 42, 97},
        {56, 63, 97},
        {14, 20, 96},
        {35, 62, 96},
        {12, 20, 95},
        {35, 61, 95},
        {12, 20, 94},
        {36, 60, 94},
        {12, 18, 93},
        {39, 58, 93},
        {11, 18, 92},
        {41, 58, 92},
        {11, 16, 91},
        {43, 57, 91},
        {11, 15, 90},
        {46, 58, 90},
        {12, 15, 89},
        {46, 58, 89},
        {12, 16, 88},
        {46, 59, 88},
        {13, 17, 87},
        {46, 59, 87},
        {11, 21, 86},
        {48, 58, 86},
        {10, 23, 85},
        {48, 58, 85},
        {9, 23, 84},
        {49, 54, 84},
        {8, 25, 83},
        {50, 54, 83},
        {8, 25, 82},
        {50, 54, 82},
        {7, 25, 81},
        {50, 54, 81},
        {7, 26, 80},
        {50, 54, 80},
        {7, 26, 79},
        {50, 54, 79},
        {8, 24, 78},
        {48, 54, 78},
        {10, 23, 77},
        {48, 53, 77},
        {10, 23, 76},
        {47, 53, 76},
        {11, 25, 75},
        {47, 53, 75},
        {16, 26, 74},
        {47, 55, 74},
        {18, 27, 73},
        {48, 57, 73},
        {21, 28, 72},
        {49, 57, 72},
        {23, 28, 71},
        {52, 57, 71},
        {23, 28, 70},
        {52, 58, 70},
        {23, 29, 69},
        {52, 59, 69},
        {23, 31, 68},
        {53, 59, 68},
        {23, 33, 67},
        {53, 59, 67},
        {25, 33, 66},
        {53, 58, 66},
        {26, 35, 65},
        {53, 57, 65},
        {28, 35, 64},
        {50, 57, 64},
        {28, 38, 63},
        {41, 44, 63},
        {49, 57, 63},
        {29, 57, 62},
        {30, 57, 61},
        {33, 56, 60},
        {33, 56, 59},
        {35, 56, 58},
        {38, 55, 57},
        {38, 55, 56},
        {50, 52, 55},
        {50, 52, 54},
        {50, 52, 53},
        {50, 52, 52},
        {26, 29, 51},
        {36, 37, 51},
        {48, 53, 51},
        {25, 30, 50},
        {34, 37, 50},
        {44, 45, 50},
        {48, 53, 50},
        {25, 30, 49},
        {33, 38, 49},
        {42, 46, 49},
        {48, 53, 49},
        {18, 23, 48},
        {26, 30, 48},
        {33, 38, 48},
        {41, 46, 48},
        {49, 52, 48},
        {17, 23, 47},
        {27, 30, 47},
        {34, 37, 47},
        {41, 46, 47},
        {16, 24, 46},
        {27, 29, 46},
        {41, 45, 46},
        {16, 24, 45},
        {41, 43, 45},
        {17, 23, 44},
        {19, 22, 43},
        {14, 16, 42},
        {13, 16, 41},
        {6, 16, 40},
        {4, 16, 39},
        {4, 17, 38},
        {3, 18, 37},
        {3, 22, 36},
        {3, 26, 35},
        {4, 15, 34},
        {18, 24, 34},
        {4, 14, 33},
        {21, 23, 33},
        {6, 15, 32},
        {7, 16, 31},
        {8, 16, 30},
        {11, 17, 29},
        {12, 18, 28},
        {13, 21, 27},
        {14, 21, 26},
        {15, 21, 25},
        {19, 21, 24}};

    for (int i = 0; i < 35; i++)
    {
        int row = TileFill2[i][2];
        if (row >= M)
            continue;
        for (int j = TileFill2[i][0]; j <= TileFill2[i][1]; j++)
        {
            if (j >= N)
                continue;
            TileType[row][j] = 2;
        }
    }

    for (int i = 0; i < 130; i++)
    {
        int row = TileFill1[i][2];
        if (row >= M)
            continue;
        for (int j = TileFill1[i][0]; j <= TileFill1[i][1]; j++)
        {
            if (j >= N)
                continue;
            TileType[row][j] = 1;
        }
    }
}

Wolf CreateWolfData(Wolf Wolves, Vector2 WolfTilePos, Texture2D WolfTexture, int TileSize, int WolfMaxHP)
{
    float Size = 4.5;
    Wolves.rec.width = WolfTexture.width / 12;
    Wolves.rec.height = WolfTexture.height / 8;
    Wolves.rec.x = Wolves.rec.width;
    Wolves.rec.y = Wolves.rec.height * 2;
    Wolves.pos.x = WolfTilePos.x * TileSize;
    Wolves.pos.y = WolfTilePos.y * TileSize;
    Wolves.size.x = Wolves.rec.width * Size;
    Wolves.size.y = Wolves.rec.height * Size;
    Wolves.origin.x = Wolves.size.x / 2;
    Wolves.origin.y = Wolves.size.y / 2;
    Wolves.rotation = 0;
    Wolves.hp = WolfMaxHP;
    Wolves.rightLeft = 1;
    Wolves.status = 0;
    Wolves.agro = false;
    Wolves.isAlive = true;
    Wolves.ImTint = 0;
    Wolves.AnimDataRun.frame = 1;
    Wolves.AnimDataRun.startFrame = 0;
    Wolves.AnimDataRun.maxFrames = 6;
    Wolves.AnimDataRun.runningTime = 0;
    Wolves.AnimDataRun.updateTime = 0.15;
    Wolves.Wait1.startTime = 0.5;
    Wolves.Wait1.curTime = Wolves.Wait1.startTime;
    Wolves.Attack.startTime = 0.5;
    Wolves.Attack.curTime = Wolves.Attack.startTime;
    Wolves.Wait2.startTime = 1;
    Wolves.Wait2.curTime = Wolves.Wait2.startTime;
    Wolves.Hit.startTime = 0.5;
    Wolves.Hit.curTime = 0;
    Wolves.IsHitByDaggers = false;
    Wolves.IsHitByDaggersLock.startTime = 0.5;
    Wolves.IsHitByDaggersLock.curTime = 0;
    return Wolves;
}