/*========================================= INCLUSOES =============================================*/

#include <assert.h>
#include <stdio.h>
#include <string.h>


#include "../atendimento.h"


/*========================================= TESTES =============================================*/
int main(void){

    Atendimento *atendimento;
    char solicitante_grande[TAM_SOLICITANTE + 1];
    char descricao_grande[TAM_DESCRICAO + 1];

    memset(solicitante_grande, 'A', TAM_SOLICITANTE);
    solicitante_grande[TAM_SOLICITANTE] = '\0';
    memset(descricao_grande, 'B', TAM_DESCRICAO);
    descricao_grande[TAM_DESCRICAO] = '\0';

    printf("=============================================================================\n");
    printf("Iniciando testes de atendimento.....\n\n");

/*=================================== VALIDACAO PROPRIEDADE =======================================*/
    printf("Testando Validadacao de prioridades.....\n");
    printf("Testando menor valor permitido: 1 ...\n");
    assert(atendimento_prioridade_valida(1));
    printf("Testando maior valor permitido: 10 ...\n");
    assert(atendimento_prioridade_valida(10));
    printf("Testando valor intermediario permitido: 5 ...\n");
    assert(atendimento_prioridade_valida(5));
    printf("Testando valor menor não permitido: 0 ...\n");
    assert(!atendimento_prioridade_valida(0));
    printf("Testando valor maior não permitido: 11 ...\n");
    assert(!atendimento_prioridade_valida(11));
    printf("Testando valor negativo não permitido: -1 ...\n");
    assert(!atendimento_prioridade_valida(-1));
    printf("=====================================================================\n");
    printf("Testes OK !!\n");
    printf("======================================================================\n\n");

    /*===================== VALIDACAO POSSIBILIDADE DE TRANSICAO DE STATUS =========================*/
    printf("Testando Possibilidade de transicao de Status......\n");
    printf("Transicoes permitidas.......\n");
    printf("ABERTO -> CLASSIFICADO...\n");
    assert(atendimento_pode_mudar_status(STATUS_ABERTO, STATUS_CLASSIFICADO));
    printf("CLASSIFICADO -> EM ANDAMENTO\n");
    assert(atendimento_pode_mudar_status(STATUS_CLASSIFICADO, STATUS_EM_ATENDIMENTO));
    printf("EM ANDAMENTO -> CONCLUIDO\n");
    assert(atendimento_pode_mudar_status(STATUS_EM_ATENDIMENTO, STATUS_CONCLUIDO));
    printf("=============================================================\n");
    printf("Transiscoes NAO permitidas.......\n\n");
    printf("ABERTO -> CONCLUIDO\n");
    assert(!atendimento_pode_mudar_status(STATUS_ABERTO, STATUS_CONCLUIDO));
    printf("CLASSIFICADO -> ABERTO\n");
    assert(!atendimento_pode_mudar_status(STATUS_CLASSIFICADO, STATUS_ABERTO));
    printf("CONCLUIDO -> ABERTO\n");
    assert(!atendimento_pode_mudar_status(STATUS_CONCLUIDO, STATUS_ABERTO));
    printf("CONCLUIDO -> CONCLUIDO\n");
    assert(!atendimento_pode_mudar_status(STATUS_CONCLUIDO, STATUS_CONCLUIDO));
    printf("VALOR INVALIDO -> STATUS VALIDO");
    assert(!atendimento_pode_mudar_status((StatusAtendimento)99, STATUS_ABERTO));
    printf("=====================================================================\n");
    printf("Testes OK !!\n");
    printf("======================================================================\n\n");

/*=============================== VALIDACAO CRIACAO ATENDIMENTO ===================================*/
    printf("Testando criacao de um atendimento....\n");
    printf("Dados de teste:\n");
    printf("Codigo: 1\n");
    printf("Solicitante: Ana Silva\n");
    printf("Descricao: Servidor principal indisponivel\n");
    printf("prioridade: 8\n");
    printf("Ordem de chegada: 1\n");
    printf("===================================================\n");
    atendimento = atendimento_criar(1, "Ana Silva", "Servidor principal indisponivel", 8, 1);
    assert(atendimento != NULL);
    assert(atendimento->codigo == 1);
    assert(atendimento->prioridade == 8);
    assert(atendimento->ordem_chegada == 1);
    assert(strcmp(atendimento->solicitante, "Ana Silva") == 0);
    assert(strcmp(atendimento->descricao, "Servidor principal indisponivel") == 0);
    assert(atendimento->classificacao == CLASSIFICACAO_NAO_DEFINIDA);
    assert(atendimento->status == STATUS_ABERTO);
    printf("=====================================================================\n");
    printf("Testes OK !!\n");
    printf("======================================================================\n\n");

/*============================ VALIDACAO CONFIRMACAO CLASSIFICACAO ================================*/
    printf("Testando confirmacao de classificacao....\n");
    printf("Classificacao INVALIDA...\n");
    assert(!atendimento_confirmar_classificacao(atendimento, CLASSIFICACAO_NAO_DEFINIDA));
    printf("Confirmacao de objeto nao modificado....\n");
    assert(atendimento->classificacao == CLASSIFICACAO_NAO_DEFINIDA);
    assert(atendimento->status == STATUS_ABERTO);
    printf("Classificacao VALIDA....\n");
    assert(atendimento_confirmar_classificacao(atendimento, CLASSIFICACAO_URGENTE));
    printf("Confirmacao de objeto modificado...\n");
    assert(atendimento->classificacao == CLASSIFICACAO_URGENTE);
    assert(atendimento->status == STATUS_CLASSIFICADO);
    printf("Testando Re-Classificacao...\n");
    assert(!atendimento_confirmar_classificacao(atendimento, CLASSIFICACAO_CRITICA));
    printf("Confirmacao de objeto nao modificado....\n");
    assert(atendimento->classificacao == CLASSIFICACAO_URGENTE);
    assert(atendimento->status == STATUS_CLASSIFICADO);
    printf("=====================================================================\n");
    printf("Testes OK !!\n");
    printf("======================================================================\n\n");

 /*============================ VALIDACAO TRANSICAO DE STATUS ================================*/  
    printf("Testando Validacao de Transicao de Status....\n");
    printf("Status atual: Classificado (obtido no teste anterior)");
    printf("CLASSIFICADO -> EM ANDAMENTO\n");
    assert(atendimento_atualizar_status(atendimento, STATUS_EM_ATENDIMENTO));
    printf("Confirmando mudanca...\n");
    assert(atendimento->status == STATUS_EM_ATENDIMENTO);
    printf("Tentando voltar para Status: Aberto...\n");
    assert(!atendimento_atualizar_status(atendimento, STATUS_ABERTO));
    printf("Confirmando que Status nao se alterou...\n");
    assert(atendimento->status == STATUS_EM_ATENDIMENTO);
    printf("EM ANDAMENTO -> CONCLUIDO\n");
    assert(atendimento_atualizar_status(atendimento, STATUS_CONCLUIDO));
    printf("Confirmando alteracao...\n");
    assert(atendimento->status == STATUS_CONCLUIDO);
    printf("Testando alterar Status concluido...\n");
    assert(!atendimento_atualizar_status(atendimento, STATUS_EM_ATENDIMENTO));
    printf("Confirmando que nao houve alteracao\n");
    assert(atendimento->status == STATUS_CONCLUIDO);
    printf("=====================================================================\n");
    printf("Testes OK !!\n");
    printf("======================================================================\n\n");

/*============================ VALIDACAO CONVERSAO DE TEXTO ================================*/
    printf("Testando Conversao de Texto....\n");
    printf("Testando Classificacao para texto....\n");
    assert(strcmp(classificacao_para_texto(CLASSIFICACAO_NAO_DEFINIDA), "nao definida") == 0);
    assert(strcmp(classificacao_para_texto(CLASSIFICACAO_COMUM), "comum") == 0);
    assert(strcmp(classificacao_para_texto(CLASSIFICACAO_MODERADA), "moderada") == 0);
    assert(strcmp(classificacao_para_texto(CLASSIFICACAO_URGENTE), "urgente") == 0);
    assert(strcmp(classificacao_para_texto(CLASSIFICACAO_CRITICA), "critica") == 0);
    assert(strcmp(classificacao_para_texto((Classificacao)99), "desconhecida") == 0);
    printf("Testando Status para texto....\n");
    assert(strcmp(status_para_texto(STATUS_ABERTO), "Aberto") == 0);
    assert(strcmp(status_para_texto(STATUS_CLASSIFICADO), "Classificado") == 0);
    assert(strcmp(status_para_texto(STATUS_EM_ATENDIMENTO), "Em Atendimento") == 0);
    assert(strcmp(status_para_texto(STATUS_CONCLUIDO), "Concluido") == 0);
    assert(strcmp(status_para_texto((StatusAtendimento)99), "Desconhecido") == 0);
    printf("=====================================================================\n");
    printf("Testes OK !!\n");
    printf("======================================================================\n\n");

/*============================ VALIDACAO CRIACAO ATENDIMENTO INVALIDO ================================*/    
    printf("Testando Criacao de atendidmento com campos invalidos....\n");
    printf("Codigo: 0 \n");
    assert(atendimento_criar(0,"Ana","Teste", 5, 1) == NULL);
    printf("Ordem de chegada 0 \n");
    assert(atendimento_criar(1,"Ana","Teste", 5, 0) == NULL);
    printf("Solicitante Nulo \n");
    assert(atendimento_criar(1,NULL,"Teste", 5, 1) == NULL);
    printf("Descricao Nula \n");
    assert(atendimento_criar(1,"Ana", NULL, 5, 1) == NULL);
    printf("Solicitante vazio \n");
    assert(atendimento_criar(1,"","Teste", 5, 1) == NULL);
    printf("Descricao vazio\n");
    assert(atendimento_criar(1,"Ana","", 5, 1) == NULL);
    printf("Prioridade menor\n");
    assert(atendimento_criar(1,"Ana","Teste", 0, 1) == NULL);
    printf("Prioridade maior\n");
    assert(atendimento_criar(1,"Ana","Teste", 11, 1) == NULL);
    printf("Solicitante maior tamanho do vetor\n");
    assert(atendimento_criar(1, solicitante_grande, "Teste", 5, 1) == NULL);
    printf("Descricao maior tamanho do vetor\n");
    assert(atendimento_criar(1, "Ana", descricao_grande, 5, 1) == NULL);
    printf("=====================================================================\n");
    printf("Testes OK !!\n");
    printf("======================================================================\n\n");

/*============================ VALIDACAO REJEICAO DE PONTEIROS NULOS ================================*/
    printf("Testando Rejeicao de ponteiros nulos nas funcoes ....\n");
    printf("Atualizar status\n");
    assert(!atendimento_atualizar_status(NULL, STATUS_CLASSIFICADO));
    printf("Confirmar Classificacao\n");
    assert(!atendimento_confirmar_classificacao(NULL, CLASSIFICACAO_COMUM));
    printf("Exibir atendimento\n");
    atendimento_exibir(NULL);
    printf("=====================================================================\n");
    printf("Testes OK !!\n");
    printf("======================================================================\n\n");

/*============================ VALIDACAO FUNCAO EXIBIR ================================*/
    printf("Testando funcao exibir atendimento....\n");
    atendimento_exibir(atendimento);
    printf("=====================================================================\n");
    printf("Testes OK !!\n");
    printf("======================================================================\n\n");

/*============================ VALIDACAO FUNCAO DESTRUIR ================================*/
    printf("Testando funcao destruir atendimento....\n");
    atendimento_destruir(atendimento);
    atendimento = NULL;
    assert(atendimento == NULL);
    printf("=====================================================================\n");
    printf("Testes OK !!\n");
    printf("======================================================================\n\n");


    printf("Todos os testes passaram!!!\n");
    printf("=============================================================================\n");
    return 0;    
}