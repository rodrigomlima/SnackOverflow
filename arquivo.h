#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

// Função para o valor aleatório
double real_aleatorio();

// Funções para os arquivos
void cria_arquivo(int tam, char arq[100]);
int carrega_vetor(int tam, double vetor[tam], char arq[100]);
void salva_arquivo(int tam, double vetor[tam], char arq[100]);

// Funções auxiliares
void arruma_nome(char arq[]);
