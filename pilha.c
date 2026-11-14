/*========================================= INCLUSOES =============================================*/
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pilha.h"

/*========================================= FUNCOES =============================================*/

/*=====================================================================================================
 INICIALIZAR PILHA
=====================================================================================================*/
void pilha_inicializar(PilhaHistorico *pilha){
    if(pilha == NULL){
        return;
    }

    pilha->topo = NULL;
    pilha->tamanho = 0UL;
    pilha->proxima_sequencia = 1UL;

}

/*=====================================================================================================
 VERIFICAR PILHA VAZIA
=====================================================================================================*/
int pilha_esta_vazia(const PilhaHistorico *pilha){
 return pilha == NULL || pilha->topo == NULL;
}

/*=====================================================================================================
 OBTER TAMANHO
=====================================================================================================*/
unsigned long pilha_obter_tamanho(const PilhaHistorico *pilha){
    if(pilha == NULL){
        return 0UL;
    }
    return pilha->tamanho;
}

/*=====================================================================================================
 EMPILHAR
=====================================================================================================*/
int pilha_empilhar(PilhaHistorico *pilha, unsigned long codigo_atendimento, const char *operacao){
    NoHistorico *novo;

    if(pilha == NULL || codigo_atendimento == 0UL || operacao == NULL || operacao[0] == '\0' || 
        strlen(operacao)>= TAM_OPERACAO || pilha->tamanho == ULONG_MAX || pilha->proxima_sequencia == 0UL
         || pilha->proxima_sequencia == ULONG_MAX){
        return 0;
    }
    novo = malloc(sizeof(*novo));

    if (novo == NULL) {
        return 0;
    }

    novo->registro.sequencia = pilha->proxima_sequencia;
    novo->registro.codigo_atendimento = codigo_atendimento;
    strcpy(novo->registro.operacao, operacao);
    novo->proximo = pilha->topo;
    pilha->topo = novo;

    pilha->tamanho++;
    pilha->proxima_sequencia++;

    return 1;

}

/*=====================================================================================================
 CONSULTAR TOPO
=====================================================================================================*/
const RegistroHistorico *pilha_consultar_topo(const PilhaHistorico *pilha){
    if(pilha_esta_vazia(pilha)){
        return NULL;
    }
    return &pilha->topo->registro;
}

/*=====================================================================================================
 DESEMPILHAR
=====================================================================================================*/
int pilha_desempilhar(PilhaHistorico *pilha, RegistroHistorico *registro_removido){
    NoHistorico *no_removido;

    if (pilha_esta_vazia(pilha) || pilha->tamanho == 0UL) {
        return 0;
    }

    no_removido = pilha->topo;

    if (registro_removido != NULL) {
        *registro_removido = no_removido->registro;
    }

    pilha->topo = no_removido->proximo;
    pilha->tamanho--;

    free(no_removido);

    return 1;
}

/*=====================================================================================================
 EXIBIR
=====================================================================================================*/
void pilha_exibir(const PilhaHistorico *pilha){
    const NoHistorico *atual;

    if (pilha_esta_vazia(pilha)) {
        printf("Historico vazio.\n");
        return;
    }

    printf("Quantidade de eventos: %lu\n", pilha->tamanho);

    atual = pilha->topo;

    while (atual != NULL) {
        printf("Sequencia: %lu | Atendimento: %lu | Operacao: %s\n", 
            atual->registro.sequencia, atual->registro.codigo_atendimento, atual->registro.operacao);
        atual = atual->proximo;
    }
}

/*=====================================================================================================
 DESTRUIR PILHA
=====================================================================================================*/
void pilha_destruir(PilhaHistorico *pilha){
    NoHistorico *no_removido;
    if(pilha == NULL){
        return;
    }
    while (pilha->topo != NULL) {
        no_removido = pilha->topo;
        pilha->topo = no_removido->proximo;
        free(no_removido);
    }

    pilha->topo = NULL;
    pilha->tamanho = 0UL;
    pilha->proxima_sequencia = 1UL;
}
