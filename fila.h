#ifndef FILA_H
#define FILA_H

/*========================================= INCLUSOES =============================================*/

#include "atendimento.h"

/*========================================= ESTRUTURAS =============================================*/

/*=====================================================================================================
 NO DA FILA
=====================================================================================================*/
typedef struct NoFila {
    Atendimento *atendimento;
    struct NoFila *proximo;
} NoFila;

/*=====================================================================================================
 FILA
=====================================================================================================*/
typedef struct {
    NoFila *inicio;
    NoFila *fim;
    unsigned long tamanho;
} FilaAtendimentos;

/*=================================== PROTOTIPO DAS FUNCOES =======================================*/
void fila_inicializar(FilaAtendimentos *fila);
int fila_esta_vazia(const FilaAtendimentos *fila);
unsigned long fila_obter_tamanho(const FilaAtendimentos *fila);
int fila_enfileirar(FilaAtendimentos *fila, Atendimento *atendimento);
Atendimento *fila_consultar_inicio(const FilaAtendimentos *fila);
Atendimento *fila_desenfileirar(FilaAtendimentos *fila);
void fila_exibir(const FilaAtendimentos *fila);
void fila_destruir(FilaAtendimentos *fila);

#endif
