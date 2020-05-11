#ifndef DRMAURO_H
#define DRMAURO_H

#define RIGHE 16
#define COLONNE 8
#define MAX_VIRUS_LINE 2
#define MOSSE_PER_TURNO 3

enum contenuto { VUOTO, MOSTRO, PASTIGLIA };
enum colore { ROSSO, GIALLO, BLU };
enum mossa { NONE, DESTRA, SINISTRA, GIU, ROT_DX, ROT_SX };
enum stato { IN_CORSO, VITTORIA, SCONFITTA };
enum posizione { ORIZZONTALE, VERTICALE, SINGOLO, DELETED };

struct cella {
  enum contenuto tipo;
  enum colore colore;
  /* ---- */
  enum posizione posizione;
  int id;

};

struct gioco {
  struct cella campo[RIGHE][COLONNE];
  int punti;
  /* ---- */
  int active_id;
  int r;
  int c;
};


void carica_campo(struct gioco *gioco, char *percorso);
void riempi_campo(struct gioco *gioco, int difficolta);
void step(struct gioco *gioco, enum mossa comando);
enum stato vittoria(struct gioco *gioco);

#endif
