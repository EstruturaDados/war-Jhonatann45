#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Definições e Estruturas ---
#define MAX_MISSAO_LEN 100

// Struct para o Território
typedef struct {
    char nome[30]; 
    char cor[10];  
    int tropas;    
} Territorio;

// Struct para o Jogador (guarda a missão alocada dinamicamente)
typedef struct {
    char cor[10];
    char* missao; // Ponteiro para a missão
} Jogador;

// Vetor de Missões
char* MISSOES[] = {
    "Controlar 3 territorios.",
    "Eliminar todas as tropas da cor Azul.",
    "Ter a maior quantidade de tropas no mapa.",
    "Conquistar o territorio com o nome mais longo.",
    "Controlar 3 territorios da cor Vermelha.",
};
const int TOTAL_MISSOES = sizeof(MISSOES) / sizeof(MISSOES[0]);

// --- Funções Auxiliares ---

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Libera toda a memória alocada dinamicamente.
void liberarMemoria(Territorio* mapa, Jogador* jogador) {
    if (mapa) free(mapa);
    if (jogador->missao) free(jogador->missao);
    printf("\nMemoria liberada. Encerrando.\n");
}

// Sorteia e copia a missão para o destino (Passagem por referência).
void atribuirMissao(char** missaoDestino) {
    int indiceSorteado = rand() % TOTAL_MISSOES;
    // Cópia da string da missão para a memória alocada do jogador.
    strcpy(*missaoDestino, MISSOES[indiceSorteado]);
}

// Exibe o estado atual dos territórios.
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n--- ESTADO DO MAPA ---\n");
    printf("| ID | %-15s | %-10s | %-5s |\n", "NOME", "DONO", "TROPAS");
    printf("----------------------------------------\n");
    for (int i = 0; i < tamanho; i++) {
        printf("| %-2d | %-15s | %-10s | %-5d |\n", 
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Simula a batalha e atualiza os dados (Uso de ponteiros).
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    printf("\n[ATAQUE] %s (%d) vs %s (%d)\n", atacante->nome, dadoA, defensor->nome, dadoD);

    if (dadoA > dadoD) { // Atacante vence
        defensor->tropas--;
        printf("VITORIA. %s perde 1 tropa (Restantes: %d).\n", defensor->nome, defensor->tropas);
        
        if (defensor->tropas <= 0) {
            printf("Territorio %s CONQUISTADO!\n", defensor->nome);
            // Muda o dono
            strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
            // Move 1 tropa
            if (atacante->tropas > 1) {
                atacante->tropas--;
                defensor->tropas = 1;
            } else {
                 defensor->tropas = 1; 
            }
        }
    } else { // Defensor vence ou Empate
        if (atacante->tropas > 1) {
            atacante->tropas--;
            printf("DERROTA. %s perde 1 tropa.\n", atacante->nome);
        } else {
             printf("DEFESA SUCEDIDA. Atacante nao perde tropas.\n");
        }
    }
}

// Verifica se a missão foi cumprida (Passagem por referência para a missão).
int verificarMissao(char* missao, Territorio* mapa, int tamanho, Jogador* jogador) {
    int count = 0;
    
    // Lógica 1: "Controlar X territorios." (Simplificada para 3)
    if (strstr(missao, "Controlar 3 territorios") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, jogador->cor) == 0) count++;
        }
        if (count >= 3) return 1;
    }
    
    // Lógica 2: "Eliminar todas as tropas da cor Azul."
    if (strstr(missao, "Eliminar todas as tropas da cor Azul") != NULL) {
        int azulPresente = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0) {
                azulPresente = 1;
                break;
            }
        }
        if (!azulPresente) return 1; // Se a cor Azul não está mais no mapa.
    }

    return 0; // Missão não cumprida
}

// --- Função Principal ---
int main() {
    srand(time(NULL)); 

    // Configuração do Jogo
    int numTerritorios = 5;
    Territorio* mapa = (Territorio*)calloc(numTerritorios, sizeof(Territorio));
    if (mapa == NULL) return 1;

    // Configuração do Jogador e Alocação da Missão (Requisito: malloc)
    Jogador jogador1 = { .cor = "Vermelha" };
    jogador1.missao = (char*)malloc(MAX_MISSAO_LEN * sizeof(char));
    if (jogador1.missao == NULL) { free(mapa); return 1; }

    // Dados iniciais (Hardcoded para manter a simplicidade)
    strncpy(mapa[0].nome, "A1", 30); strncpy(mapa[0].cor, "Vermelha", 10); mapa[0].tropas = 3;
    strncpy(mapa[1].nome, "A2", 30); strncpy(mapa[1].cor, "Azul", 10); mapa[1].tropas = 4;
    strncpy(mapa[2].nome, "B1", 30); strncpy(mapa[2].cor, "Vermelha", 10); mapa[2].tropas = 2;
    strncpy(mapa[3].nome, "B2", 30); strncpy(mapa[3].cor, "Verde", 10); mapa[3].tropas = 5;
    strncpy(mapa[4].nome, "C1", 30); strncpy(mapa[4].cor, "Azul", 10); mapa[4].tropas = 3;

    // Atribuição da Missão
    atribuirMissao(&jogador1.missao);
    printf("--- JOGO INICIADO ---\n");
    printf("Sua Cor: %s | MISSAO: %s\n", jogador1.cor, jogador1.missao);
    
    // Loop do Jogo
    char continuar = 'S';
    int idxA, idxD;

    do {
        exibirMapa(mapa, numTerritorios);
        
        // Verificação da Missão (Requisito: Exibição condicional)
        if (verificarMissao(jogador1.missao, mapa, numTerritorios, &jogador1)) {
            printf("\n*** MISSAO CUMPRIDA! JOGADOR %s VENCEU! ***\n", jogador1.cor);
            break;
        }

        // Ataque Interativo
        printf("\nFASE DE ATAQUE: ID ATACANTE (sua cor: %s) e ID DEFENSOR (0 para sair).\n", jogador1.cor);
        printf("ID Atacante: ");
        if (scanf("%d", &idxA) != 1 || idxA == 0) break;
        limpar_buffer();
        printf("ID Defensor: ");
        if (scanf("%d", &idxD) != 1) break;
        limpar_buffer();

        idxA--; idxD--; 

        // Validação Mínima
        if (idxA >= 0 && idxA < numTerritorios && idxD >= 0 && idxD < numTerritorios && idxA != idxD &&
            mapa[idxA].tropas > 1 && strcmp(mapa[idxA].cor, jogador1.cor) == 0 && 
            strcmp(mapa[idxA].cor, mapa[idxD].cor) != 0) 
        {
            // Executa o Ataque (Passando ponteiros)
            atacar(&mapa[idxA], &mapa[idxD]);
        } else {
            printf("Ataque invalido (verifique IDs, tropas ou cores).\n");
        }
        
        printf("Continuar? (S/N): ");
        scanf(" %c", &continuar);
        limpar_buffer(); 

    } while (continuar == 'S' || continuar == 's');

    // Liberação de Memória
    liberarMemoria(mapa, &jogador1);

    return 0;
}