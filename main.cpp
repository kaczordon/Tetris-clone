#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include <time.h>
#include <iostream>
#include <sstream>
extern "C" {
//#ifdef BIT64
//#include"./sdl64/include/SDL.h"
//#include"./sdl64/include/SDL_main.h"
//#else
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"
#include "Header.h"
//#endif
}
#define SCREEN_WIDTH	724
#define SCREEN_HEIGHT	484
using namespace std;
enum Shape{Red=1,Blue,Pink,Purple, Green, Yellow, Orange};

// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj¹ca znaki
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt œrodka obrazka sprite na ekranie
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
	};


// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
	};


// rysowanie linii o d³ugoœci l w pionie (gdy dx = 0, dy = 1) 
// b¹dŸ poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
		};
	};


// rysowanie prostok¹ta o d³ugoœci boków l i k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
	};

int map[20][15] = { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } };

void DrawMap(SDL_Surface *screen) {
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	int purple = SDL_MapRGB(screen->format, 139, 34, 82);
	int yellow = SDL_MapRGB(screen->format, 255, 255, 0);
	int green = SDL_MapRGB(screen->format, 0, 255, 0);
	int orange = SDL_MapRGB(screen->format, 255, 128, 0);
	int pink = SDL_MapRGB(screen->format, 255, 102, 178);

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 15; j++) {
			if (map[i][j] == 1) {
				DrawRectangle(screen, (j * 24) + 4, i* 24, 24, 24, czerwony, czerwony);
			}
			else if (map[i][j] == 2) {
				DrawRectangle(screen, (j * 24) + 4, i * 24, 24, 24, niebieski, niebieski);
			}
			else if (map[i][j] == 3) {
				DrawRectangle(screen, (j * 24) + 4, i * 24, 24, 24, pink, pink);
			}
			else if (map[i][j] == 4) {
				DrawRectangle(screen, (j * 24) + 4, i * 24, 24, 24, purple, purple);
			}
			else if (map[i][j] == 5) {
				DrawRectangle(screen, (j * 24) + 4, i * 24, 24, 24, green, green);
			}
			else if (map[i][j] == 6) {
				DrawRectangle(screen, (j * 24) + 4, i * 24, 24, 24, yellow, yellow);
			}
			else if (map[i][j] == 7) {
				DrawRectangle(screen, (j * 24) + 4, i * 24, 24, 24, orange, orange);
			}
		}
	}
}

class Tetron
{
public:
	Tetron(SDL_Surface *temp) {
		image = 0;
		x = y = 0;
		screen = temp;
		czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
		zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
		czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
		niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
		purple = SDL_MapRGB(screen->format, 139, 34, 82);
		yellow = SDL_MapRGB(screen->format, 255, 255, 0);
		green = SDL_MapRGB(screen->format, 0, 255, 0);
		orange = SDL_MapRGB(screen->format, 255, 128, 0);
		pink = SDL_MapRGB(screen->format, 255, 102, 178);

		stop = false;
	}

	void SetImage(Shape shape) {
		if (shape == Red) {
			int temp[4][4] = { { 0,0,0,0 },
			{ 1,1,0,0 },
			{ 0,1,1,0 },
			{ 0,0,0,0 } };
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					block[i][j] = temp[i][j];
				}
			}
			color = czerwony;
			rcolor = Red;
		}
		else if(shape == Pink){
			int temp[4][4] = { { 1,1,1,1 },
			{ 0,0,0,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 } };
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					block[i][j] = temp[i][j];
				}
			}
			color = pink;
			rcolor = Pink;
		}
		else if (shape == Blue) {
			int temp[4][4] = { { 1,0,0,0 },
			{ 1,1,1,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 } };
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					block[i][j] = temp[i][j];
				}
			}
			color = niebieski;
			rcolor = Blue;
		}
		else if (shape == Purple) {
			int temp[4][4] = { { 0,1,0,0 },
			{ 1,1,1,0 },
			{ 0,0,0,0 },
			{ 0,0,0,0 } };
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					block[i][j] = temp[i][j];
				}
			}
			color = purple;
			rcolor = Purple;
		}
		else if (shape == Green) {
			int temp[4][4] = { { 0,0,0,0 },
			{ 0,1,1,0 },
			{ 1,1,0,0 },
			{ 0,0,0,0 } };
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					block[i][j] = temp[i][j];
				}
			}
			color = green;
			rcolor = Green;
		}
		else if (shape == Yellow) {
			int temp[4][4] = { { 0,0,0,0 },
			{ 0,1,1,0 },
			{ 0,1,1,0 },
			{ 0,0,0,0 } };
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					block[i][j] = temp[i][j];
				}
			}
			color = yellow;
			rcolor = Yellow;
		}
		else if (shape == Orange) {
			int temp[4][4] = { { 0,0,0,1 },
			{ 0,1,1,1 },
			{ 0,0,0,0 },
			{ 0,0,0,0 } };
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					block[i][j] = temp[i][j];
				}
			}
			color = orange;
			rcolor = Orange;
		}
	}

	void SetXY(float nx, float ny) {
		x = nx;
		y = ny;
	}

	void Draw() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block[i][j] == 1) {
					int tempx = j*24 + x;
					int tempy = i*24 + y;
					DrawRectangle(screen, tempx, tempy, 24, 24, color, color);
				}
			}
		}
	}
	void CheckEdges() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block[i][j] == 1) {
					if (j + (int)((x+14) / 24)>14)
						x -= 10;
					else if (j + (int)((x+18) / 24) == 0)
						x += 10;
				}
			}
		}
	}

	void Tetron2Map() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				int mapx = x/24 + j;
				int mapy = y / 24 + i;
				if(block[i][j] == 1)
					map[mapy][mapx] = (int)rcolor;
			}
		}
	}

	void Rotate(bool flag) {
		int temp[4][4] = { { 0,0,0,0 },
		{ 0,0,0,0 },
		{ 0,0,0,0 },
		{ 0,0,0,0 } };
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				temp[j][i] = block[i][j];
			}
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				block[i][j] = temp[i][j];
			}
		}
		if (flag) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					temp[i][j] = block[i][3 - j];
				}
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					block[i][j] = temp[i][j];
				}
			}
		}
	}
	int LastRow() {
		for (int i = 3; i >= 0; i--) {
			for (int j = 0; j < 4; j++) {
				if (block[i][j] == 1)
					return i;
			}
		}
	}

	SDL_Surface *image;
	float x, y;
	int block[4][4] = { { 0,0,0,0 },
	{ 0,0,0,0 },
	{ 0,0,0,0 },
	{ 0,0,0,0 } };
	SDL_Surface *screen;
	int czarny;
	int zielony;
	int czerwony;
	int niebieski;
	int purple;
	int yellow;
	int orange;
	int green;
	int color;
	int pink;
	Shape rcolor;
	bool stop;
};

bool CheckMap(Tetron tetron) {
	int tx = tetron.x/24;
	int ty = tetron.y/24;
	for (int i = 3; i >= 0; i--) {
		for (int j = 3; j >= 0; j--) {
			int tempy = i;
			int tempx = j;
 			if (tetron.block[i][j] == 1 && map[ty + tempy + 1][tx + tempx] == 1)
				return true;
			else if (tetron.block[i][j] == 1 && map[ty + tempy + 1][tx + tempx] == 2)
				return true;
			else if (tetron.block[i][j] == 1 && map[ty + tempy + 1][tx + tempx] == 3)
				return true;
			else if (tetron.block[i][j] == 1 && map[ty + tempy + 1][tx + tempx] == 4)
				return true;
			else if (tetron.block[i][j] == 1 && map[ty + tempy + 1][tx + tempx] == 5)
				return true;
			else if (tetron.block[i][j] == 1 && map[ty + tempy + 1][tx + tempx] == 6)
				return true;
			else if (tetron.block[i][j] == 1 && map[ty + tempy + 1][tx + tempx] == 7)
				return true;
		}
	}
	return false;
}

void MoveMap(int row) {
	for (int i = row; i >= 0; i--) {
		for (int j = 0; j < 15; j++) {
			map[row][j] = map[row - 1][j];
		}
	}
}

void CheckLine(int *points, int *stage, bool *bonus) {
	int counter = 0;
	int pcounter = 0;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 15; j++) {
			if (map[i][j] != 0)
				counter++;
			if (counter == 15) {
				MoveMap(i);
				pcounter++;
			}
		}
		counter = 0;
	}
	if (pcounter != 0) {
		switch (pcounter) {
		case 1:
			*points = 100*(*stage + 1);
			*bonus = false;
			break;
		case 2:
			*points = 200*(*stage + 1);
			*bonus = false;
			break;
		case 3:
			*points = 400*(*stage + 1);
			*bonus = false;
			break;
		case 4:
			if(*bonus)
				*points = 1200 * (*stage + 1);
			else {
				*points = 800 * (*stage + 1);
				*bonus = true;
			}
			break;
		}
	}
}

bool EndGame(Tetron tetron) {
	int tempx = tetron.x / 24;
	for (int i = 3; i >= 0; i--) {
		for (int j = 0; j < 4; j++) {
			if (tetron.block[i][j] == 1) {
				if (map[0][j + tempx] != 0)
					return true;
			}
		}
	}
	return false;
}

void MoveDown(Tetron* tetron) {
	int xcoord[4] = { 100,100,100,100 };
	int ycoord[4] = { 3,3,3,3 };
	int temp = 100;
	int tempy = 3;
	for (int i = 3; i >= 0; i--) {
		for (int j = 0; j < 4; j++) {
			if (tetron->block[i][j] == 1) {
				for (int k = ((int)tetron->y / 24) + i + 1; k < 20; k++) {
					if (map[k][j+((int)tetron->x/24)] != 0) {
						ycoord[j] = i;
						xcoord[j] = k-1;
					}
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (i + 1 < 4 && xcoord[i] < xcoord[i + 1]) {
			temp = xcoord[i];
			tempy = ycoord[i];
		}
	}

	if (temp != 100)
		tetron->y = (temp-3)*24;
	else {
		tetron->y = (16+tempy) * 24;
		tetron->Tetron2Map();
	}
}

void Reverse(int *buffer, int count) {
	int temp[256];
	int p = count - 1;
	for (int i = 0; i < count; i++) {
		temp[i] = buffer[p];
		p--;
	}
	for (int i = 0; i < count; i++) {
		buffer[i] = temp[i];
	}
	buffer[0] = 0;
	for (int i = 0; i < count; i++) {
		buffer[0] += pow(10,i)*buffer[i];
	}
}

void LoadScore(char *scores, int *rank) {
	FILE *fp;
	char temp;
	int buffer[30];
	int i = 0;
	fp = fopen("Score.txt", "r");
	for (int j = 0; j < 10;) {
		scores[i] = fgetc(fp);
		if (scores[i] == '\n')
			j++;
		else if (scores[i] == ' ') {
			int count = 0;
			while (true) {
				temp = fgetc(fp);
				if (temp == '\n' || temp == EOF) {
					scores[i + count + 1] = temp;
					i = i + count + 1;
					break;
				}
				buffer[count] = temp - '0';
				scores[i+count + 1] = temp;
				count++;
			}
			Reverse(buffer, count);
			rank[j] = buffer[0];
			j++;
		}
		else if (scores[i] == EOF)
			break;
		i++;
	}
	fclose(fp);
}

void ResetMap() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 15; j++) {
			map[i][j] = 0;
		}
	}
}

void CheckScore(int *rank, int score, bool *save, char *points) {
	int temp[10];
	char buffer[30];
	for (int i = 0; i < 10; i++) {
		temp[i] = rank[i];
	}
	for (int i = 0; i < 10; i++) {
		if (score > rank[i]) {
			for (int j = i+1; j < 10; j++) {
				rank[j] = temp[j - 1];
			}
			rank[i] = score;
			sprintf(buffer, "%ld", score);
			*save = true;
			/*int k = 0;
			int c = 0;
			bool master = true;
			while (master) {
				if (points[k] == '\n')
					c++;
				if (c == i) {
					bool temp = true;
					k++;
					points[k] = '?';
					while (temp) {
						k++;
						if (points[k] == ' ') {
							k++;
							for (int n = 0;; n++) {
								if (buffer[n] == EOF || buffer[n] == '\0')
									break;
								points[k] = buffer[n];
								k++;
							}
							temp = false;
							master = false;
						}
						points[k] = ' ';
					}
				}
				k++;
			}*/
			break;
		}
	}
}

// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	int t1, t2, quit, frames, rc, movement, gravity, rblock, stage, points;
	double delta, worldTime, fpsTimer, fps, distance, etiSpeed;
	bool flag, chosen, pause, flagged, bonus, save;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *eti;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;
	Blocks CurrentBlocks;

	srand(time(NULL));

	flag = false;
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
		}

	// tryb pe³noekranowy
//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
//	                                 &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
	                                 &window, &renderer);
	if(rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
		};
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Szablon do zdania drugiego 2014");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
	                              0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	Tetron CurrentTetron(screen);
	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
	                           SDL_TEXTUREACCESS_STREAMING,
	                           SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if(charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
		};
	SDL_SetColorKey(charset, true, 0x000000);

	char text[128];
	char score[256];
	int rank[10];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	LoadScore(score, rank);

	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	movement = 0;
	worldTime = 0;
	distance = 0;
	etiSpeed = 1;
	bonus = false;
	gravity = 100;
	points = 0;
	pause = false;
	flagged = false;
	stage = 0;
	chosen = false;
	save = false;

	while(!quit) {
		t2 = SDL_GetTicks();
		rblock = rand() % 7 + 1;
		if (!chosen) {
			CurrentTetron.SetImage((Shape)rblock);
			chosen = true;
			CurrentTetron.x = (rand() % 14) * 24;
		}
		CurrentTetron.CheckEdges();
		if (!flagged) {
			delta = (t2 - t1) * 0.001;
			t1 = t2;
		}
		else {
			delta = 0;
			flagged = false;
			t1 = t2;
		}

		worldTime += delta;

		distance += etiSpeed * delta;
		CheckScore(rank, points, &save, score);

		SDL_FillRect(screen, NULL, czarny);
		DrawRectangle(screen, 5, 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 8, czerwony, czarny);
		if (CurrentTetron.LastRow()*24 + CurrentTetron.y + gravity*delta > SCREEN_HEIGHT)
			CurrentTetron.y = 0;
		else
			CurrentTetron.y += gravity*delta;
		CurrentTetron.x += movement*delta;
		if ((CurrentTetron.y + CurrentTetron.LastRow()*24) / 24 >= 19 && !CurrentTetron.stop) {
			CurrentTetron.Tetron2Map();
			//CurrentTetron.stop = true;
			CurrentTetron.y = 0;
			chosen = false;
		}
		else if (CheckMap(CurrentTetron)) {
			CurrentTetron.Tetron2Map();
			CurrentTetron.y = 0;
			chosen = false;
		}
		if (CurrentTetron.y == 0 && EndGame(CurrentTetron)) {
			//HighScore();
			bool end, render;
			int offset = 0;
			char name[20] = "";
			end = render = false;
			sprintf(text, "Enter High Score:", worldTime, fps);
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2 - 180, SCREEN_HEIGHT / 2 - 100, text, charset);
			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
			SDL_RenderPresent(renderer);
			FILE *fp;
			SDL_StartTextInput();
			while (!end) {
				render = false;
				while (SDL_PollEvent(&event)) {
					switch (event.type) {
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_BACKSPACE && offset > 0)
						{
							name[offset] = ' ';
							render = true;
							offset--;
						}
						else if (event.key.keysym.sym >96 && event.key.keysym.sym<123) {
							name[offset] = event.text.text[8];
							render = true;
							offset++;
						}
						else if (event.key.keysym.sym == 13) {
							end = true;
						}
						else if (event.key.keysym.sym == SDLK_ESCAPE)
							end = true;
					};
					if (render) {
						if (name != "") {
							SDL_FillRect(screen, NULL, czarny);
							DrawRectangle(screen, 5, 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 8, czerwony, czarny);
							sprintf(text, "Enter High Score:", worldTime, fps);
							DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2 - 180, SCREEN_HEIGHT / 2 - 100, text, charset);
							DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2 -40, SCREEN_HEIGHT / 2 - 100, name, charset);
							SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
							SDL_RenderCopy(renderer, scrtex, NULL, NULL);
							SDL_RenderPresent(renderer);
						}
					}
				};
			}
			fp = fopen("Score.txt", "w");
			for (int i = 0; i < 20; i++) {
				if (name[i] == '\0')
					break;
				fputc(name[i], fp);
			}
			if(name[0] != '\0')
				fputc('\n', fp);
			fclose(fp);
			quit = true;
		}
		CurrentTetron.Draw();
		CheckLine(&points, &stage, &bonus);
		DrawMap(screen);
		sprintf(text, "Time = %.1lf", worldTime, fps);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2 + 100, 10, text, charset);
		sprintf(text, "Stage = %d", stage);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2 + 92, 30, text, charset);
		sprintf(text, "Points = %d", points);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2 + 90, 50, text, charset);
		sprintf(text, "High Score:");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2 + 90, 70, text, charset);
		for (int i = 0; i < 128; i++) {
			text[i] = 0;
		}
		int next = 0;
		int display = 0;
			for (int j = 0;; j++) {
				if (score[j] == '\n') {
					DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2 + 90, 90 + display * 10, text, charset);
					next = 0;
					j++;
					display++;
					for (int i = 0; i < 128; i++) {
						text[i] = 0;
					}
				}
				else if (score[j] == EOF || display == 10 || score[j] == -52) {
					DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2 + 90, 90 + display * 10, text, charset);
					//i = 10;
					break;
				}
				text[next] = score[j];
				next++;
			}
//		DrawScreen(screen, plane, ship, charset, worldTime, delta, vertSpeed);

		// naniesienie wyniku rysowania na rzeczywisty ekran
//		SDL_Flip(screen);

		fpsTimer += delta;
		if(fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
			};

		// tekst informacyjny
		if (pause) {
			sprintf(text, "Paused", worldTime, fps);
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2 -180, SCREEN_HEIGHT/2-100, text, charset);
		}
		if ((int)worldTime % 100 == 0 && (int)worldTime!=0) {
			if (stage <= 9) {
				stage++;
				gravity +=100;
			}
		}
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
//		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
		if (pause) {
			while (pause) {
				while (SDL_PollEvent(&event)) {
					switch (event.type) {
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_ESCAPE) {quit = 1; pause = false;}
						else if (event.key.keysym.sym == SDLK_p) pause = false;
						break;
					case SDL_KEYUP:
						etiSpeed = 1.0;
						movement = 0;
						break;
					case SDL_QUIT:
						quit = 1;
						break;
					};
				};
			}
			flagged = true;
		}
		// obs³uga zdarzeñ (o ile jakieœ zasz³y)
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
					else if(event.key.keysym.sym == SDLK_UP) etiSpeed = 2.0;
					else if(event.key.keysym.sym == SDLK_DOWN) MoveDown(&CurrentTetron);
					else if (event.key.keysym.sym == SDLK_LEFT) {CurrentTetron.x -= 10; if (CheckMap(CurrentTetron)) { CurrentTetron.x += 10; }}
					else if (event.key.keysym.sym == SDLK_RIGHT) { CurrentTetron.x += 10; if (CheckMap(CurrentTetron)) { CurrentTetron.x -= 10; } }
					else if (event.key.keysym.sym == SDLK_SPACE) { CurrentTetron.Rotate(flag); flag = !flag; }
					else if (event.key.keysym.sym == SDLK_p) pause = true;
					else if (event.key.keysym.sym == SDLK_n) { CurrentTetron.y = 0; ResetMap(); worldTime = 0; stage = 0; gravity = 100; points = 0; }
					else if (event.key.keysym.sym == SDLK_s) { if(stage < 10)stage++; gravity+=100; }
					break;
				case SDL_KEYUP:
					etiSpeed = 1.0;
					movement = 0;
					break;
				case SDL_QUIT:
					quit = 1;
					break;
				};
			};
		frames++;
		};

	// zwolnienie powierzchni
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
	};
