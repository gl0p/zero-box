#include <Keypad.h>
#include <SPI.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <UTFT.h>

SFE_BMP180 pressure;
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
float f1 = 1.0;
double w1 = 0;
float t2 = 1.0;
float f2 = 1.0;
double w2 = 0;
String phase = ("");
float phase1 = 0.00;
float phase2 = 0.00;
float ph1 = 0.0;
float ph2 = 0.0;
float time_delay1 = 1.0;
float time_delay2 = 1.0;
double constantd = 0;
unsigned long constant = 0;
float phase_rad = 0.0;
float peroid = 0.0;
float time_per_rad = 0.0;
String duty = ("");

char status;
double T,P,K;

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  pinMode(5, INPUT_PULLUP); //Set phase shift
  pinMode(3, INPUT_PULLUP); //Set duty cycle 
  pinMode(4, OUTPUT);       //Sync output
  pinMode(2, OUTPUT);       //Sync output
  
  Serial.begin(9600);
  Serial3.begin(9600);
  Serial2.begin(9600);
  Serial1.begin(9600);
  
  pressure.begin();
  
  status = pressure.startTemperature();
  if (status != 0){
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0){
      status = pressure.startPressure(3);
      if (status != 0){
        delay(status);
        status = pressure.getPressure(P,T);
        K = T+273.15;
      }
    }
  }
  delay(500);
    

  
  lcd.InitLCD(LANDSCAPE);
  
  lcd.clrScr();
  lcd.setFont(BigFont);
  lcd.setColor(255, 255, 255);
  lcd.fillScr(0, 0, 0);
  lcd.setBackColor(0, 0, 0);

  Serial3.println("1.0");
  Serial2.println("1.0");
  
  lcd.print("Select Constant", CENTER, 10);
  lcd.print("1>Sound or 2>Light", CENTER, 30);
  while (true){
    char key = customKeypad.getKey();
    if (key == '1'){
      constant = sqrt(1.4*8.345*K/0.0289645);
      delay(250);
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
  
  digitalWrite(4, HIGH);
  digitalWrite(2, HIGH);
  delay(50);
  digitalWrite(4, LOW);
  digitalWrite(2, LOW);

  
  
}

void sound_constant(){
  status = pressure.startTemperature();
  if (status != 0){
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0){
      status = pressure.startPressure(3);
      if (status != 0){
        delay(status);
        status = pressure.getPressure(P,T);
        K = T+273.15;
        constant = sqrt(1.4*8.345*K/0.0289645);
      }
    }
  }
  delay(500);
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
  lcd.drawLine(y/2,0,y/2,x-100);
  lcd.drawLine(0,x-100,x+75,x-100);
  lcd.setColor(255, 255, 255);

}

void sync_all(){
  digitalWrite(4, HIGH);
  digitalWrite(2, HIGH);
  delay(50);
  digitalWrite(4, LOW);
  digitalWrite(2, LOW);
  phase1 = 0.00;
  phase2 = 0.00;
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
            phase_rad = phase.toFloat()*(3.14159/180);
            peroid = 1/f1;
            time_per_rad = peroid/(2*3.14159);
            time_delay1 = (time_per_rad*phase_rad)*1000000;
            if (phase1 == 0){
              sync_all();
             }
             
            Serial1.println(time_delay1);
            delay(250);
            phase = "";
            break;          
           }
          
          if (key == '2'){
            phase2 = phase.toFloat();
            phase_rad = phase2*(3.14159/180);
            peroid = 1/f2;
            time_per_rad = peroid/(2*3.14159);
            time_delay2 = (time_per_rad*phase_rad)*1000000;
            if (phase2 == 0){
              sync_all();
             }
             
            Serial.println(time_delay2);
            delay(250);
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

void dutycycle(){
  if (digitalRead(3) == LOW){
    lcd.clrScr();
    while (true){
      char key = customKeypad.getKey();
      lcd.setFont(BigFont);
      lcd.print("Enter Dutycycle", CENTER, 10);
      if (key != 'E'){
        duty = duty+String(key);
        lcd.print(duty, CENTER, x/2);
        }
      if (key == 'E'){
        lcd.clrScr();
        lcd.print("SELECT BOARD 1 OR 2", CENTER, x/2);
        while (true){
          char key = customKeypad.getKey();
          
          if (key == '1'){
            Serial3.println("duty="+duty);
            delay(250);
            duty = "";
            break;          
           }
          
          if (key == '2'){   
            Serial2.println("duty="+duty);
            delay(250);
            duty = "";
            break;          
          }
           
          if (key == '3'){
            lcd.clrScr();
            lcd.print("CANCLED!", CENTER, x/2);
            duty = "";
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

     



int y_sig = 0;

void loop(){
  /// DRAW PIXEL FREQUENCY  FROM ANALOG READ ///
  //sig = analogRead(A0)
//  if (flag == false){
//    for(int y_sig=0; y_sig<=(y/2)-2; y_sig++){
//      int sig = map(analogRead(A0), 0, 1023, 230, 140);
//      int sig2 = map(analogRead(A1), 0, 1023, 230, 140);
//      lcd.setColor(0,0,0);
//      lcd.drawLine(y_sig,140,y_sig+1,230);
//      lcd.drawLine(y_sig+(y/2)+1,140,y_sig+(y/2)+2,230);
//      lcd.setColor(0,0,255);
//      lcd.drawPixel(y_sig,sig);
//      lcd.drawPixel(y_sig+(y/2)+1,sig2);
//      if (y_sig == (y/2)-2){
//        flag = true;
//      }
//    }
//    
//  }
  

  
  phase_shift();  
  dutycycle();
  
  char key = customKeypad.getKey();
  lcd.setFont(SmallFont);
  lcd.setColor(0, 255, 0);
  lcd.print(String(t1, 4)+" S", 55, 35);
  lcd.print(String(f1,4)+" hz", 55, 65);
  lcd.print(String(w1,3)+" M", 55, 95);
  lcd.print(String(phase1,4)+" deg", 55, 125);
  
  lcd.print(String(t2,4)+" S", 55+(y/2), 35);
  lcd.print(String(f2,4)+" hz", 55+(y/2), 65);
  lcd.print(String(w2,3)+" M", 55+(y/2), 95);
  lcd.print(String(phase2,4)+" deg", 55+y/2, 125);

  /// PUT DISPLAY OF PRESSURE SENSOR HERE ///
  
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
        f1 = val1.toFloat();
        t1 = 1/f1;
        w1 = constant/f1;
        lcd.clrScr();
        if (sync_flag == false){
          Serial3.println(val1);
          sync_flag = true;
          delay(250);
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
        flag = false;
        break;          
      }

    
      if (key == '2'){
        flag = false;
        sync_flag = false;
        val2 = val;
        f2 = val2.toFloat();
        t2 = 1/f2;
        w2 = constant/f2;
        lcd.clrScr();
        if (sync_flag == false){
          Serial2.println(val2);
          sync_flag = true;
          delay(250);
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
        flag = false;
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
