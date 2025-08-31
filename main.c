#include <raylib.h>
#include <raymath.h>
#include <stdint.h>
#include <stdlib.h>

int W = 800, H = 600;
int BW = 3000, BH = 3000;
float scale = 1.0f;
float brushsize = 4.0f;
Color brushcolor = WHITE;
Vector3 HSV;
long brushtype = 0;

void draw_background() {
    ClearBackground(DARKGRAY);
    for (float i = 0, ii = 0; i < W; i += scale * 10.f, ++ii) {
	for (float j = 0, ji = 0; j < H; j += scale * 10.f, ++ji) {
	    if ((!((int)ii % 2) && ((int)ji % 2)) || (((int)ii % 2) && !((int)ji % 2)))
		DrawRectangle(i, j, scale * 10.f, scale * 10.f, GRAY);
	}
    }
}

int main(void) {
    HSV = ColorToHSV(brushcolor);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(W, H, "Raylib");

    Image img = LoadImage("myimage.png");
    Texture2D tex = LoadTextureFromImage(img);
    SetTextureFilter(tex, TEXTURE_FILTER_BILINEAR);

    Vector2 pms = GetMousePosition();

    while (!WindowShouldClose()) {
        BeginDrawing();

	W = GetScreenWidth();
	H = GetScreenHeight();
        Vector2 ms = pms;
	if (!IsKeyDown(KEY_ONE)) {
	    ms.x = GetMouseX();
	}
	if (!IsKeyDown(KEY_TWO)) {
	    ms.y = GetMouseY();
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
	    switch (brushtype) {
	    case 0: {
		float x = pms.x;
		float y = pms.y;
		float s = Vector2Distance(pms, ms);
		float dx = (ms.x - pms.x) / s;
		float dy = (ms.y - pms.y) / s;
		for (float i = 0; i < s+.001f; ++i) {
		    ImageDrawCircle(&img, x, y, brushsize, brushcolor);
		    x += dx;
		    y += dy;
		}
		break;
	    }
	    case 1: {
		for (int i = -brushsize; i < brushsize; ++i) {
		    for (int j = -brushsize; j < brushsize; ++j) {
			if (ms.x+i >= 0 && ms.x+i < BW && ms.y+j >= 0 && ms.y+j < BH && i*i+j*j < brushsize*brushsize)
			    ((uint32_t *)img.data)[(size_t)ms.x+i+((size_t)ms.y+j)*BW] = 0;
		    }
		}
		break;
	    }
	    case 2: {
		float x = pms.x;
		float y = pms.y;
		float s = Vector2Distance(pms, ms);
		float dx = (ms.x - pms.x) / s;
		float dy = (ms.y - pms.y) / s;
		for (float i = 0; i < s+.001f; ++i) {
		    ImageDrawRectangle(&img, x-brushsize/2, y-brushsize/2, brushsize, brushsize, brushcolor);
		    x += dx;
		    y += dy;
		}
		break;
	    }
	    case 3: {
		float x = pms.x;
		float y = pms.y;
		float s = Vector2Distance(pms, ms);
		float dx = (ms.x - pms.x) / s;
		float dy = (ms.y - pms.y) / s;
		for (float i = 0; i < s+.001f; ++i) {
		    ImageDrawTriangle(&img, (Vector2) {x, y-brushsize}, (Vector2) {x-brushsize, y}, (Vector2) {x+brushsize, y}, brushcolor);
		    x += dx;
		    y += dy;
		}
		break;
	    }
	    }
	}
	else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
	    while (!WindowShouldClose()) {
		BeginDrawing();
		ms = GetMousePosition();
		draw_background();
		DrawRectangle(0, 0, 30, 12, GRAY);
		DrawRectangle(30, 0, 30, 12, BLUE);
		if (ms.x < 60 && ms.y < 12) {
		    DrawText("Eracer", 0, 0, 12, ORANGE);
		    if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			brushtype = 1;
		    }
		}
		else {
		    DrawText("Eracer", 0, 0, 12, RED);
		}
		DrawRectangle(0, 15, 60, 12, DARKGRAY);
		if (ms.x < 60 && ms.y < 27 && ms.y > 15) {
		    DrawText("Pencil", 0, 15, 12, ORANGE);
		    if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			brushtype = 0;
		    }
		}
		else {
		    DrawText("Pencil", 0, 15, 12, RED);
		}
		DrawRectangle(0, 29, 60, 12, BLUE);
		if (ms.x < 60 && ms.y < 29+12 && ms.y > 29) {
		    DrawText("Square", 0, 29, 12, ORANGE);
		    if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			brushtype = 2;
		    }
		}
		else {
		    DrawText("Square", 0, 29, 12, RED);
		}
		DrawRectangle(0, 45, 60, 12, BLUE);
		if (ms.x < 60 && ms.y < 45+12 && ms.y > 45) {
		    DrawText("Triangle", 0, 45, 12, ORANGE);
		    if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			brushtype = 3;
		    }
		}
		else {
		    DrawText("Triangle", 0, 45, 12, RED);
		}
		for (float i = 0; i < 360.f; ++i) {
		    DrawRectangle(W/2-50, H/2.f-360/2.f+i, 100, 1, ColorFromHSV(i, 1, 1));
		}
		for (float i = 0; i < 360.f; ++i) {
		    DrawRectangle(W/2+50, H/2.f-360/2.f+i, 100, 1, ColorFromHSV(0, i/360.f, 1));
		}
		for (float i = 0; i < 360.f; ++i) {
		    DrawRectangle(W/2+150, H/2.f-360/2.f+i, 100, 1, ColorFromHSV(0, 1, i/360.f));
		}
		DrawRectangle(W/2.f-50, H/2.f-360/2.f-40, 300, 40, brushcolor);
		DrawRectangle(W/2-50, H/2.f-360/2.f+HSV.x, 100, 1, BLACK);
		DrawRectangle(W/2+50, H/2.f-360/2.f+HSV.y*360.f, 100, 1, BLACK);
		DrawRectangle(W/2+150, H/2.f-360/2.f+HSV.z*360.f, 100, 1, BLACK);
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		    if (ms.x >= W/2.f-50 && ms.y >= H/2.f-360/2.f && ms.x <= W/2.f+50 && ms.y <= H/2.f-360/2.f+360) {
			HSV.x = ms.y - (H/2.f-360/2.f);
			brushcolor = ColorFromHSV(HSV.x, HSV.y, HSV.z);
		    }
		    if (ms.x >= W/2.f+50 && ms.y >= H/2.f-360/2.f && ms.x <= W/2.f+150 && ms.y <= H/2.f-360/2.f+360) {
			HSV.y = (ms.y - (H/2.f-360/2.f))/360.f;
			brushcolor = ColorFromHSV(HSV.x, HSV.y, HSV.z);
		    }
		    if (ms.x >= W/2.f+150 && ms.y >= H/2.f-360/2.f && ms.x <= W/2.f+250 && ms.y <= H/2.f-360/2.f+360) {
			HSV.z = (ms.y - (H/2.f-360/2.f))/360.f;
			brushcolor = ColorFromHSV(HSV.x, HSV.y, HSV.z);
		    }
		}
		
		DrawCircleV(ms, brushsize, RED);
		if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) break;
		EndDrawing();
	    }
	}

        UpdateTexture(tex, img.data);

	draw_background();
	brushsize += brushsize * 0.1f * GetMouseWheelMove();
	if (scale < 0.1f) scale = 0.1f;
	
	DrawTextureEx(tex, (Vector2){0,0}, 0.0f, scale, WHITE);
	DrawCircleV(ms, brushsize, RED);
	DrawCircleV(pms, brushsize, RED);
        EndDrawing();
	
        pms = ms;
    }

    ExportImage(img, "myimage.png");
    UnloadTexture(tex);
    UnloadImage(img);
    CloseWindow();
    return 0;
}
