/*===========================================================================
* A simple script to drive the Node Controller mosfets
* This is a starting point if you want to write your own simple code.alignas
* If you want to add more features, check out `src/config-shield.hpp` in this repo for pin mappings
* Instructions: 
* - Select "Arduino Uno R4 Minima" in boards menu (tools/boards)
*   - Add through "boards manager" if you don't have it
* - Set the mosfet speeds in the variables below
* - Plug muscle or load into M1 or M2 port on the Node Controller
* - Press the button on the top of the controller to activate! (button close to usb)
*============================================================================*/

// These mosfets can each be driven up to 60 A
const int mosfet1 = 9;  // M1 output pin
const int mosfet2 = 6;  // M2 output pin

const int button = 7;  // Button input pin

int mosfet1_speed = 256;  // Full power on. 0=0%, 256=100%
int mosfet2_speed = 237;  // 50% power

void setup() {
  // put your setup code here, to run once:

  pinMode(mosfet1, OUTPUT);
  pinMode(mosfet2, OUTPUT);

  pinMode(button, INPUT_PULLUP);

  // Start with muscles at 0 power
  analogWrite(mosfet1, 0);
  analogWrite(mosfet2, 0);
}

// Runs repeatedly (forever)
void loop() {

  // If button is pressed, set the mosfet speeds (button logic is flipped)
  if(!digitalRead(button)) {
    analogWrite(mosfet1, mosfet1_speed);  // Change the speed at the top (or in code)
    analogWrite(mosfet2, mosfet2_speed);
  }
  else {
    analogWrite(mosfet1, 0);  // Turn off if button is not pressed
    analogWrite(mosfet2, 0);
  }
}
