#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<unistd.h>

#define MAXLIN 35 
#define MAXCOL 100

#define ALIEN1    "/M\\|B|(_)"
#define ALIEN2    " MMM {|C|} (_) "
#define ALIEN3    " HHH T[C]T (_) " 
#define BOMBA     "$"
#define EXPLOSAO  " \\'/ -   - /,\\ "
#define BARREIRA  " OOOOO OOOOOOOOO   OO"
#define TIRO      "!"
#define	CANHAO    " /^\\ WWWWW"
#define	NAVEMAE   "U"

#define DELAY 500000

typedef struct {
	int nlin;
	int  ncol;
	char corpo1[32];
	char corpo2[32];
	int condicao; 
} tsprite; 

tsprite alien1 = {3,3,ALIEN1,ALIEN1}; 
tsprite alien2 = {3,5,ALIEN2,ALIEN2};
tsprite alien3 = {3,5,ALIEN3,ALIEN3};
tsprite bomba = {1,1,BOMBA,BOMBA};
tsprite explosao = {3,5,EXPLOSAO,EXPLOSAO};
tsprite barreira = {3,7,BARREIRA,BARREIRA};
tsprite tiro = {1,1,TIRO,TIRO};
tsprite canhao = {2,5,CANHAO,CANHAO};
tsprite navemae = {1,1,NAVEMAE,NAVEMAE};

void inicia_sprite(tsprite *s,int lin ,int col ,int ver ){
	int i,j ; 
 
	for(i = 0; i < s->nlin; i++)
		for(j = 0; j < s->ncol; j++){
			move(lin + i ,col + j);
			if(ver == 1)
				addch(s->corpo1[i * s->ncol + j]);
			else 
				addch(s->corpo2[i * s->ncol + j]);
		}
	s->condicao = 1;
}

void atualiza_sprite(tsprite *s,int lin ,int col ,int ver ){
	int i,j ; 
	if (s->condicao == 1){
	for(i = 0; i < s->nlin; i++)
		for(j = 0; j < s->ncol; j++){
			move(lin + i ,col + j);
			if(ver == 1)
				addch(s->corpo1[i * s->ncol + j]);
			else 
				addch(s->corpo2[i * s->ncol + j]);
		}
	}
}

void inicia_aliens(int lin,int col , int ver){
	int i;

	for(i = 0; i < 11; i++)
		inicia_sprite (&alien1 , lin , 1+col+(i*7) , ver);
	for(i = 0; i < 11; i++)
		inicia_sprite (&alien2 , lin+4 , col+(i*7) , ver);
	for(i = 0; i < 11; i++)
		inicia_sprite (&alien2 , lin+8 , col+(i*7) , ver);
	for(i = 0; i < 11; i++)
		inicia_sprite (&alien3 , lin+12 , col+(i*7), ver);
	for(i = 0; i < 11; i++)
		inicia_sprite (&alien3 , lin+16 , col+(i*7) , ver);
}

void atualiza_aliens(int lin,int col , int ver){
	int i;

	for(i = 0; i < 11; i++)
		atualiza_sprite (&alien1 , lin , 1+col+(i*7) , ver);
	for(i = 0; i < 11; i++)
		atualiza_sprite (&alien2 , lin+4 , col+(i*7) , ver);
	for(i = 0; i < 11; i++)
		atualiza_sprite (&alien2 , lin+8 , col+(i*7) , ver);
	for(i = 0; i < 11; i++)
		atualiza_sprite (&alien3 , lin+12 , col+(i*7), ver);
	for(i = 0; i < 11; i++)
		atualiza_sprite (&alien3 , lin+16 , col+(i*7) , ver);
}

void inicia_barreiras(int lin, int col , int ver){
	int i;

	for (i = 0; i < 4;  i++)
		inicia_sprite(&barreira , lin , col+(i*21) , ver);
}

void atualiza_barreiras(int lin, int col , int ver){
	int i;

	for (i = 0; i < 4;  i++)
		inicia_sprite(&barreira , lin , col+(i*21) , ver);
}

atualiza_tiros(WINDOW *janela,int lin , int col){
	int i, j ,x ,y ,sobe ,desce;
	sobe = -1; 
	desce = 1; 

	for (i = 0; i < lin; i++)
		for (j = 0; j <col; j++){
			mvwscanw(janela,i,j,"%c",&x);
			if (x == '!'){
				mvwscanw(janela,i,j-1,"%c",&y);
				if (y != ' ')
					if (y == 'O');

			}	
			else 	
				atualiza_sprite(&tiro, lin, col, 1);

			if (x == '$'){
				mvwscanw(janela,i,j+1,"%c",&y);
				if (y != ' '){

				}
			}
		}

}






int main (){
	int maxX, maxY ;
	int c, x = 0, y = 5;
	int tecla, dir , iter = 0;
	int q, w , atirado;
	int subir = 1 ; 
	WINDOW *janela; 

	raw();
	initscr();              /* inicia a tela */
    cbreak();               /* desabilita o buffer de entrada */
    noecho();               /* não mostra os caracteres digitados */
    nodelay(stdscr, TRUE);  /* faz com que getch não aguarde a digitação */
    keypad(stdscr, TRUE);   /* permite a leitura das setas */
    curs_set(FALSE);        /* não mostra o cursor na tela */
    
	c = (MAXCOL - canhao.ncol) / 2; 
	dir =1 ;
	getmaxyx(stdscr, maxY, maxX);

	inicia_barreiras (MAXLIN - 7, 15 , 1 ); 
	inicia_aliens (y ,x ,x % 2);
	inicia_sprite(&canhao, MAXLIN - 1 , c , 1); 

	while(y < MAXLIN -1){
		getmaxyx(stdscr, maxY, maxX);
	    if (maxX < MAXCOL || maxY < MAXLIN){ 
			printf("terminar nao deve ter mais de %d linhas e %d colunas", MAXLIN, MAXCOL);
			endwin();
			exit(1);
		}
		
		tecla = getch();
		if (tecla == 'd' && c > 0)
			c++;
		else if (tecla == 'a' && c < (MAXCOL - canhao.ncol))
			c--;
		else if (tecla == KEY_LEFT && c > 0)
			c--;
		else if (tecla == KEY_RIGHT && c <(MAXCOL - canhao.ncol))
			c++;
		else if (tecla == 'c')
			inicia_sprite(&tiro, MAXLIN -3 ,c + 3 , 1);
		else if (tecla == 'q'){
			endwin();
			exit(0);
		}		
		clear();
		atualiza_tiro(janela);
		atualiza_aliens (y ,x ,x % 2);
		atualiza_barreiras(MAXLIN - 7, 15 , 1 );
		inicia_sprite(&canhao, MAXLIN - 1 , c , 1); 
		refresh ();

		if (!(iter % 10)) {
			if (dir == 1 && x>(MAXCOL - 75)){
				dir = -1;
				y++;
			}
			if (dir == -1 && x < 1){
				dir = 1; 
				y++;
			}
			x = x + dir; 
			iter = 0;	
			subir--; 
		}

		usleep(DELAY/ 10); 
		iter ++;
	}
return 0;
}	
