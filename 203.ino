#define Relay 10
String readString;

void setup() {
  Serial.begin(9600);
  pinMode(Relay, OUTPUT);
}

void loop() {
  readString = "";
  if (Serial.available()) {
    char c = Serial.read();
    readString += c;
    if (c == 'a') {
      digitalWrite(Relay, HIGH);
    }
    if (c == 'b') {
      digitalWrite(Relay, LOW);
    }
  }
  delay(2000);
}
