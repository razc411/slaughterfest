#include "TransferController.h"

TransferController::TransferController(char * ip_dest, size_t t_size, size_t win_size):
    current_ack(0), current_seq(1), current_window(1), last_acked_seq(0), ipdest(ip_dest),
    transfer_size(t_size), window_size(win_size), packet_size(P_SIZE)
{
    buffer = (char*)malloc(t_size);
    memset(buffer, 0, sizeof(t_size));
}

TransferController::~TransferController()
{
    free(buffer);
}

int TransferController::readNextPacket(char &packet)
{
    if(current_seq <= win_size * current_window){
        memcpy(buffer[(current_seq - 1) * packet_size], packet, packet_size);
        current_seq++;
        return 1;
    }
    return 0;
}

void TransferController::readPacket(char &packet, int sequence_num)
{
    memcpy(buffer[(sequence_num - 1) * packet_size], packet, packet_size);
}

int TransferController::veryifyAck(struct packet_hdr packet)
{
    if(packet.ack_value == current_ack + 1)
    {
        last_acked_seq = packet.sequence_number;

        if(((current_window * win_size)) == current_sequence)
        {
            current_window++;
            return 2;
        }
        return 1;
    }
    return 0;
}

void TransferController::write_packet_buffer()
{
    for(int i = 0; i < transfer_size/packet_size; i++)
    {
        struct packet_hdr packet_data;
        memcpy(&ipdest, packet_data.dest_ip, sizeof(ipdest));
        //memcpy(&ipsrc, packet_data.src_ip, sizeof(ipsrc));
        packet_data.ack_value = i;
        packet_data.sequence_number = i + 1;
        packet_data.window_size = transfer_size/packet_size;
        packet_data.ptype = DATA;
        memset(&packet_data.data, 'A', sizeof(packet_data.data));
        memcpy(&packet_data, &buffer[packet_size * i], packet_size);
    }
}