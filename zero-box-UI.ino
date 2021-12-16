#include <Keypad.h>
#include <SPI.h>
#include <Wire.h>
#include <UTFT.h>

UTFT lcd(ILI9341_16,38,39,40,41);   // CHANGE TO YOUR DISPLAY DRIVER, FOUND IN UTFT.h //

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

int x = lcd.getDisplayXSize();
int y = lcd.getDisplayYSize();
bool flag = false;


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

String val = ("");
String val1 = ("0.00");
String val2 = ("0.00");
float t1 = 0.0;
float f1 = 0.0;
float w1 = 0.0;
float t2 = 0.0;
float f2 = 0.0;
float w2 = 0.0;

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  lcd.InitLCD(LANDSCAPE);
  lcd.clrScr();
  gui();
  delay(1000);
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.

  Serial.begin(9600);
  Serial3.begin(9600);
  Serial2.begin(9600);
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
  
  
  
  
  lcd.setColor(252, 255, 82);
  lcd.print("OUTPUT 2", RIGHT, 0);
  lcd.setColor(255, 168, 82);
  lcd.print("T >", y/2, 30);
  lcd.print("F >", y/2, 60);
  lcd.print("W >", y/2, 90);

  
  lcd.setColor(255, 0, 0);
  lcd.drawLine(y/2,0,y/2,x);
  lcd.setColor(255, 255, 255);

}


void loop(){

  /// Add SYNC function to external button  ///
  /// which resets both raspi boards and    ///
  /// re-sends frequency to both boards at  ///
  /// same time. Also add phase shift,      ///
  /// posibly by resetting one board or the ///
  /// other then add a delay to reset the   ///
  /// opposite board.                       ///   
  
  char key = customKeypad.getKey();
  lcd.setFont(SmallFont);
  lcd.setColor(0, 255, 0);
  lcd.print(String(t1)+" S", 55, 35);
  lcd.print(val1+" hz", 55, 65);
  lcd.print(String(w1)+" M", 55, 95);
  
  lcd.print(String(t2)+" S", 55+(y/2), 35);
  lcd.print(val2+" hz", 55+(y/2), 65);
  lcd.print(String(w2)+" M", 55+(y/2), 95);
  
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
        val1 = val;
        f1 = val.toFloat();
        t1 = 1/f1;
        w1 = 343/f1;
      
        lcd.clrScr();
        gui();
        Serial3.println(val);
        delay(250);
        val = "";
        break;          
      }

    
      if (key == '2'){
        val2 = val;
        f2 = val.toFloat();
        t2 = 1/f2;
        w2 = 343/f2;
        lcd.clrScr();
        gui();
        Serial2.println(val);
        delay(250);
        val = "";
        break;
     }
     if (key == '3'){
      lcd.clrScr();
      lcd.print("CLEARED!", CENTER, x/2);
      val = "";
      delay(1000);
      lcd.clrScr();
      gui();
      break;
     }
    }
  }
  
}
