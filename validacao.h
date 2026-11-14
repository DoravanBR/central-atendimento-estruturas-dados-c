#ifndef VALIDACAO_H
#define VALIDACAO_H

/*========================================= INCLUSOES =============================================*/

#include "avl.h"
#include "bst.h"
#include "heap.h"

/*=============================== PROTOTIPOS FUNCOES =========================================*/

int validacao_bst(const ArvoreBST *arvore);
int validacao_avl(const ArvoreAVL *arvore);
int validacao_heap(const HeapMaximo *heap);

#endif