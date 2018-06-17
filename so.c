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
			PROT�TIPO DAS FUN��ES
   ------------------------------------------------- */

// Fun��o para o valor aleat�rio
double real_aleatorio();
// Fun��es para os arquivos
void cria_arquivo(int tam, char arq[100]);
int carrega_vetor(int tam, double vetor[tam], char arq[100]);
void salva_arquivo(int tam, double vetor[tam], char arq[100]);
// Fun��es para o m�todo de Merge Sort
void mergeSort(double vetor[], int inicio, int fim);
void merge(double vetor[], int inicio, int meio, int fim);
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
	arq_e[strcspn(arq_e, "\n")] = 0; // Tira o '\n' da string
    strcat(arq_e, ".txt"); // Adiciona a extens�o .txt do nome
	
	printf("Digite o nome do arquivo de saida: ");
    fgets(arq_s, 100, stdin); // Sa�da
	arq_s[strcspn(arq_s, "\n")] = 0; // Tira o '\n' da string
    strcat(arq_s, ".txt"); // Adiciona a extens�o .txt do nome

    // Chama a fun��o para criar o arquivo
    cria_arquivo(N, arq_e);
	
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
	printf("\nVetor gravado:\n");

	// Encerra o programa
	pthread_exit((void *)NULL);
}

/* -------------------------------------------------
			DECLARA��O DAS FUN��ES
   ------------------------------------------------- */

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
    int t_nome, i, max = rand() % 10;
    double valor;
    FILE *vetor_real; // Arquivo

    // Abreo o arquivo
    vetor_real = fopen(arq, "w");

    // Cria o vetor aleat�rio e salva no arquivo
    for (i = 0; i < tam; i++)
    {
        valor = real_aleatorio();
        fprintf(vetor_real, "%f ", valor);
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
		//fseek(vetor_real, 1, SEEK_CUR);		
		//fscanf(vetor_real, "%c", &c);
	}

	// Fecha o arquivo	
	fclose(vetor_real);

	return 0;
}

// Fun��o para salvar o vetor num arquivo 
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
    }

    // Fecha o arquivo
    fclose(saida);
}

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
	int i;
	// Carrega a estrutura na thread
	thread_arg *arg = (thread_arg *) var;
	
	// Chama a fun��o para o Merge Sort
	mergeSort(arg->parte, 0, arg->tamanho - 1);
	
	// Encerra a thread
	pthread_exit((void *) NULL);
}

// Para juntar vetores
void merge(double vetor[], int inicio, int meio, int fim)
{
    int i, j, k;
    int n1 = meio - inicio + 1;
    int n2 =  fim - meio; // Para vetores tempor�rios
 
    // Vetores tempor�rios
    double temp_ini[n1], temp_fim[n2];
 
    // Copia os dados do vetor para os vetores tempor�rios
    for (i = 0; i < n1; i++)
        temp_ini[i] = vetor[inicio + i];
    for (j = 0; j < n2; j++)
        temp_fim[j] = vetor[meio + 1+ j];
 
    // Junta os vetores tempor�rios de volta ao vetor original
    i = 0; // �ndice do primeiro vetor
    j = 0; // �ndice do segundo vetor
    k = inicio; // �ndice do vetor original
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

// Fun��o espec�fica do Merge Sort
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
