#ifndef ENTRADA_H
#define ENTRADA_H

/*================================ PROTOTIPOS DE FUNCOES ===========================================*/

int entrada_ler_texto(const char *mensagem, char *destino, unsigned long capacidade);
int entrada_ler_inteiro(const char *mensagem, int valor_minimo, int valor_maximo, int *valor);
int entrada_ler_positivo(const char *mensagem, unsigned long *valor);



#endif
