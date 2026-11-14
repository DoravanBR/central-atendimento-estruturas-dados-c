#ifndef PILHA_H
#define PILHA_H
/*========================================= DEFINICOES =============================================*/

#define TAM_OPERACAO 120

/*========================================= ESTRUTURAS =============================================*/

/*=====================================================================================================
 HISTORICO
=====================================================================================================*/
typedef struct {
    unsigned long sequencia;
    unsigned long codigo_atendimento;
    char operacao[TAM_OPERACAO];
} RegistroHistorico;

/*=====================================================================================================
 NO DA PILHA
=====================================================================================================*/
typedef struct NoHistorico {
    RegistroHistorico registro;
    struct NoHistorico *proximo;
} NoHistorico;

/*=====================================================================================================
 HISTORICO (PILHA)
=====================================================================================================*/
typedef struct {
    NoHistorico *topo;
    unsigned long tamanho;
    unsigned long proxima_sequencia;
} PilhaHistorico;

/*=================================== PROTOTIPO DAS FUNCOES =======================================*/
void pilha_inicializar(PilhaHistorico *pilha);
int pilha_esta_vazia(const PilhaHistorico *pilha);
unsigned long pilha_obter_tamanho(const PilhaHistorico *pilha);
int pilha_empilhar(PilhaHistorico *pilha, unsigned long codigo_atendimento, const char *operacao);
const RegistroHistorico *pilha_consultar_topo(const PilhaHistorico *pilha);
int pilha_desempilhar(PilhaHistorico *pilha, RegistroHistorico *registro_removido);
void pilha_exibir(const PilhaHistorico *pilha);
void pilha_destruir(PilhaHistorico *pilha);

#endif