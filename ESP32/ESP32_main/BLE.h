#ifndef BLE_H
#define BLE_H

#include "struct_typedef.h"

typedef struct {
  
  
} BLE_data_t;

class BLE
{
  public:
    void setup();
    bool receiveData();
    bool sendData();
    
    void printData();
};



static __inline void delay_clock(int ts);
void udelay(int us);
void mdelay(int ms);



#endif
