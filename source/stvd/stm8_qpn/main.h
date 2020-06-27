/*
 * main.h
 *
 * This file contains all the signals for all active
 * objects in the "main" system.  it also contains all
 * the external structs for all AOs and all
 * constructor funtion prototypes.  See main.c
 * for the control blocks for all AOs and thier
 * priorities
 *
 */

#ifndef MAIN_H_
#define MAIN_H_


///////////////////////////////////
//System signals for all AOs
//Q_USER_SIG is the first signal in system
//signals.  There are 7 predefined signals
//before this one defined in qepn.h

enum SystemSignals{
	BUTTON_PRESS_USER_SIG = Q_USER_SIG,     //init at Q_USER_SIG - first signal
    BUTTON_PRESS_CENTER_SIG,
    BUTTON_PRESS_UP_SIG,
    BUTTON_PRESS_DOWN_SIG,
    BUTTON_PRESS_LEFT_SIG,
    BUTTON_PRESS_RIGHT_SIG,

	MAX_PUB_SIG,					//I don't know if we need this
	MAX_SIG
};

///////////////////////////////////
//Active objects

extern struct BlinkyTag  AO_Blinky;
extern struct ButtonTag	AO_Button;

//prototypes for AO constructor functions
void Blinky_ctor(void);
void Button_ctor(void);





#endif /* MAIN_H_ */
