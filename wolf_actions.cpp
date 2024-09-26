#include "raylib.h"
#include "header.h"
#include "raymath.h"
#include <string>
#include <cstdio>
#include <cmath>

void WolfRunToPlayer(Wolf *Wolf, float WolfMS, float dt){
    Wolf->pos = Vector2Add(Wolf->pos, Vector2Scale(Wolf->wolfToPLayer, WolfMS * dt));
    if(Wolf->wolfToPLayer.x < 0){
        Wolf->rightLeft = -1;
    }
    else
        Wolf->rightLeft = 1;
}