/*

  Créé à partir de : "  Commande REGO (Robot de téléprésence)
  by Franck DE WIT <http://www.lelabtechno.com>
  Twitter : @fdw34
  Programme pour Arduino nano RP2040 Connect
  27/08/2021"

  Rémy Martinez - Pole Mélia Cahier Vert - Mines ParisTech 2022
*/
///////////////////////////////////////////////////////////////////////////////////////////////////Partie Wifi
#include <SPI.h>
#include <WiFiNINA.h> //Library for wifi communication
#include "arduino_secrets.h" //Contains SSID and Password
#include "web_interface.h" //Le code correspondant au site internet
#include <Wire.h>

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
char html[] = HTML_CODE;
int keyIndex = 0;                // your network key index number (needed only for WEP)

int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

///////////////////////////////////////////////////////////////////////////////////////////////////Partie Robot

#define A1 9  // Motor A pins
#define A2 8
#define B1 7 // Motor B pins
#define B2 6


void Avant();
void Recule();
void Gauche();
void Droite();
void stopMotor();

void setup() {
  ////////////////////////////////////////////////////////////////Partie Wifi
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  Serial.println("Access Point Web Server");

  pinMode(led, OUTPUT);      // set the LED pin mode

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // by default the local IP address will be 192.168.4.1
  // you can override it with the fol0ing:
  WiFi.config(IPAddress(192, 168, 2, 2));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }
  {
    // wait 10 seconds for connection:
    delay(10000);

    // start the web server on port 80
    server.begin();

    // you're connected now, so print out the status
    void printWiFiStatus();
  }
  ///////////////////////////////////////////////////////////////////////Partie Robot
  {
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);

  analogWrite(A1, 0);
  analogWrite(A2, 0);
  analogWrite(B1, 0);
  analogWrite(B2, 0);
  }
}


void loop() {
  ///////////////////////////////////////////////////////////////////////////////////////////Partie Wifi
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      delayMicroseconds(30);                // This is required for the Arduino Nano RP2040 Connect - otherwise it will loop so fast that SPI will never be served.
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print(html);




            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /A" or R or D or G or S:
        if (currentLine.endsWith("GET /A")) {
          Avant();               // GET /A Avance
        }
        if (currentLine.endsWith("GET /R")) {
          Recule();                // GET /R Recule
        }
        if (currentLine.endsWith("GET /D")) {
          Droite();                // GET /D Droite
        }


        if (currentLine.endsWith("GET /G")) {
          Gauche();                // GET /G Gauche
        }


        if (currentLine.endsWith("GET /S")) {
          stopMotor();                // GET /S Stop
        }
      }
    }

    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////Partie Wifi
void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}

////////////////////////////////////////////////////////////////////////////////////////////////////Partie Robot
void Gauche()
{ // Function tourne à gauche
  Serial.println("Gauche");
  analogWrite(A1, 0);
  analogWrite(A2, 255);
  analogWrite(B1, 255);
  analogWrite(B2, 0);
}

void Droite()
{ // Function tourne à droite
  Serial.println("Droite");
    analogWrite(A1, 255);
  analogWrite(A2, 0);
  analogWrite(B1, 0);
  analogWrite(B2, 255);
}

void Avant()
{ // Function Avance
  Serial.println("Avance");
    analogWrite(A1, 255);
  analogWrite(A2, 0);
  analogWrite(B1, 255);
  analogWrite(B2, 0);
}

void Recule()                              // Function Recule
{
  Serial.println("Recule");
    analogWrite(A1, 0);
  analogWrite(A2, 210);
  analogWrite(B1, 0);
  analogWrite(B2, 210);
}

void stopMotor()                         // Function to stop
{
  Serial.println("Arret des moteurs");
    analogWrite(A1, 0);
  analogWrite(A2, 0);
  analogWrite(B1, 0);
  analogWrite(B2, 0);
}
