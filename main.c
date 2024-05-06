#include "raylib/include/raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MARKER_SIZE 10.0f
#define SCREEN_FPS 60
#define DELAY_SEC (1.0f/SCREEN_FPS)
#define DELAY_MS ((Uint32) floorf(DELAY_SEC * 1000.0f))
//vact2 constructor
Vector2 vec2_constr(float a , float b){
    return (Vector2){a , b };
}

// vect2 operataions
Vector2 vec2_sub(Vector2 a , Vector2 b){
    return vec2_constr(a.x - b.x, a.y - b.y);
}

Vector2 vec2_add(Vector2 a , Vector2 b){
    return vec2_constr(a.x + b.x, a.y + b.y);
}

Vector2 vec2_scale(Vector2 a , float s){
    return vec2_constr(a.x * s, a.y * s);
}

// lerp function 1d
float lerpf(float a , float b , float p ){
    return a + (b - a) * p ;
}

//lerp fuction 2d
Vector2 lerpv2(Vector2 a , Vector2 b , float p){
    return vec2_add(a, vec2_scale(vec2_sub(b, a), p));
}

// marker fucntion 
void render_marker(Vector2 position , Color color){
    Vector2 size = vec2_constr(MARKER_SIZE, MARKER_SIZE);

    DrawRectangleV(
        vec2_sub(position , vec2_scale(size, 0.5))
                   , size, color);
}
// bezier 
void render_bezier_markers(Vector2 a , Vector2 b ,
                           Vector2 c , Vector2 d ,
                           float s){
    for(float p = 0.0f ; p <= 1.0f ; p+= s){
        Vector2 ab = lerpv2(a, b, p);
        Vector2 bc = lerpv2(b, c, p);
        Vector2 cd = lerpv2(c, d, p);
        Vector2 abc = lerpv2(ab, bc, p);
        Vector2 bcd  = lerpv2(bc, cd, p);
        Vector2 abcd = lerpv2(abc, bcd, p);
        render_marker(abcd, PURPLE);
    }
}
// ps

#define PS_CAPACITY 256
Vector2 ps[PS_CAPACITY];
size_t ps_count = 0;
int ps_selected = -1;

// ps at 
int ps_at(Vector2 pos){
    const Vector2 ps_size = vec2_constr(MARKER_SIZE, MARKER_SIZE);
    for(size_t i = 0 ; i < ps_count ; i++){
        const Vector2 ps_begin = vec2_sub(ps[i], vec2_scale(ps_size, 0.5f)) ;
        const Vector2 ps_end = vec2_add(ps_begin , ps_size);
        if(ps_begin.x   <= pos.x && pos.x <= ps_end.x
           && ps_begin.y<= pos.y && pos.y <= ps_end.y
        )
        {   
            return (int) i ;
        }
    }
    return -1 ;
}

// main funciton 
int main() {
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Quadratic Bezier Curve");

    Vector2 begin = {100.0f, 100.0f};
    Vector2 end = { 250.0f, 400.0f };
    int j = 0; 
    SetTargetFPS(SCREEN_FPS);
    float time = 0.0f;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            Vector2 mouse_pos = GetMousePosition();
            if(ps_count<4){
                ps[ps_count++] = vec2_constr(mouse_pos.x, mouse_pos.y);
            }else {
                ps_selected = ps_at(mouse_pos);
                printf("ps_selected : %d\n" , ps_selected);
            }
        }
        else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            Vector2 mouse_pos = GetMousePosition();
            if(ps_selected != -1){
               ps[ps_selected] = mouse_pos; 
            }
        }
        const float p =  (sinf(time) + 1.0f) * 0.5f ; 
        for(size_t i = 0 ; i < ps_count ; i++){
            render_marker(ps[i], RED);
        }
        if (ps_count >= 4) {
            render_bezier_markers(ps[0],ps[1],ps[2],ps[3] ,0.05f);
        }
        EndDrawing(); 
        time += DELAY_SEC;
    }

    CloseWindow();

    return 0;
}
