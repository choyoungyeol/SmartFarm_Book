#include <dht.h>
#define DHT11_PIN 12

dht DHT;

void setup() {
  Serial.begin(9600);
}

void loop() {
  float chk = DHT.read11(DHT11_PIN);
  Serial.print("{\"temp\":");
  Serial.print(DHT.temperature);
  Serial.print(",\"humidity\":");
  Serial.print(DHT.humidity);
  Serial.println("}");
  delay(5000);
}
