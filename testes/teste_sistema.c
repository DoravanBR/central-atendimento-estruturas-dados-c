/*========================================= INCLUSOES =============================================*/

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "sistema.h"

/*========================================= DEFINICOES =============================================*/

#define QUANTIDADE_ATENDIMENTOS 4UL

/*========================================= TESTES =============================================*/
int main(void)
{
    Sistema sistema;

    Atendimento *atendimentos[QUANTIDADE_ATENDIMENTOS] = {NULL};
    Atendimento *encontrado;
    Atendimento *chamado;

    const RegistroHistorico *topo_historico;

    Classificacao classificacao;
    ResultadoSistema resultado;

    unsigned long comparacoes;
    unsigned long i;

    FILE *arquivo_entrada;

    /*
      Substitui o teclado pelas respostas usadas nas quatro classificações.
     */
    arquivo_entrada = freopen("testes/dados_sistema.txt", "r", stdin);

    assert(arquivo_entrada != NULL);

    printf("Iniciando teste integrado do sistema...\n");

/*=== INICIALIZACAO ==================================================================*/
    printf("Testando inicializacao integrada...\n");

    assert(sistema_inicializar(NULL) == RESULTADO_SISTEMA_ARGUMENTO_INVALIDO);
    assert(sistema_inicializar(&sistema) == RESULTADO_SISTEMA_SUCESSO);

    assert(sistema.inicializado == 1);
    assert(sistema.proximo_codigo == 1UL);
    assert(sistema.proxima_ordem == 1UL);

    assert(repositorio_obter_quantidade(&sistema.repositorio) == 0UL);
    assert(sistema_validar(&sistema) == RESULTADO_SISTEMA_SUCESSO);

    printf("Inicializacao integrada: OK\n");

/*=== FILA E HEAP VAZIO ==================================================================*/

    printf("Testando estruturas de atendimento vazias...\n");

    chamado = (Atendimento *)1;

    resultado = sistema_chamar_proximo_fila(&sistema, &chamado);

    assert(resultado == RESULTADO_SISTEMA_FILA_VAZIA);

    assert(chamado == NULL);

    chamado = (Atendimento *)1;

    resultado = sistema_chamar_prioritario(&sistema, &chamado);

    assert(resultado == RESULTADO_SISTEMA_HEAP_VAZIO);

    assert(chamado == NULL);

    printf("Estruturas vazias: OK\n");

/*=== CADASTRO INVALIDO ==================================================================*/

    printf("Testando cadastros invalidos...\n");

    /* Prioridade fora do intervalo deve ser rejeitada. */
    resultado = sistema_cadastrar(&sistema, "Usuario invalido", "Prioridade abaixo do minimo", 0, &atendimentos[0]);

    assert(resultado == RESULTADO_SISTEMA_PRIORIDADE_INVALIDA);

    assert(atendimentos[0] == NULL);

    /* Texto nulo deve ser rejeitado como argumento inválido. */
    resultado = sistema_cadastrar(&sistema, NULL, "Descricao", 5, &atendimentos[0]);

    assert(resultado == RESULTADO_SISTEMA_ARGUMENTO_INVALIDO);

    /*
      Tentativas inválidas não podem consumir código nem ordem.
    */
    assert(sistema.proximo_codigo == 1UL);
    assert(sistema.proxima_ordem == 1UL);
    assert(repositorio_obter_quantidade(&sistema.repositorio) == 0UL);

    printf("Cadastros invalidos: OK\n");

/*=== CADASTRO VALIDO ==================================================================*/

    const char *solicitantes[QUANTIDADE_ATENDIMENTOS] = {"Ana", "Bruno", "Carla", "Daniel"};
    const char *descricoes[QUANTIDADE_ATENDIMENTOS] = {"Solicitacao comum", "Problema moderado",
         "Problema urgente", "Problema critico"};
    const int prioridades[QUANTIDADE_ATENDIMENTOS] = {3, 5, 8, 10};

     printf("Testando cadastros validos...\n");

    for (i = 0UL; i < QUANTIDADE_ATENDIMENTOS; i++) {
        resultado = sistema_cadastrar(&sistema, solicitantes[i], descricoes[i], prioridades[i],
             &atendimentos[i]);

        assert(resultado == RESULTADO_SISTEMA_SUCESSO);

        assert(atendimentos[i] != NULL);

        /* Código e ordem são sequenciais e positivos. */
        assert(atendimentos[i]->codigo == i + 1UL);

        assert(atendimentos[i]->ordem_chegada == i + 1UL);

        assert(atendimentos[i]->status == STATUS_ABERTO);

        assert(atendimentos[i]->classificacao == CLASSIFICACAO_NAO_DEFINIDA);
    }   

    assert(sistema.proximo_codigo == 5UL);
    assert(sistema.proxima_ordem == 5UL);

    assert(repositorio_obter_quantidade(&sistema.repositorio) == 4UL);

    assert(bst_obter_tamanho(&sistema.bst) == 4UL);

    assert(avl_obter_tamanho(&sistema.avl) == 4UL);

    /* Um registro de cadastro para cada atendimento. */
    assert(pilha_obter_tamanho(&sistema.historico) == 4UL);

    assert(sistema_validar(&sistema) == RESULTADO_SISTEMA_SUCESSO);

    printf("Cadastros validos: OK\n");

/*=== BUSCAS ==================================================================*/

    printf("Testando buscas integradas...\n");

    for (i = 0UL; i < QUANTIDADE_ATENDIMENTOS; i++) {
        unsigned long codigo;

        codigo = i + 1UL;

        /* Busca pela fonte central. */
        encontrado = NULL;

        resultado = sistema_buscar(&sistema, codigo, &encontrado);

        assert(resultado == RESULTADO_SISTEMA_SUCESSO);

        assert(encontrado == atendimentos[i]);

        /* Busca pelo índice BST. */
        encontrado = NULL;
        comparacoes = 0UL;

        resultado = sistema_buscar_bst(&sistema, codigo, &encontrado, &comparacoes);

        assert(resultado == RESULTADO_SISTEMA_SUCESSO);

        assert(encontrado == atendimentos[i]);

        assert(comparacoes > 0UL);

        /* Busca pelo índice AVL. */
        encontrado = NULL;
        comparacoes = 0UL;

        resultado = sistema_buscar_avl(&sistema, codigo, &encontrado, &comparacoes);

        assert(resultado == RESULTADO_SISTEMA_SUCESSO);

        assert(encontrado == atendimentos[i]);

        assert(comparacoes > 0UL);
    }

    printf("Identidade dos ponteiros: OK\n");

    /*============  BUSCA INEXISTENTE =============*/

    printf("Testando busca inexistente...\n");

    encontrado = (Atendimento *)1;

    resultado = sistema_buscar(&sistema, 999UL, &encontrado);

    assert(resultado == RESULTADO_SISTEMA_NAO_ENCONTRADO);

    assert(encontrado == NULL);

    encontrado = (Atendimento *)1;
    comparacoes = 99UL;

    resultado = sistema_buscar_bst( &sistema, 999UL, &encontrado, &comparacoes);

    assert(resultado == RESULTADO_SISTEMA_NAO_ENCONTRADO);

    assert(encontrado == NULL);
    assert(comparacoes > 0UL);

    encontrado = (Atendimento *)1;
    comparacoes = 99UL;

    resultado = sistema_buscar_avl(&sistema, 999UL, &encontrado, &comparacoes);

    assert(resultado == RESULTADO_SISTEMA_NAO_ENCONTRADO);

    assert(encontrado == NULL);
    assert(comparacoes > 0UL);

    printf("Busca inexistente: OK\n");

/*=== CLASSIFICACAO ==================================================================*/

    const Classificacao classificacoes_esperadas[QUANTIDADE_ATENDIMENTOS] = {CLASSIFICACAO_COMUM,
         CLASSIFICACAO_MODERADA, CLASSIFICACAO_URGENTE, CLASSIFICACAO_CRITICA};

    printf("Testando classificacoes integradas...\n");

    for (i = 0UL; i < QUANTIDADE_ATENDIMENTOS; i++) {
        classificacao = CLASSIFICACAO_NAO_DEFINIDA;

        resultado = sistema_classificar(&sistema, i + 1UL, &classificacao);

        assert(resultado == RESULTADO_SISTEMA_SUCESSO);

        assert(classificacao == classificacoes_esperadas[i]);

        assert(atendimentos[i]->classificacao == classificacoes_esperadas[i]);

        assert(atendimentos[i]->status == STATUS_CLASSIFICADO);
    }

    /*
      Comum e moderado entram na fila, preservando a ordem 1 → 2.
    */
    assert(fila_obter_tamanho(&sistema.fila) == 2UL);

    assert(fila_consultar_inicio(&sistema.fila) == atendimentos[0]);

    assert(sistema.fila.inicio->proximo->atendimento == atendimentos[1]);

    /*
      Urgente e crítico entram no heap.
      O código 4 tem prioridade 10 e fica na raiz.
    */
    assert(heap_obter_tamanho(&sistema.heap) == 2UL);

    assert(heap_consultar_raiz(&sistema.heap) == atendimentos[3]);

    /*
      4 cadastros + 4 classificações.
    */
    assert(pilha_obter_tamanho(&sistema.historico) == 8UL);

    assert(sistema_validar(&sistema) == RESULTADO_SISTEMA_SUCESSO);

    printf("Classificacoes integradas: OK\n");

    /*
      Um atendimento já classificado não pode percorrer novamente a árvore de decisão.
    */
    resultado = sistema_classificar(&sistema, 1UL, &classificacao);

    assert(resultado == RESULTADO_SISTEMA_CLASSIFICACAO_DUPLICADA);

    assert(fila_obter_tamanho(&sistema.fila) == 2UL);

    assert(pilha_obter_tamanho(&sistema.historico) == 8UL);

    printf("Classificacao duplicada: OK\n");   
    
/*=== EXECUCAO FILA ==================================================================*/

    printf("Testando chamadas da fila...\n");

    /* O código 1 entrou primeiro e deve sair primeiro. */
    chamado = NULL;

    resultado = sistema_chamar_proximo_fila(&sistema, &chamado);

    assert(resultado == RESULTADO_SISTEMA_SUCESSO);

    assert(chamado == atendimentos[0]);

    assert(chamado->status == STATUS_EM_ATENDIMENTO);

    assert(fila_obter_tamanho(&sistema.fila) == 1UL);

    assert(pilha_obter_tamanho(&sistema.historico) == 9UL);

    /* O código 2 era o segundo da fila. */
    chamado = NULL;

    resultado = sistema_chamar_proximo_fila(&sistema,&chamado);

    assert(resultado == RESULTADO_SISTEMA_SUCESSO);

    assert(chamado == atendimentos[1]);

    assert(chamado->status == STATUS_EM_ATENDIMENTO);

    assert(fila_obter_tamanho(&sistema.fila) == 0UL);

    assert(pilha_obter_tamanho(&sistema.historico) == 10UL);

    /*======  FILA VAZIA ======*/
    chamado = (Atendimento *)1;

    resultado = sistema_chamar_proximo_fila(&sistema, &chamado);

    assert(resultado == RESULTADO_SISTEMA_FILA_VAZIA);

    assert(chamado == NULL);

    /*
      A tentativa vazia não gera histórico.
    */
    assert(pilha_obter_tamanho(&sistema.historico) == 10UL);

    assert(sistema_validar(&sistema) == RESULTADO_SISTEMA_SUCESSO);

    printf("Chamadas da fila: OK\n");    

/*=== EXECUCAO HEAP ==================================================================*/

    printf("Testando chamadas do heap...\n");

    /*
      O código 4 é crítico e possui prioridade 10.
      Deve sair antes do código 3, prioridade 8.
    */
    chamado = NULL;

    resultado = sistema_chamar_prioritario(&sistema, &chamado);

    assert(resultado == RESULTADO_SISTEMA_SUCESSO);

    assert(chamado == atendimentos[3]);

    assert(chamado->status == STATUS_EM_ATENDIMENTO);

    assert(heap_obter_tamanho( &sistema.heap) == 1UL);

    assert(pilha_obter_tamanho(&sistema.historico) == 11UL);

    /*
      Depois da remoção da prioridade 10, o código 3 torna-se a raiz.
    */
    chamado = NULL;

    resultado = sistema_chamar_prioritario(&sistema, &chamado);

    assert(resultado == RESULTADO_SISTEMA_SUCESSO);

    assert(chamado == atendimentos[2]);

    assert(chamado->status == STATUS_EM_ATENDIMENTO);

    assert(heap_obter_tamanho(&sistema.heap) == 0UL);

    assert(pilha_obter_tamanho(&sistema.historico) == 12UL);  
    
    chamado = (Atendimento *)1;

    resultado = sistema_chamar_prioritario(&sistema, &chamado);

    assert(resultado == RESULTADO_SISTEMA_HEAP_VAZIO);

    assert(chamado == NULL);

    /*
      A tentativa vazia não gera evento.
    */
    assert(pilha_obter_tamanho(&sistema.historico) == 12UL);

    assert(sistema_validar(&sistema) == RESULTADO_SISTEMA_SUCESSO);

    printf("Chamadas do heap: OK\n");

/*=== CONCLUIR ==================================================================*/

    printf("Testando conclusoes...\n");

    for (i = 0UL; i < QUANTIDADE_ATENDIMENTOS; i++) {
        resultado = sistema_concluir(&sistema, i + 1UL);

        assert(resultado == RESULTADO_SISTEMA_SUCESSO);

        assert(atendimentos[i]->status == STATUS_CONCLUIDO);
    }

    /*
     * Eventos esperados:
     * 4 cadastros
     * 4 classificações
     * 4 inícios de atendimento
     * 4 conclusões
     * total = 16
     */
    assert(pilha_obter_tamanho(&sistema.historico) == 16UL);

    topo_historico = pilha_consultar_topo(&sistema.historico);

    assert(topo_historico != NULL);
    assert(topo_historico->sequencia == 16UL);
    assert(topo_historico->codigo_atendimento == 4UL);

    assert(strcmp(topo_historico->operacao, "Atendimento concluido") == 0);

    /*
      Um atendimento concluído não pode ser concluído novamente.
    */
    resultado = sistema_concluir(&sistema, 1UL);

    assert(resultado == RESULTADO_SISTEMA_STATUS_INVALIDO);

    assert(pilha_obter_tamanho(&sistema.historico) == 16UL);

    printf("Conclusoes: OK\n");

    /* === CONFIRMAR CONCLUIDOS AINDA PESQUISAVEIS ===*/
    for (i = 0UL; i < QUANTIDADE_ATENDIMENTOS; i++) {
        encontrado = NULL;

        resultado = sistema_buscar(&sistema, i + 1UL, &encontrado);

        assert(resultado == RESULTADO_SISTEMA_SUCESSO);

        assert(encontrado == atendimentos[i]);

        assert(encontrado->status == STATUS_CONCLUIDO);
    }

    assert(sistema_validar(&sistema) == RESULTADO_SISTEMA_SUCESSO);

/*=== CONVERSAO PARA TEXTO ==================================================================*/

    printf("Testando mensagens de resultado...\n");

    assert(strcmp(resultado_sistema_para_texto(RESULTADO_SISTEMA_SUCESSO), "sucesso") == 0);
    assert(strcmp(resultado_sistema_para_texto(RESULTADO_SISTEMA_FILA_VAZIA), "fila vazia") == 0);
    assert(strcmp(resultado_sistema_para_texto(RESULTADO_SISTEMA_HEAP_VAZIO), "heap vazio") == 0);
    assert(strcmp(resultado_sistema_para_texto((ResultadoSistema)999), "resultado desconhecido") == 0);

    printf("Mensagens de resultado: OK\n");

/*=== DESTRUIR SISTEMA ==================================================================*/

    printf("Testando destruicao integrada...\n");

    /*
     * Não destruímos os atendimentos manualmente.
     * O repositório é o proprietário e será
     * destruído por último dentro do sistema.
     */
    sistema_destruir(&sistema);

    assert(sistema.inicializado == 0);
    assert(sistema.proximo_codigo == 0UL);
    assert(sistema.proxima_ordem == 0UL);

    assert(sistema.repositorio.itens == NULL);
    assert(sistema.repositorio.quantidade == 0UL);

    assert(sistema.fila.inicio == NULL);
    assert(sistema.fila.fim == NULL);

    assert(sistema.heap.itens == NULL);
    assert(sistema.bst.raiz == NULL);
    assert(sistema.avl.raiz == NULL);
    assert(sistema.historico.topo == NULL);
    assert(sistema.arvore_decisao.raiz == NULL);

    for (i = 0UL; i < QUANTIDADE_ATENDIMENTOS; i++) {
        atendimentos[i] = NULL;
    }

    encontrado = NULL;
    chamado = NULL;
    topo_historico = NULL;

    encontrado = (Atendimento *)1;

    resultado = sistema_buscar(&sistema, 1UL, &encontrado);

    assert(resultado == RESULTADO_SISTEMA_ESTRUTURA_INVALIDA);

    assert(encontrado == NULL);

    sistema_destruir(&sistema);
    sistema_destruir(NULL);

    printf("Destruicao integrada: OK\n");

    printf("Todos os testes integrados passaram!\n");

    return 0;
}