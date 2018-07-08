/****************************************************************************/
/* Nudge controller MMA8451based + Plunger Controller with ledstrip effects */
/* Tested with Leonardo                                                     */
/****************************************************************************/

/* Librairies */
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include "Joystick.h"

/* Variables definition */
#define MMA8451_DEFAULT_ADDRESS 0x1C
Adafruit_MMA8451 mma = Adafruit_MMA8451();
#define SENSIBILITY_NUDGE 100
#define SENSIBILITY_WAIT_TIME 500
#define SENSIBILITY_NUDGE_MAX_X 512
#define SENSIBILITY_NUDGE_MAX_Y 512

// Create Joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  0, 0,                  // Button Count, Hat Switch Count
  true, true, true,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

int init_x_val = 0;
int init_y_val = 0;
int last_x_val = 0;
int last_y_val = 0;
unsigned long time_x;
unsigned long time_y;
int update_x=1, update_y=1;
int PlungerVal = 0;
int analogPin = 0;

/* -------------------VOID SETUP-------------------------*/
void setup(void) {
  
  Serial.begin(9600);

/* Nudge Setup */
  if (!mma.begin()) {
      Serial.println("Couldnt start");
  while (1) ;
  }
  Serial.println("MMA8451 found!");
  mma.setRange(MMA8451_RANGE_8_G);
  // Read the 'raw' data in 14-bit counts
  mma.read();
  init_x_val = mma.x;
  last_x_val = mma.x;
  Serial.println(init_x_val);
  init_y_val = mma.y;
  last_y_val = mma.y;
  Serial.println(init_y_val);
  time_x = millis();
  time_y = millis();

/* Joystick Setup */

  Joystick.begin();
  // Set Range Values
  Joystick.setXAxisRange(-127, 127);
  Joystick.setYAxisRange(-127, 127);
  Joystick.setZAxisRange(0, 1023);
  
}


/* -------------------VOID LOOP-------------------------*/

void loop() {
  // Read the 'raw' data in 14-bit counts
  mma.read();
  int x = mma.x;
  int y = mma.y;
  int xm, ym;
  unsigned long current_time = millis();

/* X value */
if ((current_time>time_x) && (abs(init_x_val-x)>SENSIBILITY_NUDGE)) {
update_x=1;
if(abs(x) > abs(last_x_val) && (current_time+SENSIBILITY_WAIT_TIME > time_x)) {
xm = map(constrain(x-init_x_val, -SENSIBILITY_NUDGE_MAX_X,SENSIBILITY_NUDGE_MAX_X),-SENSIBILITY_NUDGE_MAX_X,SENSIBILITY_NUDGE_MAX_X,-127,127);
//Serial.print("X : "); Serial.println(xm);
Joystick.setXAxis(xm);
last_x_val = x;
time_x = current_time;
}else{
last_x_val = init_x_val;
time_x = current_time + SENSIBILITY_WAIT_TIME;
//Serial.print("X : "); Serial.println(0);
Joystick.setXAxis(0);
}
}else{
if(update_x) {
//Serial.print("X : "); Serial.println(0);
Joystick.setXAxis(0);
update_x=0;
}
}

/* Y value */
if ((current_time>time_y) && (abs(init_y_val-y)>SENSIBILITY_NUDGE)) {
update_y=1;
if(abs(y) > abs(last_y_val) && (current_time+SENSIBILITY_WAIT_TIME > time_y)) {
ym = map(constrain(y-init_y_val, -SENSIBILITY_NUDGE_MAX_Y,SENSIBILITY_NUDGE_MAX_Y),-SENSIBILITY_NUDGE_MAX_Y,SENSIBILITY_NUDGE_MAX_Y,-127,127);
//Serial.print("Y : "); Serial.println(ym);
Joystick.setYAxis(ym);
last_y_val = y;
time_y = current_time;
}else{
last_y_val = init_y_val;
time_y = current_time + SENSIBILITY_WAIT_TIME;
//Serial.print("Y : "); Serial.println(0);
Joystick.setYAxis(0);
}
}else{
if(update_y) {
//Serial.print("Y : "); Serial.println(0);
Joystick.setYAxis(0);
update_y=0;
}
}

/* Z value */
PlungerVal = analogRead(analogPin);
Joystick.setZAxis(PlungerVal);


delay(50);
}
