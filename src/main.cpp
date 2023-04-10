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
  String message = receiveMessage(); //接收到的内容
  if(!validMessage(message)) { //验证消息是否有效
    server.send(400, "text/plain", "Error: Invalid message");
    return;
  }
  handleValidMessage(message); //处理有效的消息
  server.send(200, "text/plain", "Hello, POST: " + message);
}

/**
 * 接收消息
 * @return 消息内容
 */
String receiveMessage() {
  return server.arg("message");
}

bool validMessage(String message) { // 定义一个函数，函数名为validMessage,参数为String类型的message
  if(message == "on" || message == "off") { // 如果message 等于on 或者 off 返回true
    return true;
  } else { // 否则返回false
    return false;
  }
}

void handleValidMessage(String message) {
  // 如果接收到了“on”消息，则打开LED灯
  if(message == "on") {
    digitalWrite(led, HIGH);
  } else if(message == "off") {
    digitalWrite(led, LOW);
  }
}

// 接收消息
String receiveMessage() {
  if (server.hasArg("plain")) {
    return server.arg("plain");
  } else {
    return "";
  }
}

void setup() {
  Serial.begin(115200); // 初始化串口
  pinMode(led, OUTPUT); // 设置led引脚为输出
  WiFi.begin(ssid, password);// 连接到WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP().toString());

  server.on("/",HTTP_GET, handleRoot);
  server.on("/led",HTTP_POST, postHandler);
}

void loop() {
  // 服务器处理客户端请求
  server.handleClient();

}