// DrawerSlide.h

#ifndef _DRAWERSLIDE_h
#define _DRAWERSLIDE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
enum position {Closed = 0, Opened, Closing, Opening};
class DrawerSlideClass
{
 public:
	 position POS;
   unsigned short drawerNum;
	 uint8_t openPin;
	 uint8_t closePin;
	 uint8_t hallPin;
	 uint8_t pressPin;
	 uint8_t buttonPin;
   unsigned int myVel;
   int readyToActivate;

//********HALL EFFECT SENSOR********//
   unsigned int openThresh;
   unsigned int closedThresh;
   unsigned int hallVal;
   int openTrig;
   int closedTrig;
   float hallArray[10];
   float hallThresh;
   int hallArrayCur;
   float hallArrayAvg;
   float hallArrayTemp;
   float hallArrayAvgLast;
   int hallTrig;
  
//********PRESSURE SENSOR********//  
   float pressArray[75];
   float pressThresh;
   int pressArrayCur;
   float pressArrayAvg;
   float pressArrayTemp;
   float pressArrayAvgLast;
   float pressArrayAvgDeriv;
   int pressHold;
   int pressTrig;
   unsigned long int pressHoldTime;

	DrawerSlideClass(int num, int i, int j, int k, int l, int m);
	int isNearClosed();
	int isNearOpened();
  void changeHallArray();
	int isPressed();
	int isActivated();
	void extend(int vel);
	void retract(int vel);
  void stopMoving();
	void updateDrawer();

};

extern DrawerSlideClass DrawerSlide;

#endif
