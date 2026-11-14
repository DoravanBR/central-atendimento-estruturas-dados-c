/*========================================= INCLUSOES =============================================*/

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "entrada.h"

/*========================================= TESTES =============================================*/

int main(void)
{
    char texto[20];
    int inteiro;
    unsigned long positivo;

    FILE *arquivo_entrada;

    arquivo_entrada = freopen("testes/dados_entrada.txt", "r", stdin);
    assert(arquivo_entrada != NULL);

    printf("Iniciando testes do modulo de entrada...\n");

    strcpy(texto, "inalterado");
    inteiro = 99;
    positivo = 99;

/*============================== TESTANDO ARGUMENTOS INVALIDOS ==================================*/

    printf("Testando argumentos invalidos...\n");

/*=================================== VALIDACAO LER TEXTO =======================================*/ 

    printf("Mensagem Nula\n");
    assert(entrada_ler_texto(NULL, texto, sizeof(texto)) == 0);
    printf("Destino Nulo\n");
    assert(entrada_ler_texto("Texto: ", NULL, sizeof(texto)) == 0);
    printf("Capacidade insuficiente\n"); // minimo 2 -> [0] = caractere [1] = \0 final de string
    assert(entrada_ler_texto("Texto: ", texto, 1) == 0);
    printf("Preservacao do conteudo\n");
    assert(strcmp(texto, "inalterado") == 0);
    printf("=====================================================================\n");
    printf("Testes OK !!\n");
    printf("======================================================================\n\n");

/*=================================== VALIDACAO LER INTEIRO =======================================*/ 

    printf("Mensagem Nula\n");
    assert(entrada_ler_inteiro(NULL, 1, 10, &inteiro) == 0);
    printf("Minimo > Maximo\n");
    assert(entrada_ler_inteiro("Inteiro: ",10,1,&inteiro) == 0);
    printf("Destino Nulo\n");
    assert(entrada_ler_inteiro("Inteiro: ",1,10,NULL) == 0);
    printf("=====================================================================\n");
    printf("Testes OK !!\n");
    printf("======================================================================\n\n");

/*=================================== VALIDACAO LER POSITIVO =======================================*/ 

    printf("Mensagem Nula\n");
    assert(entrada_ler_positivo(NULL, &positivo) == 0);
    printf("Destino Nulo\n");
    assert(entrada_ler_positivo("Positivo: ", NULL) == 0);
    printf("Verificando inalteracao de variavel sentinela");
    assert(positivo == 99);
    printf("Argumentos invalidos: OK\n");

/*=================================== TESTE DE LEITURA =======================================*/

/*=================================== LER TEXTO =======================================*/

    printf("Testando leitura de texto valido...\n");
    assert(entrada_ler_texto("Digite um texto: ",texto,sizeof(texto)) == 1);
    assert(strcmp(texto, "Ana Silva") == 0);
    printf("Leitura de texto valido: OK\n");

    printf("Testando rejeicao de texto vazio...\n");
    assert(entrada_ler_texto("Digite um texto nao vazio: ",texto,sizeof(texto)) == 1);
    assert(strcmp(texto, "Carlos") == 0);
    printf("Rejeicao de texto vazio: OK\n");

/*=================================== LER INTEIRO =======================================*/

    printf("Testando leitura de inteiro valido...\n");
    assert(entrada_ler_inteiro("Digite um inteiro entre 1 e 10: ", 1, 10, &inteiro) == 1);
    assert(inteiro == 5);
    printf("Leitura de inteiro valido: OK\n");

    printf("Testando rejeicao de inteiros invalidos...\n");
    inteiro = 99;
    assert(entrada_ler_inteiro("Digite um inteiro entre 1 e 10: ", 1, 10, &inteiro) == 1);
    assert(inteiro == 7);
    printf("Rejeicao de inteiros invalidos: OK\n");

/*=================================== LER POSITIVO =======================================*/

    printf("Testando leitura de positivo valido...\n");
    assert(entrada_ler_positivo("Digite um numero positivo: ", &positivo) == 1);
    assert(positivo == 25UL);
    printf("Leitura de positivo valido: OK\n");

    printf("Testando rejeicao de positivos invalidos...\n");
    positivo = 99UL;
    assert(entrada_ler_positivo("Digite um numero positivo: ", &positivo) == 1);
    assert(positivo == 42UL);
    printf("Rejeicao de positivos invalidos: OK\n");

/*======================================================================================*/

    printf("Todos os testes de entrada passaram!\n");

    return 0;
}

