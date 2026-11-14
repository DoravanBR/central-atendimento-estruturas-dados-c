/*==================================== INCLUSOES =====================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arvore_decisao.h"
#include "entrada.h"

/*==================================== INCLUSOES =====================================================*/



/*==================================== FUNCOES =====================================================*/

/*=====================================================================================================
 CRIAR PERGUNTA
=====================================================================================================*/

static NoDecisao *criar_pergunta(const char *pergunta){

    NoDecisao *novo;

    if (pergunta == NULL || pergunta[0] == '\0' || strlen(pergunta) >= TAM_PERGUNTA) {
        return NULL;
    }

    novo = malloc(sizeof(*novo));

    if (novo == NULL) {
        return NULL;
    }

    novo->tipo = NO_DECISAO_PERGUNTA;

    strcpy(novo->pergunta, pergunta);

    novo->classificacao = CLASSIFICACAO_NAO_DEFINIDA;

    novo->resposta_sim = NULL;
    novo->resposta_nao = NULL;

    return novo;
}

/*=====================================================================================================
 CRIAR RESULTADO
=====================================================================================================*/

static NoDecisao *criar_resultado(Classificacao classificacao){

    NoDecisao *novo;

    switch (classificacao) {
        case CLASSIFICACAO_COMUM:
        case CLASSIFICACAO_MODERADA:
        case CLASSIFICACAO_URGENTE:
        case CLASSIFICACAO_CRITICA:
            break;

        case CLASSIFICACAO_NAO_DEFINIDA:
        default:
            return NULL;
    }

    novo = malloc(sizeof(*novo));

    if (novo == NULL) {
        return NULL;
    }

    novo->tipo = NO_DECISAO_RESULTADO;
    novo->pergunta[0] = '\0';
    novo->classificacao = classificacao;
    novo->resposta_sim = NULL;
    novo->resposta_nao = NULL;

    return novo;
}

/*=====================================================================================================
 DESTRUIR NO
=====================================================================================================*/

static void destruir_no(NoDecisao *no){

    if (no == NULL) {
        return;
    }

    destruir_no(no->resposta_sim);
    destruir_no(no->resposta_nao);

    free(no);
}

/*=====================================================================================================
 INICIALIZAR
=====================================================================================================*/

int arvore_decisao_inicializar(ArvoreDecisao *arvore){

    NoDecisao *pergunta_sim;
    NoDecisao *pergunta_nao;

    if (arvore == NULL) {
        return 0;
    }

    arvore->raiz = NULL;
    arvore->quantidade_nos = 0UL;

    arvore->raiz = criar_pergunta("O servico esta totalmente indisponivel?");

    if (arvore->raiz == NULL) {
        return 0;
    }

    pergunta_sim = criar_pergunta("O problema afeta varias pessoas?");

    if (pergunta_sim == NULL) {
        destruir_no(arvore->raiz);
        arvore->raiz = NULL;
        return 0;
    }

    arvore->raiz->resposta_sim = pergunta_sim;

    pergunta_nao = criar_pergunta("O problema impede a realizacao do trabalho?");

    if (pergunta_nao == NULL) {
        destruir_no(arvore->raiz);
        arvore->raiz = NULL;
        return 0;
    }

    arvore->raiz->resposta_nao = pergunta_nao;

    pergunta_sim->resposta_sim = criar_resultado(CLASSIFICACAO_CRITICA);

    if (pergunta_sim->resposta_sim == NULL) {
        destruir_no(arvore->raiz);
        arvore->raiz = NULL;
        return 0;
    }

    pergunta_sim->resposta_nao = criar_resultado(CLASSIFICACAO_URGENTE);

    if (pergunta_sim->resposta_nao == NULL) {
        destruir_no(arvore->raiz);
        arvore->raiz = NULL;
        return 0;
    }

    pergunta_nao->resposta_sim = criar_resultado(CLASSIFICACAO_MODERADA);

    if (pergunta_nao->resposta_sim == NULL) {
        destruir_no(arvore->raiz);
        arvore->raiz = NULL;
        return 0;
    }

    pergunta_nao->resposta_nao = criar_resultado(CLASSIFICACAO_COMUM);

    if (pergunta_nao->resposta_nao == NULL) {
        destruir_no(arvore->raiz);
        arvore->raiz = NULL;
        return 0;
    }

    arvore->quantidade_nos = 7UL;

    return 1;
}

/*=====================================================================================================
 CONTAR NOS
=====================================================================================================*/

unsigned long arvore_decisao_contar_nos(const ArvoreDecisao *arvore){

    if (arvore == NULL) {
        return 0UL;
    }
    return arvore->quantidade_nos;
}

/*=====================================================================================================
 CONTAR FOLHAS
=====================================================================================================*/

static unsigned long contar_folhas_no(const NoDecisao *no){

    if (no == NULL) {
        return 0UL;
    }

    if (no->tipo == NO_DECISAO_RESULTADO) {
        return 1UL;
    }

    return contar_folhas_no(no->resposta_sim) + contar_folhas_no(no->resposta_nao);
}

unsigned long arvore_decisao_contar_folhas(const ArvoreDecisao *arvore){
    if (arvore == NULL) {
        return 0UL;
    }

    return contar_folhas_no(
        arvore->raiz
    );
}

/*=====================================================================================================
 OBTER ALTURA
=====================================================================================================*/

static int obter_altura_no(const NoDecisao *no){

    int altura_sim;
    int altura_nao;

    if (no == NULL) {
        return -1;
    }

    altura_sim = obter_altura_no(no->resposta_sim);
    altura_nao = obter_altura_no(no->resposta_nao);

    if (altura_sim > altura_nao) {
        return altura_sim + 1;
    }

    return altura_nao + 1;
}

int arvore_decisao_obter_altura(const ArvoreDecisao *arvore){

    if (arvore == NULL) {
        return -1;
    }

    return obter_altura_no(
        arvore->raiz
    );
}

/*=====================================================================================================
 CLASSIFICACAO
=====================================================================================================*/

int arvore_decisao_classificar(const ArvoreDecisao *arvore, Classificacao *resultado){

    const NoDecisao *atual;
    int resposta;

    if (resultado != NULL) {
        *resultado = CLASSIFICACAO_NAO_DEFINIDA;
    }

    if (arvore == NULL || arvore->raiz == NULL || resultado == NULL) {
        return 0;
    }

    atual = arvore->raiz;

    while (atual != NULL && atual->tipo == NO_DECISAO_PERGUNTA) {
        printf("\n%s\n", atual->pergunta);

        if (!entrada_ler_inteiro("Digite 1 para sim ou 0 para nao: ", 0, 1, &resposta)) {
            return 0;
        }

        if (resposta == 1) {
            atual = atual->resposta_sim;
        } else {
            atual = atual->resposta_nao;
        }
    }

    if (atual == NULL || atual->tipo != NO_DECISAO_RESULTADO) {
        return 0;
    }

    switch (atual->classificacao) {
        case CLASSIFICACAO_COMUM:
        case CLASSIFICACAO_MODERADA:
        case CLASSIFICACAO_URGENTE:
        case CLASSIFICACAO_CRITICA:
            *resultado = atual->classificacao;
            return 1;

        case CLASSIFICACAO_NAO_DEFINIDA:
        default:
            return 0;
    }
}

/*=====================================================================================================
 EXIBIR
=====================================================================================================*/

static void exibir_indentacao(unsigned long nivel){

    unsigned long i;

    for (i = 0UL; i < nivel; i++) {
        printf("  ");
    }
}

static void exibir_no(const NoDecisao *no, unsigned long nivel, const char *caminho){

    if (no == NULL) {
        return;
    }

    exibir_indentacao(nivel);

    if (caminho != NULL) {
        printf("[%s] ", caminho);
    }

    if (no->tipo == NO_DECISAO_RESULTADO) {
        printf("Resultado: %s\n", classificacao_para_texto(no->classificacao));
        return;
    }

    printf("Pergunta: %s\n", no->pergunta);

    exibir_no(no->resposta_sim, nivel + 1UL, "Sim");

    exibir_no( no->resposta_nao, nivel + 1UL, "Nao");
}

void arvore_decisao_exibir(const ArvoreDecisao *arvore){

    if (arvore == NULL || arvore->raiz == NULL) {
        printf("Arvore de decisao vazia.\n");
        return;
    }

    exibir_no(arvore->raiz, 0UL, NULL);
}

/*=====================================================================================================
 DESTRUIR
=====================================================================================================*/

void arvore_decisao_destruir(ArvoreDecisao *arvore){
    
    if (arvore == NULL) {
        return;
    }

    destruir_no(arvore->raiz);

    arvore->raiz = NULL;
    arvore->quantidade_nos = 0UL;
}