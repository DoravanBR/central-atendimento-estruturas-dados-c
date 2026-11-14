/*========================================= INCLUSOES =============================================*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

/*========================================= DEFINICOES =============================================*/

#define CAPACIDADE_INICIAL 8UL

/*========================================= FUNCOES =============================================*/

/*=====================================================================================================
 PRECEDENCIA
=====================================================================================================*/

static int tem_precedencia(const Atendimento *atendimento1,const Atendimento *atendimento2){
    if (atendimento1 == NULL || atendimento2 == NULL) {
        return 0;
    }

    if (atendimento1->prioridade != atendimento2->prioridade) {
        return atendimento1->prioridade > atendimento2->prioridade;
    }

    return atendimento1->ordem_chegada < atendimento2->ordem_chegada;
}

/*=====================================================================================================
 TROCAR PONTEIROS
=====================================================================================================*/

static void trocar_atendimentos(Atendimento **atendimento1, Atendimento **atendimento2){
    Atendimento *temporario;

    temporario = *atendimento1;
    *atendimento1 = *atendimento2;
    *atendimento2 = temporario;
}

/*=====================================================================================================
 INICIALIZAR
=====================================================================================================*/

int heap_inicializar(HeapMaximo *heap)
{
    Atendimento **itens;

    if (heap == NULL) {
        return 0;
    }

    heap->itens = NULL;
    heap->quantidade = 0UL;
    heap->capacidade = 0UL;

    itens = malloc(CAPACIDADE_INICIAL * sizeof(*itens));

    if (itens == NULL) {
        return 0;
    }

    heap->itens = itens;
    heap->capacidade = CAPACIDADE_INICIAL;

    return 1;
}

/*=====================================================================================================
 HEAP VAZIO
=====================================================================================================*/

int heap_esta_vazio(const HeapMaximo *heap){
    return heap == NULL || heap->quantidade == 0UL;
}

/*=====================================================================================================
 OBTER TAMANHO
=====================================================================================================*/

unsigned long heap_obter_tamanho(const HeapMaximo *heap){

    if (heap == NULL) {
        return 0UL;
    }
    return heap->quantidade;
}

/*=====================================================================================================
 AUMENTAR CAPACIDADE HEAP
=====================================================================================================*/

static int aumentar_capacidade(HeapMaximo *heap){

    unsigned long nova_capacidade;
    Atendimento **novos_itens;

    if (heap == NULL || heap->itens == NULL || heap->capacidade == 0UL) {
        return 0;
    }

    if (heap->capacidade > ULONG_MAX / 2UL) {
        return 0;
    }

    nova_capacidade = heap->capacidade * 2UL;

    novos_itens = realloc(heap->itens,
        nova_capacidade * sizeof(*novos_itens));

    if (novos_itens == NULL) {
        return 0;
    }

    heap->itens = novos_itens;
    heap->capacidade = nova_capacidade;

    return 1;
}

/*=====================================================================================================
 SUBIR
=====================================================================================================*/

static void subir(HeapMaximo *heap, unsigned long indice){

    unsigned long indice_pai;

    while (indice > 0UL) {
        indice_pai = (indice - 1UL) / 2UL;
        if (!tem_precedencia(heap->itens[indice], heap->itens[indice_pai])) {
            break;
        }

        trocar_atendimentos(&heap->itens[indice],  &heap->itens[indice_pai]
        );

        indice = indice_pai;
    }
}

/*=====================================================================================================
 INSERIR
=====================================================================================================*/

int heap_inserir(HeapMaximo *heap, Atendimento *atendimento){
    unsigned long indice;

    if (heap == NULL || heap->itens == NULL || atendimento == NULL || atendimento->codigo == 0UL ||
        !atendimento_prioridade_valida(atendimento->prioridade) || atendimento->status !=
         STATUS_CLASSIFICADO || (atendimento->classificacao != CLASSIFICACAO_URGENTE &&
             atendimento->classificacao != CLASSIFICACAO_CRITICA) || heap->quantidade >
              heap->capacidade || heap->quantidade == ULONG_MAX) {
        return 0;
    }

    if (heap->quantidade == heap->capacidade) {
        if (!aumentar_capacidade(heap)) {
            return 0;
        }
    }

    indice = heap->quantidade;

    heap->itens[indice] = atendimento;
    heap->quantidade++;

    subir(heap, indice);

    return 1;
}

/*=====================================================================================================
 CONSULTAR RAIZ
=====================================================================================================*/

Atendimento *heap_consultar_raiz(const HeapMaximo *heap){
    if (heap == NULL || heap->itens == NULL || heap->quantidade == 0UL || heap->quantidade >
         heap->capacidade) {
        return NULL;
    }

    return heap->itens[0];
}

/*=====================================================================================================
 DESCER
=====================================================================================================*/

static void descer(HeapMaximo *heap, unsigned long indice){

    unsigned long indice_esquerdo;
    unsigned long indice_direito;
    unsigned long indice_precedente;

    while (1) {
        indice_esquerdo = 2UL * indice + 1UL;
        indice_direito = 2UL * indice + 2UL;
        indice_precedente = indice;

        if (indice_esquerdo < heap->quantidade && tem_precedencia(heap->itens[indice_esquerdo],
             heap->itens[indice_precedente])) {
            indice_precedente = indice_esquerdo;
        }

        if (indice_direito < heap->quantidade && tem_precedencia(heap->itens[indice_direito],
             heap->itens[indice_precedente])) {
            indice_precedente = indice_direito;
        }

        if (indice_precedente == indice) {
            break;
        }

        trocar_atendimentos(&heap->itens[indice], &heap->itens[indice_precedente]);

        indice = indice_precedente;
    }
}

/*=====================================================================================================
 REMOVER RAIZ
=====================================================================================================*/

Atendimento *heap_remover_raiz(HeapMaximo *heap){

    Atendimento *removido;

    if (heap == NULL || heap->itens == NULL || heap->quantidade == 0UL || heap->quantidade >
         heap->capacidade) {
        return NULL;
    }

    removido = heap->itens[0];

    heap->quantidade--;

    if (heap->quantidade == 0UL) {
        heap->itens[0] = NULL;
        return removido;
    }

    heap->itens[0] = heap->itens[heap->quantidade];

    heap->itens[heap->quantidade] = NULL;

    descer(heap, 0UL);

    return removido;
}

/*=====================================================================================================
 EXIBIR
=====================================================================================================*/

void heap_exibir(const HeapMaximo *heap){

    unsigned long i;
    const Atendimento *atendimento;

    if (heap == NULL || heap->itens == NULL || heap->quantidade == 0UL) {
        printf("Heap de prioridades vazio.\n");
        return;
    }

    if (heap->quantidade > heap->capacidade) {
        printf("Heap de prioridades invalido.\n");
        return;
    }

    printf("Quantidade no heap: %lu\n", heap->quantidade);

    for (i = 0UL; i < heap->quantidade; i++) {
        atendimento = heap->itens[i];
        printf("Indice %lu: codigo= %lu, prioridade= %d, ordem= %lu, classificacao= %s\n", i,
             atendimento->codigo, atendimento->prioridade, atendimento->ordem_chegada,
              classificacao_para_texto(atendimento->classificacao)
        );
    }
}

/*=====================================================================================================
 DESTRUIR
=====================================================================================================*/

void heap_destruir(HeapMaximo *heap){
    if (heap == NULL) {
        return;
    }

    free(heap->itens);

    heap->itens = NULL;
    heap->quantidade = 0UL;
    heap->capacidade = 0UL;
}

