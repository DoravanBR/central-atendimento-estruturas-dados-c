#ifndef ARVORE_DECISAO_H
#define ARVORE_DECISAO_H

/*==================================== INCLUSOES =====================================================*/

#include "atendimento.h"

/*========================================= DEFINICOES =============================================*/

/*=====================================================================================================
 CONSTANTES
=====================================================================================================*/
#define TAM_PERGUNTA 160

/*=====================================================================================================
 TIPO DECISAO
=====================================================================================================*/
typedef enum {
    NO_DECISAO_PERGUNTA,
    NO_DECISAO_RESULTADO
} TipoNoDecisao;

/*========================================= ESTRUTURAS =============================================*/

/*=====================================================================================================
 NO DECISAO
=====================================================================================================*/
typedef struct NoDecisao {
    TipoNoDecisao tipo;
    char pergunta[TAM_PERGUNTA];
    Classificacao classificacao;
    struct NoDecisao *resposta_sim;
    struct NoDecisao *resposta_nao;
} NoDecisao;

/*=====================================================================================================
 ARVORE DECISAO
=====================================================================================================*/
typedef struct {
    NoDecisao *raiz;
    unsigned long quantidade_nos;
} ArvoreDecisao;

/*==================================== PROTOTIPO DE FUNCOES ========================================*/
int arvore_decisao_inicializar(ArvoreDecisao *arvore);
unsigned long arvore_decisao_contar_nos(const ArvoreDecisao *arvore);
unsigned long arvore_decisao_contar_folhas(const ArvoreDecisao *arvore);
int arvore_decisao_obter_altura(const ArvoreDecisao *arvore);
int arvore_decisao_classificar(const ArvoreDecisao *arvore, Classificacao *resultado);
void arvore_decisao_exibir(const ArvoreDecisao *arvore);
void arvore_decisao_destruir(ArvoreDecisao *arvore);
#endif