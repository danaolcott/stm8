/*
6/16/20
Game controller file for Breakout
Game has 3 main components: player, ball, tile(s), each
with it's own typedef.  Each has an x, y position, image ptr.
Player has lives.  Ball has direction and speed.  Tile has alive
and an array of tiles
*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "register.h"
#include "game.h"
#include "bitmap.h"
#include "sound.h"



//////////////////////////////////////////////
//Game global variables
PlayerStruct mPlayer;
BallStruct mBall;
TileStruct mTile[GAME_TILE_NUM_TILE];

//score, flags, etc
static uint16_t gameScore = 0;
static uint8_t gameBallMissedFlag = 0;
static GameMode_t mGameMode = GAME_MODE_AUTO;
static uint8_t gameLevelUpFlag = 0;
static uint8_t gameLevel = 1;
static uint8_t gameOverFlag = 0;


void game_init(void)
{   
    gameScore = 0;
    gameLevel = 1;
    
    game_player_init();
    game_ball_init();
    game_tile_init();
}

void game_player_init(void)
{
    mPlayer.x = GAME_PLAYER_DEFAULT_X;
    mPlayer.y = GAME_PLAYER_DEFAULT_Y;
    mPlayer.lives = GAME_PLAYER_NUM_LIVES;
    mPlayer.imagePtr = &bmimgPlayer_16;
    mPlayer.sizeX = mPlayer.imagePtr->xSize;
    mPlayer.sizeY = mPlayer.imagePtr->ySize;
}

void game_ball_init(void)
{
    mBall.x = GAME_BALL_DEFAULT_X;
    mBall.y = GAME_BALL_DEFAULT_Y;
    mBall.speed = SPEED_SLOW;
    mBall.direction = DIRECTION_45;
    mBall.imagePtr = &bmimgBall_8;
    mBall.sizeX = mBall.imagePtr->xSize;
    mBall.sizeY = mBall.imagePtr->ySize;    
}

void game_tile_init(void)
{
    uint8_t i, j = 0;
    uint8_t index = 0;

    for (i = 0 ; i < GAME_TILE_NUM_ROW ; i++)
    {
        for (j = 0 ; j < GAME_TILE_NUM_COL ; j++)
        {
            mTile[index].x = (j * GAME_TILE_SPACING_X) + GAME_TILE_MARGINE_LEFT;
            mTile[index].y = i * GAME_TILE_SPACING_Y;
            mTile[index].alive = 1;
            mTile[index].points = 30;
            mTile[index].imagePtr = &bmimgTile_8;
            mTile[index].sizeX = mTile[index].imagePtr->xSize;
            mTile[index].sizeY = mTile[index].imagePtr->ySize;
            index++;      
        }
    }
}




void game_player_draw(uint8_t update)
{
    lcd_drawIcon(mPlayer.x, mPlayer.y, mPlayer.imagePtr, (update & 0x01));
}

void game_ball_draw(uint8_t update)
{
    lcd_drawIcon(mBall.x, mBall.y, mBall.imagePtr, (update & 0x01));
}

void game_tile_draw(uint8_t update)
{
    uint8_t i = 0;

    for (i = 0 ; i < GAME_TILE_NUM_TILE ; i++)
    {
        lcd_drawIcon(mTile[i].x, mTile[i].y, mTile[i].imagePtr, (update & 0x01));
    }
}

void game_player_move_left(void)
{
    if (mPlayer.x >= (GAME_MIN_X + GAME_PLAYER_INC_X))
        mPlayer.x -= GAME_PLAYER_INC_X;
}

void game_player_move_right(void)
{
    if (mPlayer.x < (GAME_MAX_X - GAME_PLAYER_INC_X - mPlayer.sizeX))
        mPlayer.x += GAME_PLAYER_INC_X;
}


//////////////////////////////////////////////
//Moves player left and right auto
void game_player_move_auto(void)
{
    //0 = right, 1 = left
    static uint8_t direction = 0;
    
    //moving right
    if (!direction)
    {
        if (mPlayer.x < (GAME_MAX_X + GAME_PLAYER_INC_X - mPlayer.sizeX))
            mPlayer.x += GAME_PLAYER_INC_X;
        else
            direction = 1;
    }
    
    //moving left
    else
    {
        if (mPlayer.x >= (GAME_MIN_X + GAME_PLAYER_INC_X))
            mPlayer.x -= GAME_PLAYER_INC_X;
        else
            direction = 0;
    }
}


////////////////////////////////////////////////////
//Move ball on the display, bouncing off tile,
//wall, top, or player.
void game_ball_move(void)
{
	uint8_t bBot, bTop, bLeft, bRight, bX, bY = 0;		//ball
	uint8_t tBot, tTop, tLeft, tRight, tX, tY = 0;		//tile/player
    uint8_t i, j = 0;
    int multiplier = 0;
    int dx, dy;
	uint8_t flagTile = 0;
	uint8_t flagBoundary = 0;
	uint8_t tileIndex = 0;
	Direction_t updated = mBall.direction;
    
	//evaluate the location of the ball
	bBot = mBall.y + mBall.sizeY - GAME_BALL_PADDING;
	bTop = mBall.y + GAME_BALL_PADDING;
	bLeft = mBall.x;
	bRight = mBall.x + mBall.sizeX;
	bX = (bLeft + bRight) / 2;
	bY = (bTop + bBot) / 2;

	///////////////////////////////////////////
	//connect with player
	tTop = mPlayer.y;
	tBot = mPlayer.y + mPlayer.sizeY;
	tLeft = mPlayer.x;
	tRight = mPlayer.x + mPlayer.sizeX;
	tY = (tBot + tTop) / 2;

	//ball hit player, with downward direction only
	if ((bX >= tLeft) && (bX <= tRight) && (bBot >= tTop) && (bBot <= (tTop + 4))
			&& (mBall.direction > DIRECTION_180))
	{

		//play sound
        Sound_playBallBouncePlayer();

		//corner - LHS
		if (bLeft <= SCREEN_LEFT)
			updated = DIRECTION_45;
		//corner - RHS
		else if (bRight >= SCREEN_RIGHT)
			updated = DIRECTION_135;
		else
			updated = game_getReboundAngle(mBall.direction, ORIENTATION_HORIZONTAL);
	}

	//ball drops out the bottom - set a flag to be evaluated
    //in main loop.  Decrement numplayers in main loop
	else if (bTop >= SCREEN_BOTTOM)
    {
		gameBallMissedFlag = 1;
    }
    
    
	//top - lhs corner
	else if ((bTop <= SCREEN_TOP) && (bLeft <= SCREEN_LEFT))
	{
		updated = DIRECTION_315;
		flagBoundary = 1;
	}

	//top - rhs corner
	else if ((bTop <= SCREEN_TOP) && (bRight >= SCREEN_RIGHT))
	{
		updated = DIRECTION_225;
		flagBoundary = 1;
	}

	//top - in the middle somewhere
	else if ((bTop <= SCREEN_TOP) && (bLeft > SCREEN_LEFT) && (bRight < SCREEN_RIGHT))
	{
		updated = game_getReboundAngle(mBall.direction, ORIENTATION_HORIZONTAL);
		flagBoundary = 1;
	}

	else if (bLeft <= SCREEN_LEFT)
	{
		//check if it hit it parallel
		if ((mBall.direction == DIRECTION_90) || (mBall.direction == DIRECTION_116))
			updated = DIRECTION_45;
		else if ((mBall.direction == DIRECTION_270) || (mBall.direction == DIRECTION_243))
			updated = DIRECTION_315;
		else
			updated = game_getReboundAngle(mBall.direction, ORIENTATION_VERTICAL);

		flagBoundary = 1;
	}

	else if (bRight >= SCREEN_RIGHT)
	{
		//check if it hit it parallel
		if ((mBall.direction == DIRECTION_90) || (mBall.direction == DIRECTION_63))
			updated = DIRECTION_135;
		else if ((mBall.direction == DIRECTION_270) || (mBall.direction == DIRECTION_296))
			updated = DIRECTION_225;
		else
			updated = game_getReboundAngle(mBall.direction, ORIENTATION_VERTICAL);

		flagBoundary = 1;
	}

	//hit a tile?
	else
	{
		gameBallMissedFlag = 0;

		//for each tile....
		for (i = 0 ; i < GAME_TILE_NUM_TILE ; i++)
		{
			if (mTile[i].alive == 1)
			{
				tBot = mTile[i].y + mTile[i].sizeY;
				tTop = mTile[i].y;
				tLeft = mTile[i].x;
				tRight = mTile[i].x + mTile[i].sizeX;
				tX = (tLeft + tRight) / 2;
				tY = (tTop + tBot) / 2;

				//connect on the top of the ball - bottom of a tile
				if ((bTop <= tBot) && (bTop >= tY) && (bX >= tLeft) && (bX <= tRight))
				{
					//top of ball hit bottom of a tile
					flagTile = 1;
					tileIndex = i;
					i = GAME_TILE_NUM_TILE - 1;
					updated = game_getReboundAngle(mBall.direction, ORIENTATION_HORIZONTAL);
					break;
				}

				//connect on the bottom of ball top of the tile
				else if ((bBot >= tTop) && (bBot <= tY) && (bX >= tLeft) && (bX <= tRight))
				{
					//bottom of ball hit the top of a tile
					flagTile = 1;
					tileIndex = i;
					i = GAME_TILE_NUM_TILE - 1;
					updated = game_getReboundAngle(mBall.direction, ORIENTATION_HORIZONTAL);
					break;
				}

				//connect right side of ball with left side of tile
				else if ((bRight >= tLeft) && (bRight <= tX) && (bY >= tTop) && (bY <= tBot))
				{
					//bottom of ball hit the top of a tile
					flagTile = 1;
					tileIndex = i;
					i = GAME_TILE_NUM_TILE - 1;
					updated = game_getReboundAngle(mBall.direction, ORIENTATION_VERTICAL);
					break;
				}

				//connect left side of ball with right side of tile
				else if ((bLeft <= tRight) && (bLeft >= tX) && (bY >= tTop) && (bY <= tBot))
				{
					//bottom of ball hit the top of a tile
					flagTile = 1;
					tileIndex = i;
					i = GAME_TILE_NUM_TILE - 1;
					updated = game_getReboundAngle(mBall.direction, ORIENTATION_VERTICAL);
					break;
				}

				//don't do anything, and the updated stays as it
				//was at the the top of the function
				else
				{
					flagTile = 0;
				}
			}
		}
	}

	//play only one sound if ball hits tile and boundary
	//at the same time

	//remove the tile
	if (flagTile == 1)
	{
		mTile[tileIndex].alive = 0;				        //set life to 0
		mTile[tileIndex].imagePtr = &bmimgTileBlank_8;	//clear the image
		gameScore += mTile[tileIndex].points;

		//play sound
        Sound_playBallBounceTile();
        
        if (!game_getNumTiles())
        {
            game_setLevelUpFlag();
        }
	}

	//hit a wall
	else if (flagBoundary == 1)
	{
		Sound_playBallBounceWall();
	}

	//compute new x and y position from updated
	//index x and y position by dx and dy
	mBall.direction = updated;
	dx = game_get_dx_fromAngle(updated);
	dy = game_get_dy_fromAngle(updated);

//  TODO:   multiplier = (int)Sprite_GetGameSpeed();
    multiplier = 1;

	if ((dx < 0) && (mBall.x <=SCREEN_LEFT))
	{
		mBall.x += ((-1*dx) * multiplier);
		mBall.y += (dy * multiplier);
	}
	else
	{
		mBall.x += (dx * multiplier);
		mBall.y += (dy * multiplier);
	}

}



////////////////////////////////////////////////
//Return the rebound direction based current
//direction and surface orientation the object
//collides with
Direction_t game_getReboundAngle(Direction_t direction, Orientation_t orientation)
{
    if (orientation == ORIENTATION_HORIZONTAL)
	{
		switch(direction)
		{

			case DIRECTION_0:	return	DIRECTION_0;
			case DIRECTION_26:	return	DIRECTION_333;
			case DIRECTION_45:	return	DIRECTION_315;
			case DIRECTION_63:	return	DIRECTION_296;
			case DIRECTION_90:	return	DIRECTION_270;
			case DIRECTION_116:	return	DIRECTION_243;
			case DIRECTION_135:	return	DIRECTION_225;
			case DIRECTION_153:	return	DIRECTION_206;
			case DIRECTION_180:	return	DIRECTION_180;
			case DIRECTION_206:	return	DIRECTION_153;
			case DIRECTION_225:	return	DIRECTION_135;
			case DIRECTION_243:	return	DIRECTION_116;
			case DIRECTION_270:	return	DIRECTION_90;
			case DIRECTION_296:	return	DIRECTION_63;
			case DIRECTION_315:	return	DIRECTION_45;
			case DIRECTION_333:	return	DIRECTION_26;
		}
	}
    
	else if (orientation == ORIENTATION_VERTICAL)
	{
		switch(direction)
		{
			case DIRECTION_0:	return	DIRECTION_180;
			case DIRECTION_26:	return	DIRECTION_153;
			case DIRECTION_45:	return	DIRECTION_135;
			case DIRECTION_63:	return	DIRECTION_116;
			case DIRECTION_90:	return	DIRECTION_90;
			case DIRECTION_116:	return	DIRECTION_63;
			case DIRECTION_135:	return	DIRECTION_45;
			case DIRECTION_153:	return	DIRECTION_26;
			case DIRECTION_180:	return	DIRECTION_0;
			case DIRECTION_206:	return	DIRECTION_333;
			case DIRECTION_225:	return	DIRECTION_315;
			case DIRECTION_243:	return	DIRECTION_296;
			case DIRECTION_270:	return	DIRECTION_270;
			case DIRECTION_296:	return	DIRECTION_243;
			case DIRECTION_315:	return	DIRECTION_225;
			case DIRECTION_333:	return	DIRECTION_206;
		}
	}
	else
	{
		//do nothing
	}

	return DIRECTION_0;
}



///////////////////////////////////////////////////
//Return the delta x value based on the direction
//the object is moving.
int game_get_dx_fromAngle(Direction_t angle)
{
    switch(angle)
	{
		case DIRECTION_0:	return 2;
		case DIRECTION_26:	return 2;
		case DIRECTION_45:	return 2;
		case DIRECTION_63:	return 1;
		case DIRECTION_90:	return 0;
		case DIRECTION_116:	return -1;
		case DIRECTION_135:	return -2;
		case DIRECTION_153:	return -2;
		case DIRECTION_180:	return -2;
		case DIRECTION_206:	return -2;
		case DIRECTION_225:	return -2;
		case DIRECTION_243:	return -1;
		case DIRECTION_270:	return	0;
		case DIRECTION_296:	return 1;
		case DIRECTION_315:	return 2;
		case DIRECTION_333:	return 2;
	}

	return 0;
}

//////////////////////////////////////////////
//Returns delta y value based on the direction
//the object is moving
int game_get_dy_fromAngle(Direction_t angle)
{
    switch(angle)
	{
		case DIRECTION_0:	return 0;
		case DIRECTION_26:	return -1;
		case DIRECTION_45:	return -2;
		case DIRECTION_63:	return -2;
		case DIRECTION_90:	return -2;
		case DIRECTION_116:	return -2;
		case DIRECTION_135:	return -2;
		case DIRECTION_153:	return -1;
		case DIRECTION_180:	return 0;
		case DIRECTION_206:	return 1;
		case DIRECTION_225:	return 2;
		case DIRECTION_243:	return 2;
		case DIRECTION_270:	return 2;
		case DIRECTION_296:	return 2;
		case DIRECTION_315:	return 2;
		case DIRECTION_333:	return 1;
	}

	return 0;
}




void game_clearBallMissedFlag(void)
{
    gameBallMissedFlag = 0;
}

uint8_t game_getBallMissedFlag(void)
{
    return gameBallMissedFlag;
}


void game_clearLevelUpFlag(void)
{
    gameLevelUpFlag = 0;
}

uint8_t game_getLevelUpFlag(void)
{
    return gameLevelUpFlag;
}

void game_setLevelUpFlag(void)
{
    gameLevelUpFlag = 1;
}


uint8_t game_getGameOverFlag(void)
{
    return gameOverFlag;
}

void game_clearGameOverFlag(void)
{
    gameOverFlag = 0;
}


void game_setGameOverFlag(void)
{
    gameOverFlag = 1;
}




void game_setGameMode(GameMode_t mode)
{
    if ((mode == GAME_MODE_AUTO) || (mode == GAME_MODE_MANUAL))
        mGameMode = mode;
}

GameMode_t game_getGameMode(void)
{
    return mGameMode;    
}


GameMode_t game_toggleGameMode(void)
{
    if (mGameMode == GAME_MODE_AUTO)
        mGameMode = GAME_MODE_MANUAL;
    else
        mGameMode = GAME_MODE_AUTO;
    
    return mGameMode;
}

void game_setGameLevel(uint8_t level)
{
    gameLevel = level;
}

uint8_t game_getGameLevel(void)
{
    return gameLevel;
}

void game_incrementGameLevel(void)
{
    gameLevel++;
}


uint16_t game_getGameScore(void)
{
    return gameScore;
}




uint8_t game_getNumTiles(void)
{
    uint8_t remaining = 0;
    uint8_t i = 0;
    
    for (i = 0 ; i < GAME_TILE_NUM_TILE ; i++)
    {
        if (mTile[i].alive == 1)
            remaining++;
    }
    
    return remaining;
}


uint8_t game_getNumPlayers(void)
{
    return mPlayer.lives;
}

uint8_t game_decrementPlayer(void)
{
    if (mPlayer.lives > 0)
        mPlayer.lives--;
    else
        mPlayer.lives = 0;
        
    return mPlayer.lives;
}

void game_setNumPlayers(uint8_t num)
{
    mPlayer.lives = num;
}



