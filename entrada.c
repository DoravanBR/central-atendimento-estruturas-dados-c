/*========================================= INCLUSOES =============================================*/

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "entrada.h"

/*=========================================== FUNCOES ===============================================*/

/*=====================================================================================================
 DESCARTAR RESTANTE DA LINHA
=====================================================================================================*/
static void descartar_restante_linha(void)
{
    int caractere;

    do {
        caractere = getchar();
    } while (caractere != '\n' && caractere != EOF);
}
/*=====================================================================================================
 VERIFICAR SE POSSUI APENAS ESPACO APOS NUMERO
=====================================================================================================*/
static int possui_apenas_espacos(const char *texto)
{
    while (*texto != '\0') {
        if (!isspace((unsigned char)*texto)) {
            return 0;
        }

        texto++;
    }

    return 1;
}
/*=====================================================================================================
 LER TEXTO
=====================================================================================================*/
int entrada_ler_texto(const char *mensagem, char *destino, unsigned long capacidade){
    char *quebra_linha;

    if (mensagem == NULL || destino == NULL || capacidade < 2 || capacidade > INT_MAX) {
        return 0;
    }

    while (1) {
        printf("%s", mensagem);

        if (fgets(destino, (int)capacidade, stdin) == NULL) {
            return 0;
        }

        quebra_linha = strchr(destino, '\n');

        if (quebra_linha == NULL && !feof(stdin)) {
            descartar_restante_linha();
            printf("Entrada muito longa. Tente novamente.\n");
            continue;
        }

        if (quebra_linha != NULL) {
            *quebra_linha = '\0';
        }

        if (destino[0] == '\0' ||
            possui_apenas_espacos(destino)) {
            printf("O texto nao pode ficar vazio.\n");
            continue;
        }

        return 1;
    }
}
/*=====================================================================================================
 LER INTEIRO
=====================================================================================================*/
int entrada_ler_inteiro(const char *mensagem, int valor_minimo, int valor_maximo, int *valor){
    char texto[128];
    char *fim;
    long numero;

    if (mensagem == NULL || valor == NULL || valor_minimo > valor_maximo) {
        return 0;
    }

    while (1) {
        if (!entrada_ler_texto(mensagem, texto, sizeof(texto))) {
            return 0;
        }

        errno = 0;
        fim = NULL;

        numero = strtol(texto, &fim, 10);

        if (fim == texto || errno == ERANGE || !possui_apenas_espacos(fim)) {
            printf("Digite um numero inteiro valido.\n");
            continue;
        }

        if (numero < valor_minimo || numero > valor_maximo) {
            printf("Digite um valor entre %d e %d.\n", valor_minimo, valor_maximo);
            continue;
        }

        *valor = (int)numero;
        return 1;
    } 
}   
/*=====================================================================================================
 LER POSITIVOS
=====================================================================================================*/
int entrada_ler_positivo(const char *mensagem, unsigned long *valor){
    char texto[128];
    char *inicio;
    char *fim;
    unsigned long numero;

    if (mensagem == NULL || valor == NULL) {
        return 0;
    }

    while (1) {
        if (!entrada_ler_texto(mensagem, texto, sizeof(texto))) {
            return 0;
        }

        inicio = texto;

        while (isspace((unsigned char)*inicio)) {
            inicio++;
        }

        if (*inicio == '-') {
            printf("Digite um numero inteiro positivo.\n");
            continue;
        }

        errno = 0;
        fim = NULL;

        numero = strtoul(inicio, &fim, 10);

        if (fim == inicio || errno == ERANGE || !possui_apenas_espacos(fim) || numero == 0) {
            printf("Digite um numero inteiro positivo.\n");
            continue;
        }

        *valor = numero;
        return 1;
    }
}
