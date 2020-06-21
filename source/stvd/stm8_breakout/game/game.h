/*
6/16/20
Breakout game controller file
*/


#ifndef __GAME_H
#define __GAME_H

#include "register.h"
#include "bitmap.h"
#include "lcd.h"

#define GAME_MIN_X			0
#define GAME_MAX_X			FRAME_BUFFER_WIDTH
#define GAME_MIN_Y			0
#define GAME_MAX_Y			FRAME_BUFFER_HEIGHT

//2 pixel margine all around
#define SCREEN_TOP		0
#define SCREEN_BOTTOM	58
#define SCREEN_LEFT		0
#define SCREEN_RIGHT	78


#define GAME_PLAYER_NUM_LIVES		5
#define GAME_PLAYER_DEFAULT_X		32
#define GAME_PLAYER_DEFAULT_Y		52
#define GAME_PLAYER_INC_X			2

#define GAME_TILE_MARGINE_LEFT      2
#define GAME_TILE_NUM_ROW			5
#define GAME_TILE_NUM_COL			8
#define GAME_TILE_NUM_TILE			40
#define GAME_TILE_SPACING_X			9
#define GAME_TILE_SPACING_Y			4

#define GAME_BALL_DEFAULT_X         20
#define GAME_BALL_DEFAULT_Y         20
#define GAME_BALL_PADDING           2                   //2 pixels all around
#define GAME_BALL_DEFAULT_DIRECTION     DIRECTION_45


//player type defs
typedef struct
{
	uint8_t x;
	uint8_t y;
    uint8_t sizeX;
    uint8_t sizeY;
	uint8_t lives;
	const ImageData *imagePtr;
}PlayerStruct;


typedef enum {
	SPEED_SLOW,
	SPEED_MEDIUM,
	SPEED_FAST
}Speed_t;


typedef enum {
	DIRECTION_0,
	DIRECTION_26,
	DIRECTION_45,
	DIRECTION_63,
	DIRECTION_90,
	DIRECTION_116,
	DIRECTION_135,
	DIRECTION_153,
	DIRECTION_180,
	DIRECTION_206,
	DIRECTION_225,
	DIRECTION_243,
	DIRECTION_270,
	DIRECTION_296,
	DIRECTION_315,
	DIRECTION_333
}Direction_t;


typedef enum {
    ORIENTATION_HORIZONTAL,
    ORIENTATION_VERTICAL
}Orientation_t;


typedef struct
{
	uint8_t x;
	uint8_t y;
    uint8_t sizeX;
    uint8_t sizeY;
	Speed_t speed;
	Direction_t direction;
	const ImageData *imagePtr;
}BallStruct;

typedef struct
{
	uint8_t x;
	uint8_t y;
    uint8_t sizeX;
    uint8_t sizeY;
	uint8_t alive;
	uint8_t points;
	const ImageData *imagePtr;
}TileStruct;



void game_init(void);
void game_player_init(void);
void game_ball_init(void);
void game_tile_init(void);

void game_player_draw(uint8_t update);
void game_ball_draw(uint8_t update);
void game_tile_draw(uint8_t update);

void game_player_move_left(void);
void game_player_move_right(void);
void game_player_move_auto(void);
void game_ball_move(void);



Direction_t game_getReboundAngle(Direction_t direction, Orientation_t orientation);
int game_get_dx_fromAngle(Direction_t angle);
int game_get_dy_fromAngle(Direction_t angle);

//flags
void game_clearBallMissedFlag(void);
uint8_t game_getBallMissedFlag(void);




#endif

