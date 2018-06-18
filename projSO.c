/* -------------------------------------------------
			BIBLIOTECAS
   ------------------------------------------------- */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<pthread.h>
// Criadas
#include"arquivo.h" // Para manipular os arquivos
#include"mergesort.h" // M�todo de ordena��o Merge Sort

/* -------------------------------------------------
			ESTRUTURA PARA AS THREADS
   ------------------------------------------------- */

typedef struct
{
	int tamanho, posicao;
	double *parte;
} thread_arg;

/* -------------------------------------------------
			PROT�TIPO DAS FUN��ES
   ------------------------------------------------- */

// Fun��o para threads
void *fthread (void *var);
// Fun��o auxiliar
int def_tamanho(int tam, int *resto, int n_thread);

/* -------------------------------------------------
			FUN��O MAIN
   ------------------------------------------------- */

int main (void)
{
	// Delcara��o de vari�veis
	int N, T, i;
	char arq_e[100], arq_s[100];
	int tam_parte, part_temp, resto;
	thread_arg arg;

	// Inser��o do n�mero de itens do vetor
	printf("\nDigite a quantidade de numeros desejada: ");
	scanf("%d", &N);

	// Inser��o do n�mero de threads a serem usadas   
	printf("Digite a quantidade de threads desejada (2, 4, 8 ou 16): ");
	scanf("%d", &T);

	// Verifica se foi digitado o valor correto para o n�mero de threads
	while (T != 2 && T != 4 && T != 8 && T != 16)
	{
		printf("Opcao digitada invalida! Digite novamente!");
		printf("\nDigite a quantidade de threads desejada (2, 4, 8 ou 16): ");
    	scanf("%d", &T);
	}

	// Inser��o dos nomes do arquivo de entrada
	getchar();
	printf("Digite o nome do arquivo de entrada: ");
	fgets(arq_e, 100, stdin);
	arruma_nome(arq_e);
	
	printf("Digite o nome do arquivo de saida: ");
	fgets(arq_s, 100, stdin); // Sa�da
	arruma_nome(arq_s);

	// Chama a fun��o para criar o arquivo. Comente caso queira usar o mesmo arquivo
	cria_arquivo(N, arq_e);
	printf("\nVetor aleatorio gravado como %s", arq_e);
	
	// Carrega o vetor com os valores do arquivo	
	double vetor[N];	
	carrega_vetor(N, vetor, arq_e);

	// Opera��es para dividir o vetor para as threads
	tam_parte = def_tamanho(N, &resto, T);
	
	// Cria as threads de acordo com o n�mero especificado
	pthread_t t_sort[T];
		
	// Loop para iniciar as threads	
	for(i = 0; i < T; i++)
	{		
		// Coloca o tamanho das partes do vetor em uma vari�vel tempor�ria		
		part_temp = tam_parte;		
		if (resto != 0) // Para distribuir o resto para cada thread
		{
			part_temp++;
			resto--;
		}
		// Passa os valores para struct
		arg.tamanho = part_temp;
		arg.posicao = i;
		arg.parte = &vetor[part_temp * i];

		// Cria��o das threads
		pthread_create(&t_sort[i], NULL, fthread, (void *)&arg);
	}
		
	// Espera as threads acabarem
	for(i = 0; i < T; i++)
		pthread_join(t_sort[i], NULL);

	// Une o resto usando mais uma vez Merge Sort
	mergeSort(vetor, 0, N - 1);

	// Salva o arquivo
	salva_arquivo(N, vetor, arq_s);
	printf("\nVetor ordenado gravado como %s\n", arq_s);

	// Encerra o programa
	pthread_exit((void *)NULL);
}

/* -------------------------------------------------
			DECLARA��O DAS FUN��ES
   ------------------------------------------------- */

// Fun��o para definir os tamanhos dos espa�os de trabalho das threads
int def_tamanho(int tam, int *resto, int n_thread)
{	
	int tam_parte;
	
	tam_parte = tam / n_thread; // Divis�o
	*resto = tam % n_thread;	// Resto

	return tam_parte;			// Retorna o resultado da divis�o
}

// Fun��o para thread
void *fthread (void *var)
{
	// Carrega a estrutura na thread
	thread_arg *arg = (thread_arg *) var;
	
	// Chama a fun��o para o Merge Sort
	mergeSort(arg->parte, 0, arg->tamanho - 1);
	
	// Encerra a thread
	pthread_exit((void *) NULL);
}
