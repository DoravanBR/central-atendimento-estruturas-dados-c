/*========================================= INCLUSOES =============================================*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "validacao.h"

/*========================================= FUNCOES =============================================*/

/*=====================================================================================================
 VALIDAR BST
=====================================================================================================*/
static int validar_no_bst(const NoBST *no, int possui_minimo, unsigned long minimo, int possui_maximo, 
    unsigned long maximo, unsigned long *quantidade){

    unsigned long codigo;

    if (no == NULL) {
        return 1;
    }

    if (no->atendimento == NULL) {
        return 0;
    }

    codigo = no->atendimento->codigo;

    if (codigo == 0UL) {
        return 0;
    }

    if (possui_minimo && codigo <= minimo) {
        return 0;
    }

    if (possui_maximo && codigo >= maximo) {
        return 0;
    }

    (*quantidade)++;

    if (!validar_no_bst(no->esquerda, possui_minimo, minimo, 1, codigo, quantidade)) {
        return 0;
    }

    return validar_no_bst(no->direita, 1, codigo, possui_maximo, maximo, quantidade);
}

int validacao_bst(const ArvoreBST *arvore){

    unsigned long quantidade;

    if (arvore == NULL) {
        return 0;
    }

    quantidade = 0UL;

    if (!validar_no_bst(arvore->raiz, 0, 0UL, 0, 0UL, &quantidade)) {
        return 0;
    }

    return quantidade == arvore->tamanho;
}

/*=====================================================================================================
 VALIDAR AVL
=====================================================================================================*/

static int validar_no_avl(const NoAVL *no, int possui_minimo, unsigned long minimo, int possui_maximo, 
    unsigned long maximo, unsigned long *quantidade, int *altura_calculada){

    unsigned long codigo;
    int altura_esquerda;
    int altura_direita;
    int altura_esperada;
    int fator;

    if (no == NULL) {
        *altura_calculada = -1;
        return 1;
    }

    if (no->atendimento == NULL) {
        return 0;
    }

    codigo = no->atendimento->codigo;

    if (codigo == 0UL) {
        return 0;
    }

    if (possui_minimo && codigo <= minimo) {
        return 0;
    }

    if (possui_maximo && codigo >= maximo) {
        return 0;
    }

    if (!validar_no_avl(no->esquerda, possui_minimo, minimo, 1, codigo, quantidade, &altura_esquerda)) {
        return 0;
    }

    if (!validar_no_avl( no->direita, 1, codigo, possui_maximo, maximo, quantidade, &altura_direita)) {
        return 0;
    }

    altura_esperada = 1 + (altura_esquerda > altura_direita ? altura_esquerda : altura_direita);

    if (no->altura != altura_esperada) {
        return 0;
    }

    fator = altura_esquerda - altura_direita;

    if (fator < -1 || fator > 1) {
        return 0;
    }

    (*quantidade)++;
    *altura_calculada = altura_esperada;

    return 1;
}

int validacao_avl(const ArvoreAVL *arvore){

    unsigned long quantidade;
    int altura_calculada;

    if (arvore == NULL) {
        return 0;
    }

    quantidade = 0UL;
    altura_calculada = -1;

    if (!validar_no_avl(arvore->raiz, 0, 0UL, 0, 0UL, &quantidade, &altura_calculada)) {
        return 0;
    }

    if (quantidade != arvore->tamanho) {
        return 0;
    }

    return altura_calculada == avl_obter_altura(arvore);
}

/*=====================================================================================================
 VALIDAR HEAP
=====================================================================================================*/

static int tem_precedencia_heap(const Atendimento *atendimento1, const Atendimento *atendimento2){

    if (atendimento1 == NULL || atendimento2 == NULL) {
        return 0;
    }

    if (atendimento1->prioridade != atendimento2->prioridade) {
        return atendimento1->prioridade > atendimento2->prioridade;
    }

    return atendimento1->ordem_chegada <  atendimento2->ordem_chegada;
}

int validacao_heap(const HeapMaximo *heap){

    unsigned long i;
    unsigned long indice_pai;
    const Atendimento *atendimento;

    if (heap == NULL) {
        return 0;
    }

    /*
      Estado destruído ou ainda sem alocação:
      só é coerente quando todos os metadados são zero.
    */
    if (heap->itens == NULL) {
        return heap->quantidade == 0UL && heap->capacidade == 0UL;
    }

    if (heap->capacidade == 0UL || heap->quantidade > heap->capacidade) {
        return 0;
    }

    for (i = 0UL; i < heap->quantidade; i++) {
        atendimento = heap->itens[i];
        // Toda posição ocupada precisa apontar para um objeto válido. 
        if (atendimento == NULL || atendimento->codigo == 0UL || atendimento->ordem_chegada == 0UL || 
            !atendimento_prioridade_valida(atendimento->prioridade)) {
            return 0;
        }

        // O heap aceita somente urgentes e críticos classificados. 
        if (atendimento->status != STATUS_CLASSIFICADO || (atendimento->classificacao != 
            CLASSIFICACAO_URGENTE && atendimento->classificacao != CLASSIFICACAO_CRITICA)) {
            return 0;
        }

        // A raiz não possui pai. 
        if (i == 0UL) {
            continue;
        }

        indice_pai = (i - 1UL) / 2UL;

        /*
          Se o filho vence o pai pela regra de prioridade/desempate, o heap está inválido.
        */
        if (tem_precedencia_heap(atendimento, heap->itens[indice_pai])) {
            return 0;
        }
    }

    return 1;
}


