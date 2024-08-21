#include "WiFiEsp.h"
#include <Servo.h>
#define Relay 10
char ssid[] = "vegetables";            // your network SSID (name)
char pass[] = "CYR1128!cyr";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
WiFiEspServer server(80);
int Relay_value = 0;

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, LOW);
  WiFi.init(&Serial2);
  WiFi.begin(ssid, pass);  // 네트웍에 설정된 id와 패스워드로 접속을 합니다.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // 서버 시작
  server.begin();
  Serial.println("Server started");
  // 서버의(esp8266) IP 주소를 출력합니다
  Serial.println(WiFi.localIP());
}

void loop()
{
  // 클라이언트가 접속하는지 체크 합니다
  WiFiEspClient client = server.available();
  if (!client) {
    return;
  }
  // 클라이언트가 데이터를 보낼 때까지 기다립니다.
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }
  // 요청 첫 출을 읽어 옵니다.
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  // 요청 사항과 비교해봅니다.
  if (req.indexOf("/Relay/0") != -1) {
    Relay_value = 0;
  } else if (req.indexOf("/Relay/1") != -1) {
    Relay_value = 1;
  } else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  // 요청에 따라 셋팅합니다(ON 또는 OFF)
  digitalWrite(Relay, Relay_value);
  client.flush();
  // 응답을 준비합니다.
  String s1 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\nRelay is now ";
  s1 += (Relay_value) ? "HIGH" : "LOW"; //Relay_value 값에 따라 HIGH 또는 LOW 응답합니다.
  s1 += "\n";
  // 클라이언트로 응답을 보냅니다
  client.print(s1);
  delay(1);
  Serial.println("Client disonnected");
}
