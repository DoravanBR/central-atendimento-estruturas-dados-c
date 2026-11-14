/*========================================= INCLUSOES =============================================*/

#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "sistema.h"
#include "validacao.h"

/*========================================= FUNCOES =============================================*/

/*=====================================================================================================
 VERIFICAR SISTEMA PRONTO
=====================================================================================================*/

static int sistema_esta_pronto(const Sistema *sistema){
    return sistema != NULL && sistema->inicializado == 1;
}

/*=====================================================================================================
 PREPARAR SISTEMA
=====================================================================================================*/

static void preparar_estado_vazio(Sistema *sistema){

    if (sistema == NULL) {
        return;
    }

    // Estruturas cuja inicialização não aloca memória. 
    fila_inicializar(&sistema->fila);
    pilha_inicializar(&sistema->historico);
    bst_inicializar(&sistema->bst);
    avl_inicializar(&sistema->avl);

    // Repositório ainda sem vetor alocado. 
    sistema->repositorio.itens = NULL;
    sistema->repositorio.quantidade = 0UL;
    sistema->repositorio.capacidade = 0UL;

    // Heap ainda sem vetor alocado. 
    sistema->heap.itens = NULL;
    sistema->heap.quantidade = 0UL;
    sistema->heap.capacidade = 0UL;

    // Árvore de decisão ainda sem nós. 
    sistema->arvore_decisao.raiz = NULL;
    sistema->arvore_decisao.quantidade_nos = 0UL;

    // Contadores indisponíveis até a inicialização terminar. 
    sistema->proximo_codigo = 0UL;
    sistema->proxima_ordem = 0UL;

    sistema->inicializado = 0;
}

/*=====================================================================================================
 DESTRUIR SISTEMA
=====================================================================================================*/

void sistema_destruir(Sistema *sistema){

    if (sistema == NULL) {
        return;
    }

    /*
      Primeiro liberamos as estruturas que apenas emprestam ponteiros.
    */
    fila_destruir(&sistema->fila);
    heap_destruir(&sistema->heap);
    bst_destruir(&sistema->bst);
    avl_destruir(&sistema->avl);

    /*
      O histórico e a árvore de decisão possuem objetos próprios.
    */
    pilha_destruir(&sistema->historico);

    arvore_decisao_destruir(&sistema->arvore_decisao);

    /*
      O repositório fica por último porque destrói os objetos Atendimento.
    */
    repositorio_destruir(&sistema->repositorio);

    /*
      Reaplica um estado conhecido e marca o sistema como não inicializado.
    */
    preparar_estado_vazio(sistema);
}

/*=====================================================================================================
 INICIALIZAR SISTEMA
=====================================================================================================*/

ResultadoSistema sistema_inicializar(Sistema *sistema){

    if (sistema == NULL) {
        return RESULTADO_SISTEMA_ARGUMENTO_INVALIDO;
    }

    preparar_estado_vazio(sistema);

    /*
      O repositório aloca o vetor que será proprietário dos atendimentos.
    */
    if (!repositorio_inicializar(&sistema->repositorio)) {
        sistema_destruir(sistema);
        return RESULTADO_SISTEMA_MEMORIA;
    }

    /* O heap aloca seu vetor inicial. */
    if (!heap_inicializar(&sistema->heap)) {
        sistema_destruir(sistema);
        return RESULTADO_SISTEMA_MEMORIA;
    }

    /*
      A árvore de decisão aloca três perguntas e quatro folhas.
     */
    if (!arvore_decisao_inicializar(&sistema->arvore_decisao)) {
        sistema_destruir(sistema);
        return RESULTADO_SISTEMA_MEMORIA;
    }

    sistema->proximo_codigo = 1UL;
    sistema->proxima_ordem = 1UL;
    sistema->inicializado = 1;

    return RESULTADO_SISTEMA_SUCESSO;
}

/*=====================================================================================================
 BUSCAR
=====================================================================================================*/

ResultadoSistema sistema_buscar(Sistema *sistema, unsigned long codigo,
     Atendimento **atendimento_encontrado){

    Atendimento *atendimento;

    if (atendimento_encontrado != NULL) {
        *atendimento_encontrado = NULL;
    }

    if (sistema == NULL || atendimento_encontrado == NULL || codigo == 0UL) {
        return RESULTADO_SISTEMA_ARGUMENTO_INVALIDO;
    }

    if (!sistema_esta_pronto(sistema)) {
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    atendimento = repositorio_buscar(&sistema->repositorio, codigo);

    if (atendimento == NULL) {
        return RESULTADO_SISTEMA_NAO_ENCONTRADO;
    }

    *atendimento_encontrado = atendimento;

    return RESULTADO_SISTEMA_SUCESSO;
}

ResultadoSistema sistema_buscar_bst(const Sistema *sistema, unsigned long codigo, 
    Atendimento **atendimento_encontrado, unsigned long *comparacoes){

    Atendimento *atendimento;

    if (atendimento_encontrado != NULL) {
        *atendimento_encontrado = NULL;
    }

    if (comparacoes != NULL) {
        *comparacoes = 0UL;
    }

    if (sistema == NULL || atendimento_encontrado == NULL || codigo == 0UL) {
        return RESULTADO_SISTEMA_ARGUMENTO_INVALIDO;
    }

    if (!sistema_esta_pronto(sistema)) {
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    atendimento = bst_buscar(&sistema->bst, codigo, comparacoes);

    if (atendimento == NULL) {
        return RESULTADO_SISTEMA_NAO_ENCONTRADO;
    }

    *atendimento_encontrado = atendimento;

    return RESULTADO_SISTEMA_SUCESSO;
}

ResultadoSistema sistema_buscar_avl(const Sistema *sistema, unsigned long codigo, 
    Atendimento **atendimento_encontrado, unsigned long *comparacoes){

    Atendimento *atendimento;

    if (atendimento_encontrado != NULL) {
        *atendimento_encontrado = NULL;
    }

    if (comparacoes != NULL) {
        *comparacoes = 0UL;
    }

    if (sistema == NULL || atendimento_encontrado == NULL || codigo == 0UL) {
        return RESULTADO_SISTEMA_ARGUMENTO_INVALIDO;
    }

    if (!sistema_esta_pronto(sistema)) {
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    atendimento = avl_buscar(&sistema->avl, codigo, comparacoes);

    if (atendimento == NULL) {
        return RESULTADO_SISTEMA_NAO_ENCONTRADO;
    }

    *atendimento_encontrado = atendimento;

    return RESULTADO_SISTEMA_SUCESSO;
}

/*=====================================================================================================
 CADASTRAR
=====================================================================================================*/

ResultadoSistema sistema_cadastrar(Sistema *sistema, const char *solicitante, 
    const char *descricao, int prioridade, Atendimento **atendimento_criado){

    Atendimento *novo;

    if (atendimento_criado != NULL) {
        *atendimento_criado = NULL;
    }

    if (sistema == NULL || solicitante == NULL || descricao == NULL || solicitante[0] == '\0' ||
         descricao[0] == '\0' || strlen(solicitante) >= TAM_SOLICITANTE || strlen(descricao) >= 
         TAM_DESCRICAO) {
        return RESULTADO_SISTEMA_ARGUMENTO_INVALIDO;
    }

    if (!sistema_esta_pronto(sistema)) {
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    if (!atendimento_prioridade_valida(prioridade)) {
        return RESULTADO_SISTEMA_PRIORIDADE_INVALIDA;
    }

    if (sistema->proximo_codigo == 0UL || sistema->proximo_codigo == ULONG_MAX) {
        return RESULTADO_SISTEMA_CODIGO_ESGOTADO;
    }

    if (sistema->proxima_ordem == 0UL || sistema->proxima_ordem == ULONG_MAX) {
        return RESULTADO_SISTEMA_ORDEM_ESGOTADA;
    }

    novo = atendimento_criar(sistema->proximo_codigo, solicitante, descricao, prioridade, 
        sistema->proxima_ordem);

    if (novo == NULL) {
        return RESULTADO_SISTEMA_MEMORIA;
    }

    /*
      Antes da inserção, o atendimento pertence temporariamente a esta função.
    */
    if (!repositorio_inserir(&sistema->repositorio, novo)) {
        atendimento_destruir(novo);
        return RESULTADO_SISTEMA_MEMORIA;
    }

    /*
      Depois do sucesso no repositório, somente ele poderá destruir o objeto.
    */
    if (!bst_inserir(&sistema->bst, novo)) {
        sistema_destruir(sistema);
        return RESULTADO_SISTEMA_MEMORIA;
    }

    if (!avl_inserir(&sistema->avl, novo)) {
        sistema_destruir(sistema);
        return RESULTADO_SISTEMA_MEMORIA;
    }

    if (!pilha_empilhar(&sistema->historico, novo->codigo, "Atendimento cadastrado")) {
        sistema_destruir(sistema);
        return RESULTADO_SISTEMA_MEMORIA;
    }
    
    sistema->proximo_codigo++;
    sistema->proxima_ordem++;

    if (atendimento_criado != NULL) {
        *atendimento_criado = novo;
    }

    return RESULTADO_SISTEMA_SUCESSO;
}

/*=====================================================================================================
 CLASSIFICAR
=====================================================================================================*/

ResultadoSistema sistema_classificar(Sistema *sistema, unsigned long codigo, 
    Classificacao *classificacao_obtida){

    Atendimento *atendimento;
    Classificacao classificacao;
    ResultadoSistema resultado_busca;

    int inserido;
    const char *operacao;

    if (classificacao_obtida != NULL) {
        *classificacao_obtida = CLASSIFICACAO_NAO_DEFINIDA;
    }

    if (sistema == NULL || codigo == 0UL) {
        return RESULTADO_SISTEMA_ARGUMENTO_INVALIDO;
    }

    if (!sistema_esta_pronto(sistema)) {
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    resultado_busca = sistema_buscar(sistema, codigo, &atendimento);

    if (resultado_busca != RESULTADO_SISTEMA_SUCESSO) {
        return resultado_busca;
    }

    if (atendimento->classificacao != CLASSIFICACAO_NAO_DEFINIDA) {
        return RESULTADO_SISTEMA_CLASSIFICACAO_DUPLICADA;
    }

    if (atendimento->status != STATUS_ABERTO) {
        return RESULTADO_SISTEMA_STATUS_INVALIDO;
    }

    classificacao = CLASSIFICACAO_NAO_DEFINIDA;

    if (!arvore_decisao_classificar(&sistema->arvore_decisao, &classificacao)) {
        return RESULTADO_SISTEMA_ARGUMENTO_INVALIDO;
    }

    /*
      Atualiza classificação e status antes do encaminhamento, conforme a regra definida.
    */
    if (!atendimento_confirmar_classificacao(atendimento, classificacao)) {
        return RESULTADO_SISTEMA_STATUS_INVALIDO;
    }

    if (classificacao == CLASSIFICACAO_COMUM || classificacao == CLASSIFICACAO_MODERADA) {
        inserido = fila_enfileirar( &sistema->fila, atendimento);
    } else {
        inserido = heap_inserir(&sistema->heap, atendimento);
    }

    if (!inserido) {
        /*
          A inserção falhou antes de criar qualquer vínculo na fila ou no heap.
        */
        atendimento->classificacao = CLASSIFICACAO_NAO_DEFINIDA;

        atendimento->status = STATUS_ABERTO;

        return RESULTADO_SISTEMA_MEMORIA;
    }

    switch (classificacao) {
        case CLASSIFICACAO_COMUM:
            operacao = "Classificado como comum";
            break;

        case CLASSIFICACAO_MODERADA:
            operacao = "Classificado como moderado";
            break;

        case CLASSIFICACAO_URGENTE:
            operacao = "Classificado como urgente";
            break;

        case CLASSIFICACAO_CRITICA:
            operacao = "Classificado como critico";
            break;

        case CLASSIFICACAO_NAO_DEFINIDA:
        default:
            sistema_destruir(sistema);

            return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    if (!pilha_empilhar(&sistema->historico, codigo, operacao)) {
        /*
          O atendimento já está dentro da fila ou heap; sem remoção transacional comum, 
          encerramos o estado integrado.
        */
        sistema_destruir(sistema);

        return RESULTADO_SISTEMA_MEMORIA;
    }

    if (classificacao_obtida != NULL) {
        *classificacao_obtida = classificacao;
    }

    return RESULTADO_SISTEMA_SUCESSO;
}

/*=====================================================================================================
 CHAMAR PROXIMO FILA
=====================================================================================================*/

ResultadoSistema sistema_chamar_proximo_fila(Sistema *sistema, Atendimento **atendimento_chamado){

    Atendimento *inicio;
    Atendimento *removido;

    if (atendimento_chamado != NULL) {
        *atendimento_chamado = NULL;
    }

    if (sistema == NULL || atendimento_chamado == NULL) {
        return RESULTADO_SISTEMA_ARGUMENTO_INVALIDO;
    }

    if (!sistema_esta_pronto(sistema)) {
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    inicio = fila_consultar_inicio(&sistema->fila);

    if (inicio == NULL) {
        return RESULTADO_SISTEMA_FILA_VAZIA;
    }

    /*
      Valida a transição antes de remover o nó.
      Assim, um status inválido preserva a fila.
    */
    if (!atendimento_pode_mudar_status(inicio->status, STATUS_EM_ATENDIMENTO)) {
        return RESULTADO_SISTEMA_STATUS_INVALIDO;
    }

    removido = fila_desenfileirar(&sistema->fila);

    if (removido == NULL || removido != inicio) {
        sistema_destruir(sistema);
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    if (!atendimento_atualizar_status(removido, STATUS_EM_ATENDIMENTO)) {
        sistema_destruir(sistema);
        return RESULTADO_SISTEMA_STATUS_INVALIDO;
    }

    if (!pilha_empilhar(&sistema->historico, removido->codigo, "Atendimento comum iniciado")) {
        sistema_destruir(sistema);
        return RESULTADO_SISTEMA_MEMORIA;
    }

    *atendimento_chamado = removido;

    return RESULTADO_SISTEMA_SUCESSO;
}

/*=====================================================================================================
 CHAMAR PROXIMO HEAP
=====================================================================================================*/

ResultadoSistema sistema_chamar_prioritario(Sistema *sistema, Atendimento **atendimento_chamado){

    Atendimento *raiz;
    Atendimento *removido;

    if (atendimento_chamado != NULL) {
        *atendimento_chamado = NULL;
    }

    if (sistema == NULL || atendimento_chamado == NULL) {
        return RESULTADO_SISTEMA_ARGUMENTO_INVALIDO;
    }

    if (!sistema_esta_pronto(sistema)) {
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    raiz = heap_consultar_raiz(&sistema->heap);

    if (raiz == NULL) {
        return RESULTADO_SISTEMA_HEAP_VAZIO;
    }

    /*
      Valida antes de remover a raiz,
      preservando o heap em caso de status inválido.
    */
    if (!atendimento_pode_mudar_status(raiz->status, STATUS_EM_ATENDIMENTO)) {
        return RESULTADO_SISTEMA_STATUS_INVALIDO;
    }

    removido = heap_remover_raiz(&sistema->heap);

    if (removido == NULL || removido != raiz) {
        sistema_destruir(sistema);
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    if (!atendimento_atualizar_status(removido, STATUS_EM_ATENDIMENTO)) {
        sistema_destruir(sistema);
        return RESULTADO_SISTEMA_STATUS_INVALIDO;
    }

    if (!pilha_empilhar(&sistema->historico, removido->codigo, "Atendimento prioritario iniciado")) {
        sistema_destruir(sistema);
        return RESULTADO_SISTEMA_MEMORIA;
    }

    *atendimento_chamado = removido;

    return RESULTADO_SISTEMA_SUCESSO;
}

/*=====================================================================================================
 CONCLUIR ATENDIMENTO
=====================================================================================================*/

ResultadoSistema sistema_concluir(Sistema *sistema, unsigned long codigo){

    Atendimento *atendimento;
    ResultadoSistema resultado_busca;

    if (sistema == NULL || codigo == 0UL) {
        return RESULTADO_SISTEMA_ARGUMENTO_INVALIDO;
    }

    if (!sistema_esta_pronto(sistema)) {
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    resultado_busca = sistema_buscar(sistema, codigo, &atendimento);

    if (resultado_busca != RESULTADO_SISTEMA_SUCESSO) {
        return resultado_busca;
    }

    /*
      Somente um atendimento atualmente em execução pode ser concluído.
     */
    if (atendimento->status != STATUS_EM_ATENDIMENTO) {
        return RESULTADO_SISTEMA_STATUS_INVALIDO;
    }

    if (!atendimento_atualizar_status(atendimento, STATUS_CONCLUIDO)) {
        return RESULTADO_SISTEMA_STATUS_INVALIDO;
    }

    if (!pilha_empilhar(&sistema->historico, codigo, "Atendimento concluido")) {
        /*
          O status é o único estado alterado nesta operação, então pode ser restaurado.
        */
        atendimento->status = STATUS_EM_ATENDIMENTO;

        return RESULTADO_SISTEMA_MEMORIA;
    }

    return RESULTADO_SISTEMA_SUCESSO;
}

/*=====================================================================================================
 VALIDAR
=====================================================================================================*/

ResultadoSistema sistema_validar(const Sistema *sistema){

    unsigned long quantidade_repositorio;
    unsigned long quantidade_bst;
    unsigned long quantidade_avl;

    if (sistema == NULL) {
        return RESULTADO_SISTEMA_ARGUMENTO_INVALIDO;
    }

    if (!sistema_esta_pronto(sistema)) {
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    /*
      Verifica as propriedades internas das três estruturas que possuem validadores.
    */
    if (!validacao_bst(&sistema->bst) || !validacao_avl(&sistema->avl) || !validacao_heap(&sistema->heap)) {
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    if (sistema->repositorio.itens == NULL || sistema->repositorio.capacidade == 0UL || 
        sistema->repositorio.quantidade > sistema->repositorio.capacidade) {
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    quantidade_repositorio = repositorio_obter_quantidade(&sistema->repositorio);

    quantidade_bst = bst_obter_tamanho(&sistema->bst);

    quantidade_avl = avl_obter_tamanho(&sistema->avl);    

    /*
      Todo atendimento cadastrado deve aparecer simultaneamente no repositório, BST e AVL.
    */
    if (quantidade_repositorio != quantidade_bst || quantidade_repositorio != quantidade_avl) {
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }   
    
    if (arvore_decisao_contar_nos(&sistema->arvore_decisao) != 7UL ||
     arvore_decisao_contar_folhas(&sistema->arvore_decisao) != 4UL ||
      arvore_decisao_obter_altura(&sistema->arvore_decisao) != 2) {
        return RESULTADO_SISTEMA_ESTRUTURA_INVALIDA;
    }

    return RESULTADO_SISTEMA_SUCESSO;
}

/*=====================================================================================================
 SISTEMA PARA TEXTO
=====================================================================================================*/

const char *resultado_sistema_para_texto(ResultadoSistema resultado){

    switch (resultado) {
        case RESULTADO_SISTEMA_SUCESSO:
            return "sucesso";

        case RESULTADO_SISTEMA_ARGUMENTO_INVALIDO:
            return "argumento invalido";

        case RESULTADO_SISTEMA_MEMORIA:
            return "falha de memoria";

        case RESULTADO_SISTEMA_PRIORIDADE_INVALIDA:
            return "prioridade invalida";

        case RESULTADO_SISTEMA_NAO_ENCONTRADO:
            return "atendimento nao encontrado";

        case RESULTADO_SISTEMA_STATUS_INVALIDO:
            return "status invalido para a operacao";

        case RESULTADO_SISTEMA_CLASSIFICACAO_DUPLICADA:
            return "atendimento ja classificado";

        case RESULTADO_SISTEMA_CODIGO_ESGOTADO:
            return "contador de codigo esgotado";

        case RESULTADO_SISTEMA_ORDEM_ESGOTADA:
            return "contador de ordem esgotado";

        case RESULTADO_SISTEMA_FILA_VAZIA:
            return "fila vazia";

        case RESULTADO_SISTEMA_HEAP_VAZIO:
            return "heap vazio";

        case RESULTADO_SISTEMA_ESTRUTURA_INVALIDA:
            return "estrutura do sistema invalida";

        default:
            return "resultado desconhecido";
    }
}