#include <stdio.h>
#include "raylib.h"

typedef enum GameScreen {LOGO, TITLE, GAMEPLAY, ENDING} GameScreen;

void main() {
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Hello");
  GameScreen screen = LOGO;

  int frameCounter = 0, gameResult = -1;
  bool gamePaused = false;

  SetTargetFPS(60);
  while(!WindowShouldClose()) {
    if (screen == LOGO) {
      frameCounter++;
      if(frameCounter > 180) {
        screen = TITLE;
        frameCounter = 0;
      }
    }
    else if (screen == TITLE) {
      frameCounter++;
      if(IsKeyPressed(KEY_ENTER)) screen = ENDING;
    }
    else if (screen == GAMEPLAY) {
      printf("\n~\n%ld\n~\n", sizeof(bool));
    }
    else if (screen == ENDING) {
      frameCounter++;
      if(IsKeyPressed(KEY_ENTER)) screen = TITLE;
    }

    //Draw shit...
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    if (screen == LOGO) {
      DrawText("LOGO", 20, 20, 80, LIGHTGRAY);
      DrawText("Wait for 30s", 290, 220, 20, GRAY);
    }
    else if (screen == TITLE) {
      DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
      DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
      DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
    } 
    else if (screen == GAMEPLAY) {
      DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
      DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
      DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
    }
    else if (screen == ENDING) {
      DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
      DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
      DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);
    }
    EndDrawing();
    
  }
  CloseWindow();
}
