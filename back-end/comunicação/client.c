#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <binn.h>

#define ACESSO_PERMITIDO 1
#define ACESSO_NEGADO 0

#define PORT 8888
#define BUFFER_SIZE 32

typedef struct
{
    char token[BUFFER_SIZE];
    int file_description;
    double coord[2];

} device_t;

binn *serialize(device_t *device)
{
    binn *coord;

    coord = binn_list();
    binn_list_add_double(coord, device->coord[0]);
    binn_list_add_double(coord, device->coord[1]);
    binn_list_add_str(coord, device->token);

    return coord;
}

int main(int argc, char const *argv[])
{
    int sock = 0;
    int acesso;
    binn *data;

    device_t device;

    strcpy(device.token, "5192E6266BD065EB82D5C8EE53891AA0");

    device.coord[0] = -15.765940453;
    device.coord[1] = -47.872187540;

    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    /* Convert IPv4 and IPv6 addresses from text to binary form */
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock, device.token, strlen(device.token), 0);
    printf("%s\n", device.token);

    read(sock, &acesso, sizeof(acesso));

    while (1)
    {

        data = serialize(&device);
        send(sock, binn_ptr(data), binn_size(data), 0);
        read(sock, binn_ptr(data), binn_size(data));

        device.coord[0] = binn_list_double(data, 1);
        device.coord[1] = binn_list_double(data, 2);
        printf("%lf %lf\n", device.coord[0], device.coord[1]);
        sleep(1);
    }

    close(sock);

    return 0;
}
