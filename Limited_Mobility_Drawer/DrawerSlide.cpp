// 
// 
// 

#include "DrawerSlide.h"
DrawerSlideClass::DrawerSlideClass(int num, int i, int j, int k, int l, int m)
{
    drawerNum = num;  //Unique identifier for each drawer
		openPin = i;      //Pin pulled high to open
		closePin = j;     //Pin pulled high to close
		hallPin = k;      //Analog hall effect sensor pin
		pressPin = l;     //Analog pin connected to pressure sensor
		buttonPin = m;    //Pin connected to button
		POS = Closed;     //Enumerated Status
    readyToActivate = 1;

//********HALL EFFECT SENSOR********//
   openThresh = 499;      //Tune these values for
   closedThresh = 520;    //desired end-stop behavior
   hallVal = 0; 
   openTrig = 0;
   closedTrig = 0;   

   for(int i = 0; i < 10; i++){
      hallArray[i] = 0;
   }
   hallArrayCur = 0;
   hallArrayAvg = 0;
   hallArrayTemp = 0;
   hallArrayAvgLast = 0;
   hallTrig = 0;
    
//********PRESSURE SENSOR********//
    pressTrig = 0;
    pressThresh = -0.18;          //tune this value for desired
    for(int i = 0; i < 75; i++){  //pressure sensor behavior
      pressArray[i] = 0;
    }
    pressArrayCur = 0;
    pressArrayAvg = 0;
    pressArrayTemp = 0;
    pressArrayAvgLast = 0;
    pressArrayAvgDeriv = 0;
    pressHold = 0;
    pressHoldTime = 0;
    
    pinMode(openPin, OUTPUT);
    pinMode(closePin, OUTPUT);
    //pinMode(hallPin, INPUT);
    //pinMode(pressPin, INPUT);
    pinMode(buttonPin, INPUT);
    digitalWrite(buttonPin, HIGH);

}
int DrawerSlideClass::isActivated(){
	if(digitalRead(buttonPin) == 0){
    if(readyToActivate == 1){
      readyToActivate = 0;
      Serial.print("\tActivate");
      Serial.println(drawerNum);
		  return 1;
    }
    else{
      return 0;
    }
	}
  else{
    readyToActivate = 1;
	  return 0;
  }
}


//********HALL EFFECT SENSOR********//
int DrawerSlideClass::isNearClosed(){
  changeHallArray();
  if(hallVal > closedThresh){
    return 1;
  } 
  return 0;
}
int DrawerSlideClass::isNearOpened(){
  changeHallArray();
	if(hallVal < openThresh){
    return 1;
  } 
  return 0;
}
void DrawerSlideClass::changeHallArray(){
  hallVal = ((0.9*hallVal) + (0.1*analogRead(hallPin)));
  /*hallArrayTemp = hallArray[hallArrayCur];
  hallArray[hallArrayCur] = float(analogRead(hallPin));
  hallVal = hallArray[hallArrayCur];
  hallArrayAvg = (hallArrayAvg + ((hallArray[hallArrayCur] - hallArrayTemp) / 10));
  if(hallArrayCur >=9){
    hallArrayCur = 0;
  }
  else{
    hallArrayCur++;
  }*/
}

//********PRESSURE SENSOR********// 
int DrawerSlideClass::isPressed(){
  pressArrayTemp = pressArray[pressArrayCur];
  pressArray[pressArrayCur] = float(analogRead(pressPin));
  pressArrayAvgLast = pressArrayAvg;
  pressArrayAvg = (pressArrayAvg + ((pressArray[pressArrayCur] - pressArrayTemp) / 75));
  pressArrayAvgDeriv = ((0.95*pressArrayAvgDeriv) + 
                        (0.05*(pressArrayAvg - pressArrayAvgLast)));
  if(pressArrayCur >=74){
    pressArrayCur = 0;
  }
  else{
    pressArrayCur++;
  }
	if(pressArrayAvgDeriv < pressThresh){
    if(pressHold == 0 && (millis()-pressHoldTime) > 300){
      pressHold = 1;
      pressHoldTime = millis();
		  return 1;
    }
    else{
      //Serial.print("Hold"); Serial.println(drawerNum);
      return 0;
    }
	}
  else{
    if(pressHold == 1){
      //Serial.print("~THold"); Serial.println(drawerNum);
    }
    pressHold = 0;
	  return 0;
  }  
}
void DrawerSlideClass::extend(int vel){
	analogWrite(closePin, 0);
	analogWrite(openPin, vel);
  POS = Opening;
  Serial.print("Opening"); Serial.println(drawerNum);
}
void DrawerSlideClass::retract(int vel){
	analogWrite(openPin, 0);
	analogWrite(closePin, vel);
  POS = Closing;
  Serial.print("Closing"); Serial.println(drawerNum);
}
void DrawerSlideClass::stopMoving(){
  analogWrite(openPin, 0);
  analogWrite(closePin, 0);
  Serial.print("\tSTOP"); Serial.println(drawerNum);
}


//********UPDATE DRAWER********//
void DrawerSlideClass::updateDrawer(){
  pressTrig = isPressed();
  changeHallArray();
  /*if(pressTrig == 1){
    //  Serial.print("Trig"); Serial.print(drawerNum);
    //Serial.print(":\t"); Serial.println(pressArrayAvgDeriv);
  }
  if(openTrig == 1){
    stopMoving();
  }x
  if(closedTrig == 1){
    stopMoving();
  }*/
	if(POS == Closing){
		if(isActivated() == 1)
			extend(myVel);
		if(isNearClosed() == 1){
      stopMoving();
			POS = Closed;
      Serial.print("Closed"); Serial.print(drawerNum);
      Serial.print(": "); Serial.println(hallVal);
		}
		else if(pressTrig == 1){
      Serial.print("\tTrigC"); Serial.print(drawerNum);
      Serial.print(":\t"); Serial.println(pressArrayAvgDeriv);
			extend(myVel);
		}
	}
	else if(POS == Opening){
		if(isActivated() == 1)
			retract(myVel);
		if(isNearOpened() == 1){
			POS = Opened;
      stopMoving();
      Serial.print("Opened"); Serial.print(drawerNum);
      Serial.print(": "); Serial.println(hallVal);
		}
	}
	else if(POS == Closed){
		if(isActivated() == 1)
			extend(myVel);
	}
	else{
		if(isActivated() == 1)
			retract(myVel);
	}
  
}
