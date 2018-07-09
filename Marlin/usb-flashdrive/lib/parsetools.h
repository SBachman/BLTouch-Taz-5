/* Copyright (C) 2011 Circuits At Home, LTD. All rights reserved.

This software may be distributed and modified under the terms of the GNU
General Public License version 2 (GPL2) as published by the Free Software
Foundation and appearing in the file GPL2.TXT included in the packaging of
this file. Please note that GPL2 Section 2[b] requires that all works based
on this software must also be made publicly available under the terms of
the GPL2 ("Copyleft").

Contact information
-------------------

Circuits At Home, LTD
Web      :  http://www.circuitsathome.com
e-mail   :  support@circuitsathome.com
 */

#if !defined(_usb_h_) || defined(__PARSETOOLS_H__)
#error "Never include parsetools.h directly; include Usb.h instead"
#else
#define __PARSETOOLS_H__

struct MultiValueBuffer {
        uint8_t valueSize;
        void *pValue;
} __attribute__((packed));

class MultiByteValueParser {
        uint8_t * pBuf;
        uint8_t countDown;
        uint8_t valueSize;

public:

        MultiByteValueParser() : pBuf(NULL), countDown(0), valueSize(0) {
        };

        const uint8_t* GetBuffer() {
                return pBuf;
        };

        void Initialize(MultiValueBuffer * const pbuf) {
                pBuf = (uint8_t*)pbuf->pValue;
                countDown = valueSize = pbuf->valueSize;
        };

        bool Parse(uint8_t **pp, uint16_t *pcntdn) {
                if(!pBuf) {
                        Notify(PSTR("Buffer pointer is NULL!\r\n"), 0x80);
                        return false;
                }
                for(; countDown && (*pcntdn); countDown--, (*pcntdn)--, (*pp)++)
                        pBuf[valueSize - countDown] = (**pp);

                if(countDown)
                        return false;

                countDown = valueSize;
                return true;
        }
};


class ByteSkipper {
        uint8_t *pBuf;
        uint8_t nStage;
        uint16_t countDown;

public:

        ByteSkipper() : pBuf(NULL), nStage(0), countDown(0) {
        };

        void Initialize(MultiValueBuffer *pbuf) {
                pBuf = (uint8_t*)pbuf->pValue;
                countDown = 0;
        };

        bool Skip(uint8_t **pp, uint16_t *pcntdn, uint16_t bytes_to_skip) {
                switch(nStage) {
                        case 0:
                                countDown = bytes_to_skip;
                                nStage++;
                        case 1:
                                for(; countDown && (*pcntdn); countDown--, (*pp)++, (*pcntdn)--);

                                if(!countDown)
                                        nStage = 0;
                };
                return (!countDown);
        };
};

#endif // __PARSETOOLS_H__
