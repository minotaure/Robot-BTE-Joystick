/*
  Logiciel embarqué du robot de formation Minotaure.
  Crée un point d'accès wifi et héberge un serveur http qui contrôle les roues.
  Conçu pour Arduino Nano RP2040 Connect.

  Auteur : Léopold Gravier - Assocation Minotaure - Mines Paris PSL

  Crée à partir du code de : Rémy Martinez - Pole Mélia Cahier Vert - Mines ParisTech 2022
  Pour le robot Mélia (https://github.com/minotaure/Robot-BTE-Joystick)  

  Code modifié par Romain TEISSANDIER pour un autre atelier Cahier Vert
*/


// ------- Libraries -------
#include <WiFiNINA.h> // Library for wifi communication
#include <SPI.h>
#include <ArduinoJson.h>    // Library to read JSON documents

#include "HttpRequest.h"    // Code to parse HTTP requests

// Data
#include "main_html.h"       // The web page data
#include "arduino_secrets.h" // Contains SSID and Password for the wifi


// ------- Variables -------

// --- WiFi ---
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key index number (needed only for WEP)

int wifi_status = WL_IDLE_STATUS;

// --- Web server ---
WiFiServer http_server(80);
char html_page[] = HTML_CODE;

// --- Pins ---
// Left motor
const int MOTOR_PIN_LEFT_PLUS = 9;
const int MOTOR_PIN_LEFT_MINUS = 8;
// Right motor
const int MOTOR_PIN_RIGHT_PLUS = 7;
const int MOTOR_PIN_RIGHT_MINUS = 6;

// Nombre d'exécutions d'une boucle

int execution_count = 0;


// ------- Main functions -------

// Called once when the program starts
void setup() {
  // --- Partie LED ---
  SetupLed();
  SetLedColor(255, 255, 0); // Set the embedded LED to yellow to indicate that the robot is booting up

  // --- Partie USB ---
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  
  // Wait up to 5s for the connection to establish (it is important if we want to receive the first messages)
  unsigned long timeout_date = millis() + 5000;
  while (!Serial && millis() < timeout_date) {
    delay(10);
  }

  // --- Partie Wifi ---
  
  Serial.println("Creating access Point Web Server");

  // Start the WiFi access point
  SetupWiFi();
  // Start the web server on port 80
  http_server.begin();

  PrintWiFiInfo();

  SetLedColor(0, 0, 255); // Set the embedded LED to blue to indicate that the robot is waiting for a connection

  // --- Partie Robot ---
  pinMode(MOTOR_PIN_LEFT_PLUS, OUTPUT);
  pinMode(MOTOR_PIN_LEFT_MINUS, OUTPUT);
  pinMode(MOTOR_PIN_RIGHT_PLUS, OUTPUT);
  pinMode(MOTOR_PIN_RIGHT_MINUS, OUTPUT);

  analogWrite(MOTOR_PIN_LEFT_PLUS, 0);
  analogWrite(MOTOR_PIN_LEFT_MINUS, 0);
  analogWrite(MOTOR_PIN_RIGHT_PLUS, 0);
  analogWrite(MOTOR_PIN_RIGHT_MINUS, 0);
}


// Called repeatedly
void loop() {

  if (execution_count == 0){
    // Exercice 1

    // SetMotorsVelocity(255, 255);  // On fait tourner les moteurs à pleine puissance
    // delay(10000);  // On attend 10 sec
    // SetMotorsVelocity(0, 0);  // On arrête les moteurs
    // execution_count = 1;

    // Exercice 2

    // **** à compléter ****

    auto v =0.9/2;
    auto b = v/0.0024;

    // ****

    SetMotorsVelocity(b, b);
    delay(8000);
    SetMotorsVelocity(0, 0);
    execution_count = 1;

    // Exercice 3

    // for (int i = 0; i < 1000; i++){
    //   int t = i * 10;  // conversion pour avoir un temps en ms

      // **** à compléter ****

      // auto v =
      // auto b =

      // ****

      // On vérifie que la vitesse de rotation 
      
      // if (b > 255){
      //   b = 255;
      // }

      // SetMotorsVelocity(b, b);
      // delay(10);
    // }

    SetMotorsVelocity(0, 0);
    execution_count = 1;
  }
}


// ------- Movement functions -------

void SetRobotVelocity (int forward, int right) {
/* Set the velocity of the robot
  -255 <= forward <= 255   ->   Velocity on the forward/backward axis
  -255 <= right <= 255     ->   Velocity of the left/right axis
  */

  int leftMotorRaw = forward + right;
  int rightMotorRaw = forward - right;

  int maxMotorInput = max(abs(leftMotorRaw), abs(rightMotorRaw));
  float normFact = min(1.0, 255.0 / maxMotorInput);

  SetMotorsVelocity(leftMotorRaw * normFact, rightMotorRaw * normFact);
}


void SetMotorsVelocity (int _left, int _right) {
/* Set the velocity of the two motors
  -255 <= left <= 255    ->   Velocity of the left motor
  -255 <= right <= 255   ->  Rotation speed
  */

  Serial.println("(" + String(_left) + ", " + String(_right) + ")");

  int left = -_left;
  int right = -_right;

  // Manage left motor
  if (left >= 0) {
    analogWrite(MOTOR_PIN_LEFT_PLUS, left);
    analogWrite(MOTOR_PIN_LEFT_MINUS, 0);
  } else {
    analogWrite(MOTOR_PIN_LEFT_PLUS, 0);
    analogWrite(MOTOR_PIN_LEFT_MINUS, -left);
  }

  // Manage right motor
  if (right >= 0) {
    analogWrite(MOTOR_PIN_RIGHT_PLUS, right);
    analogWrite(MOTOR_PIN_RIGHT_MINUS, 0);
  } else {
    analogWrite(MOTOR_PIN_RIGHT_PLUS, 0);
    analogWrite(MOTOR_PIN_RIGHT_MINUS, -right);
  }
}

