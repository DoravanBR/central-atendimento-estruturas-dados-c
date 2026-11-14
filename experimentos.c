/*========================================= INCLUSOES =============================================*/

#include <stdio.h>

#include "atendimento.h"
#include "avl.h"
#include "bst.h"
#include "experimentos.h"

/*========================================= FUNCOES =============================================*/

/*=====================================================================================================
 CRIAR ATENDIMENTO
=====================================================================================================*/

static Atendimento *criar_atendimento_experimento(unsigned long codigo){
    int prioridade;

    prioridade = (int)(codigo % 10UL) + 1;

    return atendimento_criar(codigo, "Usuario experimental", "Atendimento usado na comparacao BST e AVL",
         prioridade, codigo);
}

/*=====================================================================================================
 LIMPAR 
=====================================================================================================*/

static void limpar_experimento(ArvoreBST *bst, ArvoreAVL *avl, Atendimento **atendimentos,
     unsigned long quantidade){

    unsigned long i;

    /*
      Primeiro remove os nós que emprestam ponteiros para os atendimentos.
    */
    bst_destruir(bst);
    avl_destruir(avl);

    /*
      Depois libera os objetos centrais criados especificamente pelo experimento.
    */
    if (atendimentos != NULL) {
        for (i = 0UL; i < quantidade; i++) {
            atendimento_destruir(atendimentos[i]);
            atendimentos[i] = NULL;
        }
    }
}

/*=====================================================================================================
 INTERPRETACAO 
=====================================================================================================*/

static void exibir_interpretacao(int altura_bst, int altura_avl){

    printf("\nInterpretacao:\n");

    if (altura_bst > altura_avl) {
        printf("A AVL terminou com altura menor. O balanceamento reduziu a profundidade "
             "necessaria para alcancar os codigos.\n");
        return;
    }

    if (altura_bst == altura_avl) {
        printf("As duas arvores terminaram com a mesma altura neste conjunto. Isso pode ocorrer "
            "quando a ordem de insercao favorece a BST.\n");
        return;
    }

    printf("A BST terminou com altura menor neste conjunto. Esse resultado depende da ordem especifica das "
        "chaves e deve ser analisado junto das buscas.\n");
}

/*=====================================================================================================
 EXECUTAR 
=====================================================================================================*/

static int executar_experimento(const char *nome, const unsigned long codigos[QUANTIDADE_CODIGOS],
    const unsigned long buscas[QUANTIDADE_BUSCAS]){

    ArvoreBST bst;
    ArvoreAVL avl;
    Atendimento *atendimentos[QUANTIDADE_CODIGOS] = {NULL};

    ResultadoBusca resultados[QUANTIDADE_BUSCAS];

    unsigned long i;

    if (nome == NULL || codigos == NULL || buscas == NULL) {
        return 0;
    }

    bst_inicializar(&bst);
    avl_inicializar(&avl);

    printf("\n========================================\n");
    printf("Experimento: %s\n", nome);
    printf("========================================\n");    

    /*
      Cria cada atendimento uma única vez e insere o mesmo ponteiro nas duas árvores.
    */
    for (i = 0UL; i < QUANTIDADE_CODIGOS; i++) {
        atendimentos[i] = criar_atendimento_experimento(codigos[i]);
        if (atendimentos[i] == NULL) {
            printf("Falha ao criar o atendimento %lu.\n", codigos[i]);
            limpar_experimento(&bst, &avl, atendimentos, QUANTIDADE_CODIGOS);
            return 0;
        }

        if (!bst_inserir(&bst, atendimentos[i])) {
            printf("Falha ao inserir %lu na BST.\n", codigos[i]);
            limpar_experimento(&bst, &avl, atendimentos, QUANTIDADE_CODIGOS);
            return 0;
        }

        if (!avl_inserir(&avl, atendimentos[i])) {
            printf("Falha ao inserir %lu na AVL.\n", codigos[i]);
            limpar_experimento(&bst, &avl, atendimentos, QUANTIDADE_CODIGOS);
            return 0;
        }
    }

    /*
      Executa as mesmas três buscas nas duas árvores.
    */
    for (i = 0UL; i < QUANTIDADE_BUSCAS; i++) {

        Atendimento *encontrado_bst;
        Atendimento *encontrado_avl;

        resultados[i].codigo = buscas[i];

        encontrado_bst = bst_buscar(&bst, buscas[i], &resultados[i].comparacoes_bst);

        encontrado_avl = avl_buscar(&avl, buscas[i], &resultados[i].comparacoes_avl);

        /*
          Todas as chaves escolhidas para busca devem existir nos dois índices.
        */
        if (encontrado_bst == NULL || encontrado_avl == NULL || encontrado_bst != encontrado_avl) {
            printf("Falha ao localizar o codigo %lu.\n", buscas[i]);
            limpar_experimento(&bst, &avl, atendimentos, QUANTIDADE_CODIGOS);
            return 0;
        }
    }

    {
        int altura_bst;
        int altura_avl;

        altura_bst = bst_obter_altura(&bst);

        altura_avl = avl_obter_altura(&avl);

        printf("\nQuantidade de nos:\n BST: %lu\n AVL: %lu\n", bst_obter_tamanho(&bst),
         avl_obter_tamanho(&avl));

        printf("\nAltura final:\n BST: %d\n AVL: %d\n", altura_bst, altura_avl);

        printf("\nRotacoes da AVL: %lu\n", avl_obter_rotacoes(&avl));

        printf("\nBuscas:\n Codigo | BST | AVL\n");

        for (i = 0UL; i < QUANTIDADE_BUSCAS; i++) {
            printf("%6lu | %3lu | %3lu\n", resultados[i].codigo, resultados[i].comparacoes_bst,
                 resultados[i].comparacoes_avl);
        }  
        printf("\nPercurso em ordem da BST:\n");
        bst_exibir_em_ordem(&bst);

        printf("\nPercurso em ordem da AVL:\n");
        avl_exibir_em_ordem(&avl);  

        exibir_interpretacao(altura_bst, altura_avl);
    }

    /*
      Libera nós e atendimentos antes de iniciar o próximo conjunto.
    */
    limpar_experimento(&bst, &avl, atendimentos, QUANTIDADE_CODIGOS);

    return 1;
} 

int experimentos_executar(void)
{
    static const unsigned long crescente[QUANTIDADE_CODIGOS] = {1UL,  2UL,  3UL,  4UL,  5UL,
         6UL,  7UL,  8UL,  9UL, 10UL, 11UL, 12UL, 13UL, 14UL, 15UL, 16UL, 17UL, 18UL, 19UL, 20UL};

    static const unsigned long decrescente[QUANTIDADE_CODIGOS] = {20UL, 19UL, 18UL, 17UL, 16UL,
         15UL, 14UL, 13UL, 12UL, 11UL, 10UL,  9UL,  8UL,  7UL,  6UL, 5UL,  4UL,  3UL,  2UL,  1UL};

    static const unsigned long aleatoria[QUANTIDADE_CODIGOS] = {8UL,  3UL, 15UL,  1UL, 10UL,
         18UL,  5UL, 12UL, 20UL,  6UL, 14UL,  2UL, 17UL,  9UL,  4UL, 19UL, 11UL,  7UL, 16UL, 13UL};

    static const unsigned long buscas[QUANTIDADE_BUSCAS] = {1UL, 10UL, 20UL};

    /*
      A ordem crescente tende a degenerar a BST para a direita.
    */
    if (!executar_experimento("A - Insercao crescente", crescente, buscas)) {
        return 0;
    }  

    /*
      A ordem decrescente tende a degenerar a BST para a esquerda.
    */
    if (!executar_experimento("B - Insercao decrescente", decrescente, buscas)) {
        return 0;
    }
  
    /*
      A ordem fixa pseudoaleatoria representa um cenário mais próximo do uso comum.
    */
    if (!executar_experimento("C - Insercao pseudoaleatoria", aleatoria, buscas)) {
        return 0;
    }

    printf("\nTodos os experimentos foram concluidos.\n");

    return 1;
}