#ifndef MOVEMENT_H
#define MOVEMENT_H

void gererate_pill(struct gioco * gioco);
void scambia_celle(struct cella* cell1, struct cella* cell2);
int step_down(struct gioco * gioco);
void down(struct gioco* gioco);
void sx(struct gioco* gioco);
void dx(struct gioco* gioco);
void anticlockwise(struct gioco*gioco);
void clockwise(struct gioco*gioco);

#endif
