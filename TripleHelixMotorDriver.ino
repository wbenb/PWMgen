/*********************************************************************

  PWM Driver - A utility program that generates a PWM signal for the purpose of sending 
  test inputs to a motor controller.
  
  W. Bretton Jan 2016

*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Device will generate a signal that is capable of driving either a servo or a motor controller.
#define SERVO_MODE 1
#define PWM_MODE 0

int    op_mode = PWM_MODE;
int    cnt = 0;
int    potValue;
int    servoValue;
int    motorSpeed;
int    pwmValue;
int    outPin = 3;
Servo  myDevice;

/* Uncomment this block to use hardware SPI
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
*/

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {                
  myDevice.attach(outPin);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);

  // Initialize OLED and print a welcome
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Triple");
  display.setCursor(0,32);
  display.println("Helix!");
  display.display();
  delay(3000);

  // Get the initial potentiometer value.  If it's nearly fully counter-clockwise,
  // use this as a SERVO driver. Otherwise, assume motor controller.
  potValue = analogRead(A0);
  if (potValue < 100) 
     op_mode = SERVO_MODE;
} //setup

//
//
//
//
void loop() {
  // get the raw pot value and do quick a few quick conversions
  potValue = analogRead(A0);
  motorSpeed = map(potValue, 0, 1023, -100, 100);   // for display only, show the direction and speed as %
  pwmValue = map(potValue, 0, 1023, 1000, 2000);    // PWMs want to see a duty cycle between 1000 and 2000
  servoValue = map(potValue, 0, 1023, 0, 180);      // servos want to see 0 - 180

  if (op_mode == SERVO_MODE) {
     //put out value for a servo
     myDevice.write(servoValue);           
     displayText4("Pot Value", 
               String(potValue), 
               "Servo", 
               "   " + String(servoValue));
     delay(15); 
  }
  else
  {
     //put out the value that the motor controller wants to see
     myDevice.writeMicroseconds(pwmValue); 
     displayText4("Pot Value", 
               String(potValue), 
               "Motor/PWM", 
               String(motorSpeed)+ "/" + String(pwmValue));
  }
  
  
} // main loop

void displayText4 (String lin1, String lin2, String lin3, String lin4) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  //display.setTextColor(BLACK, WHITE);
  display.setCursor(12,0);
  display.println(lin1);  
  
  display.setCursor(42,16);
  display.println(lin2);
  
  //display.setTextColor(BLACK, WHITE);
  display.setCursor(12,32);
  display.println(lin3);
  
  display.setCursor(6,48);
  display.println(lin4);
  
  display.display();
} //displayText4



