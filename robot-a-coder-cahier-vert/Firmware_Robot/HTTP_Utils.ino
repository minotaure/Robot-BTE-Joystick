/*
Usefull functions for HTTP communication
*/


void SendError (WiFiClient client, String errcode) {
/* Sends an error message to a given client.
  Note : errcode is defaulted to "400" in HTTP_Server.ino
  */
  
  client.println("HTTP/1.1 " + errcode);
  client.println();
  client.flush();
}


#define PACKET_SIZE 2048
void SendReply(WiFiClient client, String content, String mimeType) {
/* Replies to the request of a client.

  content -> The payload to deliver in the reply
  mimeType -> The type of the payload sent, roughly correlates to the extension of the file (ex : .html file => text/html MIME type)
  (See https://developer.mozilla.org/fr/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types for a more detailed list)
  Note : mimeType is defaulted to "text/html" in HTTP_Server.ino
  */

  // Send header
  client.println("HTTP/1.1 200");
  client.println("Content-Type: " + mimeType);
  client.println("Content-Length: " + String(content.length()));
  client.println();

  // Send payload
  // As a data packet sent via WiFi cannot excede 2048 bytes long, the content is split into multiple packets
  unsigned int n_packet = ceil((float)content.length() / PACKET_SIZE);
  for (unsigned int i=0; i < n_packet; i++) {
    unsigned int from = i * PACKET_SIZE;
    unsigned int to = min(content.length(), (i+1)*PACKET_SIZE);
    String slice = content.substring(from, to);
    client.print(slice);
  }
  client.flush();
}


String ReadLine(WiFiClient& client, unsigned long timeout_date) {
  String line = "";

  while (millis() < timeout_date && !line.endsWith("\n"))
  {
    if (!client.available()) {
      continue;
    }
    char c = client.read();
    line += c;
  }

  line.replace("\r\n", "");

  return line;
}


void ReadContent(WiFiClient& client, unsigned long timeout_date, HttpRequest& request) {
  String content = "";
  
  content.reserve(request.content_length);

  size_t i=0;
  while ((millis() < timeout_date) && (i<request.content_length)) {
    if (!client.available()) {
      continue;
    }

    content += (char)client.read();
    i++;
  }

  request.content = content;
}


bool ParseFirstLine(const String& firstLine, HttpRequest& request) {
  int firstSpaceId = firstLine.indexOf(" ");
  if (firstSpaceId == -1) {
    return false;
  }

  request.method = firstLine.substring(0, firstSpaceId);

  int secondSpaceId = firstLine.indexOf(" ", firstSpaceId+1);
  if (secondSpaceId == -1) {
    return false;
  }
  request.path = firstLine.substring(firstSpaceId+1, secondSpaceId);

  return true;
}


String GetAttributeValue(String& attributeRawStr) {
  int separatorIndex = attributeRawStr.indexOf(":");

  String attributeValue = attributeRawStr.substring(separatorIndex + 1);
  attributeValue.trim();

  return attributeValue;
}


#define HTTP_MAX_RESPONSE_MILLIS 200
bool ReadHttpRequest(WiFiClient& client, HttpRequest& request) {
  unsigned long timeout_date = millis() + HTTP_MAX_RESPONSE_MILLIS;

  request = HttpRequest();

  String firstLine = ReadLine(client, timeout_date);
  if (!ParseFirstLine(firstLine, request)) {
    Serial.println("Couldn't parse first line");
    return false;
  }

  bool length_declared = false;
  bool type_declared = false;

  String line;
  do {
    line = ReadLine(client, timeout_date);
    String line_lower = line;
    line_lower.toLowerCase();
    
    if (line_lower.startsWith("content-type")) { 
      request.content_type = GetAttributeValue(line);
      type_declared = true;
    }
    
    if (line_lower.startsWith("content-length")) {
      request.content_length = GetAttributeValue(line).toInt();
      length_declared = true;
    }

  } while (line.length() != 0);

  request.has_content = type_declared && length_declared;

  if (request.has_content) {
    ReadContent(client, timeout_date, request);
  }

  if (millis() > timeout_date) {
    Serial.println("Connection timed out");
    return false;
  }
  
  return true;
}
