/**
    Copyright: zirexix 2016

    This program is distributed
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    This program, or portions of it cannot be used in commercial
    products without the written consent of the author: zyraxes.comatgmaildotcom.

*/

#ifndef NO_HCI_H
#define NO_HCI_H

#include <iostream>
#include <vector>
//#include "os.h"
#include "include/hci.h"
#include "hci_socket.h"
#include "bybuff.h"

#define DISCONNECT_CMD              CMD_OPCODE_PACK(OCF_DISCONNECT, OGF_LINK_CTL);
#define SET_EVENT_MASK_CMD          CMD_OPCODE_PACK(OCF_SET_EVENT_MASK, OGF_HOST_CTL)
#define RESET_CMD                   CMD_OPCODE_PACK(OCF_RESET, OGF_HOST_CTL)
#define READ_LE_HOST_SUPPORTED_CMD  CMD_OPCODE_PACK(OCF_READ_LE_HOST_SUPPORTED, OGF_HOST_CTL)
#define WRITE_LE_HOST_SUPPORTED_CMD CMD_OPCODE_PACK(OCF_WRITE_LE_HOST_SUPPORTED, OGF_HOST_CTL)
#define READ_LOCAL_VERSION_CMD      CMD_OPCODE_PACK(OCF_READ_LOCAL_VERSION , OGF_INFO_PARAM)
#define READ_BD_ADDR_CMD            CMD_OPCODE_PACK(OCF_READ_BD_ADDR , OGF_INFO_PARAM)
#define READ_RSSI_CMD               CMD_OPCODE_PACK(OCF_READ_RSSI, OGF_STATUS_PARAM)
#define LE_SET_EVENT_MASK_CMD           CMD_OPCODE_PACK(OCF_SET_EVENT_MASK, OGF_LE_CTL)
#define LE_SET_ADVERTISING_PARAMETERS_  CMD CMD_OPCODE_PACK(OCF_LE_SET_ADVERTISING_PARAMETERS, OGF_LE_CTL)
#define LE_SET_ADVERTISING_DATA_CMD     CMD_OPCODE_PACK(OCF_LE_SET_ADVERTISING_DATA, OGF_LE_CTL)
#define LE_SET_SCAN_RESPONSE_DATA_CMD   CMD_OPCODE_PACK(OCF_LE_SET_SCAN_RESPONSE_DATA, OGF_LE_CTL)
#define LE_SET_ADVERTISE_ENABLE_CMD     CMD_OPCODE_PACK(OCF_LE_SET_ADVERTISE_ENABLE, OGF_LE_CTL)
#define LE_LTK_NEG_REPLY_CMD            CMD_OPCODE_PACK(OCF_LE_LTK_NEG_REPLY, OGF_LE_CTL)
#define LE_SET_ADVERTISING_PARAMETERS_CMD CMD_OPCODE_PACK(OCF_LE_SET_ADVERTISING_PARAMETERS, OGF_LE_CTL)


struct hci_error
{
    int         nerror;
    std::string message;
};

typedef struct
{
    uint8_t     cmd;
    uint16_t    evm;
    uint8_t     len;
}__attribute__ ((packed)) hcihr;

typedef struct sdata
{
    const uint8_t*    data;
    uint16_t    len;

}__attribute__ ((packed)) sdata;


typedef struct {
	uint8_t		data;
} __attribute__ ((packed)) no_data_arr;

#define NO_EVT_CMD_COMPLETE 		0x0E
typedef struct
{
     uint16_t   cmd;
     uint8_t    status;
     uint8_t    data[0];
}__attribute__ ((packed)) no_evt_cmd_complete;

#define NO_EVT_LE_META_EVENT	0x3E
typedef struct {
	uint8_t		leMetaEventType;
	uint8_t		leMetaEventStatus;
	uint8_t		data[0];
} __attribute__ ((packed)) no_evt_le_meta_event;

#define NO_ACL_START_LEN_IDENTIC		0x02
typedef struct {
	uint16_t		cit;
	uint16_t		len;
	uint8_t		    data[0];
} __attribute__ ((packed)) no_acl_start_len_identic;

typedef struct {
	uint16_t		cit;
	uint16_t		len;
	uint16_t		expectedlen;
	std::vector<uint8_t>   byarr;
}  no_acl_start_len_dynamic;


struct bt_hci_evt_hdr {
	uint8_t  evt;
	uint8_t  plen;
} __attribute__ ((packed));

#define BT_H4_CMD_PKT	0x01
#define BT_H4_ACL_PKT	0x02
#define BT_H4_SCO_PKT	0x03
#define BT_H4_EVT_PKT	0x04

/*
typedef struct {
	uint16_t	opcode;
	uint8_t		plen;
}  __attribute__ ((packed))  hci_command_hdr;

typedef struct {
	uint8_t		evt;
	uint8_t		plen;
    uint8_t	    data[0];
}  __attribute__ ((packed))  hci_event_pckt;

*/

enum HCI_STATE
{
    STATE_UNSUPORTED=1,
    STATE_UNAUTHORISED,
    STATE_NETWORK_DOWN,
    STATE_POWEREDON,
    STATE_POWEREDOFF,
};

enum HCI_ADDRTYPE
{
    ADDR_RANDOM,
    ADDR_PUBLIC
};

#define CMD_OPCODE_PACK(ocf, ogf)	htobs((uint16_t)((ocf & 0x03ff)|(ogf << 10)))
#define CMD_OPCODE_OGF(op)		    (op >> 10)
#define CMD_OPCODE_OCF(op)		    (op & 0x03ff)


// connection request
#define LINK_CR_SCO   0x00  // SCO Connection requested
#define LINK_CR_ACL   0x01 // ACL Connection requested
#define LINK_CR_ESCO  0x02 // eSCO Connection requested


/******************************************************************************
*/
class bu_gatt;
class hci_event
{
public:
    virtual void on_disconnect(const evt_disconn_complete* evdc)=0;
    virtual void on_encrypt_chnage(const evt_encrypt_change* pecc)=0;
    virtual void on_acl_packet(uint16_t handle, uint16_t cid, const sdata& data)=0;
    virtual void on_read_version(uint8_t hciver, uint16_t hcirev, uint8_t lmpver, uint16_t man, uint16_t lmpsubver)=0;
    virtual void on_mac_change(const bdaddr_t& addr)=0;
    virtual void on_adv_status(HCI_STATE status)=0;
    virtual void on_adv_data_status(uint8_t status)=0;
    virtual void on_scan_resp_datat_status(uint8_t status)=0;
    virtual void on_adv_enable(uint8_t status)=0;
    virtual void on_rssi(uint16_t handle, uint8_t rssi)=0;
    virtual void le_ltk_neg_reply(uint16_t handle)=0;
    virtual void on_le_connected(uint8_t status, uint16_t handle,
                                uint8_t role,
                                HCI_ADDRTYPE addressType,
                                const bdaddr_t& address,
                                uint16_t interval,
                                uint16_t latency,
                                uint16_t  supervisionTimeout=0,
                                uint8_t masterClockAccuracy=0)=0;
    virtual void on_le_conn_update_complette_shit(uint8_t status,
                                                    uint16_t handle,
                                                    uint16_t interval,
                                                    uint16_t latency,
                                                    uint16_t supervisionTimeout)=0;
    virtual void on_dev_status(bool onoff)=0;
    virtual void on_configure_device(int devid)=0;
};



/******************************************************************************
*/
class SrvDevice;
class bu_hci   : public  hci_data_eater             // : public OsThread
{
public:
    friend class SrvDevice;

    bu_hci(SrvDevice* pev);
    virtual ~bu_hci();
    bool init(int& devid, bool userchannel);
    bool pool();
    void send_cmd(uint16_t ogf, uint16_t ocf, uint8_t plen, void *param);
    void set_adv_data(const sdata& data);
    void set_sca_res_data(const sdata& data);
    void enable_adv(uint8_t enable);
    void disconnect(uint16_t handle, uint8_t reason);
    void read_rssi(uint16_t handle);
    void write_ack_packet(uint16_t handle, uint16_t cid, const sdata& datah);
    void reset();
    void read_baddr();
    bool start(int delay=0);
    SrvDevice* srv()const{ return _pev;}
    bool check_dev_state();
    int read_local_name();
    void write_local_name(const char* name);
    virtual void on_error(const hci_error& error);
    virtual bool onSpin(bt_socket* sock);
    virtual int on_sock_data(uint8_t code, const sdata& buffer);

protected:
    int  _on_hci_data(uint8_t code, const sdata& buffer);
    int  _on_acl_data(uint8_t code, const sdata& buffer);
    void _oncmd_complette(const no_evt_cmd_complete* nevcc);
    void _onmeta(const no_evt_le_meta_event* neleme);
    void _onle_complette(const no_evt_le_meta_event* neleme);
    void _onle_con_update_complette(const no_evt_le_meta_event* neleme);
    void _reconfigure();
    void _spinpool(int lops=32);
    void _set_hci_filter();
    void _set_event_mask();
    void _set_le_event_mask();
    void _read_version();
    void _read_baddr();
    void _write_le_host();
    void _read_le_hosts();
    void _set_adv_params(uint16_t mini=1, uint16_t mmaxi=4, uint8_t advtype=9);
    void _onhci_state_chnaged(HCI_STATE);
    void _write_sock(const uint8_t* pt, size_t sz){
        if(_delay)
            ::usleep(_delay);
        int ret = _socket->write(pt, sz);
        if((size_t)ret != sz)
        {
            _THROW("socket send error");
        }
    }
    template <typename T>void _write_sock(const T& t, size_t sz=sizeof(T))
    {
        int ret = _socket->write(t, sz);
        if(ret != sz)
        {
            _THROW("socket send error");
        }
    }
    void _clear_cache();
    bool _recreate_sock();

private:
    int                 _devid;
    SrvDevice*          _pev;
    hci_socket_ble*     _socket;
    bool                _isDevUp;
    HCI_STATE           _state;
    HCI_ADDRTYPE        _addrtype;
    bdaddr_t            _address;
    bool                _chekingdev;
    bool                _connected;
    evt_disconn_complete _dcached;
    int                  _delay;
    std::map<uint16_t,no_acl_start_len_dynamic*> _cache;
};
#endif // NO_HCI_H
