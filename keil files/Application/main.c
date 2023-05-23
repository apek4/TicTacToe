//--------------------------------------------------------
// Application demonstrator: Tic-Tac-Toe game
//--------------------------------------------------------


#include "EDK_CM0.h" 
#include "core_cm0.h"
#include "edk_driver.h"
#include "edk_api.h"
#include <stdio.h>					

//Game region
#define left_boundary 5
#define right_boundary 96
#define top_boundary 33
#define bottom_boundary 96
#define boundary_thick 1

//Global variables
static int i;
static char key;


// Declare player name strings
char player1_name[10] = "Jugal";
char player2_name[10] = "Apeksha";

// Array to track positions of player keys and decide winner
int player1_array[9] = {0};
int player2_array[9] = {0};

int filled[9] = {0};


//---------------------------------------------
// Game
//---------------------------------------------


void Game_Init(void)
{	
	//Draw a game region
	clear_screen();
	rectangle(left_boundary,	top_boundary,	right_boundary,						top_boundary+boundary_thick,BLUE); 			// 1st horizontal line
	rectangle(left_boundary,	top_boundary*2,	right_boundary,						(top_boundary*2)+boundary_thick,BLUE);		// 2nd horizontal line
	rectangle(top_boundary,		left_boundary,	top_boundary+boundary_thick,		bottom_boundary,BLUE);						// 1st vertical line
	rectangle(top_boundary*2,	left_boundary,	(top_boundary*2)+boundary_thick,	bottom_boundary,BLUE);						// 2nd vertical line
	
	//Print instructions
	printf("\n\n----- Tic-Tac-Toe Game -----");
	printf("\n-------- Game Rules --------");
	printf("\n 1 | 2 | 3");
	printf("\n-----------");
	printf("\n 4 | 5 | 6");
	printf("\n-----------");
	printf("\n 7 | 8 | 9");
	printf("\n1. Select numbers 1-9 to input your turn, refer above for format");
	printf("\n2. Player 1 starts with X ");
	printf("\n3. Player 2 continues with O");
	printf("4. First player to get 3 consecutive X's or O's in row/column/diagonal WINS!!\n");
	printf("---------------------------");	
	printf("\nTo run the game, make sure:");
	printf("\n-UART terminal is activated");
	printf("\n-UART baud rare:  19200 bps");
	printf("\n-Keyboard is in lower case");
  	printf("\n---------------------------");
	printf("\nPress any key (1-9) for Player 1 to start\n");	
	while(KBHIT()==0);\
		
	// printf("\nScore=%d\n",score);
	
	NVIC_EnableIRQ(Timer_IRQn);			//start timing
	NVIC_EnableIRQ(UART_IRQn);	
}


//---------------------------------------------
// Function to decide winner
//---------------------------------------------

int checkWinner(int index, int player_array[], char player[]){

	player_array[index] = 1;

	if 	(player_array[0] && player_array[1] && player_array[2]){ // row 1
		rectangle(left_boundary,17,	right_boundary,	  17+boundary_thick,	GREEN);
		printf("\n%s Wins!", player);
		GameOver();
		
	}
	else if((player_array[3] && player_array[4] && player_array[5])){ // row 2
		rectangle(left_boundary,50,	right_boundary,	  50+boundary_thick,	GREEN);
		printf("\n%s Wins!", player);
		GameOver();
	}
	else if((player_array[6] && player_array[7] && player_array[8])){ // row 3
		rectangle(left_boundary,83,	right_boundary,	  83+boundary_thick,	GREEN);
		printf("\n%s Wins!", player);
		GameOver();
	}
	else if((player_array[0] && player_array[3] && player_array[6])){ // column 1
		rectangle(17,		left_boundary,	17+boundary_thick,		bottom_boundary,GREEN);	
		printf("\n%s Wins!", player);
		GameOver();
	}
	else if((player_array[1] && player_array[4] && player_array[7])){ // column 2
		rectangle(50,		left_boundary,	50+boundary_thick,		bottom_boundary,GREEN);	
		printf("\n%s Wins!", player);
		GameOver();
	}
	else if((player_array[2] && player_array[5] && player_array[8])){ // column 3
		rectangle(83,		left_boundary,	83+boundary_thick,		bottom_boundary,GREEN);	
		printf("\n%s Wins!", player);
		GameOver();
	}
	else if((player_array[0] && player_array[4] && player_array[8])){ // left to right diagonal line
		draw_tlbr(left_boundary+7, 9, right_boundary, 89+boundary_thick,  GREEN);
		printf("\n%s Wins!", player);
		GameOver();
	}
	else if((player_array[2] && player_array[4] && player_array[6])){ // right to left diagonal line
		draw_trbl(right_boundary , top_boundary,	left_boundary, bottom_boundary,GREEN);	
		printf("\n%s Wins!", player);
		GameOver();
	}
	

}


void Game_Close(void){
	clear_screen();
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");		//flush screen
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	NVIC_DisableIRQ(Timer_IRQn);			
	NVIC_DisableIRQ(UART_IRQn);	
}

	
int GameOver(){
	char key;
	
	NVIC_DisableIRQ(UART_IRQn);
	NVIC_DisableIRQ(Timer_IRQn);
	printf("\nGame over\n");
	
	printf("\nPress 'q' to quit");
	printf("\nPress 'r' to replay");
	
	while(1){
		while(KBHIT()==0);
		key = UartGetc();
		if (key == RESET){
			Game_Init();
			for(i=0; i<=8;i++){
				filled[i]= 0;
				player1_array[i] = 0;
				player2_array[i] = 0;
			}
			break;
		}
		else if (key == QUIT){	
			Game_Close();
			break;
		}
		else
			printf("\nInvalid input");
	}
		
}

//---------------------------------------------
// UART ISR -- used to input commands
//---------------------------------------------
int player1 = 1, player2 = 0;	// Start with player 1

void UART_ISR(void)
{		
	int index;
	key=UartGetc();
	index = getIndex(key);
	int canIdraw = 1;
	// First check if slot is filled, if yes, display invalid input and exit from drawing, else continue
	// Checking if slot filled
	if (filled[index]){
		printf("\nThis slot is already taken!. Use another slot\n");

		// Give the chance back to original player who gave invalid input
		if 		(player1){player1 = 1; player2 = 0;}
		else if (player2){player1 = 0; player2 = 1;}
		canIdraw = 0;
	}
		// If slot not filled
	else{
		filled[index] = 1;
		canIdraw = 1;							
	}
	if (player1 && canIdraw){
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b");
		printf("%s plays", player1_name);
		player1 = 0;
		player2 = 1;
		switch (key) {
			case ONE:	draw_X(13, 15, 23, 25, WHITE);
						break;
						
			case TWO:	draw_X(44, 15, 54, 25, WHITE);
						break;

			case THREE:	draw_X(76, 15, 86, 25, WHITE);
						break;

			case FOUR:	draw_X(13, 44, 23, 54, WHITE);
						break;

			case FIVE:	draw_X(44, 44, 54, 54, WHITE);
						break;

			case SIX:	draw_X(76, 44, 86, 54, WHITE);
						break;

			case SEVEN:	draw_X(13, 76, 23, 86, WHITE);
						break;

			case EIGHT:	draw_X(44, 76, 54, 86, WHITE);
						break;

			case NINE:	draw_X(76, 76, 86, 86, WHITE);
						break;

			default: 	printf("Invalid Key by Player 1");
						break;
		}
		
	checkWinner(index, player1_array, player1_name);
	}
	else if (player2 && canIdraw) {
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		printf("%s plays", player2_name);
		player1 = 1;
		player2 = 0;
		switch (key) {
			case ONE:   draw_O(18, 20, 6, RED);
						break;

			case TWO:   draw_O(49, 20, 6, RED);
						break;

			case THREE: draw_O(81, 20, 6, RED);
						break;

			case FOUR:  draw_O(18, 49, 6, RED);
						break;

			case FIVE:  draw_O(49, 49, 6, RED);
						break;

			case SIX:   draw_O(81, 49, 6, RED);
						break;

			case SEVEN: draw_O(18, 81, 6, RED);
						break;

			case EIGHT: draw_O(49, 81, 6, RED);
						break;

			case NINE:  draw_O(81, 81, 6, RED);
						break;
						
			default: break;
		}
		checkWinner(index, player2_array, player2_name);
	
	}
	int len_filled = sizeof(filled) / sizeof(filled[0]);
	int allOnes = 1;
	for (i = 0; i < len_filled; i++) {
        if (filled[i] != 1) {
			allOnes = 0;
            break;
        }
		}
	if (allOnes) {
		printf("\nDRAW GAME!!\n");
		for (i = 0; i < 8; i++) {
		filled[i] = 0;
    	}
		GameOver();
		
    }
	
}
 

//---------------------------------------------
// TIMER ISR -- used to move the snake
//---------------------------------------------


void Timer_ISR(void)
{

	dig4++;
	
	if(dig4==10){
		dig4=0;
		dig3++;
		if (dig3==10){
			dig3=0;
			dig2++;
			if (dig2==10){
				dig2=0;
				dig1++;
			}
		}
	}
	
	seven_seg_write( dig1, dig2, dig3, dig4);
	timer_irq_clear();
}	

//---------------------------------------------
// Main Function
//---------------------------------------------


int main(void){

	//Initialise the system
	SoC_init();
	//Initialise the game
	Game_Init();
	
	//Go to sleep mode and wait for interrupts
	while(1)
		__WFI();	
	

}