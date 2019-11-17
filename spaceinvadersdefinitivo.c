#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<unistd.h>

#define MAXLIN 36
#define MAXCOL 100
#define DELAY 500000
#define FREQTIRO 800
#define FREQNAVEMAE 300
#define DIFICULADE 1
#define PONTOSALIENS 100
#define PONTOSMAE 1000 
#define PONTOSBOMBA 20

#define ALIEN11    " /M\\  |B|  (_) "
#define ALIEN12    " /M\\  |B|  )_( "
#define ALIEN21    " MMM {|C|} (_) "
#define ALIEN22    " MMM T|C|T )_( "
#define ALIEN31    " HHH M[C]M (_) " 
#define ALIEN32    " HHH J[C]L )_( " 
#define BOMBA     "$"
#define EXPLOSAO  " \\'/ -   - /,\\ "
#define BARREIRA  "O"
#define TIRO      "!"
#define	CANHAO    " /^\\ WWWWW"
#define	NAVEMAE   " /TTTTT\\ /--BCC--\\\\_MMMMM_/"
#define MINIEXPLOSAO "@"
#define EXPLOSAONAVEMAE "\\ : | : /- . . . -/ : | : \\"

typedef struct {
	int  tamlin;
	int  tamcol;
	char corpo1[32];
	char corpo2[32];
	char corpo3[32];
	int condicao;
} tsprite; 

struct t_nodo {
   	tsprite sprite;
   	int nlin;
   	int ncol; 
    struct t_nodo *prox;
    struct t_nodo *prev;
};
typedef struct t_nodo t_nodo;

struct t_lista {
    t_nodo *ini;
    t_nodo *atual;
    t_nodo *fim;
    int tam;
};
typedef struct t_lista t_lista;

tsprite alien1 = {3,5,ALIEN11,ALIEN12,EXPLOSAO,1};
tsprite alien2 = {3,5,ALIEN21,ALIEN22,EXPLOSAO,1};
tsprite alien3 = {3,5,ALIEN31,ALIEN32,EXPLOSAO,1};
tsprite navemae = {3,9,NAVEMAE,NAVEMAE,EXPLOSAONAVEMAE,1};
tsprite barreira = {1,1,BARREIRA,BARREIRA,BARREIRA,1};
tsprite tiro = {1,1,TIRO,TIRO,TIRO,1};
tsprite bomba = {1,1,BOMBA,BOMBA,BOMBA,1};
tsprite canhao = {2,5,CANHAO,CANHAO,CANHAO,1};

/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
/*----------------------------O CODIGO DE FUNCAOES DO TIPO LISTA COMECA AQUI-----------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
int inicializa_lista(t_lista *l){
  	t_nodo *inicio,*fim;
  	inicio = (t_nodo *) malloc(sizeof(t_nodo));
  	fim = (t_nodo *) malloc(sizeof(t_nodo));
  	l->ini = inicio;
  	l->fim = fim;
  	inicio->prox = fim;
  	fim->prev = inicio;
  	inicio->prev = NULL;
  	fim->prox = NULL;
  	l->tam = 0;
  	l->atual = l->ini; 
  	return 1;
}

int lista_vazia(t_lista *l){
  	if (l->tam == 0) 
    	return 1;
    return 0; 
}

int insere_inicio_lista(tsprite sprite, int nlin, int ncol, t_lista *l){
  	t_nodo *novo;
  	t_nodo *aux;
  	novo = (t_nodo *) malloc(sizeof(t_nodo));
  	if (novo == NULL)
    	return 0;  

 	novo->sprite = sprite; 
 	novo->nlin = nlin;
 	novo->ncol = ncol; 

 	aux = l->ini->prox;
  	l->ini->prox = novo;
  	aux->prev = novo;
  	novo->prox = aux;
  	novo->prev = l->ini;
  	l->tam ++;
  	return 1; 
}   

int tamanho_lista(int *tam, t_lista *l){
  	*tam = l->tam;
  	if (l->tam < 0)
    	return 0;
  	return 1;
 } 

int insere_fim_lista(tsprite sprite, int nlin, int ncol, t_lista *l){
    if (lista_vazia(l)){
    	insere_inicio_lista(sprite,nlin,ncol,l);
    	return 1;
  	}  
  	else{
    	t_nodo *novo;
    	t_nodo *aux;
    	novo = (t_nodo *) malloc(sizeof(t_nodo));
    	if (novo == NULL)
			return 0;

    	novo->sprite = sprite; 
 		novo->nlin = nlin;
 		novo->ncol = ncol;  

    	aux = l->fim->prev;
    	aux->prox = novo;
    	novo->prev = aux;
    	novo->prox = l->fim; 
    	l->fim->prev = novo;
    	l->tam++;
    	return 1; 
    } 
}

int remove_inicio_lista(tsprite *sprite, int *nlin, int *ncol, t_lista *l){
  t_nodo *aux;
  t_nodo *aux2;
  if (l->ini->prox == l->fim )
    return 0;

   aux = l->ini->prox; 
   aux2 = aux->prox;
   l->ini->prox = aux2;
   aux2->prev = l->ini;
   aux->prox = NULL;
   aux->prev = NULL;

   *sprite = aux->sprite;
   *nlin = aux->nlin;
   *ncol = aux->ncol;

   l->tam--;
   free(aux);  
   return 1; 

}

int remove_fim_lista(tsprite *sprite, int *nlin, int *ncol, t_lista *l){
  	t_nodo *aux;
  	t_nodo *aux2;
  	if (l->fim->prev == l->ini )
   		return 0; 
	
	aux = l->fim->prev; 
	aux2 = aux->prev;
  	l->fim->prev = aux2;
  	aux2->prox = l->fim;
  	aux->prox = NULL;
  	aux->prev = NULL;

  	*sprite = aux->sprite;
   	*nlin = aux->nlin;
   	*ncol = aux->ncol;

  	l->tam--;
	free(aux);
	return 1;
}   


int inicializa_atual_inicio(t_lista *l){
   l->atual = l->ini->prox;
   return 1; 
}
  
int inicializa_atual_fim(t_lista *l){
    l->atual = l->fim->prev; 
    return 1;
} 

void incrementa_atual(t_lista *l){
    if (l->atual->prox != NULL)
      l->atual = l->atual->prox;
    else 
      l->atual = NULL;  
} 

void decrementa_atual(t_lista *l){
  if (l->atual->prev != NULL)
    l->atual = l->atual->prev;
  else 
    l->atual = NULL;
}  

int consulta_item_atual(tsprite *sprite, int *nlin, int *ncol, t_lista *l){
	if (l->atual->prox == NULL || l->atual->prev == NULL)
	    return 0;

    *sprite = l->atual->sprite;
   	*nlin = l->atual->nlin;
   	*ncol = l->atual->ncol;

 	return 1; 
}

int remove_item_atual(tsprite *sprite, int *nlin, int *ncol, t_lista *l){
  	t_nodo *auxprox;
  	t_nodo *auxprev;
  	t_nodo *aux;
  	if (l->atual->prox == NULL || l->atual->prev == NULL)
    	return 0;
  
  	auxprev = l->atual->prev;
  	auxprox = l->atual->prox;
  	aux = l->atual;
  	l->atual = l->atual->prox;
  	if (l->atual->prox == NULL)
  		l->atual->prox = l->fim;
    auxprev->prox = auxprox;
  	auxprox->prev = auxprev;
    aux->prev = NULL;
    aux->prox = NULL;

	*sprite = aux->sprite;
   	*nlin = aux->nlin;
   	*ncol = aux->ncol;

	l->tam--; 
  	free(aux);
  	return 1; 
}

void destroi_lista(t_lista *l){
  int i ,tam ,o,p;
  tsprite sprite; 	
  l->atual = NULL;
  tamanho_lista(&tam , l );
  for (i = 1 ; i <= tam ; i++)
    remove_fim_lista (&sprite ,&o ,&p,l);
  free(l->ini);
  free(l->fim);
  l->ini = NULL;
  l->fim = NULL;
  l->tam = -1; 

}
/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------O CODIGO DO SPACE INVADERS COMECA AQUI----------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
void inicia_aliens(t_lista *l){
	int i;

	for(i = 0; i < 11; i++)
		insere_inicio_lista(alien1 , 5 , 1+(i*7) ,l);
	for(i = 0; i < 11; i++)
		insere_inicio_lista(alien2 , 9 , 1+(i*7) ,l);
	for(i = 0; i < 11; i++)
		insere_inicio_lista(alien2 , 13 , 1+(i*7) ,l);
	for(i = 0; i < 11; i++)
		insere_inicio_lista(alien3 , 17 , 1+(i*7) ,l);
	for(i = 0; i < 11; i++)
		insere_inicio_lista(alien3 , 21 , 1+(i*7) ,l);
}

void inicia_barreiras(t_lista *l){
	int j;

	for (j = 14 ;j <= 85; j++){
		if(j % 21 == 16 || j % 21 == 17 || j % 21 == 18 || j % 21 == 19 || j % 21 == 20) 	
			insere_inicio_lista(barreira , 29 , j ,l);
		if(j % 21 == 15 || j % 21 == 16|| j % 21 == 17 || j % 21 == 18 || j % 21 == 19 || j % 21 == 20 || j % 21 == 0) 				
			insere_inicio_lista(barreira , 30 , j ,l);
		if(j % 21 == 15 || j % 21 == 16 || j % 21 ==20 || j % 21 ==0) 	
			insere_inicio_lista(barreira , 31 , j ,l);
	}
}


void movimenta_aliens(t_lista *aliens,int *dir){
	int i,j,tam;
	tsprite aux;
	int lin,col;
	int abaixa1, abaixa2;

	abaixa1 = 0;
	abaixa2 = 0;

	tamanho_lista(&tam, aliens);
	inicializa_atual_inicio(aliens);
	for (i = 1; i<=tam; i++){
		consulta_item_atual(&aux,&lin,&col,aliens);
			if (col+5 == MAXCOL)
				abaixa1 = 1; 
			if (col == 0)
				abaixa2 = 1; 
		incrementa_atual(aliens);
	}
	if (abaixa1 == 1 && *dir == 1)
		*dir = -1;
	 else if (abaixa2 == 1 && *dir == -1)
		*dir = 1;
	inicializa_atual_inicio(aliens);
	for (j = 1; j<=tam; j++){
		remove_item_atual(&aux,&lin,&col,aliens);
		col = col + *dir;
		lin = lin + abaixa1 + abaixa2;
		insere_inicio_lista(aux,lin,col,aliens);
	}
	
}

void movimenta_tiros(t_lista *tiros){
	int i,tam,lin,col; 	
	tsprite tiro;

	if (lista_vazia(tiros) == 0){
		tamanho_lista(&tam, tiros);
		inicializa_atual_inicio(tiros);
		for (i = 1; i<=tam ;i++){
			remove_item_atual(&tiro,&lin,&col,tiros);
			lin--;
			if (lin >= 0)
				insere_inicio_lista(tiro,lin,col,tiros);	
		}
	}
}

void criabombas(t_lista *aliens , t_lista *bombas){
	int i,tam,lin,col;
	tsprite sprite;
	if(lista_vazia(aliens) == 0){
		tamanho_lista(&tam,aliens);
		inicializa_atual_inicio(aliens);
		for (i = 1; i<=tam ;i++){
			consulta_item_atual(&sprite,&lin,&col,aliens);
			if (rand() % FREQTIRO == 1)
				insere_inicio_lista(bomba,lin+2,col+2,bombas);
		incrementa_atual(aliens);	
		}	
	}
}


void movimenta_bombas(t_lista *bombas){
	int i,tam,lin,col; 	
	tsprite sprite;
	sprite = bomba;
	if (lista_vazia(bombas) == 0){
		tamanho_lista(&tam, bombas);
		inicializa_atual_inicio(bombas);
		for (i = 1; i<=tam ;i++){
			remove_item_atual(&sprite,&lin,&col,bombas);
			lin++;
			if (lin <= 35)
				insere_inicio_lista(sprite,lin,col,bombas);	
		}
	}
}

void movnavemae(t_lista *nave){
	int lin, col ,tam;
	tsprite s; 
	s = navemae;
	tamanho_lista(&tam,nave);
	if (tam == 0 && (rand() % FREQNAVEMAE == 1)){
		insere_inicio_lista(navemae,2,0,nave);
	}
	else if (tam == 1){
		remove_inicio_lista(&s,&lin,&col,nave);
		col++;
		if (col <= 93)
		insere_inicio_lista(s,lin,col,nave);
	} 

}

int comparacolisao(tsprite *s1, int x1, int y1, tsprite *s2, int x2,int y2 ){
	int i,j,k,l;
	int linha1 ,linha2, coluna1,coluna2;  
	for (i = 1; i <= s1->tamlin; i++){
		for (j = 1; j <= s1->tamcol ; j++){
			for (k = 1; k <= s2->tamlin; k++){
				for (l = 1; l <= s2->tamcol ; l++){
					linha1 = i+x1;
					linha2 = k+x2;
					coluna1 = j+y1;
					coluna2 = l+y2;
					if ((linha1 == linha2) && (coluna1 == coluna2))
						return 1; 
				}
			}	
		}
	}
	return 0; 				
}


void analizacolisoes(t_lista *lista1,t_lista *lista2,int *pontuacao, int multiplicador){
	int i ,j,y, lin1, col1, lin2, col2;
	int lixo1,lixo3;
	int tam1, tam2;
	int flag;
	tsprite s1, s2, lixo5;
	tamanho_lista(&tam1,lista1);
	tamanho_lista(&tam2,lista2);
	inicializa_atual_inicio(lista1);
	inicializa_atual_inicio(lista2);

	if (!lista_vazia(lista1)){
		for (i = 1; i<=tam1 ; i++){
			flag = 1;
			consulta_item_atual(&s1,&lin1,&col1,lista1);
			inicializa_atual_inicio(lista2);
			if (!lista_vazia(lista2)){
				for (j = 1; j<=tam2; j++){
					if (flag){
						consulta_item_atual(&s2,&lin2,&col2,lista2);
						y = comparacolisao(&s1,lin1,col1,&s2,lin2,col2);
						if (y == 1){
							remove_item_atual(&lixo5,&lixo1,&lixo3,lista1);
							remove_item_atual(&s2,&lin2,&col2,lista2);
							s2.condicao = 3; 
							insere_inicio_lista(s2,lin2,col2,lista2);
							flag = 0;	
							*pontuacao = *pontuacao + multiplicador;
						}
						else
							incrementa_atual(lista2);
					}	
				}
			if (flag)	
				incrementa_atual(lista1);		
			}	
		}
	}
}			

void analizacolisoescanhao(t_lista *lista1,t_lista *lista2, int *vitoria){
	int i ,j,y, lin1, col1, lin2, col2;
	int lixo1,lixo3;
	int tam1, tam2;
	int flag;
	tsprite s1, s2, lixo5;
	tamanho_lista(&tam1,lista1);
	tamanho_lista(&tam2,lista2);
	inicializa_atual_inicio(lista1);
	inicializa_atual_inicio(lista2);

	if (!lista_vazia(lista1)){
		for (i = 1; i<=tam1 ; i++){
			flag = 1;
			consulta_item_atual(&s1,&lin1,&col1,lista1);
			inicializa_atual_inicio(lista2);
			if (!lista_vazia(lista2)){
				for (j = 1; j<=tam2; j++){
					if (flag){
						consulta_item_atual(&s2,&lin2,&col2,lista2);
						y = comparacolisao(&s1,lin1,col1,&s2,lin2,col2);
						if (y == 1){
							remove_item_atual(&lixo5,&lixo1,&lixo3,lista1);
							remove_item_atual(&s2,&lin2,&col2,lista2);
							s2.condicao = 3; 
							insere_inicio_lista(s2,lin2,col2,lista2);
							flag = 0;	
							*vitoria = 0; 
						}
						else
							incrementa_atual(lista2);
					}	
				}
			if (flag)	
				incrementa_atual(lista1);
			}	
		}
	}
}

void analizacolisoescanhaobarreira(t_lista *lista1,t_lista *lista2){
	int i ,j,y, lin1, col1, lin2, col2;
	int lixo1,lixo3;
	int tam1, tam2;
	int flag;
	tsprite s1, s2, lixo5;
	tamanho_lista(&tam1,lista1);
	tamanho_lista(&tam2,lista2);
	inicializa_atual_inicio(lista1);
	inicializa_atual_inicio(lista2);

	if (!lista_vazia(lista1)){
		for (i = 1; i<=tam1 ; i++){
			flag = 1;
			consulta_item_atual(&s1,&lin1,&col1,lista1);
			inicializa_atual_inicio(lista2);
			if (!lista_vazia(lista2)){
				for (j = 1; j<=tam2; j++){
					if (flag){
						consulta_item_atual(&s2,&lin2,&col2,lista2);
						y = comparacolisao(&s1,lin1,col1,&s2,lin2,col2);
						if (y == 1){
							remove_item_atual(&lixo5,&lixo1,&lixo3,lista2);
							flag = 0;	
						}
						else
							incrementa_atual(lista2);
					}	
				}
			if (flag)	
				incrementa_atual(lista1);		
			}	
		}
	}
}




void inicia_canhao(t_lista *l, int c){
	insere_inicio_lista(canhao,34,c,l);
}

void atualiza_canhao(t_lista *l,int c){
	tsprite sprite;
	int lin, lixo;

	inicializa_atual_inicio(l);
	remove_item_atual(&sprite,&lin,&lixo,l);
	insere_inicio_lista(sprite,lin,c,l);
}


void inicia_sprite(tsprite *sprite,int lin ,int col,int ver ){
	int i,j ; 
 
	for(i = 0; i < sprite->tamlin; i++)
		for(j = 0; j < sprite->tamcol; j++){
			move(lin + i ,col + j);
			if(ver == 1)
				addch(sprite->corpo1[i * sprite->tamcol + j]);
			else if (ver == 2)
				addch(sprite->corpo2[i * sprite->tamcol + j]);
			else if (ver == 3)
				addch(sprite->corpo3[i * sprite->tamcol + j]);
		}
}

void imprime_lista(t_lista *l,int ver){
	int tam,i,lin,col;
	tsprite s;  
	int lixo1,lixo2;
	tsprite lixo3; 

	tamanho_lista(&tam,l);
	inicializa_atual_inicio(l);
	for (i = 0 ; i<= tam ; i++){
		consulta_item_atual(&s,&lin,&col,l);
		if (s.condicao == 3){
			inicia_sprite(&s ,lin,col,s.condicao);
			remove_item_atual(&lixo3,&lixo1,&lixo2,l);
		}
		else {
			inicia_sprite(&s ,lin,col,ver);
			incrementa_atual(l);
		}
	}
}


/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------A MAIN COMECA AQUI-----------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
int main(){
	int maxX, maxY ;
	int nivel = 0;
	int c;
	int dir = 1, iter;
	int tecla;
	int tam = 0 ;
	int ver  = 1;
	int pontuacao = 0; 
	int tamanhoaliens,velocidadealiens;
	int vitoria = 1; 
	t_lista aliens;
	t_lista barreiras;
	t_lista exercito;
	t_lista tiros;
	t_lista bombas;
	t_lista maedetodos;
	inicializa_lista(&aliens);
	inicializa_lista(&barreiras);
	inicializa_lista(&exercito);
	inicializa_lista(&tiros);
	inicializa_lista(&bombas);
	inicializa_lista(&maedetodos);

	raw();
	initscr();              /* inicia a tela */
    cbreak();               /* desabilita o buffer de entrada */
    noecho();               /* não mostra os caracteres digitados */
    nodelay(stdscr, TRUE);  /* faz com que getch não aguarde a digitação */
    keypad(stdscr, TRUE);   /* permite a leitura das setas */
    curs_set(FALSE);        /* não mostra o cursor na tela */

	c = (MAXCOL - canhao.tamcol) / 2; 
	 inicia_canhao(&exercito,c);
/*------------------------------------------------------------------------------------------------------*/
	while(vitoria){
		inicia_aliens(&aliens);
		inicia_barreiras(&barreiras);
		tamanho_lista (&tamanhoaliens,&aliens);
		iter = 1; 
		while(vitoria && tamanhoaliens != 0){
			getmaxyx(stdscr, maxY, maxX);
		    if (maxX < MAXCOL || maxY < MAXLIN){ 
				endwin();
				printf("terminar nao deve ter mais de %d linhas e %d colunas", MAXLIN, MAXCOL);
				exit(1);
			}				

			tecla = getch();
			if (tecla == 'd' && c > 0)
				c++;
			else if (tecla == 'a' && c < (MAXCOL - canhao.tamcol))
				c--;
			else if (tecla == KEY_LEFT && c > 0)
				c--;
			else if (tecla == KEY_RIGHT && c <(MAXCOL - canhao.tamcol))
				c++;
			else if (tecla == ' '){
				tamanho_lista(&tam,&tiros);
				if (tam < 4)
					insere_inicio_lista(tiro,33,c+2,&tiros);
			}
			else if (tecla == 'q'){
				endwin();
				exit(1);
			}		
			clear();
			mvprintw(0,44 ,"pontuacao: %i ", pontuacao);
			atualiza_canhao(&exercito,c);
			criabombas(&aliens,&bombas);

			analizacolisoes(&tiros,&aliens,&pontuacao,PONTOSALIENS);
			analizacolisoes(&tiros,&barreiras,&pontuacao,0);
			analizacolisoes(&tiros,&bombas,&pontuacao,PONTOSBOMBA);
			analizacolisoes(&tiros,&maedetodos,&pontuacao,PONTOSMAE);
			analizacolisoes(&bombas,&barreiras,&pontuacao,0);
			analizacolisoescanhao(&bombas,&exercito,&vitoria);
			analizacolisoescanhao(&aliens,&exercito,&vitoria);
			analizacolisoescanhaobarreira(&aliens,&barreiras);

			imprime_lista(&tiros,ver);
			imprime_lista(&aliens,ver);
			imprime_lista(&barreiras,ver);
			imprime_lista(&exercito,ver);
			imprime_lista(&bombas,ver);
			imprime_lista(&maedetodos,ver);

			refresh ();


			tamanho_lista (&tamanhoaliens,&aliens);
			velocidadealiens = (tamanhoaliens/(DIFICULADE+nivel))+ 1;
			if (iter % 7 == 0 )
				movimenta_bombas(&bombas);
			if (iter % 3 == 0 )
				movnavemae(&maedetodos);
			if (iter % 3 == 0)
				movimenta_tiros(&tiros);
			if (iter % velocidadealiens == 0){
				movimenta_aliens(&aliens, &dir);
				if (ver == 1)
					ver = 2; 
				else if (ver == 2)
					ver = 1; 	
			}

			usleep(16666);
			iter ++;
		}
	nivel++	;
	}	
clear();	
mvprintw(17,33 ,"GAME OVER, PONTUACAO FINAL: %i", pontuacao);
refresh();
sleep(7);
endwin();	
return 0;
}	



