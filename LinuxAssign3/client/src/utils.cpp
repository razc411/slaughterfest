#include "client_defs.h"
#include "utils.h"

/*------------------------------------------------------------------------------------------------------------------
--      SOURCE FILE:    utils.cpp -An application that will allow users to join chat channel
--                                      with asynchronous socket communcation using Select
--
--      PROGRAM:        client_chat
--
--      FUNCTIONS:		int dispatch_thread(void *(*function)(void *), void *params, pthread_t *handle)
--						int read_pipe(int fd, void *buf, size_t count)
--						int write_pipe(int fd, const void *buf, size_t count)
--						
--						int create_accept_socket()
--						int accept_new_client(int listen_sd)
--						
--						int write_packet(int sockfd, int type, void * packet)
--						void serialize_cjoin(void* packet, int sockfd)
--						void serialize_cinfo(void* packet, int sockfd)
--						void serialize_smsg_skick(void* packet, int sockfd)
--						
--						void* read_packet(int sockfd, int * type)
--						void * recieve_smsg_skick(int sockfd)
--						void * recieve_cinfo(int sockfd)
--						void * recieve_cmsg(int sockfd)
--						void * recieve_cquit(int sockfd)
--						void * recieve_cjoin(int sockfd)
--						void rcv_variable(int sockfd, void * incoming, int size)
--
--      DATE:           March 8, 2014
--
--      REVISIONS:      (Date and Description)
--
--      DESIGNER:       Ramzi Chennafi
--
--      PROGRAMMER:     Ramzi Chennafi
--
--      NOTES:
--      Contains common helper functions.
----------------------------------------------------------------------------------------------------------------------*/

int packet_sizes[] = {
	((sizeof(char) * MAX_USER_NAME) + (sizeof(char) * MAX_CHANNEL_NAME) + (sizeof(char) * MAX_STRING)),
	((sizeof(char) * MAX_USER_NAME) + (sizeof(char) * MAX_CHANNEL_NAME) + (sizeof(char) * MAX_STRING)),
	((sizeof(int) * 2) + (sizeof(char) * MAX_CHANNEL_NAME) + (((sizeof(char) * MAX_USER_NAME) * MAX_CLIENTS))),
	0, // unset
	(sizeof(char) * MAX_STRING),
	sizeof(int),
	((sizeof(char) * MAX_USER_NAME) + (sizeof(char) * MAX_CHANNEL_NAME) + sizeof(int))
};

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       dispatch_thread
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      int dispatch_thread(void *(*function)(void *), void *params, pthread_t *handle)
--						void *(*function)(void *) - function to be put on thread
--						void *params, pthread_t *handle - handle to the thread
--
--      RETURNS:        int
--
--      NOTES:
--      Wrapper for starting a new thread.
----------------------------------------------------------------------------------------------------------------------*/
int dispatch_thread(void *(*function)(void *), void *params, pthread_t *handle)
{
	if(pthread_create(handle, NULL, function, params) == -1)
	{
		perror("dispatch_thread");
		return -1;
	}

	pthread_detach(*handle);
	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       read_pipe
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      int read_pipe(int fd, void *buf, size_t count)
--						int fd - file descriptor for read pipe
--						void *buf - buffer to store messages
--						size_t count - size of buffer
--
--      RETURNS:        int
--
--      NOTES:
--      Wrapper to read from a pipe.
----------------------------------------------------------------------------------------------------------------------*/
int read_pipe(int fd, void *buf, size_t count)
{
    int ret = read(fd, buf, count);
    
    if(ret == -1)
        perror("read_pipe");        

    return ret;
}
/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       write_pipe
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      int write_pipe(int fd, const void *buf, size_t count)
--						int fd - file descriptor for read pipe
--						const void *buf - buffer to store messages
--						size_t count - size of buffer
--
--      RETURNS:        int
--
--      NOTES:
--      Wrapper to write to a pipe.
----------------------------------------------------------------------------------------------------------------------*/
int write_pipe(int fd, const void *buf, size_t count)
{
    int ret = write(fd, buf, count);
    
    if(ret == -1)
        perror("write_pipe");        

    return ret;
}

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       create_accept_socket
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      int create_accept_socket()
--
--      RETURNS:        int
--
--      NOTES:
--      creates an accepting socket
----------------------------------------------------------------------------------------------------------------------*/
int create_accept_socket(){

    int listen_sd, arg = 1;
    struct sockaddr_in server;

    if ((listen_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Cannot Create Socket!");
    	return -1;
    }
    if (setsockopt (listen_sd, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg)) == -1)
    {
        perror("setsockopt");
        return -2;
    }

    bzero((char *)&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(TCP_PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_sd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("bind error");
        return -3;
    }

    listen(listen_sd, 5);

    return listen_sd;
}

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       accept_new_client
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      int accept_new_client(int listen_sd)
--						int listen_sd - socket descriptor of the listen socket
--
--      RETURNS:        int
--
--      NOTES:
--      Accepts a new client.
----------------------------------------------------------------------------------------------------------------------*/
int accept_new_client(int listen_sd)
{
	unsigned int client_len;
	int client_sd = 0;
	struct sockaddr_in client_addr;

	client_len = sizeof(client_addr);
	if ((client_sd = accept(listen_sd, (struct sockaddr *) &client_addr, &client_len)) == -1)
	{
	    perror("accept error");
	    return -1;
	}
	
	printf(" Remote Address:  %s\n", inet_ntoa(client_addr.sin_addr));

	return client_sd;
}

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       read_packet
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      void* read_packet(int sockfd, int * type)
--						int sockfd - socket descriptor for a socket
--						int * type - type of message to determine if its control packet or message
--
--      RETURNS:        void*
--
--      NOTES:
--      Reads an incoming packet on the sockfd, sets the type and returns the packet.
----------------------------------------------------------------------------------------------------------------------*/
void* read_packet(int sockfd, int * type)
{
	int bytes_to_read = TYPE_SIZE;
	int numread = 0;
	
	while ((numread = read(sockfd, type, bytes_to_read)) > 0)
	{
		bytes_to_read -= numread;
	}

	switch(*type)
	{
		case SERVER_MSG_PKT:
			return recieve_smsg_skick(sockfd);
		break;

		case CHANNEL_INFO_PKT:
			return recieve_cinfo(sockfd);
		break;

		case CHANNEL_ERROR:
			//someday
		break;

		case CLIENT_MSG_PKT:
			return recieve_cmsg(sockfd);
		break;

		case CLIENT_QUIT_PKT:
			return recieve_cquit(sockfd);
		break;

		case CLIENT_JOIN_PKT:
			return recieve_cjoin(sockfd);
		break;
	}

	return NULL;
}

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       write_packet
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      int write_packet(int sockfd, int type, void * packet)
--						int sockfd - socket descriptor to write on
--						int type - the type of packet that will be written	
--						void * packet - the packet to write			
--
--      RETURNS:        int
--
--      NOTES:
--      Writes the passed in packet to the sockfd. Returns the type of packet written (should be the same as the type
--		specified in the interface on success.)
----------------------------------------------------------------------------------------------------------------------*/
int write_packet(int sockfd, int type, void * packet)
{
	write(sockfd, &type, TYPE_SIZE);
	switch(type)
	{
		case SERVER_MSG_PKT:
		case SERVER_KICK_PKT:
			serialize_smsg_skick(packet, sockfd);
		break;

		case CHANNEL_INFO_PKT:
			serialize_cinfo(packet, sockfd);
		break;

		case CHANNEL_ERROR:
			//someday
		break;

		case CLIENT_MSG_PKT:
			write(sockfd, ((C_MSG_PKT*)packet)->msg, packet_sizes[type]);
		break;

		case CLIENT_QUIT_PKT:
			write(sockfd, &((C_QUIT_PKT*)packet)->code, packet_sizes[type]);
		break;

		case CLIENT_JOIN_PKT:
			serialize_cjoin(packet, sockfd);
		break;

	}

	return type;
}

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       serialize_cjoin
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      void serialize_cjoin(void* packet, int sockfd)
--						void * packet - control packet	
--						int sockfd - descriptor to send data on
--
--      RETURNS:        void
--
--      NOTES:
--      Serializes the cjoin struct over the specified descriptor.
----------------------------------------------------------------------------------------------------------------------*/
void serialize_cjoin(void* packet, int sockfd)
{
	C_JOIN_PKT * cjoin = (C_JOIN_PKT*) packet;

	write(sockfd, (void*)cjoin->client_name, MAX_USER_NAME);
	write(sockfd, (void*)cjoin->channel_name, MAX_CHANNEL_NAME);
	write(sockfd, &cjoin->tcp_socket, sizeof(int));
}

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       serialize_smsg_skick
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      void serialize_smsg_skick(void* packet, int sockfd)
--						void * packet - control packet	
--						int sockfd - descriptor to send data on
--
--      RETURNS:        void
--
--      NOTES:
--      Serializes the smsg struct over the specified descriptor.
----------------------------------------------------------------------------------------------------------------------*/
void serialize_smsg_skick(void* packet, int sockfd)
{
	S_MSG_PKT * smsgkick = (S_MSG_PKT*) packet;

	write(sockfd, (void*)smsgkick->client_name, MAX_USER_NAME);
	write(sockfd, (void*)smsgkick->msg, MAX_STRING);
	write(sockfd, (void*)smsgkick->channel_name, MAX_CHANNEL_NAME);
}

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       serialize_cinfo
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      void serialize_cinfo(void* packet, int sockfd)
--						void * packet - control packet	
--						int sockfd - socket descriptor for socket
--
--      RETURNS:        void
--
--      NOTES:
--      Serialzies the specified channel info struct over the specified descriptor.
----------------------------------------------------------------------------------------------------------------------*/
void serialize_cinfo(void* packet, int sockfd)
{
	S_CHANNEL_INFO_PKT * cinfo = (S_CHANNEL_INFO_PKT*) packet;

	write(sockfd, &cinfo->code, sizeof(int));
	write(sockfd, &cinfo->num_clients, sizeof(int));
	write(sockfd, (void*)cinfo->channel_name, MAX_CHANNEL_NAME);

	for(int i = 0; i < MAX_CLIENTS; i++)
		write(sockfd, (void*)cinfo->channel_clients[i], MAX_USER_NAME);
}

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       receive_smsg_skick
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      void * recieve_smsg_skick(int sockfd)
--						int sockfd - descriptor to recieve data on
--
--      RETURNS:        void*
--
--      NOTES:
--      Recieves the server message packet. Returns a pointer to it.
----------------------------------------------------------------------------------------------------------------------*/
void * recieve_smsg_skick(int sockfd)
{
	S_MSG_PKT * smsgkick = (S_MSG_PKT*) malloc(packet_sizes[SERVER_MSG_PKT]);

	rcv_variable(sockfd, smsgkick->client_name, MAX_USER_NAME);
	rcv_variable(sockfd, smsgkick->msg, MAX_STRING);
	rcv_variable(sockfd, smsgkick->channel_name, MAX_CHANNEL_NAME);
	return smsgkick;
}

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       receive_cinfo
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      void * recieve_cinfo(int sockfd)
--						int sockfd - descriptor to recieve data on
--
--      RETURNS:        void*
--
--      NOTES:
--      Recieves the channel info packet. Returns a pointer to it.
----------------------------------------------------------------------------------------------------------------------*/
void * recieve_cinfo(int sockfd)
{
	S_CHANNEL_INFO_PKT * cinfo = (S_CHANNEL_INFO_PKT*) malloc(packet_sizes[CHANNEL_INFO_PKT]);

	rcv_variable(sockfd, &cinfo->code, sizeof(int));
	rcv_variable(sockfd, &cinfo->num_clients, sizeof(int));
	rcv_variable(sockfd, cinfo->channel_name, MAX_CHANNEL_NAME);

	for(int i = 0; i < MAX_CLIENTS; i++)
		rcv_variable(sockfd, (void*)cinfo->channel_clients[i], MAX_USER_NAME);

	return cinfo;
}

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       receive_cmsg
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      void * recieve_cmsg(int sockfd)
--						int sockfd - descriptor to recieve data on
--
--      RETURNS:        void*
--
--      NOTES:
--      Recieves the client message packet. Returns a pointer to it.
----------------------------------------------------------------------------------------------------------------------*/
void * recieve_cmsg(int sockfd)
{
	C_MSG_PKT * cmsg = (C_MSG_PKT*) malloc(packet_sizes[CLIENT_MSG_PKT]);

	rcv_variable(sockfd, cmsg->msg, MAX_STRING);

	return cmsg;
}

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       recieve_cquit
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      void *recieve_cquit(int sockfd)
--						int sockfd - socket descriptor for socket
--
--      RETURNS:        void*
--
--      NOTES:
--      Recieves the client quit message packet. Returns a pointer to it.
----------------------------------------------------------------------------------------------------------------------*/
void *recieve_cquit(int sockfd)
{
	C_QUIT_PKT * cquit = (C_QUIT_PKT*) malloc(packet_sizes[CLIENT_QUIT_PKT]);

	rcv_variable(sockfd, &cquit->code, sizeof(int));

	return cquit;
}

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       recieve_cjoin
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      void * recieve_cjoin(int sockfd)
--						int sockfd - socket descriptor for socket
--
--      RETURNS:        void*
--
--      NOTES:
--      Recieves the client join message packet. Returns a pointer to it.
----------------------------------------------------------------------------------------------------------------------*/
void * recieve_cjoin(int sockfd)
{
	C_JOIN_PKT * packet = (C_JOIN_PKT*) malloc(packet_sizes[CLIENT_JOIN_PKT]);

	rcv_variable(sockfd, packet->client_name, MAX_USER_NAME);
	rcv_variable(sockfd, packet->channel_name, MAX_CHANNEL_NAME);
	rcv_variable(sockfd, &packet->tcp_socket, sizeof(int));

	return packet;
}

/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION:       rcv_variable
--
--      DATE:           March 8 2014
--      REVISIONS:      none
--
--      DESIGNER:       Ramzi Chennafi
--      PROGRAMMER:     Ramzi Chennafi
--
--      INTERFACE:      void rcv_variable(int sockfd, void * incoming, int size)
--						int sockfd - socket descriptor for socket
--						void * incoming - incoming message
--						int size - message size
--
--      RETURNS:        void
--
--      NOTES:
--      Recieves a specified variable over a socket. Works for both pipes and tcp sockets.
----------------------------------------------------------------------------------------------------------------------*/
void rcv_variable(int sockfd, void * incoming, int size)
{
	int bytes_to_read = size;
	char * buf;
	int numread;

	buf = (char*) incoming;
	while ((numread = read(sockfd, buf, bytes_to_read)) > 0)
	{
		buf += numread;
		bytes_to_read -= numread;
	}
}