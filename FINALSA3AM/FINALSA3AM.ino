#define button 5
#define reed_switch 2 // Connect reed switch to pin 2 (for interupt)
#define relay 4     
#define buzzer 3 // Connect reed switch to pin 2 (for interupt)
#define status_LED 13
bool button_history = true;
bool latch = true;
bool door_state = true;
int milli;
int seconds;
int minutes;
bool LED_state;
void(* resetFunc) (void) = 0; // To reset Arduino via code

// include the library code:
#include <LiquidCrystal_I2C.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

void setup() {
   lcd.init(); // initialize the lcd
  lcd.backlight();
  Serial.begin(9600);
  lcd.begin(16, 2);
  BOOTUP();
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT);
  pinMode(reed_switch, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(status_LED, OUTPUT);
  
  HOME();
  BUTTON_TRUE();
  
}

void loop() {
  // scroll 13 positions (string length) to the left
  // to move it offscreen left:
   
  SANITIZE();

}

void BOOTUP(){
  String BootupMSG_T = "-UV Box-";
  String BootupMSG_B = "BSITNS3A";
  lcd.setCursor(4,0);
  lcd.print(BootupMSG_T);
  
  lcd.setCursor(4,1);
  lcd.print(BootupMSG_B);

  delay(2000);
  
  for (int positionCounter = 0; positionCounter < BootupMSG_T.length()+3; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(200);
  }
  lcd.clear();
}

void HOME(){
  
  Serial.println("entered into HOME()");
  bool button_state = digitalRead(button);
 Serial.println("button state if 0 naka bukas if 1 naka sara");
 Serial.println(button_state);
 Serial.println(button_history);
 digitalWrite(relay,HIGH);
digitalWrite(buzzer, HIGH);
 
 if (button_state == false){
      button_history = false;
      Serial.println("pressed");
      return;
    }
  
  while(button_history == true){
  bool button_state = digitalRead(button);
  
    if (button_state == false){
      button_history = false;
      return;
    }
      
  Serial.println("In while loop");
  String HOME_T = "UV Box:";
  String HOME_B = "Press To Start";
  lcd.setCursor(0,0);
  lcd.print(HOME_T);
  
  lcd.setCursor(1,1);
  lcd.print(HOME_B);
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(HOME_T);
  delay(600);
  }
}

void BUTTON_TRUE(){
  lcd.clear();
  String BT_T = "Preparing to";
  String BT_B = "Sterilize ";
  lcd.setCursor(2,0);
  lcd.print(BT_T);
  
  lcd.setCursor(4,1);
  lcd.print(BT_B);
  delay(1000);
  //check if door is closed (for safety reasons)
  
  while (latch == true){
   lcd.clear();
   latch = digitalRead(reed_switch);
   if (latch == false){
    break;
   }
   Serial.println(latch);
   String LATCH_T = "Please Close";
   String LATCH_B = "Door";
   
   lcd.setCursor(2,0);
   lcd.print(LATCH_T);
  
   lcd.setCursor(6,1);
   lcd.print(LATCH_B);
   delay(500);
   
   latch = digitalRead(reed_switch);
   if (latch == false){
    break;
   }
   
   lcd.clear();
   delay(500);
  }

}

void SANITIZE(){
 Serial.println("sanatize na");
  digitalWrite(buzzer, HIGH);
  
  
  
  door_state = digitalRead(reed_switch);
  
  if (door_state == true){
    digitalWrite(relay, HIGH);
     LID_OPEN();
  }
  digitalWrite(relay,LOW);
  //lcd.clear();
  String SANTIZIE_T = "Sterilizing: ";
  lcd.setCursor(2,0);
  lcd.print(SANTIZIE_T);
  lcd.setCursor(5,1);
  lcd.print(14-minutes);
  lcd.print(":");
  if (seconds > 49){
    lcd.print("0");
    lcd.print(59-seconds);
  }
  if (seconds <50){
    lcd.print(59-seconds);
  }
 
  
  
  milli++;
  if (milli >= 10){
    seconds++;
    if (LED_state == false){
      digitalWrite(status_LED, HIGH);
      LED_state == true;
    }
    else{
      digitalWrite(status_LED, LOW);
      LED_state == false;
    }
    milli = 0;
    
  }
  if (seconds >= 60){
    lcd.clear();
    minutes ++;
    seconds = 0;
  }
  if (minutes >= 1){
    minutes = 0;
    DONE();
  }
  Serial.println(milli);
  Serial.println(seconds);
  Serial.println(minutes);
  delay(100);
  
  
}

void DONE(){
  lcd.clear();

  String DONE_T = "Sterilization";
  String DONE_B = "Complete";
  int counter;
  
//  while (latch == true){
//    latch = digitalRead(reed_switch);
    
    lcd.setCursor(1,0);
    lcd.print(DONE_T);
    lcd.setCursor(3,1);
    lcd.print(DONE_B);

//    if (counter < 3){ // Beep buzzer 3 times after done (so it does not get annoying)
      digitalWrite(buzzer, LOW);
      digitalWrite(status_LED, HIGH);
//      counter++;
//    }
    
    delay(500);

    lcd.clear();
    digitalWrite(buzzer, HIGH);
    digitalWrite(status_LED, LOW);
    delay(500);
//  }
  button_history = true; 
  HOME();
  BUTTON_TRUE();
}

void LID_OPEN(){
  Serial.println("In LID_OPEN");
  digitalWrite(relay, HIGH);
  digitalWrite(buzzer, LOW);
  delay(1000);
  
  latch = digitalRead(reed_switch);
  
  if (latch == false){
    return;
   }
   
  //Screen 1:
  String LCD_OPEN_T1 = "-Warning-";
  //No LCD_OPEN_T1 
  //Screen 2:
  String LCD_OPEN_T2 = "Sterilizing";
  String LCD_OPEN_B2 = "Stopped";
  //Screen 3:
  String LCD_OPEN_T3 = "Please Close";
  String LCD_OPEN_B3 = "Door";
  
  while(latch == true){
   delay(1000);
   lcd.clear();
   Serial.println("In LID_OPEN While LOOP");
   latch = digitalRead(reed_switch);
   
   lcd.setCursor(3,0);
   lcd.print(LCD_OPEN_T1);
   digitalWrite(buzzer, LOW);
   digitalWrite(status_LED, HIGH);
   
  
  if (latch == false){
    break;
  }
   latch = digitalRead(reed_switch);
   delay(500);
   digitalWrite(buzzer, LOW);
   digitalWrite(status_LED, LOW);
   lcd.clear();
   delay(500);
   lcd.setCursor(3,0);
   lcd.print(LCD_OPEN_T1);
   latch = digitalRead(reed_switch);
  
  if (latch == false){
    break;
   }
   digitalWrite(buzzer, HIGH);
   digitalWrite(status_LED, HIGH);
   delay(500);
   digitalWrite(buzzer, LOW);
   digitalWrite(status_LED, LOW);
   lcd.clear();
   delay(500);
   
   lcd.setCursor(3,0);
   lcd.print(LCD_OPEN_T1);
   latch = digitalRead(reed_switch);
   
  if (latch == false){
    break;
   }
   digitalWrite(buzzer, HIGH);
   digitalWrite(status_LED, HIGH);
   delay(500);
   
   digitalWrite(buzzer, LOW);
   digitalWrite(status_LED, LOW);
   lcd.clear();
   if (latch == false){
    break;
   }
   delay(500);
   
   lcd.setCursor(2,0);
   lcd.print(LCD_OPEN_T2);
   lcd.setCursor(4,1);
   lcd.print(LCD_OPEN_B2);
   latch = digitalRead(reed_switch);
  
  if (latch == false){
    break;
   }
   digitalWrite(buzzer, HIGH);
   digitalWrite(status_LED, HIGH);
   delay(1000);
    if (latch == false){
    break;
   }
  
   lcd.clear();
   
   lcd.setCursor(2,0);
   lcd.print(LCD_OPEN_T3);
   
   lcd.setCursor(6,1);
   lcd.print(LCD_OPEN_B3);
   
   latch = digitalRead(reed_switch);
  
  if (latch == false){
    break;
   }
   digitalWrite(buzzer, LOW);
   digitalWrite(status_LED, LOW);
   delay(750);
  }
  return;
}
