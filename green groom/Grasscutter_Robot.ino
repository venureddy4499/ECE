#include <LiquidCrystal_I2C.h> 
#include "DHT.h" 
#include <SoftwareSerial.h> 
String value_1; 
int TxD = 4; 
int RxD = 3; 
int lcdColumns = 16; 
int lcdRows = 2; 
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
int16_t tfDist; // distance in centimeters 
int16_t tfAddr = TFL_DEF_ADR; // Use this default I2C address 
 
#define DHTPIN 2 
#define Left_Ir 7 
#define Right_Ir 6 
 
#define motor1 5 
#define motor11 11 
#define motor2 9 
#define motor22 10 
#define Switch_mode A2  
#define Cutter_Robot 8 
 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE); 
SoftwareSerial bluetooth(TxD, RxD); 
int FLAG = 0; 
 
void setup(){ 
Serial.begin(9600); // Initalize serial port 
bluetooth.begin(9600); 
 
Wire.begin(); // Initalize Wire library 
lcd.init(); 
lcd.backlight(); 
 
 
lcd.setCursor(0, 0); 
lcd.print("ECO GRASS CUTTER"); 
 
pinMode(Left_Ir,INPUT); 
pinMode(Right_Ir,INPUT); 
pinMode(Switch_mode,INPUT_PULLUP); 
 
dht.begin(); 
 
pinMode(motor1, OUTPUT); 
pinMode(motor11, OUTPUT); 
pinMode(motor2, OUTPUT); 
pinMode(motor22, OUTPUT); 
pinMode(Cutter_Robot, OUTPUT); 
 
digitalWrite(Cutter_Robot, HIGH); 
 
} 
 
void Forward(){ 
digitalWrite(motor1, HIGH); 
digitalWrite(motor11, LOW); 
digitalWrite(motor2, HIGH); 
digitalWrite(motor22, LOW); 
Serial.println("FORWARD"); 
} 
void Backward(){ 
  
digitalWrite(motor1, LOW); 
digitalWrite(motor11, HIGH); 
digitalWrite(motor2, LOW); 
digitalWrite(motor22, HIGH); 
Serial.println("BACKWARD");  
} 
void Right(){ 
digitalWrite(motor1, HIGH); 
digitalWrite(motor11, LOW); 
digitalWrite(motor2, LOW); 
digitalWrite(motor22, HIGH); 
Serial.println("RIGHT");  
} 
void Left(){ 
digitalWrite(motor1, LOW); 
digitalWrite(motor11, HIGH); 
digitalWrite(motor2, HIGH); 
digitalWrite(motor22, LOW); 
Serial.println("LEFT");  
} 
void Stop(){ 
digitalWrite(motor1, LOW); 
digitalWrite(motor11, LOW); 
digitalWrite(motor2, LOW); 
digitalWrite(motor22, LOW); 
Serial.println("STOP"); 
} 
void loop(){ 
int tempp = dht.readTemperature(); 
int hum = dht.readHumidity(); 
 
// if(tflI2C.getData(tfDist, tfAddr)){ 
// if((tfDist < 5) || (digitalRead(Left_Ir) == 0) || (digitalRead(Right_Ir)==0)){ 
// Stop(); 
// } 
// } 
lcd.setCursor(0, 1); 
lcd.print("T:"+String(tempp)); 
lcd.setCursor(5, 1); 
lcd.print("H:"+String(hum)); 
 
digitalWrite(Cutter_Robot, LOW); 
 
 
if(digitalRead(Switch_mode) == 0){ 
if(FLAG == 1){ 
Stop(); 
FLAG = 0; 
} 
Serial.println("ROBOT IS IN BLUETOOTH MODE"); 
if(bluetooth.available() > 0){ 
while (bluetooth.available() > 0) 
 

{ 
char inChar = (char)bluetooth.read();  
value_1 += inChar; }  
Serial.println(value_1); 
if (value_1 == "F"){Forward();} 
else if (value_1 == "B"){Backward();} 
else if (value_1 == "R"){Right();} 
else if (value_1 == "L"){Left();} 
else if (value_1 == "S"){Stop();} 
value_1 = "";  
} 
} 
 
else{ 
FLAG =1; 
Serial.println("ROBOT IS IN AUTOMATED MODE"); 
if(tflI2C.getData(tfDist, tfAddr)) 
{ 
if((tfDist < 5) || (digitalRead(Left_Ir) == 0) || (digitalRead(Right_Ir)==0)){ 
Right(); 
} 
else{ 
Forward(); 
