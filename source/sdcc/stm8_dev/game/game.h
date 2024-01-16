/*
Game with scrolling screen

*/

#ifndef __GAME_H
#define __GAME_H

#include "lcd.h"


//limits of the ground surface - y positive down
#define GAME_GROUND_MAX     60
#define GAME_GROUND_MIN     30
#define GAME_GROUND_LENGTH  102

#define GAME_MAX_LIVES      3
#define GAME_PLAYER_MIN_Y   10
#define GAME_PLAYER_MAX_Y   50
#define GAME_PLAYER_MIN_X   0
#define GAME_PLAYER_MAX_X   90

#define GAME_PLAYER_DEFAULT_X   20
#define GAME_PLAYER_DEFAULT_Y   20

#define GAME_NUM_MISSILE    8

#define GAME_NUM_ENEMY      6

#define SCORE_BUFFER_SIZE   16

typedef enum
{
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_NONE
} Direction_t;


////////////////////////////////////////
//Player Definition
//
typedef struct
{
	uint8_t numLives;
	uint8_t x;
	uint8_t y;
	const ImageData* image;
}PlayerStruct;

typedef struct
{
    uint8_t alive;
    uint8_t x;
    uint8_t y;

}MissileStruct;

typedef struct
{
    uint8_t alive;
    uint8_t x;
    uint8_t y;
    uint8_t points;
    const ImageData* image;    
}EnemyStruct;



void game_init(void);

void game_displayStartup(void);
void game_displayGameOver(void);

void game_playerInit(void);
void game_groundInit(void);
void game_missileInit(void);
void game_enemyInit(void);

void game_scoreInit(void);
uint16_t game_getHighScore(void);
uint16_t game_getScore(void);
void game_setHighScore(uint16_t score);
void game_drawScore(void);


void game_playerReset(void);
uint8_t game_getNumPlayer(void);
void game_setNumPlayer(uint8_t num);
uint8_t game_killPlayer(void);

void game_updateGround(void);
void game_drawGround(uint8_t update);

Direction_t game_readJoystick(void);
uint8_t game_playerMove(Direction_t direction);
void game_drawPlayer(uint8_t update);


void game_fireMissile(void);
void game_moveMissile(void);
void game_drawMissile(uint8_t update);

void game_generateEnemy(void);
void game_moveEnemy(void);
void game_drawEnemy(uint8_t update);


#endif


