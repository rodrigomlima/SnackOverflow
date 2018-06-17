#include "arquivo.h"

// Declara��o das fun��es

// Fun��o para gerar n�meros reais aleat�rios
double real_aleatorio()
{
    // A multiplica��o por 1000 faz com que se gere n�meros reais de 0 a 1000
    return (((double)rand() / (double)RAND_MAX) * 1000);
}

// Fun��o que cria o arquivo
void cria_arquivo(int tam, char arq[100])
{
    // Garante que o n�mero ser� aleat�rio
    srand((unsigned)time(NULL));

    // Declara��o de vari�veis
    int i;
    double valor;
    FILE *vetor_real; // Arquivo

    // Abreo o arquivo
    vetor_real = fopen(arq, "w");

    // Cria o vetor aleat�rio e salva no arquivo
    for (i = 0; i < tam; i++)
    {
        valor = real_aleatorio();
        fprintf(vetor_real, "%f ", valor);
        printf("%d - %f\n", i, valor);
    }

    // Fecha o arquivo
    fclose(vetor_real);
}

// Fun��o que carrega os valores do arquivo num vetor
int carrega_vetor(int tam, double vetor[tam], char arq[100])
{
	// Declara��o de vari�veis	
	int i;	
	//double vetor[tam];
	char c;
	//vetor = malloc (tam * sizeof(double));
	FILE *vetor_real; // Arquivo

	// Abertura do arquivo para leitura	
	vetor_real = fopen(arq, "r");

	// Leitura de cada item do arquivo para double	
	for (i = 0; i < tam; i++)
	{
		fscanf(vetor_real, "%lf ", &vetor[i]); // N�o t� funcionando direito!!
	}

	// Fecha o arquivo	
	fclose(vetor_real);

	return 0;
}

void salva_arquivo(int tam, double vetor[tam], char arq[100])
{
	int i;
    FILE *saida; // Arquivo

    // Abreo o arquivo
    saida = fopen(arq, "w");

    // Cria o vetor aleat�rio e salva no arquivo
    for (i = 0; i < tam; i++)
    {
        fprintf(saida, "%f ", vetor[i]);
        printf("%d - %f\n", i, vetor[i]);
    }

    // Fecha o arquivo
    fclose(saida);
}
