//#include <stdio.h>
#include "raylib.h"

//Call Func
void initialize();
void update();
void draw();

//DataType
typedef enum GameScreen {LOGO, TITLE, GAMEPLAY, ENDING} GameScreen;
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

//Const
#define PLAYER_LIFES 5
#define BRICKS_LINES 5
#define BRICKS_PER_LINE 20
#define BRICKS_POSITION_Y 50
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

//globa Var Decleare
int frameCounter = 0, gameResult = -1;
bool gamePaused = false;
Player player = { 0 };
Ball ball = { 0 };
Brick bricks[BRICKS_LINES][BRICKS_PER_LINE] = { 0 };
GameScreen screen = LOGO;
Texture2D texLogo, texBall, texPaddle, texBrick;
Font font;
Sound fxStart, fxBounce, fxExplode;
//Music music;

void main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ARKANOID");
  InitAudioDevice();
  SetTargetFPS(60);
  initialize();
  
  //gameLoop
  while(!WindowShouldClose()) {
    frameCounter++;
    if(frameCounter > 120) frameCounter = 0;
    
    //screen state & MATHS
    update();
    //UpdateMusicStream(music);
    
    //Let's Draw
    BeginDrawing();
    draw();
    EndDrawing();
  }
  
  //Bye
  UnloadTexture(texLogo);
  UnloadTexture(texPaddle);
  UnloadTexture(texBall);
  UnloadTexture(texBrick);
  UnloadFont(font);
  //UnloadMusicStream(music);
  UnloadSound(fxStart);
  UnloadSound(fxBounce);
  UnloadSound(fxExplode);
  
  CloseAudioDevice();
  CloseWindow();
}

void initialize() {
  texLogo = LoadTexture("./resources/raylib_logo.png");
  texBall = LoadTexture("./resources/ball.png");
  texPaddle = LoadTexture("./resources/paddle.png");
  texBrick = LoadTexture("./resources/brick.png");
  font = LoadFont("./resources/setback.png");

  fxStart = LoadSound("./resources/start.wav");
  fxBounce = LoadSound("./resources/bounce.wav");
  fxExplode = LoadSound("./resources/explosion.wav");
  
  //music = LoadMusicStream("./resources/blockshock.mod");
  //PlayMusicStream(music);
  
  player.position = (Vector2){ SCREEN_WIDTH/2, SCREEN_HEIGHT*7/8 };
  player.speed = (Vector2){ 8.0f, 0.0f };
  player.size = (Vector2){ 100, 24 };
  player.lifes = PLAYER_LIFES;
  
  ball.active = false;
  ball.radius = 10.0f;
  ball.position = (Vector2){ player.position.x + player.size.x/2, player.position.y - ball.radius*2 };
  ball.speed = (Vector2){ 4.0f, 4.0f };
  
  for (int y=0; y<BRICKS_LINES; y++){
    for(int x=0; x<BRICKS_PER_LINE; x++){
      bricks[y][x].size = (Vector2){ SCREEN_WIDTH/BRICKS_PER_LINE, 20};
      bricks[y][x].position = (Vector2){ x*bricks[y][x].size.x, y*bricks[y][x].size.y + BRICKS_POSITION_Y };
      bricks[y][x].bounds = (Rectangle){ bricks[y][x].position.x, bricks[y][x].position.y, bricks[y][x].size.x, bricks[y][x].size.y };
      bricks[y][x].active = true;
    }
  }
}

void update() {
  if (screen == LOGO) {
    if(frameCounter > 80) {
      screen = TITLE;
      frameCounter = 0;
    }
  }
  else if (screen == TITLE) {
    if(IsKeyPressed(KEY_ENTER)){
     screen = GAMEPLAY; 
     PlaySound(fxStart);
    }
  }
  else if (screen == GAMEPLAY) {
    if(IsKeyPressed('p')) gamePaused = !gamePaused;
    if(!gamePaused){
      if(IsKeyDown(KEY_LEFT)) player.position.x -= player.speed.x;
      if(IsKeyDown(KEY_RIGHT)) player.position.x += player.speed.x;

      if(player.position.x <= 0) player.position.x = 0;
      if(player.position.x + player.size.x >= SCREEN_WIDTH) player.position.x = SCREEN_WIDTH - player.size.x;

      player.bounds = (Rectangle){ player.position.x, player.position.y, player.size.x, player.size.y };

      if(ball.active){
        ball.position.x += ball.speed.x;
        ball.position.y += ball.speed.y;
        if(((ball.position.x + ball.radius) >= SCREEN_WIDTH ) || ((ball.position.x - ball.radius) <= 0 )) ball.speed.x *= -1;
        if((ball.position.y + ball.radius) <= 0 ) ball.speed.y *= -1;

        //ball VS player
        if(CheckCollisionCircleRec(ball.position, ball.radius, player.bounds)){
          ball.speed.y *= -1;
          ball.speed.x = (ball.position.x - (player.position.x + player.size.x/2))/player.size.x*5.0f;
          PlaySound(fxBounce);
        }
        //ball VS bricks
        for (int y=0; y<BRICKS_LINES; y++){
          for(int x=0; x<BRICKS_PER_LINE; x++){
            if(bricks[y][x].active && (CheckCollisionCircleRec(ball.position, ball.radius, bricks[y][x].bounds))){
              bricks[y][x].active = false;
              ball.speed.y *= -1;
              PlaySound(fxExplode);

              break;
            }
          }
        }
        
        //Game Lose
        if((ball.position.y + ball.radius) >= SCREEN_HEIGHT){
          ball.position.x = player.position.x + player.size.x/2;
          ball.position.y = player.position.y - ball.radius - 1.0f;
          ball.speed = (Vector2){ 0, 0 };
          ball.active = false;
          
          player.lifes--;
        }

        if(player.lifes < 0) {
          screen = ENDING;
          player.lifes = PLAYER_LIFES;
          frameCounter = 0;
        }
      }
      else {
        ball.position.x = player.position.x + player.size.x/2;

        if(IsKeyPressed(KEY_SPACE)){
          ball.active = true;
          ball.speed = (Vector2){ 0, -5.0f };
        }
      }
    }
  }
  else if (screen == ENDING) {
    if(IsKeyPressed(KEY_ENTER)) screen = TITLE;
  }
}

void draw() {
  ClearBackground(RAYWHITE);
  
  if (screen == LOGO) {
    //DrawText("LOGO", 20, 20, 80, LIGHTGRAY);
    DrawTexture(texLogo, SCREEN_WIDTH/2 - texLogo.width/2, SCREEN_HEIGHT/2 - texLogo.height/2, WHITE);
  }
  else if (screen == TITLE) {
    //DrawText("PRESS TO PLAY", 20, 20, 40, DARKGREEN);
    DrawTextEx(font, "BLOCKS", (Vector2){100, 80}, 160, 10, MAROON);
  
    if ((frameCounter/30)%2 == 0) {
      char *text = "Press [ENTER] to Start";
      int fontSize = 20;
      DrawText(text, GetScreenWidth()/2 - MeasureText(text, fontSize)/2, GetScreenHeight()/2 + 60, fontSize, DARKGREEN);
    }
  } 
  else if (screen == GAMEPLAY) {
    //DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, BLACK);
    DrawTextureEx(texPaddle, player.position, 0.0f, 1.0f, WHITE);
    //DrawCircleV(ball.position, ball.radius, MAROON);
    DrawTexture(texBall, ball.position.x - ball.radius/2, ball.position.y - ball.radius/2, MAROON);

    //draw brick
    for(int y=0; y<BRICKS_LINES; y++){
      for(int x=0; x<BRICKS_PER_LINE; x++){
        if(bricks[y][x].active) {
          if((x+y)%2 == 0) {
            //DrawRectangle(bricks[y][x].position.x, bricks[y][x].position.y, bricks[y][x].size.x, bricks[y][x].size.y, GRAY);
            DrawTextureEx(texBrick, bricks[y][x].position, 0.0f, 1.0f, GRAY);
          }
          else {
            //DrawRectangle(bricks[y][x].position.x, bricks[y][x].position.y, bricks[y][x].size.x, bricks[y][x].size.y, DARKGRAY);
            DrawTextureEx(texBrick, bricks[y][x].position, 0.0f, 1.0f, DARKGRAY);
          }
        }
      }
    }

    //GUI
    //playerLife
    for(int i=0; i<player.lifes; i++) DrawRectangle(20 + 40*i, SCREEN_HEIGHT-30, 35, 10, LIGHTGRAY);

    char *pauseText = "PAUSED";
    if(gamePaused) DrawText(pauseText, SCREEN_WIDTH/2 - MeasureText(pauseText, 40)/2, SCREEN_HEIGHT/2 + 60, 40, GRAY);
  }
  else if (screen == ENDING) {
    DrawTextEx(font, "GAME OVER", (Vector2){80, 100}, 80, 6, MAROON);
    
    char *endText = "Press [ENTER] to Play Again";
    if((frameCounter/30)%2 == 0) DrawText(endText, GetScreenWidth()/2 - MeasureText(endText, 20)/2, GetScreenHeight()/2 +80, 20, GRAY);
  }
}
