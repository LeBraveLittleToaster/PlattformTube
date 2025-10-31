#ifndef HTTPS_AUTH_SERVER_H
#define HTTPS_AUTH_SERVER_H


// WiFi
#include <WiFi.h>

// Middleware
#include <functional>

// Internal headers used to carry auth data post-authentication
#define HEADER_USERNAME "X-USERNAME"
#define HEADER_GROUP    "X-GROUP"

// HTTPS server includes
#include <HTTPSServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>

using namespace httpsserver;

class HttpsAuthServer {
public:
  HttpsAuthServer();
  ~HttpsAuthServer();

  void begin(const char* ssid, const char* password);

  void loop();

  HttpsAuthServer(const HttpsAuthServer&) = delete;
  HttpsAuthServer& operator=(const HttpsAuthServer&) = delete;

private:
  // Handlers
  static void handleRoot(HTTPRequest * req, HTTPResponse * res);
  static void handleInternalPage(HTTPRequest * req, HTTPResponse * res);
  static void handleAdminPage(HTTPRequest * req, HTTPResponse * res);
  static void handlePublicPage(HTTPRequest * req, HTTPResponse * res);
  static void handle404(HTTPRequest * req, HTTPResponse * res);

  // Middleware
  static void middlewareAuthentication(HTTPRequest * req, HTTPResponse * res, std::function<void()> next);
  static void middlewareAuthorization(HTTPRequest * req, HTTPResponse * res, std::function<void()> next);

  // Internals
  static SSLCert*       s_cert;
  static HTTPSServer*   s_server;
  static bool           s_started;

  // Setup helpers
  static void setupRoutes();
  static void setupMiddleware();
  static void startServer();
};

#endif // HTTPS_AUTH_SERVER_H
