/*========================================= INCLUSOES =============================================*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "fila.h"

/*========================================= FUNCOES =============================================*/

/*=====================================================================================================
 INICIALIZAR
=====================================================================================================*/
void fila_inicializar(FilaAtendimentos *fila){
    if (fila == NULL) {
        return;
    }

    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0UL;
}

/*=====================================================================================================
 FILA VAZIA
=====================================================================================================*/
int fila_esta_vazia(const FilaAtendimentos *fila){
    return fila == NULL || fila->inicio == NULL;
}

/*=====================================================================================================
 OBTER TAMANHO
=====================================================================================================*/
unsigned long fila_obter_tamanho(const FilaAtendimentos *fila){
    if(fila == NULL){
        return 0UL;
    }
    return fila->tamanho;
}

/*=====================================================================================================
 ENFILEIRAR
=====================================================================================================*/
int fila_enfileirar(FilaAtendimentos *fila, Atendimento *atendimento){
    NoFila *novo;

    if(fila == NULL || atendimento == NULL || atendimento->codigo == 0UL || 
        atendimento->status != STATUS_CLASSIFICADO || (atendimento->classificacao != CLASSIFICACAO_COMUM 
            && atendimento->classificacao != CLASSIFICACAO_MODERADA) || fila->tamanho == ULONG_MAX){
                return 0;
        }
    if((fila->inicio == NULL && fila->fim != NULL) || (fila->inicio != NULL && fila->fim == NULL) 
        || (fila->inicio == NULL && fila->tamanho != 0UL) || (fila->inicio != NULL && fila->tamanho == 0UL)){
        return 0;
    }

    novo = malloc(sizeof(*novo));

    novo->atendimento = atendimento;
    novo->proximo = NULL;

    if(fila->fim == NULL){
        fila->inicio = novo;
    }else{
        fila->fim->proximo = novo;
    }

    fila->fim = novo;
    fila->tamanho++;
    return 1;
}

/*=====================================================================================================
 CONSULTAR
=====================================================================================================*/
Atendimento *fila_consultar_inicio(const FilaAtendimentos *fila){
    if(fila_esta_vazia(fila) || fila->fim == NULL || fila->tamanho == 0UL){
        return NULL;
    }
    return fila->inicio->atendimento;
}

/*=====================================================================================================
 DESENFILEIRAR
=====================================================================================================*/
Atendimento *fila_desenfileirar(FilaAtendimentos *fila){
    
    NoFila *no_removido;
    Atendimento *atendimento;

    if (fila_esta_vazia(fila) || fila->fim == NULL || fila->tamanho == 0UL) {
        return NULL;
    }

    no_removido = fila->inicio;
    atendimento = no_removido->atendimento;

    fila->inicio = no_removido->proximo;
    fila->tamanho--;

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    free(no_removido);

    return atendimento;
}

/*=====================================================================================================
 EXIBIR
=====================================================================================================*/
void fila_exibir(const FilaAtendimentos *fila){
    
    const NoFila *atual;
    unsigned long posicao;

    if (fila_esta_vazia(fila)) {
        printf("Fila de atendimentos vazia.\n");
        return;
    }

    if (fila->fim == NULL || fila->tamanho == 0UL) {
        printf("Fila de atendimentos invalida.\n");
        return;
    }

    printf( "Quantidade na fila: %lu\n", fila->tamanho);

    atual = fila->inicio;
    posicao = 1UL;

    while (atual != NULL) {
        printf( "\n--- Posicao %lu ---\n", posicao);
        atendimento_exibir(atual->atendimento);
        atual = atual->proximo;
        posicao++;
    }
}

/*=====================================================================================================
 DESTRUIR
=====================================================================================================*/
void fila_destruir(FilaAtendimentos *fila){
    
    NoFila *no_removido;

    if (fila == NULL) {
        return;
    }

    while (fila->inicio != NULL) {
        no_removido = fila->inicio;
        fila->inicio = no_removido->proximo;
        free(no_removido);
    }

    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0UL;
}