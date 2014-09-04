#include "ble_gatts.h"

#include "nrf.h"
#include "app_error.h"

#include "ble.h"
#include "ble_types.h"
#include "ble_gap.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_gatts.h"

#include "config.h"

#define READ_PERM_MASK          (1 << 0)
#define WRITE_PERM_MASK         (1 << 1)
#define WRITE_PROP_MASK         (1 << 2)
#define WRITE_NO_RESP_PROP_MASK (1 << 3)
#define READ_PROP_MASK          (1 << 4)
#define NOTIFY_PROP_MASK        (1 << 6)
#define INDICATE_PROP_MASK      (1 << 7)

static uint16_t const services_uuids[SERVICE_MAX_COUNT] =
{
    0xff00, // SERVCIE_1
};

static const uint16_t characteristics_uuids[CHARACTERISTIC_MAX_COUNT] =
{
    0xff01, // CHAR_1
};

static uint16_t services_handles[SERVICE_MAX_COUNT] = { 0 };
static ble_gatts_char_handles_t characteristics_handles[SERVICE_MAX_COUNT] = { {0} };
static void ble_uuids_init(void);
static void ble_services_init();
static void ble_service_1_characteristic_1_init();

uint32_t ble_gatts_characteristic_add(const ble_uuid_t uuid, 
                                            const uint16_t properties,
                                            uint8_t * p_characteristic_value, 
                                            const uint16_t characteristic_value_length,
                                            const uint16_t characteristic_max_value_length,
                                            ble_gatts_char_handles_t * p_handles);

void ble_gatts_init(void)
{
    memset(services_handles, 0, sizeof(services_handles));
    memset(characteristics_handles, 0, sizeof(characteristics_handles));
    ble_uuids_init();
    ble_services_init();
}

void ble_uuids_init(void)
{
    /*
    ble_uuid128_t uuid;
    uint8_t uuid128[16] = { 0 };
    memcpy(uuid.uuid128, uuid128, sizeof(uuid128));
    APP_ERROR_CHECK(sd_ble_uuid_vs_add(&uuid, &uuids_type[UUID128_AUTH]));
    */
}

void ble_services_init()
{
    ble_uuid_t uuid;
    uuid.type = BLE_UUID_TYPE_BLE;

    int index = SERVICE_1;
    uuid.uuid = services_uuids[index];
    APP_ERROR_CHECK(sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &uuid, &services_handles[index]));

    // according to s110/ble_gatts.h:360 -> can only add a characteristic to the last added service
    ble_service_1_characteristic_1_init();
}

void ble_service_1_characteristic_1_init()
{
    const int i = CHARACTERISTIC_1;

    ble_uuid_t uuid;
    // uuid
    uuid.type = BLE_UUID_TYPE_BLE;
    uuid.uuid = characteristics_uuids[i];

    APP_ERROR_CHECK(ble_gatts_characteristic_add(uuid, 
                                WRITE_PROP_MASK|WRITE_PERM_MASK|READ_PERM_MASK,
                                NULL,
                                0,
                                0,
                                &characteristics_handles[i]));
}

uint32_t ble_gatts_characteristic_add(const ble_uuid_t uuid, 
                                            const uint16_t properties,
                                            uint8_t * p_characteristic_value, 
                                            const uint16_t characteristic_value_length,
                                            const uint16_t characteristic_max_value_length,
                                            ble_gatts_char_handles_t * p_handles)
{
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t attr_md;
    ble_gatts_attr_t attr_value;

    // cccd_md
    memset(&cccd_md, 0, sizeof(cccd_md));
    if(properties & READ_PERM_MASK) {BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);}
    if(properties & WRITE_PERM_MASK) {BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);}
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    // char md
    memset(&char_md, 0, sizeof(char_md));
    if(properties & WRITE_PROP_MASK){char_md.char_props.write = true;}
    if(properties & WRITE_NO_RESP_PROP_MASK){char_md.char_props.write_wo_resp = true;}
    if(properties & READ_PROP_MASK){char_md.char_props.read = true;}
    if(properties & NOTIFY_PROP_MASK){char_md.char_props.notify = true;}
    if(properties & INDICATE_PROP_MASK){char_md.char_props.indicate = true;}

    char_md.p_cccd_md = &cccd_md;

    // attr md
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.read_perm = cccd_md.read_perm;
    attr_md.write_perm = cccd_md.write_perm;
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.vlen = 0;

    // attr value
    memset(&attr_value, 0, sizeof(attr_value));
    attr_value.p_attr_md = &attr_md;

    ble_uuid_t u = uuid;
    attr_value.p_uuid = &u;
    attr_value.init_len = characteristic_max_value_length;
    attr_value.p_value = p_characteristic_value;
    attr_value.init_offs = 0;
    attr_value.max_len = characteristic_value_length;

    // according to doc to sd_ble_gatts_characteristic_add, use BLE_GATT_HANDLE_INVALID, the characteristic will be placed sequentially
    return sd_ble_gatts_characteristic_add(BLE_GATT_HANDLE_INVALID, &char_md, &attr_value, p_handles); 
}
