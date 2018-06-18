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
#include"mergesort.h" // Método de ordenação Merge Sort

/* -------------------------------------------------
			ESTRUTURA PARA AS THREADS
   ------------------------------------------------- */

typedef struct
{
	int tamanho, posicao;
	double *parte;
} thread_arg;

/* -------------------------------------------------
			PROTÓTIPO DAS FUNÇÕES
   ------------------------------------------------- */

// Função para threads
void *fthread (void *var);
// Função auxiliar
int def_tamanho(int tam, int *resto, int n_thread);

/* -------------------------------------------------
			FUNÇÃO MAIN
   ------------------------------------------------- */

int main (void)
{
	// Delcaração de variáveis
	int N, T, i;
	char arq_e[100], arq_s[100];
	int tam_parte, part_temp, resto;
	thread_arg arg;

	// Inserção do número de itens do vetor
	printf("\nDigite a quantidade de numeros desejada: ");
	scanf("%d", &N);

	// Inserção do número de threads a serem usadas   
	printf("Digite a quantidade de threads desejada (2, 4, 8 ou 16): ");
	scanf("%d", &T);

	// Verifica se foi digitado o valor correto para o número de threads
	while (T != 2 && T != 4 && T != 8 && T != 16)
	{
		printf("Opcao digitada invalida! Digite novamente!");
		printf("\nDigite a quantidade de threads desejada (2, 4, 8 ou 16): ");
    	scanf("%d", &T);
	}

	// Inserção dos nomes do arquivo de entrada
	getchar();
	printf("Digite o nome do arquivo de entrada: ");
	fgets(arq_e, 100, stdin);
	arruma_nome(arq_e);
	
	printf("Digite o nome do arquivo de saida: ");
	fgets(arq_s, 100, stdin); // Saída
	arruma_nome(arq_s);

	// Chama a função para criar o arquivo. Comente caso queira usar o mesmo arquivo
	cria_arquivo(N, arq_e);
	printf("\nVetor aleatorio gravado como %s", arq_e);
	
	// Carrega o vetor com os valores do arquivo	
	double vetor[N];	
	carrega_vetor(N, vetor, arq_e);

	// Operações para dividir o vetor para as threads
	tam_parte = def_tamanho(N, &resto, T);
	
	// Cria as threads de acordo com o número especificado
	pthread_t t_sort[T];
		
	// Loop para iniciar as threads	
	for(i = 0; i < T; i++)
	{		
		// Coloca o tamanho das partes do vetor em uma variável temporária		
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

		// Criação das threads
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
			DECLARAÇÃO DAS FUNÇÕES
   ------------------------------------------------- */

// Função para definir os tamanhos dos espaços de trabalho das threads
int def_tamanho(int tam, int *resto, int n_thread)
{	
	int tam_parte;
	
	tam_parte = tam / n_thread; // Divisão
	*resto = tam % n_thread;	// Resto

	return tam_parte;			// Retorna o resultado da divisão
}

// Função para thread
void *fthread (void *var)
{
	// Carrega a estrutura na thread
	thread_arg *arg = (thread_arg *) var;
	
	// Chama a função para o Merge Sort
	mergeSort(arg->parte, 0, arg->tamanho - 1);
	
	// Encerra a thread
	pthread_exit((void *) NULL);
}
