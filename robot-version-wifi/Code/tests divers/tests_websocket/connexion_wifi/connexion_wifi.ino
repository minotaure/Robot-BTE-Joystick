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
#include <WiFiNINA.h> //Library for wifi communication*
#include <Wire.h>

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "RobotMelia";        // your network SSID (name)
char pass[] = "C4hierV3rt";    // your network password (use for WPA, or use as key for WEP)

int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

///////////////////////////////////////////////////////////////////////////////////////////////////Partie Robot


void setup() {
  ////////////////////////////////////////////////////////////////Partie Wifi
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while(!Serial);
  
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
  // you can override it with the following:
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
    Wire.begin(); // join i2c bus (address optional for master)
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
