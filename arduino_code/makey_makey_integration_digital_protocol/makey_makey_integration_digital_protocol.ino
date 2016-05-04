/*   makey_makey_integration.ino
 Purpose: Project EduBot
 Authot: Tiago Pimentel
        t.pimentelms@gmail.com
 Projeto EduBot
 Student Chapter of IEEE - RAS Universidade de Brasília (UnB)
 LARA (Laboratório de Robótica e Automação), UnB
 Date: May, 24, 2014
 
 Invariants
   RELAY 4
   MAKEY_MAKEY_INPUT 7
   MAKEY_MAKEY_INPUT 6
 Description:
   Use a relay to turn lights on or off
   Switch the lights (on/off) if :
     Activity on the makey makey
 Version Log:
   05/24/2014, Tiago Pimentel
     Created file;
     Created relay activating and deactivating code, and physical project;
     Created makey makey detection from serial port
*/
#include "Arduino.h"
#include "stdio.h"
#include "stdlib.h"

// Debug definition.
#define DEBUG
// #define DEBUG2

// Names relay pin and sets a variable with its state.
#define LED1_PIN 4
#define INPUT_PIN_1 7
#define INPUT_PIN_2 6
int led1State = LOW;

boolean gettingMessage = false;

void setup() {
#ifdef DEBUG
  Serial.begin(9600);  // Serial for debugging.
#endif

  // set the digital pin as output:
  pinMode(LED1_PIN, OUTPUT);
  pinMode(INPUT_PIN_1, INPUT);
  digitalWrite(INPUT_PIN_1, LOW);
  pinMode(INPUT_PIN_2, INPUT);
  digitalWrite(INPUT_PIN_2, LOW);
}

void loop()
{
  static boolean oldBitMeasure = false;
  static int bitCounter = 0;
  
  boolean isCommunicating = digitalRead(INPUT_PIN_1);
  
  // If input pin 1 is high, makey_makey is communicating with arduino.
  if(isCommunicating && !gettingMessage)
  {
#ifdef DEBUG
    Serial.println("Communication started");
#endif
    gettingMessage = true;
  }
  // While makey makey is communicating, count number of highs in input pin 2.
  else if(isCommunicating && gettingMessage)
  {
#ifdef DEBUG2
    Serial.println("Getting message");
#endif
    boolean bitMeasure = digitalRead(INPUT_PIN_2);
    if (bitMeasure && ! oldBitMeasure)
      bitCounter++;
    
    oldBitMeasure = bitMeasure;
  }
  // When communication stops, act depending on number of highs in input.
  else if (!isCommunicating && gettingMessage)
  {
#ifdef DEBUG
    Serial.print("Communication ended! Symbol: ");
    Serial.println(bitCounter);
#endif

    // If makey makey sent 3 highs, turn on/off led in output pin.
    if(bitCounter == 3)
    {
      led1State = !led1State;
      digitalWrite(LED1_PIN, led1State);
      
      Serial.print("Setting pin: ");
      Serial.println(bitCounter);
    }
    
    // Reset communication variables.
    gettingMessage = false;
    oldBitMeasure = false;
    bitCounter = 0;
  }
}
