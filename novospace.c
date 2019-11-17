#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<unistd.h>

#define MAXLIN 35 
#define MAXCOL 100

#define DELAY 500000
typedef struct {
	char tela[MAXLIN][MAXCOL];
	int lin;
	int col;
} tmundo ;

typedef struct {
	char sprite;
	int condicao;
} tsprite; 

typedef struct {
	int condicao; 
	char sprite;
} talien;



void imprime_sprite(tmundo *mundo,int lin , int col, tsprite objeto){
	int i, j ; 

	if (objeto.condicao == 1)
	mundo->tela[lin][col] = objeto.sprite;
	mvprintw(lin, col ,"%c",mundo->tela[lin][col]);
}

void imprime_aliens(tmundo *mundo , int lin , int col , talien *aliens){
	int i , j , k; 

	for (i = 0; i < 11 ;i++){
		if ((aliens+i)->condicao == 1){
			mundo->tela[lin][col+(i*7)] = (aliens+i)->sprite;
			mvprintw(lin, col+(i*7), "%c" ,mundo->tela[lin][col+(i*7)]);
		}	 
	}

}


void imprime_barreiras(tmundo *mundo,int lin , int col,tsprite *barreiras ){
	int k ; 

	for (k = 0; k <=70; k++)
		if ((barreiras+k)->condicao == 1 && (barreiras+k)->sprite != ' '){
			mvprintw(lin ,k+col,"%c",(barreiras+k)->sprite);
			mundo->tela[lin][k+col] = (barreiras+k)->sprite;
		}
}


void atualiza_tiros(tmundo mundo, tsprite tiro ,int subir){
	int i ,j ; 

	for (i = 0; i < MAXLIN; i++)
		for (j = 0; j <MAXCOL; j++){ 
				if (mundo.tela[i][j] == tiro.sprite){
					imprime_sprite (&mundo , i-subir , j , tiro);
				}
			}
}



int main(){
	int maxY,maxX,i,j ;
	tmundo mundo;
	mundo.lin = 0;
	mundo.col = 0; 
	int tecla;
	i = 0; 
	int c; 
	int x = 0 ,y = 5; 
	int controletemp = 0; 
	int dir = 1; 
	int vitoria = 1; 
	int subir;
	tsprite canhao;
	canhao.sprite = 'W';
	canhao.condicao = 1; 

	/*talien alien; 
	alien.topo[5] =  " /H\\ ";
	alien.meio[5] =  "T[h]T";
	alien.baixo[5] = " (_) ";
	alien.condicao = 1; */

	tsprite tiro;
	tiro.sprite = '!';
	tiro.condicao = 1;

	tsprite bomba;
	bomba.sprite = '$';
	bomba.condicao = 1;


	talien vetoralien[11];
	for (i = 1; i<= 11 ;i++){
		vetoralien[i].sprite = 'H';
		vetoralien[i].condicao = 1;
	}






/*----------------------barreiras parte 1 ----------------------*/
	tsprite barreiras1[70];
	for (i = 0; i<=70; i++)
		barreiras1[i].sprite = ' ';

	barreiras1[2].sprite = 'O';
	barreiras1[2].condicao = 1; 
	barreiras1[3].sprite = 'O';
	barreiras1[3].condicao = 1; 
	barreiras1[4].sprite = 'O';
	barreiras1[4].condicao = 1; 
	barreiras1[5].sprite = 'O';
	barreiras1[5].condicao = 1; 
	barreiras1[23].sprite = 'O';
	barreiras1[23].condicao = 1; 
	barreiras1[24].sprite = 'O';
	barreiras1[24].condicao = 1; 
	barreiras1[25].sprite = 'O';
	barreiras1[25].condicao = 1; 
	barreiras1[26].sprite = 'O';
	barreiras1[26].condicao = 1; 
	barreiras1[44].sprite = 'O';
	barreiras1[44].condicao = 1; 
	barreiras1[45].sprite = 'O';
	barreiras1[45].condicao = 1; 
	barreiras1[46].sprite = 'O';
	barreiras1[46].condicao = 1; 
	barreiras1[47].sprite = 'O';
	barreiras1[47].condicao = 1;
	barreiras1[65].sprite = 'O';
	barreiras1[65].condicao = 1;
	barreiras1[66].sprite = 'O';
	barreiras1[66].condicao = 1;
	barreiras1[67].sprite = 'O';
	barreiras1[67].condicao = 1;
	barreiras1[68].sprite = 'O';
	barreiras1[68].condicao = 1;

/*----------------------barreiras parte 2----------------------*/
	tsprite barreiras2[70];
	for (i = 0; i<=70; i++)
		barreiras2[i].sprite = ' ';

	barreiras2[0].sprite = 'O';
	barreiras2[0].condicao = 1; 
	barreiras2[1].sprite = 'O';
	barreiras2[1].condicao = 1; 
	barreiras2[2].sprite = 'O';
	barreiras2[2].condicao = 1; 
	barreiras2[3].sprite = 'O';
	barreiras2[3].condicao = 1; 
	barreiras2[4].sprite = 'O';
	barreiras2[4].condicao = 1; 
	barreiras2[5].sprite = 'O';
	barreiras2[5].condicao = 1; 
	barreiras2[6].sprite = 'O';
	barreiras2[6].condicao = 1; 
	barreiras2[7].sprite = 'O';
	barreiras2[7].condicao = 1; 
	barreiras2[21].sprite = 'O';
	barreiras2[21].condicao = 1; 
	barreiras2[22].sprite = 'O';
	barreiras2[22].condicao = 1; 
	barreiras2[23].sprite = 'O';
	barreiras2[23].condicao = 1; 
	barreiras2[24].sprite = 'O';
	barreiras2[24].condicao = 1; 
	barreiras2[25].sprite = 'O';
	barreiras2[25].condicao = 1; 
	barreiras2[26].sprite = 'O';
	barreiras2[26].condicao = 1; 
	barreiras2[27].sprite = 'O';
	barreiras2[27].condicao = 1; 
	barreiras2[28].sprite = 'O';
	barreiras2[28].condicao = 1; 
	barreiras2[42].sprite = 'O';
	barreiras2[42].condicao = 1; 
	barreiras2[43].sprite = 'O';
	barreiras2[43].condicao = 1; 
	barreiras2[44].sprite = 'O';
	barreiras2[44].condicao = 1; 
	barreiras2[45].sprite = 'O';
	barreiras2[45].condicao = 1; 
	barreiras2[46].sprite = 'O';
	barreiras2[46].condicao = 1; 
	barreiras2[47].sprite = 'O';
	barreiras2[47].condicao = 1;
	barreiras2[48].sprite = 'O';
	barreiras2[48].condicao = 1;
	barreiras2[49].sprite = 'O';
	barreiras2[49].condicao = 1;
	barreiras2[63].sprite = 'O';
	barreiras2[63].condicao = 1;
	barreiras2[64].sprite = 'O';
	barreiras2[64].condicao = 1;
	barreiras2[65].sprite = 'O';
	barreiras2[65].condicao = 1;
	barreiras2[66].sprite = 'O';
	barreiras2[66].condicao = 1;
	barreiras2[67].sprite = 'O';
	barreiras2[67].condicao = 1;
	barreiras2[68].sprite = 'O';
	barreiras2[68].condicao = 1;
	barreiras2[69].sprite = 'O';
	barreiras2[69].condicao = 1;
	barreiras2[70].sprite = 'O';
	barreiras2[70].condicao = 1;

/*----------------------barreiras parte 3 ----------------------*/
	tsprite barreiras3[70];
	for (i = 0; i<=70; i++)
		barreiras3[i].sprite = ' ';

	barreiras3[0].sprite = 'O';
	barreiras3[0].condicao = 1; 
	barreiras3[1].sprite = 'O';
	barreiras3[1].condicao = 1; 
	barreiras3[6].sprite = 'O';
	barreiras3[6].condicao = 1; 
	barreiras3[7].sprite = 'O';
	barreiras3[7].condicao = 1; 
	barreiras3[21].sprite = 'O';
	barreiras3[21].condicao = 1; 
	barreiras3[22].sprite = 'O';
	barreiras3[22].condicao = 1; 
	barreiras3[27].sprite = 'O';
	barreiras3[27].condicao = 1; 
	barreiras3[28].sprite = 'O';
	barreiras3[28].condicao = 1; 
	barreiras3[42].sprite = 'O';
	barreiras3[42].condicao = 1; 
	barreiras3[43].sprite = 'O';
	barreiras3[43].condicao = 1; 
	barreiras3[48].sprite = 'O';
	barreiras3[48].condicao = 1; 
	barreiras3[49].sprite = 'O';
	barreiras3[49].condicao = 1; 
	barreiras3[63].sprite = 'O';
	barreiras3[63].condicao = 1; 
	barreiras3[64].sprite = 'O';
	barreiras3[64].condicao = 1; 
	barreiras3[69].sprite = 'O';
	barreiras3[69].condicao = 1; 
	barreiras3[70].sprite = 'O';
	barreiras3[70].condicao = 1; 



	raw();
	initscr();              /* inicia a tela */
    cbreak();               /* desabilita o buffer de entrada */
    noecho();               /* não mostra os caracteres digitados */
    nodelay(stdscr, TRUE);  /* faz com que getch não aguarde a digitação */
    keypad(stdscr, TRUE);   /* permite a leitura das setas */
    curs_set(FALSE);        /* não mostra o cursor na tela */

	c = 50;
	while (vitoria){
		getmaxyx(stdscr, maxY, maxX);
	    if (maxX < MAXCOL || maxY < MAXLIN){ 
			printf("terminar nao deve ter mais de %d linhas e %d colunas", MAXLIN, MAXCOL);
			endwin();	
		    exit(1);
        }
		tecla = getch();
		else if (tecla == KEY_LEFT && c > 0)
			c--;
		else if (tecla == KEY_RIGHT && c <(95))
			c++;
		else if (tecla == 'c'){
			imprime_sprite(&mundo , MAXLIN - 2 , c , tiro);
			subir = MAXLIN -2; 
		}
		else if (tecla == 'q'){
			endwin();
			exit(0);
		}
		clear();	
		/*atualiza_tiros(mundo,&vitoria);*/ 
		imprime_sprite(&mundo, MAXLIN-1 , c , canhao);
		imprime_barreiras(&mundo , MAXLIN-7 , 15 , &barreiras1 );
		imprime_barreiras(&mundo , MAXLIN-6 , 15 , &barreiras2 );
		imprime_barreiras(&mundo , MAXLIN-5 , 15 , &barreiras3 );
		imprime_aliens(&mundo , y , x , &vetoralien);
		imprime_aliens(&mundo , y+4 , x , &vetoralien);
		imprime_aliens(&mundo , y+8 , x , &vetoralien);
		imprime_aliens(&mundo , y+12, x , &vetoralien);
		imprime_aliens(&mundo , y+16 , x , &vetoralien);
		atualiza_tiros(mundo ,tiro ,subir );
		refresh();	

		if (!(controletemp % 10)) {
			if (dir == 1 && x>(MAXCOL - 75)){
				dir = -1;
				y++;
			}
			if (dir == -1 && x < 1){
				dir = 1; 
				y++;
			}
			x = x + dir; 
			controletemp = 0;	
			subir--;
		}

		usleep(500000/ 10); 
		controletemp++;		
	}
	return 0; 
}		
