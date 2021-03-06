#include <plot.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "ssid";
const char *passwd = "passwd";
const char* filename = "/animated.png";

WebServer server;

void writePng(const char* plFilename) {
  FILE *plFile = fopen(plFilename, "wb");
  plPlotter *plotter;
  plPlotterParams *plotter_params;
  plotter_params = pl_newplparams();
  pl_setplparam(plotter_params, "BITMAPSIZE", (void*)"320x240");
  plotter = pl_newpl_r ("png", stdin, plFile, stderr, plotter_params);
  pl_openpl_r (plotter);
  pl_fspace_r (plotter, -0.5, -0.5, 149.5, 99.5);
  pl_pencolorname_r (plotter, "red");
  pl_linewidth_r (plotter, 5);
  pl_filltype_r (plotter, 1);
  pl_fillcolorname_r (plotter, "black");
  for (int i = 0; i < 180 ; i += 15) {
    pl_erase_r (plotter);
    pl_ellipse_r (plotter, 75, 50, 40, 20, i);
  }
  pl_closepl_r (plotter);
  pl_deletepl_r (plotter);

}

void setup() {
    Serial.begin(115200);
    SPIFFS.begin(true);
    String pxFilename = "/spiffs" + (String)filename;
    writePng(pxFilename.c_str());
    WiFi.begin(ssid,passwd);
    WiFi.waitForConnectResult();
    server.begin();
    server.serveStatic("/", SPIFFS, filename);
    server.onNotFound([](){server.send(404, "text/plain", "404 - File not found");});
    Serial.print("http://");
    Serial.println(WiFi.localIP());
}
    
void loop() {
  server.handleClient();
  delay(150);
}
