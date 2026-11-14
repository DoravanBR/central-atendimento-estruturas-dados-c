/*========================================= INCLUSOES =============================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atendimento.h"

/*========================================= FUNCOES =============================================*/

/*=====================================================================================================
 COPIA TEXTOS
=====================================================================================================*/
static void copiar_texto(char *destino, size_t capacidade, const char *origem) {
    if (destino == NULL || capacidade == 0) {
        return;
    }

    if (origem == NULL) {
        destino[0] = '\0';
        return;
    }

    (void)snprintf(destino, capacidade, "%s", origem);
}
/*=====================================================================================================
 VALIDACAO PRIORIDADE
=====================================================================================================*/
int atendimento_prioridade_valida(int prioridade){
    return prioridade >= 1 && prioridade <= 10;
}
/*=====================================================================================================
 VALIDACAO TRANSICAO DE STATUS
=====================================================================================================*/
int atendimento_pode_mudar_status(StatusAtendimento status_atual, StatusAtendimento novo_status){
    switch (status_atual) {
        case STATUS_ABERTO:
            return novo_status == STATUS_CLASSIFICADO;

        case STATUS_CLASSIFICADO:
            return novo_status == STATUS_EM_ATENDIMENTO;

        case STATUS_EM_ATENDIMENTO:
            return novo_status == STATUS_CONCLUIDO;

        case STATUS_CONCLUIDO:
        default:
            return 0;
    }
}
/*=====================================================================================================
 ALTERACAO DE STATUS
=====================================================================================================*/
int atendimento_atualizar_status(Atendimento *atendimento, StatusAtendimento novo_status){
    if(atendimento == NULL){
        return 0;
    }
    if(!atendimento_pode_mudar_status(atendimento->status, novo_status)){
        return 0;
    }
     atendimento->status = novo_status;
     return 1;
}
/*=====================================================================================================
 CONFIRMA CLASSIFICACAO
=====================================================================================================*/
int atendimento_confirmar_classificacao(Atendimento *atendimento, Classificacao classificacao){
    if(atendimento == NULL){
        return 0;
    }
    if(atendimento->classificacao != CLASSIFICACAO_NAO_DEFINIDA){
        return 0;
    }
    if(!atendimento_pode_mudar_status(atendimento->status, STATUS_CLASSIFICADO)){
        return 0;
    }

    switch (classificacao)
    {
    case CLASSIFICACAO_COMUM:
    case CLASSIFICACAO_MODERADA:
    case CLASSIFICACAO_URGENTE:
    case CLASSIFICACAO_CRITICA:    
        break;
    case CLASSIFICACAO_NAO_DEFINIDA:
    default:
        return 0;
    }

    atendimento->classificacao = classificacao;
    atendimento->status = STATUS_CLASSIFICADO;
    return 1;
}
/*=====================================================================================================
 CLASSIFICACAO PARA TEXTO
=====================================================================================================*/
const char *classificacao_para_texto(Classificacao classificacao){
     switch (classificacao) {
        case CLASSIFICACAO_NAO_DEFINIDA:
            return "nao definida";

        case CLASSIFICACAO_COMUM:
            return "comum";

        case CLASSIFICACAO_MODERADA:
            return "moderada";

        case CLASSIFICACAO_URGENTE:
            return "urgente";

        case CLASSIFICACAO_CRITICA:
            return "critica";

        default:
            return "desconhecida";
    }
}
/*=====================================================================================================
 STATUS PARA TEXTO
=====================================================================================================*/
const char *status_para_texto(StatusAtendimento status){
    switch (status)
    {
    case STATUS_ABERTO:
        return "Aberto";
    case STATUS_CLASSIFICADO:
        return "Classificado";
    case STATUS_EM_ATENDIMENTO:
        return "Em Atendimento";
    case STATUS_CONCLUIDO:
        return "Concluido";
    default:
        return "Desconhecido";
    }
}
/*=====================================================================================================
 CRIAR ATENDIMENTO
=====================================================================================================*/
Atendimento *atendimento_criar(unsigned long codigo, const char *solicitante,
     const char *descricao, int prioridade, unsigned long ordem_chegada){
        Atendimento *novo;

        if (codigo == 0 || ordem_chegada == 0 || solicitante == NULL || descricao == NULL || 
            solicitante[0] == '\0' || descricao[0] == '\0' || strlen(solicitante) >= TAM_SOLICITANTE ||
            strlen(descricao) >= TAM_DESCRICAO || !atendimento_prioridade_valida(prioridade)) {
                return NULL;
        }

        novo = malloc(sizeof *novo);

        if (novo == NULL) {
            return NULL;
        }

        novo->codigo = codigo;
        novo->prioridade = prioridade;
        novo->ordem_chegada = ordem_chegada;

        novo->classificacao = CLASSIFICACAO_NAO_DEFINIDA;

        novo->status = STATUS_ABERTO;

        copiar_texto(novo->solicitante, sizeof novo->solicitante, solicitante);

        copiar_texto(novo->descricao, sizeof novo->descricao, descricao);

        return novo;
}
/*=====================================================================================================
 EXIBIR ATENDIMENTO
=====================================================================================================*/
void atendimento_exibir(const Atendimento *atendimento){
    if(atendimento == NULL){
        printf("Atendimento inexistente!\n");
        return;
    }
    printf("Codigo: %lu\n", atendimento->codigo);
    printf("Solicitante: %s\n", atendimento->solicitante);
    printf("Descricao: %s\n", atendimento->descricao);
    printf("Prioridade: %d\n", atendimento->prioridade);
    printf("Classificacao: %s\n", classificacao_para_texto(atendimento->classificacao));
    printf("Status: %s\n", status_para_texto(atendimento->status));
    printf("Ordem de chegada: %lu\n", atendimento->ordem_chegada); 
}
/*=====================================================================================================
 DESTRUIR ATENDIMENTO
=====================================================================================================*/
void atendimento_destruir(Atendimento *atendimento){
    free(atendimento);
}