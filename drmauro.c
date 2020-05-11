#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "drmauro.h"
#include "movement.c"

/* Funzione di Debug
	stampa il contenuto della matrice*/
void stampa_elemento_campo(struct gioco* gioco){
	int i, j;

	for (i=0; i< RIGHE; i++){

		printf ("\n|");
		for (j=0; j< COLONNE; j++){
			printf(" %d", gioco->campo[i][j].id);
		}
		printf (" |\n");
	}
	printf ("\n");
}

/* 	funzione che controlla se una cella è sicura per la generazione di un virus
	se la cella è vuota e nella riga e colonna della cella non vi sono più di MAX_VIRUS_LINE virus di fila ritorna 1
	0 altrimenti
*/
int possible ( struct gioco* gioco, int r, int c, enum colore color){

	if (gioco->campo[r][c].tipo != VUOTO)
		return 0;

	else {
		int i;

		for(i=0; i <= MAX_VIRUS_LINE; i++){
			if ( r - i >= 0 && r + (MAX_VIRUS_LINE - i) < RIGHE){
				int j, n=0;

				for	(j= r-i; j <= r + MAX_VIRUS_LINE-i; j++){
					if ( gioco->campo[j][c].tipo == MOSTRO && gioco->campo[j][c].colore == color )
						n++;
				}
				if (n>= MAX_VIRUS_LINE) return 0;
			}

			if ( c - i >= 0 && c + (MAX_VIRUS_LINE - i) < COLONNE){
				int j, n=0;

				for	(j= c-i; j <= c + MAX_VIRUS_LINE-i; j++){
					if ( gioco->campo[r][j].tipo == MOSTRO && gioco->campo[r][j].colore == color )
						n++;
				}
				if (n>= MAX_VIRUS_LINE) return 0;
			}
		}
	}
	return 1;
}

/*
La funzione si occupa anche di impostare i valori per la creazione delle pillole(id delle celle, r e c del gioco ).
*/
void carica_campo(struct gioco *gioco, char *percorso) {

	int r, c;
    size_t a = COLONNE;
	FILE *file;
	char *vet = (char *)malloc(sizeof(char)*a);
	gioco->r = 0;
	gioco->c = COLONNE/2-1;
	gioco->campo[gioco->r][gioco->c].id = gioco->campo[gioco->r][gioco->c +1].id = 1;
	
	for (r = 0; r < RIGHE; r++)
		for(c = 0; c < COLONNE; c++){
			gioco->campo[r][c].id = -1;
			gioco->campo[r][c].posizione = SINGOLO;
			gioco->campo[r][c].tipo = VUOTO;
		}
	
	file = fopen(percorso, "r");
	if (file){
		for (r = 0; r < RIGHE; r++){
			getline(&vet, &a, file);
			for(c = 0; c < COLONNE; c++){
				switch(vet[c]){
					case 'R':{
					gioco->campo[r][c].tipo = MOSTRO;
					gioco->campo[r][c].colore = ROSSO;
					} break;
					case 'G':{
					gioco->campo[r][c].tipo = MOSTRO;
					gioco->campo[r][c].colore = GIALLO;
					} break;
					case 'B':{
					gioco->campo[r][c].tipo = MOSTRO;
					gioco->campo[r][c].colore = BLU;
					} break;
				}
			}
		}
	}
	else{
		perror("File corrotto oppure inesistente.");
	}
	fclose(file);
	free(vet);
}

/* la funzione scorre il campo da gioco e imposta tutte le id a "-1", tutte le posizioni a "SINGOLO".
	Sceglie una posizione a caso e dei colori a caso finchè non ha finito di sistemare i virus.
	Se la posizione è sicura allora inserisce il virus.
	La funzione si occupa anche di impostare i valori per la creazione delle pillole(id delle celle, r e c del gioco ).
*/
void riempi_campo(struct gioco *gioco, int difficolta) {

	int virus_n = 4 * (difficolta + 1), i, j ;
	
	gioco->active_id = 0;
	for (i=0; i< RIGHE; i++)
		for (j=0; j < COLONNE; j++){
			gioco->campo[i][j].id = -1;
			gioco->campo[i][j].posizione = SINGOLO;
		}

	gioco->r = 0;
	gioco->c = COLONNE/2-1;
	gioco->campo[gioco->r][gioco->c].id = gioco->campo[gioco->r][gioco->c +1].id = 1;
	srand(time(NULL));

	while (virus_n > 0) {

		int r = (rand() % (RIGHE - 5) + 5);
		int c = (rand() % COLONNE);
		enum colore color = rand() % 3;

		if (possible(gioco, r, c, color)) {
			gioco->campo[r][c].tipo = MOSTRO;
			gioco->campo[r][c].colore = color;
			virus_n--;
		}
	}
}


/*	la funzione imposta la posizione di ogni cella con id differente a quelle adiacenti in SINGOLO	*/
/*void single(struct gioco * gioco){
	int i,j;
	for (i=RIGHE-1; i > 0; i--){

		for (j=0; j< COLONNE-1;j++)
			if( (gioco->campo[i][j].id != gioco->campo[i][j+1].id ) && (gioco->campo[i][j].id != gioco->campo[i-1][j].id ))
				gioco->campo[i][j].posizione = SINGOLO;
	}
	i = 0;
	for (j=0; j< COLONNE-1;j++)
		if ( (gioco->campo[i][j].id!= gioco->campo[i][j+1].id ) && (gioco->campo[i][j].id != gioco->campo[i+1][j].id ) )
			gioco->campo[i][j].posizione = SINGOLO;

	j= COLONNE-1;
	for (i=0; i<RIGHE-1; i++)
		if ( (gioco->campo[i][j].id!= gioco->campo[i][j-1].id) && ((gioco->campo[i][j].id!= gioco->campo[i+1][j].id)) )
			gioco->campo[i][j].posizione = SINGOLO;

	i++;
	j++;
	if( (gioco->campo[i][j].id != gioco->campo[i-1][j].id ) &&( gioco->campo[i][j].id != gioco->campo[i][j-1].id) )
		gioco->campo[i][j].posizione = SINGOLO;
}
*/

/* Verifica che una pillola possa cadere*/ 
int cade(struct gioco * gioco, int i, int j){
	int res = 0;
	
	if (i+1 < RIGHE && gioco->campo[i][j].tipo == PASTIGLIA && gioco->campo[i+1][j].tipo == VUOTO 
		&& ((j+1<COLONNE && (gioco->campo[i][j].id != gioco->campo[i][j+1].id || (gioco->campo[i][j].id == gioco->campo[i][j+1].id && gioco->campo[i+1][j+1].tipo == VUOTO)))
		|| (j == COLONNE - 1 && gioco->campo[i][j-1].id != gioco->campo[i][j].id) ) && (j == 0 || (j-1 >=0 && gioco->campo[i][j-1].id != gioco->campo[i][j].id )))
		res = 1;
	return res;
}
/* la funzione sposta in basso tutte le pastiglie con uno spazio vuoto sottostante e affiancata da celle dall'id differente dalla propria.\
ritorna 0 se non sono state mosse celle, il numero di celle spostate altrimenti*/

int gravita(struct gioco * gioco){
	int res = 0, i, j;

	for (i= RIGHE-1; i>=0; i--)
		for (j=0; j< COLONNE; j++)
			if(cade(gioco, i, j)){
				scambia_celle( &(gioco->campo[i+1][j]), &(gioco->campo[i][j]) );
				res++;
			}

	return res;
}

/* funzione che somma potenze di 2 a partire da 2^1 e le moltiplica per 100 */
int punti(int n){
	int res=0, count=1, i;
	for (i=0; i<n; i++){
		count *=2;
		res += count;
	}
	return res*100;
}

/*
Funzione che conta le celle con il parametro posizione settato a DELETED e ne setta il parametro tipo a VUOTO,
la funzione poi chiama la funzione single;
infine chiama la funzione gravita e ritorna 1 se ha cancellato qualcosa, 0 altrimenti.
Salva inoltre il numero di virus cancellati e ne calcola il punteggio (senza combo) con la funzione punti.
Salva il punteggio della mossa (senza combo) nella cella di memoria puntata da *points.
*/
int esplodi(struct gioco*gioco, int* points){
	int i, j, n=0, flag = 0;

	for (i=0; i< RIGHE; i++){
		for (j=0; j< COLONNE;j++){
			int  k=0;

			if (gioco->campo[i][j].tipo != VUOTO){
				while(j+k < COLONNE && gioco->campo[i][j+k].tipo != VUOTO && gioco->campo[i][j].colore == gioco->campo[i][j+k].colore)
			 		k++;
				if ( k >= 4){
					k--;
					for( ; k>=0; k--){
						gioco->campo[i][j+k].posizione = DELETED;
						gioco->campo[i][j+k].id = -1;
					}
				}
				k=0;
				while(i+k < RIGHE && gioco->campo[i+k][j].tipo != VUOTO && gioco->campo[i][j].colore == gioco->campo[i+k][j].colore)
	 				k++;
				if ( k >= 4){
					k--;
					for( ; k>=0; k--){
						gioco->campo[i+k][j].posizione = DELETED;
						gioco->campo[i+k][j].id = -1;
					}
				}
			}
		}
	}

	for (i=0; i< RIGHE; i++){
		for (j=0; j< COLONNE;j++){
			if(gioco->campo[i][j].posizione == DELETED){
				flag =1;
				if (gioco->campo[i][j].tipo == MOSTRO)
					n++;
				gioco->campo[i][j].tipo = VUOTO;
				gioco->campo[i][j].posizione = SINGOLO;

			}
		}
	}
	*points = punti(n);
	return flag;
}

/*
Per i seguenti valori di active id la funzione si comporta in modo differente
-Active id = 0:
	grea una pillola e decrementa l'active id
-Active id compreso tra ( 0 , - MOSSE_PER_TURNO ]:
	attraverso uno switch la funzione muove la pillola a seconda del comando
	se le mosse sinistra, destra, o ruota non vanno a buon fine la funzione si comporta come se non avesse ricevuto comando
	se queste vanno a buon fine decrementano active id
	se la mosse down è eseguita l'active id è impostato a - MOSSE_PER_TURNO -1
	se la mossa è nessuna la funzione chiama la funzione step_down e ne salva l'esecuzione(si è mossa o meno la pillola).
	in caso affermativo l'active id viene decrementata, in caso contrario viene impostato l'active id a 1
-Active id = 1
	è chiamata la funzione esplodi e se viene impostata una cella a VUOTO viene impostato l'active id a 2, a 0 altrimenti
	se vengono eliminati virus viene calcolato il punteggio con la funzione punti e aggiunto al punteggio corrente
-Active id = 2^n, n>=1
	è chiamata la funzione gravita e se va a buon fine è chiamata la funzione esplodi e il punteggio è calcolato come con l'active id a 1
	il punteggio viene però calcolato moltiplicandolo per l'active id, che assume valori di potenze di 2.
	se vengono eliminati altri virus in combo l'active id viene moltiplicato per 2 e si continua a chiamare la funzione gravita finchè gravita o esplodi falliscono
 */

void step(struct gioco *gioco, enum mossa comando) {
	int flag = 0, goes = -1, x, points=0;

	if (gioco->active_id == 0){
		generate_pill(gioco);
		gioco->active_id = -1;
	}
	else if (gioco->active_id < 0){
		if(gioco->active_id > MOSSE_PER_TURNO * (-1) -1 ){
			switch( comando){
				case GIU :{
					down(gioco); 
					gioco->active_id = 1;
					goes = 0;} break;
				case SINISTRA :
					flag = sx(gioco); break;
				case DESTRA :
					flag = dx(gioco); break;
				case ROT_SX :
					flag = anticlockwise(gioco); break;
				case ROT_DX :
					flag = clockwise(gioco); break;
				case NONE :
					goes = step_down(gioco); break;
				default: break;
			}
			/*
			chiama la step_down per mosse scorrette
			evita di chiamare la step_down se il comando è giu
			*/
			if (flag)
				gioco->active_id -= 1;

			else if (comando != NONE && comando != GIU)
				goes = step_down(gioco);
		}
		else
			goes = step_down(gioco);

		if(goes==1 && gioco->active_id < MOSSE_PER_TURNO-1)
			gioco->active_id = -1;
		
		else if  (!goes)
			gioco->active_id = 1; 
	}

	else if (gioco->active_id == 1) {
		gioco->campo[0][COLONNE /2 -1].id = gioco->campo[0][COLONNE /2 ].id = gioco->campo[gioco->r][gioco->c].id+1;
		gioco->r = 0;
		gioco->c = COLONNE/2 - 1;
		x = esplodi(gioco, &points);
		if (x){
			gioco->punti += gioco->active_id * points;
			gioco->active_id *= 2;
		}
		else 
			gioco->active_id = 0;
	
	}
	else if (gioco->active_id >1){
		int k = gravita(gioco);
		if (!k){
			int points;
			x = esplodi(gioco, &points);	/* x indica se la esplodi ha fatto esplodere qualcosa */
			if(x){
				gioco->punti+= gioco->active_id * points;
				/*if (points)*/
				gioco->active_id *= 2;
			}
			else{
				gioco->campo[0][COLONNE /2 -1].id = gioco->campo[0][COLONNE /2 ].id = gioco->campo[gioco->r][gioco->c].id+1;
				gioco->r = 0;
				gioco->c = COLONNE/2 - 1;
				gioco->active_id = 0;
			}
		}
	}
}

/* 	ritorna sconfitta se le prime due celle sono occupate quando si dovrebbe procedere a creare una nuova pillola
	ritorna vittoria se non sono trovati virus all'inizio della step
	ritorna in corso altrimenti*/
enum stato vittoria(struct gioco *gioco) {
	int trovato = 0;
	int i = 5;
	int j = 0;

	if ( gioco->active_id == 0 && ( gioco->campo[0][COLONNE /2 -1].tipo != VUOTO ||  gioco->campo[0][COLONNE /2].tipo != VUOTO  ) )
		return SCONFITTA;

	while (!trovato && i < RIGHE) {
		j = 0;
		while (!trovato && j < COLONNE) {
			if ((*gioco).campo[i][j].tipo == MOSTRO)
				trovato = 1;
			j++;
		}
		i++;
	}

	if (!trovato) return VITTORIA;

	return IN_CORSO;
}
