#ifndef AVL_H
#define AVL_H

/*========================================= INCLUSOES =============================================*/

#include "atendimento.h"

/*======================================== ESTRUTURAS ============================================*/

/*=====================================================================================================
 NO DA AVL
=====================================================================================================*/

typedef struct NoAVL {
    Atendimento *atendimento;
    int altura;
    struct NoAVL *esquerda;
    struct NoAVL *direita;
} NoAVL;

/*=====================================================================================================
 ARVORE AVL
=====================================================================================================*/

typedef struct {
    NoAVL *raiz;
    unsigned long tamanho;
    unsigned long rotacoes;
} ArvoreAVL;

/*==================================== PROTOTIPOS DE FUNCOES ========================================*/

void avl_inicializar(ArvoreAVL *arvore);
int avl_esta_vazia(const ArvoreAVL *arvore);
unsigned long avl_obter_tamanho(const ArvoreAVL *arvore);
unsigned long avl_obter_rotacoes(const ArvoreAVL *arvore);
int avl_inserir(ArvoreAVL *arvore, Atendimento *atendimento);
Atendimento *avl_buscar(const ArvoreAVL *arvore, unsigned long codigo, unsigned long *comparacoes);
int avl_obter_altura(const ArvoreAVL *arvore);
int avl_obter_fator_balanceamento(const NoAVL *no);
void avl_exibir_pre_ordem(const ArvoreAVL *arvore);
void avl_exibir_em_ordem(const ArvoreAVL *arvore);
void avl_exibir_pos_ordem(const ArvoreAVL *arvore);
void avl_destruir(ArvoreAVL *arvore);


#endif