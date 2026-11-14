/*========================================= INCLUSOES =============================================*/

#include <assert.h>
#include <stdio.h>

#include "heap.h"

/*========================================= DEFINICOES =============================================*/

#define QUANTIDADE_EXPANSAO 10UL

/*=== DADOS SIMULADOS PARA TESTE ==================================================================*/

static Atendimento *criar_prioritario(unsigned long codigo, int prioridade, unsigned long ordem,
     Classificacao classificacao){

    Atendimento *atendimento;

    atendimento = atendimento_criar(codigo, "Usuario de teste", "Atendimento usado no teste do heap",
        prioridade, ordem);

    if (atendimento == NULL) {
        return NULL;
    }

    /*
      Confirma a classificação e atualiza o status  para STATUS_CLASSIFICADO.
    */
    if (!atendimento_confirmar_classificacao(atendimento, classificacao)) {
        atendimento_destruir(atendimento);
        return NULL;
    }

    return atendimento;
}

/*========================================= TESTES =============================================*/

int main(void)
{
    HeapMaximo heap;
    Atendimento *atendimento1;
    Atendimento *atendimento2;
    Atendimento *atendimento3;
    Atendimento *removido;
    Atendimento *aberto;
    Atendimento *comum;
    Atendimento *atendimentos_expansao[QUANTIDADE_EXPANSAO];

    unsigned long i;

    printf("Iniciando testes do heap...\n");

/*=== INICIALIZACAO ==================================================================*/

    printf("Testando inicializacao...\n");

    // Ponteiro nulo deve ser rejeitado. 
    assert(heap_inicializar(NULL) == 0);

    // Inicializa vetor com capacidade 8 e nenhuma posição ocupada. 
    assert(heap_inicializar(&heap) == 1);
    assert(heap.itens != NULL);
    assert(heap.quantidade == 0UL);
    assert(heap.capacidade == 8UL);

    assert(heap_esta_vazio(&heap) == 1);
    assert(heap_obter_tamanho(&heap) == 0UL);
    assert(heap_consultar_raiz(&heap) == NULL);
    assert(heap_remover_raiz(&heap) == NULL);

    // Consultas nulas também devem ser seguras. 
    assert(heap_esta_vazio(NULL) == 1);
    assert(heap_obter_tamanho(NULL) == 0UL);
    assert(heap_consultar_raiz(NULL) == NULL);
    assert(heap_remover_raiz(NULL) == NULL);

    printf("Inicializacao: OK\n");

/*=== INSERCOES INVALIDAS ===============================================================*/

    printf("Testando insercoes invalidas...\n");

    // Heap nulo e atendimento nulo devem ser rejeitados. 
    assert(heap_inserir( NULL, NULL) == 0);
    assert(heap_inserir(&heap, NULL) == 0);

    // Cria um atendimento que ainda está aberto. 
    aberto = atendimento_criar(100UL, "Usuario aberto", "Atendimento ainda nao classificado",
        8, 100UL);

    assert(aberto != NULL);
    assert(aberto->status == STATUS_ABERTO);

    /*
      Mesmo com prioridade alta, não pode entrar antes de ter sua classificação confirmada.
    */
    assert(heap_inserir(&heap, aberto) == 0);

    // Cria um atendimento classificado como comum. 
    comum = atendimento_criar(101UL, "Usuario comum", "Atendimento destinado a fila", 9, 101UL);

    assert(comum != NULL);

    assert(atendimento_confirmar_classificacao(comum, CLASSIFICACAO_COMUM) == 1);

    /*
      Um chamado comum pertence à fila, mesmo que sua prioridade numérica seja alta.
    */
    assert(heap_inserir(&heap, comum) == 0);

    // As tentativas inválidas não podem alterar o heap. 
    assert(heap.quantidade == 0UL);
    assert(heap.capacidade == 8UL);
    assert(heap_consultar_raiz(&heap) == NULL);

    atendimento_destruir(aberto);
    atendimento_destruir(comum);

    aberto = NULL;
    comum = NULL;

    printf("Insercoes invalidas: OK\n");

/*=== CRIAR E INSERIR ===============================================================*/

    printf("Testando subida por prioridade...\n");

    atendimento1 = criar_prioritario(1UL, 7, 1UL, CLASSIFICACAO_URGENTE);
    atendimento2 = criar_prioritario(2UL, 10, 2UL, CLASSIFICACAO_CRITICA);
    atendimento3 = criar_prioritario(3UL, 8, 3UL, CLASSIFICACAO_URGENTE);

    assert(atendimento1 != NULL);
    assert(atendimento2 != NULL);
    assert(atendimento3 != NULL);

    /*
      O primeiro elemento ocupa diretamente a raiz.
    */
    assert(heap_inserir(&heap, atendimento1) == 1);
    assert(heap_consultar_raiz(&heap) == atendimento1);

    /*
      Prioridade 10 deve subir e substituir prioridade 7 na raiz.
    */
    assert(heap_inserir(&heap, atendimento2) == 1);
    assert(heap_consultar_raiz(&heap) == atendimento2);

    /*
      Prioridade 8 fica abaixo da raiz 10, pois não possui precedência sobre ela.
    */
    assert(heap_inserir(&heap, atendimento3) == 1);
    assert(heap_consultar_raiz(&heap) == atendimento2);

    assert(heap_obter_tamanho(&heap) == 3UL);
    assert(heap.capacidade == 8UL);

    /*
      Vetor esperado:
     
      índice 0 → prioridade 10
      índice 1 → prioridade 7
      índice 2 → prioridade 8
    */
    assert(heap.itens[0] == atendimento2);
    assert(heap.itens[1] == atendimento1);
    assert(heap.itens[2] == atendimento3);

    printf("Subida por prioridade: OK\n");

/*=== REMOVER ===============================================================*/

    printf("Testando remocao e descida...\n");

    /*
      Remove a raiz de prioridade 10.
      O último elemento, prioridade 8, ocupa a raiz.
    */
    removido = heap_remover_raiz(&heap);

    assert(removido == atendimento2);
    assert(heap_obter_tamanho(&heap) == 2UL);

    /*
      Prioridade 8 deve permanecer acima de 7.
     
      Vetor esperado:
      índice 0 → prioridade 8
      índice 1 → prioridade 7
    */
    assert(heap_consultar_raiz(&heap) == atendimento3);

    assert(heap.itens[0] == atendimento3);
    assert(heap.itens[1] == atendimento1);

    printf("Remocao e descida: OK\n");


    printf("Testando ordem das remocoes...\n");

    /*
     A segunda remoção deve devolver prioridade 8. 
    */
    removido = heap_remover_raiz(&heap);

    assert(removido == atendimento3);
    assert(heap_obter_tamanho(&heap) == 1UL);

    assert(heap_consultar_raiz(&heap) == atendimento1);

    // A última remoção deve devolver prioridade 7. 
    removido = heap_remover_raiz(&heap);

    assert(removido == atendimento1);
    assert(heap_obter_tamanho(&heap) == 0UL);
    assert(heap_esta_vazio(&heap) == 1);
    assert(heap_consultar_raiz(&heap) == NULL);

    /*
      Outra remoção em heap vazio deve ser segura e não alterar os metadados.
    */
    assert(heap_remover_raiz(&heap) == NULL);
    assert(heap.quantidade == 0UL);
    assert(heap.capacidade == 8UL);

    printf("Ordem das remocoes: OK\n");

    // Liberar atendimentos que nao estao mais no heap
    atendimento_destruir(atendimento1);
    atendimento_destruir(atendimento2);
    atendimento_destruir(atendimento3);

    atendimento1 = NULL;
    atendimento2 = NULL;
    atendimento3 = NULL;
    removido = NULL;

/*=== TESTAR DESEMPATE ===============================================================*/

    printf("Testando desempate por ordem...\n");

    /*
      Todos possuem prioridade 9.
      A menor ordem representa o mais antigo.
    */
    atendimento1 = criar_prioritario(10UL, 9, 30UL, CLASSIFICACAO_URGENTE);
    atendimento2 = criar_prioritario(11UL, 9, 10UL, CLASSIFICACAO_URGENTE);
    atendimento3 = criar_prioritario(12UL, 9, 20UL, CLASSIFICACAO_CRITICA);

    assert(atendimento1 != NULL);
    assert(atendimento2 != NULL);
    assert(atendimento3 != NULL);

    /*
      Insere fora da ordem cronológica:
      ordem 30, depois 10, depois 20.
    */
    assert(heap_inserir(&heap, atendimento1) == 1);
    assert(heap_inserir(&heap, atendimento2) == 1);

    assert(heap_inserir(&heap, atendimento3) == 1);

    /*
      Ordem 10 deve subir à raiz, pois é a mais antiga entre as prioridades 9.
    */
    assert(heap_consultar_raiz(&heap) == atendimento2);

    assert(heap_consultar_raiz(&heap)->ordem_chegada == 10UL);

    // Primeiro deve sair ordem 10. 
    removido = heap_remover_raiz(&heap);
    assert(removido == atendimento2);

    // Depois deve sair ordem 20. 
    removido = heap_remover_raiz(&heap);
    assert(removido == atendimento3);

    // Por último deve sair ordem 30. 
    removido = heap_remover_raiz(&heap);
    assert(removido == atendimento1);

    assert(heap_esta_vazio(&heap) == 1);

    printf("Desempate por ordem: OK\n");

    atendimento_destruir(atendimento1);
    atendimento_destruir(atendimento2);
    atendimento_destruir(atendimento3);

    atendimento1 = NULL;
    atendimento2 = NULL;
    atendimento3 = NULL;
    removido = NULL;

/*=== TESTAR EXPANSAO DO HEAP ===============================================================*/

    printf("Testando expansao do vetor...\n");

    /*
      Insere prioridades de 1 até 10.
      O nono elemento deve forçar 8 → 16.
    */
    for (i = 0UL; i < QUANTIDADE_EXPANSAO; i++) {
        atendimentos_expansao[i] = criar_prioritario(100UL + i, (int)(i + 1UL), 100UL + i, CLASSIFICACAO_URGENTE);
        assert(atendimentos_expansao[i] != NULL);
        assert(heap_inserir(&heap, atendimentos_expansao[i]) == 1);
    }

    // Dez posições estão ocupadas no vetor expandido. 
    assert(heap_obter_tamanho(&heap) == 10UL);
    assert(heap.capacidade == 16UL);

    /*
      A prioridade 10 foi inserida por último, mas deve ter subido até a raiz.
    */
    assert(heap_consultar_raiz(&heap) == atendimentos_expansao[9]);
    assert(heap_consultar_raiz(&heap)->prioridade == 10);

    printf("Expansao do vetor: OK\n");

/*=== EXIBIR ===============================================================*/

    printf("Exibindo heap expandido...\n");

    /*
      A exibição mostra o vetor interno.
      Ela não representa uma lista totalmente ordenada.
    */
    heap_exibir(&heap);

    assert(heap_obter_tamanho(&heap) == 10UL);
    assert(heap.capacidade == 16UL);

/*=== REMOCAO ===============================================================*/

    printf("Testando remocoes em ordem de prioridade...\n");

    /*
      Como as prioridades são únicas, devem sair:
      10, 9, 8, ..., 1.
    */
    for (i = 0UL; i < QUANTIDADE_EXPANSAO; i++) {
        int prioridade_esperada;
        unsigned long indice_esperado;

        prioridade_esperada = 10 - (int)i;

        indice_esperado = QUANTIDADE_EXPANSAO - 1UL - i;

        removido = heap_remover_raiz(&heap);

        assert(removido != NULL);

        assert(removido == atendimentos_expansao[indice_esperado]);

        assert(removido->prioridade == prioridade_esperada);

        /*
          O atendimento já saiu do heap, então o teste pode destruí-lo.
        */
        atendimento_destruir(removido);

        atendimentos_expansao[indice_esperado] = NULL;

        removido = NULL;
    }

    assert(heap_esta_vazio(&heap) == 1);
    assert(heap_obter_tamanho(&heap) == 0UL);

    /*
      Remover elementos não reduz a capacidade;
      o vetor expandido continua pronto para reutilização.
    */
    assert(heap.capacidade == 16UL);
    assert(heap_consultar_raiz(&heap) == NULL);

    printf("Remocoes por prioridade: OK\n");

/*=== TESTANDO HEAP VAZIO ===============================================================*/

    printf("Testando exibicao de heap vazio...\n");

    // Heap nulo e heap sem elementos devem ser seguros. 
    heap_exibir(NULL);
    heap_exibir(&heap);

    printf("Exibicao vazia: OK\n");

    printf("Testando destruicao...\n");

    /*
      O heap está vazio, mas seu vetor de capacidade 16 ainda está alocado e deve ser liberado.
    */
    heap_destruir(&heap);

    assert(heap.itens == NULL);
    assert(heap.quantidade == 0UL);
    assert(heap.capacidade == 0UL);

    /*
      Destruir novamente ou receber NULL não pode causar liberação dupla.
    */
    heap_destruir(&heap);
    heap_destruir(NULL);

    assert(heap.itens == NULL);
    assert(heap.quantidade == 0UL);
    assert(heap.capacidade == 0UL);

    printf("Destruicao: OK\n");    

    printf(
        "Todos os testes do heap passaram!\n"
    );

    return 0;
}