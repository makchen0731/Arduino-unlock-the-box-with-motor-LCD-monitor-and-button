#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
#include <SPI.h> // f.k. for Arduino-1.5.2
#include <Bounce2.h>
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
//#include <Adafruit_TFTLCD.h>
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define GRAY 0x8410
#define WHITE 0xFFFF
#define RED 0xF800
#define ORANGE 0xFA60
#define YELLOW 0xFFE0
#define LIME 0x07FF
#define GREEN 0x07E0
#define CYAN 0x07FF
#define AQUA 0x04FF
#define BLUE 0x001F
#define MAGENTA 0xF81F
#define PINK 0xF8FF
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

int a=0;
const int Pin1 = 30;
const int Pin2 = 31;
const int Pwm = 13;
const int buttonPinY = 10;// the number of the pushbutton pin
const int buttonPinN = 11;
int PinStateY = LOW;
int PinStateN = LOW;
Bounce debouncer1 = Bounce(); // Instantiate a Bounce object
Bounce debouncer2 = Bounce(); //debounce可以設1,2,或其他
uint16_t pixel;
uint16_t g_identifier;

void strDelay(const char* str1,uint32_t delay_msec){ //回傳字元+words verbatim(逐字出現)
  while(*str1!='\0'){
    tft.print(*str1);
    delay(delay_msec);
    ++str1;
  }
}

void setup(void){
  Serial.begin(9600);
  uint32_t when = millis();
  // while (!Serial) ; //hangs a Leonardo until you connect a Serial
  if (!Serial) delay(5000); //allow some time for Leonardo
  g_identifier = tft.readID(); //
  Serial.print("ID = 0x");
  tft.begin(g_identifier);
  pinMode(buttonPinY,INPUT);
  pinMode(buttonPinN,INPUT);
  pinMode(Pin1,OUTPUT);
  pinMode(Pin2,OUTPUT);
  pinMode(Pwm,OUTPUT);
  debouncer1.attach(buttonPinY); // 為消除按鈕顫動程式
  debouncer2.attach(buttonPinN);
  debouncer1.interval(20);
  debouncer2.interval(20);
} 

void loop() {
  debouncer1.update(); //for Y
  debouncer2.update(); //for N
  if(a==0){
    a++;
    tft.fillScreen(BLACK);
    tft.setRotation(45);
    tft.setTextSize(4);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextWrap(true);
    char* str = " TITLE ";
    strDelay(str,70);
  }
  if(a==1 && debouncer1.fell()){ //read boutton signal
    a=a+50;
    tft.fillScreen(BLACK);
    tft.setRotation(45);
    tft.setTextSize(4);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextWrap(true);
    char* str = "Qy";
    strDelay(str,70);
    debouncer1.update();
    debouncer2.update();
  } //Y
  if(a==51 && debouncer1.fell()){
    a=520;
    tft.fillScreen(BLACK);
    tft.setRotation(45);
    tft.setTextSize(4);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextWrap(true);
    char* str = "Qyy";
    strDelay(str,70);
    debouncer1.update();
    debouncer2.update();
  } //YY
  if(a == 51 && debouncer2.fell()){
    a=2;
    tft.fillScreen(BLACK);
    tft.setRotation(45);
    tft.setTextSize(4);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextWrap(true);
    char* str = "Qyn";
    strDelay(str,70);
    debouncer1.update();
    debouncer2.update();
  } //YN
  if(a == 1 && debouncer2.fell()){
    a++;
    tft.fillScreen(BLACK);
    tft.setRotation(45);
    tft.setTextSize(4);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextWrap(true);
    char* str = "Qn";
    strDelay(str,70);
    debouncer1.update();
    debouncer2.update();
  } //N
  if(a == 2 && debouncer1.fell()){
    a=520;
    tft.fillScreen(BLACK);
    tft.setRotation(45);
    tft.setTextSize(4);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextWrap(true);
    char* str = "QnyQyny";
    strDelay(str,70);
    debouncer1.update();
    debouncer2.update();
  } //NY & YNY
  if(a == 2 && debouncer2.fell()){
    a=520;
    tft.fillScreen(BLACK);
    tft.setRotation(45);
    tft.setTextSize(4);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextWrap(true);
    char* str = "QnnQynn";
    strDelay(str,70);
    debouncer1.update();
    debouncer2.update();
  } //NN & YNN
  if(a==520 && (debouncer2.fell()|| debouncer1.fell())) { // Call code if Bounce fell (transition from HIGH to LOW)
    a++;
    tft.fillScreen(BLACK);
    tft.setCursor(0,70);
    tft.setRotation(45);
    tft.setTextSize(4);
    tft.setTextColor(WHITE);
    tft.setTextWrap(true);
    char* str = "END";
    strDelay(str,100);
    digitalWrite(Pin1,LOW); //驅動直流馬達 轉1.7秒
    digitalWrite(Pin2,HIGH);
    analogWrite(Pwm,255);
    delay(1700);
    digitalWrite(Pin1,LOW);
    digitalWrite(Pin2,LOW);
    analogWrite(Pwm,255);
  }
}