/*
 * game.c
 *
 *  Created on: Sep 8, 2019
 *      Author: danao
 * 
 * Configuration file for the game.  Contains instances of 
 * the player, enemy, and missile arrays, all functions for
 * movement, launching missiles, etc.
 * 
 * Note: This was originally created for different dev board
 * and adapted for the stm8 gameboard
 * 
 */
#include <stdlib.h>

#include "register.h"
#include "system.h"         //interrupts
#include "timer.h"          //delay
#include "game.h"
#include "lcd.h"
#include "bitmap.h"
#include "gpio.h"
#include "sound.h"
#include "eeprom.h"


//Game objects
//Note: Declare as static and init to 0x00 to 
//get compiler to put into .bss.  Otherwise, it
//ends up in .common.  Erroneous results happen
//when it ends up in .common.

static PlayerStruct mPlayer = {0x00};
static EnemyStruct mEnemy[GAME_ENEMY_NUM_ENEMY] = {0x00};

//Missile arrays, assume the player and the enemy each get 4
static MissileStruct mPlayerMissile[GAME_MISSILE_NUM_MISSILE] = {0x00};
static MissileStruct mEnemyMissile[GAME_MISSILE_NUM_MISSILE] = {0x00};

//flags
volatile uint8_t mPlayerHitFlag = 0x00;
volatile uint8_t mGameOverFlag = 0x00;
volatile uint8_t mGameLevelUpFlag = 0x00;
volatile uint8_t mEnemyHitFlag = 0x00;

//score and level
static uint16_t mGameScore = 0x00;
static uint8_t mGameLevel = 0x00;
static uint16_t mGameHighScore = 0x00;


/////////////////////////////////////////
//Game_init()
//
void Game_init(void)
{
	//reset the score, flags, etc
	mPlayerHitFlag = 0x00;
	mGameLevelUpFlag = 0x00;
	mEnemyHitFlag = 0x00;
	mGameScore = 0x00;
	mGameLevel = 0x01;
	mGameOverFlag = 0x00;
    mGameHighScore = 0x00;
    
    //read the highscore from eeprom
    mGameHighScore = EEPROM_getHighScore();
	
	lcd_clear(0x00);			//clear screen
	lcd_clearBackground(0xAA);	//margins

	Game_playerInit();
	Game_enemyInit();
	Game_missileInit();
	
	Game_playerDraw();
	Game_enemyDraw();
	
	//update the display contents
	lcd_updateFrameBuffer();
}

///////////////////////////////////////////////
//Player_init
//Reset the num lives and set initial position
void Game_playerInit(void)
{
	mPlayer.numLives = GAME_PLAYER_NUM_LIVES;
	mPlayer.xPosition = GAME_PLAYER_DEFAULT_X;
}

//////////////////////////////////////////////
//Enemy_init
//
void Game_enemyInit(void)
{
	uint8_t i, j = 0;
	uint8_t index = 0;

	for (i = 0 ; i < GAME_ENEMY_NUM_ROWS ; i++)
	{
		for (j = 0 ; j < GAME_ENEMY_NUM_COLS ; j++)
		{
			mEnemy[index].flag_VH = 0x03;		//down right
			mEnemy[index].alive = 1;
			mEnemy[index].xPosition = (GAME_ENEMY_X_SPACING * j);
			mEnemy[index].yPosition = (GAME_ENEMY_Y_SPACING * i);
			index++;
		}
	}
}


///////////////////////////////////////////////
//Missile_init
void Game_missileInit(void)
{
	uint8_t i = 0;
	
	for (i = 0 ; i < GAME_MISSILE_NUM_MISSILE ; i++)
	{
		mEnemyMissile[i].alive = 0;
		mEnemyMissile[i].x = 0x00;
		mEnemyMissile[i].y = 0x00;
		
		mPlayerMissile[i].alive = 0x00;
		mPlayerMissile[i].x = 0x00;
		mPlayerMissile[i].y = 0x00;
	}
}


void Game_playerMoveLeft(void)
{
	if (mPlayer.xPosition > GAME_PLAYER_MIN_X + 2)
		mPlayer.xPosition-=2;	
}

void Game_playerMoveRight(void)
{
	if (mPlayer.xPosition < GAME_PLAYER_MAX_X - 2)
		mPlayer.xPosition+=2;
}



///////////////////////////////////////////////
//Move enemies left and right, up and down
//
void Game_enemyMove(void)
{
	uint8_t i, j = 0;
	uint8_t flag = 0x00;
	
	//sizes
	uint8_t sizeX = bmenemy1Bmp.xSize;
	uint8_t sizeY = bmenemy1Bmp.ySize;

	//direction flags
	uint8_t right = 0x00;
	uint8_t down = 0x00;
	
	for (i = 0 ; i < GAME_ENEMY_NUM_ENEMY ; i++)
	{
		//get flags
		right = (mEnemy[i].flag_VH) & 0x01;

		//moving right
		if (right == 1)
		{
			if (((mEnemy[i].xPosition + sizeX) < GAME_ENEMY_MAX_X) && (mEnemy[i].alive == 1))
				mEnemy[i].xPosition += 2;
		}
		
		//moving left
		else
		{
			if ((mEnemy[i].xPosition > GAME_ENEMY_MIN_X) && (mEnemy[i].alive == 1))
				mEnemy[i].xPosition -= 2;
		}
	}
	
	//direction change - left
	flag = 0x00;	
	for (i = 0 ; i < GAME_ENEMY_NUM_ENEMY ; i++)
	{		
		if (((mEnemy[i].xPosition + sizeX) >= GAME_ENEMY_MAX_X) && (mEnemy[i].alive == 1))
		{
			flag = 1;
			i = GAME_ENEMY_NUM_ENEMY;
			break;
		}
	}
	
	if (flag == 1)
	{
		//change the direction bit for each enemy - clear it for left
		for (i = 0 ; i < GAME_ENEMY_NUM_ENEMY ; i++)
		{
			mEnemy[i].flag_VH &=~ BIT_0;
		}		
	}
	
	//direction change - right
	flag = 0x00;	
	for (i = 0 ; i < GAME_ENEMY_NUM_ENEMY ; i++)
	{		
		if ((mEnemy[i].xPosition <= GAME_ENEMY_MIN_X) && (mEnemy[i].alive == 1))
		{
			flag = 1;
			i = GAME_ENEMY_NUM_ENEMY;
			break;
		}
	}
	
	if (flag == 1)
	{
		//flip direction to right
		for (i = 0 ; i < GAME_ENEMY_NUM_ENEMY ; i++)
		{
			mEnemy[i].flag_VH |= BIT_0;			//set direction right
			
			down = ((mEnemy[i].flag_VH) >> 1) & 0x01;

			//enemy are moving down
			if (down == 1)
			{
				//move down
				if(((mEnemy[i].yPosition + sizeY) < GAME_ENEMY_MAX_Y) && (mEnemy[i].alive == 1))
					mEnemy[i].yPosition++;
			}
			
			//enemy are moving up
			else
			{
				if((mEnemy[i].yPosition > GAME_ENEMY_MIN_Y) && (mEnemy[i].alive == 1))
					mEnemy[i].yPosition--;
			}
		}
	}
	
	//check for direction change up
	flag = 0;
	for (i = 0 ; i < GAME_ENEMY_NUM_ENEMY ; i++)
	{
		if(((mEnemy[i].yPosition + sizeY) >= GAME_ENEMY_MAX_Y) && (mEnemy[i].alive == 1))
		{
			flag = 1;
			i = GAME_ENEMY_NUM_ENEMY;
			break;
		}
	}
	
	//set the direction to up
	if (flag == 1)
	{
		for (i = 0 ; i < GAME_ENEMY_NUM_ENEMY ; i++)
		{
			mEnemy[i].flag_VH &=~ BIT_1;			//clear the vertical bit
			mEnemy[i].yPosition -= 1;			//move up
		}
	}
	
	
	//check for direction change down
	flag = 0;
	for (i = 0 ; i < GAME_ENEMY_NUM_ENEMY ; i++)
	{
		if((mEnemy[i].yPosition <= GAME_ENEMY_MIN_Y) && (mEnemy[i].alive == 1))
		{
			flag = 1;
			i = GAME_ENEMY_NUM_ENEMY;
			break;
		}
	}
	
	//set the direction to down
	if (flag == 1)
	{
		for (i = 0 ; i < GAME_ENEMY_NUM_ENEMY ; i++)
		{
			mEnemy[i].flag_VH |= BIT_1;		//set Vertical bit
			mEnemy[i].yPosition += 1;		//move down
		}
	}
}




/////////////////////////////////////////////////
//Game_missileMove()
//
//Update the position of the missiles.  Enemy
//missiles move down and player missiles move up
uint8_t Game_missileMove(void)
{
	uint8_t i, j = 0;
	uint8_t mX, mY, bot, top, left, right = 0x00;
	uint8_t numEnemyRemaining = 0x00;
	uint8_t numPlayerRemaining = 0x00;
	
	for (i = 0 ; i < GAME_MISSILE_NUM_MISSILE ; i++)
	{
		//player missile - moving up
		if ((mPlayerMissile[i].y > GAME_MISSILE_MIN_Y) && (mPlayerMissile[i].alive == 1))
			mPlayerMissile[i].y-=2;
		else
		{
			//remove it from the alive list
			mPlayerMissile[i].alive = 0;
			mPlayerMissile[i].x = 0;
			mPlayerMissile[i].y = 0;
		}
		
		//check for player missile hit enemy
		if (mPlayerMissile[i].alive == 1)
		{
			//loop over each enemy, did the missile hit it
			for (j = 0 ; j < GAME_ENEMY_NUM_ENEMY ; j++)
			{
				//alive?
				if ((mEnemy[j].alive == 1))
				{
					//get the coordinates of the missile and
					//box that outlines the enemy
					mX = mPlayerMissile[i].x;
					mY = mPlayerMissile[i].y;
					
					bot = mEnemy[j].yPosition + GAME_ENEMY_HEIGHT;
					top = mEnemy[j].yPosition;
					left = mEnemy[j].xPosition + GAME_IMAGE_MARGIN;
					right = mEnemy[j].xPosition + GAME_ENEMY_WIDTH - GAME_IMAGE_MARGIN;
					
                    //tip of the missile in the enemy box?
                    if ((mX >= left) && (mX <= right) && (mY <= bot) && (mY >= top))
                    {
                        //score hit!! - pass enemy index and missile index
                    	//numEnemyRemaining = 0x00;
                    	numEnemyRemaining = Game_scoreEnemyHit(j, i);
                    	
                    	//set the enemy hit flag
                    	mEnemyHitFlag = 1;
                    	
                    	if (numEnemyRemaining == 0)
                    	{                    		
                    		//set the level up flag and exit
                    		mGameLevelUpFlag = 1;                    		
                    		return 1;
                    	}
                    }
				}
			}
		}

		//enemy missile - moving down
		if ((mEnemyMissile[i].y < GAME_MISSILE_MAX_Y) && (mEnemyMissile[i].alive == 1))
			mEnemyMissile[i].y+=2;
		else
		{
			mEnemyMissile[i].alive = 0;
			mEnemyMissile[i].x = 0;
			mEnemyMissile[i].y = 0;
		}
		
		//check for enemy missile hitting player
		if (mEnemyMissile[i].alive == 1)
		{
			//get the location of the missile and 
			//compare with the player location.  since the
			//player is outside the framebuffer, it uses x only
			//in the player footprint
			mX = mEnemyMissile[i].x;
			mY = mEnemyMissile[i].y;

			left = mPlayer.xPosition - FRAME_BUFFER_OFFSET_X + GAME_IMAGE_MARGIN;
			right = mPlayer.xPosition + GAME_PLAYER_WIDTH - FRAME_BUFFER_OFFSET_X - GAME_IMAGE_MARGIN;

			top = GAME_MISSILE_MAX_Y - 2;
			bot = GAME_MISSILE_MAX_Y;

            //tip of the missile in the player box?
            if ((mX >= left) && (mX <= right) && (mY <= bot) && (mY >= top))
            {
            	//score player hit
            	numPlayerRemaining = Game_scorePlayerHit(i);
            	
            	//set the player hit flag - polled and cleared in main
            	mPlayerHitFlag = 1;
            	            	
            	//last player??
            	if (numPlayerRemaining == 0)
            	{
            		//set the game over flag and exit
            		mGameOverFlag = 1;
            		return 1;
            	}
            }
		}
	}
	
	return 0;
}




//////////////////////////////////////////
//Draw player on the player page
//updates the contents of the display
void Game_playerDraw(void)
{
	lcd_clearPage(GAME_PLAYER_PAGE, 0x00);
	lcd_drawImagePage(GAME_PLAYER_PAGE, mPlayer.xPosition, BITMAP_PLAYER);
}


//////////////////////////////////////////////
//Draw enemy array into framebuffer.  Does not
//update the contents of the lcd
void Game_enemyDraw(void)
{
	uint8_t i = 0;
	for (i = 0 ; i < GAME_ENEMY_NUM_ENEMY ; i++)
	{
		if (mEnemy[i].alive == 1)
		{	
			lcd_drawEnemyBitmap(mEnemy[i].xPosition, mEnemy[i].yPosition);
		}
	}
}


//////////////////////////////////////////////////
//Draw missile objects for all missiles
//that are alive.  Applies to be the player
//array of missiles and the enemy array of missiles
//Missiles are a blob of pixels, no image is associated
//with the missile
void Game_missileDraw(void)
{
	uint8_t i = 0;
	
	for (i = 0 ; i < GAME_MISSILE_NUM_MISSILE ; i++)
	{
		//player missile
		if (mPlayerMissile[i].alive == 1)
		{
			lcd_putPixelRam(mPlayerMissile[i].x, mPlayerMissile[i].y, 1, 0);
			lcd_putPixelRam(mPlayerMissile[i].x, mPlayerMissile[i].y - 1, 1, 0);
			
			lcd_putPixelRam(mPlayerMissile[i].x + 1, mPlayerMissile[i].y, 1, 0);
			lcd_putPixelRam(mPlayerMissile[i].x + 1, mPlayerMissile[i].y - 1, 1, 0);
		}
		
		//enemy missile
		if (mEnemyMissile[i].alive == 1)
		{
			lcd_putPixelRam(mEnemyMissile[i].x, mEnemyMissile[i].y, 1, 0);
			lcd_putPixelRam(mEnemyMissile[i].x, mEnemyMissile[i].y + 1, 1, 0);
			
			lcd_putPixelRam(mEnemyMissile[i].x + 1, mEnemyMissile[i].y, 1, 0);
			lcd_putPixelRam(mEnemyMissile[i].x + 1, mEnemyMissile[i].y + 1, 1, 0);
		}
	}
}


//////////////////////////////////////////
//Get first missile from array with alive = 0,
//set alive = 1, and x = player x, and y = player y
uint8_t Game_missilePlayerLaunch(void)
{
	uint8_t result = 0x00;
	uint8_t i = 0;
	uint8_t index = 0;
	uint8_t isAvailable = 0x00;
	
	//get the index of the first missile with alive = 0
	for (i = 0 ; i < GAME_MISSILE_NUM_MISSILE ; i++)
	{
		if (mPlayerMissile[i].alive == 0)
		{
			isAvailable = 1;
			index = i;
			i = GAME_MISSILE_NUM_MISSILE;
			break;
		}		
	}

	if (isAvailable == 1)
	{
		//set the missile
		mPlayerMissile[index].alive = 1;
		mPlayerMissile[index].y = GAME_MISSILE_MAX_Y - 2;
		mPlayerMissile[index].x = mPlayer.xPosition + GAME_MISSILE_OFFSET_X - FRAME_BUFFER_OFFSET_X;		
		
		result = 1;
	}
	
	return result;
}


///////////////////////////////////////////
//Launch missile from a random enemy
//returns 1 if success, 0 if not
uint8_t Game_missileEnemyLaunch(void)
{
	uint8_t result = 0;
	uint8_t i = 0;
	int index = 0x00;
	uint8_t missileIndex = 0x00;
	uint8_t isAvailable = 0x00;
	
	//get the index of a live random enemy
	index = Game_enemyGetRandomEnemy();
	
	if ((index >= 0) && (mEnemy[index].alive == 1))
	{
		//find the first available missile from enemy
		//missile array and set it to true, x and y
		//to the enemy
		for (i = 0 ; i < GAME_MISSILE_NUM_MISSILE ; i++)
		{
			if (mEnemyMissile[i].alive == 0)
			{
				isAvailable = 1;				//there is one available
				missileIndex = i;
				i = GAME_MISSILE_NUM_MISSILE;
				break;
			}
		}
		
		if (isAvailable == 1)
		{
			//launch
			mEnemyMissile[missileIndex].alive = 1;
			mEnemyMissile[missileIndex].x = mEnemy[index].xPosition + GAME_ENEMY_OFFSET_X;
			mEnemyMissile[missileIndex].y = mEnemy[index].yPosition + GAME_ENEMY_HEIGHT;
			
			result = 1;
		}		
	}
	
	return result;
}


///////////////////////////////////////////
//Returns number of enemy with flag alive = 1
uint8_t Game_enemyGetNumEnemy(void)
{
	uint8_t i = 0;
	uint8_t count = 0;
	for (i = 0 ; i < GAME_ENEMY_NUM_ENEMY ; i++)
	{
		if (mEnemy[i].alive == 1)
			count++;
	}
	
	return count;
}


//////////////////////////////////////////
//Return the index of a random live enemy
int Game_enemyGetRandomEnemy(void)
{
	uint8_t i = 0;
	uint8_t index = 0;
	static uint8_t enemyCounter = 0x00;
	uint8_t numEnemy = Game_enemyGetNumEnemy();

	if (numEnemy > 0)
	{
		//get a random index value, 0 to numEnemy-1
		//add 1 to it and compare with a 1-based counter
		//Might be the zero-zero comparison causing the bug
		//or something being optimised out
		index = (uint8_t)(rand() % numEnemy);
		index++;
		
		//go to the random index, skipping over dead enemy
		enemyCounter = 1;
		for (i = 0 ; i < GAME_ENEMY_NUM_ENEMY ; i++)
		{
			if (mEnemy[i].alive == 1)
			{
				if (index == enemyCounter)
				{
					return (enemyCounter - 1);
				}
				
				enemyCounter++;
			}
		}
	}
	
	return -1;		//no enemy remaining
}



///////////////////////////////////////////////////
//Missile hit enemy.  Return number of enemy
//remaining.  Remove the enemny as alive and
//missile as alive.  Update the player score
uint8_t Game_scoreEnemyHit(uint8_t enemyIndex, uint8_t missileIndex)
{
	uint8_t remaining = 0x0;
	
	//clear the index of the enemy
	mEnemy[enemyIndex].alive = 0;
	mEnemy[enemyIndex].flag_VH = 0x00;
	mEnemy[enemyIndex].xPosition = 0x00;
	mEnemy[enemyIndex].yPosition = 0x00;
	
	//clear the missile from the player
	mPlayerMissile[missileIndex].alive = 0;
	mPlayerMissile[missileIndex].x = 0x00;
	mPlayerMissile[missileIndex].y = 0x00;
	
	//update the score
	mGameScore += GAME_ENEMY_POINTS;

	remaining = Game_enemyGetNumEnemy();
	
	return remaining;
}


/////////////////////////////////////////////
//Enemy missile hit the player
//returns the number of players remaining
uint8_t Game_scorePlayerHit(uint8_t missileIndex)
{
	mEnemyMissile[missileIndex].alive = 0x00;
	mEnemyMissile[missileIndex].x = 0x00;
	mEnemyMissile[missileIndex].y = 0x00;
			
	if (mPlayer.numLives > 1)
		mPlayer.numLives--;		
	
	else
		mPlayer.numLives = 0;
	
	return mPlayer.numLives;
}


/////////////////////////////////////////
//Cleared the enemy array
//update the game level, reset the enemy arrays
//and missile arrays
void Game_levelUp(void)
{
	mGameLevel++;				//increase the level
	Game_enemyInit();			//reset the enemy
	Game_missileInit();			//reset the missiles
}



///////////////////////////////////////////
uint16_t Game_getGameScore(void)
{
	return mGameScore;
}

///////////////////////////////////////////
uint8_t Game_getGameLevel(void)
{
	return mGameLevel;
}

///////////////////////////////////////////
uint8_t Game_getNumPlayers(void)
{
	return mPlayer.numLives;
}


////////////////////////////////////////////
void Game_setHighScore(uint16_t score)
{
    mGameHighScore = score;
}

uint16_t Game_getHighScore(void)
{
    return mGameHighScore;
}



////////////////////////////////////////////
//Flags - These are typically set object move
//functions and checked in the main game loop
//Flag is used to play sound, an image sequence
uint8_t Game_flagGetPlayerHitFlag(void)
{
	return mPlayerHitFlag;
}

/////////////////////////////////////////////
void Game_flagClearPlayerHitFlag(void)
{
	mPlayerHitFlag = 0;
}

/////////////////////////////////////////////
uint8_t Game_flagGetGameOverFlag(void)
{
	return mGameOverFlag;
}

////////////////////////////////////////////
void Game_flagClearGameOverFlag(void)
{
	mGameOverFlag = 0x00;
}

////////////////////////////////////////////
uint8_t Game_flagGetLevelUpFlag(void)
{
	return mGameLevelUpFlag;
}

///////////////////////////////////////////
void Game_flagClearLevelUpFlag(void)
{
	mGameLevelUpFlag = 0x00;
}

///////////////////////////////////////////
uint8_t Game_flagGetEnemyHitFlag(void)
{
	return mEnemyHitFlag;
}

///////////////////////////////////////////
void Game_flagClearEnemyHitFlag(void)
{
	mEnemyHitFlag = 0x00;
}



//////////////////////////////////////////////
//Draw player ship explosion with delay
//player explodes.  Should remove all missiles
//in the buffer so you can't see them hanging
//
void Game_playExplosionPlayer(void)
{
	system_disableInterrupts();
	Game_missileInit();
	lcd_clearFrameBuffer(0x00, 0);
	Game_enemyDraw();
	Game_missileDraw();
	lcd_updateFrameBuffer();
	system_enableInterrupts();
	
	lcd_drawImagePage(LCD_PLAYER_PAGE, mPlayer.xPosition, BITMAP_PLAYER_EXP1);
	timer_delay_ms(20);
	lcd_drawImagePage(LCD_PLAYER_PAGE, mPlayer.xPosition, BITMAP_PLAYER_EXP2);
	timer_delay_ms(20);
	lcd_drawImagePage(LCD_PLAYER_PAGE, mPlayer.xPosition, BITMAP_PLAYER_EXP3);
	timer_delay_ms(20);
	lcd_drawImagePage(LCD_PLAYER_PAGE, mPlayer.xPosition, BITMAP_PLAYER_EXP4);
	timer_delay_ms(20);
}


//////////////////////////////////////////////
//Draw player ship explosion with delay
//player explodes.  Should remove all missiles
//in the buffer so you can't see them hanging
//This version adds sound to the sequence
//Interrupts are disabled for clearing the display
void Game_playExplosionPlayer_withSound(void)
{
	system_disableInterrupts();
	Game_missileInit();
	lcd_clearFrameBuffer(0x00, 0);
	Game_enemyDraw();
	Game_missileDraw();
	lcd_updateFrameBuffer();
	system_enableInterrupts();
    
    
    //play sound and make the explosion
    Sound_play(&soundSquare_4ms_250hz, 20, 1);
	lcd_drawImagePage(LCD_PLAYER_PAGE, mPlayer.xPosition, BITMAP_PLAYER_EXP1);
    while (Sound_isPlaying()){};

    Sound_play(&soundSquare_4ms_500hz, 20, 1);
	lcd_drawImagePage(LCD_PLAYER_PAGE, mPlayer.xPosition, BITMAP_PLAYER_EXP2);
    while (Sound_isPlaying()){};
	
    Sound_play(&soundSquare_4ms_1800hz, 20, 1);
	lcd_drawImagePage(LCD_PLAYER_PAGE, mPlayer.xPosition, BITMAP_PLAYER_EXP3);
    while (Sound_isPlaying()){};
    
    Sound_play(&soundSquare_4ms_2600hz, 20, 1);
	lcd_drawImagePage(LCD_PLAYER_PAGE, mPlayer.xPosition, BITMAP_PLAYER_EXP4);
    while (Sound_isPlaying()){};
}


////////////////////////////////////////////
//Game Over
//Draw game over on the screen.  Init the game
//draw a sequence on the boarder, flip from AA to 55
void Game_playGameOver(void)
{
	static uint8_t toggle = 0x00;
    uint16_t length = 0x00;
    uint16_t score = Game_getHighScore();
    uint8_t buffer[16] = {0x00};
    
	system_disableInterrupts();
	lcd_clearFrameBuffer(0x00, 0);
	lcd_updateFrameBuffer();
    system_enableInterrupts();

	lcd_drawString(2, 16, "Game Over");
//	lcd_drawString(5, 31, "Press");
	lcd_drawString(6, 8, "PressButton");
	
    //display the high score
    lcd_drawString(3, 8, "High Score");    
    length = lcd_decimalToBuffer(score, buffer, 16);
    lcd_drawStringLength(4, 32, buffer, length);

	if (toggle == 1)
	{
		lcd_clearBackground(0xAA);
		toggle = 0x00;
	}
	else
	{
		lcd_clearBackground(0x55);
		toggle = 0x01;
	}
}






