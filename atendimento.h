#ifndef ATENDIMENTO_H
#define ATENDIMENTO_H
/*========================================= DEFINICOES =============================================*/

/*=====================================================================================================
 CONSTANTES
=====================================================================================================*/
#define TAM_SOLICITANTE 80
#define TAM_DESCRICAO 160
/*=====================================================================================================
 CLASSIFICACAO
=====================================================================================================*/
typedef enum{
    CLASSIFICACAO_NAO_DEFINIDA,
    CLASSIFICACAO_COMUM,
    CLASSIFICACAO_MODERADA,
    CLASSIFICACAO_URGENTE,
    CLASSIFICACAO_CRITICA
} Classificacao;
/*=====================================================================================================
 STATUS
=====================================================================================================*/
typedef enum{
    STATUS_ABERTO,
    STATUS_CLASSIFICADO,
    STATUS_EM_ATENDIMENTO,
    STATUS_CONCLUIDO
} StatusAtendimento;

/*==================================== ESTRUTURA =====================================================*/

/*=====================================================================================================
 ATENDIMENTO
=====================================================================================================*/
typedef struct {
    unsigned long codigo;

    char solicitante[TAM_SOLICITANTE];
    char descricao[TAM_DESCRICAO];

    int prioridade;
    Classificacao classificacao;
    StatusAtendimento status;

    unsigned long ordem_chegada;
} Atendimento;

/*================================ PROTOTIPOS DE FUNCOES ===========================================*/

Atendimento *atendimento_criar(unsigned long codigo, const char *solicitante,
     const char *descricao, int prioridade, unsigned long ordem_chegada);

int atendimento_prioridade_valida(int prioridade);
int atendimento_pode_mudar_status(StatusAtendimento status_atual, StatusAtendimento novo_status);
int atendimento_atualizar_status(Atendimento *atendimento, StatusAtendimento novo_status);
int atendimento_confirmar_classificacao(Atendimento *atendimento, Classificacao classificacao);
const char *classificacao_para_texto(Classificacao classificacao);
const char *status_para_texto(StatusAtendimento status);
void atendimento_exibir(const Atendimento *atendimento);
void atendimento_destruir(Atendimento *atendimento);

#endif
