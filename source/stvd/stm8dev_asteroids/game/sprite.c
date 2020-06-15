/*
////////////////////////////////////////////////////////
Sprite Files
Controller file for managing the objects in the game.
These include:
player - ship that moves and rotates and fires
ememy - asteroids, these move up, down, left, right
missile - missiles shot from the player ship

Movements occur in the main polling loop, flags
are set here and polled in the main loop for
player hit, asteroid hit, level up, etc.

/////////////////////////////////////////////////////////
*/
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "register.h"
#include "system.h"         //interrupts
#include "timer.h"          //delay
#include "sprite.h"
#include "lcd.h"
#include "bitmap.h"
#include "gpio.h"
#include "sound.h"
#include "eeprom.h"

//player, astroid, missiles
static PlayerStruct mPlayer;
static AstroidStruct mAstroid[NUM_ASTROID];
static MissileStruct mMissile[NUM_MISSILE];

//flags - polled in main loop
static uint8_t mMissileLaunchFlag = 0;			//missile launch
static uint8_t mPlayerThrustFlag = 0;			//fire thrusters
static uint8_t mPlayerHitFlag = 0;              //player hit by asteroid
static uint8_t mAsteroidHitFlag = 0;            //asteroid is hit by missile
static uint8_t mLevelUpFlag = 0;                //new level flag
static uint8_t mGameOverFlag = 0;               //game over

static uint16_t mGameScore = 0;
static uint8_t mGameLevel = 0;
static uint16_t mGameHighScore = 0;


///////////////////////////////////////////
//Local delay not linked to a timer
//
void Sprite_DummyDelay(uint16_t delay)
{
    volatile uint16_t time = delay;

    while (time > 0)
        time--;
}

/////////////////////////////////////
//init all sprites in the game
void Sprite_Init(void)
{
	//init flags
    mMissileLaunchFlag = 0x00;		//missile launch
    mPlayerThrustFlag = 0x00;		//fire thrusters
    mPlayerHitFlag = 0;             //player hit by asteroid
    mAsteroidHitFlag = 0;           //asteroid is hit by missile
    mLevelUpFlag = 0;               //new level flag
    mGameOverFlag = 0;
    
    mGameScore = 0x00;
    mGameLevel = 1;
    mGameHighScore = 0x00;

    Sprite_Player_Init();				//init sprites
    Sprite_Astroid_Init(SPRITE_SPEED_MEDIUM);
    Sprite_Missile_Init();
}




////////////////////////////////////////////
//Player init - sets up the mPlayer struct
//at an initial position on the screen, lives,
//movement, etc
void Sprite_Player_Init(void)
{
	mPlayer.numLives = PLAYER_DEFAULT_LIVES;
    mPlayer.image = &bmimgPlayer0Bmp;
	mPlayer.x = PLAYER_DEFAULT_X;
	mPlayer.y = PLAYER_DEFAULT_Y;
	mPlayer.sizeX = bmimgPlayer0Bmp.xSize;
	mPlayer.sizeY = bmimgPlayer0Bmp.ySize;
	mPlayer.direction = SPRITE_DIRECTION_0;
	mPlayer.rotation = SPRITE_DIRECTION_0;
	mPlayer.speed = SPRITE_SPEED_STOP;
	mPlayer.thrusterOn = 0x00;		//off
	mPlayer.thrustTimeout = SPRITE_THRUSTER_TIMEOUT_VALUE;		//reset timeout
}


////////////////////////////////////////////////
//Astroid init
//Sets up the mAstroid array with 1/2 of the
//astroids on one side of the screen and 1/2 on
//the other.  Offset left and right are random
//they start out moving up and down, but the
//directions get altered in the main loop.
//
//initializes the astroids with an initial
//speed
void Sprite_Astroid_Init(SpriteSpeed_t speed)
{
    uint8_t i = 0;
    uint8_t count = 0;
    uint8_t size, sp = 0;
    
    for (i = 0 ; i < NUM_ASTROID / 2 ; i++)
    {
    	uint32_t left = rand() % (20 + 1 - 0) + 0;		//random offset
    	uint32_t offset = rand() % (9 + 1 - 4) + 4;		//random offset

		mAstroid[count].life = 1;                           //life - 1 = alive, 0 = dead
		mAstroid[count].image = &bmimgAsteroidSMBmp;             		//pointer to image data
		mAstroid[count].points = 30;                        //points
		mAstroid[count].x = left;              				//x position
		mAstroid[count].y = i * (bmimgAsteroidSMBmp.ySize + offset); 	//y position
		mAstroid[count].sizeX = bmimgAsteroidSMBmp.xSize;        		//image width
		mAstroid[count].sizeY = bmimgAsteroidSMBmp.ySize;        		//image height
		mAstroid[count].bearing = BEARING_0;   				//initial direction
		mAstroid[count].speed = speed;						//initial speed
		mAstroid[count].size = ASTROID_SIZE_SMALL;     		//moving down

		count++;
    }

    for (i = 0; i < NUM_ASTROID / 2 ; i++)
    {
    	uint32_t right = rand() % (100 + 1 - 80) + 50;	//random offset
    	uint32_t offset = rand() % (9 + 1 - 4) + 4;		//random offset

		mAstroid[count].life = 1;                           //life - 1 = alive, 0 = dead
		mAstroid[count].image = &bmimgAsteroidSMBmp;             		//pointer to image data
		mAstroid[count].points = 30;                        //points
		mAstroid[count].x = right;              			//x position
		mAstroid[count].y = i * (bmimgAsteroidSMBmp.ySize + offset); 	//y position
		mAstroid[count].sizeX = bmimgAsteroidSMBmp.xSize;        		//image width
		mAstroid[count].sizeY = bmimgAsteroidSMBmp.ySize;        		//image height
		mAstroid[count].bearing = BEARING_180;   			//initial direction
		mAstroid[count].speed = speed;						//initial speed
		mAstroid[count].size = ASTROID_SIZE_SMALL;     		//moving down

		count++;
    }

    //for all asteroids, randomly resize and assign speed
    //according to size.
    for (i = 0 ; i < NUM_ASTROID ; i++)
    {
    	size = rand() % 2;		//random size
    	sp = rand() % 2;		//random speed

    	//size
    	switch(size)
    	{
			case 0:
			{
				mAstroid[i].image = &bmimgAsteroidSMBmp;
				mAstroid[i].sizeX = bmimgAsteroidSMBmp.xSize;
				mAstroid[i].sizeY = bmimgAsteroidSMBmp.ySize;
				mAstroid[i].size = ASTROID_SIZE_SMALL;
				break;
			}
			case 1:
			{
				mAstroid[i].image = &bmimgAsteroidMDBmp;
				mAstroid[i].sizeX = bmimgAsteroidMDBmp.xSize;
				mAstroid[i].sizeY = bmimgAsteroidMDBmp.ySize;
				mAstroid[i].size = ASTROID_SIZE_MEDIUM;
				break;
			}
			default:
			{
				mAstroid[i].image = &bmimgAsteroidSMBmp;
				mAstroid[i].sizeX = bmimgAsteroidSMBmp.xSize;
				mAstroid[i].sizeY = bmimgAsteroidSMBmp.ySize;
				mAstroid[i].size = ASTROID_SIZE_SMALL;
				break;
			}
    	}

    	//speed
    	switch(sp)
    	{
			case 0: 	mAstroid[i].speed = SPRITE_SPEED_SLOW;		break;
			case 1: 	mAstroid[i].speed = SPRITE_SPEED_MEDIUM;	break;
			default:	mAstroid[i].speed = SPRITE_SPEED_SLOW;		break;
    	}
    }
}


///////////////////////////////////////
//Missile Init
//Setup the missile array with initial
//position, speed, directin, etc.
//
void Sprite_Missile_Init(void)
{
    uint8_t i = 0;
    for (i = 0 ; i < NUM_MISSILE ; i++)
    {
        mMissile[i].life = 0;                          //life - 1 = alive, 0 = dead
        mMissile[i].x = 0;
        mMissile[i].y = 0;
        mMissile[i].size = 10;
        mMissile[i].direction = SPRITE_DIRECTION_0;		//should be heading of player
        mMissile[i].speed = SPRITE_SPEED_STOP;
    }
}



///////////////////////////////////////////
//Player move.
//player moves in direction, not rotation
//for each call to player move, the thruster
//timeout tick decrements by 1.  timeout
//and thrusters on are stored in player sturct
//one timeout complete, the speed goes
//to 0
//Player can move multiple pixels on a single move,
//not like astroids.
//
void Sprite_Player_Move(void)
{
    int dx, dy = 0;
    uint8_t delta = 0x00;
    
	if ((mPlayer.thrustTimeout > 0) && (mPlayer.speed != SPRITE_SPEED_STOP))
	{
		if (mPlayer.numLives > 0)
		{
			//move player according to direction not rotation.
			dx = Sprite_GetDX_FromDirection(mPlayer.direction, mPlayer.speed);
			dy = Sprite_GetDY_FromDirection(mPlayer.direction, mPlayer.speed);

			//move the ship and wrap - x direction
			if ((dx  >  0) && ((mPlayer.x + dx) <= (FRAME_BUFFER_WIDTH - 1)))
				mPlayer.x += dx;
                
			//moving right, wrap
			else if ((dx  >  0) && ((mPlayer.x + dx) > (FRAME_BUFFER_WIDTH - 1)))
				mPlayer.x = (mPlayer.x + dx) - FRAME_BUFFER_WIDTH;

			//moving left - no wrap
			else if ((dx  <  0) && (((int)mPlayer.x + dx) >= 0 ))
				mPlayer.x -= (uint8_t)(-1 * dx);

			//moving left - wrap
			else if ((dx  <  0) && (((-1*dx) > mPlayer.x)))
                mPlayer.x = FRAME_BUFFER_WIDTH - (uint8_t)(-1 * dx);

			////////////////////////////////////////////////////
			//y-direction
			//moving down - dy > 0            
			if (dy > 0)
			{
				if ((mPlayer.y + dy) <= (FRAME_BUFFER_HEIGHT - 1))
					mPlayer.y += dy;

				else
					mPlayer.y = (mPlayer.y + dy) - FRAME_BUFFER_HEIGHT;
			}

			else if (!dy)
				mPlayer.y = mPlayer.y;

			//moving up
			else
			{
				delta = (uint8_t)(-1*dy);

				if (mPlayer.y >= delta)
					mPlayer.y -= delta;
				else
					mPlayer.y = FRAME_BUFFER_HEIGHT + mPlayer.y - delta;
			}

		}
		else
		{
			//no other lives. - dont move, put player
            //at default x and y as a sign something is wrong
            mPlayer.x = PLAYER_DEFAULT_X;
            mPlayer.y = PLAYER_DEFAULT_Y;
		}

		//decrement the timeout
		mPlayer.thrustTimeout--;

		//after decrementing, check if it timed out and
		//speed reduces to slow
		if (!mPlayer.thrustTimeout)
		{
			if (mPlayer.speed == SPRITE_SPEED_MEDIUM)
			{
				mPlayer.speed = SPRITE_SPEED_SLOW;						//reduce speed
				mPlayer.thrustTimeout = SPRITE_THRUSTER_TIMEOUT_VALUE;	//reset
			}
			else if (mPlayer.speed == SPRITE_SPEED_SLOW)
			{
				mPlayer.speed = SPRITE_SPEED_STOP;
				mPlayer.thrustTimeout = 0;
			}
		}
	}

	else
	{
		//thruster = 0, so set speed to stop, timeout
		//is reset in the thruster event.
		mPlayer.speed = SPRITE_SPEED_STOP;
	}

}

/////////////////////////////////////////////////////
//Astroid Move
//Loop over all astroid.  If the life == 1,
//move the astroid based on speed and bearing.
//
//
void Sprite_Astroid_Move(void)
{
	int dx, dy;
	uint32_t pX, pY, aTop, aBot, aLeft, aRight = 0x00;
    uint8_t i = 0;
    uint8_t delta = 0x00;
    int rem = 0x00;

	for (i = 0 ; i < NUM_ASTROID ; i++)
	{
		if (mAstroid[i].life == 1)
		{
			dx = Sprite_GetDX_FromBearing(mAstroid[i].bearing, mAstroid[i].speed);
			dy = Sprite_GetDY_FromBearing(mAstroid[i].bearing, mAstroid[i].speed);

			//////////////////////////////////////////////////
			//x-direction
			if ((dx  >  0) && ((mAstroid[i].x + dx) <= (FRAME_BUFFER_WIDTH - 1)))
				mAstroid[i].x += dx;

			//moving right, wrap
			else if ((dx  >  0) && ((mAstroid[i].x + dx) > (FRAME_BUFFER_WIDTH - 1)))
				mAstroid[i].x = (mAstroid[i].x + dx) - FRAME_BUFFER_WIDTH;
            
			//moving left
			else if (dx < 0)
			{
				delta = (uint8_t)(-1 * dx);

				//moving left, no wrap
				if (mAstroid[i].x >= delta)
					mAstroid[i].x -= delta;
				else
				{
					mAstroid[i].x = FRAME_BUFFER_WIDTH + mAstroid[i].x - delta;
				}
			}


			//////////////////////////////////////////////////
			//y-direction
			//moving down, no wrap
			if ((dy  >  0) && ((mAstroid[i].y + dy) <= (FRAME_BUFFER_HEIGHT -1)))
				mAstroid[i].y += dy;

			//moving down, wrap
			else if ((dy  >  0) && ((mAstroid[i].y + dy) > (FRAME_BUFFER_HEIGHT - 1)))
				mAstroid[i].y = (mAstroid[i].y + dy) - FRAME_BUFFER_HEIGHT;

			else if (dy < 0)
			{
				delta = (uint8_t)(-1 * dy);

				//moving up - no wrap
				if (mAstroid[i].y >= delta)
					mAstroid[i].y -= delta;
				else
				{
					mAstroid[i].y = FRAME_BUFFER_HEIGHT + mAstroid[i].y - delta;
				}
			}


			//////////////////////////////////////////////
			//Collisions - Astroid Hit Player?
			//astroid index i, center of player
			//in the footprint of the astroid

			pX = mPlayer.x + mPlayer.sizeX / 2;
			pY = mPlayer.y + mPlayer.sizeY / 2;

			//center of the player has to be in the footprint of the astroid
			aTop = mAstroid[i].y;
			aBot = mAstroid[i].y + mAstroid[i].sizeY;
			aLeft = mAstroid[i].x;
			aRight = mAstroid[i].x + mAstroid[i].sizeX;

			if ((pX >= aLeft) && (pX <= aRight) && (pY >= aTop) && (pY <= aBot))
			{
				//astroid hit player - remove a player and astroid
				//play sound, increment score, etc.
				//repositions the player
				rem = Sprite_Astroid_ScorePlayerHit(i);

				//if !rem, all astroid is cleared and reset
				if (!rem)
				{
                    mLevelUpFlag = 1;       //set the levelup flag- clear in main
					mGameLevel++;
					Sprite_Astroid_Init(Sprite_GetGameSpeedFromLevel());  //reset the astroid
				}
			}
		}
	}
}


////////////////////////////////////////////
//loop over all player missile and
//enemy missile.  if life == 1
//move it.  Player missiles move up (y-)
//and enemy missiles move down (y+)
void Sprite_Missile_Move(void)
{
    uint16_t mX, mY, bot, top, left, right = 0x00;
    uint32_t delta = 0x00;
    uint8_t i, j = 0;

    for (i = 0 ; i < NUM_MISSILE ; i++)
    {
    	//alive and moving - moving the missiles
    	if ((mMissile[i].life == 1) && (mMissile[i].speed != SPRITE_SPEED_STOP))
    	{
        	//get dx and dy from direction and speed and move
        	int dx = Sprite_GetDX_FromDirection(mMissile[i].direction, mMissile[i].speed);
        	int dy = Sprite_GetDY_FromDirection(mMissile[i].direction, mMissile[i].speed);

        	//x direction
        	if (dx < 0)
        	{
        		delta = (-1*dx);

        		if (mMissile[i].x >= delta)
        			mMissile[i].x -= delta;
        		else
        		{
        			//delta is greater than x....  its moving off the screen, so kill it
        			mMissile[i].life = 0;
        			mMissile[i].speed = SPRITE_SPEED_STOP;
        			mMissile[i].direction = SPRITE_DIRECTION_0;
        		}
        	}

        	//positive dx - moving right
        	else
        	{
        		delta = dx;

        		if ((mMissile[i].x + delta) <= (FRAME_BUFFER_WIDTH - 1))
        			mMissile[i].x += delta;

        		else
        		{
        			//delta is greater than x....  its moving off the screen, so kill it
        			mMissile[i].life = 0;
        			mMissile[i].speed = SPRITE_SPEED_STOP;
        			mMissile[i].direction = SPRITE_DIRECTION_0;
        		}
        	}


        	//y direction
			if (dy < 0)
			{
				delta = (-1*dy);

				if (mMissile[i].y >= delta)
					mMissile[i].y -= delta;
				else
				{
					//delta is greater than y....  its moving off the screen, so kill it
					mMissile[i].life = 0;
					mMissile[i].speed = SPRITE_SPEED_STOP;
					mMissile[i].direction = SPRITE_DIRECTION_0;
				}
			}

			//positive dy - moving down or nothing
			else
			{
				delta = dy;

				if ((mMissile[i].y + delta) <= (FRAME_BUFFER_HEIGHT - 1))
					mMissile[i].y += delta;

				else
				{
					//delta is greater than y - it's moving off bottom of screen
					mMissile[i].life = 0;
					mMissile[i].speed = SPRITE_SPEED_STOP;
					mMissile[i].direction = SPRITE_DIRECTION_0;
				}
			}
    	}

    	//done moving and leaving the screen.... now, check
    	//to see if it hit anything
    	if ((mMissile[i].life == 1) && (mMissile[i].speed != SPRITE_SPEED_STOP))
		{
    		for (j = 0 ; j < NUM_ASTROID ; j++)
    		{
    			//hit a live astroid...
    			if (mAstroid[j].life == 1)
    			{
    				mX = mMissile[i].x + mMissile[i].size / 2;	//use center of the blob
    				mY = mMissile[i].y + mMissile[i].size / 2;	//use center of the blob

					bot = mAstroid[j].y + mAstroid[j].sizeY - ASTROID_IMAGE_PADDING;
					top = mAstroid[j].y + ASTROID_IMAGE_PADDING;
					left = mAstroid[j].x + ASTROID_IMAGE_PADDING;
					right = mAstroid[j].x + mAstroid[j].sizeX - ASTROID_IMAGE_PADDING;

					//tip of the missile in the enemy box?
					if ((mX >= left) && (mX <= right) && (mY <= bot) && (mY >= top))
					{
						//score hit!! - pass astroid index and missile index and
						//returns remaining astroids

						int rem = Sprite_Missile_ScoreAstroidHit(j, i);

						//if !rem, all astroid is cleared and reset
						if (!rem)
						{
                            Sound_playLevelUp(); 	//play a sound
							mGameLevel++;
							Sprite_Astroid_Init(Sprite_GetGameSpeedFromLevel());  //reset the astroid

						}
					}
    			}
    		}		//end of astroid loop
		}
    }
}




/////////////////////////////////////////////
//get the array index of the next available
//missile and set life = 1, direction and
//speed same as player
uint8_t Sprite_Missile_Launch(void)
{
    int dx, dy = 0;
    int nextMissile = Sprite_GetNextMissile();
    uint8_t result = 0x00;

    if (nextMissile >= 0)
    {
        result = 1;                 //missile available
        
        //sound is played in the main loop when result == 1

    	dx = Sprite_Missile_XOffsetFromPlayerRotation(mPlayer.rotation, mPlayer.speed);
    	dy = Sprite_Missile_YOffsetFromPlayerRotation(mPlayer.rotation, mPlayer.speed);

        //set the missile in the array as live
    	mMissile[nextMissile].life = 1;
    	mMissile[nextMissile].size = MISSILE_SIZE;
    	mMissile[nextMissile].direction = mPlayer.rotation;		//fire in direction of player rotation
    	mMissile[nextMissile].speed = SPRITE_SPEED_MEDIUM;

    	//set the x
    	if (dx >= 0)
    		mMissile[nextMissile].x = mPlayer.x + dx;
    	else
    	{
    		uint32_t d = (uint32_t)(-1* dx);
    		if (d <= mPlayer.x)
    			mMissile[nextMissile].x = mPlayer.x - d;
    		else
    			mMissile[nextMissile].x = mPlayer.x;
    	}

    	//set the y
    	if (dy >= 0)
    		mMissile[nextMissile].y = mPlayer.y + dy;
    	else
    	{
    		uint32_t d = (uint32_t)(-1* dy);
    		if (d <= mPlayer.y)
    			mMissile[nextMissile].y = mPlayer.y - d;
    		else
    			mMissile[nextMissile].y = mPlayer.y;
    	}
    }
    
    return result;
}


////////////////////////////////////////
//returns the index of the array element
//containing the next missile with live = 0
//returns -1 for no available missiles
int Sprite_GetNextMissile(void)
{
    uint8_t i = 0;
    for (i = 0 ; i < NUM_MISSILE ; i++)
    {
        if (!(mMissile[i].life))
            return i;
    }

    return - 1;
}



/////////////////////////////////////////////
void Sprite_SetMissileLaunchFlag(void)
{
    mMissileLaunchFlag = 1;
}

/////////////////////////////////////////////
uint8_t Sprite_GetMissileLaunchFlag(void)
{
    return mMissileLaunchFlag;
}

void Sprite_ClearMissileLaunchFlag(void)
{
	mMissileLaunchFlag = 0;
}



////////////////////////////////////////////////
//Score hit from player missile to the astroid.
//Play sound, reset the astroid to default values
//reset missile to default values.
//get the number of astroids remaining and return
//num remaining
//
int Sprite_Missile_ScoreAstroidHit(uint8_t astroidIndex, uint8_t missileIndex)
{
    int newIndex = 0;
    int rem = 0;
    
    Sound_playEnemyExplode();

	mGameScore += mAstroid[astroidIndex].points;	//get points if you get hit

	switch(mAstroid[astroidIndex].size)
	{
		case ASTROID_SIZE_SMALL:
		{
			//remove the astroid
			mAstroid[astroidIndex].life = 0;
			mAstroid[astroidIndex].x = 0;
			mAstroid[astroidIndex].y = 0;
			break;
		}

		//breaks into 2 smalls
		case ASTROID_SIZE_MEDIUM:
		{
			//convert to medium - no change in trajectory
			mAstroid[astroidIndex].image = &bmimgAsteroidSMBmp;
			mAstroid[astroidIndex].size = ASTROID_SIZE_SMALL;
			mAstroid[astroidIndex].sizeX = bmimgAsteroidSMBmp.xSize;
			mAstroid[astroidIndex].sizeY = bmimgAsteroidSMBmp.ySize;

			//add a small asteroid if there is an empty index
			newIndex = Sprite_GetNewAstroidIndex();


			if (newIndex >= 0)
			{
				mAstroid[newIndex].life = 1;                           //life - 1 = alive, 0 = dead
				mAstroid[newIndex].image = &bmimgAsteroidSMBmp;        //pointer to image data
				mAstroid[newIndex].points = 30;                        //points
				mAstroid[newIndex].x = mAstroid[astroidIndex].x;		//x position
				mAstroid[newIndex].y = mAstroid[astroidIndex].y;		//y
				mAstroid[newIndex].sizeX = bmimgAsteroidSMBmp.xSize;    //image width
				mAstroid[newIndex].sizeY = bmimgAsteroidSMBmp.ySize;	//image height
				mAstroid[newIndex].bearing = BEARING_10;   				//initial direction
				mAstroid[newIndex].speed = SPRITE_SPEED_SLOW;			//initial speed
				mAstroid[newIndex].size = ASTROID_SIZE_SMALL;

				//adjust bearing to make it opposite original
				//if original > =350, make it down
				if (mAstroid[astroidIndex].bearing >= BEARING_350)
					mAstroid[newIndex].bearing = BEARING_170;
			}

			else
			{
				//do nothing - the array is full
			}

			break;
		}

		default:
		{
			break;
		}
	}

	//all cases - remove the missile
	mMissile[missileIndex].life = 0;
	mMissile[missileIndex].x = 0;
	mMissile[missileIndex].y = 0;

	rem = Sprite_GetNumAstroid();

	return rem;
}




//////////////////////////////////////////////////
//Score - Astroid Hits Player
//Remove one player life.
//remove astroid
//play explosion and image sequence.
//use dummy delay
//set game over flag if it's the last player
//
int Sprite_Astroid_ScorePlayerHit(uint8_t astroidIndex)
{
    int rem = 0;
    uint8_t lives = 0;
    
    //set the player hit flag - clear in main loop
    mPlayerHitFlag = 1;
    
    Sound_playPlayerExplode();
    Sprite_DisplayPlayerExplosion();
    
	//remove the astroid
	mGameScore += mAstroid[astroidIndex].points;		//get points if you get hit
	mAstroid[astroidIndex].life = 0;					//dead
	mAstroid[astroidIndex].x = 0;						//default x
	mAstroid[astroidIndex].y = 0;						//default y
	mAstroid[astroidIndex].speed = SPRITE_SPEED_STOP;	//speed
    
    //remove player, check if its the last player
    if (mPlayer.numLives == 1)
    {
        //last player
        mGameOverFlag = 1;
        mPlayer.numLives = 0;
    }
    
    else
    {
        //remove the player
        mPlayer.numLives--;
        lives = mPlayer.numLives;       //store the new lives
        Sprite_Player_Init();           //reset the player position
        mPlayer.numLives = lives;       //update lives to current
    }
    
	rem = Sprite_GetNumAstroid();
	return rem;
}


///////////////////////////////////////////////
void Sprite_DisplayPlayerExplosion(void)
{
    lcd_drawIconWrap(mPlayer.x, mPlayer.y, &bmimgPlayerExp1Bmp, 1);		//refresh
    Sprite_DummyDelay(50000);
    lcd_drawIconWrap(mPlayer.x, mPlayer.y, &bmimgPlayerExp2Bmp, 1);		//refresh
    Sprite_DummyDelay(50000);
    lcd_drawIconWrap(mPlayer.x, mPlayer.y, &bmimgPlayerExp3Bmp, 1);		//refresh
    Sprite_DummyDelay(50000);
    Sprite_DummyDelay(50000);    
}




/////////////////////////////////////////////////
//Position the starting point of the missile
//from the player based on rotation and speed
//x offset
int Sprite_Missile_XOffsetFromPlayerRotation(SpriteDirection_t rotation, SpriteSpeed_t speed)
{
	int offset = 0x00;
	int dx = 0x00;

	///////////////////////////////////////
	//set the multiplier based on the speed
	switch(speed)
	{
		case SPRITE_SPEED_STOP: 	offset = 8;		break;
		case SPRITE_SPEED_SLOW: 	offset = 8;		break;
		case SPRITE_SPEED_MEDIUM: 	offset = 8;		break;
		default:					offset = 8;		break;
	}

	//////////////////////////////////////
	//set the dx from direction
	switch(rotation)
	{
		case SPRITE_DIRECTION_0:	dx =  mPlayer.sizeX;				break;
		case SPRITE_DIRECTION_26:	dx =  mPlayer.sizeX;				break;
		case SPRITE_DIRECTION_45:	dx =  mPlayer.sizeX;				break;
		case SPRITE_DIRECTION_63:	dx =  mPlayer.sizeX /2 + offset;	break;
		case SPRITE_DIRECTION_90:	dx =  mPlayer.sizeX /2;				break;
		case SPRITE_DIRECTION_116:	dx =  0;							break;
		case SPRITE_DIRECTION_135:	dx = -1 * (offset / 2);				break;
		case SPRITE_DIRECTION_153:	dx = -1 * offset / 2;				break;
		case SPRITE_DIRECTION_180:	dx = -1 * offset / 2;				break;
		case SPRITE_DIRECTION_206:	dx = -1 * offset / 2;				break;
		case SPRITE_DIRECTION_225:	dx = 0;								break;
		case SPRITE_DIRECTION_243:	dx = 0;								break;
		case SPRITE_DIRECTION_270:	dx = mPlayer.sizeX / 2;				break;
		case SPRITE_DIRECTION_296:	dx = mPlayer.sizeX - (offset / 2);	break;
		case SPRITE_DIRECTION_315:	dx = mPlayer.sizeX - offset;		break;
		case SPRITE_DIRECTION_333:	dx = mPlayer.sizeX;					break;
		default:					dx = 0;								break;
	}

	return dx;
}

/////////////////////////////////////////////////
//Position the starting point of the missile
//from the player based on rotation and speed
//y offset
int Sprite_Missile_YOffsetFromPlayerRotation(SpriteDirection_t rotation, SpriteSpeed_t speed)
{
	int offset = 0x00;
	int dy = 0x00;

	///////////////////////////////////////
	//set the multiplier based on the speed
	switch(speed)
	{
		case SPRITE_SPEED_STOP: 	offset = 8;		break;
		case SPRITE_SPEED_SLOW: 	offset = 8;		break;
		case SPRITE_SPEED_MEDIUM: 	offset = 8;		break;
		default:					offset = 8;		break;
	}

	//////////////////////////////////////
	//set the dx from direction
	switch(rotation)
	{
		case SPRITE_DIRECTION_0:	dy =  mPlayer.sizeY / 2;			break;
		case SPRITE_DIRECTION_26:	dy =  mPlayer.sizeY / 4;			break;
		case SPRITE_DIRECTION_45:	dy =  0;							break;
		case SPRITE_DIRECTION_63:	dy =  -1 * offset / 2;				break;
		case SPRITE_DIRECTION_90:	dy =  -1 * offset / 2;				break;
		case SPRITE_DIRECTION_116:	dy =  -1 * offset / 2;				break;
		case SPRITE_DIRECTION_135:	dy =  -1 * offset / 2;				break;
		case SPRITE_DIRECTION_153:	dy =   0;							break;
		case SPRITE_DIRECTION_180:	dy =  mPlayer.sizeY / 2;			break;
		case SPRITE_DIRECTION_206:	dy = mPlayer.sizeY;					break;
		case SPRITE_DIRECTION_225:	dy = mPlayer.sizeY + (offset / 4);	break;
		case SPRITE_DIRECTION_243:	dy = mPlayer.sizeY;					break;
		case SPRITE_DIRECTION_270:	dy = mPlayer.sizeY;					break;
		case SPRITE_DIRECTION_296:	dy = mPlayer.sizeY;					break;
		case SPRITE_DIRECTION_315:	dy = mPlayer.sizeY - offset;		break;
		case SPRITE_DIRECTION_333:	dy = (mPlayer.sizeY / 2) + offset;	break;
		default:					dy = 0;								break;
	}

	return dy;
}


////////////////////////////////////
//Get remaining number of players
uint8_t Sprite_GetNumPlayers(void)
{
    return mPlayer.numLives;
}

////////////////////////////////////
//Get number of live astroids
int Sprite_GetNumAstroid(void)
{
    int num = 0;
    uint8_t i = 0;
    for (i = 0 ; i < NUM_ASTROID ; i++)
    {
        if (mAstroid[i].life == 1)
            num++;
    }

    return num;
}


///////////////////////////////////////////////
//GetRandomAstroid
//Returns the index of a randomly selected
//astroid from a collection of live astroids.
//
int Sprite_GetRandomAstroid(void)
{
    int i, num = 0;
    num = Sprite_GetNumAstroid();

    if (num > 0)
    {
        //get the random index
        //rand() % (max_number + 1 - minimum_number) + minimum_number
        //index is the count within a set of live astroids.
        int index = rand() % (num - 1 + 1 - 0) + 0;
        int counter = 0;

        for (i = 0 ; i < NUM_ASTROID ; i++)
        {
            if (mAstroid[i].life == 1)
            {
                if (index == counter)
                    return counter;

                counter++;      //increment only for live enemy
            }
        }
    }

    return -1;
}




////////////////////////////////////////////////////
//Returns the index in the array of astroids
//that is empty.
int Sprite_GetNewAstroidIndex(void)
{
    uint8_t i = 0;
	for (i = 0 ; i < NUM_ASTROID ; i++)
	{
		if (!(mAstroid[i].life))
			return i;
	}

	return - 1;
}


//////////////////////////////////////////
//Change the trajectory of a randomly selected
//astroid.  Change by one enum value in Bearing,
//randomly move increase or decrease in bearing
//offset from true/vertical.  Keep in range
//
int Sprite_WormHole(void)
{
	int index = Sprite_GetRandomAstroid();
    int shift = rand() % (2) + 1;		//1 or 2

	//valid??
	if (index >= 0)
	{
		Bearing_t bearing = mAstroid[index].bearing;

		//bearing shift clockwise or ccw based on shift
		if (shift == 1)
		{
			switch (bearing)
			{
				case BEARING_0: 	mAstroid[index].bearing = BEARING_5;		break;
				case BEARING_5: 	mAstroid[index].bearing = BEARING_10;		break;
				case BEARING_10: 	mAstroid[index].bearing = BEARING_5;		break;
				case BEARING_355: 	mAstroid[index].bearing = BEARING_0;		break;
				case BEARING_350: 	mAstroid[index].bearing = BEARING_355;		break;
				case BEARING_170: 	mAstroid[index].bearing = BEARING_175;		break;
				case BEARING_175: 	mAstroid[index].bearing = BEARING_180;		break;
				case BEARING_180: 	mAstroid[index].bearing = BEARING_185;		break;
				case BEARING_185: 	mAstroid[index].bearing = BEARING_190;		break;
				case BEARING_190: 	mAstroid[index].bearing = BEARING_185;		break;
				//represents an error, so make it obvious
				default:			mAstroid[index].bearing = BEARING_180;		break;
			}
		}
		else
		{
			switch (bearing)
			{
				case BEARING_0: 	mAstroid[index].bearing = BEARING_355;		break;
				case BEARING_5: 	mAstroid[index].bearing = BEARING_0;		break;
				case BEARING_10: 	mAstroid[index].bearing = BEARING_5;		break;
				case BEARING_355: 	mAstroid[index].bearing = BEARING_350;		break;
				case BEARING_350: 	mAstroid[index].bearing = BEARING_355;		break;

				case BEARING_170: 	mAstroid[index].bearing = BEARING_175;		break;
				case BEARING_175: 	mAstroid[index].bearing = BEARING_170;		break;
				case BEARING_180: 	mAstroid[index].bearing = BEARING_175;		break;
				case BEARING_185: 	mAstroid[index].bearing = BEARING_180;		break;
				case BEARING_190: 	mAstroid[index].bearing = BEARING_185;		break;
				//represents an error, so make it obvious
				default:			mAstroid[index].bearing = BEARING_0;		break;
			}
		}
	}

	return index;
}

/////////////////////////////////////////////////////////
//Update the display.
//Clear the framebuffer, draw player, astroids, missiles
//to the framebuffer, dump framebuffer to display,
//draw scrore along the top
//
void Sprite_UpdateDisplay(void)
{
    uint8_t buffer[10] = {0x00};
    uint8_t len = 0;

    lcd_clearFrameBuffer(0x00, 0);

	Sprite_Player_Draw();			//draw player
	Sprite_Astroid_Draw();			//draw astroids
	Sprite_Missle_Draw();			//draw missiles

	lcd_updateFrameBuffer();

    lcd_clearPage(0, 0x00);
	//draw score, level, players    
    lcd_drawString(0, 0, "S");
    len = lcd_decimalToBuffer(mGameScore, buffer, 10);
    lcd_drawStringLength(0, 8, buffer, len);

    lcd_drawString(0, 60, "L");
    len = lcd_decimalToBuffer(mGameLevel, buffer, 10);
    lcd_drawStringLength(0, 68, buffer, len);

    lcd_drawString(0, 84, "P");
    len = lcd_decimalToBuffer(mPlayer.numLives, buffer, 10);
    lcd_drawStringLength(0, 92, buffer, len);
    
}




/////////////////////////////////////////////
//Draws game over on the lcd
//
void Sprite_DisplayGameOver(void)
{
    lcd_clearFrameBuffer(0x00, 0);
    //draw player ship and asteroids at top
    lcd_drawIconWrap(50, 15, &bmimgPlayer45Bmp, 0);
    lcd_drawIconWrap(20, 20, &bmimgAsteroidMDBmp, 0);
    lcd_drawIconWrap(80, 10, &bmimgAsteroidSMBmp, 0);
    lcd_drawIconWrap(10, 10, &bmimgAsteroidSMBmp, 0);    
    lcd_updateFrameBuffer();
    
    lcd_clearPage(0, 0x00);                 //clear score, etc
    lcd_clearPage(6, 0x00);
    lcd_clearPage(7, 0x00);
                    
    lcd_drawString(0, 2, "Asteroids!!");
	lcd_drawString(6, 3, "Press Button");
	lcd_drawString(7, 15, "To Start");
}




/////////////////////////////////////////////
//Draw the player icon at the player x,y
//position.  Transparent color arg (in this
//case black) is based on the background of the
//player image.  Set to an obscure value if
//one does not want to use transparent color.
//Transparent color is not drawn.  (ie, pointer
//simply skips over it).
//
void Sprite_Player_Draw(void)
{
    if (mPlayer.numLives > 0)
    {
    	lcd_drawIconWrap(mPlayer.x, mPlayer.y, mPlayer.image, 0);
    }
}

////////////////////////////////////////////
//Draw Astroid
//Draw all astroids with life = 1
//
void Sprite_Astroid_Draw(void)
{
    uint8_t i = 0;
    for (i = 0 ; i < NUM_ASTROID ; i++)
    {
        if (mAstroid[i].life == 1)
        {
        	lcd_drawIconWrap(mAstroid[i].x, mAstroid[i].y, mAstroid[i].image, 0);
        }
    }
}



/////////////////////////////////////////////////
//Draw Missile.
//Draw single pixel for now
//
void Sprite_Missle_Draw(void)
{
    uint8_t i = 0;
    for (i = 0 ; i < NUM_MISSILE ; i++)
    {
        //check enemy missile
        if (mMissile[i].life == 1)
        {
        	//color = 1, no update = 0
        	lcd_putPixelRam(mMissile[i].x, mMissile[i].y, 1, 0);
        	lcd_putPixelRam(mMissile[i].x, mMissile[i].y + 1, 1, 0);
        	lcd_putPixelRam(mMissile[i].x + 1, mMissile[i].y, 1, 0);
        	lcd_putPixelRam(mMissile[i].x + 1, mMissile[i].y + 1, 1, 0);
        }
    }
}




////////////////////////////////////////////
//Helper - Get dX from direction and speed
//All dx and dy based on 2 pixel minimum delta.
//ie, 45 deg angle is 2 pixels up and 2 over
//for a multiplier of 1
//
int Sprite_GetDX_FromDirection(SpriteDirection_t direction, SpriteSpeed_t speed)
{
	int multiplier = 0x00;
	int dx = 0x00;

	///////////////////////////////////////
	//set the multiplier based on the speed
	switch(speed)
	{
		case SPRITE_SPEED_STOP: 	multiplier = 0;		break;
		case SPRITE_SPEED_SLOW: 	multiplier = 1;		break;
		case SPRITE_SPEED_MEDIUM: 	multiplier = 2;		break;
		default:					multiplier = 1;		break;
	}

	//////////////////////////////////////
	//set the dx from direction
	switch(direction)
	{
		case SPRITE_DIRECTION_0:	dx =  2 * multiplier;	break;
		case SPRITE_DIRECTION_26:	dx =  2 * multiplier;	break;
		case SPRITE_DIRECTION_45:	dx =  2 * multiplier;	break;
		case SPRITE_DIRECTION_63:	dx =  1 * multiplier;	break;
		case SPRITE_DIRECTION_90:	dx =  0 * multiplier;	break;
		case SPRITE_DIRECTION_116:	dx = -1 * multiplier;	break;
		case SPRITE_DIRECTION_135:	dx = -2 * multiplier;	break;
		case SPRITE_DIRECTION_153:	dx = -2 * multiplier;	break;
		case SPRITE_DIRECTION_180:	dx = -2 * multiplier;	break;
		case SPRITE_DIRECTION_206:	dx = -2 * multiplier;	break;
		case SPRITE_DIRECTION_225:	dx = -2 * multiplier;	break;
		case SPRITE_DIRECTION_243:	dx = -1 * multiplier;	break;
		case SPRITE_DIRECTION_270:	dx = 0 * multiplier;	break;
		case SPRITE_DIRECTION_296:	dx = 1 * multiplier;	break;
		case SPRITE_DIRECTION_315:	dx = 2 * multiplier;	break;
		case SPRITE_DIRECTION_333:	dx = 2 * multiplier;	break;
		default:					dx = 2 * multiplier;	break;
	}

	return dx;
}

/////////////////////////////////////////////
//Helper - Get dY from direction and speed
//All dy base on 2 pixel minimum delta.
//ie, 45 deg angle is 2 up and 2 over for a
//multiplier of 1.
//
int Sprite_GetDY_FromDirection(SpriteDirection_t direction, SpriteSpeed_t speed)
{
	int multiplier = 0x00;
	int dy = 0x00;

	///////////////////////////////////////
	//set the multiplier based on the speed
	switch(speed)
	{
		case SPRITE_SPEED_STOP: 	multiplier = 0;		break;
		case SPRITE_SPEED_SLOW: 	multiplier = 1;		break;
		case SPRITE_SPEED_MEDIUM: 	multiplier = 2;		break;
		default:					multiplier = 1;		break;
	}

	//////////////////////////////////////
	//set the dx from direction
	switch(direction)
	{
		case SPRITE_DIRECTION_0:	dy =  0 * multiplier;	break;
		case SPRITE_DIRECTION_26:	dy =  -1 * multiplier;	break;
		case SPRITE_DIRECTION_45:	dy =  -2 * multiplier;	break;
		case SPRITE_DIRECTION_63:	dy =  -2 * multiplier;	break;
		case SPRITE_DIRECTION_90:	dy =  -2 * multiplier;	break;
		case SPRITE_DIRECTION_116:	dy =  -2 * multiplier;	break;
		case SPRITE_DIRECTION_135:	dy =  -2 * multiplier;	break;
		case SPRITE_DIRECTION_153:	dy =  -1 * multiplier;	break;
		case SPRITE_DIRECTION_180:	dy =  0 * multiplier;	break;
		case SPRITE_DIRECTION_206:	dy =  1 * multiplier;	break;
		case SPRITE_DIRECTION_225:	dy =  2 * multiplier;	break;
		case SPRITE_DIRECTION_243:	dy =  2 * multiplier;	break;
		case SPRITE_DIRECTION_270:	dy =  2 * multiplier;	break;
		case SPRITE_DIRECTION_296:	dy =  2 * multiplier;	break;
		case SPRITE_DIRECTION_315:	dy =  2 * multiplier;	break;
		case SPRITE_DIRECTION_333:	dy =  1 * multiplier;	break;
		default:					dy =  0 * multiplier;	break;
	}

	return dy;
}


//////////////////////////////////////////////////////////
//Get DX from Bearing.
//Used for computing movements to astroid array.
//Increment is 5 degrees and evaluated every 11 game
//cycles. ie, tan -1 (1/11) about 5 deg.
//
int Sprite_GetDX_FromBearing(Bearing_t bearing, SpriteSpeed_t speed)
{
	int multiplier = 0x00;
	int dx = 0x00;

	//static counter for tracking modulus
	static uint32_t dxCounter = 0x00;

	///////////////////////////////////////
	//set the multiplier based on the speed
	switch(speed)
	{
		case SPRITE_SPEED_STOP: 	multiplier = 0;		break;
		case SPRITE_SPEED_SLOW: 	multiplier = 1;		break;
		case SPRITE_SPEED_MEDIUM: 	multiplier = 2;		break;
		default:					multiplier = 1;		break;
	}

	//////////////////////////////////////
	//set the dx from bearing
	switch(bearing)
	{
		case BEARING_170:	dx =  -2 * multiplier;	break;
		case BEARING_175:	dx =  -1 * multiplier;	break;
		case BEARING_180:	dx =  0 * multiplier;	break;
		case BEARING_185:	dx =  1 * multiplier;	break;
		case BEARING_190:	dx =  2 * multiplier;	break;
		case BEARING_350:	dx =  -2 * multiplier;	break;
		case BEARING_355:	dx =  -1 * multiplier;	break;
		case BEARING_0:		dx =  0 * multiplier;	break;
		case BEARING_5:		dx =  1 * multiplier;	break;
		case BEARING_10:	dx =  2 * multiplier;	break;
		default:			dx =  0 * multiplier;	break;
	}


	//only return values on %11 to get 5 deg increments
	if ((dxCounter % 11) > 0)
		dx = 0;

	dxCounter++;

	return dx;
}


//////////////////////////////////////////////////////////
//Get DY from Bearing.
//Used for computing movements to astroid array.
//Increment is 5 degrees.  Restricted  to vertically
//oriented movements.  y direction is therefore always
//one pixel in the general direction.  x direction evaluated
//every 11 cycles.
//
int Sprite_GetDY_FromBearing(Bearing_t bearing, SpriteSpeed_t speed)
{
	int multiplier = 0x00;
	int dy = 0x00;

	///////////////////////////////////////
	//set the multiplier based on the speed
	switch(speed)
	{
		case SPRITE_SPEED_STOP: 	multiplier = 0;		break;
		case SPRITE_SPEED_SLOW: 	multiplier = 1;		break;
		case SPRITE_SPEED_MEDIUM: 	multiplier = 2;		break;
		default:					multiplier = 1;		break;
	}

	//////////////////////////////////////
	//set the dy from bearing
	switch(bearing)
	{
		case BEARING_170:	dy =  1 * multiplier;	break;
		case BEARING_175:	dy =  1 * multiplier;	break;
		case BEARING_180:	dy =  1 * multiplier;	break;
		case BEARING_185:	dy =  1 * multiplier;	break;
		case BEARING_190:	dy =  1 * multiplier;	break;

		case BEARING_350:	dy =  -1 * multiplier;	break;
		case BEARING_355:	dy =  -1 * multiplier;	break;
		case BEARING_0:		dy =  -1 * multiplier;	break;
		case BEARING_5:		dy =  -1 * multiplier;	break;
		case BEARING_10:	dy =  -1 * multiplier;	break;
		default:			dy =  0 * multiplier;	break;
	}

	return dy;
}



//////////////////////////////////////////////
//Rotate player CW and set the image pointer
//Direction - 0 - Pointing Left.  90 - Pointing up
//
void Sprite_PlayerRotateCW(void)
{
	SpriteDirection_t current = Sprite_PlayerGetRotation();
	SpriteDirection_t next = current;

	switch(current)
	{
		case SPRITE_DIRECTION_0:	next = SPRITE_DIRECTION_333;	break;
		case SPRITE_DIRECTION_26:	next = SPRITE_DIRECTION_0;		break;
		case SPRITE_DIRECTION_45:	next = SPRITE_DIRECTION_26;		break;
		case SPRITE_DIRECTION_63:	next = SPRITE_DIRECTION_45;		break;
		case SPRITE_DIRECTION_90:	next = SPRITE_DIRECTION_63;		break;
		case SPRITE_DIRECTION_116:	next = SPRITE_DIRECTION_90;		break;
		case SPRITE_DIRECTION_135:	next = SPRITE_DIRECTION_116;	break;
		case SPRITE_DIRECTION_153:	next = SPRITE_DIRECTION_135;	break;
		case SPRITE_DIRECTION_180:	next = SPRITE_DIRECTION_153;	break;
		case SPRITE_DIRECTION_206:	next = SPRITE_DIRECTION_180;	break;
		case SPRITE_DIRECTION_225:	next = SPRITE_DIRECTION_206;	break;
		case SPRITE_DIRECTION_243:	next = SPRITE_DIRECTION_225;	break;
		case SPRITE_DIRECTION_270:	next = SPRITE_DIRECTION_243;	break;
		case SPRITE_DIRECTION_296:	next = SPRITE_DIRECTION_270;	break;
		case SPRITE_DIRECTION_315:	next = SPRITE_DIRECTION_296;	break;
		case SPRITE_DIRECTION_333:	next = SPRITE_DIRECTION_315;	break;
		default:					next = SPRITE_DIRECTION_90;		break;
	}

	Sprite_PlayerSetRotation(next);
}



/////////////////////////////////////////////
//Rotate player CCW and set the image pointer
//Direction - 0 - Pointing Left.  90 - Pointing up
//
void Sprite_PlayerRotateCCW(void)
{
	SpriteDirection_t current = Sprite_PlayerGetRotation();
	SpriteDirection_t next = current;

	switch(current)
	{
		case SPRITE_DIRECTION_0:	next = SPRITE_DIRECTION_26;		break;
		case SPRITE_DIRECTION_26:	next = SPRITE_DIRECTION_45;		break;
		case SPRITE_DIRECTION_45:	next = SPRITE_DIRECTION_63;		break;
		case SPRITE_DIRECTION_63:	next = SPRITE_DIRECTION_90;		break;
		case SPRITE_DIRECTION_90:	next = SPRITE_DIRECTION_116;	break;
		case SPRITE_DIRECTION_116:	next = SPRITE_DIRECTION_135;	break;
		case SPRITE_DIRECTION_135:	next = SPRITE_DIRECTION_153;	break;
		case SPRITE_DIRECTION_153:	next = SPRITE_DIRECTION_180;	break;
		case SPRITE_DIRECTION_180:	next = SPRITE_DIRECTION_206;	break;
		case SPRITE_DIRECTION_206:	next = SPRITE_DIRECTION_225;	break;
		case SPRITE_DIRECTION_225:	next = SPRITE_DIRECTION_243;	break;
		case SPRITE_DIRECTION_243:	next = SPRITE_DIRECTION_270;	break;
		case SPRITE_DIRECTION_270:	next = SPRITE_DIRECTION_296;	break;
		case SPRITE_DIRECTION_296:	next = SPRITE_DIRECTION_315;	break;
		case SPRITE_DIRECTION_315:	next = SPRITE_DIRECTION_333;	break;
		case SPRITE_DIRECTION_333:	next = SPRITE_DIRECTION_0;		break;
		default:					next = SPRITE_DIRECTION_90;		break;
	}

	Sprite_PlayerSetRotation(next);
}


/////////////////////////////////////////////////
//Sprite_PlayerSetRotation
//Set rotation and update the image pointer.  Rotation
//refers to the orientation of the ship and affects
//firing and direction changes when thrusters are on.
//
void Sprite_PlayerSetRotation(SpriteDirection_t rotation)
{
	ImageData* ptr = (ImageData*)&bmimgPlayer0Bmp;
	mPlayer.rotation = rotation;

	//set the image pointer
	switch(rotation)
	{
		case SPRITE_DIRECTION_0:	ptr = (ImageData*)&bmimgPlayer0Bmp; 	break;
		case SPRITE_DIRECTION_26:	ptr = (ImageData*)&bmimgPlayer26Bmp; 	break;
		case SPRITE_DIRECTION_45:	ptr = (ImageData*)&bmimgPlayer45Bmp; 	break;
		case SPRITE_DIRECTION_63:	ptr = (ImageData*)&bmimgPlayer63Bmp; 	break;
		case SPRITE_DIRECTION_90:	ptr = (ImageData*)&bmimgPlayer90Bmp; 	break;
		case SPRITE_DIRECTION_116:	ptr = (ImageData*)&bmimgPlayer116Bmp; 	break;
		case SPRITE_DIRECTION_135:	ptr = (ImageData*)&bmimgPlayer135Bmp; 	break;
		case SPRITE_DIRECTION_153:	ptr = (ImageData*)&bmimgPlayer153Bmp; 	break;
		case SPRITE_DIRECTION_180:	ptr = (ImageData*)&bmimgPlayer180Bmp; 	break;
		case SPRITE_DIRECTION_206:	ptr = (ImageData*)&bmimgPlayer206Bmp; 	break;
		case SPRITE_DIRECTION_225:	ptr = (ImageData*)&bmimgPlayer225Bmp; 	break;
		case SPRITE_DIRECTION_243:	ptr = (ImageData*)&bmimgPlayer243Bmp; 	break;
		case SPRITE_DIRECTION_270:	ptr = (ImageData*)&bmimgPlayer270Bmp; 	break;
		case SPRITE_DIRECTION_296:	ptr = (ImageData*)&bmimgPlayer296Bmp; 	break;
		case SPRITE_DIRECTION_315:	ptr = (ImageData*)&bmimgPlayer315Bmp; 	break;
		case SPRITE_DIRECTION_333:	ptr = (ImageData*)&bmimgPlayer333Bmp; 	break;
		default:					ptr = (ImageData*)&bmimgPlayer0Bmp; 	break;
	}

	//image data
	mPlayer.image = ptr;			//image data
	mPlayer.sizeX = ptr->xSize;		//sizeX
	mPlayer.sizeY = ptr->ySize;		//sizeY
}


SpriteDirection_t Sprite_PlayerGetRotation(void)
{
	return mPlayer.rotation;
}



///////////////////////////////////////////////////////
//Flips the player ship 180 deg
void Sprite_PlayerFlipRotation(void)
{
	SpriteDirection_t current = Sprite_PlayerGetRotation();
	SpriteDirection_t next = current;

	switch(current)
	{
		case SPRITE_DIRECTION_0:	next = SPRITE_DIRECTION_180;	break;
		case SPRITE_DIRECTION_26:	next = SPRITE_DIRECTION_206;	break;
		case SPRITE_DIRECTION_45:	next = SPRITE_DIRECTION_225;	break;
		case SPRITE_DIRECTION_63:	next = SPRITE_DIRECTION_243;	break;
		case SPRITE_DIRECTION_90:	next = SPRITE_DIRECTION_270;	break;
		case SPRITE_DIRECTION_116:	next = SPRITE_DIRECTION_296;	break;
		case SPRITE_DIRECTION_135:	next = SPRITE_DIRECTION_315;	break;
		case SPRITE_DIRECTION_153:	next = SPRITE_DIRECTION_333;	break;
		case SPRITE_DIRECTION_180:	next = SPRITE_DIRECTION_0;		break;
		case SPRITE_DIRECTION_206:	next = SPRITE_DIRECTION_26;		break;
		case SPRITE_DIRECTION_225:	next = SPRITE_DIRECTION_45;		break;
		case SPRITE_DIRECTION_243:	next = SPRITE_DIRECTION_63;		break;
		case SPRITE_DIRECTION_270:	next = SPRITE_DIRECTION_90;		break;
		case SPRITE_DIRECTION_296:	next = SPRITE_DIRECTION_116;	break;
		case SPRITE_DIRECTION_315:	next = SPRITE_DIRECTION_135;	break;
		case SPRITE_DIRECTION_333:	next = SPRITE_DIRECTION_153;	break;
		default:					next = SPRITE_DIRECTION_90;		break;
	}

	Sprite_PlayerSetRotation(next);


}

/////////////////////////////////////////
//Player Fire Thruster.
//Increases player speed.  For now, only one
//speed is implemented.  Sets a gametick
//timeout on player move.  This is stored in the
//player move function.  Sets a thruster
//on flag that is evaluated in the main loop
//
void Sprite_PlayerFireThruster(void)
{
	static uint8_t thrustCount = SPRITE_ACCELERATION_TIMEOUT_VALUE;

	//play sound
    Sound_playFireThruster();

	//enable thrusters and reset the timeout
	//timeout counts down in player move function
	//velocity decreases when thrustTimeout = 0
	//
	mPlayer.thrusterOn = 1;
	mPlayer.thrustTimeout = SPRITE_THRUSTER_TIMEOUT_VALUE;

	//TODO: Make this more detailed, allowing ship
	//to slide in one direction with thrusters an other
	//with direction set as average of the two.
	//This is a simplification for now.

    //TODO: NOTE: Keep the speed max at slow speed for now!!
    
	if (mPlayer.speed == SPRITE_SPEED_STOP)
	{
		mPlayer.speed = SPRITE_SPEED_SLOW;
		thrustCount = SPRITE_ACCELERATION_TIMEOUT_VALUE;
	}
	else if (mPlayer.speed == SPRITE_SPEED_SLOW)
	{
		//hold slow for thruster timeout value
		if (thrustCount > 0)
			thrustCount--;
		else
		{
			thrustCount = SPRITE_ACCELERATION_TIMEOUT_VALUE;
			mPlayer.speed = SPRITE_SPEED_SLOW;
		}

	}

	//set the direction based on the current rotation.
	Sprite_PlayerSetDirection(mPlayer.rotation);

}

void Sprite_PlayerSetDirection(SpriteDirection_t direction)
{
	mPlayer.direction = direction;
}

SpriteDirection_t Sprite_PlayerGetDirection(void)
{
	return mPlayer.direction;
}



///////////////////////////////////////
//Set Thruster Flag
//Called from ADC Conversion Complete callback
//on a joystick event.  This flag is polled
//in the main loop.
void Sprite_PlayerSetThursterFlag(void)
{
	mPlayerThrustFlag = 1;
}

//////////////////////////////////////////
//Get Thruster Flag
//Polled in the main loop
uint8_t Sprite_PlayerGetThrusterFlag(void)
{
	return mPlayerThrustFlag;
}

//////////////////////////////////////////
//Clear Thruster Flag
//Cleared in the main loop
void Sprite_PlayerClearThrusterFlag(void)
{
	mPlayerThrustFlag = 0;
}

uint8_t Sprite_GetPlayerHitFlag(void)
{
    return mPlayerHitFlag;
}

void Sprite_ClearPlayerHitFlag(void)
{
    mPlayerHitFlag = 0;
}

uint8_t Sprite_GetAsteroidHitFlag(void)
{
    return mAsteroidHitFlag;
}

void Sprite_ClearAsteroidHitFlag(void)
{
    mAsteroidHitFlag = 0;
}

uint8_t Sprite_GetLevelUpFlag(void)
{
    return mLevelUpFlag;
}

void Sprite_ClearLevelUpFlag(void)
{
    mLevelUpFlag = 0;
}


////////////////////////////////////////////
//
void Sprite_ClearGameOverFlag(void)
{
	mGameOverFlag = 0x00;
}

void Sprite_SetGameOverFlag(void)
{
	mGameOverFlag = 1;
}

uint8_t Sprite_GetGameOverFlag(void)
{
	return mGameOverFlag;
}

//////////////////////////////////////////////
//Get sprite speed from game level
//intended for moving astroids faster
//as the game progresses.
//
SpriteSpeed_t Sprite_GetGameSpeedFromLevel(void)
{
	if(mGameLevel <= 2)
		return SPRITE_SPEED_SLOW;
	else if ((mGameLevel > 2) && (mGameLevel <= 4))
		return SPRITE_SPEED_MEDIUM;
	else
		return SPRITE_SPEED_MEDIUM;
}


uint32_t Sprite_GetGameScore(void)
{
	return mGameScore;

}


uint8_t Sprite_GetGameLevel(void)
{
	return mGameLevel;
}


/////////////////////////////////////////////
//Read high score from EEPROM
uint16_t Sprite_GetGameHighScore(void)
{
    mGameHighScore = EEPROM_getHighScore();
    return mGameHighScore;
}

////////////////////////////////////////////////
//Set the game high score, write value to eeprom
void Sprite_SetGameHighScore(uint16_t score)
{
    mGameHighScore = score;
    EEPROM_updateHighScore(score);
}



