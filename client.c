#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

#include "excercise.pb.h"
#include <pb_decode.h>

#define RECV_BUFFER_SIZE 64

void process_data(const char* buffer, size_t len)
{
    hello_message message = hello_message_init_zero;

    pb_istream_t input_stream = pb_istream_from_buffer(buffer, len);
    if (!pb_decode(&input_stream, hello_message_fields, &message))
    {
        printf("could not encode!\n");
        return;
    }

    printf("server says: %d\n", (int)message.a_number);
}

int main()
{
    printf("hell0 client\n");

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket == -1)
    {
        printf("cannot create socket: %d %s\n", errno, strerror(errno));
        return 1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8081);

    // converts network address from character string representation into
    // bytes in network byte order. not checking return value, because we know
    // it is a valid address.
    inet_aton("127.0.0.1", (struct in_addr *) &(address.sin_addr.s_addr));

    // connect to a given address.
    int connect_result = connect(client_socket, (struct sockaddr *) &address, sizeof(address));

    if (connect_result == -1)
    {
        printf("cannot create socket: %d %s\n", errno, strerror(errno));
        close(client_socket);
        return 1;
    }

    char recv_buffer[RECV_BUFFER_SIZE] = "";

    ssize_t recv_result = recv(client_socket, recv_buffer, RECV_BUFFER_SIZE, 0);

    if (recv_result == -1)
    {
        printf("recv error: %d %s\n", errno, strerror(errno));
        close(client_socket);
        return 1;
    }

    process_data(recv_buffer, recv_result);

    close(client_socket);
}
