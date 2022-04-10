
#include "mastermind.h"
#include "lcd_wrapper.h"
#include <Arduino.h>


void setup(){
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(A0,INPUT_PULLUP);
  
}

void loop(){
  char* secret = generate_code(false, 4);
  play_game(secret);
  free(secret);
  
}
