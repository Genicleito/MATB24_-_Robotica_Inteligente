/* Servidor */

/* Arquivo: servidor.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#define porta 7777
#define tmb 1024

char buffer[tmb], bufaux[tmb], *loc;
char comandos[] = "/x /mem /disc /time /pros /port /help";
int pontarq, tbuf, skt, tskt, escolha;
struct sockaddr_in serv;

main(){
    
    system("clear");
    
    skt = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons (porta);
    memset(&(serv.sin_zero),0x00,sizeof(serv.sin_zero));
    tskt = sizeof(struct sockaddr_in);
    
    printf("\n    Aula de Robotica Inteligente - UFBA\n\n");

    bind(skt,(struct sockaddr *)&serv,sizeof(struct sockaddr));
    listen(skt,1);
    printf(">> Servidor esta escutando na porta %d\n\n",porta);
    
    
    skt = accept(skt,(struct sockaddr *)&serv,&tskt);
    printf(">> A Conexao com o endereco %s foi estabelecida\n\n",inet_ntoa(serv.sin_addr));

    
    // Envia ack p/ cli
    strcpy(buffer,"Servidor Comunicando OK!!!");
    strcpy(bufaux,buffer);
    send(skt,buffer,strlen(buffer), 0);

    // Recebe ack do cli
    tbuf = recv(skt, buffer,tmb, 0);
    buffer[tbuf]=0x00;
    printf(">: %s\n",buffer);
    
    do{
    tbuf = recv(skt,buffer,tmb,0);
    buffer[tbuf]=0x00;
    
    if (strncmp(buffer,"/",1) == 0){

        loc = strstr(comandos,buffer);
        escolha = loc - comandos;
        switch(escolha){
            case 0: break;
     
            case 3:
                    puts(">> Memoria Disponivel Requisitada");
                    system("free -mot > temp");
                    pontarq = open("temp",O_RDONLY,0666);
                    if(pontarq < 0){
                        puts("Erro no arquivo temporario!");
                    }      
                    tbuf = read(pontarq,buffer,sizeof(buffer));
                    close(pontarq);
                    send(skt,buffer,tbuf,0);
                    system("rm -r temp");
                    break;
                 
            case 8:
                    puts(">> Partiçoes de disco Requisitadas");
                    system("df -h > temp");
                    pontarq = open("temp", O_RDONLY, 0666);
                    if(pontarq < 0){
                        puts("Erro no arquivo temporario!");
                    }      
                    tbuf = read(pontarq,buffer,sizeof(buffer));
                    close(pontarq);
                    send(skt,buffer,tbuf,0);
                    system("rm -r temp");
                    break;

            case 14:
                    puts(">> Tempo Logado Requisitado");
                    system("uptime > temp");
                    pontarq = open("temp", O_RDONLY, 0666);
                    if(pontarq < 0){
                        puts("Erro no arquivo temporario!");
                    }      
                    tbuf = read(pontarq,buffer,sizeof(buffer));
                    close(pontarq);
                    send(skt,buffer,tbuf,0);
                    system("rm -r temp");
                    break;
            
            case 20:
                    puts(">> Processos Rodando no servidor");
                    system("ps -a > temp");
                    pontarq = open("temp", O_RDONLY, 0666);
                    if(pontarq < 0){
                        puts("Erro no arquivo temporario!");
                    }      
                    tbuf = read(pontarq,buffer,sizeof(buffer));
                    close(pontarq);
                    send(skt,buffer,tbuf,0);
                    system("rm -r temp");
                    break;
            
            case 26:
                    puts(">> Portas abertas no servidor");
                    system("netstat -ant > temp");
                    pontarq = open("temp", O_RDONLY, 0666);
                    if(pontarq < 0){
                        puts("Erro no arquivo temporario!");
                    }      
                    tbuf = read(pontarq,buffer,sizeof(buffer));
                    close(pontarq);
                    send(skt,buffer,tbuf,0);
                    system("rm -r temp");
                    break;
     
            case 32:
                    puts(">> Help Solicitado");
                    pontarq = open("help", O_RDONLY, 0666);
                    if(pontarq < 0){
                        puts("Erro no arquivo temporario!");
                    }      
                    tbuf = read(pontarq,buffer,sizeof(buffer));
                    close(pontarq);
                    send(skt,buffer,tbuf,0);
                    break;
        }
    }
    else
        while(strcmp(bufaux,buffer) != 0){
           printf(">: %s\n",buffer);    
           strcpy(bufaux,buffer); 
        }   
    }while(strcmp(buffer,"/x") != 0);
    
    close(skt);
    printf(">> A Conexao com o host %s foi encerrada!!!\n\n",inet_ntoa(serv.sin_addr));
    exit(0);
}
