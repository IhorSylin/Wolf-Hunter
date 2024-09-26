#include "raylib.h"
#include "header.h"
#include "raymath.h"
#include <string>
#include <cstdio>
#include <cmath>


int main()
{
    // Window Initialization
    float WindowDimensions[2]{1920, 1080};
    InitWindow(WindowDimensions[0], WindowDimensions[1], "Wolf Hunter");

    // Textures
    Texture2D Button = LoadTexture("Assets/My/Button.png");
    Texture2D Menu = LoadTexture("Assets/My/Menu.png");
    Texture2D PlayerTexture = LoadTexture("Assets/My/Character.png");
    Texture2D TileMap = LoadTexture("Assets/Map/Main Map.png");
    Texture2D FrontTrees = LoadTexture("Assets/Map/FrontTrees.png");
    Texture2D CursorTexture = LoadTexture("Assets/My/Cursor.png");
    Texture2D HealthBarBackground = LoadTexture("Assets/My/HeathBarBackground.png");
    Texture2D HealthBarHealth = LoadTexture("Assets/My/HeathBarHealth.png");
    Texture2D Weapons[2];
    Weapons[0] = LoadTexture("Assets/My/Daggers.png");
    Weapons[1] = LoadTexture("Assets/My/BowPlayer.png");
    Texture2D DaggerSlashesTexture = LoadTexture("Assets/My/DaggerSlashes.png");
    Texture2D WolfTexture = LoadTexture("Assets/wolfpack_9.27.21/full_color/wolf_black_full.png");
    Texture2D ArrowTexture = LoadTexture("Assets/My/Arrow.png");
    Texture2D DaggerIcon = LoadTexture("Assets/My/DaggerIcon.png");
    Texture2D BowIcon = LoadTexture("Assets/My/BowIcon.png");
    Texture2D Controls[2];
    Controls[0] = LoadTexture("Assets/My/ControlsHidden.png");
    Controls[1] = LoadTexture("Assets/My/ControlsFull.png");

    // Global
    bool InMenu = true;
    Vector2 MousePos = {0.0f, 0.0f};
    int GameEndCond = 0;
    int WolvesDiedCount = 0;
    int ControlsForm = 0;

    // Save Check
    int SaveCheck = false;
    FILE *FilePointer = NULL;
    FilePointer = fopen("Save", "rb");
    if (FilePointer != NULL) SaveCheck = true;
    fclose(FilePointer);
    bool LoadGameCheck = false;

    // Map
    int MapScale = 4;
    Vector2 TileMapPos{0, 0};
    int TileType[M][N]{};
    int TileSize{16 * MapScale};
    GenerateCollisions(TileType);

    // Player Data
    Unit Player = CreatePlayerData(Player, FilePointer, PlayerTexture, TileSize);

    //Player Weapon Data
    int WeaponType = 0;
    RectTextureData WeaponData = CreateWeaponData(WeaponData, Weapons[0], Weapons[1], WeaponType, TileSize);
    //Daggers
    AnimData Daggers{0, 0, 2, 0, 1};
    RectTextureData DaggerSlashes;
    Rectangle DaggerSlashesCol;
    float DaggerSlashesColPad = 20;
    float PlayerAttackCD = 0.5;
    float PlayerAttackCurCD = 0;
    float AttackDur = 0.15;
    float AttackCurDur = 0;
    float DaggerSlashesExistanceTime = 0.15;
    float DaggerSlashesCurExistanceTime = 0;
    float ChainAttackTimer = 1.2;
    float ChainAttackCurTimer = 0;
    float ChainAttackNum = 0;
    Vector2 DaggerSlashesDirection;
    int DaggerDamage;

    // Bow And Arrow
    AnimData BowStart{0, 0, 4, 0, 0.30};
    AnimData InitBowStart = BowStart;
    AnimData BowPost{4, 4, 7, 0, 0.1};
    AnimData InitBowPost = BowPost;
    RectTextureData Arrow;
    Vector2 ArrowDirection;
    float ArrowFlyTime = 2;
    float ArrowCurFlyTime = 0;
    float ArrowFlySpeed = 1000;
    bool ArrowHitAnEnemy = false;
    float ArrowDamage = 50;
    Rectangle ArrowCol;
    Vector2 ArrowDestGrower;
    Vector2 ArrowColDestGrower;

    // Player Additional Data
    bool IsPlayerRunning = false;
    float RollCD = 0.75;
    float RollCurCD = 0;
    Vector2 PlayerPosOld{};
    Color PlayerTint = WHITE;
    bool PlayerInvunerable = false;
    Timer PlayerHitTime;
    PlayerHitTime.startTime = 0.5;
    PlayerHitTime.curTime = 0;
    // 0 - idle, 3 - being hit; 2 - roling; 1 - moving;
    int PlayerStatus = 0;
    // Colision
    Rectangle PlayerCol;
    float PlayerColPad = 20;

    // Wolf Data
    Wolf Wolves[8];
    Color WolfTint[2]{WHITE, RED};
    Vector2 WolfTilePos[8]{
        {11, 78},
        {22, 81},
        {48, 89},
        {57, 88},
        {54, 94},
        {5, 36},
        {9, 32},
        {12, 34}};
    float WolfMaxHp = 100;
    float WolfDamage = 20;
    float WolfMoveSpeed = 400;
    float WolfDashSpeed = 1000;
    for (int i = 0; i < 8; i++)
        Wolves[i] = CreateWolfData(Wolves[i], WolfTilePos[i], WolfTexture, TileSize, WolfMaxHp);

    // Player Anim Data;
    // Roll
    AnimData RollAnimData{0, 0, 6, 0, 0.5};
    Vector2 RollVector;
    Vector2 PlayerRollStartPos{};
    Vector2 PlayerNewLocCoordinates{};
    int RollRightLeft;
    // Idle
    AnimData IdleAnimData{0, 0, 2, 0, 0.4};
    // Run
    AnimData RunAnimData{3, 3, 6, 0, 0.2};
    AnimData InitRunAnimData = RunAnimData;

    // Cursor Data
    RectTextureData Cursor = CreateCursorData(Cursor, CursorTexture);

    // Camera Data
    Camera2D Camera = {0};
    Camera.target = (Vector2){Player.pos.x + Player.rec.width / 2, Player.pos.y + Player.rec.height / 2};
    Camera.offset = (Vector2){WindowDimensions[0] / 2.0f - Player.rec.width / 2, WindowDimensions[1] / 2.0f - Player.rec.width / 2};
    Camera.rotation = 0.0f;
    Camera.zoom = 1.0f;

    // Global
    Vector2 RotationVector;
    Vector2 PlayerScreenPos;
    float dt;

    //UI Param
    float WeaponIconScale1;
    float WeaponIconScale2;

    HideCursor();
    ToggleFullscreen();
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {   
        // Custom Mouse Coordinates Update
        MousePos = GetMousePosition();
        Cursor.dest.x = MousePos.x;
        Cursor.dest.y = MousePos.y;

        BeginDrawing();
        ClearBackground(WHITE);

        if (InMenu)
            InMenu = DrawMenu(WindowDimensions[0], WindowDimensions[1], Menu, Button, SaveCheck, MousePos, &LoadGameCheck);
        else
        {
            if (LoadGameCheck == true)
            {
                LoadGame(&WolvesDiedCount, &Player, Wolves, &Camera.target);
                LoadGameCheck = false;
            }

            // Get Delta Time
            dt = GetFrameTime();

            // Cooldowns Update
            if (RollCurCD > 0){
                RollCurCD -= dt;
            }
            // Camera Center Update
            Camera.target = (Vector2){Player.pos};

            // Saving Old Player Pos
            PlayerPosOld = Player.pos;

            // Debug Camera And Pos
            Player.pos = CheckAndUpdatePos(PlayerPosOld, Player.pos, TileType, TileSize, Player.size, PlayerStatus);
            Camera.target = CheckCamera(Camera.target, TileSize, WindowDimensions[0], WindowDimensions[1]);

            // Player Screan Position Update
            PlayerScreenPos.x = WindowDimensions[0] / 2 + (WindowDimensions[0] / 2 - ((Camera.target.x + WindowDimensions[0] / 2) - Player.pos.x));
            PlayerScreenPos.y = WindowDimensions[1] / 2 + (WindowDimensions[1] / 2 - ((Camera.target.y + WindowDimensions[1] / 2) - Player.pos.y));

            //Get Rotation Vector
            RotationVector = GetDirectionVect(PlayerScreenPos, {Cursor.dest.x, Cursor.dest.y});

            //Update RightLeft
            Player.rightLeft = MousePos.x >= PlayerScreenPos.x ? 1 : -1;
            if (IsKeyDown(KEY_TAB)){
                ControlsForm = 1;
            }
            else{
                ControlsForm = 0;
            }

            if (IsKeyPressed(KEY_F8))
                {
                    SaveGame(WolvesDiedCount, Player, Wolves, Camera.target);
                }
            if (IsKeyPressed(KEY_F9))
            {
                LoadGame(&WolvesDiedCount, &Player, Wolves, &Camera.target);
            }

            if (IsKeyPressed(KEY_ONE))
            {
                WeaponType = 0;
            }

            if (IsKeyPressed(KEY_TWO))
            {
                WeaponType = 1;
            }

            // if(PlayerStatus == 2)
            // {
            //     PlayerInvunerable = true;
            //     PlayerTint = RED;
            //     PlayerInvunerable = true;
            //     if(PlayerHitTime.curTime > 0){
            //         PlayerHitTime.curTime -= dt;
            //     }
            //     else{
            //         PlayerStatus = 0;
            //         PlayerInvunerable = false;
            //         PlayerTint = WHITE;
            //         PlayerHitTime.curTime = PlayerHitTime.startTime;
            //         PlayerInvunerable = false;
            //     }
            // }
            if(GameEndCond == 0){
                if (PlayerStatus == 1)
                {
                    // Roll Action Continuation
                    DoRoll(&Player, &RollAnimData, &PlayerStatus, RollVector, dt, Player.rightLeft, &PlayerInvunerable);
                    Player.pos = CheckAndUpdatePos(PlayerPosOld, Player.pos, TileType, TileSize, Player.size, PlayerStatus);
                    if (PlayerStatus == 0)
                    {
                        PlayerNewLocCoordinates = CheckPosCoordinates(Player.pos, TileSize, Player.size);
                        Player.pos = CheckAndUpdatePos(PlayerRollStartPos, Player.pos, TileType, TileSize, Player.size, PlayerStatus);
                        if (Player.pos.x == PlayerRollStartPos.x && Player.pos.y == PlayerRollStartPos.y && TileType[(int)PlayerNewLocCoordinates.x][(int)PlayerNewLocCoordinates.y] == 1)
                        {
                            Player.hp = Player.hp - 20;
                        }
                        RollCurCD = RollCD;
                    }
                }
                else{
                    // WASD movement
                    Player.pos = PlayerRun(Player.pos, Player.moveSpeed, dt, &IsPlayerRunning);
                    // Debugging Pos
                    Player.pos = CheckAndUpdatePos(PlayerPosOld, Player.pos, TileType, TileSize, Player.size, PlayerStatus);
                    // Roll Action
                    if(IsKeyPressed(KEY_SPACE) && RollCurCD <= 0 && DaggerSlashesCurExistanceTime == 0){
                        RollVector = RotationVector;
                        PlayerRollStartPos = (Vector2){Player.pos};
                        RollRightLeft = Player.rightLeft;
                        PlayerInvunerable = true;
                        DoRoll(&Player, &RollAnimData, &PlayerStatus, RollVector, dt, RollRightLeft, &PlayerInvunerable);
                    }
                    // Animation 
                    if(IsPlayerRunning == true){
                        RunAnimData = UpdateAnimData(RunAnimData, dt);
                        Player.rec.x = (PlayerTexture.width / 6) * RunAnimData.frame;
                    }
                    else{
                        RunAnimData = InitRunAnimData;
                        IdleAnimData = UpdateAnimData(IdleAnimData, dt);
                        Player.rec.x= (PlayerTexture.width / 6) * IdleAnimData.frame;
                    }
                    if(WeaponType == 0){
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && PlayerAttackCurCD == 0){
                            // Slashes Anim Update Start
                            AttackCurDur = AttackDur;
                            Daggers.frame = 1;
                            WeaponData.source.x = Weapons[0].width / 2 * Daggers.frame;
                            // Attack CD Start
                            PlayerAttackCurCD = PlayerAttackCD;
                            // Chain Attack Timer Start
                            ChainAttackCurTimer = ChainAttackTimer;
                            // DaggerSlashes Data Initialisation
                            DaggerSlashes = WeaponData;
                            DaggerSlashes.source.width = DaggerSlashesTexture.width/3;
                            DaggerSlashes.source.height = DaggerSlashesTexture.height;
                            DaggerSlashes.dest.width = DaggerSlashes.source.width * 5;
                            DaggerSlashes.dest.height = DaggerSlashes.source.height * 5;
                            DaggerSlashes.source.x = 0;
                            DaggerSlashes.source.y = 0;
                            DaggerSlashesCurExistanceTime = DaggerSlashesExistanceTime;
                            DaggerSlashesDirection = Vector2Normalize(RotationVector);                        
                            if (Player.rightLeft == -1)
                            {
                                DaggerSlashes.rotation = DaggerSlashes.rotation + 180;
                            }
                            Vector2 DaggerSlashesDestGrower = Vector2Add({DaggerSlashes.dest.x, DaggerSlashes.dest.y}, Vector2Scale(DaggerSlashesDirection, 50));
                            DaggerSlashes.dest.x = DaggerSlashesDestGrower.x;
                            DaggerSlashes.dest.y = DaggerSlashesDestGrower.y;
                            ChainAttackNum++;
                            for (int i = 0; i < 8; i++){
                                Wolves[i].IsHitByDaggers = false;
                            }
                        }
                    }
                    if(WeaponType == 1){
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                        {
                            if (BowStart.frame < 3) BowStart = UpdateAnimData(BowStart, dt);
                            WeaponData.source.x = Weapons[WeaponType].width / 7 * BowStart.frame;
                        }
                        else if(BowStart.frame == 3){
                            if (BowPost.frame < 6){
                                BowPost = UpdateAnimData(BowPost, dt);
                                WeaponData.source.x = Weapons[WeaponType].width / 7 * BowPost.frame;
                            } 
                            else{
                                BowPost = InitBowPost;
                                BowStart = InitBowStart;
                                Arrow = WeaponData;
                                ArrowHitAnEnemy = false;
                                Arrow.source.width = ArrowTexture.width;
                                Arrow.source.height = ArrowTexture.height;
                                ArrowCurFlyTime = ArrowFlyTime;
                                ArrowDirection = Vector2Normalize(RotationVector);
                                if (Player.rightLeft == -1)
                                {
                                    Arrow.rotation = Arrow.rotation + 180;
                                }
                            }
                            
                        } 
                        else
                        {
                            BowPost = InitBowPost;
                            BowStart = InitBowStart;
                            WeaponData.source.x = Weapons[WeaponType].width / 7 * BowStart.frame;
                        }
                    }
                }
            }
            // Arrow Pos and FlyTimeUpdate
            if (ArrowCurFlyTime > 0){
                ArrowDestGrower = Vector2Add({Arrow.dest.x, Arrow.dest.y}, Vector2Scale(ArrowDirection, ArrowFlySpeed * dt));
                Arrow.dest.x = ArrowDestGrower.x;
                Arrow.dest.y = ArrowDestGrower.y;
                ArrowCurFlyTime -= dt;
            }
            else ArrowCurFlyTime = 0;
            //Weapon 1 AnimData
            if(WeaponType == 0){
            // Dagger Anim Update
            if (AttackCurDur > 0){
                AttackCurDur -= dt;
            }
            else {
                AttackCurDur = 0;
                Daggers.frame = 0;
                WeaponData.source.x = Weapons[0].width / 2 * Daggers.frame;
                Daggers.runningTime = 0;
            }
            // Attack CD Update
            if (PlayerAttackCurCD > 0)
            {
                PlayerAttackCurCD -= dt;
            }
            else
                PlayerAttackCurCD = 0;
            // Attack Chain Update
            if (ChainAttackCurTimer > 0)
            {
                ChainAttackCurTimer -= dt;
            }
            else
                ChainAttackCurTimer = 0;
            if (ChainAttackCurTimer == 0) ChainAttackNum = 0;
            // Slashes Frame Update
            DaggerSlashes.source.x = DaggerSlashes.source.width * (ChainAttackNum - 1);
            if ((ChainAttackNum - 1) == 2) ChainAttackNum = 0;
            // Slashes Update Pos
            DaggerSlashes.dest.x = WeaponData.dest.x;
            DaggerSlashes.dest.y = WeaponData.dest.y;
            Vector2 DaggerSlashesDestGrower = Vector2Add({DaggerSlashes.dest.x, DaggerSlashes.dest.y}, Vector2Scale(DaggerSlashesDirection, 50));
            DaggerSlashes.dest.x = DaggerSlashesDestGrower.x;
            DaggerSlashes.dest.y = DaggerSlashesDestGrower.y;
            // Slashes Timer Update
            if (DaggerSlashesCurExistanceTime > 0)
            {
                DaggerSlashesCurExistanceTime -= dt;
            }
            else
                DaggerSlashesCurExistanceTime = 0;
            }
            // Debug Camera And Pos
            Player.pos = CheckAndUpdatePos(PlayerPosOld, Player.pos, TileType, TileSize, Player.size, PlayerStatus);
            Camera.target = CheckCamera(Camera.target, TileSize, WindowDimensions[0], WindowDimensions[1]);
            if(GameEndCond == 0){
                for (int i = 0; i < 8; i++){
                    if (Wolves[i].isAlive == true)
                    {
                        // Update Wolf Data
                        Wolves[i].wolfToPLayer = GetDirectionVect(Wolves[i].pos, Player.pos);
                        // Wolf AI

                        if (Wolves[i].agro == false)
                        {
                            if (Vector2Length(Vector2Subtract(Player.pos, Wolves[i].pos)) < 800)
                                Wolves[i].agro = true;
                        }
                        else{
                        if (Wolves[i].status == 3) // Wait After Attack
                        {
                            if (Wolves[i].Wait2.curTime > 0)
                            {
                                Wolves[i].Wait2.curTime -= dt;
                                Wolves[i].rec.x = WolfTexture.width / 12;
                            }
                            else
                            {
                                Wolves[i].Wait2.curTime = Wolves[i].Wait2.startTime;
                                Wolves[i].status = 0;
                            }
                        }
                        if (Wolves[i].status == 2) // Attack
                        {
                            if (Wolves[i].Attack.curTime > 0)
                            {
                                Wolves[i].pos = Vector2Add(Wolves[i].pos, Vector2Scale(Wolves[i].AttackDirection, WolfDashSpeed * dt));
                                Wolves[i].Attack.curTime -= dt;
                            }
                            else
                            {
                                Wolves[i].Attack.curTime = Wolves[i].Attack.startTime;
                                Wolves[i].status = 3;
                            }
                        }
                        if (Wolves[i].status == 1) // Wait Before Attack
                        {
                            if (Wolves[i].Wait1.curTime > 0)
                            {
                                Wolves[i].Wait1.curTime -= dt;
                                Wolves[i].rec.x = (WolfTexture.width / 12) * 6;
                            }
                            else
                            {
                                Wolves[i].Wait1.curTime = Wolves[i].Wait1.startTime;
                                Wolves[i].AttackDirection = Wolves[i].wolfToPLayer;
                                Wolves[i].status = 2;
                            }
                        }   
                        else if (Wolves[i].status == 0)
                        {
                            WolfRunToPlayer(&Wolves[i], WolfMoveSpeed, dt);
                            Wolves[i].AnimDataRun = UpdateAnimData(Wolves[i].AnimDataRun, dt);
                            Wolves[i].rec.x = (WolfTexture.width / 12) * Wolves[i].AnimDataRun.frame;
                            if (Vector2Length(Vector2Subtract(Player.pos, Wolves[i].pos)) < 300)
                            {
                                Wolves[i].status = 1;
                            }
                        }
                        
                        }
                        Wolves[i].rec.width = Wolves[i].rightLeft * WolfTexture.width / 11;
                    }
                }
            }
            // Player Screen Position Update
            PlayerScreenPos.x = WindowDimensions[0] / 2 + (WindowDimensions[0] / 2 - ((Camera.target.x + WindowDimensions[0] / 2) - Player.pos.x));
            PlayerScreenPos.y = WindowDimensions[1] / 2 + (WindowDimensions[1] / 2 - ((Camera.target.y + WindowDimensions[1] / 2) - Player.pos.y));

            // Updating Weapon Pos
            WeaponData.dest.x = Player.pos.x;
            WeaponData.dest.y = Player.pos.y + 12;

            // Player And Weapon RightLeft Update
            Player.rightLeft = MousePos.x >= PlayerScreenPos.x ? 1 : -1;
            Player.rec.width = Player.rightLeft * PlayerTexture.width / 6;
            WeaponData.source.width = Player.rightLeft * PlayerTexture.width / 6;

            // Wolf RightLeft Update
            

            // Update Rotation Vector
            RotationVector = GetDirectionVect(PlayerScreenPos, {Cursor.dest.x, Cursor.dest.y});

            // Update Weapon Rotation Vector
            WeaponData.rotation = GetVectRotationDegree(RotationVector);
            if (Player.rightLeft == -1)
            {
                WeaponData.rotation = WeaponData.rotation + 180;
            }

            if(WeaponType == 0){
                WeaponIconScale1 = 4;
                WeaponIconScale2 = 3;
            }
            else{
                WeaponIconScale1 = 3;
                WeaponIconScale2 = 4;
            }

            // Update Colisions

            //Player
            PlayerCol.x = Player.pos.x - Player.size.x / 2 + PlayerColPad * 1.6;
            PlayerCol.y = Player.pos.y - Player.size.y / 2 + PlayerColPad * 0.75 + 7;
            PlayerCol.width = Player.size.x - PlayerColPad * 3.4;
            PlayerCol.height = Player.size.y - PlayerColPad * 1.5;

            //Slashes
            if(ChainAttackNum == 3){
                DaggerDamage = 30;
                DaggerSlashesCol.x = DaggerSlashes.dest.x - DaggerSlashes.dest.width / 2 - DaggerSlashesColPad * 0.5;
                DaggerSlashesCol.y = DaggerSlashes.dest.y - DaggerSlashes.dest.height / 2 - DaggerSlashesColPad * 0.5;
                DaggerSlashesCol.width = DaggerSlashes.dest.width + DaggerSlashesColPad;
                DaggerSlashesCol.height = DaggerSlashes.dest.height + DaggerSlashesColPad;
            }
            else
            {
                DaggerDamage = 20;
                DaggerSlashesCol.x = DaggerSlashes.dest.x - DaggerSlashes.dest.width / 2;
                DaggerSlashesCol.y = DaggerSlashes.dest.y - DaggerSlashes.dest.height / 2;
                DaggerSlashesCol.width = DaggerSlashes.dest.width;
                DaggerSlashesCol.height = DaggerSlashes.dest.height;
            }

            // Arrow
            ArrowCol.x = Arrow.dest.x - Arrow.dest.width / 10;
            ArrowCol.y = Arrow.dest.y - Arrow.dest.height / 10;
            ArrowCol.width = Arrow.dest.width / 5;
            ArrowCol.height = Arrow.dest.height / 5;
            ArrowColDestGrower = Vector2Add({ArrowCol.x, ArrowCol.y}, Vector2Scale(ArrowDirection, ArrowFlySpeed * dt * 2.8));
            ArrowCol.x = ArrowColDestGrower.x;
            ArrowCol.y = ArrowColDestGrower.y;

            // Wolves
            for (int i = 0; i < 8; i++){
                Wolves[i].Col.x = Wolves[i].pos.x - Wolves[i].size.x / 2 + 28;
                Wolves[i].Col.y = Wolves[i].pos.y - Wolves[i].size.y / 2 + 45;
                Wolves[i].Col.width = Wolves[i].size.x - 45;
                Wolves[i].Col.height = Wolves[i].size.y - 45;
            }

            // Colisions Checkers
            for (int i = 0; i < 8; i++){
                if (CheckCollisionRecs(ArrowCol, Wolves[i].Col) == true && ArrowHitAnEnemy == false && ArrowCurFlyTime > 0 && WeaponType == 1)
                {
                    Wolves[i].hp -= ArrowDamage;
                    ArrowHitAnEnemy = true;
                    Wolves[i].Hit.curTime = Wolves[i].Hit.startTime;
                }
                if (CheckCollisionRecs(DaggerSlashesCol, Wolves[i].Col) == true && Wolves[i].IsHitByDaggers == false && WeaponType == 0 && DaggerSlashesCurExistanceTime > 0)
                {
                    Wolves[i].hp -= DaggerDamage;
                    Wolves[i].IsHitByDaggers = true;
                    Wolves[i].Hit.curTime = Wolves[i].Hit.startTime;
                }
                if (Wolves[i].Hit.curTime > 0)
                {
                    Wolves[i].ImTint = 1;
                    Wolves[i].Hit.curTime -= dt;
                }
                else
                {
                    Wolves[i].ImTint = 0;
                }
                if (CheckCollisionRecs(PlayerCol, Wolves[i].Col) == true && Wolves[i].status == 2 && PlayerInvunerable == false && PlayerHitTime.curTime == 0 && Wolves[i].isAlive == true)
                {
                    PlayerHitTime.curTime = PlayerHitTime.startTime;
                    Player.hp -= WolfDamage;
                    PlayerStatus = 2;
                }
                if (Wolves[i].hp <= 0)
                {
                    Wolves[i].isAlive = false;
                    WolvesDiedCount++;
                }
            }

            if (PlayerHitTime.curTime > 0)
            {
                PlayerTint = RED;
                PlayerInvunerable = true;
                PlayerHitTime.curTime -= dt;
                if (PlayerHitTime.curTime <= 0)
                {
                    PlayerHitTime.curTime = 0;
                    PlayerTint = WHITE;
                    PlayerInvunerable = false;
                }
            }

            if (Player.hp <= 0)
            {
                GameEndCond = 1;
            }
            else if (Wolves[0].isAlive + Wolves[1].isAlive + Wolves[2].isAlive + Wolves[3].isAlive + Wolves[4].isAlive + Wolves[5].isAlive + Wolves[6].isAlive + Wolves[7].isAlive == 0)
            {
                GameEndCond = 2;
            }
            else{
                GameEndCond = 0;
            }

            BeginMode2D(Camera);
            DrawTextureEx(TileMap, TileMapPos, 0, MapScale, WHITE);
            DrawTexturePro(PlayerTexture, Player.rec, {Player.pos.x, Player.pos.y, Player.size.x, Player.size.y}, Player.origin, Player.rotation, PlayerTint);
            // DrawRectangle(PlayerCol.x, PlayerCol.y, PlayerCol.width, PlayerCol.height, RED);
            DrawTexturePro(Weapons[WeaponType], WeaponData.source, WeaponData.dest, WeaponData.origin, WeaponData.rotation, WHITE);
            for (int i = 0; i < 8; i++){
                if (Wolves[i].isAlive == true)
                {
                    DrawTexturePro(WolfTexture, Wolves[i].rec, {Wolves[i].pos.x, Wolves[i].pos.y, Wolves[i].size.x, Wolves[i].size.y}, Wolves[i].origin, Wolves[i].rotation, WolfTint[Wolves[i].ImTint]);
                    // DrawRectangle(Wolves[i].Col.x, Wolves[i].Col.y, Wolves[i].Col.width, Wolves[i].Col.height, VIOLET);
                }
            }
            DrawTextureEx(FrontTrees, TileMapPos, 0, MapScale, WHITE);
            if (ArrowCurFlyTime > 0 && ArrowHitAnEnemy == false)
            {
                DrawTexturePro(ArrowTexture, Arrow.source, Arrow.dest, Arrow.origin, Arrow.rotation, WHITE);
                // DrawRectangle(ArrowCol.x, ArrowCol.y, ArrowCol.width, ArrowCol.height, GREEN);
            }
            if (DaggerSlashesCurExistanceTime > 0)
            {
                DrawTexturePro(DaggerSlashesTexture, DaggerSlashes.source, DaggerSlashes.dest, DaggerSlashes.origin, DaggerSlashes.rotation, WHITE);
                // DrawRectangle(DaggerSlashesCol.x, DaggerSlashesCol.y, DaggerSlashesCol.width, DaggerSlashesCol.height, BLUE);
            }
            // UI
            for (int i = 0; i < 8; i++)
            {
                if (Wolves[i].isAlive == true)
                {
                    DrawEnemyHealthBar(Wolves[i], Wolves[i].hp, WolfMaxHp, HealthBarBackground, HealthBarHealth);
                }
            }
            DrawPlayerHealthBar(Player.hp, Player.maxHp, WindowDimensions[0], WindowDimensions[1], HealthBarBackground, HealthBarHealth, Camera.target);
            DrawWeaponIcon1(WindowDimensions[0], WindowDimensions[1], DaggerIcon, Camera.target, WeaponIconScale1);
            DrawWeaponIcon2(WindowDimensions[0], WindowDimensions[1], BowIcon, Camera.target, WeaponIconScale2);
            DrawTexture(Controls[ControlsForm], Camera.target.x - WindowDimensions[0] / 2, Camera.target.y - WindowDimensions[1] / 2 + 32, WHITE);
            if(GameEndCond == 2){
                DrawText("YOU WON!", Camera.target.x - WindowDimensions[0] / 6 + 30, Camera.target.y - 200, 130, GREEN);
                DrawText("Press ESC to exit", Camera.target.x - WindowDimensions[0] / 7 - 20, Camera.target.y + 50, 70, GREEN);
            }
            if (GameEndCond == 1)
            {
                DrawText("YOU'VE LOST!", Camera.target.x - WindowDimensions[0] / 6 - 80, Camera.target.y - 200, 130, RED);
                DrawText("Reload save or press ESC to exit", Camera.target.x - WindowDimensions[0] / 4 - 50, Camera.target.y + 50, 70, RED);
            }
            EndMode2D();
        }

        DrawTexturePro(CursorTexture, Cursor.source, Cursor.dest, Cursor.origin, Cursor.rotation, WHITE);
        EndDrawing();
    }
    UnloadTexture(Button);
    UnloadTexture(Menu);
    UnloadTexture(PlayerTexture);
    UnloadTexture(TileMap);
    UnloadTexture(FrontTrees);
    UnloadTexture(CursorTexture);
    UnloadTexture(HealthBarBackground);
    UnloadTexture(HealthBarHealth);
    UnloadTexture(Weapons[0]);
    UnloadTexture(Weapons[1]);
    UnloadTexture(DaggerSlashesTexture);
    UnloadTexture(WolfTexture);
    UnloadTexture(ArrowTexture);
    UnloadTexture(DaggerIcon);
    UnloadTexture(BowIcon);
    UnloadTexture(Controls[0]);
    UnloadTexture(Controls[1]);
    CloseWindow();
}