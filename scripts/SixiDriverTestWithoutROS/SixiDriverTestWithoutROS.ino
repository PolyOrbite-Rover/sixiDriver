// PolyOrbite: Test to make to robot move with new SixiDriver class
// Authors: Ian Gagnon
// Date: 2021-05-07

/* Example of use -> run these commands one by one in the Serial monitor


v300,300,300,300,300,300
v700,700,700,700,700,700
v1000,1000,1000,1000,1000,1000
v2000,2000,2000,2000,2000,1000
t45
p45,-45,45,0,0,0
p0,0,0,0,0,0
p-30,20,60,150,80,90

*/
#define  BufferSize   30            //Input Buffer to incoming comunications
String InputString = "";          // a string to hold incoming data
String InputString2 = "";
String InputString3 = "";
String InputString4 = "";
String InputString5 = "";
String InputString6 = "";
String *currentInputString = &InputString;
boolean  StringComplete = false;    // Flag, whether the string is complete
float input_value=0, input_value2=0, input_value3=0, input_value4=0, input_value5=0, input_value6=0;

// Declare the new line character
const char newLineChar='\n';
int data = 0; // 0 = vitesse, 1 = position et 2=outil

#include "sixiDriver.h"

SixiDriver driver(1000, true);

void setup()
{
  Serial.begin(57600);
  driver.enableMotors();
  Serial.println("Started!");
  driver.attachTool();
 //driver.moveToHome();
}

void loop()
{
  if (StringComplete)
  {  
    input_value = (float)InputString.toInt();
    input_value2 = (float)InputString2.toInt();
    input_value3 = (float)InputString3.toInt();
    input_value4 = (float)InputString4.toInt();
    input_value5 = (float)InputString5.toInt();
    input_value6 = (float)InputString6.toInt();

    if (data == 0){        
      if (input_value > 0){
        Serial.print("Speeds are set to: "); 
        Serial.print(input_value); Serial.print(" ");
        Serial.print(input_value2); Serial.print(" ");
        Serial.print(input_value3); Serial.print(" ");
        Serial.print(input_value4); Serial.print(" ");
        Serial.print(input_value5); Serial.print(" ");
        Serial.print(input_value6); Serial.print(" ");
        Serial.println(" pulses/sec");
        float arrayForSpeedAndAccel[6] = {input_value, input_value2, input_value3, input_value4, input_value5, input_value6};
        driver.setMaxSpeedForAll(arrayForSpeedAndAccel);
        driver.setAccelerationForAll(arrayForSpeedAndAccel);
      }
      else
        Serial.println("Speed can NOT be negative!");
    }
    
    else if (data == 1){  
      Serial.print("Position: "); 
      Serial.print(input_value); Serial.print(" ");
      Serial.print(input_value2); Serial.print(" ");
      Serial.print(input_value3); Serial.print(" ");
      Serial.print(input_value4); Serial.print(" ");
      Serial.print(input_value5); Serial.print(" ");
      Serial.print(input_value6); Serial.println();
      float arrayForPosition[6] = {input_value, input_value2, input_value3, input_value4, input_value5, input_value6};
      driver.moveAllTo(arrayForPosition);   
    }

    else if (data == 2){  
      int ToolPos = (int)input_value;
      Serial.print("Position outils: "); 
      Serial.print(ToolPos); Serial.println();
      driver.moveTool(ToolPos);
    }

    //clear all after reception
    InputString = "";
    InputString2 = "";
    InputString3 = "";
    InputString4 = "";
    InputString5 = "";
    InputString6 = "";
    StringComplete = false;
  }
  driver.runAll();
}



void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it.
    if (inChar == newLineChar){
      StringComplete = true;
      currentInputString = &InputString;
    }
    // Add it to the inputString: if not null
    else if (inChar != '\0') {
      if (inChar == 'v')
          data=0;
      else if (inChar == 'p')
          data=1;
      else if (inChar == 't')
          data=2;
      else if (inChar == ','){
        if (currentInputString == &InputString) currentInputString = &InputString2;
        else if (currentInputString == &InputString2) currentInputString = &InputString3;
        else if (currentInputString == &InputString3) currentInputString = &InputString4;
        else if (currentInputString == &InputString4) currentInputString = &InputString5;
        else if (currentInputString == &InputString5) currentInputString = &InputString6;
        else currentInputString = &InputString;
      }
      else
        *currentInputString += inChar;
    }
    
    // If buffer overloaded, reset input
    if (InputString.length()>=BufferSize) {
      Serial.println("Buffer Overloaded");
      InputString = "";
    }
  }
}
