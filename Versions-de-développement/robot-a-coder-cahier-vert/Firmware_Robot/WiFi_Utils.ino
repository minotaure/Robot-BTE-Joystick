/*
Usefull functions to manage the WiFi connection
*/

void SetupWiFi() {
/* Setups the WiFi access point
  */

  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // Check for update
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // By default the local IP address will be 192.168.4.1
  // You can override it with the following:
  WiFi.config(IPAddress(192, 168, 2, 2));

  // Print the network name (SSID);
  Serial.print("Creating access point named : ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  wifi_status = WiFi.beginAP(ssid, pass);
  if (wifi_status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }
}


void PrintWiFiInfo() {
/* Displays informations about the WiFi access point
  */
  
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


void CheckWiFiClientConnected() {
/* Displays a message when the clients are connected or disconnected.
  */

  // compare the previous status to the current status
  if (wifi_status != WiFi.status()) {
    // it has changed update the variable
    wifi_status = WiFi.status();

    if (wifi_status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
      SetLedColor(0, 255, 0); // Set the embedded LED to green to indicate that a device is connected
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
      SetLedColor(0, 0, 255); // Set the embedded LED to blue to indicate that the robot is waiting for a connection
    }
  }
}