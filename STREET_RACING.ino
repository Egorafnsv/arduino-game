
#include <LiquidCrystal.h>
#define buttonPin 8
#define POT_PIN     A0
#define BUZZER_PIN 7

LiquidCrystal lcd(11, 12, 5, 4, 3, 2);

int k = 0;
int i = 0;
int m;
int s;
int lvl = 0;
int last_i_k[2];
unsigned long click_time;
unsigned long speed_game;
unsigned long your_time = 0;
unsigned long record_time = 999999999;
unsigned long start_time;
unsigned long finish_time
boolean current_button;
boolean was_press = false;
boolean changeLevel = false;

int level_1[2][16] = {
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
int level_2[2][16] = {
  {0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0}
};
int level_3[2][16] = {
  {0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0}
};
int level_4[2][16] = {
  {0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1}
};
int level_5[2][16] = {
  {0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0}
};
int level_6[2][16] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0}
};
int level_7[2][16] = {
  {0,1,1,0,0,0,1,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0}
};

int level[2][16];

byte car[8] = {
  B00000,
  B10010,
  B11111,
  B11101,
  B11101,
  B11111,
  B10010,
  B00000
};
byte empty[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};


byte building[8] = {
  B00000,
  B11111,
  B10101,
  B11111,
  B10101,
  B11111,
  B11011,
  B11011
};


void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.createChar(0, car);
  lcd.createChar(1, building);
  lcd.createChar(2, empty);
  lcd.begin(16, 2); 
  Serial.begin(9600);
  lcd.setCursor(1,0);
  lcd.print("STREET RACING");
  lcd.setCursor(3,1);
  lcd.print("UNLIMITED");
  delay(2000);
  startNewGame();
}

boolean checkButton(){
  current_button = !digitalRead(8); 
  if(current_button == true && was_press == false && millis() - click_time > 100){
    click_time = millis();
    was_press = true;
    tone(BUZZER_PIN, 3000, 70);
    return true;
  }
  else if (current_button == false && was_press == true && millis() - click_time > 100){
    was_press = false;
    click_time = millis();
  }
  return false;
}

void setLevel(int x){
  Serial.print("x ");
  Serial.println(x);
  switch(x){
    case 0:
      drawLevel(level_1);
      break;
    case 1:
      drawLevel(level_2);
      break;
    case 2:
      drawLevel(level_3);
      break;
    case 3:
      drawLevel(level_4);
      break;
    case 4:
      drawLevel(level_5);
      break;
    case 5:
      drawLevel(level_6);
      break;
    case 6:
      drawLevel(level_7);
      break;
  }
}

void drawLevel(int newLevel[2][16]){
  for(int i = 0; i < 2; i++){
    for(int j = 0; j < 16; j++){
      level[i][j] = newLevel[i][j];
      Serial.print(level[i][j]);
      Serial.print(" ");
    }
   Serial.println("");
  }
  lcd.clear();
  for(int i = 0; i < 2; i++){
    for(int j = 0; j < 16; j++){
      if(level[i][j] == 1){
        lcd.setCursor(j, i);
        lcd.write(byte(1));
      }
    }
  }
}

void gameOverScreen(){
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("YOU");
  lcd.setCursor(4,1);
  lcd.print("CRASHED");
  tone(BUZZER_PIN, 3000, 200);
  delay(200);
  tone(BUZZER_PIN, 2500, 200);
  delay(200);
  tone(BUZZER_PIN, 3000, 200);
  delay(200);
  tone(BUZZER_PIN, 2500, 500);
  delay(1600);
  startNewGame();
}

void finishScreen(){
        finish_time = millis();
        your_time = finish_time - start_time;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("The race is over");
        tone(BUZZER_PIN, 3500, 250);
        delay(250);
        tone(BUZZER_PIN, 3500, 250);
        delay(250);
        tone(BUZZER_PIN, 3500, 250);
        delay(250);
        tone(BUZZER_PIN, 2500, 150);
        delay(155);
        tone(BUZZER_PIN, 3600, 500);
        delay(1500);
        lcd.clear();
        m = your_time / 60000;
        s = (your_time / 1000) - m * 60;
        lcd.print(m);
        lcd.print("m : ");
        lcd.print(s);
        lcd.print("s");
        delay(3000);
        if(your_time < record_time){
          lcd.clear();
          lcd.print("NEW RECORD!");
          record_time = your_time;
          delay(3000);
        }
        startNewGame();
}


void startNewGame(){
  lcd.clear();
  lcd.print("BEST TIME:");
  lcd.setCursor(0,1);
  if (record_time == 999999999){
    lcd.print("--------");
  }
  else{
    m = record_time / 60000;
    s = (record_time / 1000) - m * 60;
    lcd.print(m);
lcd.print("m : ");
lcd.print(s);
lcd.print("s");
  }
  delay(2000);
  lcd.clear();
  lcd.print("  Press BUTTON");
  lcd.setCursor(3,1);
  lcd.print("to start...");
  while(!checkButton()){
   delay(100); 
  }
  i = 0;
  k = 0;
  lvl = 0; 
  lcd.clear();
  setLevel(0);
  start_time = millis(); 
}

void loop() {
    if (changeLevel){
      setLevel(lvl);
      changeLevel = false;
    }
    
    int speed_car;
    speed_car = analogRead(POT_PIN);
    speed_car = map(speed_car, 0, 1023, 50, 500);
    
    if(checkButton()){
      k = (k + 1) % 2;
    }
    if(millis() - speed_game >= speed_car){
      lcd.setCursor(last_i_k[0],last_i_k[1]);
      lcd.write(byte(2));
      lcd.setCursor(i,k);
      
      if(level[k][i] == 1){
        gameOverScreen();
        return;
      }   
      lcd.write(byte(0));
      if (i == 15){
        lvl += 1;
        changeLevel = true;
      }
      if (lvl == 7){
        finishScreen();
      }
      last_i_k[0] = i;
      last_i_k[1] = k;
      i = (i + 1) % 16;
      speed_game = millis();
    }
}
