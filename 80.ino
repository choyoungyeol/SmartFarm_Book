#include <SPI.h>
#include <Ethernet.h>
#include <dht.h>
#define DHT22_PIN 7

dht DHT;

//Ethernet Shield MAC address
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress dnServer(000, 000, 000, 000);
IPAddress gateway(000, 000, 000, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip(000, 000, 000, 000);            //IP 주소
EthernetServer server(80);

float temperature;
float After_temperature;
float humidity;
float After_humidity;

String readString;

void setup() {
  Serial.begin(9600);
  Serial.println("Sensor Ready");
  Serial.println("Trying to get an IP address using DHCP");
  Ethernet.begin(mac, ip, dnServer, gateway, subnet);
  server.begin();
}

void loop() {
  float chk = DHT.read22(DHT22_PIN);
  temperature = DHT.temperature;
  humidity = DHT.humidity;

  if ((temperature <= -10) || (humidity > 100)) {
    temperature = After_temperature;
    humidity = After_humidity;
  }
  After_temperature = temperature;
  After_humidity = humidity;
  readString = "";
  String data = "$" + String(temperature) + "/" + String(humidity) + "$";
  Serial.println(data);

  while (Serial.available()) {
    delay(3);
    char c = Serial.read();
    readString += c;
  }
  delay(1000);

  EthernetClient client = server.available();
  if (client.available()) {
    char c = client.read();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connnection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<center>");
    client.print("<br><br><br>");
    client.print("<font size=3 color=black> <b>");
    client.print("Greenhouse Environment Condition<br>");
    client.print("<br>");
    client.print("<font size=2 color=green> <b>");
    client.print("Temperature = ");
    client.print(temperature, 1);
    client.print(" oC");
    client.print("&nbsp;");
    client.print("&nbsp;");
    client.print("&nbsp;");
    client.print("<font size=2 color=green> <b>");
    client.print("Relative Humidity = ");
    client.print(humidity, 1);
    client.print(" %");
    client.print("<br><br><br>");
    client.println("<font size=2 color=red><b>");
    client.println("Set Temperature : ");
    client.print("24");
    client.print(" oC");
    client.print("&nbsp;");
    client.print("&nbsp;");
    client.print("&nbsp;");
    client.println("Set Relative Humidity : ");
    client.print("40");
    client.print(" %");
    client.print("<br>");
    client.println("</body>");
    client.println("</html>");
    delay(1);
    client.stop();
  }
}
