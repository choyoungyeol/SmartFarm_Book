#include "WiFiEsp.h"
#include <dht.h>
#define DHT22_PIN 7
dht DHT;
float temperature ;
float humidity ;
char ssid[] = "ID*******";            // your network SSID (name)
char pass[] = "Password**";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 1;
WiFiEspServer server(80);

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  WiFi.init(&Serial2);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }
  Serial.println("You're connected to the network");
  printWifiStatus();

  server.begin();
}

void loop()
{
  float chk = DHT.read22(DHT22_PIN);
  temperature = DHT.temperature;         //온도
  humidity = DHT.humidity;                //상대습도

  WiFiEspClient client = server.available();
  if (client) {
    Serial.println("New client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          Serial.println("Sending response");
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "Refresh: 20\r\n"
            "\r\n");
          client.print("<!DOCTYPE HTML>\r\n");
          client.print("<html>\r\n");
          client.print("<h1>Temperature and Humidity</h1>\r\n");
          client.print("Temperature : ");
          client.print(temperature);
          client.print(" oC");
          client.print("<br>\r\n");
          client.print("Humidity : ");
          client.print(humidity);
          client.print(" %");
          client.print("<br>\r\n");
          client.print("</html>\r\n");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(10);
    //client.stop();
    Serial.println("Client disconnected");
  }
  delay(1500);
  Serial2.print(temperature);
  Serial2.print(", ");
  Serial2.println(humidity);
}

void printWifiStatus()
{
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
