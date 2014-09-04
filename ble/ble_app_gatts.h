#ifndef _BLE_APP_GATTS_H_
#define _BLE_APP_GATTS_H_

typedef enum services_t {
    SERVICE_1 = 0,
    SERVICE_MAX_COUNT,
} services_t;

typedef enum characteristic_t {
    CHARACTERISTIC_1 = 0,
    CHARACTERISTIC_MAX_COUNT,
}characteristic_t;

void ble_app_gatts_init(void);

#endif //  _BLE_APP_GATTS_H_
