/* -------------------------------------------------
			BIBLIOTECAS
   ------------------------------------------------- */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<pthread.h>

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

// Função para o valor aleatório
double real_aleatorio();
// Funções para os arquivos
void cria_arquivo(int tam, char arq[100]);
int carrega_vetor(int tam, double vetor[tam], char arq[100]);
void salva_arquivo(int tam, double vetor[tam], char arq[100]);
// Funções para o método de Merge Sort
void mergeSort(double vetor[], int inicio, int fim);
void merge(double vetor[], int inicio, int meio, int fim);
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
	arq_e[strcspn(arq_e, "\n")] = 0; // Tira o '\n' da string
    strcat(arq_e, ".txt"); // Adiciona a extensão .txt do nome
	
	printf("Digite o nome do arquivo de saida: ");
    fgets(arq_s, 100, stdin); // Saída
	arq_s[strcspn(arq_s, "\n")] = 0; // Tira o '\n' da string
    strcat(arq_s, ".txt"); // Adiciona a extensão .txt do nome

    // Chama a função para criar o arquivo
    cria_arquivo(N, arq_e);
	
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
	printf("\nVetor gravado:\n");

	// Encerra o programa
	pthread_exit((void *)NULL);
}

/* -------------------------------------------------
			DECLARAÇÃO DAS FUNÇÕES
   ------------------------------------------------- */

// Função para gerar números reais aleatórios
double real_aleatorio()
{
    // A multiplicação por 1000 faz com que se gere números reais de 0 a 1000
    return (((double)rand() / (double)RAND_MAX) * 1000);
}

// Função que cria o arquivo
void cria_arquivo(int tam, char arq[100])
{
    // Garante que o número será aleatório
    srand((unsigned)time(NULL));

    // Declaração de variáveis
    int t_nome, i, max = rand() % 10;
    double valor;
    FILE *vetor_real; // Arquivo

    // Abreo o arquivo
    vetor_real = fopen(arq, "w");

    // Cria o vetor aleatório e salva no arquivo
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
	//double vetor[tam];
	char c;
	//vetor = malloc (tam * sizeof(double));
	FILE *vetor_real; // Arquivo

	// Abertura do arquivo para leitura	
	vetor_real = fopen(arq, "r");

	// Leitura de cada item do arquivo para double	
	for (i = 0; i < tam; i++)
	{
		fscanf(vetor_real, "%lf ", &vetor[i]); // Não tá funcionando direito!!
		//fseek(vetor_real, 1, SEEK_CUR);		
		//fscanf(vetor_real, "%c", &c);
	}

	// Fecha o arquivo	
	fclose(vetor_real);

	return 0;
}

// Função para salvar o vetor num arquivo 
void salva_arquivo(int tam, double vetor[tam], char arq[100])
{
	int i;
    FILE *saida; // Arquivo

    // Abreo o arquivo
    saida = fopen(arq, "w");

    // Cria o vetor aleatório e salva no arquivo
    for (i = 0; i < tam; i++)
    {
        fprintf(saida, "%f ", vetor[i]);
    }

    // Fecha o arquivo
    fclose(saida);
}

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
	int i;
	// Carrega a estrutura na thread
	thread_arg *arg = (thread_arg *) var;
	
	// Chama a função para o Merge Sort
	mergeSort(arg->parte, 0, arg->tamanho - 1);
	
	// Encerra a thread
	pthread_exit((void *) NULL);
}

// Para juntar vetores
void merge(double vetor[], int inicio, int meio, int fim)
{
    int i, j, k;
    int n1 = meio - inicio + 1;
    int n2 =  fim - meio; // Para vetores temporários
 
    // Vetores temporários
    double temp_ini[n1], temp_fim[n2];
 
    // Copia os dados do vetor para os vetores temporários
    for (i = 0; i < n1; i++)
        temp_ini[i] = vetor[inicio + i];
    for (j = 0; j < n2; j++)
        temp_fim[j] = vetor[meio + 1+ j];
 
    // Junta os vetores temporários de volta ao vetor original
    i = 0; // Índice do primeiro vetor
    j = 0; // Índice do segundo vetor
    k = inicio; // Índice do vetor original
    while (i < n1 && j < n2)
    {
        if (temp_ini[i] <= temp_fim[j])
        {
            vetor[k] = temp_ini[i];
            i++;
        }
        else
        {
            vetor[k] = temp_fim[j];
            j++;
        }
        k++;
    }
 
    // Copia os elementos faltantes de temp_ini[], se houver
    while (i < n1)
    {
        vetor[k] = temp_ini[i];
        i++;
        k++;
    }
 
    // Copia os elementos faltantes de temp_fim[], se houver
    while (j < n2)
    {
        vetor[k] = temp_fim[j];
        j++;
        k++;
    }
}

// Função específica do Merge Sort
void mergeSort(double vetor[], int inicio, int fim)
{
    if (inicio < fim)
    {
        // Calcula o meio
        int meio = inicio+(fim-inicio)/2;
 
        // Ordena as duas metades
        mergeSort(vetor, inicio, meio);
        mergeSort(vetor, meio+1, fim);
 
	// Une as 2 metades        
	merge(vetor, inicio, meio, fim);
    }
}
