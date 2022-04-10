#include "Arduino.h"
#include "mastermind.h"
#include "lcd_wrapper.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
char* generate_code(bool repeat, int length) {
    if(!repeat && length > 10) {
        return NULL;
    }
    char* result = (char*)calloc(length+1,sizeof(char));
    randomSeed(analogRead(A2));
    randomSeed(analogRead(random(5,4563)));
      for(int i = 0 ; i < length ; ++i) {
        result[i] = random(0,9) + 48;
        if(repeat == false) {
          for(int j = 0 ; j < i ; ++j) {
            if(result[j] == result[i]) {
              i--;
              break;
            }
          }
        }
      }
    result[length] = '\0';
    return result;
}
void get_score(const char* secret, const char* guess, int* peg_a, int* peg_b) {
  int help = 0;
  int help1 = 0;
  for(int i = 0 ; i < strlen(secret) ; ++i) {
    if(secret[i] == guess[i]) {
      help++;
    }
  }
  for(int i = 0 ; i < strlen(secret) ; ++i) {
    for(int j = 0 ; j < strlen(secret) ; ++j) {
      if(i != j && secret[i] == guess[j]) {
        help1++;
        continue;
      }
    }
  }
  *peg_a = help;
  *peg_b = help1;
}
void turn_off_leds() {
  digitalWrite(LED_BLUE_1,LOW);
  digitalWrite(LED_RED_1,LOW);
  digitalWrite(LED_BLUE_2,LOW);
  digitalWrite(LED_RED_2,LOW);
  digitalWrite(LED_BLUE_3,LOW);
  digitalWrite(LED_RED_3,LOW);
  digitalWrite(LED_BLUE_4,LOW);
  digitalWrite(LED_RED_4,LOW);
}
void render_leds(const int peg_a, const int peg_b){
  int a = 0;
  int b = 0;
  for(int i = 0 ; i < 8 ; ++i) {
    if(a<peg_a) {
      digitalWrite(6+i,HIGH);
      a++;
      i++;
      continue;
    }
    if(b<peg_b) {
      digitalWrite(7+i,HIGH);
      b++;
      i++;
    }
  }
}
void render_history(char* secret, char** history, const int entry_nr) {
  int peg_a = 0;
  int peg_b = 0;
  get_score(secret, history[entry_nr], &peg_b, &peg_a);
  render_leds(peg_a,peg_b);
  lcd.print(history[entry_nr]);
}
void win() {
  digitalWrite(6,HIGH);
  delay(100);
  digitalWrite(7,HIGH);
  delay(100);
  digitalWrite(8,HIGH);
  delay(100);
  digitalWrite(9,HIGH);
  delay(100);
  digitalWrite(10,HIGH);
  delay(100);
  digitalWrite(11,HIGH);
  delay(100);
  digitalWrite(12,HIGH);
  delay(100);
  digitalWrite(13,HIGH);
  delay(1000);
  turn_off_leds();
}
void play_game(char* secret) {
//  char** history = calloc(sizeof(char*),10);
  int peg_a = 0;
  int peg_b = 0;
//  for(int i = 0 ; i < 4 l ; ++i) {
//    history[i] = calloc(sizeof(char),4);
//  }
    lcd.init();
    lcd.backlight();
    lcd.print("Welcome");
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Generate number");
    delay(5000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Guess");
    lcd.setCursor(0,1);
    char* guess = (char*)calloc(4,sizeof(char));
    for(int i = 0 ; i < 4 ; ++i) {
      guess[i] = '0';
    }
    guess[4] = '\0';
    lcd.print('>');
    int help = -1;
    int help1 = -1;
    int help2 = -1;
    int help3 = -1;
    int count = 0;
    while(true) {
      turn_off_leds();
      lcd.setCursor(15,1);
      lcd.print(count);
      if(digitalRead(2) == 0) {
        help++;  
        if(help > 9) {
          help = 0;
        }      
        delay(250);
        lcd.setCursor(2,1);
        lcd.print(help);
      }
      if(digitalRead(3) == 0) {
        help1++;
        if(help1 > 9) {
          help1 = 0;
        }
        delay(250);
        lcd.setCursor(3,1);
        lcd.print(help1);
      }
      if(digitalRead(4) == 0) {
        help2++;
        if(help2 > 9) {
          help2 = 0;
        }
        delay(250);
        lcd.setCursor(4,1);
        lcd.print(help2);
      }
      if(digitalRead(5) == 0) {
        help3++;
        if(help3 > 9) {
          help3 = 0;
        }
        delay(250);
        lcd.setCursor(5,1);
        lcd.print(help3);
      }
      if(digitalRead(A0) == 0) {
        guess[0] = help + 48;
        guess[1] = help1 + 48;
        guess[2] = help2 + 48;
        guess[3] = help3 + 48;
        get_score(secret,guess,&peg_a,&peg_b);
        if(peg_a == 4) {
          lcd.clear();
          lcd.setCursor(0,0);
          win();
          lcd.print("WIN");
          break;
        } else {
          count++;
        }
        if(count == 10) {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Lose");
          break;
        }
        render_leds(peg_a,peg_b);
        delay(3000);
      }
    }
    delay(4000);


}
