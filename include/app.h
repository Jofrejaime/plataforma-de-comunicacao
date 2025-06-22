#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include "hash.h"

bool login(HashTable* ht, char* email, char* senha);
bool registrar(HashTable* ht, const char* email, const char* senha, TipoMembro tipo);

#endif
