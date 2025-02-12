/*
This file declares the behavior of the HTTP server particularly, and contains all the handlers to react to requests.
*/

// Pre-declaring functions that we will need in this document, but are implemented later
void SendError (WiFiClient client, String errcode="400");
void SendReply(WiFiClient client, String content, String mimeType="text/html");


void HandleHttpClient(WiFiClient& client) {
/* Handles an HTTP client : retreives and processes the HTTP request that it sends.
  */

  Serial.println("Handling client");

  // Verify that the client is connected
  if (!client || !client.connected()) {
    Serial.println("Invalid client");
    return;
  }

  // Retreives the HTTP request
  Serial.println("Reading request...");
  HttpRequest request;
  if (!ReadHttpRequest(client, request)) {
    Serial.println("Couldn't read request");
    return;
  }
  Serial.println("Done");

  // React accordingly whether it is a GET or a PUT request
  String method = request.method;

  if (method == "GET") {
    HandleGetRequest(client, request);
  }
  else if (method == "PUT") {
    HandlePutRequest(client, request);
  }
}


void HandleGetRequest(WiFiClient& client, HttpRequest& request) {
/* Replies to a GET request. 
  Either sends the main HTML page, or a 404 error. 
  */
  Serial.println("Received HTTP GET request");
  
  String path = request.path;
  if (path == "/") {
    // The main HTML page was requested, send it
    Serial.print("Sending web page... ");
    SendReply(client, html_page);
    Serial.println("Done !");
  } else {
    // 404 Error
    Serial.println("Unknown path : " + path);
    SendError(client, "404");
  }
}


void HandlePutRequest(WiFiClient& client, HttpRequest& request) {
/* Manages a PUT request. 
  If it is a request to set the velocity of the robot, do it.
*/
  Serial.println("Received HTTP PUT request");

  // Checking that the right path was given
  String path = request.path;
  if (path != "/api/set_movement") {
    Serial.println("Unknown path : " + path);
    SendError(client, "404");
    return;
  }

  Serial.print("Movement set by remote : ");

  // Read the data provided
  JsonDocument json;
  DeserializationError error = deserializeJson(json, request.content);

  if (error) {
    Serial.println("\nCouldn't read json : \"" + String(request.content) + "\"");
    SendError(client, "500");
    return;
  }
  int forward = json["forward"];
  int right = json["right"];
  Serial.println("(" + String(forward) + ", " + String(right) + ")");

  // Apply the movement to the robot
  SetRobotVelocity(forward, right);

  // Send a "success" reply with no content
  SendReply(client, "");
}