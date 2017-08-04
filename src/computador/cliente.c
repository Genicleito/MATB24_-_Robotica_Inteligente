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

#include "minimax.c"

//#define ip "192.168.1.124"
#define tmb 1024
#define porta 23
int main ()
{

    // game
    printf("Computer: O, You: X\nPlay (1)st or (2)nd? ");
    int board[9] = {0,0,0,0,0,0,0,0,0};
    //computer quadrados are 1, player quadrados are -1.
    int player=0;
    scanf("%d",&player);
    printf("\n");
    unsigned turn;

    char ip[16] = "192.168.0.115";
    char buffer[tmb], bufaux[tmb], *loc;
    char comandos[]="/x /mem /disc /time /pros /port /help";
    int tbuf, skt, escolha;
    struct sockaddr_in serv;

    system("clear");

    printf("\n    Robotica Inteligente - UFBA\n\n");

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


    // switch (cmd[0]) {
    //   case 'G':
    //
    //     break;
    // }

    // Envia ack p/ serv
    strcpy(buffer, "Client comunicando OK!!!");
    send(skt, buffer, strlen(buffer), 0 );
    // while(strcmp(buffer,"/x") != 0){

    desenhar(board);

    for(turn = 0; turn < 9 && vencer(board) == 0; ++turn) {
        int move;
        if((turn+player) % 2 == 0){
            move = movimentoDoRobo(board);
            printf("Movimento do Robo: %d\n",move);
            desenhar(board);
            // gets(buffer);
            // strcpy(bufaux,buffer);
            sprintf(buffer,"%d",move);
            // itoa(move,buffer,10);
            send(skt, buffer, strlen(buffer), 0);
            printf("Waiting for response message: \n");
            // Recebe ack do serv
            tbuf = recv (skt, buffer, tmb, 0);
            buffer[tbuf] = 0x00;
            while(strcmp(bufaux,buffer) != 0){
              printf ("Response: %s\n",buffer);
              strcpy(bufaux,buffer);
            }

        }
        else {
            movimentoDoJogador(board);
            desenhar(board);
        }

      }
      switch(vencer(board)) {
          case 0:
              printf("A draw. How droll.\n");
              break;
          case 1:
              desenhar(board);
              printf("You lose.\n");
              break;
          case -1:
              printf("You win. Inconceivable!\n");
              break;
      }
     close(skt);
     printf (">>A conexao com o servidor foi finalizada!!!\n\n");
     exit(0);
}

void jogada(){
    // if((turn+player) % 2 == 0)
    //     movimentoDoRobo(board);
    // else {
    //     desenhar(board);
    //     movimentoDoJogador(board);
    // }
    // // }
    // switch(vencer(board)) {
    //     case 0:
    //         printf("A draw. How droll.\n");
    //         break;
    //     case 1:
    //         desenhar(board);
    //         printf("You lose.\n");
    //         break;
    //     case -1:
    //         printf("You win. Inconceivable!\n");
    //         break;
    // }
}
