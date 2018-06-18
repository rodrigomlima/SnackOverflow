#include "arquivo.h"

// Declaração das funções

// Função para gerar números reais aleatórios
double real_aleatorio()
{
    // A multiplicação por 1000 faz com que se gere números reais de 0 a 1000
    return (((double)rand() / (double)RAND_MAX) * 1000);
}

// Função para arrumar os nomes dos arquivos
void arruma_nome(char arq[])
{
	arq[strcspn(arq, "\n")] = 0; // Tira o '\n' da string
    strcat(arq, ".txt"); // Adiciona a extensão .txt do nome
}

// Função que cria o arquivo
void cria_arquivo(int tam, char arq[100])
{
    // Garante que o número será aleatório
    srand((unsigned)time(NULL));

    // Declaração de variáveis
    int i;
    double valor;
    FILE *vetor_real; // Arquivo

    // Abre o arquivo
    vetor_real = fopen(arq, "w");

    // Cria valores aleatórios e salva no arquivo
    for (i = 0; i < tam; i++)
    {
        valor = real_aleatorio();
        fprintf(vetor_real, "%f ", valor);
    }

    // Fecha o arquivo
    fclose(vetor_real);
}

// Função que carrega os valores do arquivo num vetor
int carrega_vetor(int tam, double vetor[tam], char arq[100])
{
	// Declaração de variáveis	
	int i;
	FILE *vetor_real; // Arquivo

	// Abertura do arquivo para leitura	
	vetor_real = fopen(arq, "r");

	// Leitura de cada item do arquivo para carregar no vetor	
	for (i = 0; i < tam; i++)
	{
		fscanf(vetor_real, "%lf ", &vetor[i]);
	}

	// Fecha o arquivo	
	fclose(vetor_real);

	return 0;
}

void salva_arquivo(int tam, double vetor[tam], char arq[100])
{
	// Declaração de variáveis	
	int i;
    FILE *saida; // Arquivo

    // Abre o arquivo
    saida = fopen(arq, "w");

    // Cria o vetor aleatório e salva no arquivo
    for (i = 0; i < tam; i++)
    {
        fprintf(saida, "%f ", vetor[i]);
    }

    // Fecha o arquivo
    fclose(saida);
}
