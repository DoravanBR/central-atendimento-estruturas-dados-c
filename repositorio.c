/*========================================= INCLUSOES =============================================*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "repositorio.h"

/*========================================= DEFINICOES =============================================*/

#define CAPACIDADE_INICIAL 8UL

/*========================================= FUNCOES =============================================*/

/*=====================================================================================================
 LOCALIZA INDICE DO CODIGO
=====================================================================================================*/
static int localizar_indice(const Repositorio *repositorio, unsigned long codigo, unsigned long *indice){
    unsigned long i;

    if (repositorio == NULL || repositorio->itens == NULL || codigo == 0 || indice == NULL) {
        return 0;
    }

    for (i = 0; i < repositorio->quantidade; i++) {
        if (repositorio->itens[i] != NULL && repositorio->itens[i]->codigo == codigo) {
            *indice = i;
            return 1;
        }
    }

    return 0;
}
/*=====================================================================================================
 AUMENTA CAPACIDADE DO REPOSITORIO
=====================================================================================================*/
static int aumentar_capacidade(Repositorio *repositorio){
    unsigned long nova_capacidade;
    Atendimento **novos_itens;

    if (repositorio == NULL || repositorio->itens == NULL || repositorio->capacidade == 0) {
        return 0;
    }

    if (repositorio->capacidade > ULONG_MAX / 2UL) {
        return 0;
    }

    nova_capacidade = repositorio->capacidade * 2UL;

    novos_itens = realloc(
        repositorio->itens,
        nova_capacidade * sizeof(*novos_itens)
    );

    if (novos_itens == NULL) {
        return 0;
    }

    repositorio->itens = novos_itens;
    repositorio->capacidade = nova_capacidade;

    return 1;
}
/*=====================================================================================================
 INICIALIZAR REPOSITORIO
=====================================================================================================*/
int repositorio_inicializar(Repositorio *repositorio){
    Atendimento **itens;

    if (repositorio == NULL) {
        return 0;
    }

    repositorio->itens = NULL;
    repositorio->quantidade = 0;
    repositorio->capacidade = 0;

    itens = malloc(
        CAPACIDADE_INICIAL * sizeof(*itens)
    );

    if (itens == NULL) {
        return 0;
    }

    repositorio->itens = itens;
    repositorio->capacidade = CAPACIDADE_INICIAL;

    return 1;
}
/*=====================================================================================================
 BUSCAR ATENDIMENTO NO REPOSITORIO
=====================================================================================================*/
Atendimento *repositorio_buscar(Repositorio *repositorio, unsigned long codigo){
    unsigned long indice;

    if (!localizar_indice(repositorio, codigo, &indice)) {
        return NULL;
    }

    return repositorio->itens[indice];
}
/*=====================================================================================================
 INSERIR NO REPOSITORIO
=====================================================================================================*/
int repositorio_inserir(Repositorio *repositorio, Atendimento *atendimento){
    if (repositorio == NULL || repositorio->itens == NULL || atendimento == NULL || 
        atendimento->codigo == 0 || repositorio->quantidade > repositorio->capacidade) {
        return 0;
    }

    if (repositorio_buscar(repositorio, atendimento->codigo) != NULL) {
        return 0;
    }

    if (repositorio->quantidade == repositorio->capacidade) {
        if (!aumentar_capacidade(repositorio)) {
            return 0;
        }
    }

    repositorio->itens[repositorio->quantidade] = atendimento;
    repositorio->quantidade++;

    return 1;
}
/*=====================================================================================================
 OBTER QUANTIDADE
=====================================================================================================*/
unsigned long repositorio_obter_quantidade(const Repositorio *repositorio){
    if (repositorio == NULL) {
        return 0;
    }

    return repositorio->quantidade;
}
/*=====================================================================================================
 EXIBIR REPOSITORIO
=====================================================================================================*/
void repositorio_exibir(const Repositorio *repositorio){
    unsigned long i;

    if (repositorio == NULL || repositorio->itens == NULL) {
        printf("Repositorio invalido.\n");
        return;
    }

    if (repositorio->quantidade == 0) {
        printf("Repositorio vazio.\n");
        return;
    }

    printf("Quantidade de atendimentos: %lu\n", repositorio->quantidade);

    for (i = 0; i < repositorio->quantidade; i++) {
        printf("\n--- Atendimento %lu ---\n", i + 1UL);
        atendimento_exibir(repositorio->itens[i]);
    }
}
/*=====================================================================================================
 EXCLUIR REPOSITORIO
=====================================================================================================*/
void repositorio_destruir(Repositorio *repositorio){
    unsigned long i;

    if (repositorio == NULL) {
        return;
    }

    if (repositorio->itens != NULL) {
        for (i = 0; i < repositorio->quantidade; i++) {
            atendimento_destruir( repositorio->itens[i]);
            repositorio->itens[i] = NULL;
        }
        free(repositorio->itens);
    }

    repositorio->itens = NULL;
    repositorio->quantidade = 0;
    repositorio->capacidade = 0;
}

