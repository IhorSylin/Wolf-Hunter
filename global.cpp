#include "raylib.h"
#include "header.h"
#include "raymath.h"
#include <string>
#include <cstdio>
#include <cmath>

Vector2 CheckAndUpdatePos(Vector2 OldPos, Vector2 NewPos, int TypeMas[M][N], int TileSize, Vector2 Size, int PlayerStatus)
{
    int m = (NewPos.y + Size.y / 3)  / TileSize;
    int n = NewPos.x / TileSize;
    if(TypeMas[m][n] == 1){
        return NewPos;
    }
    else if (TypeMas[m][n] == 2 && PlayerStatus == 1)
    {
        return NewPos;
    }
    else{
        return OldPos;
    } 
}

Vector2 CheckPosCoordinates(Vector2 Pos, int TileSize, Vector2 Size)
{
    int m = (Pos.y + Size.y / 3) / TileSize;
    int n = Pos.x / TileSize;
    return {n, m};
}

Vector2 CheckCamera(Vector2 CameraPos, int TileSize, int WindowWidth, int WindowHeight)
{  
    if(((CameraPos.x + (WindowWidth / 2)) / TileSize) >= N - 1)
    {
        CameraPos.x = TileSize * (N-1) - WindowWidth / 2;
    }
    if(((CameraPos.x - (WindowWidth / 2)) / TileSize) <= 0)
    {
        CameraPos.x = WindowWidth / 2;
    }
    if(((CameraPos.y + (WindowHeight / 2)) / TileSize) >= M - 1)
    {
        CameraPos.y = TileSize * (M-1) - WindowHeight / 2;
    }
    if(((CameraPos.y - (WindowHeight / 2)) / TileSize) <= 0)
    {
        CameraPos.y = WindowHeight / 2;
    }
    return CameraPos;
}



float GetRotation180(int RightLeft, Vector2 PlayerPos, Vector2 CursorPos)
{
    Vector2 RotationVector = Vector2Subtract(PlayerPos, CursorPos);
    Vector2Normalize(RotationVector);
    float RotationInRadians = atan2(RotationVector.y, RotationVector.x);
    // Перетворення в градуси та обробка негативних кутів
    float RotationInDegrees = RotationInRadians * RAD2DEG;
    return RotationInDegrees;
}

float GetVectRotationDegree(Vector2 Vector)
{
    float RotationInRadians = atan2(Vector.y, Vector.x);
    float RotationInDegrees = RotationInRadians * RAD2DEG;
    return RotationInDegrees;
}

Vector2 GetDirectionVect(Vector2 VectorStart, Vector2 VectorEnd)
{
    Vector2 RotationVector = Vector2Subtract(VectorEnd, VectorStart);
    RotationVector = Vector2Normalize({RotationVector});
    return RotationVector;
}



AnimData UpdateAnimData(AnimData data, float dt)
{
    // Update Running Time
    data.runningTime += dt;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // Update Animation Frame
        data.frame++;
        if (data.frame >= data.maxFrames)
        {
            data.frame = data.startFrame;
        }
    }
    return data;
}

void SaveGame(int WolvesDiedCount, Unit Player, Wolf *Wolves, Vector2 CameraTarget)
{
    FILE *FilePointer = NULL;
    FilePointer = fopen("Save", "wb");
    if (FilePointer == NULL)
    {
        perror("Error opening file");
        return;
    }
    fwrite(&WolvesDiedCount, sizeof(int), 1, FilePointer);
    fwrite(&Player, sizeof(Unit), 1, FilePointer);
    for (int i = 0; i < 8; i++){
        fwrite(&Wolves[i], sizeof(Wolf), 1, FilePointer);
    }
    fwrite(&CameraTarget, sizeof(Vector2), 1, FilePointer);
    fclose(FilePointer);
}

void LoadGame(int *WolvesDiedCount, Unit *Player, Wolf *Wolves, Vector2* CameraTarget)
{
    FILE *FilePointer = fopen("Save", "rb");
    if (FilePointer == NULL)
    {
        perror("Error opening file");
        return;
    }
    fread(WolvesDiedCount, sizeof(int), 1, FilePointer);
    fread(Player, sizeof(Unit), 1, FilePointer);
    fread(Wolves, sizeof(Wolf), 8, FilePointer);
    fread(CameraTarget, sizeof(Vector2), 1, FilePointer);
    fclose(FilePointer);
};
