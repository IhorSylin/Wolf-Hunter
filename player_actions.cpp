#include "raylib.h"
#include "header.h"
#include "raymath.h"
#include <string>
#include <cstdio>
#include <cmath>

Vector2 PlayerRun(Vector2 PlayerPos, int MoveSpeed, float dt, bool *IsPlayerRunning)
{
    Vector2 MoveDirection{};
    if (IsKeyDown(KEY_W))
        MoveDirection.y -= 1;
    if (IsKeyDown(KEY_A))
        MoveDirection.x -= 1;
    if (IsKeyDown(KEY_S))
        MoveDirection.y += 1;
    if (IsKeyDown(KEY_D))
        MoveDirection.x += 1;
    if (Vector2Length(MoveDirection) != 0)
    {
        PlayerPos = Vector2Add(PlayerPos, Vector2Scale(Vector2Normalize(MoveDirection), MoveSpeed * dt));
        *IsPlayerRunning = true;
    }
    else{
        *IsPlayerRunning = false;
    }
    return PlayerPos;
}

void DoRoll(Unit *Player, AnimData *RollAnimData, int *PlayerStatus, Vector2 PlayerCursor, float dt, int RollRightLeft, bool *PlayerInvunerable)
{
    Vector2Normalize(PlayerCursor);
    float TickRotation = 360 / RollAnimData->updateTime * dt;
    float Velocity = 800 * dt;
    if (RollRightLeft == -1)
    {
        Player->rotation = Player->rotation - TickRotation;
    }
    if (RollRightLeft == 1)
    {
        Player->rotation = Player->rotation + TickRotation;
    }
    Player->pos = Vector2Add(Player->pos, Vector2Scale(PlayerCursor, Velocity));
    RollAnimData->runningTime = RollAnimData->runningTime + dt;
    if(RollAnimData->runningTime >= RollAnimData->updateTime){
        *PlayerStatus = 0;
        *PlayerInvunerable = false;
        RollAnimData->runningTime = 0;
        Player->rotation = 0;
    }
    else{
        *PlayerStatus = 1;
    }
}

