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
#define IP_SERVER "172.17.0.3"

//LIMITE INFERIOR = 20
//LIMITE SUPERIOR = 100
//LIMITE TEMPERATURA = 70

pthread_t tid0,tid1,tid2,tid3; // TREADS
int s0, temperatura[10], linferior[10], lsuperior[10]; //SOCKET E ARRAYS
int Tindex = 0, Linfindex = 0, Lsupindex = 0; // VARIAVEIS PARA CONTROLE DE INDEX DOS ARRAYS

void *tp0(void *){ // THREAD QUE GERA TEMPERATURA
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
    int linf;
    while(1){
        sleep(2);
        linf = rand()%100;

        if(Linfindex < 10){
            linferior[Linfindex] = linf;
        } else{
            for (size_t i = 0; i < 10; i++){
                if(i < 9){
                    linferior[i] = linferior[i+1];
                } else{
                    linferior[i] = linf;
                }
            }
            
        }

        Linfindex++;
    }
}

void *tp2(void *){ // THREAD QUE GERA LIMITE SUPERIOR
    int lsup;
    while(1){
        sleep(3);
        lsup = rand()%100;

        if(Lsupindex < 10){
            lsuperior[Lsupindex] = lsup;
        } else{
            for (size_t i = 0; i < 10; i++){
                if(i < 9){
                    lsuperior[i] = lsuperior[i+1];
                } else{
                    lsuperior[i] = lsup;
                }
            }
            
        }

        Lsupindex++;
    }
}

int calculaMediaTemperatura(){
    int soma = 0, media = 0, tamanho;

    if(Tindex < 10){
        tamanho = Tindex
    } else{
        tamanho = 10;
    }

    for (size_t i = 0; i < tamanho; i++){
        soma += temperatura[i];
    }

    media = soma/tamanho;
    return media;
}

int calculaMediaLimiteInferior(){
    int soma = 0, media = 0, tamanho;

    if(Linfindex < 10){
        tamanho = Linfindex
    } else{
        tamanho = 10;
    }

    for (size_t i = 0; i < tamanho; i++){
        soma += temperatura[i];
    }
    
    media = soma/tamanho;
    return media;
}

int calculaMediaLimiteSuperior(){
    int soma = 0, media = 0, tamanho;

    if(Lsupindex < 10){
        tamanho = Lsupindex
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

    }
    
}

int main() {
    srand( (unsigned)time(NULL));
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
    printf("SENSORES: Conectado!\n");
    //2:FIM CONEXAO

    pthread_create(&tid0,NULL,tp0,NULL);
    pthread_create(&tid1,NULL,tp1,NULL);
    pthread_create(&tid2,NULL,tp2,NULL);
    pthread_create(&tid3,NULL,ta,NULL);

    pthread_join(tid0,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);  
}