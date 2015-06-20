/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __receivers_h__
#define __receivers_h__

#include <Arduino.h>

#include "config/config.h"
#include "settings.h"
#include "storage/storage.h"
#include "rf/rf.h"

struct ReceiverState {
    bool active : 1;
    bool forced : 1;

    bool got_rf : 1;

    byte :0;

    int8_t current_job : 4;
    int8_t    next_job : 4;
};


extern ReceiverState receivers[];

#define selected_receiver receivers[state.selected_recv]

void update_schedule();

#endif