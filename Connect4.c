#include <stdio.h>
#include <string.h>
#define rows 9
#define columns 9
#define player1 1
#define player2 2

int player1_pieces = 40, player2_pieces = 40;
int board[rows][columns];
int row, col, winner;

char player1_name[20], player2_name[20], win_type[20];
char coin_flipping_answer1, coin_flipping_answer2, coin_flipping_result;

void coin_flipping();
void play_turns(int);
void change_board(int, int), call_board(), draw_board(), reset_board();
void save_move();
void game_over_check();

int main(){
	printf("\n\tConnect 4\n");
	
	printf("\n\tEnter A Player Name : ");
	scanf("%s", player1_name);
	
	printf("\n\tEnter A Player Name : ");
	scanf("%s", player2_name);
	
	if (sizeof(board) == 0)
		change_board(1, player1);
		
	call_board();
	reset_board();
	printf("\n\t%s is Player 1 and will be represented by '1'", player1_name);
	printf("\n\t%s is Player 2 and will be represented by '2'", player2_name);
	
	while(1){
		
		play_turns(player1);
		if (col == -1)
			break;
		call_board();
		draw_board();
		
		play_turns(player2);
		if (col == -1)
			break;
		call_board();
		draw_board();
	}
	
	char exit[2];
	scanf("%s", exit);
	return 0;
}



void coin_flipping(){
	int not_resulted = 1;
	do{
		srand(time(0));
		coin_flipping_result = (rand() % 2 == 0) ? 'h' : 't';
		
		printf("\n\n\t%s Heads or Tails (h / t) : ", player1_name);
		scanf(" %c", &coin_flipping_answer1);
		
		printf("\n\n\t%s Heads or Tails (h / t) : ", player2_name);
		scanf(" %c", &coin_flipping_answer2);
		
		if((coin_flipping_answer1 != 'h' && coin_flipping_answer1 != 't') || (coin_flipping_answer2 != 'h' && coin_flipping_answer2 != 't')){
			printf("\n\n\tInvalid Answer.");
		}
		
		else if(coin_flipping_answer1 != coin_flipping_answer2){
			if(coin_flipping_answer2 == coin_flipping_result){
				char player_1_name_tmp[20];
				strcpy(player_1_name_tmp, player1_name);
				strcpy(player1_name, player2_name);
				strcpy(player2_name, player_1_name_tmp);
			}
			not_resulted = 0;
		}
		else{
			printf("\n\n\tNo Winner.\n");
		}
	}
	while((coin_flipping_answer1 == coin_flipping_answer2) || not_resulted);
}



void play_turns(int active_player){
	char col_tmp[10];
	while(1){
		game_over_check();
		
		if (col == -1)
			break;
			
		printf("\n\n\t%s : 1 (%d Remaining Pieces)", player1_name, player1_pieces);
		printf("\n\t%s : 2 (%d Remaining Pieces)", player2_name, player2_pieces);
		
		printf("\n\n\t%s (Enter -1 to Exit) Enter Column No. : ", (active_player == player1) ? player1_name : player2_name);
		scanf("%s", col_tmp);
		
		col = atoi(col_tmp);
		
		if(col == 0){
			int i = 0;
			while(col_tmp[i]) {
    			if(!isdigit(col_tmp[i])) {
        			col = -2;
        			break;
    			}
    			i++;
			}	
		}

		game_over_check();
		
		if (col == -1)
			break;
		
		if (col >= 0 && col < columns){
			change_board(0, active_player);
	
			if(active_player == player1)
				player1_pieces -= 1;
			else
    			player2_pieces -= 1;	
    			
    		save_move(0, active_player);
    		
			break;
		}
		else if(col == -1)
			break;	
		else{
			draw_board();
			printf("\n\n\tInvalid Value");
		}				
	}
}



void change_board(int clear, int active_player){
	FILE *file = fopen("./Tahta.txt", "w");
	
	if (file == NULL){
    	printf("\n\n\tFile Does Not Exist");
    	return 0;
	}
	
	if (clear){
		for(int i = 0; i < rows; i++)
			for(int j = 0; j < columns; j++)
				board[i][j] = 0;
	}
	else{
		for(int i = rows - 1; i >= 0; i--){
			if(board[i][col] == 0){
				row = i;
				board[i][col] = ((active_player == player1) ? 1 : 2);
				break;
			}
		}
	}
	
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			fprintf(file, "%d", board[i][j]);		
		fprintf(file, "\n");
	}
	
	fclose (file);
}


void call_board(){
	FILE *file = fopen("./Tahta.txt", "r");
  	char board_tmp[rows][columns];
  	
	for (int i = 0; i < rows; i++) 
	    for (int j = 0; j < columns; j++) 
    		fscanf(file, " %c", &board_tmp[i][j]);
	
	for (int i = 0; i < rows; i++) 
	    for (int j = 0; j < columns; j++) 
    		board[i][j] = (int)(board_tmp[i][j] - '0');
  
	fclose(file);
}



void draw_board(){
	printf("\n\n\n\n");
    for(int idx = 0; idx < rows; idx++)
    	printf("\t  %d ", idx);	
	printf("\t|\n");
	
	for(int idx = 0; idx < rows; idx++)
		printf("\t-----");
	printf("\t   -----\n");
	
    for(int i = 0; i < rows; i++){
    	for(int j = 0; j < columns; j++)
			printf("\t  %d ", board[i][j]);	
		printf("\t|    %d\n\n", i);
	}	
}



void reset_board(){
	char answer;
	while (1){
		printf("\n\n\tReset Table? (y/n) : ");
		scanf(" %c", &answer);
		if (answer == 'y'){
			coin_flipping();
			change_board(1, player1);
			save_move(1, player1);
			call_board();
			draw_board();
			break;
		}
		else if(answer == 'n'){
			for(int i = 0; i < rows; i++){
				for(int j = 0; j < columns; j++){
					if(board[i][j] == 1)
						player1_pieces -= 1;
					else if(board[i][j] == 2)
						player2_pieces -= 1;
				}
			}
			draw_board();
			break;
		}
		else
			printf("\tInvalid Answer");
	}
}



int horizontal_check(){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns - 3; j++){
			if(board[i][j] != 0)
				if ((board[i][j] == board[i][j+1]) && (board[i][j+2] == board[i][j+3]) && (board[i][j] == board[i][j+3])){
					winner = board[i][j];
					strcpy(win_type, "Horizontal");
					return 1;
				}
		}
	}
	return 0;
}



int vertical_check(){
	for(int i = 0; i < rows - 3; i++){
		for(int j = 0; j < columns; j++){
			if(board[i][j] != 0)
				if ((board[i][j] == board[i+1][j]) && (board[i+2][j] == board[i+3][j]) && (board[i][j] == board[i+3][j])){
					winner = board[i][j];
					strcpy(win_type, "Vertical");
					return 1;
				}		
		}
	}
	return 0;
}



int diagonal_check(){	
	for(int i = 0; i < rows - 3; i++){
		for(int j = 0; j < columns - 3; j++){
			if(board[i][j] != 0)
				if ((board[i][j] == board[i+1][j+1]) && (board[i+2][j+2] == board[i+3][j+3]) && (board[i][j] == board[i+3][j+3])){
					winner = board[i][j];
					strcpy(win_type, "Diagonal");
					return 1;
				}		
		}
	}
	for(int i = rows-1; i >= 0; i--){
		for(int j = 0; j < columns; j++){
			if(board[i][j] != 0)
				if ((board[i][j] == board[i-1][j+1]) && (board[i-2][j+2] == board[i-3][j+3]) && (board[i][j] == board[i-3][j+3])){
					winner = board[i][j];
					strcpy(win_type, "Diagonal");
					return 1;
				}		
		}
	}
	return 0;
}



void game_over_check(){
	if(player1_pieces <= 0 && player2_pieces <= 0){
		col = -1;
		printf("\n\n\tGame is draw.");
		return;
	}
	if(horizontal_check() || vertical_check() || diagonal_check()){
		col = -1;
		draw_board();
		printf("\n\n\t%s is the Winner.(%s Win)", (winner == 1) ? player1_name : player2_name, win_type);
		return;
	}
}



void save_move(int clear, int active_player){
	FILE *file;
	if(clear == 0){
		file = fopen("./Hamle.txt", "a+");
		if (file == NULL){
    		printf("\n\n\tFile Does Not Exist");
    		return 0;
		}
		if (col != -1){
			fprintf(file, "%s : %d , %d\n", (active_player == player1 ? player1_name : player2_name), row, col);	
		}
	}
	else
		file = fopen("./Hamle.txt", "w");
		
	fclose(file);
}