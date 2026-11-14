/*========================================= INCLUSOES =============================================*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "avl.h"

/*========================================= FUNCOES =============================================*/

/*=====================================================================================================
 INICIALIZACAO
=====================================================================================================*/

void avl_inicializar( ArvoreAVL *arvore){
    if (arvore == NULL) {
        return;
    }

    arvore->raiz = NULL;
    arvore->tamanho = 0UL;
    arvore->rotacoes = 0UL;
}

/*=====================================================================================================
 ARVORE VAZIA
=====================================================================================================*/

int avl_esta_vazia(const ArvoreAVL *arvore){
    return arvore == NULL || arvore->raiz == NULL;
}

/*=====================================================================================================
 OBTER TAMANHO
=====================================================================================================*/

unsigned long avl_obter_tamanho(const ArvoreAVL *arvore){
    if (arvore == NULL) {
        return 0UL;
    }

    return arvore->tamanho;
}

/*=====================================================================================================
 OBTER ALTURA 
=====================================================================================================*/
static int obter_altura_no(const NoAVL *no){
    if (no == NULL) {
        return -1;
    }
    return no->altura;
}

int avl_obter_altura(const ArvoreAVL *arvore){
    if (arvore == NULL) {
        return -1;
    }
    return obter_altura_no(arvore->raiz);
}

/*=====================================================================================================
 MAIOR ALTURA
=====================================================================================================*/

static int maior_altura(int valor1, int valor2){
    if (valor1 > valor2) {
        return valor1;
    }

    return valor2;
}

/*=====================================================================================================
 ATUALIZAR ALTURA
=====================================================================================================*/

static void atualizar_altura(NoAVL *no){
    if (no == NULL) {
        return;
    }
    no->altura = 1 + maior_altura(obter_altura_no(no->esquerda), obter_altura_no(no->direita));

}

/*=====================================================================================================
 FATOR DE BALANCEAMENTO
=====================================================================================================*/

int avl_obter_fator_balanceamento(const NoAVL *no){
    if (no == NULL) {
        return 0;
    }

    return obter_altura_no(no->esquerda) - obter_altura_no(no->direita);

}

/*=====================================================================================================
 ROTACAO DIREITA
=====================================================================================================*/

static NoAVL *rotacionar_direita(NoAVL *raiz_antiga, unsigned long *rotacoes){

    NoAVL *nova_raiz;
    NoAVL *subarvore_transferida;

    nova_raiz = raiz_antiga->esquerda;
    subarvore_transferida = nova_raiz->direita;

    nova_raiz->direita = raiz_antiga;
    raiz_antiga->esquerda = subarvore_transferida;

    atualizar_altura(raiz_antiga);
    atualizar_altura(nova_raiz);

    (*rotacoes)++;

    return nova_raiz;
}

/*=====================================================================================================
 ROTACAO ESQUERDA
=====================================================================================================*/

static NoAVL *rotacionar_esquerda(NoAVL *raiz_antiga, unsigned long *rotacoes){

    NoAVL *nova_raiz;
    NoAVL *subarvore_transferida;

    nova_raiz = raiz_antiga->direita;
    subarvore_transferida = nova_raiz->esquerda;

    nova_raiz->esquerda = raiz_antiga;
    raiz_antiga->direita = subarvore_transferida;

    atualizar_altura(raiz_antiga);
    atualizar_altura(nova_raiz);

    (*rotacoes)++;

    return nova_raiz;
}

/*=====================================================================================================
 CRIAR NO
=====================================================================================================*/

static NoAVL *criar_no(Atendimento *atendimento){

    NoAVL *novo;

    novo = malloc(sizeof(*novo));

    if (novo == NULL) {
        return NULL;
    }

    novo->atendimento = atendimento;
    novo->altura = 0;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

/*=====================================================================================================
 REBALANCEAR
=====================================================================================================*/

static NoAVL *rebalancear(NoAVL *no, unsigned long *rotacoes){

    int fator;

    atualizar_altura(no);

    fator = avl_obter_fator_balanceamento(no);

    if (fator > 1) {
        if (avl_obter_fator_balanceamento(no->esquerda) < 0) {
            no->esquerda = rotacionar_esquerda(no->esquerda, rotacoes);
        }
        return rotacionar_direita(no, rotacoes);
    }

    if (fator < -1) {
        if (avl_obter_fator_balanceamento(no->direita) > 0) {
            no->direita = rotacionar_direita(no->direita, rotacoes);
        }
        return rotacionar_esquerda(no, rotacoes);
    }

    return no;
}

/*=====================================================================================================
 INSERIR NO
=====================================================================================================*/

static NoAVL *inserir_no(NoAVL *no, Atendimento *atendimento, int *resultado, unsigned long *rotacoes){

    if (no == NULL) {

        NoAVL *novo;

        novo = criar_no(atendimento);

        if (novo == NULL) {
            *resultado = -1;
            return NULL;
        }

        *resultado = 1;
        return novo;
    }

    if (atendimento->codigo < no->atendimento->codigo) {
        no->esquerda = inserir_no(no->esquerda, atendimento, resultado, rotacoes);
    } else if (atendimento->codigo > no->atendimento->codigo) {
        no->direita = inserir_no(no->direita, atendimento, resultado, rotacoes);
    } else {
        *resultado = 0;
        return no;
    }

    if (*resultado != 1) {
        return no;
    }

    return rebalancear(no, rotacoes);

}

/*=====================================================================================================
 INSERIR ARVORE AVL
=====================================================================================================*/

int avl_inserir(ArvoreAVL *arvore, Atendimento *atendimento){

    int resultado;

    if (arvore == NULL || atendimento == NULL || atendimento->codigo == 0UL ||
        arvore->tamanho == ULONG_MAX || arvore->rotacoes > ULONG_MAX - 2UL) {
        return 0;
    }

    if ((arvore->raiz == NULL && arvore->tamanho != 0UL) || (arvore->raiz != NULL &&
         arvore->tamanho == 0UL)) {
        return 0;
    }

    resultado = 0;

    arvore->raiz = inserir_no(arvore->raiz, atendimento, &resultado, &arvore->rotacoes);
    if (resultado != 1) {
        return 0;
    }

    arvore->tamanho++;

    return 1;
}

/*=====================================================================================================
 OBTER ROTACOES
=====================================================================================================*/

unsigned long avl_obter_rotacoes(const ArvoreAVL *arvore){
    if (arvore == NULL) {
        return 0UL;
    }
    return arvore->rotacoes;
}

/*=====================================================================================================
 BUSCAR
=====================================================================================================*/

Atendimento *avl_buscar(const ArvoreAVL *arvore, unsigned long codigo, unsigned long *comparacoes){

    const NoAVL *atual;

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

/*====== EXIBIR ================================================================================*/

/*=====================================================================================================
 PRE-ORDEM
=====================================================================================================*/

static void exibir_no_pre_ordem(const NoAVL *no){

    if (no == NULL) {
        return;
    }

    printf("%lu(h=%d,fb=%d) ", no->atendimento->codigo, no->altura, avl_obter_fator_balanceamento(no));
    exibir_no_pre_ordem(no->esquerda);
    exibir_no_pre_ordem(no->direita);
}

void avl_exibir_pre_ordem(const ArvoreAVL *arvore){
    if (avl_esta_vazia(arvore)) {
        printf("AVL vazia.\n");
        return;
    }

    printf("AVL em pre-ordem: ");

    exibir_no_pre_ordem(
        arvore->raiz
    );

    printf("\n");
}

/*=====================================================================================================
 EM ORDEM
=====================================================================================================*/

static void exibir_no_em_ordem(const NoAVL *no){

    if (no == NULL) {
        return;
    }

    exibir_no_em_ordem(no->esquerda);
    printf("%lu(h=%d,fb=%d) ", no->atendimento->codigo, no->altura, avl_obter_fator_balanceamento(no));
    exibir_no_em_ordem(no->direita);
}

void avl_exibir_em_ordem(const ArvoreAVL *arvore){

    if (avl_esta_vazia(arvore)) {
        printf("AVL vazia.\n");
        return;
    }

    printf("AVL em ordem: ");

    exibir_no_em_ordem(
        arvore->raiz
    );

    printf("\n");
}

/*=====================================================================================================
 POS-ORDEM
=====================================================================================================*/

static void exibir_no_pos_ordem(const NoAVL *no){

    if (no == NULL) {
        return;
    }

    exibir_no_pos_ordem(no->esquerda);
    exibir_no_pos_ordem(no->direita);
    printf("%lu(h=%d,fb=%d) ", no->atendimento->codigo, no->altura, avl_obter_fator_balanceamento(no));
}

void avl_exibir_pos_ordem(const ArvoreAVL *arvore){

    if (avl_esta_vazia(arvore)) {
        printf("AVL vazia.\n");
        return;
    }

    printf("AVL em pos-ordem: ");

    exibir_no_pos_ordem(
        arvore->raiz
    );

    printf("\n");
}

/*=====================================================================================================
 DESTRUIR 
=====================================================================================================*/

static void destruir_no(NoAVL *no){
    if (no == NULL) {
        return;
    }

    destruir_no(no->esquerda);
    destruir_no(no->direita);

    free(no);
}

void avl_destruir(ArvoreAVL *arvore){

    if (arvore == NULL) {
        return;
    }

    destruir_no(arvore->raiz);

    arvore->raiz = NULL;
    arvore->tamanho = 0UL;
    arvore->rotacoes = 0UL;
}






