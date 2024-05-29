#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>

#include "excercise.pb.h"
#include <pb_encode.h>

// TODO
// exchange client and server who sends what
// add interrupt handler to properly exit
// extend message with len fields

void send_data_to_client(int socket)
{
    hello_message message = hello_message_init_zero;
    message.a_number = 42;
    char buffer[64];

    pb_ostream_t output_stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    if (!pb_encode(&output_stream, hello_message_fields, &message))
    {
        printf("could not encode!\n");
        return;
    }

    send(socket, buffer, output_stream.bytes_written, 0);
}

int main()
{
    printf("hell0 server\n");

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        printf("cannot create socket: %d %s\n", errno, strerror(errno));
        return 1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8081);
    address.sin_addr.s_addr = INADDR_ANY;

    int bind_result = bind(server_socket, (struct sockaddr *) &address, sizeof(address));

    if (bind_result == -1)
    {
        printf("cannot bind socket: %d %s\n", errno, strerror(errno));
        close(server_socket);
        return 1;
    }

    int listen_result = listen(server_socket, 0);

    if (listen_result == -1)
    {
        printf("cannot listen to socket: %d %s\n", errno, strerror(errno));
        close(server_socket);
        return 1;
    }

    for (;;)
    {
        int client_socket = accept(server_socket, NULL, NULL);

        if (client_socket == -1)
        {
            printf("cannot accept: %d %s\n", errno, strerror(errno));
            close(server_socket);
            return 1;
        }

        printf("accepted new connection\n");

        send_data_to_client(client_socket);

        close(client_socket);
    }

    close(server_socket);
}
