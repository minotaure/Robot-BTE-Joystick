/*
  Rémy Martinez - Pole Mélia Cahier Vert - Mines ParisTech 2022
*/
///////////////////////////////////////////////////////////////////////////////////////////////////Partie Wifi
#include <SPI.h>
#include <WiFiNINA.h> //Library for wifi communication
#include "arduino_secrets.h" //Contains SSID and Password
#include "web_interface_joystick.h" //Le code correspondant au site internet
#include <Wire.h>
#include "defines.h"

#include <WebSockets2_Generic.h>

using namespace websockets2_generic;

WebsocketsServer websocket_server;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
String html = HTML_CODE;
int keyIndex = 0;                // your network key index number (needed only for WEP)

int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);


void heartBeatPrint(void)
{
  static int num = 1;

  if (WiFi.status() == WL_CONNECTED)
    Serial.print("H");        // H means server WiFi connected
  else  
    Serial.print("F");        // F means server WiFi not connected
    
  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(" ");
  }
}

void check_status()
{
  static unsigned long checkstatus_timeout = 0;

  //KH
#define HEARTBEAT_INTERVAL    10000L
  // Print hearbeat every HEARTBEAT_INTERVAL (10) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = millis() + HEARTBEAT_INTERVAL;
  }
}

void setup() {
  ////////////////////////////////////////////////////////////////Partie Wifi
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial);
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
  ///////////////////////////////////////////////////////////////////////Partie Socket
  Serial.println("\nStarting RP2040-Server with WiFiNINA on " + String(BOARD_NAME));
  Serial.println(WEBSOCKETS2_GENERIC_VERSION);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) 
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    return;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) 
  {
    Serial.println("Please upgrade the firmware");
  }

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);

  // Wait some time to connect to wifi
  for (int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++)
  {
    Serial.print(".");
    delay(1000);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nWiFi connected");
  }
  else
  {
    Serial.println("\nNo WiFi");
    return;
  }

  websocket_server.listen(WEBSOCKETS_PORT);
  
  Serial.print(websocket_server.available() ? "WebSockets Server Running and Ready on " : "Server Not Running on ");
  Serial.println(BOARD_NAME);
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());     //You can get IP address assigned to SAMD
  Serial.print(", Port: ");
  Serial.println(WEBSOCKETS_PORT);    // Websockets Server Port
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
            int len = html.length()/1000;
            for(int i =0;i<=len;i++){
              String bloc = html.substring(1000*i,1000*(i+1)); // Limite de taille de ce qu'on peut envoyer donc on découpe en blocs de 1000 caracteres
              client.print(bloc);
            }
            

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
        check_status();
  
          WebsocketsClient socket_client = websocket_server.accept();
         
          if (client.available())
          {
            WebsocketsMessage msg = socket_client.readNonBlocking();
        
            // log
            Serial.print("Got Message: ");
            Serial.println(msg.data());
        
            // return echo
            socket_client.send("Echo: " + msg.data());
        
            // close the connection
            socket_client.close();
          }
        
          delay(1000);
        
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
