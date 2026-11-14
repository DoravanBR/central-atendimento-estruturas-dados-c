/*========================================= INCLUSOES =============================================*/
#include <assert.h>
#include <stdio.h>

#include "repositorio.h"

/*========================================= TESTES =============================================*/

int main(void)
{
    Repositorio repositorio;
    Atendimento *atendimento1;
    Atendimento *encontrado;
    Atendimento *duplicado;
    Repositorio nao_inicializado = {0};
    Atendimento *novo;
    Repositorio vazio;

    unsigned long codigo;

    printf("Iniciando testes do repositorio...\n");

/*======================================= INICIALIZACAO ===========================================*/

    printf("Testando inicializacao...\n");
    printf("Repositorio inexistente\n");
    assert(repositorio_inicializar(NULL) == 0);
    printf("Repositorio valido\n");
    assert(repositorio_inicializar(&repositorio) == 1);
    printf("Verificacao de campos\n");
    assert(repositorio.itens != NULL);
    assert(repositorio.quantidade == 0UL);
    assert(repositorio.capacidade == 8UL);
    printf("Inicializacao: OK\n");

/*====================================== VALIDACAO FUNCOES ==========================================*/ 
/*==== OBTER QUANTIDADE ==========================================*/  

    printf("Testando Funcoes...\n");
    printf("Obter quantidade - VALIDO\n");
    assert(repositorio_obter_quantidade(&repositorio) == 0UL);
    printf("Obter quantidade - INEXISTENTE");
    assert(repositorio_obter_quantidade(NULL) == 0UL);

/*===== INSERIR ==========================================*/    
        
    printf("Testando primeira insercao...\n");
    atendimento1 = atendimento_criar(1UL, "Ana Silva", "Servidor principal indisponivel", 8, 1UL);
    printf("Atendimento Inexistente\n");
    assert(atendimento1 != NULL);
    printf("Inserir no repositorio");
    assert(repositorio_inserir(&repositorio, atendimento1) == 1);
    printf("Validacao estrutura repositorio\n");
    assert(repositorio.quantidade == 1UL);
    assert(repositorio.capacidade == 8UL);
    assert(repositorio.itens[0] == atendimento1);
    printf("Validando quantidade\n");
    assert(repositorio_obter_quantidade(&repositorio) == 1UL);
    printf("Primeira insercao: OK\n");

/*===== BUSCA ==========================================*/

    printf("Testando busca por codigo...\n");
    encontrado = repositorio_buscar(&repositorio,1UL);
    printf("Atendimento nao encontrado\n");
    assert(encontrado != NULL);
    printf("Atendimento encontrado\n");
    assert(encontrado == atendimento1);
    printf("Validacao do codigo\n");
    assert(encontrado->codigo == 1UL);
    printf("Codigo inexistente\n");
    assert(repositorio_buscar(&repositorio,999UL) == NULL);
    printf("codigo 0 - PROIBIDO\n");
    assert(repositorio_buscar(&repositorio,0UL) == NULL);
    printf("Repositorio Nulo\n");
    assert(repositorio_buscar(NULL,1UL) == NULL);
    printf("Busca por codigo: OK\n");
    
/*===== VALILDACAO CODIGO DUPLICADO ==========================================*/
   
    printf("Testando rejeicao de duplicidade...\n");
    duplicado = atendimento_criar(1UL, "Bruno Costa", "Impressora sem comunicacao", 4, 2UL);
    printf("Atendimento inexistente\n");
    assert(duplicado != NULL);
    printf("Valida rejeicao duplicado\n");
    assert(repositorio_inserir(&repositorio, duplicado) == 0);
    printf("Valida quantidade inalterada\n");
    assert(repositorio.quantidade == 1UL);
    printf("Valida posicao inalterada\n");
    assert(repositorio.itens[0] == atendimento1);
    atendimento_destruir(duplicado);
    duplicado = NULL;
    printf("Rejeicao de duplicidade: OK\n");

/*===== ARGUMENTOS INVALIDOS NA INSERCAO ==========================================*/
    
    printf("Testando insercoes invalidas...\n");
    printf("Atendimento Inexistente\n");
    assert(repositorio_inserir(&repositorio, NULL) == 0);
    printf("Repositorio Inexistente\n");
    assert(repositorio_inserir(NULL, atendimento1) == 0);
    printf("Repositorio nao inicializado\n");
    assert(repositorio_inserir(&nao_inicializado, atendimento1) == 0);
    printf("Valida rejeicao de atendimentos invalidos\n");
    assert(repositorio.quantidade == 1UL);
    printf("Insercoes invalidas: OK\n");

/*===== EXPANSAO DO VETOR CAPACIDADE ==========================================*/
    
    printf("Testando expansao do vetor...\n");

    for (codigo = 2UL; codigo <= 9UL; codigo++) {
        novo = atendimento_criar(codigo, "Usuario de teste", "Atendimento para testar expansao",
             (int)codigo, codigo);     
        assert(novo != NULL);
        assert(repositorio_inserir( &repositorio, novo ) == 1);
    }
    // confirma expansao
    assert(repositorio.quantidade == 9UL);
    assert(repositorio.capacidade == 16UL);
    printf("Expansao do vetor: OK\n");

/*===== VERIFICANDO DADOS APOS EXPANSAO ==========================================*/
    
    printf("Verificando dados apos expansao...\n");

    for (codigo = 1UL; codigo <= 9UL; codigo++) {
        encontrado = repositorio_buscar(&repositorio,codigo);
        assert(encontrado != NULL);
        assert(encontrado->codigo == codigo);
        assert(repositorio.itens[codigo - 1UL] == encontrado);
    }
    printf("Dados apos expansao: OK\n");

/*===== TESTANDO EXIBICAO ==========================================*/

    printf("Testando exibicao...\n");
    repositorio_exibir(NULL);
    repositorio_exibir(&nao_inicializado);
    assert(repositorio_inicializar(&vazio) == 1);
    repositorio_exibir(&vazio);
    repositorio_destruir(&vazio);
    repositorio_exibir(&repositorio);
    printf("Exibicao executada: OK\n");

/*===== TESTANDO EXCLUIR ==========================================*/
    printf("Testando destruicao...\n");
    repositorio_destruir(&repositorio);
    assert(repositorio.itens == NULL);
    assert(repositorio.quantidade == 0UL);
    assert(repositorio.capacidade == 0UL);

    atendimento1 = NULL;
    encontrado = NULL;
    novo = NULL;

    repositorio_destruir(&repositorio);
    repositorio_destruir(NULL);

    printf("Destruicao: OK\n");

    printf(
        "Todos os testes do repositorio passaram!\n"
    );

    return 0;
}
