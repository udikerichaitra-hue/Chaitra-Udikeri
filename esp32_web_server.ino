#include <WiFi.h>

const char* ssid = "Aura";
const char* password = "1234567890";

// Use GPIO 2 (Built-in LED on most ESP32 boards)
const int ledPin = 13;
const int BUZZER = 35;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(BUZZER, LOW);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("\nWiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
  Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("<h1>ESP32 CHAITRA U Web Server</h1>");

// LED Controls
client.print("<h3>ledPin Control</h3>");
client.print("<p>");
client.print("<a href=\"/LED_ON\"><Checkbox style='height:50px;width:100px'>LED ON</Checkbox></a>");
client.print("<a href=\"/LED_OFF\"><Checkbox style='height:50px;width:100px'>LED OFF</Checkbox></a>");
client.print("</p>");

// Buzzer Controls
client.print("<h3>BUZZER Control</h3>");
client.print("<p>");
client.print("<a href=\"/BUZZ_ON\"><Checkbox style='height:50px;width:100px'>BUZZ ON</Checkbox></a>");
client.print("<a href=\"/BUZZ_OFF\"><Checkbox style='height:50px;width:100px'>BUZZ OFF</Checkbox></a>");
client.print("</p>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        // LED control
if (currentLine.endsWith("GET /LED_ON")) {
  digitalWrite(ledPin, HIGH);
}
if (currentLine.endsWith("GET /LED_OFF")) {
  digitalWrite(ledPin, LOW);
}

// Buzzer control
if (currentLine.endsWith("GET /BUZZ_ON")) {
  digitalWrite(BUZZER, HIGH);
}
if (currentLine.endsWith("GET /BUZZ_OFF")) {
  digitalWrite(BUZZER, LOW);
}
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}