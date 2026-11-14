/*========================================= INCLUSOES =============================================*/

#include <stdio.h>

#include "entrada.h"
#include "experimentos.h"
#include "sistema.h"

/*========================================= FUNCOES =============================================*/

/*=====================================================================================================
 EXIBIR MENU
=====================================================================================================*/

static void exibir_menu(void){
    printf(
        "\n========== CENTRAL DE ATENDIMENTO ==========\n"
        "1. Cadastrar atendimento\n"
        "2. Classificar atendimento\n"
        "3. Consultar pela BST\n"
        "4. Consultar pela AVL\n"
        "5. Exibir percursos e medidas da BST\n"
        "6. Exibir percursos e medidas da AVL\n"
        "7. Exibir fila de atendimentos\n"
        "8. Chamar proximo atendimento comum\n"
        "9. Exibir heap de prioridades\n"
        "10. Chamar atendimento prioritario\n"
        "11. Concluir atendimento\n"
        "12. Exibir historico\n"
        "13. Comparar BST e AVL\n"
        "14. Validar estruturas\n"
        "15. Exibir arvore de decisao\n"
        "0. Encerrar\n"
        "============================================\n"
    );
}

/*=====================================================================================================
 EXIBIR RESULTADO
=====================================================================================================*/

static void exibir_resultado(ResultadoSistema resultado){
    printf("Resultado: %s.\n", resultado_sistema_para_texto( resultado));
}

/*=====================================================================================================
 CADASTRO
=====================================================================================================*/

static void opcao_cadastrar(Sistema *sistema){

    char solicitante[TAM_SOLICITANTE];
    char descricao[TAM_DESCRICAO];
    int prioridade;

    Atendimento *criado;
    ResultadoSistema resultado;

    if (!entrada_ler_texto("Solicitante: ", solicitante, sizeof(solicitante))) {
        printf("Entrada encerrada.\n");
        return;
    }

    if (!entrada_ler_texto("Descricao: ", descricao, sizeof(descricao))) {
        printf("Entrada encerrada.\n");
        return;
    }

    if (!entrada_ler_inteiro("Prioridade de 1 a 10: ", 1, 10, &prioridade)) {
        printf("Entrada encerrada.\n");
        return;
    }

    criado = NULL;

    resultado = sistema_cadastrar(sistema, solicitante, descricao, prioridade, &criado);

    exibir_resultado(resultado);

    if (resultado == RESULTADO_SISTEMA_SUCESSO && criado != NULL) {
        printf("Codigo gerado: %lu\n", criado->codigo);
        printf("Ordem de chegada: %lu\n", criado->ordem_chegada);
    }
}

/*=====================================================================================================
 CLASSIFICAR
=====================================================================================================*/
static void opcao_classificar(Sistema *sistema){

    unsigned long codigo;
    Classificacao classificacao;
    ResultadoSistema resultado;

    if (!entrada_ler_positivo("Codigo do atendimento: ", &codigo)) {
        printf("Entrada encerrada.\n");
        return;
    }

    classificacao = CLASSIFICACAO_NAO_DEFINIDA;

    resultado = sistema_classificar(sistema, codigo, &classificacao);

    exibir_resultado(resultado);

    if (resultado == RESULTADO_SISTEMA_SUCESSO) {
        printf("Classificacao obtida: %s\n", classificacao_para_texto(classificacao));
    }
}

/*=====================================================================================================
 CONSULTAR
=====================================================================================================*/
static void opcao_consultar(Sistema *sistema, int usar_avl){

    unsigned long codigo;
    unsigned long comparacoes;

    Atendimento *encontrado;
    ResultadoSistema resultado;

    if (!entrada_ler_positivo("Codigo do atendimento: ", &codigo)) {
        printf("Entrada encerrada.\n");
        return;
    }

    encontrado = NULL;
    comparacoes = 0UL;

    if (usar_avl) {
        resultado = sistema_buscar_avl(sistema, codigo, &encontrado, &comparacoes);
    } else {
        resultado = sistema_buscar_bst(sistema, codigo, &encontrado, &comparacoes);
    }

    exibir_resultado(resultado);

    if (resultado != RESULTADO_SISTEMA_SUCESSO) {
        return;
    }

    printf("Estrutura utilizada: %s\n", usar_avl ? "AVL" : "BST");

    printf("Comparacoes: %lu\n", comparacoes);

    atendimento_exibir(encontrado);
}

/*=====================================================================================================
 EXIBIR BST
=====================================================================================================*/

static void opcao_exibir_bst(const Sistema *sistema){

    printf("\nQuantidade de nos: %lu\n", bst_obter_tamanho(&sistema->bst));

    printf("Altura: %d\n", bst_obter_altura(&sistema->bst));

    bst_exibir_pre_ordem(&sistema->bst);

    bst_exibir_em_ordem(&sistema->bst);

    bst_exibir_pos_ordem(&sistema->bst);
}

/*=====================================================================================================
 EXIBIR AVL
=====================================================================================================*/

static void opcao_exibir_avl(const Sistema *sistema){

    printf("\nQuantidade de nos: %lu\n", avl_obter_tamanho(&sistema->avl));

    printf("Altura: %d\n", avl_obter_altura(&sistema->avl));

    printf("Rotacoes realizadas: %lu\n", avl_obter_rotacoes(&sistema->avl));

    avl_exibir_pre_ordem(&sistema->avl);

    avl_exibir_em_ordem(&sistema->avl);

    avl_exibir_pos_ordem(&sistema->avl);
}

/*=====================================================================================================
 EXECUTAR ATENDIMENTO
=====================================================================================================*/

static void opcao_chamar(Sistema *sistema, int prioritario){
    
    Atendimento *chamado;
    ResultadoSistema resultado;

    chamado = NULL;

    if (prioritario) {
        resultado = sistema_chamar_prioritario(sistema, &chamado);
    } else {
        resultado = sistema_chamar_proximo_fila(sistema, &chamado);
    }

    exibir_resultado(resultado);

    if (resultado != RESULTADO_SISTEMA_SUCESSO) {
        return;
    }

    printf("Atendimento iniciado pela %s:\n", prioritario ? "prioridade" : "fila");

    atendimento_exibir(chamado);
}

/*=====================================================================================================
 CONCLUIR ATENDIMENTO
=====================================================================================================*/

static void opcao_concluir(Sistema *sistema){
    unsigned long codigo;

    Atendimento *atendimento;
    ResultadoSistema resultado;

    if (!entrada_ler_positivo("Codigo do atendimento: ", &codigo)) {
        printf("Entrada encerrada.\n");
        return;
    }

    resultado = sistema_concluir(sistema, codigo);

    exibir_resultado(resultado);

    if (resultado != RESULTADO_SISTEMA_SUCESSO) {
        return;
    }

    atendimento = NULL;

    resultado = sistema_buscar( sistema, codigo, &atendimento);

    if (resultado == RESULTADO_SISTEMA_SUCESSO && atendimento != NULL) {
        atendimento_exibir(atendimento);
    }
}

/*=====================================================================================================
 VALIDACAO
=====================================================================================================*/

static void opcao_validar(const Sistema *sistema){

    ResultadoSistema resultado;

    resultado = sistema_validar(sistema);

    exibir_resultado(resultado);

    if (resultado != RESULTADO_SISTEMA_SUCESSO) {
        return;
    }

    printf("Repositorio: %lu atendimentos\n", repositorio_obter_quantidade(&sistema->repositorio));

    printf("BST: %lu nos, altura %d\n", bst_obter_tamanho(&sistema->bst), bst_obter_altura(&sistema->bst));

    printf("AVL: %lu nos, altura %d, %lu rotacoes\n", avl_obter_tamanho(&sistema->avl), 
        avl_obter_altura(&sistema->avl), avl_obter_rotacoes(&sistema->avl));

    printf("Fila: %lu atendimentos\n", fila_obter_tamanho(&sistema->fila));

    printf("Heap: %lu atendimentos\n", heap_obter_tamanho(&sistema->heap));

    printf("Historico: %lu eventos\n", pilha_obter_tamanho(&sistema->historico));
}

/*==================================== EXECUCAO SISTEMA =============================================*/

int main(void){

    Sistema sistema;
    ResultadoSistema resultado;
    int opcao;

    resultado = sistema_inicializar(&sistema);

    if (resultado != RESULTADO_SISTEMA_SUCESSO) {
        exibir_resultado(resultado);
        return 1;
    }

    printf("Central de Atendimento inicializada.\n");

    while (1) {
        exibir_menu();

        if (!entrada_ler_inteiro("Escolha uma opcao: ", 0, 15, &opcao)) {
            printf("Entrada encerrada. Finalizando o sistema.\n");
            break;
        }

        if (opcao == 0) {
            break;
        }

        switch (opcao) {
                   
            case 1:
                opcao_cadastrar(&sistema);
                break;

            case 2:
                opcao_classificar(&sistema);
                break;

            case 3:
                /* 0 seleciona a BST. */
                opcao_consultar(&sistema, 0);
                break;

            case 4:
                /* 1 seleciona a AVL. */
                opcao_consultar(&sistema, 1);
                break;

            case 5:
                opcao_exibir_bst(&sistema);
                break;

            case 6:
                opcao_exibir_avl(&sistema);
                break;

            case 7:
                fila_exibir(&sistema.fila);
                break;

            case 8:
                /* 0 seleciona a fila comum. */
                opcao_chamar(&sistema, 0);
                break;
            
            case 9:
                heap_exibir(&sistema.heap);
                break;

            case 10:
                /* 1 seleciona o heap prioritário. */
                opcao_chamar(&sistema, 1);
                break;

            case 11:
                opcao_concluir(&sistema);
                break;

            case 12:
                pilha_exibir(&sistema.historico);
                break;

            case 13:
                if (!experimentos_executar()) {
                    printf("Falha ao executar os experimentos.\n");
                }
                break;

            case 14:
                opcao_validar(&sistema);
                break;

            case 15:
                arvore_decisao_exibir(&sistema.arvore_decisao);
                break;

            default:
                printf("Opcao invalida.\n");
                break;
        }

        if (sistema.inicializado == 0) {
            printf("O sistema foi encerrado devido a uma falha critica.\n");
            break;
        }
    }

    sistema_destruir(&sistema);

    printf("Memoria liberada. Programa encerrado.\n");

    return 0;
}