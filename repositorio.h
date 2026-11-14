#ifndef REPOSITORIO_H
#define REPOSITORIO_H

/*========================================= INCLUSOES =============================================*/

#include "atendimento.h"

/*========================================= ESTRUTURA =============================================*/

/*=====================================================================================================
 REPOSITORIO
=====================================================================================================*/
typedef struct {
    Atendimento **itens;
    unsigned long quantidade;
    unsigned long capacidade;
} Repositorio;

/*=================================== PROTOTIPOS DE FUNCOES =======================================*/

int repositorio_inicializar(Repositorio *repositorio);
int repositorio_inserir(Repositorio *repositorio, Atendimento *atendimento);
Atendimento *repositorio_buscar(Repositorio *repositorio, unsigned long codigo);
unsigned long repositorio_obter_quantidade(const Repositorio *repositorio);
void repositorio_exibir(const Repositorio *repositorio);
void repositorio_destruir(Repositorio *repositorio);

#endif
