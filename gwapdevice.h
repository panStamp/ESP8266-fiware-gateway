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

#ifndef _GWAPDEVICE_H
#define _GWAPDEVICE_H

#include "gwappacket.h"

#define getEndpointInfo  getEndpointUlString

/**
 * Class: GWAPDEVICE
 * 
 * Description:
 * 
 * GWAP device primary class
 */
class GWAPDEVICE
{
  public:
    /**
     * Device UID
     */
    uint8_t *uid;

    /**
     * Cyclic nonce
     */
    uint8_t nonce;

    /**
     * Register id
     */
    uint8_t regId;

    /**
     * Pointer to payload
     */
    uint8_t *payload;

    /**
     * Payload field length
     */
    uint8_t payloadLen;
    
    /**
     * Class constructor
     *
     * @param pkt GWAP packet
     */
    inline GWAPDEVICE(GWAPPACKET *pkt)
    {
      uid = pkt->uid;
      nonce = pkt->nonce;
      regId = pkt->regId;
      payload = pkt->payload;
      payloadLen = pkt->payloadLen;
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
    virtual bool getEndpointUlString(char *buf);
};

#endif

