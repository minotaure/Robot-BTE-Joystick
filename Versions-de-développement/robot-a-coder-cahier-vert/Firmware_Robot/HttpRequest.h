#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

class HttpRequest {
  public :
    String method;
    String path;

    bool has_content;

    String content_type;
    size_t content_length;
    String content;

    HttpRequest() {}
  
  private:
};

#endif