#include "control/HttpsAuthServer.h"

#include "config/cert.h"
#include "config/private_key.h"
#include "config/HardwareConfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


// Static storage
SSLCert*       HttpsAuthServer::s_cert    = nullptr;
HTTPSServer*   HttpsAuthServer::s_server  = nullptr;
bool           HttpsAuthServer::s_started = false;

HttpsAuthServer::HttpsAuthServer() {}
HttpsAuthServer::~HttpsAuthServer() {
  if (s_server && s_started) {
    s_server->stop();
    s_started = false;
  }
}

void HttpsAuthServer::begin(const char* ssid, const char* password) {
  Serial.begin(115200);
  delay(50);

  // WiFi up (if not already)
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[HTTPS] Connecting WiFi...");
    WiFi.begin(ssid, password);
    uint32_t t0 = millis();
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
      if (millis() - t0 > 30000) { // 30s hard timeout
        Serial.println("\n[HTTPS] WiFi connect failed. Check credentials or signal.");
        break;
      }
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("\n[HTTPS] WiFi OK, IP=");
      Serial.println(WiFi.localIP());
    }
  } else {
    Serial.print("[HTTPS] WiFi already connected, IP=");
    Serial.println(WiFi.localIP());
  }

  if (!s_cert) {
    s_cert = new SSLCert(
      example_crt_DER, example_crt_DER_len,
      example_key_DER, example_key_DER_PRIVATE_len
    );
  }

  // Create server
  if (!s_server) {
    s_server = new HTTPSServer(s_cert);
  }

  setupRoutes();
  setupMiddleware();
  startServer();
}

void HttpsAuthServer::setupRoutes() {
  // Resource nodes
  ResourceNode * nodeRoot     = new ResourceNode("/",             "GET", &handleRoot);
  ResourceNode * nodeInternal = new ResourceNode("/internal",     "GET", &handleInternalPage);
  ResourceNode * nodeAdmin    = new ResourceNode("/internal/admin","GET", &handleAdminPage);
  ResourceNode * nodePublic   = new ResourceNode("/public",       "GET", &handlePublicPage);
  ResourceNode * node404      = new ResourceNode("",              "GET", &handle404);

  s_server->registerNode(nodeRoot);
  s_server->registerNode(nodeInternal);
  s_server->registerNode(nodeAdmin);
  s_server->registerNode(nodePublic);

  s_server->setDefaultNode(node404);
}

void HttpsAuthServer::setupMiddleware() {
  // Order matters: auth before authorization.
  s_server->addMiddleware(&middlewareAuthentication);
  s_server->addMiddleware(&middlewareAuthorization);
}

void HttpsAuthServer::startServer() {
  if (s_started) return;

  Serial.println("[HTTPS] Starting server...");
  s_server->start();
  s_started = s_server->isRunning();

  if (s_started) {
    Serial.println("[HTTPS] Server ready.");
  } else {
    Serial.println("[HTTPS] Server failed to start. Check cert/key and heap.");
  }
}

void HttpsAuthServer::loop() {
  if (s_server && s_started) {
    s_server->loop();
  }
  //delay(.01);
}

/* ===================== Middleware ===================== */

void HttpsAuthServer::middlewareAuthentication(HTTPRequest * req, HTTPResponse * res, std::function<void()> next) {
  // Nuke any client-provided internal headers — no auth bypasses on our watch.
  req->setHeader(HEADER_USERNAME, "");
  req->setHeader(HEADER_GROUP, "");

  // Parse Basic Auth credentials (empty strings if none/invalid)
  std::string reqUsername = req->getBasicAuthUser();
  std::string reqPassword = req->getBasicAuthPassword();

  if (reqUsername.length() > 0 && reqPassword.length() > 0) {
    bool authValid = true;
    std::string group = "";

    // Yes, hardcoded. This is a demo. Replace with real user storage if you care about security.
    if (reqUsername == "admin" && reqPassword == "secret") {
      group = "ADMIN";
    } else if (reqUsername == "user" && reqPassword == "test") {
      group = "USER";
    } else {
      authValid = false;
    }

    if (authValid) {
      req->setHeader(HEADER_USERNAME, reqUsername);
      req->setHeader(HEADER_GROUP,    group);
      next();
    } else {
      res->setStatusCode(401);
      res->setStatusText("Unauthorized");
      res->setHeader("Content-Type", "text/plain");
      res->setHeader("WWW-Authenticate", "Basic realm=\"ESP32 privileged area\"");
      res->println("401. Unauthorized (try admin/secret or user/test)");
      // Do NOT call next()
    }
  } else {
    // No attempt — pass through
    next();
  }
}

void HttpsAuthServer::middlewareAuthorization(HTTPRequest * req, HTTPResponse * res, std::function<void()> next) {
  std::string username = req->getHeader(HEADER_USERNAME);

  // Block /internal... when not logged in
  if (username == "" && req->getRequestString().substr(0, 9) == "/internal") {
    res->setStatusCode(401);
    res->setStatusText("Unauthorized");
    res->setHeader("Content-Type", "text/plain");
    res->setHeader("WWW-Authenticate", "Basic realm=\"ESP32 privileged area\"");
    res->println("401. Unauthorized (try admin/secret or user/test)");
  } else {
    next();
  }
}

/* ===================== Handlers ===================== */

void HttpsAuthServer::handleInternalPage(HTTPRequest * req, HTTPResponse * res) {
  res->setStatusCode(200);
  res->setStatusText("OK");
  res->setHeader("Content-Type", "text/html; charset=utf8");

  res->println("<!DOCTYPE html>");
  res->println("<html><head><title>Internal Area</title></head><body>");

  res->print("<h1>Hello ");
  res->printStd(req->getHeader(HEADER_USERNAME));
  res->print("!</h1>");

  res->println("<p>Welcome to the internal area. Congratulations on successfully entering your password!</p>");

  if (req->getHeader(HEADER_GROUP) == "ADMIN") {
    res->println("<div style=\"border:1px solid red;margin:20px auto;padding:10px;background:#ff8080\">");
    res->println("<h2>You are an administrator</h2>");
    res->println("<p>You are allowed to access the admin page:</p>");
    res->println("<p><a href=\"/internal/admin\">Go to secret admin page</a></p>");
    res->println("</div>");
  }

  res->println("<p><a href=\"/\">Go back home</a></p>");
  res->println("</body></html>");
}

void HttpsAuthServer::handleAdminPage(HTTPRequest * req, HTTPResponse * res) {
  res->setHeader("Content-Type", "text/html; charset=utf8");

  std::string header = "<!DOCTYPE html><html><head><title>Secret Admin Page</title></head><body><h1>Secret Admin Page</h1>";
  std::string footer = "</body></html>";

  if (req->getHeader(HEADER_GROUP) == "ADMIN") {
    res->setStatusCode(200);
    res->setStatusText("OK");
    res->printStd(header);
    res->println("<div style=\"border:1px solid red;margin:20px auto;padding:10px;background:#ff8080\">");
    res->println("<h1>Congratulations</h1>");
    res->println("<p>You found the secret administrator page!</p>");
    res->println("<p><a href=\"/internal\">Go back</a></p>");
    res->println("</div>");
  } else {
    res->printStd(header);
    res->setStatusCode(403);
    res->setStatusText("Unauthorized");
    res->println("<p><strong>403 Unauthorized</strong> You have no power here!</p>");
  }

  res->printStd(footer);
}

void HttpsAuthServer::handlePublicPage(HTTPRequest * req, HTTPResponse * res) {
  res->setHeader("Content-Type", "text/html");
  res->println("<!DOCTYPE html>");
  res->println("<html>");
  res->println("<head><title>Hello World!</title></head>");
  res->println("<body>");
  res->println("<h1>Hello World!</h1>");
  res->print("<p>Your server is running for ");
  res->print((int)(millis()/1000), DEC);
  res->println(" seconds.</p>");
  res->println("<p><a href=\"/\">Go back</a></p>");
  res->println("</body>");
  res->println("</html>");
}

void HttpsAuthServer::handleRoot(HTTPRequest * req, HTTPResponse * res) {
  res->setHeader("Content-Type", "text/html");
  res->println("<!DOCTYPE html>");
  res->println("<html>");
  res->println("<head><title>Hello World!</title></head>");
  res->println("<body>");
  res->println("<h1>Hello World!</h1>");
  res->println("<p>This is the authentication and authorization example. When asked for login "
               "information, try admin/secret or user/test.</p>");
  res->println("<p>Go to: <a href=\"/internal\">Internal Page</a> | <a href=\"/public\">Public Page</a></p>");
  res->println("</body>");
  res->println("</html>");
}

void HttpsAuthServer::handle404(HTTPRequest * req, HTTPResponse * res) {
  req->discardRequestBody();
  res->setStatusCode(404);
  res->setStatusText("Not Found");
  res->setHeader("Content-Type", "text/html");
  res->println("<!DOCTYPE html>");
  res->println("<html>");
  res->println("<head><title>Not Found</title></head>");
  res->println("<body><h1>404 Not Found</h1><p>The requested resource was not found on this server.</p></body>");
  res->println("</html>");
}
