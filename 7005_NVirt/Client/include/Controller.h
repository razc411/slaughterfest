#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "CommandController.h"
#include "TransferController.h"
#include "definitions.h"

class Controller
{
    public:
        Controller();
        virtual ~Controller();

        void execute();
        void recieve_data();
        int transmit_data();
        int write_udp_socket(struct packet_hdr * packet);
        int create_udp_socket(int port);
        void notify_terminal(int type, struct packet_hdr * pkt);

        int send_ack(int seq, char * dest_ip);
        void check_packet(struct packet_hdr * packet);
    protected:
    private:
        clock_t timer;
        double timeout = CLOCKS_PER_SEC * 5;
        bool timer_enabled;
        int ctrl_socket;
        CommandController * cmd_control;
};
#endif // CONTROLLER_H
