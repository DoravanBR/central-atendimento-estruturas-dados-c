/*========================================= INCLUSOES =============================================*/

#include <assert.h>
#include <stdio.h>

#include "experimentos.h"

/*========================================= TESTES =============================================*/

int main(void)
{
    printf("Iniciando comparacao experimental entre BST e AVL...\n");

    /*
      A função retorna 1 somente se os três conjuntos forem concluídos com sucesso.
    */
    assert(experimentos_executar() == 1);

    printf("\nTeste dos experimentos: OK\n");

    return 0;
}