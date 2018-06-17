#include"mergesort.h"

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
