#ifndef EXPERIMENTOS_H
#define EXPERIMENTOS_H


/*========================================= DEFINICOES =============================================*/

#define QUANTIDADE_CODIGOS 20UL
#define QUANTIDADE_BUSCAS 3UL

/*======================================== ESTRUTURAS ============================================*/

typedef struct {
    unsigned long codigo;
    unsigned long comparacoes_bst;
    unsigned long comparacoes_avl;
} ResultadoBusca;

/*===================================== PROTOTIPO FUNCOES =========================================*/

int experimentos_executar(void);

#endif