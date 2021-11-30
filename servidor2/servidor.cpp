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
//VITOR WOLFF BORDIGNON
//VARIAVEIS GLOBAIS SOCKET
pthread_t tid0,tid1,tid2;
int sock,length,s0,s1,s2;
struct sockaddr_in server;

void *escutatp0(void *){
    int mediaTemperatura;
    while(1){
        recv(s0, &mediaTemperatura, sizeof(mediaTemperatura), 0); 
        printf("ALERTA DE TEMPERATURA ACIMA DE 70°, MEDIA ATUAL: %d\n", mediaTemperatura);
    }
}

void *escutatp1(void *){
    int linferior;
    while(1){
        recv(s1, &linferior, sizeof(linferior), 0); 
        printf("ALERTA DE LIMITE INFERIOR MENOR QUE 10, LIMITE INFERIOR ATUAL: %d\n", linferior);
    }
}

void *escutatp2(void *){
    int lsuperior;
    while(1){
        recv(s2, &lsuperior, sizeof(lsuperior), 0); 
        printf("ALERTA DE LIMITE SUPERIOR MAIOR QUE 80, LIMITE SUPERIOR ATUAL: %d\n", lsuperior);
    }
}


int main(){

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
    printf("SERVER: Aguardando conexões!\n");  
    //FIM LISTEN

    //4:ACCEPT;
    s0 = accept(sock,(struct sockaddr *)0,0);
    s1 = accept(sock,(struct sockaddr *)0,0);
    s2 = accept(sock,(struct sockaddr *)0,0);
    printf("SERVER: Conexões estabelecidas!\n");  
    //FIM ACCEPT

    pthread_create(&tid0,NULL,escutatp0,NULL);
    pthread_create(&tid1,NULL,escutatp1,NULL);
    pthread_create(&tid2,NULL,escutatp2,NULL);

    pthread_join(tid0,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

}
