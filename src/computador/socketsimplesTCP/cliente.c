/* Cliente */

/* Arquivo: cliente.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

//#define ip "127.0.0.1"
#define tmb 1024
#define porta 7777
main ()
{

    char ip[16] = "127.0.0.1"; 
    char buffer[tmb], bufaux[tmb], *loc;
    char comandos[]="/x /mem /disc /time /pros /port /help";
    int tbuf, skt, escolha;
    struct sockaddr_in serv;
    
    system("clear");
    
    printf("\n    Aula de Robotica Inteligente - UFBA\n\n");
    
    //printf("\n>> Digite o ip do Servidor ou /x para sair: ");
    //gets(ip);
    
    skt = socket(AF_INET, SOCK_STREAM, 0);
    
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(ip);
    serv.sin_port = htons (porta);

    memset (&(serv.sin_zero), 0x00, sizeof (serv.sin_zero));
    if(connect (skt, (struct sockaddr *)&serv, sizeof (struct sockaddr)) != 0)
    {
    puts("\n> Servidor nao Encontrado\n");
    exit(0);
    
    }
    printf(">> A Conexao com o Servidor %s foi estabelecida na porta %d \n\n",ip,porta);
    // Recebe ack do serv
    tbuf = recv (skt, buffer, tmb, 0);
    buffer[tbuf] = 0x00;
    printf (">: %s\n",buffer);

    // Envia ack p/ serv
    strcpy(buffer, "Client comunicando OK!!!");
    send(skt, buffer, strlen(buffer), 0 );    
    while(strcmp(buffer,"/x") != 0){
      printf("> ");
      gets(buffer);
      strcpy(bufaux,buffer);
      while(strncmp(buffer,"/",1) == 0)
         if ((strcmp(buffer,"/mem") != 0) && (strcmp(buffer,"/disc") != 0)
         && (strcmp(buffer,"/time") != 0) && (strcmp(buffer,"/pros") != 0)
         && (strcmp(buffer,"/port") != 0) && (strcmp(buffer,"/help") != 0)
         && (strcmp(buffer,"/x") != 0)){
             puts("> Comando invalido ");
             printf("> ");
             gets(buffer);
         }
         else break;    
         send(skt, buffer, strlen(buffer), 0);
         if ((strncmp(buffer,"/",1) == 0) && (strcmp(buffer,"/x") != 0)){
             tbuf = recv(skt, buffer, tmb, 0);
             buffer[tbuf] = 0x00;
             loc = strstr(comandos,bufaux);
             escolha = loc - comandos;
             switch (escolha){
               case 3: printf ("\n>: Memoria Disponivel:\n\n %s\n",buffer);break;
               case 8: printf ("\n>: Partiçoes de Disco:\n\n %s\n",buffer);break;
               case 14: printf ("\n>: Tempo Total logado:\n\n %s\n",buffer);break;
               case 20: printf ("\n>: Processo Rodando no Servidor:\n\n %s\n",buffer);break;
               case 26: printf ("\n>: Portas Abertas no Servidor:\n\n %s\n",buffer);break;
               case 32: printf ("\n\n %s\n",buffer);break;
             }       
         }
      }
     close(skt);
     printf (">>A conexao com o servidor foi finalizada!!!\n\n");
     exit(0);    
}
