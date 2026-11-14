#ifndef HEAP_H
#define HEAP_H

/*========================================= INCLUSOES =============================================*/

#include "atendimento.h"

/*========================================= ESTRUTURAS =============================================*/

typedef struct {
    Atendimento **itens;
    unsigned long quantidade;
    unsigned long capacidade;
} HeapMaximo;

/*=================================== PROTOTIPO DE FUNCOES =======================================*/

int heap_inicializar(HeapMaximo *heap);
int heap_esta_vazio(const HeapMaximo *heap);
unsigned long heap_obter_tamanho(const HeapMaximo *heap);
int heap_inserir(HeapMaximo *heap, Atendimento *atendimento);
Atendimento *heap_consultar_raiz(const HeapMaximo *heap);
Atendimento *heap_remover_raiz(HeapMaximo *heap);
void heap_exibir(const HeapMaximo *heap);
void heap_destruir(HeapMaximo *heap);






#endif