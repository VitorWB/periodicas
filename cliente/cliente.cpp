#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define PORT 6325
#define IP_SERVER "172.17.0.2"

//VITOR WOLFF BORDIGNON
//LIMITE INFERIOR = 10
//LIMITE SUPERIOR = 80
//LIMITE TEMPERATURA = 70

pthread_t tid0,tid1,tid2,tid3; // TREADS
int s0, s1, s2, temperatura[10], linferior, lsuperior, Tindex = 0; 

void *tp0(void *){ // THREAD QUE GERA TEMPERATURA
    struct sockaddr_in server;
    //1:SOCKET
    s0=socket(AF_INET, SOCK_STREAM, 0);
        if (s0<0){
            perror("opening stream socket");
            exit(1);
        }
    //1:FIM SOCKET

    //2:CONEXAO COM SERVIDOR
    bzero(&server, sizeof(server)) ;
    server.sin_family = AF_INET ;
    server.sin_port = htons(PORT) ;
    server.sin_addr.s_addr = inet_addr(IP_SERVER);

    if (connect(s0, (struct sockaddr *) &server, sizeof(server))){
        perror("connectando stream socket");
        exit(0);
    }
    printf("SENSOR TP0: Conectado!\n");
    //2:FIM CONEXAO

    int temp;
    while(1){
        sleep(1);
        temp = rand()%100;

        if(Tindex < 10){
            temperatura[Tindex] = temp;
        } else{
            for (size_t i = 0; i < 10; i++){
                if(i < 9){
                    temperatura[i] = temperatura[i+1];
                } else{
                    temperatura[i] = temp;
                }
            }
            
        }

        Tindex++;
    }
}

void *tp1(void *){ // THREAD QUE GERA LIMITE INFERIOR
    struct sockaddr_in server;
    //1:SOCKET
    s1=socket(AF_INET, SOCK_STREAM, 0);
        if (s1<0){
            perror("opening stream socket");
            exit(1);
        }
    //1:FIM SOCKET

    //2:CONEXAO COM SERVIDOR
    bzero(&server, sizeof(server)) ;
    server.sin_family = AF_INET ;
    server.sin_port = htons(PORT) ;
    server.sin_addr.s_addr = inet_addr(IP_SERVER);

    if (connect(s1, (struct sockaddr *) &server, sizeof(server))){
        perror("connectando stream socket");
        exit(0);
    }
    printf("SENSOR TP1: Conectado!\n");
    //2:FIM CONEXAO

    while(1){
        sleep(2);
        linferior = rand()%100;
    }
}

void *tp2(void *){ // THREAD QUE GERA LIMITE SUPERIOR
    struct sockaddr_in server;
    //1:SOCKET
    s2=socket(AF_INET, SOCK_STREAM, 0);
        if (s2<0){
            perror("opening stream socket");
            exit(1);
        }
    //1:FIM SOCKET

    //2:CONEXAO COM SERVIDOR
    bzero(&server, sizeof(server)) ;
    server.sin_family = AF_INET ;
    server.sin_port = htons(PORT) ;
    server.sin_addr.s_addr = inet_addr(IP_SERVER);

    if (connect(s2, (struct sockaddr *) &server, sizeof(server))){
        perror("connectando stream socket");
        exit(0);
    }
    printf("SENSOR TP1: Conectado!\n");
    //2:FIM CONEXAO

    while(1){
        sleep(3);
        lsuperior = rand()%100;
    }
}

int calculaMediaTemperatura(){
    int soma = 0, media = 0, tamanho;

    if(Tindex < 10){
        tamanho = Tindex;
    } else{
        tamanho = 10;
    }

    for (size_t i = 0; i < tamanho; i++){
        soma += temperatura[i];
    }

    media = soma/tamanho;
    return media;
}

void *ta(void *){
    while(1){
        sleep(4);
        int mediaTemperatura;
        mediaTemperatura = calculaMediaTemperatura();

        printf("\n\n");
        printf("Temperatura média: %d\n", mediaTemperatura);
        printf("Limite inferior: %d\n", linferior);
        printf("Limite superior: %d\n", lsuperior);

        if(mediaTemperatura > 70){
            printf("Enviando para o servidor que a media de temperatura superou 70\n");
            send(s0,&mediaTemperatura,sizeof(mediaTemperatura),0);
        }

        if(linferior < 10){
            printf("Enviando para o servidor que o limite inferior é menor que 10\n");
            send(s1,&linferior,sizeof(linferior),0);
        }

        if(lsuperior > 80){
            printf("Enviando para o servidor que o limite superior superou 80\n");
            send(s2,&lsuperior,sizeof(lsuperior),0);
        }
    }
    
}

int main() {
    srand( (unsigned)time(NULL));
    pthread_create(&tid0,NULL,tp0,NULL);
    pthread_create(&tid1,NULL,tp1,NULL);
    pthread_create(&tid2,NULL,tp2,NULL);
    pthread_create(&tid3,NULL,ta,NULL);

    pthread_join(tid0,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);  
}