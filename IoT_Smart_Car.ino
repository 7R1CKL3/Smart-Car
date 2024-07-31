#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

#define ENA 4 // Enable/speed motors Right GPIO4(D2)
#define ENB 14 // Enable/speed motors Left GPIO12(D6)
#define IN_1 16 // L298N in1 motors Right GPIO15(D8)
#define IN_2 5 // L298N in2 motors Right GPIO13(D7)
#define IN_3 0 // L298N in3 motors Left GPIO2(D4)
#define IN_4 2 // L298N in4 motors Left GPIO0(D3)
#define steering 1
#define PIR 15

int pos;

Servo myservo;

String command; //String to store app command state.
int speedCar = 150; // 400 - 1023.
int speed_Coeff = 3;

const char* ssid = "NodeMCU Car";
ESP8266WebServer server(80);

void setup() {

Serial.begin(115200);
pinMode(ENA, OUTPUT);
pinMode(ENB, OUTPUT);
pinMode(IN_1, OUTPUT);
pinMode(IN_2, OUTPUT);
pinMode(IN_3, OUTPUT);
pinMode(IN_4, OUTPUT);
myservo.attach(steering);
pinMode(PIR, INPUT);
myservo.write(0);

// Connecting WiFi

WiFi.mode(WIFI_AP);
WiFi.softAP(ssid);

IPAddress myIP = WiFi.softAPIP();
Serial.print("AP IP address: ");
Serial.println(myIP);

// Starting WEB-server
server.on("/", HTTP_handleRoot);
server.onNotFound(HTTP_handleRoot);
server.begin();
}

void goAhead() {

digitalWrite(IN_1, HIGH);
digitalWrite(IN_2, LOW);
analogWrite(ENA, speedCar);
digitalWrite(IN_3, LOW);
digitalWrite(IN_4, HIGH);
analogWrite(ENB, speedCar);

}

void goBack() {

digitalWrite(IN_1, LOW);
digitalWrite(IN_2, HIGH);
analogWrite(ENA, speedCar);
digitalWrite(IN_3, HIGH);
digitalWrite(IN_4, LOW);
analogWrite(ENB, speedCar);
}

void goRight() {
pos = pos + 5;
myservo.write(pos);

}

void goLeft() {
pos = pos - 5;
myservo.write(pos);
}

void stopRobot() {digitalWrite(IN_1, LOW);
digitalWrite(IN_2, LOW);
analogWrite(ENA, speedCar);

digitalWrite(IN_3, LOW);
digitalWrite(IN_4, LOW);
analogWrite(ENB, speedCar);
}

void loop() {
server.handleClient();
command = server.arg("State");

if (command == "B") goBack();
else if (command == "L") goLeft();
else if (command == "R") goRight();
else if (command == "S") stopRobot();
else if (digitalRead(PIR) == 1) {
if (command == "F") goAhead();

}
}

void HTTP_handleRoot(void) {

if (server.hasArg("State")) {
Serial.println(server.arg("State"));
}
server.send(200, "text/html", "");
delay(1);
}