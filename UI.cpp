#include "raylib.h"
#include "header.h"
#include "raymath.h"
#include <string>
#include <cstdio>
#include <cmath>

bool DrawMenu(float WindowWidth, float WindowHeight, Texture2D Menu, Texture2D Button, bool SaveExist, Vector2 MousePos, bool *LoadGameCheck)
{
    Button.height = Button.height * 3;
    Button.width = Button.width * 3;

    ButtonData ButtonContinue{};
    if (SaveExist == false)
    {
        ButtonContinue.frame = 2;
        ButtonContinue.textcolor = DarkBrown;
    }
    else
    {
        ButtonContinue.frame = 0;
        ButtonContinue.textcolor = Green;
    }
    ButtonContinue.rec.width = Button.width;
    ButtonContinue.rec.height = Button.height / 3;
    ButtonContinue.rec.x = 0;
    ButtonContinue.rec.y = ButtonContinue.rec.height * ButtonContinue.frame;
    ButtonContinue.pos.x = WindowWidth / 2 - ButtonContinue.rec.width / 2;
    ButtonContinue.pos.y = WindowHeight / 2 + ButtonContinue.rec.height * (-0.35);

    ButtonData ButtonNewGame{};
    ButtonNewGame.rec.width = Button.width;
    ButtonNewGame.rec.height = Button.height / 3;
    ButtonNewGame.rec.x = 0;
    ButtonNewGame.rec.y = 0;
    ButtonNewGame.pos.x = WindowWidth / 2 - ButtonNewGame.rec.width / 2;
    ButtonNewGame.pos.y = WindowHeight / 2 + ButtonContinue.rec.height * 0.90;
    ButtonNewGame.frame = 0;
    ButtonNewGame.textcolor = Green;

    if (CheckCollisionPointRec(MousePos, (Rectangle){ButtonContinue.pos.x, ButtonContinue.pos.y, ButtonContinue.rec.width, ButtonContinue.rec.height}) && SaveExist == true)
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            ButtonContinue.frame = 2;
            ButtonContinue.rec.y = ButtonContinue.rec.height * ButtonContinue.frame;
        }
        else
        {
            ButtonContinue.frame = 1;
            ButtonContinue.rec.y = ButtonContinue.rec.height * ButtonContinue.frame;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            *LoadGameCheck = true;
            return false;
        }
    }
    else
    {
        ButtonContinue.frame = 0;
    }

    if (CheckCollisionPointRec(MousePos, (Rectangle){ButtonNewGame.pos.x, ButtonNewGame.pos.y, ButtonNewGame.rec.width, ButtonNewGame.rec.height}))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            ButtonNewGame.frame = 2;
            ButtonNewGame.rec.y = ButtonNewGame.rec.height * ButtonNewGame.frame;
        }
        else
        {
            ButtonNewGame.frame = 1;
            ButtonNewGame.rec.y = ButtonNewGame.rec.height * ButtonNewGame.frame;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            return false;
        }
    }
    else
    {
        ButtonNewGame.frame = 0;
    }

    Vector2 MenuPos{0, 0};
    DrawTextureEx(Menu, MenuPos, 0, 12, WHITE);
    DrawTextureRec(Button, ButtonContinue.rec, ButtonContinue.pos, WHITE);
    DrawTextureRec(Button, ButtonNewGame.rec, ButtonNewGame.pos, WHITE);
    DrawText("WOLF HUNTER", WindowWidth / 4, WindowHeight / 6, 130, GREEN);
    DrawText("Continue", ButtonContinue.pos.x + ButtonContinue.rec.width / 7, ButtonContinue.pos.y + ButtonContinue.rec.height / 3.3, 80, ButtonContinue.textcolor);
    DrawText("New Game", ButtonNewGame.pos.x + ButtonNewGame.rec.width / 9.5, ButtonNewGame.pos.y + ButtonNewGame.rec.height / 3.3, 80, ButtonNewGame.textcolor);

    return true;
}


void DrawPlayerHealthBar(int HP, int MaxHP, int WindowWidth, int WindowHeight, Texture2D HealthBarBackground,Texture2D HealthBarHealth, Vector2 CameraPos)
{
    Vector2 CameraRelativeOrigin{CameraPos.x - WindowWidth / 2, CameraPos.y - WindowHeight / 2};
    float HPpercentage = ((float)HP / (float)MaxHP) * 100;
    float Scale = 2;
    Rectangle HealthBarBackgroundSource{0, 0, HealthBarBackground.width, HealthBarBackground.height};
    Rectangle HealthBarBackgroundDest{CameraRelativeOrigin.x + WindowWidth/16, CameraRelativeOrigin.y + WindowHeight / 12, HealthBarBackground.width * Scale, HealthBarBackground.height * Scale};
    Rectangle HealthBarHealthSource{0, 0, ((float)HealthBarHealth.width / 100) * HPpercentage, HealthBarHealth.height};
    Rectangle HealthBarHealthDest{CameraRelativeOrigin.x + WindowWidth/16, CameraRelativeOrigin.y + WindowHeight / 12, HealthBarHealthSource.width * Scale, HealthBarHealthSource.height * Scale};
    Vector2 BarOrigin{0, 0};
    float BarRotation = 0;
    DrawTexturePro(HealthBarBackground, HealthBarBackgroundSource, HealthBarBackgroundDest, BarOrigin, BarRotation, WHITE);
    DrawTexturePro(HealthBarHealth, HealthBarHealthSource, HealthBarHealthDest, BarOrigin, BarRotation, WHITE);
}

void DrawWeaponIcon1(int WindowWidth, int WindowHeight, Texture2D Icon, Vector2 CameraPos, float Scale)
{
    Vector2 CameraRelativeOrigin{CameraPos.x - WindowWidth / 2, CameraPos.y - WindowHeight / 2};
    Rectangle IconSource{0, 0, Icon.width, Icon.height};
    Rectangle IconDest{CameraRelativeOrigin.x + WindowWidth * 1.6 / 16, CameraRelativeOrigin.y + WindowHeight * 9 / 12, Icon.width * Scale, Icon.height * Scale};
    Vector2 IconOrigin{IconDest.width / 2, IconDest.height / 2};
    float IconRotation = 0;
    DrawTexturePro(Icon, IconSource, IconDest, IconOrigin, IconRotation, WHITE);
}

void DrawWeaponIcon2(int WindowWidth, int WindowHeight, Texture2D Icon, Vector2 CameraPos, float Scale)
{
    Vector2 CameraRelativeOrigin{CameraPos.x - WindowWidth / 2, CameraPos.y - WindowHeight / 2};
    Rectangle IconSource{0, 0, Icon.width, Icon.height};
    Rectangle IconDest{CameraRelativeOrigin.x + WindowWidth * 2.8 / 16, CameraRelativeOrigin.y + WindowHeight * 10.5 / 12, Icon.width * Scale, Icon.height * Scale};
    Vector2 IconOrigin{IconDest.width / 2, IconDest.height / 2};
    float IconRotation = 0;
    DrawTexturePro(Icon, IconSource, IconDest, IconOrigin, IconRotation, WHITE);
}

void DrawEnemyHealthBar(Wolf Wolf, float HP, float MaxHP, Texture2D HealthBarBackground, Texture2D HealthBarHealth){
    float HPpercentage = ((float)HP / (float)MaxHP) * 100;
    float Scale = 0.7;
    Rectangle HealthBarBackgroundSource{0, 0, HealthBarBackground.width, HealthBarBackground.height};
    Rectangle HealthBarBackgroundDest{Wolf.pos.x, Wolf.pos.y - 50, HealthBarBackground.width * Scale, HealthBarBackground.height * Scale};
    Rectangle HealthBarHealthSource{0, 0, ((float)HealthBarHealth.width / 100) * HPpercentage, HealthBarHealth.height};
    Rectangle HealthBarHealthDest{Wolf.pos.x, Wolf.pos.y - 50, HealthBarHealthSource.width * Scale, HealthBarHealthSource.height * Scale};
    Vector2 BarOrigin{HealthBarBackgroundDest.width / 2, HealthBarBackgroundDest.height / 2};
    float BarRotation = 0;
    DrawTexturePro(HealthBarBackground, HealthBarBackgroundSource, HealthBarBackgroundDest, BarOrigin, BarRotation, WHITE);
    DrawTexturePro(HealthBarHealth, HealthBarHealthSource, HealthBarHealthDest, BarOrigin, BarRotation, WHITE);
}