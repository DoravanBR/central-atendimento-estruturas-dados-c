#ifndef BST_H
#define BST_H

/*========================================= INCLUSOES =============================================*/

#include "atendimento.h"

/*======================================== ESTRUTURAS ============================================*/
/*=====================================================================================================
 NO DA BST
=====================================================================================================*/
typedef struct NoBST {
    Atendimento *atendimento;
    struct NoBST *esquerda;
    struct NoBST *direita;
} NoBST;

/*=====================================================================================================
 ARVORE BST
=====================================================================================================*/
typedef struct {
    NoBST *raiz;
    unsigned long tamanho;
} ArvoreBST;

/*==================================== PROTOTIPOS DE FUNCOES ========================================*/
void bst_inicializar(ArvoreBST *arvore);
int bst_esta_vazia(const ArvoreBST *arvore);
unsigned long bst_obter_tamanho(const ArvoreBST *arvore);
int bst_inserir(ArvoreBST *arvore, Atendimento *atendimento);
Atendimento *bst_buscar(const ArvoreBST *arvore, unsigned long codigo, unsigned long * comparacoes);
int bst_obter_altura(const ArvoreBST *arvore);
void bst_exibir_em_ordem(const ArvoreBST *arvore);
void bst_exibir_pre_ordem(const ArvoreBST *arvore);
void bst_exibir_pos_ordem(const ArvoreBST *arvore);
void bst_destruir(ArvoreBST *arvore);



#endif