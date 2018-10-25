/**
 * Copyright (c) 2018 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
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
 * Creation date: Oct 19 2018
 */

#ifndef _GWAPMODEM_H
#define _GWAPMODEM_H

#include "Arduino.h"

#define SERIAL_BUFFER_LEN  132

/**
 * Class: GWAPMODEM
 * 
 * Description:
 *
 * GWAP serial interface
 */
class GWAPMODEM
{
  private:
    /**
     * Serial port
     */
    HardwareSerial *serPort;

    /**
     * Serial baudrate
     */
    uint32_t baudrate;

    /**
     * Serial Rx buffer
     */
    char rxBuffer[SERIAL_BUFFER_LEN];

    /**
     * Current length of serial packet
     */
    uint8_t rxLength;

  public:
    /**
     * Class constructor
     */
    inline GWAPMODEM(HardwareSerial *port=&Serial, uint32_t speed=38400)
    {
      serPort = port;
      baudrate = speed;
      clearRxBuffer();
    }

    /**
     * Start modem
     *
     * @param speed Serial speed
     */
    inline void begin(void)
    {
      serPort->begin(baudrate);
      serPort->setDebugOutput(0);
      serPort->println("");
      clearRxBuffer();
      delay(2000);
    }

    /**
     * config
     *
     * Configure modem
     *
     * @param channel RF chanel
     * @param netidH  Network ID, high byte
     * @param netidL  Network ID, low byte
     * @param address modem's device address
     */
    bool config(uint8_t channel, uint8_t netidH, uint8_t netidL, uint8_t address);

    /**
     * clearRxBuffer
     *
     * Clear Rx buffer
     */
    inline void clearRxBuffer(void)
    {
      memset(rxBuffer, 0, sizeof(rxBuffer));
      rxLength = 0;
    }

    /**
     * available
     *
     * Return true if a SWAP packet is waiting to be read from the buffer
     *
     * @return true if a paquet is waiting to be read. Return false otherwise
     */
    bool available(void);

    /**
     * check
     * 
     * Check integrity of packet received
     * 
     * @return True if check is OK
     */
    bool check(void);

    /**
     * read
     *
     * Get pointer to buffer containing the last packet received
     *
     * @param pointer to the buffer
     * 
     * @retuen Buffer length
     */
    inline uint8_t read(char *buf)
    {
      uint8_t len = 0;
      
      // Basic packet validation
      if ((rxBuffer[0] == '(') && (rxBuffer[5] == ')'))
      {
        if ((rxLength % 2) == 0)
        {
          len = rxLength;          
          strcpy(buf, rxBuffer);
        }
      }
       
      clearRxBuffer();
      
      return len;
    }
    
    /**
     * write
     *
     * Send packet to serial modem
     *
     * @param buf Buffer to be transmitted
     *
     * @return amount of bytes transmitted
     */
    uint8_t write(char *buf)
    {
      serPort->flush();
      serPort->println(buf);
    }
};

#endif

