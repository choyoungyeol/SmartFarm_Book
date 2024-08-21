#include <OneWire.h>
#include <DallasTemperature.h>
#include <PID_v1.h>
#define ONE_WIRE_BUS 2
#define Left_Open_Window 4
#define Left_Close_Window 5
#define Right_Open_Window 6
#define Right_Close_Window 7

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

uint8_t sensor1[8] = { 0x28, 0xFF, 0x6E, 0x73, 0x71, 0x17, 0x04, 0x7C };
uint8_t sensor2[8] = { 0x28, 0xFF, 0x2B, 0xFB, 0x71, 0x17, 0x04, 0x82 };

float Dry_Temp;
float Wet_Temp;

double Setpoint;
double Input;
double Output;

double aggKp = 2, aggKi = 0.5, aggKd = 1;
double consKp = 1, consKi = 0.05, consKd = 0.25;
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

void setup(void)
{
  pinMode(Left_Open_Window, OUTPUT);
  pinMode(Left_Close_Window, OUTPUT);
  pinMode(Right_Open_Window, OUTPUT);
  pinMode(Right_Close_Window, OUTPUT);

  Serial.begin(9600);
  sensors.begin();

  Setpoint = 1.0;

  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-5, 5);
}

void loop(void)
{
  float Patm = 101.325;   //기압 kPa
  sensors.requestTemperatures();
  Wet_Temp = sensors.getTempC(sensor1);
  Dry_Temp = sensors.getTempC(sensor2);
  float Psdt = 0.61078 * exp((17.27 * Dry_Temp) / (237.3 + Dry_Temp)); //kPa
  float Pswt = 0.61078 * exp((17.27 * Wet_Temp) / (237.3 + Wet_Temp)); //kPa
  float Pv = Pswt - Patm * 0.000662 * (Dry_Temp - Wet_Temp);
  float Humidity = 100 * Pv / Psdt;
  float VPD = Psdt * (1 - Humidity / 100);
  float Dew_Temp = (Dry_Temp - (100 - Humidity) / 5);
  float AH = 2.16679 * Pv / (273.15 + Dry_Temp);

  Input = VPD;

  double gap = abs(Setpoint - Input); //distance away from setpoint
  if (gap < 0.5)
  { //we're close to setpoint, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
    digitalWrite(Left_Open_Window, HIGH);
    digitalWrite(Left_Close_Window, LOW);
    digitalWrite(Right_Open_Window, HIGH);
    digitalWrite(Right_Close_Window, LOW);
  }
  else
  {
    //we're far from setpoint, use aggressive tuning parameters
    myPID.SetTunings(aggKp, aggKi, aggKd);
    digitalWrite(Left_Open_Window, LOW);
    digitalWrite(Left_Close_Window, HIGH);
    digitalWrite(Right_Open_Window, LOW);
    digitalWrite(Right_Close_Window, HIGH);
  }
  myPID.Compute();
  Serial.println(Input);
  Serial.println(Output);
  Serial.println(gap);
}
