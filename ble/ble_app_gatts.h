#ifndef _BLE_APP_GATTS_H_
#define _BLE_APP_GATTS_H_

#include "ble.h"

typedef enum services_t {
    SERVICE_1 = 0,
    SERVICE_MAX_COUNT,
} services_t;

typedef enum characteristic_t {
    CHARACTERISTIC_1 = 0,
    CHARACTERISTIC_MAX_COUNT,
}characteristic_t;

void ble_app_gatts_init(void);

void ble_app_gatts_on_event(ble_evt_t * p_ble_evt);

#endif //  _BLE_APP_GATTS_H_
