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

#ifndef _GWAPMASTER_H
#define _GWAPMASTER_H

#include "gwapmodem.h"
#include "dev_00010001.h"
#include "dev_00010004.h"
#include "dev_00010008.h"
#include "dev_00010009.h"

/**
 * Class: GWAPMASTER
 * 
 * Description:
 * 
 * GWAP master class
 */
class GWAPMASTER
{
  private:
    /**
     * GWAP modem
     */
    GWAPMODEM modem;

  public:
    /**
     * GWAP UID of last device received
     */
    char deviceUid[GWAP_PACKET_UID_LEN * 2 + 1];
     
    /**
     * GWAP device endpoint information
     */
    char endpointInfo[64];

    /**
     * begin
     *
     * Initialize GWAP master
     */
    inline void begin(void)
    {
      modem.begin();
    }

    /**
     * available
     *
     * Check if there is a GWAP packet available
     *
     * @return True in case of packet available
     */
    inline bool available(void)
    {
      if (modem.available())
      {
        if (modem.check())
        {
          char buf[128];
          modem.read(buf);
          // Avoid packets broadcasted with other protocols
          if (buf[6] != '0' || buf[7] != '0')
          {
            // Skip too short frames
            if (strlen(buf) >= 34)
            {
              GWAPPACKET packet(buf);

              if (packet.checkCrc())
              {
                char *ptr = deviceUid;
                char b[3];
                for(uint8_t i=0 ; i<12 ; i++)
                {
                  if (packet.uid[i] < 0x10)
                    sprintf(ptr, "0%X", packet.uid[i]);
                  else
                    sprintf(ptr, "%X", packet.uid[i]);
                  ptr += 2;
                }

                if (packet.pCode == 0x00010001)
                {
                  DEV_00010001 device(&packet);
                  return device.getEndpointInfo(endpointInfo);
                }
                else if (packet.pCode == 0x00010004)
                {
                  DEV_00010004 device(&packet);
                  return device.getEndpointInfo(endpointInfo);
                }
                else if (packet.pCode == 0x00010008)
                {
                  DEV_00010008 device(&packet);
                  return device.getEndpointInfo(endpointInfo);
                }
                else if (packet.pCode == 0x00010009)
                {
                  DEV_00010009 device(&packet);
                  return device.getEndpointInfo(endpointInfo);
                }
              }
            }
          }
        }
      }
      return false;
    }
};

#endif

