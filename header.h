#include "raylib.h"
#include "raymath.h"
#include <string>
#include <cstdio>
#include <cmath>
#define M 98
#define N 65

// structures
struct RectTextureData
{
    Texture2D texture;
    Rectangle source;
    Rectangle dest;
    Vector2 origin;
    float rotation;
    Color tint;
};

struct AnimData
{
    int frame;
    int startFrame;
    int maxFrames;
    float runningTime;
    float updateTime;
};

struct Unit
{
    Rectangle rec;
    Vector2 pos;
    Vector2 size;
    Vector2 origin;
    float rotation;
    int rightLeft;
    int hp;
    int maxHp;
    int moveSpeed;
    bool isAlive;
};

struct Timer
{
    float curTime;
    float startTime;
};


struct Wolf{
    Rectangle rec;
    Vector2 pos;
    Vector2 size;
    Vector2 origin;
    float rotation;
    int rightLeft;
    int hp;
    Vector2 wolfToPLayer;
    Vector2 AttackDirection;
    int status;
    bool agro;
    bool isAlive;
    int ImTint;
    Timer Wait1;
    Timer Attack;
    Timer Wait2;
    Timer Hit;
    AnimData AnimDataRun;
    Rectangle Col;
    bool IsHitByDaggers;
    Timer IsHitByDaggersLock;
};

struct ButtonData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    Color textcolor;
};

// Global
Vector2 CheckPosCoordinates(Vector2 Pos, int TileSize, Vector2 Size);
Vector2 CheckAndUpdatePos(Vector2 OldPos, Vector2 NewPos, int TypeMas[M][N], int TileSize, Vector2 Size, int PlayerStatus);
Vector2 CheckCamera(Vector2 CameraPos, int TileSize, int WindowWidth, int WindowHeight);
float GetVectRotationDegree(Vector2 Vector);
Vector2 GetDirectionVect(Vector2 VectorStart, Vector2 VectorEnd);


// collors
extern Color Green;
extern Color DarkGreen;
extern Color DesaturatedDarkGreen;
extern Color DarkBrown;

// Data
Wolf CreateWolfData(Wolf Wolf, Vector2 WolfTilePos, Texture2D PlayerTexture, int TileSize, int WolfMaxHP);
extern RectTextureData Cursor;
Unit CreatePlayerData(Unit Player, FILE *SaveFile, Texture2D PlayerTexture, int TileSize);
void GenerateCollisions(int TypeMas[M][N]);
RectTextureData CreateCursorData(RectTextureData Cursor, Texture2D CursorTexture);
RectTextureData CreateWeaponData(RectTextureData WeaponData, Texture2D WeaponTexture1, Texture2D WeaponTexture2, int WeaponType, int TileSize);
AnimData UpdateAnimData(AnimData data, float dt);
void SaveGame(int WolvesDiedCount, Unit Player, Wolf *Wolves, Vector2 CameraTarget);
void LoadGame(int *WolvesDiedCount, Unit *Player, Wolf *Wolves, Vector2 *CameraTarget);

// functions
// UI
void DrawPlayerHealthBar(int HP, int MaxHP, int WindowWidth, int WindowHeight, Texture2D HealthBarBackground, Texture2D HealthBarHealth, Vector2 CameraPos);
void DrawEnemyHealthBar(Wolf Wolf, float HP, float MaxHP, Texture2D HealthBarBackground, Texture2D HealthBarHealth);
bool DrawMenu(float WindowWidth, float WindowHeight, Texture2D Menu, Texture2D Button, bool SaveExist, Vector2 MousePos, bool *LoadGameCheck);
void DrawWeaponIcon1(int WindowWidth, int WindowHeight, Texture2D Icon, Vector2 CameraPos, float Scale);
void DrawWeaponIcon2(int WindowWidth, int WindowHeight, Texture2D Icon, Vector2 CameraPos, float Scale);

// Player Actions
void DoRoll(Unit *Player, AnimData *RollAnimData, int *PlayerStatus, Vector2 PlayerCursor, float dt, int RollRightLeft, bool *PlayerInvunerable);
Vector2 PlayerRun(Vector2 PlayerPos, int MoveSpeed, float dt, bool *IsPlayerRunning);

// Volf Actions
void WolfRunToPlayer(Wolf *Wolf, float WolfMS, float dt);
