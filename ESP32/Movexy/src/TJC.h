#ifndef TJC_H
#define TJC_H

#include "struct_typedef.h"
#define FRAME_LENGTH 7

typedef struct {
  
  
} TJC_data_t;

class TJC
{
  public:

  
    void setup();
    bool receiveData();
    bool sendData();
    
    void printData();
};




#endif
