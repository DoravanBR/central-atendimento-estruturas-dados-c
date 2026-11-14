/*========================================= INCLUSOES =============================================*/
#include <assert.h>
#include <stdio.h>

#include "avl.h"

/*========================================= DEFINICOES =============================================*/

#define QUANTIDADE_CRESCENTE 7UL

/*=== DADOS SIMULADOS PARA TESTE ==================================================================*/

static Atendimento *criar_atendimento_teste(unsigned long codigo){
    return atendimento_criar(codigo, "Usuario de teste", "Atendimento usado no teste da AVL", 5, codigo);
}

/*========================================= TESTES =============================================*/

int main(void)
{
    ArvoreAVL arvore;
    ArvoreAVL arvore_vazia;
    Atendimento *atendimento1;
    Atendimento *atendimento2;
    Atendimento *atendimento3;
    Atendimento *encontrado;
    Atendimento *duplicado;
    Atendimento *atendimentos_crescentes[QUANTIDADE_CRESCENTE];

    unsigned long i;
    unsigned long comparacoes;

    printf("Iniciando testes da AVL...\n");

/*=== INICIALIZACAO ==================================================================*/

    printf("Testando inicializacao...\n");

    avl_inicializar(NULL);

    assert(avl_esta_vazia(NULL) == 1);
    assert(avl_obter_tamanho(NULL) == 0UL);
    assert(avl_obter_rotacoes(NULL) == 0UL);
    assert(avl_obter_altura(NULL) == -1);

    avl_inicializar(&arvore);

    assert(arvore.raiz == NULL);
    assert(arvore.tamanho == 0UL);
    assert(arvore.rotacoes == 0UL);

    assert(avl_esta_vazia(&arvore) == 1);
    assert(avl_obter_tamanho(&arvore) == 0UL);
    assert(avl_obter_rotacoes(&arvore) == 0UL);
    assert(avl_obter_altura(&arvore) == -1);

    assert( avl_obter_fator_balanceamento(NULL) == 0);

    printf("Inicializacao: OK\n");

/*=== OPERACOES INVALIDAS ==================================================================*/

    printf("Testando operacoes invalidas...\n");

    atendimento1 = criar_atendimento_teste(30UL);

    assert(atendimento1 != NULL);// Verifica se nao houve falha na criacao do atendimento

    assert(avl_inserir(NULL, atendimento1) == 0);
    assert(avl_inserir(&arvore, NULL) == 0);

    assert(arvore.raiz == NULL);
    assert(arvore.tamanho == 0UL);
    assert(arvore.rotacoes == 0UL);

    // Busca Invalida

    comparacoes = 99UL;

    encontrado = avl_buscar(NULL, 30UL,&comparacoes);// Arvore vazia

    assert(encontrado == NULL);
    assert(comparacoes == 0UL);

    comparacoes = 99UL;

    encontrado = avl_buscar(&arvore, 0UL, &comparacoes);

    assert(encontrado == NULL);
    assert(comparacoes == 0UL);

    comparacoes = 99UL;

    encontrado = avl_buscar( &arvore, 30UL, &comparacoes);

    assert(encontrado == NULL);
    assert(comparacoes == 0UL);

    printf("Operacoes invalidas: OK\n");

/*=== ROTACAO LL ==================================================================*/

    printf("Testando rotacao LL...\n");

    // Cria a sequencia decrescente 30, 20, 10. 
    atendimento2 = criar_atendimento_teste(20UL);
    atendimento3 = criar_atendimento_teste(10UL);

    assert(atendimento2 != NULL);
    assert(atendimento3 != NULL);

    // Insere os valores que provocam excesso à esquerda. 
    assert(avl_inserir(&arvore, atendimento1) == 1);
    assert(avl_inserir(&arvore, atendimento2) == 1);
    assert(avl_inserir(&arvore, atendimento3) == 1);

    /*
      A rotação à direita deve transformar:
     
            30              20
           /               /  \
          20      em      10   30
         /
        10
     */

    assert(arvore.raiz != NULL);
    assert( arvore.raiz->atendimento->codigo == 20UL);
    assert(arvore.raiz->esquerda->atendimento->codigo == 10UL);
    assert(arvore.raiz->direita->atendimento->codigo == 30UL);

    // Confirma tamanho, altura e uma rotação primitiva. 
    assert(avl_obter_tamanho(&arvore) == 3UL);
    assert(avl_obter_altura(&arvore) == 1);
    assert(avl_obter_rotacoes(&arvore) == 1UL);

    // Confirma as alturas armazenadas nos três nós. 
    assert(arvore.raiz->altura == 1);
    assert(arvore.raiz->esquerda->altura == 0);
    assert(arvore.raiz->direita->altura == 0);

    // A árvore resultante deve estar balanceada. 
    assert(avl_obter_fator_balanceamento( arvore.raiz) == 0);
    assert(avl_obter_fator_balanceamento(arvore.raiz->esquerda) == 0);
    assert(avl_obter_fator_balanceamento(arvore.raiz->direita) == 0);

    printf("Rotacao LL: OK\n");

/*=== BUSCA ==================================================================*/
    
    printf("Testando buscas na AVL...\n");

    // Busca a raiz: somente um nó deve ser visitado. 
    encontrado = avl_buscar(&arvore, 20UL, &comparacoes);

    assert(encontrado == atendimento2);
    assert(comparacoes == 1UL);

    // Busca uma folha: visita a raiz e o filho esquerdo. 
    encontrado = avl_buscar(&arvore, 10UL, &comparacoes);

    assert(encontrado == atendimento3);
    assert(comparacoes == 2UL);

    // Busca inexistente: percorre 20, 30 e termina em NULL. 
    encontrado = avl_buscar(&arvore, 99UL, &comparacoes);

    assert(encontrado == NULL);
    assert(comparacoes == 2UL);

    // A busca também deve funcionar sem contador. 
    encontrado = avl_buscar(&arvore, 30UL, NULL);

    assert(encontrado == atendimento1);

    printf("Buscas: OK\n");

/*=== DUPLICIDADE ==================================================================*/

    printf("Testando codigo duplicado...\n");

    // Cria outro objeto, mas com uma chave já existente. 
    duplicado = criar_atendimento_teste(20UL);

    assert(duplicado != NULL);

    // A AVL deve rejeitar a chave repetida. 
    assert(avl_inserir( &arvore, duplicado) == 0);

    // Duplicidade não pode alterar os metadados. 
    assert(avl_obter_tamanho(&arvore) == 3UL);
    assert(avl_obter_rotacoes(&arvore) == 1UL);
    assert(avl_obter_altura(&arvore) == 1);

    /*
      A inserção falhou, então o objeto duplicado continua pertencendo ao teste.
    */
    atendimento_destruir(duplicado);
    duplicado = NULL;

    printf("Codigo duplicado: OK\n");

/*=== FINALIZANDO ROTACAO LL =============================================================*/

    printf("Finalizando teste LL...\n");

    /*
      Destrói apenas os nós da AVL.
      Os atendimentos devem continuar válidos.
    */
    avl_destruir(&arvore);

    assert(arvore.raiz == NULL);
    assert(arvore.tamanho == 0UL);
    assert(arvore.rotacoes == 0UL);
    assert(avl_obter_altura(&arvore) == -1);

    // Confirma que a AVL não destruiu os atendimentos. 
    assert(atendimento1->codigo == 30UL);
    assert(atendimento2->codigo == 20UL);
    assert(atendimento3->codigo == 10UL);

    // Agora o teste libera os objetos que ainda possui. 
    atendimento_destruir(atendimento1);
    atendimento_destruir(atendimento2);
    atendimento_destruir(atendimento3);

    atendimento1 = NULL;
    atendimento2 = NULL;
    atendimento3 = NULL;
    encontrado = NULL;

    printf("Finalizacao LL: OK\n");

/*=== ROTACAO RR ==================================================================*/

    printf("Preparando rotacao RR...\n");

    /*
      Sequência crescente:
      10, 20, 30 provoca excesso à direita.
    */
    atendimento1 = criar_atendimento_teste(10UL);
    atendimento2 = criar_atendimento_teste(20UL);
    atendimento3 = criar_atendimento_teste(30UL);

    assert(atendimento1 != NULL);
    assert(atendimento2 != NULL);
    assert(atendimento3 != NULL);

    assert(avl_inserir(&arvore, atendimento1) == 1);
    assert(avl_inserir(&arvore, atendimento2) == 1);
    assert(avl_inserir(&arvore, atendimento3) == 1);

    /*
      A rotação à esquerda deve transformar:
     
        10                  20
          \                /  \
           20     em      10   30
             \
              30
    */
    assert(arvore.raiz != NULL);

    assert(arvore.raiz->atendimento->codigo == 20UL);
    assert(arvore.raiz->esquerda->atendimento->codigo == 10UL);
    assert(arvore.raiz->direita->atendimento->codigo == 30UL);

    // Confirma os metadados resultantes. 
    assert(avl_obter_tamanho(&arvore) == 3UL);
    assert(avl_obter_altura(&arvore) == 1);
    assert(avl_obter_rotacoes(&arvore) == 1UL);

    // Confirma alturas e balanceamento dos nós. 
    assert(arvore.raiz->altura == 1);
    assert(arvore.raiz->esquerda->altura == 0);
    assert(arvore.raiz->direita->altura == 0);
    assert(avl_obter_fator_balanceamento(arvore.raiz) == 0);

    printf("Rotacao RR: OK\n");

    /*
      Libera os nós antes dos atendimentos, respeitando a propriedade da memória.
    */
    avl_destruir(&arvore);

    assert(arvore.raiz == NULL);
    assert(arvore.tamanho == 0UL);
    assert(arvore.rotacoes == 0UL);

    // Os atendimentos permanecem válidos após destruir a AVL. 
    assert(atendimento1->codigo == 10UL);
    assert(atendimento2->codigo == 20UL);
    assert(atendimento3->codigo == 30UL);

    atendimento_destruir(atendimento1);
    atendimento_destruir(atendimento2);
    atendimento_destruir(atendimento3);

    atendimento1 = NULL;
    atendimento2 = NULL;
    atendimento3 = NULL;

    printf("Finalizacao RR: OK\n");

/*=== ROTACAO LR ==================================================================*/  

    printf("Testando rotacao LR...\n");

    /*
      Sequência 30, 10, 20:
     
          30
         /
        10
          \
           20
    */
    atendimento1 = criar_atendimento_teste(30UL);
    atendimento2 = criar_atendimento_teste(10UL);
    atendimento3 = criar_atendimento_teste(20UL);

    assert(atendimento1 != NULL);
    assert(atendimento2 != NULL);
    assert(atendimento3 != NULL);

    // Insere os valores que provocam o caso esquerda-direita. 
    assert(avl_inserir(&arvore, atendimento1) == 1);
    assert(avl_inserir(&arvore, atendimento2) == 1);

    assert(avl_inserir(&arvore, atendimento3) == 1);

    /*
      O rebalanceamento realiza:
     
      1. rotação à esquerda no nó 10;
      2. rotação à direita no nó 30.
     
      Resultado:
     
           20
          /  \
        10    30
    */
    assert(arvore.raiz != NULL);

    assert(arvore.raiz->atendimento->codigo == 20UL);
    assert(arvore.raiz->esquerda->atendimento->codigo == 10UL);
    assert(arvore.raiz->direita->atendimento->codigo == 30UL);

    // Uma rotação dupla conta duas primitivas. 
    assert(avl_obter_rotacoes(&arvore) == 2UL);
    assert(avl_obter_tamanho(&arvore) == 3UL);
    assert(avl_obter_altura(&arvore) == 1);

    // Todos os nós devem terminar balanceados. 
    assert(avl_obter_fator_balanceamento(arvore.raiz) == 0);
    assert(avl_obter_fator_balanceamento(arvore.raiz->esquerda) == 0);
    assert(avl_obter_fator_balanceamento(arvore.raiz->direita) == 0);

    printf("Rotacao LR: OK\n");

    /*
      Remove os nós da árvore antes de liberar os atendimentos usados no cenário.
    */
    avl_destruir(&arvore);

    assert(arvore.raiz == NULL);
    assert(arvore.tamanho == 0UL);
    assert(arvore.rotacoes == 0UL);

    assert(atendimento1->codigo == 30UL);
    assert(atendimento2->codigo == 10UL);
    assert(atendimento3->codigo == 20UL);

    atendimento_destruir(atendimento1);
    atendimento_destruir(atendimento2);
    atendimento_destruir(atendimento3);

    atendimento1 = NULL;
    atendimento2 = NULL;
    atendimento3 = NULL;

    printf("Finalizacao LR: OK\n");

/*=== ROTACAO RL ==================================================================*/ 

    printf("Testando rotacao RL...\n");

    /*
      Sequência 10, 30, 20:
     
      10
        \
         30
        /
       20
    */
    atendimento1 = criar_atendimento_teste(10UL);
    atendimento2 = criar_atendimento_teste(30UL);
    atendimento3 = criar_atendimento_teste(20UL);

    assert(atendimento1 != NULL);
    assert(atendimento2 != NULL);
    assert(atendimento3 != NULL);

    // Insere os valores que provocam o caso direita-esquerda. 
    assert(avl_inserir(&arvore, atendimento1) == 1);
    assert(avl_inserir(&arvore, atendimento2) == 1);
    assert(avl_inserir(&arvore, atendimento3) == 1);

    /*
      O rebalanceamento realiza:
     
      1. rotação à direita no nó 30;
      2. rotação à esquerda no nó 10.
     
      Resultado:
     
           20
          /  \
        10    30
    */
    assert(arvore.raiz != NULL);

    assert(arvore.raiz->atendimento->codigo == 20UL);
    assert(arvore.raiz->esquerda->atendimento->codigo == 10UL);
    assert(arvore.raiz->direita->atendimento->codigo == 30UL);

    // O caso RL também utiliza duas rotações primitivas. 
    assert(avl_obter_rotacoes(&arvore) == 2UL);
    assert(avl_obter_tamanho(&arvore) == 3UL);
    assert(avl_obter_altura(&arvore) == 1);

    assert(avl_obter_fator_balanceamento(arvore.raiz) == 0);

    printf("Rotacao RL: OK\n");

/*=== EXIBICAO - PRE-ORDEM - EM ORDEM - POS-ORDEM =========================================*/

    printf("Testando percursos...\n");

    avl_inicializar(&arvore_vazia);

    // Árvores nula e vazia devem ser tratadas com segurança. 
    avl_exibir_pre_ordem(NULL);
    avl_exibir_em_ordem(&arvore_vazia);
    avl_exibir_pos_ordem(&arvore_vazia);

    // Exibe a árvore formada pelo caso RL. 
    avl_exibir_pre_ordem(&arvore);
    avl_exibir_em_ordem(&arvore);
    avl_exibir_pos_ordem(&arvore); 

/*=== Verifica que percurso nao altera AVL =========================================*/

    // Percursos são consultas e não podem modificar a AVL. 
    assert(avl_obter_tamanho(&arvore) == 3UL);
    assert(avl_obter_altura(&arvore) == 1);

    assert(avl_obter_rotacoes(&arvore) == 2UL);
    assert(arvore.raiz->atendimento->codigo == 20UL);
    assert(arvore.raiz->esquerda->atendimento->codigo == 10UL);
    assert(arvore.raiz->direita->atendimento->codigo == 30UL);

    printf("Percursos: OK\n");

/*=== DESTRUIR NOS RL =========================================*/

    /*
      Destrói os nós do caso RL e confirma que os atendimentos continuam vivos.
    */
    avl_destruir(&arvore);

    assert(arvore.raiz == NULL);
    assert(arvore.tamanho == 0UL);
    assert(arvore.rotacoes == 0UL);

    assert(atendimento1->codigo == 10UL);
    assert(atendimento2->codigo == 30UL);
    assert(atendimento3->codigo == 20UL);

    atendimento_destruir(atendimento1);
    atendimento_destruir(atendimento2);
    atendimento_destruir(atendimento3);

    atendimento1 = NULL;
    atendimento2 = NULL;
    atendimento3 = NULL;
    encontrado = NULL;

    printf("Finalizacao RL: OK\n");

/*=== CRIAR E INSERIR =========================================*/

    printf("Testando insercao crescente...\n");

    /*
      A mesma sequência produziu altura 6 na BST.
      A AVL deve reorganizar os nós automaticamente.
    */
    for (i = 0UL; i < QUANTIDADE_CRESCENTE; i++) {
        atendimentos_crescentes[i] = criar_atendimento_teste(i + 1UL);
        assert(atendimentos_crescentes[i] != NULL);
        assert(avl_inserir(&arvore, atendimentos_crescentes[i]) == 1);
    }

/*=== VERIFICAR ESTRUTURA ==================================================================*/

    /*
      Estrutura esperada após inserir 1 até 7:
     
               4
             /   \
            2     6
           / \   / \
          1   3 5   7
    */
    assert(avl_obter_tamanho(&arvore) == 7UL);
    assert(avl_obter_altura(&arvore) == 2);

    assert(arvore.raiz != NULL);

    assert(arvore.raiz->atendimento->codigo == 4UL);
    assert(arvore.raiz->esquerda->atendimento->codigo == 2UL);
    assert(arvore.raiz->direita->atendimento->codigo == 6UL);

/*=== NUMERO ROTACOES ==================================================================*/
    
    /*
      Para esta sequência, a implementação padrão realiza quatro rotações simples à esquerda.
    */
    assert(avl_obter_rotacoes(&arvore) == 4UL);

/*=== BUSCAR ESTREMIDADE ==================================================================*/

    // Buscar 1 percorre os nós 4, 2 e 1. 
    encontrado = avl_buscar(&arvore, 1UL, &comparacoes);

    assert(encontrado == atendimentos_crescentes[0]);
    assert(comparacoes == 3UL);

    // Buscar 7 percorre os nós 4, 6 e 7. 
    encontrado = avl_buscar(&arvore, 7UL, &comparacoes);

    assert(encontrado == atendimentos_crescentes[6]);
    assert(comparacoes == 3UL);

    // Confirmar balanceamento
    assert(avl_obter_fator_balanceamento(arvore.raiz) == 0);

    printf("Insercao crescente: OK\n");

/*=== EXIBIR PERCURSOS ==================================================================*/

    printf("Exibindo AVL crescente...\n");

    /*
      Pré-ordem esperada:
      4, 2, 1, 3, 6, 5, 7
    */
    avl_exibir_pre_ordem(&arvore);

    /*
      Em ordem esperada:
      1, 2, 3, 4, 5, 6, 7
    */
    avl_exibir_em_ordem(&arvore);

    /*
      Pós-ordem esperada:
      1, 3, 2, 5, 7, 6, 4
    */
    avl_exibir_pos_ordem(&arvore);

    // Exibir não pode alterar os metadados. 
    assert(avl_obter_tamanho(&arvore) == 7UL);
    assert(avl_obter_altura(&arvore) == 2);
    assert(avl_obter_rotacoes(&arvore) == 4UL);

    printf("Percursos crescentes: OK\n");

/*=== DESTRUIR ==================================================================*/  
  
    printf("Testando destruicao final...\n");

    /*
      Libera os sete nós da AVL, mas não os
      atendimentos apontados por eles.
    */
    avl_destruir(&arvore);

    assert(arvore.raiz == NULL);
    assert(arvore.tamanho == 0UL);
    assert(arvore.rotacoes == 0UL);
    assert(avl_obter_altura(&arvore) == -1);

    // Confirma que os objetos continuam válidos. 
    for (i = 0UL; i < QUANTIDADE_CRESCENTE; i++) {
        assert(atendimentos_crescentes[i]->codigo == i + 1UL);
    }

    // Libera atendimentos do teste
    for (i = 0UL; i < QUANTIDADE_CRESCENTE; i++) {
        atendimento_destruir( atendimentos_crescentes[i]);
        atendimentos_crescentes[i] = NULL;
    }

    /*
      Destruir árvore vazia, já destruída ou nula deve ser uma operação segura.
    */
    avl_destruir(&arvore);
    avl_destruir(&arvore_vazia);
    avl_destruir(NULL);

    assert(arvore_vazia.raiz == NULL);
    assert(arvore_vazia.tamanho == 0UL);
    assert(arvore_vazia.rotacoes == 0UL);

    printf("Destruicao final: OK\n");

    printf(
        "Todos os testes da AVL passaram!\n"
    );

    return 0;
}