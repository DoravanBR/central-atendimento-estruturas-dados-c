/*========================================= INCLUSOES =============================================*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

/*========================================= FUNCOES =============================================*/

/*=====================================================================================================
 INICIALIZACAO
=====================================================================================================*/
void bst_inicializar(ArvoreBST *arvore){
    if (arvore == NULL) {
        return;
    }

    arvore->raiz = NULL;
    arvore->tamanho = 0UL;
}
/*=====================================================================================================
 ARVORE ESTA VAZIA
=====================================================================================================*/
int bst_esta_vazia(const ArvoreBST *arvore){
    return arvore == NULL || arvore->raiz == NULL;
}

/*=====================================================================================================
 OBTER TAMANHO
=====================================================================================================*/
unsigned long bst_obter_tamanho(const ArvoreBST *arvore){
    if (arvore == NULL) {
        return 0UL;
    }

    return arvore->tamanho;
}

/*=====================================================================================================
 INSERIR NO
=====================================================================================================*/
static int inserir_no(NoBST **subarvore, Atendimento *atendimento){
    NoBST *novo;

    if (subarvore == NULL || atendimento == NULL) {
        return 0;
    }

    if (*subarvore == NULL) {
        novo = malloc(sizeof(*novo));

        if (novo == NULL) {
            return 0;
        }

        novo->atendimento = atendimento;
        novo->esquerda = NULL;
        novo->direita = NULL;

        *subarvore = novo;

        return 1;
    }

    if (atendimento->codigo < (*subarvore)->atendimento->codigo) {
        return inserir_no(&(*subarvore)->esquerda, atendimento);
    }

    if (atendimento->codigo > (*subarvore)->atendimento->codigo) {
        return inserir_no(&(*subarvore)->direita, atendimento
        );
    }

    return 0;
}

/*=====================================================================================================
 INSERIR ARVORE
=====================================================================================================*/
int bst_inserir( ArvoreBST *arvore, Atendimento *atendimento){

    if (arvore == NULL || atendimento == NULL || atendimento->codigo == 0UL ||
         arvore->tamanho == ULONG_MAX) {
        return 0;
    }

    if ((arvore->raiz == NULL && arvore->tamanho != 0UL) || (arvore->raiz != NULL &&
         arvore->tamanho == 0UL)) {
        return 0;
    }

    if (!inserir_no(&arvore->raiz, atendimento)) {
        return 0;
    }

    arvore->tamanho++;

    return 1;
}

/*=====================================================================================================
 BUSCA
=====================================================================================================*/
Atendimento *bst_buscar(const ArvoreBST *arvore, unsigned long codigo, unsigned long *comparacoes){

    const NoBST *atual;

    if (comparacoes != NULL) {
        *comparacoes = 0UL;
    }

    if (arvore == NULL || codigo == 0UL) {
        return NULL;
    }

    atual = arvore->raiz;

    while (atual != NULL) {
        if (comparacoes != NULL) {
            (*comparacoes)++;
        }

        if (codigo == atual->atendimento->codigo) {
            return atual->atendimento;
        }

        if (codigo < atual->atendimento->codigo) {
            atual = atual->esquerda;
        } else {
            atual = atual->direita;
        }
    }

    return NULL;
}

/*=====================================================================================================
 OBTER ALTURA NO
=====================================================================================================*/
static int obter_altura_no(const NoBST *no){

    int altura_esquerda;
    int altura_direita;

    if (no == NULL) {
        return -1;
    }

    altura_esquerda = obter_altura_no(no->esquerda);
    altura_direita = obter_altura_no(no->direita);
    if (altura_esquerda > altura_direita) {
        return altura_esquerda + 1;
    }

    return altura_direita + 1;
}

/*=====================================================================================================
 OBTER ALTURA ARVORE
=====================================================================================================*/
int bst_obter_altura(const ArvoreBST *arvore){
    if (arvore == NULL) {
        return -1;
    }

    return obter_altura_no(
        arvore->raiz
    );
}

/*====== EXIBIR ================================================================================*/

/*=====================================================================================================
 PRE-ORDEM
=====================================================================================================*/
static void exibir_no_pre_ordem(const NoBST *no){
    if (no == NULL) {
        return;
    }

    printf("%lu ", no->atendimento->codigo);

    exibir_no_pre_ordem(no->esquerda);
    exibir_no_pre_ordem(no->direita);
}

void bst_exibir_pre_ordem(const ArvoreBST *arvore){

    if (bst_esta_vazia(arvore)) {
        printf("BST vazia.\n");
        return;
    }

    printf("BST em pre-ordem: ");

    exibir_no_pre_ordem(
        arvore->raiz
    );

    printf("\n");
}

/*=====================================================================================================
 EM ORDEM
=====================================================================================================*/
static void exibir_no_em_ordem(const NoBST *no){

    if (no == NULL) {
        return;
    }

    exibir_no_em_ordem(no->esquerda);
    printf( "%lu ", no->atendimento->codigo);
    exibir_no_em_ordem(no->direita);
}

void bst_exibir_em_ordem(const ArvoreBST *arvore){

    if (bst_esta_vazia(arvore)) {
        printf("BST vazia.\n");
        return;
    }

    printf("BST em ordem: ");

    exibir_no_em_ordem(
        arvore->raiz
    );

    printf("\n");
}
/*=====================================================================================================
 POS-ORDEM
=====================================================================================================*/
static void exibir_no_pos_ordem(const NoBST *no){

    if (no == NULL) {
        return;
    }

    exibir_no_pos_ordem(no->esquerda);
    exibir_no_pos_ordem(no->direita);
    printf("%lu ", no->atendimento->codigo);
}

void bst_exibir_pos_ordem(const ArvoreBST *arvore){

    if (bst_esta_vazia(arvore)) {
        printf("BST vazia.\n");
        return;
    }

    printf("BST em pos-ordem: ");

    exibir_no_pos_ordem(
        arvore->raiz
    );

    printf("\n");
}

/*=====================================================================================================
 DESTRUIR
=====================================================================================================*/

static void destruir_no(NoBST *no){

    if (no == NULL) {
        return;
    }

    destruir_no(no->esquerda);
    destruir_no(no->direita);

    free(no);
}

void bst_destruir(ArvoreBST *arvore){

    if (arvore == NULL) {
        return;
    }

    destruir_no(arvore->raiz);

    arvore->raiz = NULL;
    arvore->tamanho = 0UL;
}
