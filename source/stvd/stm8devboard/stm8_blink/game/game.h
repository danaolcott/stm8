/*
 * game.h
 *
 *  Created on: Sep 8, 2019
 *      Author: danao
 * 
 * Configuration file for the game.  Contains instances of 
 * the player, enemy, and missile arrays, all functions for
 * movement, launching missiles, etc.  
 *
 * Note:  This was orginally created for a different game
 * dev board and adapted to the stm8 game board
 */

#ifndef GAME_H_
#define GAME_H_

#include "register.h"
#include "bitmap.h"
#include "lcd.h"

#define GAME_PRINT_BUFFER_SIZE		10
#define GAME_PLAYER_PAGE			7
#define GAME_PLAYER_WIDTH			16
#define GAME_PLAYER_MIN_X			FRAME_BUFFER_OFFSET_X
#define GAME_PLAYER_MAX_X			(LCD_WIDTH - FRAME_BUFFER_OFFSET_X - GAME_PLAYER_WIDTH)
#define GAME_PLAYER_DEFAULT_X		40
#define GAME_PLAYER_NUM_LIVES		3

#define GAME_ENEMY_NUM_ENEMY		8
#define GAME_ENEMY_NUM_ROWS			2
#define GAME_ENEMY_NUM_COLS			4
#define GAME_ENEMY_X_SPACING		12
#define GAME_ENEMY_Y_SPACING		10

#define GAME_ENEMY_MIN_X			0
#define GAME_ENEMY_MAX_X			86
#define GAME_ENEMY_MIN_Y			2
#define GAME_ENEMY_MAX_Y			30
#define GAME_ENEMY_OFFSET_X			4
#define GAME_ENEMY_HEIGHT			8
#define GAME_ENEMY_WIDTH			12
#define GAME_ENEMY_POINTS			30

#define GAME_MISSILE_NUM_MISSILE	4
#define GAME_MISSILE_MIN_Y			4
#define GAME_MISSILE_MAX_Y			48
#define GAME_MISSILE_SIZE_X			2
#define GAME_MISSILE_SIZE_Y			4
#define GAME_MISSILE_OFFSET_X		8

#define GAME_IMAGE_MARGIN			1

#define GAME_FLAG_PLAYER_HIT		BIT_0
#define GAME_FLAG_GAME_OVER			BIT_1



/////////////////////////////////////
//Player Definition
typedef struct{
	uint8_t numLives;
	uint8_t xPosition;
}PlayerStruct;


/////////////////////////////////////////////
//Enemy Definition
//flag_VH - bits containing the direction of
//the enemy, V - vertical, high = down
//H = horizontal, high = right
//
typedef struct{
	uint8_t flag_VH;		//Vertical - down, Horiz - left (bits 1 and 0)
	uint8_t alive;
	uint8_t xPosition;
	uint8_t yPosition;	
}EnemyStruct;


//////////////////////////////////////////
//Missile Definition
typedef struct
{
	uint8_t alive;
	uint8_t x;
	uint8_t y;	
}MissileStruct;



///////////////////////////////////////////
//Prototypes
void Game_init(void);
void Game_playerInit(void);
void Game_enemyInit(void);
void Game_missileInit(void);

void Game_playerMoveLeft(void);
void Game_playerMoveRight(void);
void Game_enemyMove(void);
uint8_t Game_missileMove(void);

void Game_playerDraw(void);
void Game_enemyDraw(void);
void Game_missileDraw(void);

uint8_t Game_missilePlayerLaunch(void);
uint8_t Game_missileEnemyLaunch(void);

uint8_t Game_enemyGetNumEnemy(void);
int Game_enemyGetRandomEnemy(void);

uint8_t Game_scoreEnemyHit(uint8_t enemyIndex, uint8_t missileIndex);
uint8_t Game_scorePlayerHit(uint8_t missileIndex);
void Game_levelUp(void);

//score and level, players, stats, etc
uint16_t Game_getGameScore(void);
uint8_t Game_getGameLevel(void);
uint8_t Game_getNumPlayers(void);

//flags
uint8_t Game_flagGetPlayerHitFlag(void);
void Game_flagClearPlayerHitFlag(void);

uint8_t Game_flagGetGameOverFlag(void);
void Game_flagClearGameOverFlag(void);

uint8_t Game_flagGetLevelUpFlag(void);
void Game_flagClearLevelUpFlag(void);

uint8_t Game_flagGetEnemyHitFlag(void);
void Game_flagClearEnemyHitFlag(void);

//play sequences, explosion, explosion with sound
void Game_playExplosionPlayer(void);
void Game_playExplosionPlayer_withSound(void);
void Game_playGameOver(void);


///////////////////////////////////

#endif /* GAME_H_ */
