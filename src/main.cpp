#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Airtel_My_Network";
const char* password = "9910357357";

WebServer server(80);

const int led1 = 4;
const int led2 = 5;

String output1state = "OFF";
String output2state = "OFF";

// Forward declaration
void handleroot();

void handleGPIO1OFF() {
  digitalWrite(led1, LOW);
  output1state = "OFF";
  handleroot();
}

void handleGPIO1ON() {
  digitalWrite(led1, HIGH);
  output1state = "ON";
  handleroot();
}

void handleGPIO2OFF() {
  digitalWrite(led2, LOW);
  output2state = "OFF";
  handleroot();
}

void handleGPIO2ON() {
  digitalWrite(led2, HIGH);
  output2state = "ON";
  handleroot();
}

v8oid handleroot() {
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>";
  html += "html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; }";
  html += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; }";
  html += ".button2 { background-color: #555555; }";
  html += "</style></head>";
  html += "<body><h1>ESP32 Web Server</h1>";

  // First LED control
  html += "<p>First LED - State " + output1state + "</p>";
  if (output1state == "OFF") {
    html += "<p><a href=\"/1/on\"><button class=\"button\">ON</button></a></p>";
  } else {
    html += "<p><a href=\"/1/off\"><button class=\"button button2\">OFF</button></a></p>";
  }

  // Second LED control
  html += "<p>Second LED - State " + output2state + "</p>";
  if (output2state == "OFF") {
    html += "<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>";
  } else {
    html += "<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>";
  }

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleroot);
  server.on("/1/on", handleGPIO1ON);
  server.on("/1/off", handleGPIO1OFF);
  server.on("/2/on", handleGPIO2ON);
  server.on("/2/off", handleGPIO2OFF);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}