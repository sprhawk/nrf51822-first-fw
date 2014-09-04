#include "ble_app.h"

#include "ble_stack_handler_types.h"
#include "ble.h"
#include "ble_types.h"
#include "ble_gap.h"
#include "ble_hci.h"
#include "ble_advdata.h"

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "nrf.h"
#include "app_error.h"

#include "config.h"
#include "ble_gatts.h"

uint8_t * ble_app_adv_data_new(size_t size);

static void ble_on_event_handler(ble_evt_t * p_ble_evt);
static void ble_gap_init(void);
static void ble_adv_init(void);

static uint16_t m_client_conn_handle = 0;

void ble_app_init(void)
{
    // service_changed determines whether service changes will be updated on central ?
    ble_enable_params_t bep = { { 1 } };
    APP_ERROR_CHECK(sd_ble_enable(&bep));
    
    APP_ERROR_CHECK(softdevice_ble_evt_handler_set(ble_on_event_handler));
    
    ble_gap_init();
    ble_adv_init();
    ble_gatts_init();

/*  
    uint16_t conn_handle = 0;
    uint8_t hci_status_code = BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION;
    APP_ERROR_CHECK(sd_ble_gap_disconnect(conn_handle, hci_status_code));
    */
}

// setup gap related parameters
void ble_gap_init(void)
{
    // setup ble address type
#if 0
    ble_gap_addr_t addr = { 0 };
    addr.addr_type = BLE_GAP_ADDR_TYPE_PUBLIC;
    APP_ERROR_CHECK(sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &addr));

    ble_opt_t opt;
    memset(&opt, 0, sizeof(opt));
    opt.gap.privacy.interval_s = 1; // seconds
    APP_ERROR_CHECK(sd_ble_opt_set(BLE_GAP_OPT_PRIVACY, &opt));
#endif

    // setup ble device name
    ble_gap_conn_sec_mode_t sec_mode;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    uint16_t len = strlen(BLE_DEVICE_NAME);
    APP_ERROR_CHECK(sd_ble_gap_device_name_set(&sec_mode, (uint8_t *const)BLE_DEVICE_NAME, len));

    // setup ble tx power
    APP_ERROR_CHECK(sd_ble_gap_tx_power_set(BLE_DEVICE_DEFAULT_TX_POWER));

    // setup ble appearance
    uint16_t appearance = BLE_APPEARANCE_UNKNOWN;
    APP_ERROR_CHECK(sd_ble_gap_appearance_set(appearance));

    // setup ble conn parameters
    ble_gap_conn_params_t conn_params;
    memset(&conn_params, 0, sizeof(conn_params));
    conn_params.min_conn_interval = BLE_CONN_MIN_INTERVAL;
    conn_params.max_conn_interval = BLE_CONN_MAX_INTERVAL;
    conn_params.slave_latency = BLE_CONN_SLAVE_LANTENCY;
    conn_params.conn_sup_timeout = BLE_CONN_SUPERVISION_TIMEOUT;
    APP_ERROR_CHECK(sd_ble_gap_ppcp_set(&conn_params));
}

void ble_adv_init(void)
{
#if 1
    ble_advdata_t adv_data;
    memset(&adv_data, 0, sizeof(adv_data));
    adv_data.name_type = BLE_ADVDATA_SHORT_NAME;
    adv_data.short_name_len = strlen(BLE_DEVICE_NAME);
    adv_data.include_appearance = true;

    adv_data.uuids_complete.uuid_cnt = 1;
    ble_uuid_t uuids[1] = { {BLE_ADVERTISED_SERVICE_UUID_16BIT, BLE_UUID_TYPE_BLE }};
    adv_data.uuids_complete.p_uuids = uuids;

    uint8_t flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    adv_data.flags.size = sizeof(flags);
    adv_data.flags.p_data = &flags;

    // ble_advdata_t resp_data;
    // memset(&resp_data, 0, sizeof(resp_data));

    APP_ERROR_CHECK(ble_advdata_set(&adv_data,NULL));
#else
    uint8_t data[] = { 0x02, 0x01, 0x06, 
                       0x1A, 0xFF, 0x4C, 0x00, 0x02, 0x15, 
                       0xa1, 0x23, 0xc1, 0xc4, 0x9a, 0x01, 0x44, 0x86, 0x94, 0x74, 0x34, 0x5c, 0x8f, 0xee, 0x73, 0x16,  // UUID
                       0x00, 0x02, // major
                       0x00, 0x0A, // minor
                       0xC5        // measured power
    };
    uint8_t * p_advdata = ble_app_adv_data_new(sizeof(data));
    memcpy(p_advdata, data, sizeof(data));

    APP_ERROR_CHECK(sd_ble_gap_adv_data_set(p_advdata, sizeof(data), NULL, 0));
    free(p_advdata);
    p_advdata = NULL;
#endif
}

void ble_app_adv_start(void)
{
    ble_gap_adv_params_t params = {0};
    memset(&params, 0, sizeof(params));
    params.type =BLE_GAP_ADV_TYPE_ADV_IND;// BLE_GAP_ADV_TYPE_ADV_NONCONN_IND, BLE_GAP_ADV_TYPE_ADV_DIRECT_IND, BLE_GAP_ADV_TYPE_ADV_SCAN_IND
    params.interval = 160 ;// 100 ms

    APP_ERROR_CHECK(sd_ble_gap_adv_start(&params));
}

void ble_app_adv_stop(void)
{
    APP_ERROR_CHECK(sd_ble_gap_adv_stop());
}

#ifdef DEBUG
static void log_addr(const char * prefix, const ble_gap_addr_t * addr)
{
    if(addr) {
        const uint8_t *a = addr->addr;
        if(prefix) {
            printf("%s%x %x %x %x %x %x\r\n", prefix, *a, *(a + 1), *(a + 2), *(a + 3), *(a + 4), *(a + 5));
        }
        else {
            printf("%x %x %x %x %x %x\r\n",*a, *(a + 1), *(a + 2), *(a + 3), *(a + 4), *(a + 5));
        }
    }
}

static void log_conn_params(const ble_gap_conn_params_t *p)
{
    if(p) {
        printf("ble gap conn params: min intv: %u, max intv: %u, slave lentancy: %u, supervision: %u\r\n", p->min_conn_interval, p->max_conn_interval, p->slave_latency, p->conn_sup_timeout);
    }
}
#endif

void ble_on_event_handler(ble_evt_t * p_ble_evt)
{
    switch(p_ble_evt->header.evt_id) {
        // common events
        case BLE_EVT_TX_COMPLETE:
#ifdef DEBUG
    printf("ble evt: tx_complete(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_EVT_USER_MEM_REQUEST:
#ifdef DEBUG
    printf("ble evt: user_mem_request(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_EVT_USER_MEM_RELEASE:
#ifdef DEBUG
    printf("ble evt:user_mem_release(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        
        // gap events
        case BLE_GAP_EVT_CONNECTED:
#ifdef DEBUG
    printf("ble evt: gap_evt_connected(%d)\r\n", p_ble_evt->header.evt_id);
    log_addr("peer addr:", &p_ble_evt->evt.gap_evt.params.connected.peer_addr);
    ble_gap_conn_params_t *p = &p_ble_evt->evt.gap_evt.params.connected.conn_params;
    log_conn_params(p);
#endif
            m_client_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            break;
        case BLE_GAP_EVT_DISCONNECTED:
#ifdef DEBUG
    printf("ble evt: gap_evt_disconnected(%d) reason:%u\r\n", p_ble_evt->header.evt_id, p_ble_evt->evt.gap_evt.params.disconnected.reason);
#endif
            m_client_conn_handle = 0;
            ble_app_adv_start();
            break;
        case BLE_GAP_EVT_CONN_PARAM_UPDATE:
#ifdef DEBUG
    printf("ble evt: evt_conn_param_update(%d)\r\n", p_ble_evt->header.evt_id);
    log_conn_params(&p_ble_evt->evt.gap_evt.params.conn_param_update.conn_params);
#endif
            break;
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
#ifdef DEBUG
    printf("ble evt: evt_sec_params_request(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GAP_EVT_SEC_INFO_REQUEST:
#ifdef DEBUG
    printf("ble evt: evt_sec_info_request(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GAP_EVT_PASSKEY_DISPLAY:
#ifdef DEBUG
    printf("ble evt: evt_passkey_display(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GAP_EVT_AUTH_KEY_REQUEST:
#ifdef DEBUG
    printf("ble evt: evt_auth_key_request(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GAP_EVT_AUTH_STATUS:
#ifdef DEBUG
    printf("ble evt: evt_auth_status(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GAP_EVT_CONN_SEC_UPDATE:
#ifdef DEBUG
    printf("ble evt: evt_conn_sec_udpate(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GAP_EVT_TIMEOUT:
#ifdef DEBUG
    printf("ble evt: gap_evt_timeout(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GAP_EVT_RSSI_CHANGED:
#ifdef DEBUG
    printf("ble evt: gap_evt_rssi_changed(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;

        // l2cap events
        case BLE_L2CAP_EVT_RX:
#ifdef DEBUG
    printf("ble evt: l2cap_evt_rx(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;

        // gattc events
        case BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP:
#ifdef DEBUG
    printf("ble evt: gattc_evt_prim_srvc_disc_resp(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GATTC_EVT_REL_DISC_RSP:
#ifdef DEBUG
    printf("ble evt: gattc_evt_rel_disc_rsp(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GATTC_EVT_CHAR_DISC_RSP:
#ifdef DEBUG
    printf("ble evt: gattc_evt_char_disc_rsp(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GATTC_EVT_DESC_DISC_RSP:
#ifdef DEBUG
    printf("ble evt: gattc_evt_desc_disc_rsp(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GATTC_EVT_CHAR_VAL_BY_UUID_READ_RSP:
#ifdef DEBUG
    printf("ble evt: gattc_evt_char_val_by_uuid_read_rsp(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GATTC_EVT_READ_RSP:
#ifdef DEBUG
    printf("ble evt: gattc_evt_read_rsp(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GATTC_EVT_CHAR_VALS_READ_RSP:
#ifdef DEBUG
    printf("ble evt: gattc_evt_char_vals_read_rsp(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GATTC_EVT_WRITE_RSP:
#ifdef DEBUG
    printf("ble evt: gattc_evt_write_rsp(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GATTC_EVT_HVX:
#ifdef DEBUG
    printf("ble evt: gattc_evt_hvx(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GATTC_EVT_TIMEOUT:
#ifdef DEBUG
    printf("ble evt: gattc_evt_timeout(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
    
        // gatts events
        case BLE_GATTS_EVT_WRITE:
#ifdef DEBUG
    printf("ble evt: gatts_evt_write(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
#ifdef DEBUG
    printf("ble evt: gatts_evt_rw_authorize_request(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
#ifdef DEBUG
    printf("ble evt: gatts_evt_sys_attr_missing(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GATTS_EVT_HVC:
#ifdef DEBUG
    printf("ble evt: gatts_evt_hvc(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GATTS_EVT_SC_CONFIRM:
#ifdef DEBUG
    printf("ble evt: gatts_evt_sc_confirm(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
        case BLE_GATTS_EVT_TIMEOUT:
#ifdef DEBUG
    printf("ble evt: gatts_evt_timeout(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            if (BLE_GATT_TIMEOUT_SRC_PROTOCOL == p_ble_evt->evt.gatts_evt.params.timeout.src) {
                uint32_t err_code = sd_ble_gap_disconnect(m_client_conn_handle,
                                                            BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
                m_client_conn_handle = 0;
                if(NRF_SUCCESS != err_code) {
                    printf("timeout disconnecting failed:%4lx\r\n", err_code);
                }
            }
            break;
            break;

        default:
#ifdef DEBUG
    printf("ble evt: unknown(%d)\r\n", p_ble_evt->header.evt_id);
#endif
            break;
    }
}

uint8_t * ble_app_adv_data_new(size_t size)
{
    uint8_t * pdata = NULL;
    const size_t max_size = sizeof(uint8_t) * BLE_GAP_ADV_MAX_SIZE;
    if (max_size < size) {
        APP_ERROR_HANDLER(NRF_ERROR_DATA_SIZE);
        return NULL;
    }
    pdata = malloc(size);
    if(NULL == pdata) {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
        return NULL;
    }
    memset(pdata, 0, size);
    return pdata;
}
