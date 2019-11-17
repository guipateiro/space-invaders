#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<unistd.h>

#define MAXLIN 35 
#define MAXCOL 100

#define DELAY 500000


typedef struct {
	char tela[MAXLIN][MAXCOL];
} tmundo ;

void iniciatela(tmundo *mundo){
	int i ,j;
	char barreira = 'O';
	char alien = 'H';

	for (i = 0 ;i <MAXLIN; i++)
		for (j =0 ; j<MAXCOL ; j++)
			mundo->tela[i][j] = ' ';

	for (i = 0 ;i <MAXLIN; i++)
		for (j =0 ; j<MAXCOL ; j++)	{
			if (i > 4 && i % 4 == 1 && i < 24 )
				if(j < 77 && j % 7 == 0)
					mundo->tela[i][j] = alien;	

			if (i == MAXLIN - 7)
				if(j > 14 && (j % 21 == 16 || j % 21 == 17 || j % 21 == 18 || j % 21 == 19 || j % 21 == 20) && j< 85 ) 	
					mundo->tela[i][j] = barreira;
			if (i == MAXLIN - 6)
				if(j > 14 && (j % 21 == 15 || j % 21 == 16|| j % 21 == 17 || j % 21 == 18 || j % 21 == 19 || j % 21 == 20 || j % 21 == 0) && j< 85) 	
					mundo->tela[i][j] = barreira;
			if (i == MAXLIN - 5)
				if(j > 14 && (j % 21 == 15 || j % 21 == 16 || j % 21 ==20 || j % 21 ==0) && j< 85 ) 	
					mundo->tela[i][j] = barreira;	

		}			
}

void inicia_tiro(tmundo *mundo, int c ){

	mundo->tela[MAXLIN -2][c] = '!';
}


void atualiza_tiro(tmundo *mundo){
	int i,j; 
	char tiro = '!';
	char vazio = ' ';
	char barreira = 'O';
	char alien = 'H';
	char bomba = '$';

	for (i = 0 ;i <MAXLIN; i++)
		for (j = 0 ; j<MAXCOL ; j++){
			if (mundo->tela[i][j] == tiro && mundo->tela[i-1][j] == vazio){
				mundo->tela[i-1][j] = mundo->tela[i][j];
				mundo->tela[i][j] = vazio;
			}
			if (mundo->tela[i][j] == tiro && mundo->tela[i-1][j] == barreira){
				mundo->tela[i][j] = vazio;
				mundo->tela[i-1][j] = vazio;	
			}
			if (mundo->tela[i][j] == tiro && mundo->tela[i-1][j] == alien){
				mundo->tela[i][j] = vazio;
				mundo->tela[i-1][j] = vazio;
			}
			if (mundo->tela[i][j] == tiro && i == 0)
				mundo->tela[i][j] = vazio;

			if (mundo->tela[i][j] == tiro && mundo->tela[i-1][j] == bomba){
				mundo->tela[i][j] = vazio;
				mundo->tela[i-1][j] = vazio;
			}
		}
}

void atualiza_bomba(tmundo *mundo ,int *vitoria){
	int i,j; 
	char vazio = ' ';
	char barreira = 'O';
	char alien = 'H';
	char bomba = '$';
	char canhao = 'W';

	for (i = MAXLIN ;i > 0 ; i--)
		for (j = MAXCOL ; j > 0 ; j--){
			if (mundo->tela[i][j] == bomba && mundo->tela[i+1][j] == vazio){
				mundo->tela[i+1][j] = mundo->tela[i][j];
				mundo->tela[i][j] = vazio;
			}
			if (mundo->tela[i][j] == bomba && mundo->tela[i+1][j] == barreira){
				mundo->tela[i][j] = vazio;
				mundo->tela[i+1][j] = vazio;	
			}
			if (mundo->tela[i][j] == bomba && mundo->tela[i+1][j] == canhao){
				mundo->tela[i][j] = vazio;
				mundo->tela[i+1][j] = vazio;
				vitoria = 0; 
			}
			if (mundo->tela[i][j] == bomba && i == MAXLIN)
				mundo->tela[i][j] = vazio;

			if (mundo->tela[i][j] == bomba && mundo->tela[i+1][j] == alien){
				mundo->tela[i+2][j] = bomba;
				mundo->tela[i][j] = vazio;
			}	
		}
}

void inicia_bombas(tmundo *mundo) {
	int i ,j; 
	char bomba = '$';
	char alien = 'H';

	for (i = MAXLIN ;i > 0 ; i--)
		for (j = MAXCOL ; j > 0 ; j--){
			if (mundo->tela[i][j] == alien && ((rand() % 1000) == 376))
				mundo->tela[i+1][j] = bomba;
		}	
}
void atualiza_aliens(tmundo *mundo ,int *dir ){
	int i, j ,altura = 0;
	char vazio = ' ';
	char alien = 'H';
	for (i = 0 ;i <MAXLIN; i++)
		if (mundo->tela[i][0] == alien){
			altura = 1;
			*dir = 1; 
		}
	for (i = 0 ;i <MAXLIN; i++)
		if (mundo->tela[i][MAXCOL-1] == alien){
			altura = 1;
			*dir = -1;
		}	
	if (*dir == 1)	
		for (i = MAXLIN ;i > 0 ; i--)
			for (j = MAXCOL ; j > 0 ; j--)
				if (mundo->tela[i][j] == alien){
					mundo->tela[i][j+*dir] = alien;
					mundo->tela[i][j] = vazio; 
			}
	if (*dir == -1)	
		for (i = 0 ;i <MAXLIN; i++)
			for (j = 0 ; j<MAXCOL ; j++)
				if (mundo->tela[i][j] == alien){
					mundo->tela[i][j+*dir] = alien;
					mundo->tela[i][j] = vazio;				 
			}
	if (altura == 1)	
		for (i = MAXLIN ;i > 0 ; i--)
			for (j = MAXCOL ; j > 0 ; j--)
				if (mundo->tela[i][j] == alien){
					mundo->tela[i+altura][j] = alien;
					mundo->tela[i][j] = vazio; 
				}			
}

void atualiza_canhao(tmundo *mundo,int c){
	int i ; 
	char canhao = 'W';
	for(i = 0 ; i<MAXCOL; i++)
		if (i != c)
			mundo->tela[MAXLIN - 1][i] = ' ';
		else 
			mundo->tela[MAXLIN - 1][c] = canhao; 
}

void imprimetela(tmundo mundo){
	int i,j; 
	for (i = 0 ;i <MAXLIN; i++)
		for (j =0 ; j<MAXCOL ; j++)
			mvprintw(i, j ,"%c",mundo.tela[i][j]);
}


int main(){
	int maxY,maxX;
	tmundo mundo; 
	int tecla;
	int c; 
	int controletemp = 0; 
	int dir = 1; 
	int vitoria = 1; 
	iniciatela(&mundo);


	raw();
	initscr();              /* inicia a tela */
    cbreak();               /* desabilita o buffer de entrada */
    noecho();               /* não mostra os caracteres digitados */
    nodelay(stdscr, TRUE);  /* faz com que getch não aguarde a digitação */
    keypad(stdscr, TRUE);   /* permite a leitura das setas */
    curs_set(FALSE);        /* não mostra o cursor na tela */

	c = 50;
	while (vitoria != 0){
		getmaxyx(stdscr, maxY, maxX);
	    if (maxX < MAXCOL || maxY < MAXLIN){ 
			printf("terminar nao deve ter mais de %d linhas e %d colunas", MAXLIN, MAXCOL);
			endwin();	
		    exit(1);
        }
        imprimetela(mundo);
		tecla = getch();
		if (tecla == KEY_LEFT && c > 0)
			c--;
		else if (tecla == KEY_RIGHT && c <(99))
			c++;
		else if (tecla == 'c')
			inicia_tiro(&mundo , c );
		else if (tecla == 'q'){
			endwin();
			exit(0);
		}
		clear();	
		atualiza_tiro(&mundo);
		atualiza_canhao(&mundo ,c);
		atualiza_bomba(&mundo , &vitoria);
		atualiza_aliens(&mundo ,&dir);
		inicia_bombas(&mundo);
		imprimetela(mundo);
		refresh();	

		usleep(100000); 
		
	}
	return 0; 
}		
