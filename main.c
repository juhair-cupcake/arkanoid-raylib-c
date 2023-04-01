//#include <stdio.h>
#include "raylib.h"

//Const
#define PLAYER_LIFES 5
#define BRICKS_LINES 5
#define BRICKS_PER_LINE 20
#define BRICKS_POSITION_Y 50

typedef enum GameScreen {LOGO, TITLE, GAMEPLAY, ENDING} GameScreen;

//DataType
typedef struct Player {
  Vector2 position;
  Vector2 speed;
  Vector2 size;
  Rectangle bounds;
  int lifes;
} Player;
typedef struct Ball {
  Vector2 position;
  Vector2 speed;
  float radius;
  bool active;
} Ball;
typedef struct Brick {
  Vector2 position;
  Vector2 size;
  Rectangle bounds;
  int resistance;
  bool active;
} Brick;

void main() {
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "BLOCKS GAME");
  GameScreen screen = LOGO;

  //initialize
  int frameCounter = 0, gameResult = -1;
  bool gamePaused = false;

  Player player = { 0 };
  player.position = (Vector2){ screenWidth/2, screenHeight*7/8 };
  player.speed = (Vector2){ 8.0f, 0.0f };
  player.size = (Vector2){ 100, 24 };
  player.lifes = PLAYER_LIFES;
  
  Ball ball = { 0 };
  ball.active = false;
  ball.radius = 10.0f;
  ball.position = (Vector2){ player.position.x + player.size.x/2, player.position.y - ball.radius*2 };
  ball.speed = (Vector2){ 4.0f, 4.0f };
  
  Brick bricks[BRICKS_LINES][BRICKS_PER_LINE] = { 0 };
  //i=y, j=x;
  for (int y=0; y<BRICKS_LINES; y++){
    for(int x=0; x<BRICKS_PER_LINE; x++){
      bricks[y][x].size = (Vector2){ screenWidth/BRICKS_PER_LINE, 20};
      bricks[y][x].position = (Vector2){ x*bricks[y][x].size.x, y*bricks[y][x].size.y + BRICKS_POSITION_Y };
      bricks[y][x].bounds = (Rectangle){ bricks[y][x].position.x, bricks[y][x].position.y, bricks[y][x].size.x, bricks[y][x].size.y };
      bricks[y][x].active = true;
    }
  }


  SetTargetFPS(60);
  while(!WindowShouldClose()) {
    frameCounter++;
    if(frameCounter > 120) frameCounter = 0;
    
    //screen state
    if (screen == LOGO) {
      if(frameCounter > 80) {
        screen = TITLE;
        frameCounter = 0;
      }
    }
    else if (screen == TITLE) {
      if(IsKeyPressed(KEY_ENTER)) screen = GAMEPLAY;
    }
    else if (screen == GAMEPLAY) {
      if(!gamePaused){}
    }
    else if (screen == ENDING) {
      if(IsKeyPressed(KEY_ENTER)) screen = TITLE;
    }

    //Let's Draw
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    if (screen == LOGO) {
      DrawText("LOGO", 20, 20, 80, LIGHTGRAY);
    }
    else if (screen == TITLE) {
      DrawText("Yo Yo Chiki Chiki", 20, 20, 40, DARKGREEN);
      if ((frameCounter/30)%2 == 0) {
        char *text = "Press [ENTER] to Start";
        int fontSize = 20;
        DrawText(text, GetScreenWidth()/2 - MeasureText(text, fontSize)/2, GetScreenHeight()/2 + 60, fontSize, DARKGREEN);
      }
    } 
    else if (screen == GAMEPLAY) {
      DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, BLACK);
      DrawCircleV(ball.position, ball.radius, MAROON);

      //draw brick
      for(int y=0; y<BRICKS_LINES; y++){
        for(int x=0; x<BRICKS_PER_LINE; x++){
          if(bricks[y][x].active) {
            if((x+y)%2 == 0) {
              DrawRectangle(bricks[y][x].position.x, bricks[y][x].position.y, bricks[y][x].size.x, bricks[y][x].size.y, GRAY);
            }
            else {
              DrawRectangle(bricks[y][x].position.x, bricks[y][x].position.y, bricks[y][x].size.x, bricks[y][x].size.y, DARKGRAY);
            }
          }
        }
      }

      //GUI
      //playerLife
      for(int i=0; i<player.lifes; i++) DrawRectangle(20 + 40*i, screenHeight-30, 35, 10, LIGHTGRAY);

      char *pauseText = "PAUSED";
      if(gamePaused) DrawText(pauseText, screenWidth/2 - MeasureText(pauseText, 40)/2, screenHeight/2 + 60, 40, GRAY);
    }
    else if (screen == ENDING) {
      DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
      char *endText = "Press [ENTER] to Play Again";
      if((frameCounter/30)%2 == 0) DrawText(endText, GetScreenWidth()/2 - MeasureText(endText, 20)/2, GetScreenHeight()/2 +80, 20, GRAY);
    }
    EndDrawing();
    
  }
  CloseWindow();
}
