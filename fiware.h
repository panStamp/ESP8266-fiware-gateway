/**
 * Copyright (c) 2018 panStamp <contact@panstamp.com>
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: Oct 8 2018
 */

#ifndef _FIWARE_H
#define _FIWARE_H

#include <stdarg.h>
#include <Arduino.h>
#include <ESP8266HTTPClient.h>

#define FIWARE_SERVER_STRING_MAXLEN  32

class FIWARE
{
  private:
    /**
     * HTTP object
     */
    HTTPClient http;
    
    /**
     * FIWARE server URL
     */
    char server[FIWARE_SERVER_STRING_MAXLEN];

    /**
     * FIWARE UltraLight API port
     */
    uint16_t ulPort;

    /**
     * Ultralight IoT agent API key
     */
    char apiKey[FIWARE_SERVER_STRING_MAXLEN];
        
  public:
    /**
     * Class constructor
     * 
     * @param fiwareServer FIWARE server URL or IP address
     * @param port Port for UltraLight API
     * @param apiK UltraLight API key
     */
    inline FIWARE(const char *fiwareServer, const uint16_t port, const char *apiK)
    {
      strcpy(server, fiwareServer);
      ulPort = port;
      strcpy(apiKey, apiK);
    }

    /**
     * send
     * 
     * Send attribute value
     * 
     * @param entity Entity name
     * @param attributes Attributes following UltraLight format (attr1|val1#attr1|val2...)
     * 
     * @return True in case of success. Return false otherwise
     */
    inline bool send(char *entity, char *attributes)
    {           
      // Make a HTTP request:
      char url[128];
      sprintf(url, "http://%s:%d/iot/d?k=%s&i=%s&getCmd=1", server, ulPort, apiKey, entity);
                
      http.begin(url);
      http.addHeader("Content-Type", "text/plain");
      int httpCode = http.POST(attributes);
      String payload = http.getString();
      
      if (httpCode == 200)
        return true;

      return false;
    }
};
#endif
