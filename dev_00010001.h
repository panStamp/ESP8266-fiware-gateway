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

#ifndef _DEV_00010001_H
#define _DEV_00010001_H

#include "gwapdevice.h"

/**
 * Class: DEV_00010001
 * 
 * Description:
 * 
 * Dual temperature + humidity sensor
 */
class DEV_00010001 : public GWAPDEVICE
{
  public:
    /**
     * Class constructor
     *
     * @param pkt GWAP packet
     */
    inline DEV_00010001(GWAPPACKET *pkt) : GWAPDEVICE(pkt)
    {
    }

    /**
     * getEndpointUlString
     *
     * Obtain UltraLight 2 (FIWARE) string containing endpoint information
     *
     * @param buf pointer to buffer receiving the UL string
     *
     * @return false in case of problem
     */
    inline bool getEndpointUlString(char *buf)
    {
      if (regId == 7)
      {
        if (payloadLen == 6)
        {
          uint16_t val;

          val = payload[0] << 8;
          val |= payload[1];

          float voltage = val / 1000.0;

          val = payload[2] << 8;
          val |= payload[3];

          float temperature = val / 10.0 - 50;

          val = payload[4] << 8;
          val |= payload[5];

          float humidity = val / 10.0;

          sprintf(buf, "voltage|%.2f#temperature|%.2f#humidity|%.2f", voltage, temperature, humidity);
          return true;
        }
      }
      return false;
    }
};

#endif

