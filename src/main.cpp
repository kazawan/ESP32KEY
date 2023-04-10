#include <Arduino.h>
#include <WiFi.h>
#include <webserver.h>

#define led 2


const char *ssid = "K'S_HOME";
const char *password = "00189423";


WebServer server(8000);

void handleRoot() {
  server.send(200, "text/plain", "hello from esp32!");
}

void postHandler() {
  String message;
  if (server.hasArg("plain")) {
    message = server.arg("plain");
    if(message == "on") {
      digitalWrite(led, HIGH);
    } else if(message == "off") {
      digitalWrite(led, LOW);
    }
  } else {
    message = "No message sent";
    digitalWrite(led, LOW);
  }
  server.send(200, "text/plain", "Hello, POST: " + message);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/led",HTTP_POST, postHandler);
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  
}