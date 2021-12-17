#include <Keypad.h>
#include <SPI.h>
#include <Wire.h>
#include <UTFT.h>

UTFT lcd(ILI9341_16,38,39,40,41);   // CHANGE TO YOUR DISPLAY DRIVER, FOUND IN UTFT.h //

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

int x = lcd.getDisplayXSize();
int y = lcd.getDisplayYSize();

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'7','8','9'},
  {'4','5','6'},
  {'1','2','3'},
  {'0','.','E'}
};
byte rowPins[ROWS] = {12,11,10,9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6,7,8}; //connect to the column pinouts of the keypad

bool flag = false;
bool sync_flag = false;
String val = ("");
String val1 = ("1.00");
String val2 = ("1.00");
float t1 = 1.0;
float f1 = 0.0;
unsigned long w1 = 0;
float t2 = 1.0;
float f2 = 0.0;
unsigned long w2 = 0;
String phase = ("");
float phase1 = 0.00;
float phase2 = 0.00;
float ph1 = 0.0;
float ph2 = 0.0;
float time_delay1 = 1.0;
float time_delay2 = 1.0;
unsigned long constant = 0;

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  pinMode(5, INPUT_PULLUP); //Initiate sync
  pinMode(4, OUTPUT);       //Sync output
  pinMode(2, OUTPUT);       //Sync output
  lcd.InitLCD(LANDSCAPE);
  lcd.clrScr();
  gui();
  delay(1000);
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.

  Serial.begin(9600);
  Serial3.begin(9600);
  Serial2.begin(9600);

  Serial3.println("1.0");
  Serial2.println("1.0");
  delay(1000);
  digitalWrite(4, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(2, LOW);

  lcd.clrScr();
  lcd.print("Select Constant", CENTER, 10);
  lcd.print("1>Sound or 2>Light", CENTER, 30);
  while (true){
    char key = customKeypad.getKey();
    if (key == '1'){
      constant = 343;
      lcd.clrScr();
      gui();
      break;
    }
    if (key == '2'){
      constant = 299702547;
      lcd.clrScr();
      gui();
      break;
    }
  }
  
}

void gui(){
  //lcd.clrScr();
  lcd.setColor(252, 255, 82);
  lcd.setBackColor(0, 0, 0);
  lcd.setFont(BigFont);
  
  lcd.print("OUTPUT 1", LEFT, 0);
  lcd.setColor(255, 168, 82);
  lcd.print("T >", LEFT, 30);
  lcd.print("F >", LEFT, 60);
  lcd.print("W >", LEFT, 90);
  lcd.print("P >", LEFT, 120);
  
  
  
  lcd.setColor(252, 255, 82);
  lcd.print("OUTPUT 2", RIGHT, 0);
  lcd.setColor(255, 168, 82);
  lcd.print("T >", y/2, 30);
  lcd.print("F >", y/2, 60);
  lcd.print("W >", y/2, 90);
  lcd.print("P >", y/2, 120);

  
  lcd.setColor(255, 0, 0);
  lcd.drawLine(y/2,0,y/2,x);
  lcd.setColor(255, 255, 255);

}

void phase_shift(){
  if (digitalRead(5) == LOW){
    lcd.clrScr();
    while (true){
      char key = customKeypad.getKey();
      lcd.setFont(BigFont);
      lcd.print("Enter Phase 0 - 360", CENTER, 10);
      if (key != 'E'){
        phase = phase+String(key);
        lcd.print(phase, CENTER, x/2);
        }
      if (key == 'E'){
        lcd.clrScr();
        lcd.print("SELECT BOARD 1 OR 2", CENTER, x/2);
        while (true){
          char key = customKeypad.getKey();
          
          if (key == '1'){
            phase1 = phase.toFloat();
            ph1 = phase1/(360*f1);
            time_delay1 = ph1*1000;
            if (phase1 == 0){
              sync_all();
             }
            digitalWrite(4, HIGH);
            delayMicroseconds(time_delay1);
            digitalWrite(4, LOW);
            
            Serial.print("Time Delay 1: ");
            Serial.print(time_delay1);
            Serial.print(" Phase: ");
            Serial.println(phase1);
            phase = "";
            break;          
          }
    
        
          if (key == '2'){
            phase2 = phase.toFloat();
            ph2 = phase2/(360*f1);
            time_delay2 = ph2*1000;
            if (phase2 == 0){
              sync_all();
             }
            digitalWrite(2, HIGH);
            delayMicroseconds(time_delay2);
            digitalWrite(2, LOW);
            
            Serial.print("Time Delay 2: ");
            Serial.print(time_delay2);
            Serial.print(" Phase: ");
            Serial.println(phase2);
            phase = "";
            break;          
          }
           
           
          if (key == '3'){
            lcd.clrScr();
            lcd.print("CANCLED!", CENTER, x/2);
            phase = "";
            delay(1000);
            lcd.clrScr();
            gui();
            break;
           }
           
        }
        lcd.clrScr();
        gui();
        break;
        
      }
    }
  }
}
void sync_all(){
  digitalWrite(4, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(2, LOW);
  phase1 = 0.00;
  phase2 = 0.00;
}

void loop(){
  phase_shift();  
  
  char key = customKeypad.getKey();
  lcd.setFont(SmallFont);
  lcd.setColor(0, 255, 0);
  lcd.print(String(t1)+" S", 55, 35);
  lcd.print(val1+" hz", 55, 65);
  lcd.print(String(w1)+" M", 55, 95);
  lcd.print(String(phase1)+" deg", 55, 125);
  
  lcd.print(String(t2)+" S", 55+(y/2), 35);
  lcd.print(val2+" hz", 55+(y/2), 65);
  lcd.print(String(w2)+" M", 55+(y/2), 95);
  lcd.print(String(phase2)+" deg", 55+y/2, 125);
  
  if (key != 'E'){
    val = val+String(key);
    lcd.setFont(BigFont);
    lcd.setColor(255, 255, 255);
    lcd.print(val, CENTER, x-20);
    delay(25);
  }

  if (key == 'E'){
    lcd.setFont(BigFont);
    lcd.clrScr();
    lcd.print("SELECT BOARD 1 OR 2", CENTER, x/2);
    
    
    while (true){
      char key = customKeypad.getKey();
      
      if (key == '1'){
        sync_flag = false;
        val1 = val;
        f1 = val.toFloat();
        t1 = 1/f1;
        w1 = constant/f1;
        lcd.clrScr();
        if (sync_flag == false){
          Serial3.println(val1);
          delay(50);
          sync_flag = true;
        }
        while (true){
          char key = customKeypad.getKey();
          lcd.print("Sync other device?", CENTER, 10); 
          lcd.print("1 > YES or 2 > NO", CENTER, 30);
          
          if (key == '1'){
            sync_all();
            delay(250);
            break;
          }
          if (key == '2'){
            delay(250);
            break;
          }
        }
        lcd.clrScr();
        gui();
        val = "";
        sync_flag = false;
        break;          
      }

    
      if (key == '2'){
        sync_flag = false;
        val2 = val;
        f2 = val.toFloat();
        t2 = 1/f2;
        w2 = constant/f2;
        lcd.clrScr();
        if (sync_flag == false){
          Serial2.println(val2);
          delay(50);
          sync_flag = true;
        }
        while (true){
          char key = customKeypad.getKey();
          lcd.print("Sync other device?", CENTER, 10); 
          lcd.print("1 > YES or 2 > NO", CENTER, 30);
          
          if (key == '1'){
            sync_all();
            delay(250);
            break;
          }
          if (key == '2'){
            delay(250);
            break;
          }
        }
        lcd.clrScr();
        gui();
        val = "";
        sync_flag = false;
        break;
     }
     if (key == '3'){
      lcd.clrScr();
      lcd.print("CANCLED!", CENTER, x/2);
      val = "";
      delay(1000);
      lcd.clrScr();
      gui();
      break;
     }
    }
  }
  
}
