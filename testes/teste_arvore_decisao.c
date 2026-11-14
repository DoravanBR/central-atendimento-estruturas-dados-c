/*==================================== INCLUSOES =====================================================*/

#include <assert.h>
#include <stdio.h>

#include "arvore_decisao.h"

int main(void)
{
    ArvoreDecisao arvore;
    Classificacao resultado;
    FILE *arquivo_entrada;

    /*
      Substitui o teclado pelo arquivo contendo as respostas dos quatro percursos.
    */
    arquivo_entrada = freopen("testes/dados_arvore_decisao.txt", "r", stdin);

    assert(arquivo_entrada != NULL);

/*==================================== TESTES =====================================================*/

    printf(
        "Iniciando testes da arvore de decisao...\n"
    );

/*==== CASOS NULOS ===============================================================================*/

    printf("Testando argumentos nulos...\n");

    assert(arvore_decisao_inicializar(NULL) == 0);
    assert(arvore_decisao_contar_nos(NULL) == 0UL);
    assert(arvore_decisao_contar_folhas(NULL) == 0UL);
    assert(arvore_decisao_obter_altura(NULL) == -1);

    resultado = CLASSIFICACAO_CRITICA;

    assert(arvore_decisao_classificar(NULL, &resultado) == 0);

    /*
      Mesmo na falha, a saída deve ser reiniciada para CLASSIFICACAO_NAO_DEFINIDA.
     */
    assert(resultado == CLASSIFICACAO_NAO_DEFINIDA);

    printf("Argumentos nulos: OK\n");

/*==== INICIALIZACAO ============================================================================*/

    printf("Testando construcao da arvore...\n");

    assert(arvore_decisao_inicializar(&arvore) == 1);

    // Confirma os metadados gerais. 
    assert(arvore.raiz != NULL);
    assert(arvore_decisao_contar_nos(&arvore ) == 7UL);
    assert(arvore_decisao_contar_folhas(&arvore) == 4UL);
    assert(arvore_decisao_obter_altura(&arvore) == 2);

    // A raiz é a primeira pergunta. 
    assert(arvore.raiz->tipo == NO_DECISAO_PERGUNTA);
    assert(arvore.raiz->classificacao == CLASSIFICACAO_NAO_DEFINIDA);
    assert(arvore.raiz->resposta_sim != NULL);
    assert(arvore.raiz->resposta_nao != NULL);

    // Os dois filhos da raiz também são perguntas. 
    assert(arvore.raiz->resposta_sim->tipo == NO_DECISAO_PERGUNTA);
    assert(arvore.raiz->resposta_nao->tipo == NO_DECISAO_PERGUNTA);   
    
    /*
      Caminho sim/sim: indisponível e afeta várias pessoas → crítica.
    */
    assert(arvore.raiz->resposta_sim->resposta_sim ->classificacao == CLASSIFICACAO_CRITICA);

    /*
      Caminho sim/não: indisponível, mas não afeta várias pessoas → urgente.
    */
    assert(arvore.raiz->resposta_sim->resposta_nao->classificacao == CLASSIFICACAO_URGENTE);

    /*
      Caminho não/sim: não está totalmente indisponível, mas impede o trabalho → moderada.
    */
    assert(arvore.raiz->resposta_nao->resposta_sim->classificacao == CLASSIFICACAO_MODERADA);

    /*
      Caminho não/não: não está indisponível e não impede o trabalho → comum.
    */
    assert(arvore.raiz->resposta_nao->resposta_nao->classificacao == CLASSIFICACAO_COMUM);

    printf("Construcao da arvore: OK\n");    

/*==== TESTANDO CLASSIFICACAO ====================================================================*/

    // Valida Critica
    printf("Testando classificacao critica...\n");

    /*
      Respostas consumidas do arquivo:
      1 → serviço totalmente indisponível;
      1 → afeta várias pessoas.
    */
    resultado = CLASSIFICACAO_NAO_DEFINIDA;

    assert(arvore_decisao_classificar(&arvore, &resultado) == 1);
    assert(resultado == CLASSIFICACAO_CRITICA);

    printf("Classificacao critica: OK\n");

    // Valida Urgente
    printf("Testando classificacao urgente...\n");

    /*
      Próximas respostas:
      1 → serviço totalmente indisponível;
      0 → não afeta várias pessoas.
    */
    resultado = CLASSIFICACAO_NAO_DEFINIDA;

    assert(arvore_decisao_classificar(&arvore, &resultado) == 1);
    assert(resultado == CLASSIFICACAO_URGENTE);

    printf("Classificacao urgente: OK\n");    

    // Valida Moderado
    printf("Testando classificacao moderada...\n");

    /*
      Próximas respostas:
      0 → serviço não está totalmente indisponível;
      1 → impede a realização do trabalho.
    */
    resultado = CLASSIFICACAO_NAO_DEFINIDA;

    assert(arvore_decisao_classificar(&arvore, &resultado) == 1);
    assert(resultado == CLASSIFICACAO_MODERADA);

    printf("Classificacao moderada: OK\n");

    // Valida Comum
    printf("Testando classificacao comum...\n");

    /*
      Últimas respostas:
      0 → serviço não está totalmente indisponível;
      0 → não impede a realização do trabalho.
    */
    resultado = CLASSIFICACAO_NAO_DEFINIDA;

    assert(arvore_decisao_classificar(&arvore, &resultado) == 1);
    assert(resultado == CLASSIFICACAO_COMUM);

    printf("Classificacao comum: OK\n");

/*==== EXIBICAO ====================================================================*/  

    printf("Testando exibicao...\n");

    // Uma árvore nula deve ser tratada com segurança. 
    arvore_decisao_exibir(NULL);

    // Exibe perguntas, caminhos e resultados. 
    arvore_decisao_exibir(&arvore);

    // A exibição não pode modificar a estrutura. 
    assert(arvore_decisao_contar_nos(&arvore) == 7UL);
    assert(arvore_decisao_contar_folhas(&arvore) == 4UL);
    assert(arvore_decisao_obter_altura(&arvore) == 2);

    printf("Exibicao: OK\n");

    /*
      Sem uma variável de saída, a classificação deve falhar antes de tentar ler outra resposta.
    */
    assert(arvore_decisao_classificar(&arvore, NULL) == 0);

/*==== DESTRUIR ====================================================================*/

    printf("Testando destruicao...\n");

    // Libera os sete nós da árvore. 
    arvore_decisao_destruir(&arvore);

    assert(arvore.raiz == NULL);
    assert(arvore.quantidade_nos == 0UL);

    assert(arvore_decisao_contar_nos(&arvore) == 0UL);
    assert(arvore_decisao_contar_folhas(&arvore) == 0UL);
    assert(arvore_decisao_obter_altura(&arvore) == -1);

    // Chamadas repetidas e nulas devem ser seguras. 
    arvore_decisao_destruir(&arvore);
    arvore_decisao_destruir(NULL);

    printf("Destruicao: OK\n");

    printf(
        "Todos os testes da arvore de decisao passaram!\n"
    );

    return 0;
}