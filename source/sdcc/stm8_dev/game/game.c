/*
 * 2023 - Game with scrolling screen
 * Player ship flies over random generated ground
 * surface, shooting at enemies.

*/

#include <stdint.h>
#include <stdlib.h>

#include "register.h"
#include "timer.h"
#include "gpio.h"
#include "game.h"
#include "lcd.h"
#include "sound.h"
#include "eeprom.h"

//globals for game
static uint8_t groundSurfaceHead = 0;

static PlayerStruct playerShip;
static MissileStruct missileArray[GAME_NUM_MISSILE];
static EnemyStruct enemyArray[GAME_NUM_ENEMY];

uint8_t groundSurface[GAME_GROUND_LENGTH];

uint8_t scoreBuffer[SCORE_BUFFER_SIZE] = {0x00};

uint16_t gameScore = 0;
uint16_t gameHighScore = 0;

/////////////////////////////////////////
//initialize the game
void game_init(void)
{
    game_playerInit();
    game_groundInit();
    game_missileInit();
    game_enemyInit();

    game_scoreInit();

    lcd_clearFrameBuffer(0x00, 0);
    game_drawGround(0);
    game_drawPlayer(0);
    lcd_updateDisplay();
    game_drawScore();
}

//////////////////////////////////////////
//Display the start screen, high score, etc
void game_displayStartup(void)
{
    uint8_t i, length;

    for (i = 0 ; i < SCORE_BUFFER_SIZE ; i++)
        scoreBuffer[i] = 0x00;

    length = lcd_decimalToBuffer(gameHighScore, scoreBuffer, SCORE_BUFFER_SIZE);

    lcd_clear(0x00);
    lcd_drawString(1, 30, "Press");
    lcd_drawString(2, 25, "Button");
    lcd_drawString(3, 20, "to Start");

    lcd_drawString(5, 15, "High Score");
    lcd_drawStringLength(6, 40, scoreBuffer, length);
}


//////////////////////////////////////
//display game over
void game_displayGameOver(void)
{
    uint8_t length, i;
    for (i = 0 ; i < SCORE_BUFFER_SIZE ; i++)
        scoreBuffer[i] = 0x00;

    length = lcd_decimalToBuffer(gameHighScore, scoreBuffer, SCORE_BUFFER_SIZE);

    lcd_clear(0x00);
    lcd_drawString(0, 15, "Game Over");
    lcd_drawString(1, 30, "Press");
    lcd_drawString(2, 25, "Button");
    lcd_drawString(3, 20, "To Start");

    lcd_drawString(5, 15, "High Score");
    lcd_drawStringLength(6, 40, scoreBuffer, length);
}



void game_playerInit(void)
{
    //initialize the player ship
    playerShip.numLives = 3;
    playerShip.x = GAME_PLAYER_DEFAULT_X;
    playerShip.y = GAME_PLAYER_DEFAULT_Y;
    playerShip.image = &imgPlayerShip;
}

/////////////////////////////////////////
//setup the initial ground surface array
//the tail of the array is updated with
//each game loop
void game_groundInit(void)
{
    uint8_t i;

    //initialize the ground surface array
    groundSurfaceHead = (GAME_GROUND_MAX + GAME_GROUND_MIN) / 2;

    //init the ground surface at midpoint of max and min
    for (i = 0 ; i < GAME_GROUND_LENGTH ; i++)
        groundSurface[i] = groundSurfaceHead;
}


////////////////////////////////////////////////
//init the missile array
//static MissileStruct missileArray[GAME_NUM_MISSILE];
void game_missileInit(void)
{
    uint8_t i;
    
    for (i = 0 ; i < GAME_NUM_MISSILE ; i++)
    {
        missileArray[i].alive = 0;
        missileArray[i].x = 0;
        missileArray[i].y = 0;
    }
}


///////////////////////////////////////////
//enemy init - initialize the enemy array
void game_enemyInit(void)
{
    uint8_t i;
    
    for (i = 0 ; i < GAME_NUM_ENEMY ; i++)
    {
        enemyArray[i].alive = 0;
        enemyArray[i].x = 0;
        enemyArray[i].y = 0;
        enemyArray[i].points = 0;
        enemyArray[i].image = &imgEnemy1;
    }
}


//////////////////////////////////////////////
//resets the current game score, calls the
//high score set in eeprom and stores to
//gameHighScore.  scoreBuffer is used to draw score
//across the top of the display during the game
void game_scoreInit(void)
{
    uint8_t i;
    gameScore = 0;

    for (i = 0 ; i < SCORE_BUFFER_SIZE ; i++)
        scoreBuffer[i] = 0x00;

    gameHighScore = EEPROM_getHighScore();
}

/////////////////////////////////
//returns the current game score
uint16_t game_getScore(void)
{
    return gameScore;
}

/////////////////////////////////////////
//read the high score from eeprom, set the
//game high score variable, and return the value
uint16_t game_getHighScore(void)
{
	uint16_t score = EEPROM_getHighScore();
	gameHighScore = score;
	return score;
}

///////////////////////////////////////////
//set the game high score in eeprom memory
void game_setHighScore(uint16_t score)
{
	gameHighScore = score;
	EEPROM_updateHighScore(score);
}


////////////////////////////////////////////
//draw the score, num players, etc on the
//top of the screen, page 0.  draws directly
//to the LCD, not in the ram buffer.
void game_drawScore(void)
{
    uint8_t length, i;
    const ImageDataVertical *image = &bmimgPlayerIcon_1;

    lcd_drawString(0, 0, "S:");

    //clear the buffer
    for (i = 0 ; i < SCORE_BUFFER_SIZE ; i++)
        scoreBuffer[i] = 0x00;

    length = lcd_decimalToBuffer(gameScore, (char*)scoreBuffer, SCORE_BUFFER_SIZE);
    lcd_drawStringLength(0, 20, (char*)scoreBuffer, length);

    switch(playerShip.numLives)
    {

        case 1:     image = &bmimgPlayerIcon_1;     break;
        case 2:     image = &bmimgPlayerIcon_2;     break;
        case 3:     image = &bmimgPlayerIcon_3;     break;
        default:    image = &bmimgPlayerIcon_3;     break;
    }

    lcd_drawString(0, 60, "P:");
    lcd_drawIconPage(0, 80, image);
}


////////////////////////////////////////////////
// returns the number of players remaining
uint8_t game_getNumPlayer(void)
{
    return playerShip.numLives;
}


/////////////////////////////////////////////
//sets the number of players
void game_setNumPlayer(uint8_t num)
{
    if (num <= GAME_MAX_LIVES)
        playerShip.numLives = num;
}


/////////////////////////////////////////////////
//removes 1 player and returns the number of
//players remaining
uint8_t game_killPlayer(void)
{
    if (playerShip.numLives >= 1)
        playerShip.numLives--;

    return playerShip.numLives;
}

///////////////////////////////////////////
//reset the player position to default
//called after crashing into ground, enemy, etc
void game_playerReset(void)
{
    playerShip.x = GAME_PLAYER_DEFAULT_X;
    playerShip.y = GAME_PLAYER_DEFAULT_Y;
}

///////////////////////////////////////////////////
//game_updateGround()
//updates the ground surface assuming the player ship
//is flying left to right.  updates the ground surface
//array with a new head pixel based on random number.
//
void game_updateGround(void)
{
    uint8_t current = groundSurfaceHead;
    uint8_t next = 0;
    uint8_t i, temp;

    //rand() % (max_number + 1 - minimum_number) + minimum_number
    //generate number from 0-2
    temp = rand() % (2 + 1 - 0) + 0;
    
    switch(temp)
    {
        case 0:     next = current - 1;     break;
        case 1:     next = current;         break;
        case 2:     next = current + 1;     break;
    }

    if ((next <= GAME_GROUND_MAX) && (next >= GAME_GROUND_MIN))
        groundSurfaceHead = next;

    //update the ground surface array with last element = head
    for (i = 0 ; i < GAME_GROUND_LENGTH - 2 ; i++)
    {
        groundSurface[i] = groundSurface[i+1];
        groundSurface[i+1] = groundSurface[i+2];
    }

    groundSurface[GAME_GROUND_LENGTH - 2] = groundSurfaceHead;
    groundSurface[GAME_GROUND_LENGTH - 1] = groundSurfaceHead;
}


////////////////////////////////////////
//Draws ground surface in the ram buffer
//uses the groundSurface array with 
//element 0 = x0, 1 = x1... etc
//values in the array are y values
void game_drawGround(uint8_t update)
{
    uint8_t i;

    for (i = 0 ; i < GAME_GROUND_LENGTH ; i++)
        lcd_putPixelRam(i, groundSurface[i], 1, 0);

    if (update == 1)
        lcd_updateDisplay();
}



///////////////////////////////////////
//Read the joystick to get the direction
Direction_t game_readJoystick(void)
{
    Direction_t direction = MOVE_NONE;

    //read the joystick and move the player
    //up - PD2
    if (!(PD_IDR & BIT_2))
        direction = MOVE_UP;
    //down - PD4
    else if (!(PD_IDR & BIT_4))
        direction = MOVE_DOWN;
    //left - PD0
    else if (!(PD_IDR & BIT_0))
        direction = MOVE_LEFT;
    //right - PD3
    else if (!(PD_IDR & BIT_3))
        direction = MOVE_RIGHT;

    return direction;
}

////////////////////////////////////////////
//read the joystick and move the player
//evaluate the location based on the ground 
//surface, LHS and RHS of the display.
//returns 0 for no collision, 1 for collision with
//ground, 2 for collision with enemy
uint8_t game_playerMove(Direction_t direction)
{
    uint8_t i;
    uint8_t yleft = groundSurface[playerShip.x];
    uint8_t yright = groundSurface[(playerShip.x) + 8];
    uint8_t shipBot = playerShip.y + 8;
    uint8_t shipTop = playerShip.y;
    uint8_t shipLeft = playerShip.x;
    uint8_t shipRight = playerShip.x + 8;
    uint8_t xleft, xright, ytop, ybot;
    uint8_t collisionFlag = 0;


    switch(direction)
    {
        case MOVE_UP:
        {
            if (playerShip.y > GAME_PLAYER_MIN_Y)
                playerShip.y-=2;
            break;
        }
    
        case MOVE_DOWN:
        {
            if (playerShip.y < GAME_PLAYER_MAX_Y)
                playerShip.y+=2;
            break;
        }

        case MOVE_LEFT:
        {
            if (playerShip.x > GAME_PLAYER_MIN_X)
                playerShip.x-=2;
            break;
        }

        case MOVE_RIGHT:
        {
            if (playerShip.x < GAME_PLAYER_MAX_X)
                playerShip.x+=2;
            break;
        }
    }


    //test for collision - collide with ground surface
    if ((yleft < shipBot) || (yright < shipBot))
    {
        collisionFlag = 1;
        return collisionFlag;
    }

    //test if it hit an enemy
    for (i = 0 ; i < GAME_NUM_ENEMY ; i++)
    {
        if (enemyArray[i].alive == 1)
        {
            xleft = enemyArray[i].x;
            xright = enemyArray[i].x + 16;
            ytop = enemyArray[i].y;
            ybot = enemyArray[i].y + 8;

            //did the ship box overlap the enemy box
            if (((shipBot > ytop) && (shipTop < ybot)) && 
                ((shipRight > xleft) && (shipLeft < xright)))
            {
                //ship hit an enemy
                enemyArray[i].alive = 0;
                enemyArray[i].x = 0;
                enemyArray[i].y = 0;
                enemyArray[i].image = &imgEnemy1;

                gameScore = gameScore + enemyArray[i].points;
                
                collisionFlag = 2;

                return collisionFlag;
            }
        }
    }

    return 0;
}



///////////////////////////////////////////
//draw player ship into ram buffer, update
//the display if update == 1
void game_drawPlayer(uint8_t update)
{
    lcd_drawIcon(playerShip.x, playerShip.y, playerShip.image, update);
}



//////////////////////////////////////////
//fire the missile from player
void game_fireMissile(void)
{
    uint8_t i;

    //get the first available missile in the array
    //with alive = 0;
    for (i = 0 ; i < GAME_NUM_MISSILE ; i++)
    {
        if (!missileArray[i].alive)
        {
            missileArray[i].alive = 1;
            missileArray[i].x = playerShip.x + 8;
            missileArray[i].y = playerShip.y + 4;

            i = GAME_NUM_MISSILE;         //exit the loop
        }        
    }
}


//////////////////////////////////////////
//move all active missiles until they hit
//the ground surface or move off the screen
void game_moveMissile(void)
{
    uint8_t i, j;
    uint8_t etop, ebot, eleft, eright, mx, my;

    for (i = 0 ; i < GAME_NUM_MISSILE ; i++)
    {
        if (missileArray[i].alive == 1)
        {
            if ((missileArray[i].x + 3) < (LCD_WIDTH - 1))
                missileArray[i].x = missileArray[i].x + 3;
            else
            {
                //kill off the missile
                missileArray[i].alive = 0;
                missileArray[i].x = 0;
                missileArray[i].y = 0;                
            }

            //for each live missile after it's moved or 
            //off the screen, evaluate if it hit an enemy
            //loop over enemy array and test if missile possition
            //is within the enemy box
            for (j = 0 ; j < GAME_NUM_ENEMY ; j++)
            {
                if (enemyArray[j].alive == 1)
                {
                    etop = enemyArray[j].y;
                    ebot = enemyArray[j].y + 8;
                    eleft = enemyArray[j].x;
                    eright = enemyArray[j].x + 8;

                    mx = missileArray[i].x;
                    my = missileArray[i].y;

                    if (((my > etop) && (my < ebot)) && ((mx > eleft) && (mx < eright)))
                    {
                        //kill of the missile
                        missileArray[i].x = 0;
                        missileArray[i].y = 0;
                        missileArray[i].alive = 0;
                        
                        //kill off the enemy
                        enemyArray[j].alive = 0;
                        enemyArray[j].x = 0;
                        enemyArray[j].y = 0;
                        enemyArray[j].image = &imgEnemy1;

                        gameScore = gameScore + enemyArray[j].points;

                        Sound_playEnemyExplode();
                    }
                }
            }
        }
    }
}




//////////////////////////////////////////////
//draw 2 pixels for each missile that is alive
void game_drawMissile(uint8_t update)
{
    uint8_t i;
    for (i = 0 ; i < GAME_NUM_MISSILE ; i++)
    {
        if (missileArray[i].alive == 1)
        {
            lcd_putPixelRam(missileArray[i].x, missileArray[i].y, 1, update);
            lcd_putPixelRam((missileArray[i].x + 1), missileArray[i].y, 1, update);
            lcd_putPixelRam(missileArray[i].x, (missileArray[i].y + 1), 1, update);
            lcd_putPixelRam((missileArray[i].x + 1), (missileArray[i].y + 1), 1, update);
        }
    }
}



////////////////////////////////////////////////
//generate enemy at random, appearing on LHS
//of the screen between top of LCD and ground
//surface.  find the first available enemy in
//the array with alive = 0.  choose enemy 1-3
//at random.  The ground surface limit is the max
//over the rhs of the screen to avoid enemies from
//flying into the ground.
void game_generateEnemy(void)
{
    uint8_t i, temp, temp2, temp4, ypos;
    static uint16_t loopCounter = 0;
    uint8_t enemy = 0;
    const ImageData* ptr = &imgEnemy1;
    uint8_t max = groundSurface[GAME_GROUND_LENGTH - 2];

    //get the highest point on the ground as baseline for
    //setting y positions for new enemy.
    for (i = 0 ; i < (GAME_GROUND_LENGTH - 2) ; i++)
    {
        if (groundSurface[i] < max)
            max = groundSurface[i];
    }

    //rand() % (max_number + 1 - minimum_number) + minimum_number
    //generate number from 0-2
    temp = rand() % (2 + 1 - 0) + 0;        //what enemy
    temp2 = rand() % (2 + 1 - 0) + 0;       
    temp4 = rand() %(3 + 1 - 0) + 0;        //1/4 pts for y position
    
    ypos = (max / 4) * temp4 + 8;

    //enemy overlaps the ground?
    if ((ypos + 8) >= groundSurface[GAME_GROUND_LENGTH - 2])
        ypos = ypos - 10;

    switch(temp)
    {
        case 0:     ptr = &imgEnemy1;      break;
        case 1:     ptr = &imgEnemy2;      break;
        case 2:     ptr = &imgEnemy3;      break;
    }

    //try to make it somewhat random but not too often.
    if ((!(loopCounter % 15)) && (temp2 == 2))
    {
        //get the first available enemy in the array
        for (i = 0 ; i < GAME_NUM_ENEMY ; i++)
        {
            if (!enemyArray[i].alive)
            {
                enemyArray[i].alive = 1;
                enemyArray[i].x = LCD_WIDTH - 8;
                enemyArray[i].y = ypos;
                enemyArray[i].points = (temp + 1) *10;
                enemyArray[i].image = ptr;
                
                i = GAME_NUM_ENEMY;
            }
        }
    }

    loopCounter++;
}

////////////////////////////////////////////////
void game_moveEnemy(void)
{
    uint8_t i;
    for (i = 0 ; i < GAME_NUM_ENEMY ; i++)
    {
        if (enemyArray[i].alive == 1)
        {
            if (enemyArray[i].x >= 2)
                enemyArray[i].x = enemyArray[i].x - 2;
            else
            {
                //kill off the enemy
                enemyArray[i].alive = 0;
                enemyArray[i].x = 0;
                enemyArray[i].y = 0;
                enemyArray[i].points = 0;
                enemyArray[i].image = &imgEnemy1;                
            }
        }
    }
}

////////////////////////////////////////////////
//draw the enemies into the frame buffer
//update the lcd if update == 1
void game_drawEnemy(uint8_t update)
{
    uint8_t i;
    for (i = 0 ; i < GAME_NUM_ENEMY ; i++)
    {
        if (enemyArray[i].alive == 1)
            lcd_drawIcon(enemyArray[i].x, enemyArray[i].y, enemyArray[i].image, update);
    }
}



