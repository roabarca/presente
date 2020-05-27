#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include <math.h>

#include "level.h"
#include "draw.h"
#include "state.h"
#include "file.h"

int main(int argc, char const *argv[]){

    // Initialization
    const int screen_width = 800;
    const int screen_height = 600;

    InitWindow(screen_width,screen_height,"Presente - the game");

    InitAudioDevice(); // Initialize audio device
    //Cargamos los distintos archivos de musica
    Music fondo = LoadMusicStream("resources/YouSayRun.mp3"); 
    Music victoria = LoadMusicStream("resources/FFVI_Victory_Fanfare.ogg");
    Music derrota = LoadMusicStream("resources/FFIV_DS_Dungeon_Theme.ogg");
    //Iniciamos la musica de fondo
    PlayMusicStream(fondo);

    SetTargetFPS(60);

    // Initialize level and fill randomly
    level *lvl = level_new(50,40);
    level_fill_random(lvl,6);

    // Initialize state (and add enemies)
    state *sta = state_new();
    state_populate_random(lvl,sta,40);

    // Initialize highscore and score
    int highscore;
    int score = 0;
    highscore = puntajeMaximo();

    // Main loop
    while(!WindowShouldClose()){

        // Update input depending if keys are pressed or not
        sta->button_state[0] = IsKeyDown(KEY_D);
        sta->button_state[1] = IsKeyDown(KEY_W);
        sta->button_state[2] = IsKeyDown(KEY_A);
        sta->button_state[3] = IsKeyDown(KEY_S);
        sta->button_state[4] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        // Compute the angle the player is looking at from the mouse's position relative to the screen's center
        float mouse_y = GetMouseY()-GetScreenHeight()/2;
        float mouse_x = GetMouseX()-GetScreenWidth()/2;
        sta->aim_angle = atan2(-mouse_y,mouse_x);

        // Update the state
        state_update(lvl,sta,&score);

        // Update the music
        UpdateMusicStream(fondo);
        UpdateMusicStream(victoria);
        UpdateMusicStream(derrota);

        // Drawing
        BeginDrawing();

            ClearBackground(RAYWHITE);
            draw_state(lvl, sta);
            DrawText(FormatText("HIGH SCORE: %08i ", highscore) ,10,10,20,GREEN);
            DrawText(FormatText("CURRENT SCORE: %08i ", score) ,10,40,20,YELLOW);
            DrawText(FormatText("MONSTER: %02i ", sta->n_enemies),600,540,20,BLACK);
            DrawText("Presente profe!",190,200,20,LIGHTGRAY);
            DrawText("Press F1 to Lose",600,560,20,BLACK);
            DrawText("Press F2 to Win!",600,580,20,BLACK);

        EndDrawing();

        if (sta->n_enemies == 0){ //Si no quedan enemigos en state (cuando ganas)
            StopMusicStream(fondo);
            PlayMusicStream(victoria);
            DrawText("VICTORIA",300,150,50,BLACK);
            if (score > highscore){
                cambiarPuntaje(&score);
            }
        }
         if (sta->pla.ent.dead){ //Si la hp del play es igual o menos a 0 (cuando pierdes)
            StopMusicStream(fondo);
            PlayMusicStream(derrota);
            DrawText("DERROTA",300,150,50,BLACK);
            if (score >= highscore){
                cambiarPuntaje(&score);
            }
        }
        if (score>=highscore){ //Si el score actual es mayor que el highscore
            highscore=score;
        }
    }

    // Closer window
    UnloadMusicStream(fondo);   // Unload music stream buffers from RAM
    UnloadMusicStream(victoria);
    UnloadMusicStream(derrota);
    CloseAudioDevice();       // Close audio device (music streaming is automatically stopped)
    CloseWindow();              // Close window and OpenGL context

    // Free memory
    state_free(sta);
    level_free(lvl);

    return 0;
}
