#include <stdio.h>
#include <string.h>

// Define o tamanho fixo do vetor de territórios.
#define NUM_TERRITORIOS 5

// --- Definição da Estrutura ---
// A struct armazena os dados de um único território.
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Função auxiliar para limpar o buffer (crucial após scanf)
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Função Principal ---
int main() {
    // Declaração do vetor estático de 5 elementos.
    Territorio mapa[NUM_TERRITORIOS];
    int i;

    printf("--- Cadastro de %d Territorios ---\n", NUM_TERRITORIOS);

    // 1. Loop para Cadastro dos Dados
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\nTerritorio %d:\n", i + 1);

        // Nome e Cor (usando fgets para ler strings, incluindo espacos, de forma segura)
        printf("  Nome: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0'; // Remove o \n
        
        printf("  Cor do Exercito: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0'; // Remove o \n

        // Tropas (usando scanf para inteiros)
        printf("  Numero de Tropas: ");
        if (scanf("%d", &mapa[i].tropas) != 1) {
            printf("Erro: Entrada de tropas invalida.\n");
            return 1;
        }
        
        // Limpa o buffer para a proxima iteracao do fgets
        limpar_buffer();
    }

    // 2. Exibição dos Dados
    printf("\n\n--- Estado Atual do Mapa ---\n");
    
    // Cabeçalho e separador
    printf("+--------------------------------+------------+----------+\n");
    printf("| %-30s | %-10s | %-8s |\n", "Territorio", "Cor", "Tropas");
    printf("+--------------------------------+------------+----------+\n");

    // Loop para exibir cada território
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("| %-30s | %-10s | %-8d |\n", 
               mapa[i].nome, 
               mapa[i].cor, 
               mapa[i].tropas);
    }
    printf("+--------------------------------+------------+----------+\n");

    return 0;
}