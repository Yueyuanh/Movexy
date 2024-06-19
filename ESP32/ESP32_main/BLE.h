#ifndef BLE_H
#define BLE_H

#include "struct_typedef.h"

typedef struct {
  
  
} BLE_data_t;

class BLE
{
  public:
    uint8_t Connected;
    uint8_t KeyboardConnected;
    uint8_t MouseConnected;
    

  
    void setup();
    bool receiveData();
    bool sendKeyData();
    bool sendMouseData();
    void printData();
};



static __inline void delay_clock(int ts);
void udelay(int us);
void mdelay(int ms);



#endif
