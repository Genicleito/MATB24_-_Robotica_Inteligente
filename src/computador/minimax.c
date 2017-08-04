#include <stdio.h>

char gridChar(int i) {
    switch(i) {
        case -1:
            return 'X';
        case 0:
            return ' ';
        case 1:
            return 'O';
    }
}

void desenhar(int b[9]) {
    printf(" %c | %c | %c\n",gridChar(b[0]),gridChar(b[1]),gridChar(b[2]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n",gridChar(b[3]),gridChar(b[4]),gridChar(b[5]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n",gridChar(b[6]),gridChar(b[7]),gridChar(b[8]));
}

/* Determina se o jogador ganhou */
int vencer(const int board[9]) {
    unsigned jogadasVitoriosas[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    int i;
    for(i = 0; i < 8; ++i) {
        if(board[jogadasVitoriosas[i][0]] != 0 &&
           board[jogadasVitoriosas[i][0]] == board[jogadasVitoriosas[i][1]] &&
           board[jogadasVitoriosas[i][0]] == board[jogadasVitoriosas[i][2]])
            return board[jogadasVitoriosas[i][2]];
    }
    return 0;
}

int minimax(int board[9], int player) {

    int vencedor = vencer(board);
    if(vencedor != 0) return vencedor * player;

    int move = -1;
    int score = -2; // Movimentos de perder podem ser necessários (quando o jogo já tiver perdido)
    int i;
    for(i = 0; i < 9; ++i) {
        if(board[i] == 0) { //se for posível jogar então...
            board[i] = player; // ... tenta o possível movimento
            int thisScore = -minimax(board, player * -1);
            if(thisScore > score) {
                score = thisScore;
                move = i;
            }// Escolher o pior para o oponente
            board[i] = 0;
        }
    }
    if(move == -1) return 0;
    return score;
}

int movimentoDoRobo(int board[9]) {
    int move = -1;
    int score = -2;
    int i;
    for(i = 0; i < 9; ++i) {
        if(board[i] == 0) {
            board[i] = 1;
            int tempScore = -minimax(board, -1);
            board[i] = 0;
            if(tempScore > score) {
                score = tempScore;
                move = i;
            }
        }
    }
    // retorna uma pontuação baseada em um determinado nó do minimax
    board[move] = 1;
    return move;
}

void movimentoDoJogador(int board[9]) {
    int move = 0;
    do {
        printf("\nConfiguracao do Grid ([0..8]): ");
        scanf("%d", &move);
        printf("\n");
    } while (move >= 9 || move < 0 && board[move] == 0);
    board[move] = -1;
}