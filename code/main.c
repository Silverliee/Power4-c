#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct {
    char *color;
    unsigned char token;
    int playedTurnCount;
} Player;

typedef struct {
    short **grille;
    int row;
    int column;
    int winCount;
} Board;

typedef struct {
    Player **players;
    int playerCount;
    Board *board;
    int whoPlay;
} Game;

Player *create_player(char *color, char token) {
    Player *newPlayer = malloc(sizeof(Player));
    newPlayer->color = malloc(sizeof(char) * 10);
    newPlayer->token = token;
    strcpy(newPlayer->color, color);
    newPlayer->playedTurnCount = 1;

    return newPlayer;
}

Board *create_board(int row, int column, int winCount) {
    Board *myBoard = malloc(sizeof(Board));
    if ((winCount <= row || winCount <= column) && winCount >= 0) {
        myBoard->row = row;
        myBoard->column = column;
        myBoard->winCount = winCount;
    } else {
        printf("Impossible de créer le tableau de jeu :/");
        free(myBoard);
        return NULL;
    }

    myBoard->grille = calloc(column, sizeof(short *));
    for (int i = 0; i < row; i++) {
        myBoard->grille[i] = calloc(column, sizeof(short));
    }

    return myBoard;
}

Game *init_game(int playerCount, int row, int column, int winCount) {
    Game *newGame = malloc(sizeof(Game));
    newGame->playerCount = playerCount;
    newGame->whoPlay = 0;
    newGame->players = malloc(sizeof(Player) * playerCount);
    newGame->board = create_board(row, column, winCount);
    char *color[4] = {"red", "blue", "green", "yellow"};
    char token[4] = {'X', 'O', 'Z', 'H'};
    for (int i = 0; i < playerCount; i++) {
        newGame->players[i] = create_player(color[i], token[i]);
    }
    return newGame;
}

int getUpperToken(Board *board, int column) {
    int maxRow = board->row;
    int row = 0;
    while (row < maxRow && board->grille[row][column] == 0)
        row++;
    return row;
}

void play(Game *game, short playerId, int column) {
    int row = getUpperToken(game->board, column);
    game->board->grille[row - 1][column] = playerId;
    game->players[playerId - 1]->playedTurnCount++;
    //short test = board->grille[row-1][column];
}

int check_winner_vertical(Board *board, int column) {
    int maxRow = board->row;
    int row = 0;
    while (row < maxRow && board->grille[row][column] == 0)
        row++;
    if (row == maxRow)
        return 0;
    short joueur_actif = board->grille[row][column];

    int nb_of_aligned = 1;
    while ((row + nb_of_aligned) < board->row && board->grille[row + nb_of_aligned][column] == joueur_actif)
        nb_of_aligned++;

    if (nb_of_aligned >= board->winCount)
        return 1;
    return 0;
}

int check_winner_horizontal(Board *board, int column) {
    int maxRow = board->row;
    int row = 0;
    while (row < maxRow && board->grille[row][column] == 0)
        row++;
    if (row == maxRow)
        return 0;
    short joueur_actif = board->grille[row][column];

    int nb_of_aligned_side_1 = 1, nb_of_aligned_side_2 = 0;
    while ((column + nb_of_aligned_side_1) < board->column &&
           board->grille[row][column + nb_of_aligned_side_1] == joueur_actif)
        nb_of_aligned_side_1++;
    while ((column - nb_of_aligned_side_2 - 1) >= 0 &&
           board->grille[row][column - nb_of_aligned_side_2 - 1] == joueur_actif)
        nb_of_aligned_side_2++;
    if ((nb_of_aligned_side_1 + nb_of_aligned_side_2) >= board->winCount)
        return 1;

    return 0;
}

int check_winner_diagonal(Board *board, int column) {
    int maxRow = board->row;
    int row = 0;
    while (row < maxRow && board->grille[row][column] == 0)
        row++;
    if (row == maxRow)
        return 0;
    short joueur_actif = board->grille[row][column];
    if (joueur_actif < 0)
        return 0;

    int nb_of_aligned_side_1 = 1, nb_of_aligned_side_2 = 0;
    while ((row + nb_of_aligned_side_1) < board->row && (column + nb_of_aligned_side_1) < board->column
           && board->grille[row + nb_of_aligned_side_1][column + nb_of_aligned_side_1] == joueur_actif)
        nb_of_aligned_side_1++;
    while ((row - nb_of_aligned_side_2 - 1) >= 0 && (column - nb_of_aligned_side_2 - 1) >= 0
           && board->grille[row - nb_of_aligned_side_2 - 1][column - nb_of_aligned_side_2 - 1] == joueur_actif)
        nb_of_aligned_side_2++;
    if ((nb_of_aligned_side_1 + nb_of_aligned_side_2) >= board->winCount)
        return 1;

    nb_of_aligned_side_1 = 1;
    nb_of_aligned_side_2 = 0;
    while ((row - nb_of_aligned_side_1) >= 0 && (column + nb_of_aligned_side_1) < board->column
           && board->grille[row - nb_of_aligned_side_1][column + nb_of_aligned_side_1] == joueur_actif)
        nb_of_aligned_side_1++;
    while ((row + nb_of_aligned_side_2 + 1) < board->row && (column - nb_of_aligned_side_2 - 1) >= 0
           && board->grille[row + nb_of_aligned_side_2 + 1][column - nb_of_aligned_side_2 - 1] == joueur_actif)
        nb_of_aligned_side_2++;
    if ((nb_of_aligned_side_1 + nb_of_aligned_side_2) >= board->winCount)
        return 1;
    return 0;
}

int check_winner(Board *board, int column) {
    return (check_winner_vertical(board, column) ||
            check_winner_horizontal(board, column) ||
            check_winner_diagonal(board, column));
}

void free_board(Board *board) {
    for (int i = 0; i < board->row; i++) {
        free(board->grille[i]);
    }
    free(board->grille);
    free(board);
}

void free_player(Player *player) {
    free(player->color);
    free(player);
}

void free_game(Game *game) {
    for (int i = 0; i < game->playerCount; i++) {
        free_player(game->players[i]);
    }
    free_board(game->board);
    free(game);
}

// Hello, ici les fonctions de changement de couleur, ATTENTION il faut utiliser la fonction resetColor pour repasser à la normal;
void red() {
    printf("\033[0;31m");
}

void blue() {
    printf("\033[0;34m");
}

void green() {
    printf("\033[0;32m");
}

void yellow() {
    printf("\033[0;33m");
}

void resetColor() {
    printf("\033[0m");
}

void print_player_action(Game *game, int playerId) {
    if (((int) strcmp(game->players[playerId]->color, "red")) == 0) { //utiliser strcmp
        //red();
        printf("%c", game->players[playerId]->token);
        //resetColor();
    } else if (strcmp(game->players[playerId]->color, "blue") == 0) {
        //blue();
        printf("%c", game->players[playerId]->token);
        //resetColor();
    } else if (strcmp(game->players[playerId]->color, "green") == 0) {
        //green();
        printf("%c", game->players[playerId]->token);
        //resetColor();
    } else if (strcmp(game->players[playerId]->color, "yellow") == 0) {
        //yellow();
        printf("%c", game->players[playerId]->token);
        //resetColor();
    }
}

/** Hello, ici la fonction d'affichage du board...
 * Comment ça marche ?
 *  - Il va avoir deux boucles i et j, il sert à parcourir les lignes, j à parcourir les colonnes
 *  - Sur chaque case de la grille il va vérifier la valeur, 0 pour empty, 1 pour joueur, 2 pour joeur 2 etc
 *  - Il va récupérer le token et la couleur pour faire l'affichage correctement grace a un jeu de condition, puis il reset la couleur pour repasser à la normal
 *  - Une fois à la fin d'une ligne on fait un retour chariot et on passe à la ligne suivante
 *  - Une fois la grille affiché il affiche un petit message avec le "c'est le tout du joueur n"
**/
void print_board(Game *game) {
    //clrscr();
    for (int i = 0; i < game->board->row; i++) {
        printf("| ");
        for (int j = 0; j < game->board->column; j++) {
            if (game->board->grille[i][j] == 0) {
                printf(" ");
            } else {
                print_player_action(game, game->board->grille[i][j] - 1);
            }
            printf(" | ");
        }
        printf("\n");
    }
}

void print_game_over(int hasWon, Game *game) {
    print_board(game);
    printf("GAME OVER\n");
    if (!hasWon)
        printf("Match nul.");
    else {
        printf("Bravo ! Vous gagnez une tartiflette aux epinards.");
               /*au joueur des %c pour cette belle victoire en %d coups !",
               game->players[game->whoPlay]->token, game->players[game->whoPlay]->playedTurnCount);/**/
    }
}


int main() {
    int playerCount = 0;
    int row;
    int column;
    int winCount;
    int columnPlayed;
    int ifparam;
    printf("#############################################\n");
    printf("Celui qui gagne a droit a une tartiflette aux epinards\n");
    printf("#############################################\n");
    printf("Voulez-vous utiliser les parametres par defaut ? [0-> non] [1-> oui]\n");
    scanf("%d", &ifparam);
    if (ifparam) {
        FILE *f = fopen("params.txt", "r");
        char *buffer = malloc(sizeof(char)*50);
        fgets(buffer,50,f);
        playerCount = atoi(buffer);
        fgets(buffer,50,f);
        row = atoi(buffer);
        fgets(buffer,50,f);
        column = atoi(buffer);
        fgets(buffer,50,f);
        winCount = atoi(buffer);
        free(buffer);
        fclose(f);
    } else {
        do {
            printf("Entrer le nombre de joueurs (2-4):\n");
            scanf("%d", &playerCount);
            getchar();
        } while (playerCount != 2 && playerCount != 3 && playerCount != 4);
        do {
            printf("Nombre de lignes ? (2-20):\n");
            scanf("%d", &row);
            getchar();
        } while (row < 2 || row > 20);

        do {
            printf("Nombre de colonnes ? (2-20):\n");
            scanf("%d", &column);
            getchar();
        } while (column < 2 || column > 20);

        do {
            printf("Nombre de coup pour gagner ?\n");
            scanf("%d", &winCount);
            getchar();
        } while (winCount < 2 || (winCount > column && winCount > row));
    }

    Game *game = init_game(playerCount, row, column, winCount);

    int number_of_turns_played = 0, max_number_of_turns = row * column;
    while (number_of_turns_played < max_number_of_turns) {
        game->whoPlay++;
        //game->players[game->whoPlay]->playedTurnCount++;
        if (game->whoPlay == ((game->playerCount) + 1)) {
            game->whoPlay = 1;
        }
        print_board(game);
        do {
            printf("C'est au tour du joueur %d...\n", game->whoPlay);
            printf("Dans quelle colonne voulez-vous jouer ?\n");
            scanf("%d", &columnPlayed);
            getchar();
        } while (columnPlayed < 0 || columnPlayed >= column || getUpperToken(game->board, columnPlayed) == 0);
        // colonne vérifiée et valide
        play(game, (short) game->whoPlay, columnPlayed);
        if (check_winner(game->board, columnPlayed)) {
            print_game_over(1, game);
            free_game(game);
            return 0;
        }
        number_of_turns_played++;
    }
    print_game_over(0, game);
    free_game(game);
    return 0;
}
