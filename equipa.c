#include "equipa.h"

equipa	criar_equipa(char *nome, int tipo)
{
	equipa	new_time;
	new_time->nome = nome;
	if(tipo == 1)
		new_time->tipo = PUBLICA;
	else
		new_time->tipo = PRIVADA;
	new_time->membros = NULL;
	new_time->prox = NULL;
	new_time->totalMembros = 0;
}
