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
#define IP_SERVER "192.168.15.4"

pthread_t tid0,tid1,tid2,tid3;
int s0,var,tmed,linf,lsup;

typedef struct info{
    int id;
    int info;
}s_info;

void calcMedia(int temp){
    // printf("tmed[%d]+temp[%d]\n",tmed,temp);
    tmed=((tmed+temp)/2);
    // printf("tmed=%d\n",tmed);
}

void *tp0(void *){
    //TEMPERATURA
    int i=0;
    s_info s;
    s.id=1;
    while(1){
        sleep(2);
        s.info=rand()%100;
        if(i==0){
            tmed=s.info;
            i=1;
        }else{
            //calcula média
            calcMedia(s.info);
        }
        // printf("SENSOR[%d] DE TEMPERATURA:%d\n",s.id,s.info);
        send(s0,&s,sizeof(s),0);
    } 
}

void *tp1(void *){
    //LIMITE INFERIOR
    s_info s;
    s.id=2;
    while(1){
        sleep(3);
        s.info=rand()%10;
        linf=s.info;
        // printf("SENSOR[%d] DE LIMITE INFERIOR:%d\n",s.id,s.info);
        send(s0,&s,sizeof(s),0);
    } 
}

void *tp2(void *){
    //LIMITE SUPERIOR
    s_info s;
    s.id=3;
    while(1){
        sleep(4);
        s.info=rand()%10;
        lsup=s.info;
        // printf("SENSOR[%d] DE LIMITE SUPERIOR:%d\n",s.id,s.info);
        send(s0,&s,sizeof(s),0);
    } 
}

void *ta0(void *){
    //ENVIA TEMPERATURA MÉDIA E AVISO SE NÃO TIVER OK OS LIMITES INFERIORES E SUPERIORES
    int tsleep=1,aviso,tsleep_ant;
    s_info s;
    s.id=3;
    sleep(5);
    while(1){
        tsleep_ant=tsleep;
        tsleep=rand()%6;
        if(tsleep_ant==0){
            tsleep=2;
        }
        sleep(tsleep);
        s.info=tmed;

        printf("\nSENSOR[%d] tsleep:%d\n",s.id,tsleep);
        printf("1)TEMPERATURA MÉDIA:%d\n",s.info);
        printf("2)LIMITE INFERIOR:%d\n",linf);
        printf("3)LIMITE SUPERIOR:%d\n\n",lsup);

        send(s0,&s,sizeof(s),0);
        //verifica limites
        if(linf<=1){
            s.info=123; //123 é o código de aviso de limite inferior
            send(s0,&s,sizeof(s),0);
        }else if(lsup>=9){
            s.info=321; //321 é o código de aviso de limite inferior
            send(s0,&s,sizeof(s),0);
        }
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
    pthread_create(&tid3,NULL,ta0,NULL);

    pthread_join(tid0,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);  
}