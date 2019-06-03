/*This sketch along with accompanying library is meant for an automated drawer
 * for users with limited mobility. The dresser used has three drawers which 
 * can be activated by a button. 
 * A hall-effect sensor is used near each to determine when a drawer has reached 
 * the end of its travel. 
 * Pieces of static-dissipative foam with aluminum tape on either side are used 
 * as a safety feature to sense an obstacle getting caught in a drawer. These 
 * sensors can be cut to any size and shape.
 * 
 * Explanations of ech pin are described in DrawerSlide.cpp
 * 
 * 
 * 
 */
#include "DrawerSlide.h"
int timeElapsed = 0;
unsigned long timeLast = 0;
unsigned long timeCur = 0;
int timeThresh = 300;
unsigned int thisVel = 254;

DrawerSlideClass drawer1(1, 10, 9, 1, 0, 2);
DrawerSlideClass drawer2(2, 11, 3, 3, 2, 8);
DrawerSlideClass drawer3(3, 6, 5, 5, 4, 7);
void setup() {
  Serial.begin(9600);
  Serial.println("Setting Up");
  drawer1.myVel = thisVel;
  drawer2.myVel = thisVel;
  drawer3.myVel = thisVel;
}
void loop() {
  /*
   * updateDrawer() handles reading 
   * all inputs and changing all 
   * outputs for a given drawer.
   */
  drawer1.updateDrawer(); 
  drawer2.updateDrawer();
  drawer3.updateDrawer();

 //DEBUGGING
  timeCur = millis();
  timeElapsed = timeCur - timeLast;
  if(timeElapsed > timeThresh){
    timeLast = timeCur;
   // Serial.println(drawer1.pressArrayAvgDeriv);
   //Serial.println(drawer1.hallVal);
  }
}
