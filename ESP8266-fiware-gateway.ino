/**
 * Copyright (c) 2018 LIBERiot/panStamp <dberenguer@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: Oct 24 2018
 */

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

//#include <WiFiClient.h>
#include "gwapmaster.h"
#include "fiware.h"

/**
 * Debug option
 */
#define DEBUG_ENABLED  1

/**
 * Modem reset line
 */
#define MODEM_RESET_LINE  15

/**
 * LED pins
 */
#define LED  2

// Wifi manager
WiFiManager wifiManager;
const char wmPassword[] = "fiwarelab";

// Gateway MAC address
char gatewayMac[16];

// Description string
char deviceId[32];

// Application name
const char appName[] = "FIWARE gateway";

/**
 * GWAP manager
 */
GWAPMASTER gwap;

/**
 * FIWARE settings
 */
const char FIWARE_SERVER[] = "195.235.93.235";
const uint16_t FIWARE_PORT = 8085;
const char FIWARE_APIKEY[] = "hmrfb5prhiqnu715qz9jb9sxt";
FIWARE fiware(FIWARE_SERVER, FIWARE_PORT, FIWARE_APIKEY);


/**
 * Main setup function
 */
void setup()
{ 
  // Modem reset ESP pin
  pinMode(MODEM_RESET_LINE, OUTPUT);
  digitalWrite(MODEM_RESET_LINE, LOW);
 
  // Config LED pin
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Get MAC
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  sprintf(gatewayMac, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  // Set device ID
  sprintf(deviceId, "%s %s", appName, gatewayMac);

  // Init UART
  Serial.begin(38400);

  // Setup Wifi manager
  wifiManager.autoConnect(deviceId, wmPassword);

  digitalWrite(LED, HIGH);

  if (WiFi.status() == WL_CONNECTED)
  {
    #ifdef DEBUG_ENABLED
    Serial.println("");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Opening modem");
    #endif

    // Reset modem
    Serial.end();
    digitalWrite(MODEM_RESET_LINE, HIGH);
    delay(1000);
    Serial.begin(38400);
    Serial.println(deviceId);
    // Start GWAP manager
    gwap.begin();
  }
}

/**
 * Endless loop
 */
void loop()
{ 
  if (gwap.available())
  {
    digitalWrite(LED, LOW);
    fiware.send(gwap.deviceUid, gwap.endpointInfo);
    digitalWrite(LED, HIGH);
  }
}

