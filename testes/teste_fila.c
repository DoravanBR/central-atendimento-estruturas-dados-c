/*========================================= INCLUSOES =============================================*/
#include <assert.h>
#include <stdio.h>

#include "fila.h"

/*========================================= TESTES =============================================*/

int main(void)
{
    FilaAtendimentos fila;
    FilaAtendimentos fila_vazia;
    Atendimento *atendimento1;
    Atendimento *atendimento2;
    Atendimento *atendimento3;
    Atendimento *removido;

    printf("Iniciando testes da fila...\n");

/*=== DADOS SIMULADOS PARA TESTE ==================================================================*/

    printf("Criando atendimentos de teste...\n");

    atendimento1 = atendimento_criar(1UL, "Ana Silva", "Computador sem acesso ao sistema", 3, 1UL);
    atendimento2 = atendimento_criar(2UL, "Bruno Costa", "Impressora indisponivel", 5, 2UL);
    atendimento3 = atendimento_criar( 3UL, "Carla Souza", "Servidor principal indisponivel", 10, 3UL);

    // Verifica se atendimentos foram criados
    assert(atendimento1 != NULL);
    assert(atendimento2 != NULL);
    assert(atendimento3 != NULL);

    // Verificas se Status inicial é aberto
    assert(atendimento1->status == STATUS_ABERTO);
    assert(atendimento2->status == STATUS_ABERTO);
    assert(atendimento3->status == STATUS_ABERTO);

    printf("Criacao dos atendimentos: OK\n");

/*=== INICIALIZACAO =============================================================================*/
    
    printf("Testando inicializacao...\n");

    fila_inicializar(NULL); // Inicializa sem fila existente

    assert(fila_esta_vazia(NULL) == 1); // Testa se fila esta vazia ou inexistente
    assert(fila_obter_tamanho(NULL) == 0UL);// Verifica se tamanho = 0 
    assert(fila_consultar_inicio(NULL) == NULL);// verifica inicio nulo
    assert(fila_desenfileirar(NULL) == NULL);// verifica desenfileirar retorna nulo
 
    fila_inicializar(&fila);// Inicializa uma fila

    // Verifica se os campos iniciaram corretamente
    assert(fila.inicio == NULL); 
    assert(fila.fim == NULL);
    assert(fila.tamanho == 0UL);

    assert(fila_esta_vazia(&fila) == 1); // Verifica se funcao retorna fila vazia
    assert(fila_obter_tamanho(&fila) == 0UL);// Verifica se tamanho = 0
    assert(fila_consultar_inicio(&fila) == NULL);// verifica inicio nulo
    assert(fila_desenfileirar(&fila) == NULL);// verifica desenfileirar retorna nulo

    printf("Inicializacao: OK\n");

/*=== INSERCAO INVALIDA =========================================================================*/

    printf("Testando insercoes invalidas...\n");

    assert(fila_enfileirar(NULL, atendimento1) == 0); // Verifica rejeicao para fila nula
    assert(fila_enfileirar(&fila,  NULL) == 0); // Verifica rejeicao para atendimento nulo
    assert(fila_enfileirar(&fila, atendimento1) == 0); // Verifica rejeicao atendimento nao classificado

    // Verifica que a fila nao teve alteracao
    assert(fila.inicio == NULL);
    assert(fila.fim == NULL);
    assert(fila.tamanho == 0UL);

    printf("Insercoes invalidas: OK\n");

/*=== CLASSIFICACAO DOS ATENDIMENTOS =============================================================*/    

    printf("Classificando atendimentos...\n");
    
    // Verifica se classificacao foi feita corretamente
    assert(atendimento_confirmar_classificacao( atendimento1, CLASSIFICACAO_COMUM) == 1);
    assert(atendimento_confirmar_classificacao( atendimento2, CLASSIFICACAO_MODERADA) == 1);
    assert(atendimento_confirmar_classificacao(  atendimento3, CLASSIFICACAO_CRITICA) == 1);

    // Verifica a alteracao de status
    assert(atendimento1->status == STATUS_CLASSIFICADO);
    assert(atendimento2->status == STATUS_CLASSIFICADO);
    assert(atendimento3->status == STATUS_CLASSIFICADO);

    printf("Classificacoes confirmadas: OK\n");

/*======================================== ENFILEIRAR ============================================*/

/*=== PRIMEIRA INSERCAO =============================================================*/

    printf("Testando primeira insercao...\n");

    assert(fila_enfileirar(&fila, atendimento1) == 1);// Verifica sucesso ao inserir atendimento na fila
    assert(fila_esta_vazia(&fila) == 0);// Verifica se fila nao esta vazia (1 = vazia, 0 = não vazia)
    assert(fila_obter_tamanho(&fila) == 1UL);// Verifica atualizacao do tamanho da fila apos insercao

    // Verifica se os ponteiros da fila estao corretos
    assert(fila.inicio != NULL);
    assert(fila.fim != NULL);
    assert(fila.inicio == fila.fim);

    // Verifica integridade da fila
    assert(fila.inicio->atendimento == atendimento1);
    assert(fila.inicio->proximo == NULL);
    assert(fila_consultar_inicio(&fila) == atendimento1);

    printf("Primeira insercao: OK\n");

/*=== SEGUNDA INSERCAO =============================================================*/

    printf("Testando segunda insercao...\n");

    assert(fila_enfileirar( &fila, atendimento2) == 1);// Verifica sucesso ao inserir atendimento na fila
    assert(fila_obter_tamanho(&fila) == 2UL);// Verifica atualizacao do tamanho da fila apos insercao
    assert(fila.inicio != fila.fim);// Verifica se ptr fim foi atualizado ficando != de inicio

    // Verifica integridade da fila
    assert(fila.inicio->atendimento == atendimento1);
    assert(fila.fim->atendimento == atendimento2);
    assert(fila.inicio->proximo == fila.fim);
    assert(fila.fim->proximo == NULL);
    assert(fila_consultar_inicio(&fila) == atendimento1);

    printf("Segunda insercao: OK\n");

/*=== TERCEIRA INSERCAO =============================================================*/

    printf("Testando rejeicao de critico...\n");

    assert(fila_enfileirar(&fila, atendimento3) == 0);// Verifica a rejeicao da insercao (class = critico)
    assert(fila_obter_tamanho(&fila) == 2UL);// Verifica se tamanho nao se alterou
    assert(fila.inicio->atendimento == atendimento1);// Verifica se inicio se mantem

    assert( fila.fim->atendimento == atendimento2);// Verfica se final da fila continua inalterado

    printf("Rejeicao de critico: OK\n");

/*======================================== DESENFILEIRAR ============================================*/

/*=== PRIMEIRA REMOCAO =============================================================*/

    printf("Testando primeira remocao...\n");

    removido = fila_desenfileirar(&fila);// Retorna o primeiro elemento da fila

    assert(removido != NULL);// Verifica se nao e nulo
    assert(removido == atendimento1); // Verifica se e o primeiro elemento da fila

    // Verifica Integridade da fila
    assert(fila_obter_tamanho(&fila) == 1UL); // Verifica atualizacao do tamanho
    assert(fila.inicio != NULL);
    assert(fila.fim != NULL);
    assert(fila.inicio == fila.fim);

    // Verifica os dados da fila
    assert(fila.inicio->atendimento == atendimento2);
    assert(fila.inicio->proximo == NULL);
    assert(fila_consultar_inicio(&fila) == atendimento2);

    printf("Primeira remocao: OK\n");

/*=== REMOCAO DO ULTIMO ELEMENTO =============================================================*/

    printf("Testando remocao do ultimo elemento...\n");

    removido = fila_desenfileirar(&fila);// Retorna o primeiro elemento da fila

    assert(removido != NULL);// Verifica se nao e nulo
    assert(removido == atendimento2);// Verifica se e o primeiro elemento da fila

    assert(fila_esta_vazia(&fila) == 1);// Verifica se a fila ficou vazia
    assert(fila_obter_tamanho(&fila) == 0UL);// Verifica atualizacao do tamanho

    // Verifica integridade da fila
    assert(fila.inicio == NULL);
    assert(fila.fim == NULL);
    assert(fila_consultar_inicio(&fila) == NULL);

    printf("Remocao do ultimo elemento: OK\n");

/*=== REMOCAO EM FILA VAZIA =============================================================*/

    printf("Testando remocao em fila vazia...\n");

    removido = fila_desenfileirar(&fila);

    assert(removido == NULL);// Verifica se retorna nulo pois fila esta vazia

    assert(fila.inicio == NULL);
    assert(fila.fim == NULL);
    assert(fila.tamanho == 0UL);

    printf("Remocao em fila vazia: OK\n");

/*=== EXIBICAO =============================================================*/

    printf("Preparando teste de exibicao...\n");

    // Enfileira dois atendimentos na fila e verifica sucesso
    assert(fila_enfileirar(&fila, atendimento1) == 1);
    assert(fila_enfileirar( &fila, atendimento2) == 1);
    assert(fila_obter_tamanho(&fila) == 2UL);

    fila_inicializar(&fila_vazia); // inicializa um fila vazia

    fila_exibir(NULL);// Exibe mensagem para fila invalida
    fila_exibir(&fila_vazia);// Exibe mensagem para fila vazia
    fila_exibir(&fila);// Exibe a fila

    printf("Exibicao executada: OK\n");

    // Verifica que fila nao foi alteraca apos exibicao
    assert(fila_obter_tamanho(&fila) == 2UL);
    assert(fila_consultar_inicio(&fila) == atendimento1);
    assert(fila.inicio->proximo->atendimento == atendimento2);

/*=== DESTRUIR FILA =============================================================*/

    printf("Testando destruicao da fila...\n");

    fila_destruir(&fila);// Destroi a fila

    // Verifica esta de fila vazia
    assert(fila.inicio == NULL);
    assert(fila.fim == NULL);
    assert(fila.tamanho == 0UL);
    assert(fila_esta_vazia(&fila) == 1);

    // Verifica se destruir apaga apenas os nos e nao os atendimentos
    assert(atendimento1->codigo == 1UL);
    assert(atendimento2->codigo == 2UL);

    printf("Atendimentos preservados: OK\n");

    
    fila_destruir(&fila);
    fila_destruir(&fila_vazia);
    fila_destruir(NULL);

    atendimento_destruir(atendimento1);
    atendimento_destruir(atendimento2);
    atendimento_destruir(atendimento3);

    atendimento1 = NULL;
    atendimento2 = NULL;
    atendimento3 = NULL;
    removido = NULL;

    printf("Destruicao: OK\n");

    printf(
        "Todos os testes da fila passaram!\n"
    );

    return 0;
}