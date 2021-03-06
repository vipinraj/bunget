/**
    Copyright: zirexix 2016

    This program is distributed
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    This program, or portions of it cannot be used in commercial
    products without the written consent of the author: zyraxes.comatgmaildotcom.

*/

#ifndef HCI_SOCKET_H
#define HCI_SOCKET_H

#include "bt_socket.h"

class hci_data_eater;
class l2cap_socket;
class hci_socket_ble : public bt_socket
{
public:
     hci_socket_ble(hci_data_eater* hci):bt_socket(hci),_devId(0),_adaptivespeed(LOOP_SLEEP_IDLE),_loops(0){}
     ~hci_socket_ble();
    void create();
    void create_bind(int dev_id);
    void stop();
    void bind(const bdaddr_t& addr, uint8_t channel, uint16_t psm=0, uint8_t cid=0){};
    int bind_raw(int* devId);
    int bind_user(int* devId);
    void bind_ctrl();
    bool is_up();
    void set_filter(const uint8_t* data, int length);
    int read(uint8_t* buffer, int sizeb);
    bool pool(int* pbytes=0);
#ifdef USE_UVLIB  /// this was not tested !?!
    static void uv_this_cb(uv_poll_t* handle, int status, int events);
    static void uv_this_cb_close(uv_poll_t* handle);
#endif //
private:
    int _resolve_devid(int* pDevId, bool isUp);
    void _notify_read();
private:
    int         _devId;
    std::map<uint16_t,l2cap_socket*> _l2sockets;
#ifdef USE_UVLIB
    uv_poll_t   _pollHandle;
#endif //
    int         _checkfreq;
    int         _adaptivespeed;
    int         _loops;
};

#endif // HCI_SOCKET_H
