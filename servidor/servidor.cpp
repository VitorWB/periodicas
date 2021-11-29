#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#define PORT 6325

//VARIAVEIS GLOBAIS SOCKET
int sock,length,s0,var;
struct sockaddr_in server,client;

typedef struct info{
    int id;
    int info;
}s_info;

int main(){
    s_info s;

    //1:SOCKET
    sock = socket(AF_INET, SOCK_STREAM, 0); 
        if (sock < 0) {
            perror("opening stream socket");
            exit(0);
        }
    //FIM SOCKET

    //2:BIND
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons (PORT);
    length = sizeof (server);
        if (bind(sock, (struct sockaddr *)&server, length) < 0){
            perror("binding stream socket");
            exit(0);
        }
    //FIM BIND

    //3:LISTEN
    listen(sock,5);
    printf("SERVER: Aguardando conexão!\n");  
    //FIM LISTEN

    //4:ACCEPT;
    s0 = accept(sock,(struct sockaddr *)0,0);  
    printf("SERVER: Conexão estabelecida!\n");  
    //FIM ACCEPT

    while(1){
        recv(s0, &s, sizeof(s), 0);
        // printf("Sensor %d: %d\n",s.id,s.info);
        if(s.id==3){
            if(s.info>70&&s.info<101){
                printf("ALERTA DE TEMPERATURA MÉDIA SUPERIOR A 70 - SENSOR[%d]:%d\n",s.id,s.info);
            }else if(s.info==123){
                printf("ALERTA DE LIMITE INFERIOR - SENSOR[%d]:%d\n",s.id,s.info);
            }else if(s.info==321){
                printf("ALERTA DE LIMITE SUPERIOR - SENSOR[%d]:%d\n",s.id,s.info);
            }
        }
    }
}
