#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "drmauro.h"

void generate_pill(struct gioco * gioco){
	enum colore color1 = rand() %3, color2 = rand() %3;
	int r = gioco->r, c= gioco->c;
	struct cella * pill1 = &(gioco->campo[r][c]);
	struct cella * pill2 = &(gioco->campo[r][c+1]);

	pill1->tipo = pill2->tipo = PASTIGLIA;
	pill1->posizione = pill2->posizione = ORIZZONTALE;
	pill1->colore = color1;
	pill2->colore = color2;
}
/* scambia i parametri di due celle*/
void scambia_celle(struct cella* cell1, struct cella* cell2){
	enum colore color1 = cell1->colore;
	enum posizione pos1 = cell1->posizione;
	enum contenuto type1 = cell1->tipo;
	int id1 = cell1->id;

	cell1->colore = cell2 ->colore;
	cell1->posizione = cell2-> posizione;
	cell1->tipo = cell2->tipo;
	cell1->id = cell2->id;

	cell2->colore = color1;
	cell2->posizione = pos1;
	cell2->tipo = type1;
	cell2->id = id1;

}
/* controlla se sotto la pillola vi sono celle vuote:
	se vi sono ritorna la sposta e ritorna 1, se non vi sono ritorna 0 */
int step_down(struct gioco * gioco){
		int r = gioco ->r, c = gioco->c, flag = 0;

	if (gioco->campo[r][c].posizione == VERTICALE){
		if(r+1 < RIGHE && gioco->campo[r+1][c].tipo == VUOTO){
			flag = 1;
			scambia_celle(&(gioco->campo[r][c]), &(gioco->campo[r+1][c]) );
			scambia_celle(&(gioco->campo[r-1][c]), &(gioco->campo[r][c]) );
			gioco->r +=1;
		}
	}
	else if(gioco->campo[r][c].posizione == ORIZZONTALE){
		if( r+1 < RIGHE && gioco->campo[r+1][c].tipo == VUOTO && gioco->campo[r+1][c+1].tipo == VUOTO ){
			flag = 1;
			scambia_celle(&(gioco->campo[r][c]), &(gioco->campo[r+1][c]) );
			scambia_celle(&(gioco->campo[r][c+1]), &(gioco->campo[r+1][c+1]) );
			gioco->r +=1;
		}
	}
	
	return flag;
}

void down(struct gioco* gioco){
	int r = gioco ->r, c = gioco->c , k = r;

	if (gioco->campo[r][c].posizione == VERTICALE){
		while(k+1<RIGHE && gioco->campo[k+1][c].tipo == VUOTO)
			k++;
		if(k != r){
			scambia_celle(&(gioco->campo[r][c]), &(gioco->campo[k][c]) );
			scambia_celle(&(gioco->campo[r-1][c]), &(gioco->campo[k-1][c]) );
		}
	}
	else if(gioco->campo[r][c].posizione == ORIZZONTALE){
		while(k+1<RIGHE && gioco->campo[k+1][c].tipo == VUOTO && gioco->campo[k+1][c+1].tipo == VUOTO )
			k++;
		if(k != r){
			scambia_celle(&(gioco->campo[r][c]), &(gioco->campo[k][c]) );
			scambia_celle(&(gioco->campo[r][c+1]), &(gioco->campo[k][c+1]) );
			gioco -> r = k;
		}
	}
}

int sx(struct gioco* gioco){
	int flag=0, r = gioco ->r, c = gioco->c;
	if(c-1 >= 0){
		if(gioco->campo[r][c].posizione == ORIZZONTALE && gioco->campo[r][c-1].tipo == VUOTO){
			scambia_celle(&(gioco->campo[r][c]),&(gioco->campo[r][c-1]));
			scambia_celle(&(gioco->campo[r][c]),&(gioco->campo[r][c+1]));
			gioco -> c -= 1;
			flag =1;
		}
		else if(gioco->campo[r][c].posizione == VERTICALE && gioco->campo[r][c-1].tipo == VUOTO && gioco->campo[r-1][c-1].tipo == VUOTO ){
			scambia_celle(&(gioco->campo[r][c]),&(gioco->campo[r][c-1]));
			scambia_celle(&(gioco->campo[r-1][c]),&(gioco->campo[r-1][c-1]));
			gioco -> c -= 1;
			flag = 1;
		}
	}
	return flag;
}

int dx(struct gioco* gioco){
	int flag=0, r = gioco ->r, c = gioco->c;
	if(c+2 < COLONNE && gioco->campo[r][c].posizione == ORIZZONTALE && gioco->campo[r][c+2].tipo == VUOTO){
		scambia_celle(&(gioco->campo[r][c+1]),&(gioco->campo[r][c+2]));
		scambia_celle(&(gioco->campo[r][c]),&(gioco->campo[r][c+1]));
		gioco -> c += 1;
		flag = 1;
	}
	else if( c+1 < COLONNE  && gioco->campo[r][c].posizione == VERTICALE && gioco->campo[r][c+1].tipo == VUOTO && gioco->campo[r-1][c+1].tipo == VUOTO ){
		scambia_celle(&(gioco->campo[r][c]),&(gioco->campo[r][c+1]));
		scambia_celle(&(gioco->campo[r-1][c]),&(gioco->campo[r-1][c+1]));
		gioco -> c += 1;
		flag = 1;
	}
	return flag;
}

int anticlockwise(struct gioco*gioco){
	int flag=0, r = gioco ->r, c = gioco->c;

	if(gioco->campo[r][c].posizione == ORIZZONTALE && r > 0){
		if(r-1 >= 0 && gioco->campo[r-1][c].tipo == VUOTO){
			gioco->campo[r][c].posizione = gioco->campo[r][c+1].posizione = VERTICALE;
			scambia_celle(&(gioco->campo[r-1][c]), &(gioco->campo[r][c+1]));
			flag =1;
		}
	}
	else if(gioco->campo[r][c].posizione == VERTICALE){
		if(c+1 < COLONNE && gioco->campo[r][c+1].tipo == VUOTO){
			gioco->campo[r][c].posizione  = gioco->campo[r-1][c].posizione = ORIZZONTALE;
			scambia_celle(&(gioco->campo[r-1][c]), &(gioco->campo[r][c+1]));
			scambia_celle(&(gioco->campo[r][c]), &(gioco->campo[r][c+1]));
			flag = 1;
		}
		else if (c < COLONNE && (c+1 == COLONNE || (gioco->campo[r][c+1].tipo != VUOTO && c-1 >= 0)) && gioco->campo[r][c-1].tipo == VUOTO ){
			gioco->campo[r][c].posizione  = gioco->campo[r-1][c].posizione = ORIZZONTALE;
			scambia_celle(&(gioco->campo[r-1][c]), &(gioco->campo[r][c-1]));
			gioco->c = c-1;
			flag = 1;
		}
	}
	
	return flag;
}

int clockwise(struct gioco*gioco){
	int flag=0, r = gioco ->r, c = gioco->c;

	if(gioco->campo[r][c].posizione == ORIZZONTALE && r > 0){
		if(r-1 >= 0 && gioco->campo[r-1][c].tipo == VUOTO){
			gioco->campo[r][c+1].posizione = gioco->campo[r][c].posizione = VERTICALE;
			scambia_celle(&(gioco->campo[r][c]), &(gioco->campo[r][c+1]));
			scambia_celle(&(gioco->campo[r-1][c]), &(gioco->campo[r][c+1]));
			flag = 1;
		}

	}
	else if(gioco->campo[r][c].posizione == VERTICALE){
		if(c+1 < COLONNE && gioco->campo[r][c+1].tipo == VUOTO){
			gioco->campo[r][c].posizione = gioco->campo[r-1][c].posizione = ORIZZONTALE;
			scambia_celle(&(gioco->campo[r-1][c]), &(gioco->campo[r][c+1]));
			flag = 1;
		}
		else if (c < COLONNE && (c+1 == COLONNE || (gioco->campo[r][c+1].tipo != VUOTO && c-1 >= 0)) && gioco->campo[r][c-1].tipo == VUOTO ){
			gioco->campo[r][c].posizione  = gioco->campo[r-1][c].posizione = ORIZZONTALE;
			scambia_celle(&(gioco->campo[r-1][c]), &(gioco->campo[r][c]));
			scambia_celle(&(gioco->campo[r-1][c]), &(gioco->campo[r][c-1]));
			gioco->c = c-1;
			flag = 1;
		}
	}
	
	return flag;
}
