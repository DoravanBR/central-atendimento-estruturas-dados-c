/*========================================= INCLUSOES =============================================*/
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "pilha.h"

/*========================================= TESTES =============================================*/
int main(void)
{
    PilhaHistorico pilha;
    const RegistroHistorico *topo;
    RegistroHistorico removido;

    printf("Iniciando testes da pilha...\n");

/*=== INICIALIZACAO =============================================================================*/

    printf("Testando inicializacao...\n");
    pilha_inicializar(NULL);
    assert(pilha_esta_vazia(NULL) == 1);
    assert(pilha_obter_tamanho(NULL) == 0UL);
    assert(pilha_consultar_topo(NULL) == NULL);

    pilha_inicializar(&pilha);
    
    assert(pilha.topo == NULL);
    assert(pilha.tamanho == 0UL);
    assert(pilha.proxima_sequencia == 1UL);

    assert(pilha_esta_vazia(&pilha) == 1);
    assert(pilha_obter_tamanho(&pilha) == 0UL);
    assert(pilha_consultar_topo(&pilha) == NULL);

    printf("Inicializacao: OK\n");

/*=== EMPILHAR INVALIDOS ========================================================================*/

    char operacao_grande[TAM_OPERACAO + 1];

    printf("Testando empilhamentos invalidos...\n");

    memset(operacao_grande, 'A', TAM_OPERACAO);
    operacao_grande[TAM_OPERACAO] = '\0';

    assert(pilha_empilhar(NULL, 1UL, "Evento") == 0); // Pilha nula
    assert(pilha_empilhar(&pilha, 0UL, "Evento") == 0); // codigo invalido
    assert(pilha_empilhar(&pilha, 1UL, NULL) == 0); // operação nula
    assert(pilha_empilhar(&pilha, 1UL, "") == 0); // operacao vazia
    assert(pilha_empilhar(&pilha, 1UL, operacao_grande) == 0); // capacidade do vetor excedida
    
    //Confirmando que não incluiu nenhum registro invalido na pilha
    assert(pilha.topo == NULL); 
    assert(pilha.tamanho == 0UL);
    assert(pilha.proxima_sequencia == 1UL);

    printf("Empilhamentos invalidos: OK\n");

/*=== EMPILHAR VALIDOS ========================================================================*/

    printf("Testando primeiro empilhamento...\n");

    assert(pilha_empilhar(&pilha, 10UL, "Atendimento cadastrado") == 1);// Testando insercao
    assert(pilha_esta_vazia(&pilha) == 0); // Testando se a pilha existe e esta vazia (0 = não vazia)
    assert(pilha_obter_tamanho(&pilha) == 1UL);// Testando se tamnaho == 1
    assert(pilha.proxima_sequencia == 2UL);// Testando se proxima sequencia == 2

    topo = pilha_consultar_topo(&pilha);

    assert(topo != NULL); // Testa se topo é != de NULL
    assert(topo->sequencia == 1UL); // Testa se sequencia é = 1
    assert(topo->codigo_atendimento == 10UL);  // Testa se codigo é = 10
    assert(strcmp(topo->operacao, "Atendimento cadastrado") == 0); // Testa comparacao de characteres 

    assert(pilha.topo->proximo == NULL);// Testa se para o primeiro registro o ptr para proximo é nulo

    printf("Primeiro empilhamento: OK\n");

/*=== EMPILHAR MULTIPLOS ========================================================================*/
    
    printf("Testando multiplos empilhamentos...\n");

    assert(pilha_empilhar(&pilha, 10UL, "Atendimento classificado") == 1); // Testa inserir atualizacao operacao
    assert(pilha_empilhar(&pilha, 20UL,"Atendimento cadastrado") == 1); // Testa inserir novo atendimento
    assert(pilha_obter_tamanho(&pilha) == 3UL); // Testa se tamanho = 3 (1 anterior + 2 registros novos)
    assert(pilha.proxima_sequencia == 4UL);// Testa se a proxima sequencia é 4

    topo = pilha_consultar_topo(&pilha);

    assert(topo != NULL);
    assert(topo->sequencia == 3UL);
    assert(topo->codigo_atendimento == 20UL);
    assert(strcmp(topo->operacao, "Atendimento cadastrado") == 0);
    
    // Testa se mantem a estrutura de pilha
    assert(pilha.topo->registro.sequencia == 3UL);
    assert(pilha.topo->proximo->registro.sequencia == 2UL);
    assert(pilha.topo->proximo->proximo->registro.sequencia == 1UL);
    assert(pilha.topo->proximo->proximo->proximo == NULL);

    printf("Multiplos empilhamentos: OK\n");    

/*=== DESEMPILHAMENTO COM COPIA ======================================================================*/

    printf("Testando desempilhamento com copia...\n");

    assert(pilha_desempilhar(&pilha, &removido) == 1); // Testa se desempilhar obtem sucesso

    topo = NULL;
    
    //Verifica se o nó removido foi o correto
    assert(removido.sequencia == 3UL);
    assert(removido.codigo_atendimento == 20UL);
    assert(strcmp(removido.operacao, "Atendimento cadastrado") == 0);

    assert(pilha_obter_tamanho(&pilha) == 2UL);// Verifica se tamanho diminuiu
    assert(pilha.proxima_sequencia == 4UL);// Verifica se prox. sequencia se mantem

    topo = pilha_consultar_topo(&pilha);

    //Verifica os dados do novo topo
    assert(topo != NULL);
    assert(topo->sequencia == 2UL);
    assert(topo->codigo_atendimento == 10UL);

    printf("Desempilhamento com copia: OK\n");

/*=== DESEMPILHAMENTO SEM COPIA ======================================================================*/
    
    printf("Testando desempilhamento sem copia...\n");
    assert(pilha_desempilhar(&pilha, NULL) == 1); // Testa desempilhar com param. de copia nulo

    assert(pilha_obter_tamanho(&pilha) == 1UL); // Verifica atualização do tamanho
    assert(pilha.proxima_sequencia == 4UL);// Verifica se prox. sequencia se mantem

    topo = pilha_consultar_topo(&pilha);

    assert(topo != NULL);
    assert(topo->sequencia == 1UL);

    printf("Desempilhamento sem copia: OK\n");

/*=== CONTINUIDADE DE SEQUENCIA ===================================================================*/

    printf("Testando continuidade da sequencia...\n");

    assert(pilha_empilhar( &pilha, 10UL, "Atendimento iniciado") == 1);// Testa novo empilhamento

    assert(pilha_obter_tamanho(&pilha) == 2UL);//Verifica atualizacao de tamanho
    assert(pilha.proxima_sequencia == 5UL);// Verifica atualizacao de sequencia

    topo = pilha_consultar_topo(&pilha);

    // Verifica os dados do novo topo
    assert(topo != NULL);
    assert(topo->sequencia == 4UL);
    assert(topo->codigo_atendimento == 10UL);
    assert(strcmp(topo->operacao, "Atendimento iniciado") == 0);
    assert(pilha.topo->proximo->registro.sequencia == 1UL);

    printf("Continuidade da sequencia: OK\n");

/*=== EXIBICAO ===================================================================*/

    PilhaHistorico pilha_vazia;

    printf("Testando exibicao...\n");

    pilha_exibir(NULL);// Verifica exibir mensagem para pilha inexitente

    pilha_inicializar(&pilha_vazia); // inicializa uma pilha vazia
    pilha_exibir(&pilha_vazia); // Verifica mensagem para pilha vazia

    pilha_exibir(&pilha); // imprime a pilha

    printf("Exibicao executada: OK\n");

/*=== DESTRUIR ===================================================================*/

    printf("Testando destruicao...\n");

    pilha_destruir(&pilha);//executa funcao destruir pilha

    topo = NULL;

    //Verifica se a pilha esta no estado inicial (vazia)
    assert(pilha.topo == NULL);
    assert(pilha.tamanho == 0UL);
    assert(pilha.proxima_sequencia == 1UL);

    assert(pilha_esta_vazia(&pilha) == 1);
    assert(pilha_consultar_topo(&pilha) == NULL);

    assert(pilha_desempilhar(&pilha, &removido) == 0); // Verifica desempilhar para pilha vazia

    pilha_destruir(&pilha);
    pilha_destruir(&pilha_vazia);
    pilha_destruir(NULL);

    assert(pilha_vazia.topo == NULL);
    assert(pilha_vazia.tamanho == 0UL);

    assert(pilha_vazia.proxima_sequencia == 1UL);

    printf("Destruicao: OK\n");

    printf("Todos os testes da pilha passaram!\n");

    return 0;
}