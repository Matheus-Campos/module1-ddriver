#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define DEVICE "/dev/module1device"

int main(void)
{
        int i, fd;
        char ch, write_buf[100], read_buf[100];

        fd = open(DEVICE, O_RDWR); // open device

        if (fd == -1)
        {
                printf("arquivo %s nao existe ou esta bloqueado por outro processo\n", DEVICE);
                exit(-1);
        }

        printf("r para ler do driver\nw para escrever no driver\ndigite o comando: ");
        scanf("%c", &ch);

        switch (ch)
        {
        case 'w':
                printf("digite a entrada\n");
                scanf(" %[^\n]", write_buf);
                write(fd, write_buf, sizeof(write_buf));
                break;
        case 'r':
                read(fd, read_buf, sizeof(read_buf));
                printf("dispositivo: %s\n", read_buf);
                break;
        default:
                printf("comando nao reconhecido\n");
                break;
        }

        return 0;
}
