#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>

#define PHOTO A0
#define Water_Pin A1
#define pinDHT11 12
#define Fan_Pin 32
#define Light_Pin 4
#define Servo_Pin 9

DHT dht(pinDHT11, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;
Servo servo;

String readString;
int Photo_Value = 0;
float temperature, humidity;
int cdc = 0;
int water_Content = 0;
int co2 = 0;
int Auto = 0;
char ch;

void setup() {
  pinMode(Fan_Pin, OUTPUT);
  pinMode(Light_Pin, OUTPUT);
  servo.attach(Servo_Pin);
  Serial.begin(9600);
  Serial1.begin(115200);
  Serial2.begin(9600);
  Serial3.begin(9600);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  lcd.clear();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("JNU SmartFarm");
  lcd.setCursor(0, 1);
  lcd.print("Vegetables Lab.");
  delay(5000);
}

void loop() {
  DateTime now = rtc.now();
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  double gamma = log(humidity / 100) + ((17.62 * temperature) / (243.5 + temperature));
  double dp = 243.5 * gamma / (17.62 - gamma);

  Photo_Value = analogRead(PHOTO);
  float pv_value = float(Photo_Value * 5) / 1023;
  float Rp = (10 * pv_value) / (5 - pv_value);
  float y = (log10(200 / Rp)) / 0.7;
  float Ea = pow(10, y);
  int water = analogRead(Water_Pin);
  int water_Content = map(water, 994, 424, 0, 100);

  if (water_Content >= 100) {
    water_Content = 100;
  }
  if (water_Content <= 0) {
    water_Content = 0;
  }

  Serial.print("{\"temp\":");
  Serial.print(temperature);
  Serial.print(",\"humidity\":");
  Serial.print(humidity);
  Serial.print(",\"cdc\":");
  Serial.print(Ea);
  Serial.print(",\"water\":");
  Serial.print(water_Content);
  Serial.print(",\"co2\":");
  Serial.print(co2);
  Serial.println("}");
  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" oC");
  lcd.setCursor(0, 1);
  lcd.print("R H: ");
  lcd.print(humidity);
  lcd.print(" %");
  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Photo: ");
  lcd.print(Ea);
  lcd.print(" lx");
  lcd.setCursor(0, 1);
  lcd.print("Water: ");
  lcd.print(water_Content);
  lcd.print(" %");
  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CO2: ");
  lcd.print(co2);
  lcd.print(" ppm");
  lcd.setCursor(0, 1);
  lcd.print("DewP: ");
  lcd.print(dp);
  lcd.print(" oC");
  delay(5000);

  if (Auto == 1) {
    if ((now.hour() >= 9) && (now.hour() <= 18)) {
      if (temperature >= 25) {
        digitalWrite(Fan_Pin, HIGH);
        servo.write(90);
      } else {
        digitalWrite(Fan_Pin, LOW);
        servo.write(0);
      }

      if (Ea <= 600) {
        digitalWrite(Light_Pin, HIGH);
      } else {
        digitalWrite(Light_Pin, LOW);
      }
    } else {
      digitalWrite(Fan_Pin, LOW);
      digitalWrite(Light_Pin, LOW);
      servo.write(0);
    }
  }
  if (Auto == 0) {
    digitalWrite(Fan_Pin, LOW);
    digitalWrite(Light_Pin, LOW);
    servo.write(0);
  }

  if (Serial.available() > 0) {
    delay(3);
    ch = Serial.read();
  }
  if (ch == 97) {
    Auto = 1;
  }
  if (ch == 98) {
    Auto = 0;
  }
  if (ch == 99) {
    digitalWrite(Fan_Pin, HIGH);
  }
  if (ch == 100) {
    digitalWrite(Fan_Pin, LOW);
  }
  if (ch == 101) {
    servo.write(90);
  }
  if (ch == 102) {
    servo.write(0);
  }
  if (ch == 48) {
    analogWrite(Light_Pin, 0);
  }
  if (ch == 49) {
    analogWrite(Light_Pin, 25);
  }
  if (ch == 50) {
    analogWrite(Light_Pin, 50);
  }
  if (ch == 51) {
    analogWrite(Light_Pin, 75);
  }
  if (ch == 52) {
    analogWrite(Light_Pin, 100);
  }
  if (ch == 53) {
    analogWrite(Light_Pin, 125);
  }
  if (ch == 54) {
    analogWrite(Light_Pin, 150);
  }
  if (ch == 55) {
    analogWrite(Light_Pin, 175);
  }
  if (ch == 56) {
    analogWrite(Light_Pin, 200);
  }
  if (ch == 57) {
    analogWrite(Light_Pin, 225);
  }
  
  Auto = Auto;
}
