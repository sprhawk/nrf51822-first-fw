#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "nrf.h"
#include "app_error.h"

#include "ble_app.h"

#include "ble_stack_handler_types.h"
#include "ble.h"
#include "ble_types.h"
#include "ble_gap.h"
#include "ble_hci.h"
#include "ble_advdata.h"

#include "config.h"

static void ble_on_event_handler(ble_evt_t * p_ble_evt);

void ble_app_init(void)
{
    ble_enable_params_t bep = { { 0 } };
    APP_ERROR_CHECK(sd_ble_enable(&bep));
    
    APP_ERROR_CHECK(softdevice_ble_evt_handler_set(ble_on_event_handler));
    
    ble_gap_conn_sec_mode_t sec_mode;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    uint16_t len = strlen(BLE_DEVICE_NAME);
    APP_ERROR_CHECK(sd_ble_gap_device_name_set(&sec_mode, (uint8_t *const)BLE_DEVICE_NAME, len));

    ble_advdata_t adv_data;
    memset(&adv_data, 0, sizeof(adv_data));
    adv_data.name_type = BLE_ADVDATA_SHORT_NAME;
    adv_data.short_name_len = strlen(BLE_DEVICE_NAME);
    adv_data.include_appearance = true;
    uint8_t flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    adv_data.flags.size = sizeof(flags);
    adv_data.flags.p_data = &flags;

    ble_advdata_t resp_data;
    memset(&resp_data, 0, sizeof(resp_data));

    APP_ERROR_CHECK(ble_advdata_set(&adv_data,NULL));

    APP_ERROR_CHECK(sd_ble_gap_tx_power_set(-30));

    uint16_t appearance = BLE_APPEARANCE_UNKNOWN;
    APP_ERROR_CHECK(sd_ble_gap_appearance_set(appearance));

    ble_gap_adv_params_t params = {0};
    params.type = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND; //BLE_GAP_ADV_TYPE_ADV_IND, BLE_GAP_ADV_TYPE_ADV_DIRECT_IND, BLE_GAP_ADV_TYPE_ADV_SCAN_IND
    APP_ERROR_CHECK(sd_ble_gap_adv_start(&params));
/*    APP_ERROR_CHECK(sd_ble_gap_adv_stop());

    uint16_t conn_handle = 0;
    uint8_t hci_status_code = BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION;
    APP_ERROR_CHECK(sd_ble_gap_disconnect(conn_handle, hci_status_code));
    */
}


void ble_on_event_handler(ble_evt_t * p_ble_evt)
{
    switch(p_ble_evt->header.evt_id) {
        // common events
        case BLE_EVT_TX_COMPLETE:
            break;
        case BLE_EVT_USER_MEM_REQUEST:
            break;
        case BLE_EVT_USER_MEM_RELEASE:
            break;
        
        // gap events
        case BLE_GAP_EVT_CONNECTED:
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            break;
        case BLE_GAP_EVT_CONN_PARAM_UPDATE:
            break;
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            break;
        case BLE_GAP_EVT_SEC_INFO_REQUEST:
            break;
        case BLE_GAP_EVT_PASSKEY_DISPLAY:
            break;
        case BLE_GAP_EVT_AUTH_KEY_REQUEST:
            break;
        case BLE_GAP_EVT_AUTH_STATUS:
            break;
        case BLE_GAP_EVT_CONN_SEC_UPDATE:
            break;
        case BLE_GAP_EVT_TIMEOUT:
            break;
        case BLE_GAP_EVT_RSSI_CHANGED:
            break;

        // l2cap events
        case BLE_L2CAP_EVT_RX:
            break;

        // gattc events
        case BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP:
            break;
        case BLE_GATTC_EVT_REL_DISC_RSP:
            break;
        case BLE_GATTC_EVT_CHAR_DISC_RSP:
            break;
        case BLE_GATTC_EVT_DESC_DISC_RSP:
            break;
        case BLE_GATTC_EVT_CHAR_VAL_BY_UUID_READ_RSP:
            break;
        case BLE_GATTC_EVT_READ_RSP:
            break;
        case BLE_GATTC_EVT_CHAR_VALS_READ_RSP:
            break;
        case BLE_GATTC_EVT_WRITE_RSP:
            break;
        case BLE_GATTC_EVT_HVX:
            break;
        case BLE_GATTC_EVT_TIMEOUT:
            break;
    
        // gatts events
        case BLE_GATTS_EVT_WRITE:
            break;
        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            break;
        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            break;
        case BLE_GATTS_EVT_HVC:
            break;
        case BLE_GATTS_EVT_SC_CONFIRM:
            break;
        case BLE_GATTS_EVT_TIMEOUT:
            break;

        default:
            break;
    }
}

