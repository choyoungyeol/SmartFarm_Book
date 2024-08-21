#include <SPI.h>
#include <WiFi.h>
#include <dht.h>
#define DHT22_PIN 8

dht DHT;
float temperature;
float After_temperature;
float humidity;
float After_humidity;
String readString;

char ssid[] = "000000";         //무선 ID 
char pass[] = "000000";                     //비밀번호
int keyIndex = 0;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }
  server.begin();
  printWifiStatus();
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
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 1");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<meta charset=utf-8/>");
          client.print("<meta name=view content=width=device-width, ");
          client.println("initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, user-scalable=no />");
          client.println("<html>");
          client.println("<head>");
          client.println("<title>Environment Condition</title>");
          client.println("</head>");
          client.println("<h1>Many Sensor</h1>");
          client.println("<div data-role=content>");
          client.print("Temperature = ");
          client.println(temperature);
          client.println("<br>");
          client.println("<br>");
          client.print("Relative humidity = ");
          client.println(humidity);
          client.println("<br>");
          client.println("</div>");
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("client disonnected");
  }
  readString = "";
  String data = "$" + String(temperature) + "/" + String(humidity) + "$";
  Serial.println(data);
  delay(2000);
}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
