/*========================================= INCLUSOES =============================================*/
#include <assert.h>
#include <stdio.h>

#include "bst.h"

/*========================================= DEFINICOES =============================================*/
#define QUANTIDADE_TESTE 7UL

/*=== DADOS SIMULADOS PARA TESTE ==================================================================*/

static Atendimento *criar_atendimento_teste(unsigned long codigo){
    return atendimento_criar(codigo, "Usuario de teste", "Atendimento usado no teste da BST", 5, codigo);
}

/*========================================= TESTES =============================================*/
int main(void)
{
    ArvoreBST arvore;
    ArvoreBST arvore_vazia;
    ArvoreBST arvore_crescente;
    Atendimento *atendimentos[QUANTIDADE_TESTE];
    Atendimento *atendimentos_crescentes[QUANTIDADE_TESTE];
    Atendimento *encontrado;
    Atendimento *duplicado;
    
    unsigned long comparacoes;
    unsigned long i;

    const unsigned long codigos[QUANTIDADE_TESTE] = {20UL, 10UL, 30UL, 5UL, 15UL, 25UL, 35UL};

    printf("Iniciando testes da BST...\n");

/*=== INICIALIZACAO ==================================================================*/

    printf("Testando inicializacao...\n");

    bst_inicializar(NULL); // inicializa a bst nula

    // Verifica a inicializacao
    assert(bst_esta_vazia(NULL) == 1);
    assert(bst_obter_tamanho(NULL) == 0UL);
    assert(bst_obter_altura(NULL) == -1);

    comparacoes = 99UL; // valor sentinela

    assert(bst_buscar( NULL, 1UL, &comparacoes) == NULL); // Verifica se buscar retorna nulo

    assert(comparacoes == 0UL);// Verifica se buscar reseta comparacoes

    bst_inicializar(&arvore); // inicializar a bst

    // Verifica inicializacao
    assert(arvore.raiz == NULL);
    assert(arvore.tamanho == 0UL);

    // Verifica se arvore vazia
    assert(bst_esta_vazia(&arvore) == 1);
    assert(bst_obter_tamanho(&arvore) == 0UL);
    assert(bst_obter_altura(&arvore) == -1);

    comparacoes = 99UL;

    assert(bst_buscar( &arvore, 1UL, &comparacoes) == NULL);// Verifica se buscar retorna nulo

    assert(comparacoes == 0UL);// Verifica se buscar reseta comparacoes

    printf("Inicializacao: OK\n");

/*=== CRIAR E INSERIR ATENDIMENTOS ==================================================================*/

    printf("Criando e inserindo atendimentos...\n");

    for (i = 0UL; i < QUANTIDADE_TESTE; i++) {
        atendimentos[i] = criar_atendimento_teste(codigos[i]);

        assert(atendimentos[i] != NULL);// Verifica se atendimento foi criado
        assert(bst_inserir(&arvore, atendimentos[i]) == 1); // Verifica insercao na bst
        assert(bst_obter_tamanho(&arvore) == i + 1UL); // Verifica atulizacao do tamanho
    }

    // Verifica Integridade da arvore
    assert(bst_esta_vazia(&arvore) == 0);
    assert(arvore.raiz != NULL);
    assert(arvore.raiz->atendimento->codigo == 20UL);

    assert(bst_obter_tamanho(&arvore) == 7UL);
    assert(bst_obter_altura(&arvore) == 2);

    printf("Insercoes: OK\n");

/*=== VERIFICAR ESTRUTURA ==================================================================*/

    printf("Verificando estrutura da BST...\n");

/*
                                                  20
                                                /    \
                                             10      30
                                            /  \    /  \
                                           5   15  25  35
*/    

    assert(arvore.raiz->esquerda ->atendimento->codigo == 10UL);// Verifica filho esquerdo da raiz
    assert(arvore.raiz->direita ->atendimento->codigo == 30UL);// Verifica filho direito da raiz
    assert( arvore.raiz->esquerda->esquerda->atendimento->codigo == 5UL);// Verifica filho esq de 10
    assert(arvore.raiz->esquerda->direita->atendimento->codigo == 15UL);// Verifica filho dir de 10
    assert(arvore.raiz->direita->esquerda->atendimento->codigo == 25UL);// Verifica filho esq de 30
    assert(arvore.raiz->direita->direita->atendimento->codigo == 35UL);// Verifica filho dir de 30

    printf("Estrutura: OK\n");

/*=== VERIFICAR CODIGO DUPLICADO ==================================================================*/

    printf("Testando codigo duplicado...\n");

    duplicado = criar_atendimento_teste(20UL);

    assert(duplicado != NULL);

    assert(bst_inserir(&arvore, duplicado) == 0);// Verifica rejeicao por ser duplicidade

    // Verifica a nao alteracao da bst
    assert(bst_obter_tamanho(&arvore) == 7UL);
    assert(bst_obter_altura(&arvore) == 2);

    atendimento_destruir(duplicado);
    duplicado = NULL;

    printf("Codigo duplicado: OK\n");

/*=== BUSCAS E COMPARACOES ==================================================================*/

    printf("Testando buscas...\n");

    encontrado = bst_buscar(&arvore, 20UL, &comparacoes); // Busca a raiz

    // Verifica se encontrou corretamente
    assert(encontrado == atendimentos[0]);
    assert(comparacoes == 1UL);

    encontrado = bst_buscar(&arvore, 5UL, &comparacoes); // Busca a folha esquerda

    // Verifica se encontrou corretamente
    assert(encontrado == atendimentos[3]);
    assert(comparacoes == 3UL);

    encontrado = bst_buscar(&arvore, 25UL, &comparacoes); // Busca folha esquerda da sub tree direita

    // Verifica se encontrou corretamente
    assert(encontrado == atendimentos[5]);
    assert(comparacoes == 3UL);

    encontrado = bst_buscar(&arvore, 99UL, &comparacoes);// Busca codigo inexistente

    // Verifica se retorna nulo e o total de comparacoes 
    assert(encontrado == NULL);
    assert(comparacoes == 3UL);

    comparacoes = 99UL;

    encontrado = bst_buscar(&arvore, 0UL, &comparacoes);// Busca codigo invalido

    // Verifica se retorna nulo e comparacoes = 0 -> rejeicao
    assert(encontrado == NULL);
    assert(comparacoes == 0UL);

    encontrado = bst_buscar(&arvore, 15UL, NULL);// Busca sem retorno de comparacoes

    // Verifica se encontrou corretamente
    assert(encontrado == atendimentos[4]);

    printf("Buscas: OK\n");



/*=== EXIBICAO - PRE-ORDEM - EM ORDEM - POS-ORDEM =========================================*/

    printf("Testando percursos...\n");

    bst_inicializar(&arvore_vazia);// Inicia arvore vazia

    bst_exibir_pre_ordem(NULL);// Mensagem para arvore vazia
    bst_exibir_em_ordem(&arvore_vazia);// Mensagem para arvore vazia

    // Exibe os percursos
    bst_exibir_pre_ordem(&arvore);
    bst_exibir_em_ordem(&arvore);
    bst_exibir_pos_ordem(&arvore);

    printf("Percursos executados: OK\n");

    // Verifica que percurso nao altera a arvore
    assert(bst_obter_tamanho(&arvore) == 7UL); 
    assert(bst_obter_altura(&arvore) == 2);
    assert(arvore.raiz->atendimento->codigo == 20UL);


/*==================== DEGENERACAO - CRESCENTE ========================*/

/*
                          1
                           \
                             2
                              \
                                3
                                 \
                                  4
                                    \
                                     5
                                      \
                                       6
                                        \
                                         7
*/

/*=== CRIAR E INSERIR =========================================*/

    printf("Testando insercao crescente...\n");

    bst_inicializar(&arvore_crescente);

    for (i = 0UL; i < QUANTIDADE_TESTE; i++) {
        atendimentos_crescentes[i] = criar_atendimento_teste(i + 1UL);
        assert(atendimentos_crescentes[i] != NULL); // Verifica se atendimento foi criado
        assert(bst_inserir( &arvore_crescente, atendimentos_crescentes[i]) == 1);// Verifica insercao na bst
    }

    assert(bst_obter_tamanho( &arvore_crescente) == 7UL);// Verifica tamanho
    assert(bst_obter_altura(&arvore_crescente) == 6);// Verifica altura

/*=== BUSCAS E COMPARACOES =========================================*/

    encontrado = bst_buscar(&arvore_crescente, 1UL, &comparacoes);// Busca raiz

    // Verifica codigo foi encontrado
    assert(encontrado == atendimentos_crescentes[0]);
    assert(comparacoes == 1UL);

    encontrado = bst_buscar(&arvore_crescente,7UL,&comparacoes); // Busca folha

    // Verifica codigo foi encontrado
    assert(encontrado == atendimentos_crescentes[6]);
    assert(comparacoes == 7UL); // Verifica total comparacoes

    printf("Insercao crescente: OK\n");

/*=== DESTRUIR ARVORES =========================================*/

    printf("Testando destruicao...\n");

    bst_destruir(&arvore);
    bst_destruir(&arvore_vazia);
    bst_destruir(&arvore_crescente);

    //Verifica destruicao das arvores
    assert(arvore.raiz == NULL);
    assert(arvore.tamanho == 0UL);

    assert(arvore_vazia.raiz == NULL);
    assert(arvore_vazia.tamanho == 0UL);

    assert(arvore_crescente.raiz == NULL);
    assert(arvore_crescente.tamanho == 0UL);

    // Verifica que apenas as bsts foram destruidas 
    for (i = 0UL; i < QUANTIDADE_TESTE; i++) {
        assert(atendimentos[i] != NULL);
        assert(atendimentos[i]->codigo == codigos[i]);
        assert(atendimentos_crescentes[i] != NULL);
        assert(atendimentos_crescentes[i]->codigo == i + 1UL);
    }

    printf("Atendimentos preservados: OK\n");

    // Libera os atendimentos simulados
    for (i = 0UL; i < QUANTIDADE_TESTE; i++) {
        atendimento_destruir(atendimentos[i]);
        atendimento_destruir(atendimentos_crescentes[i]);
        atendimentos[i] = NULL;
        atendimentos_crescentes[i] = NULL;
    }

    bst_destruir(&arvore);
    bst_destruir(&arvore_vazia);
    bst_destruir(&arvore_crescente);
    bst_destruir(NULL);

    encontrado = NULL;

    printf("Destruicao: OK\n");

    printf(
        "Todos os testes da BST passaram!\n"
    );

    return 0;
}