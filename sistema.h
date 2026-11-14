#ifndef SISTEMA_H
#define SISTEMA_H

/*========================================= INCLUSOES =============================================*/

#include "arvore_decisao.h"
#include "avl.h"
#include "bst.h"
#include "fila.h"
#include "heap.h"
#include "pilha.h"
#include "repositorio.h"

/*========================================= DEFINICOES =============================================*/
typedef enum {
    RESULTADO_SISTEMA_SUCESSO,
    RESULTADO_SISTEMA_ARGUMENTO_INVALIDO,
    RESULTADO_SISTEMA_MEMORIA,
    RESULTADO_SISTEMA_PRIORIDADE_INVALIDA,
    RESULTADO_SISTEMA_NAO_ENCONTRADO,
    RESULTADO_SISTEMA_STATUS_INVALIDO,
    RESULTADO_SISTEMA_CLASSIFICACAO_DUPLICADA,
    RESULTADO_SISTEMA_CODIGO_ESGOTADO,
    RESULTADO_SISTEMA_ORDEM_ESGOTADA,
    RESULTADO_SISTEMA_FILA_VAZIA,
    RESULTADO_SISTEMA_HEAP_VAZIO,
    RESULTADO_SISTEMA_ESTRUTURA_INVALIDA
} ResultadoSistema;

/*========================================= ESTRUTURAS =============================================*/

typedef struct {
    Repositorio repositorio;
    FilaAtendimentos fila;
    PilhaHistorico historico;
    ArvoreBST bst;
    ArvoreAVL avl;
    HeapMaximo heap;
    ArvoreDecisao arvore_decisao;

    unsigned long proximo_codigo;
    unsigned long proxima_ordem;

    int inicializado;
} Sistema;

/*=================================== PROTOTIPO DE FUNCOES =======================================*/

ResultadoSistema sistema_inicializar(Sistema *sistema);
void sistema_destruir(Sistema *sistema);
ResultadoSistema sistema_cadastrar(Sistema *sistema, const char *solicitante, const char *descricao,
     int prioridade, Atendimento **atendimento_criado);
ResultadoSistema sistema_buscar(Sistema *sistema, unsigned long codigo,
     Atendimento **atendimento_encontrado);
ResultadoSistema sistema_classificar(Sistema *sistema, unsigned long codigo, 
    Classificacao *classificacao_obtida);
ResultadoSistema sistema_chamar_proximo_fila(Sistema *sistema, Atendimento **atendimento_chamado);
ResultadoSistema sistema_chamar_prioritario(Sistema *sistema, Atendimento **atendimento_chamado);
ResultadoSistema sistema_concluir(Sistema *sistema, unsigned long codigo);
ResultadoSistema sistema_buscar_bst(const Sistema *sistema, unsigned long codigo, 
    Atendimento **atendimento_encontrado, unsigned long *comparacoes);
ResultadoSistema sistema_buscar_avl(const Sistema *sistema, unsigned long codigo, 
    Atendimento **atendimento_encontrado, unsigned long *comparacoes);
ResultadoSistema sistema_validar(const Sistema *sistema);
const char *resultado_sistema_para_texto(ResultadoSistema resultado);

#endif