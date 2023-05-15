

#include <WiFi.h>
#include "DHT.h"
#include <FirebaseESP32.h>
#include <BlynkSimpleEsp32.h>
#include <SimpleTimer.h>

#include <Wire.h>                 //Thư viện giao tiếp I2C
#include <LiquidCrystal_I2C.h>    //Thư viện LCD
LiquidCrystal_I2C lcd(0x27,16,2); //Thiết lập địa chỉ và loại LCD

#define DHTPIN1 16
#define DHTPIN2 17
#define up1      18
#define down1    21
#define up2      25
#define down2    26
#define enter1   5
#define enter2   35
#define den1     12
#define den2     27

#define DHTTYPE DHT11
#define WIFI_SSID "vivo 1915"
#define WIFI_PASSWORD "23456789"
#define FIREBASE_HOST "https://kc326nhom11-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "4J1OihUwOVzypHwww3gcaOYJNb30P8bBpC2PrYlm"
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
FirebaseData fbdb;
SimpleTimer timer, timer1, timer2;
int timerID1, timerID2, timerID3;

byte icon[] = {B01110,B10001,B10001,B01110,B0000,B0000,B0000,B0000};

unsigned long times=millis();
int dem1 = 0, dem2 = 0, t1 = 0, h1 = 0, t2 = 0, h2 = 0, ngay = 0, set1 = 0, set2 = 0;
void setup() {
  pinMode (up1, INPUT_PULLUP);
  pinMode (down1, INPUT_PULLUP);
  pinMode (up2, INPUT_PULLUP);
  pinMode (down2, INPUT_PULLUP);
  pinMode (enter1, INPUT_PULLUP);
  pinMode (enter2, INPUT_PULLUP);
  pinMode (den1, OUTPUT);
  pinMode (den2, OUTPUT);
  Serial.begin(115200);

  Wire.begin(22,23);               //Thiết lập chân kết nối I2C (SDA,SCL);
  lcd.init();                      //Khởi tạo LCD
  lcd.clear();                     //Xóa màn hình
  lcd.backlight();                 //Bật đèn nền
  lcd.setCursor(0,0);              //Đặt vị trí ở ô thứ 3 trên dòng 1
  lcd.print("Dang ket noi... ");          //Ghi đoạn text "Welcom to"

  
  dht1.begin();
  dht2.begin();
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Firebase.setInt(fbdb,"/Soluong1", dem1);
  Firebase.setInt(fbdb,"/Soluong2", dem2);
  Firebase.setInt(fbdb,"/Nhiet1", t1);
  Firebase.setInt(fbdb,"/Am1", h1);
  Firebase.setInt(fbdb,"/Nhiet2", t2);
  Firebase.setInt(fbdb,"/Am2", h2);
  Firebase.setInt(fbdb,"/Ngay1", ngay);
  Firebase.setInt(fbdb,"/Ngay2", ngay);
//  Firebase.reconnectWiFi(true);
//
//  Firebase.setReadTimeout(fbdb,1000*60);
//  Firebase.setWriteSizeLimit(fbdb,"tiny");

  timerID1 = timer.setInterval(60000, ngayap);
}

void loop() {
  timer.run();
  check_DHT11();
  soluong();
  capnhat();
}
void check_DHT11(){
    // Reading temperature or humidity
    t1 = dht1.readTemperature();
    h1 = dht1.readHumidity();
    if (isnan(h1) || isnan(t1)) {
      Serial.println(F("Failed to read from DHT1 sensor!"));
      return;
    }
    t2 = dht2.readTemperature();
    h2 = dht2.readHumidity();
    if (isnan(h2) || isnan(t2)) {
      Serial.println(F("Failed to read from DHT2 sensor!"));
      return;
    }
    if (t1 > 37) digitalWrite(den1, 0);
    if (t2 > 37) digitalWrite(den2, 0);
    if (t1 < 36) digitalWrite(den1, 1);
    if (t2 < 36) digitalWrite(den2, 1);
    lcd.setCursor(0,0);              
    lcd.print("T:");                 
    lcd.setCursor(2,0);              
    lcd.print(t1);
    lcd.createChar(1,icon);
    lcd.setCursor(4,0);
    lcd.write(1); 
    lcd.setCursor(5,0);             
    lcd.print("C");
    lcd.setCursor(6,0);             
    lcd.print(" ");
    lcd.setCursor(7,0);              
    lcd.print("H:");                 
    lcd.setCursor(9,0);              
    lcd.print(h1);
    lcd.setCursor(11,0);              
    lcd.print("%"); 
    lcd.setCursor(12,0);              
    lcd.print("  ");

    lcd.setCursor(0,1);              
    lcd.print("T:");                 
    lcd.setCursor(2,1);              
    lcd.print(t2);
    lcd.setCursor(4,1);
    lcd.write(1);
    lcd.setCursor(5,1);              
    lcd.print("C");
    lcd.setCursor(6,1);             
    lcd.print(" ");
    lcd.setCursor(7,1);              
    lcd.print("H:");                 
    lcd.setCursor(9,1);              
    lcd.print(h2);
    lcd.setCursor(11,1);              
    lcd.print("%"); 
    lcd.setCursor(12,1);              
    lcd.print("  ");
     

    

}
void soluong(){
  int tang1 = digitalRead (up1);
  int giam1 = digitalRead (down1);
  int tang2 = digitalRead (up2);
  int giam2 = digitalRead (down2);
  if (tang1 == 0) {
    dem1++;
    delay(100);
    }
  if (giam1 == 0) {
    dem1--;
    delay(100);
  }
   if (tang2 == 0) {
    dem2++;
    delay(100);
    }
  if (giam2 == 0) {
    dem2--;
    delay(100);
  }
  lcd.setCursor(14,0);                            
  lcd.print(dem1);
  lcd.setCursor(14,1);                            
  lcd.print("  ");
  lcd.setCursor(14,1);                            
  lcd.print(dem2);
  Serial.print(F("Trung1 "));
  Serial.println(dem1);
  Serial.print(F("Trung2 "));
  Serial.println(dem2);

  }
void capnhat(){
  int ent1 = digitalRead (enter1);
  int ent2 = digitalRead (enter2);
  if (ent1 == 0)  {
    set1 = 1;
  }
  if (set1 == 1) {
  Firebase.setInt(fbdb,"/Soluong1", dem1);
  Firebase.setInt(fbdb,"/Nhiet1", t1);
  Firebase.setInt(fbdb,"/Am1", h1);
  Firebase.setInt(fbdb,"/Ngay1", ngay);
  Firebase.setInt(fbdb,"/Soluong2", dem2);
  Firebase.setInt(fbdb,"/Nhiet2", t2);
  Firebase.setInt(fbdb,"/Am2", h2);
  Firebase.setInt(fbdb,"/Ngay2", ngay);
  }
  
}

void ngayap(){
  ngay++;
  }
