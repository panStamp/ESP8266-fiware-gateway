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

#ifndef _GWAPPACKET_H
#define _GWAPPACKET_H

#define GWAP_PACKET_UID_LEN      12
#define GWAP_PACKET_PAYLOAD_LEN  46

/**
 * Class: GWAPPACKET
 * 
 * Description:
 * 
 * GWAP packet class
 */
class GWAPPACKET
{
  private:
    /**
     * strToHexByte
     *
     * @param buf Hex byte as a char array
     *
     * @return Hex byte
     */
    inline uint8_t strToHexByte(char *buf)
    {
      uint8_t res = 0;
      
      for (uint8_t i=0 ; i<2 ; i++)
      {
        res <<= 4;
        if (buf[i] >= 'A' && buf[i] <= 'F')
          res |= buf[i] - 55;
        else if (buf[i] >= 'a' && buf[i] <= 'f')
          res |= buf[i] - 87;
        else if (buf[i] >= '0' && buf[i] <= '9')
          res |= buf[i] - 48;
        else
          return 0;
      }

      return res;
    }
    
  public:
    /**
     * GWAP product code
     */
    uint32_t pCode;

    /**
     * RSSI
     */
    uint8_t rssi;

    /**
     * LQI
     */
    uint8_t lqi;
    
    /**
     * GWAP UID
     */
    uint8_t uid[GWAP_PACKET_UID_LEN];

    /**
     * Cyclic nonce
     */
    uint8_t nonce;

    /**
     * Function code
     */
    uint8_t function;

    /**
     * Register id
     */
    uint8_t regId;

    /**
     * Payload
     */
    uint8_t payload[GWAP_PACKET_PAYLOAD_LEN];

    /**
     * Payload field length
     */
    uint8_t payloadLen;

    /**
     * CRC check
     */
    bool crcOk;
    
    /**
     * Class constructor
     *
     * @param packet GWAP packet in null-terminated string format
     */
    inline GWAPPACKET(char *packet)
    {
      uint8_t trueCrc = 0;
      char *ptr = packet + 1;
      
      rssi = strToHexByte(ptr);
      ptr += 2;
     
      lqi = strToHexByte(ptr);
      ptr += 3;

      uint8_t len = strlen(ptr) / 2;

      for(uint8_t i=0 ; i<sizeof(uid) ; i++)
      {
        uid[i] = strToHexByte(ptr);
        trueCrc += uid[i];
        ptr += 2;
      }
      
      pCode = 0;
      for(uint8_t i=8 ; i<12 ; i++)
      {
        pCode <<= 8;
        pCode |= uid[i];
      }

      nonce = strToHexByte(ptr);
      trueCrc += nonce;
      ptr += 2;
      function = strToHexByte(ptr);
      trueCrc += function;
      ptr += 2;
      regId = strToHexByte(ptr);
      trueCrc += regId;
      ptr += 2;

      payloadLen = len - 16;

      for(uint8_t i=0 ; i<payloadLen ; i++)
      {
        payload[i] = strToHexByte(ptr);
        trueCrc += payload[i];
        ptr += 2;
      }
      
      uint8_t crc = strToHexByte(ptr);

      crcOk = false;
      if (crc == trueCrc)
        crcOk = true;
    }

    /**
     * checkCrc
     *
     * Check CRC field against packet contents
     *
     * @return True in case of CRC pass
     */
    inline bool checkCrc(void)
    {
      return crcOk;
    }
};

#endif

