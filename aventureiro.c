#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Estrutura de Dados ---
// Struct para armazenar as informações do território (campos reduzidos para simplicidade)
typedef struct {
    char nome[30]; 
    char cor[10];  
    int tropas;    
} Territorio;

// --- Protótipos das Funções ---
void limpar_buffer();
Territorio* cadastrarTerritorios(int* numTerritorios);
void exibirMapa(Territorio* mapa, int tamanho);
void atacar(Territorio* atacante, Territorio* defensor);
void simularAtaques(Territorio* mapa, int numTerritorios);
void liberarMemoria(Territorio* mapa);

// --- 1. Função Principal ---
int main() {
    // Inicializa a semente para a geração de números aleatórios (rand())
    srand(time(NULL));

    Territorio* mapa = NULL;
    int numTerritorios = 5; // Mantemos 5 como padrão inicial, mas o usuário poderia definir.

    // Cadastro e Alocação Dinâmica
    mapa = cadastrarTerritorios(&numTerritorios);

    if (mapa == NULL) {
        printf("Falha na alocacao de memoria. Encerrando.\n");
        return 1;
    }

    // Loop Interativo de Ataques
    simularAtaques(mapa, numTerritorios);

    // Liberação de Memória
    liberarMemoria(mapa);

    return 0;
}

// --- 2. Implementação das Funções Auxiliares e Modulares ---

// Função auxiliar para limpar o buffer de entrada (necessária após scanf)
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Aloca memória dinamicamente e preenche os dados iniciais dos territórios.
 * @param numTerritorios Ponteiro para o número de territórios (fixo em 5, mas dinâmico).
 * @return Ponteiro para o vetor de Territorios alocado.
 */
Territorio* cadastrarTerritorios(int* numTerritorios) {
    // Requisito: calloc para alocação dinâmica.
    // Usamos calloc para alocar *numTerritorios (5) structs Territorio.
    Territorio* mapa = (Territorio*)calloc(*numTerritorios, sizeof(Territorio));

    if (mapa == NULL) return NULL; // Retorna NULL se a alocação falhar.

    printf("--- Cadastro dos %d Territorios ---\n", *numTerritorios);

    // Loop para preencher os dados, acessando via notação de array [i].
    for (int i = 0; i < *numTerritorios; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        
        // Nome
        printf("  Nome (sem espacos): ");
        if (scanf("%29s", mapa[i].nome) != 1) { // Usa scanf para strings curtas
            limpar_buffer();
            break;
        }
        
        // Cor
        limpar_buffer();
        printf("  Cor: ");
        if (fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin) == NULL) break;
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0'; // Remove o '\n'
        
        // Tropas
        printf("  Tropas (min 1): ");
        while (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 1) {
            limpar_buffer(); 
            printf("Invalido. Tropas (min 1): ");
        }
        limpar_buffer(); 
    }
    return mapa;
}

/**
 * Exibe o estado atual de todos os territórios de forma organizada.
 * @param mapa Ponteiro para o vetor de Territorios.
 * @param tamanho O número total de territórios.
 */
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n--- ESTADO ATUAL DO MAPA ---\n");
    printf("+------------------------------+------------+----------+\n");
    printf("| ID | %-25s | %-10s | %-8s |\n", "NOME", "COR", "TROPAS");
    printf("+----+--------------------------+------------+----------+\n");

    for (int i = 0; i < tamanho; i++) {
        // Acesso aos campos via notação de array (mapa[i])
        printf("| %-2d | %-25s | %-10s | %-8d |\n", 
               i + 1, // ID para seleção
               mapa[i].nome, 
               mapa[i].cor, 
               mapa[i].tropas);
    }
    printf("+------------------------------------------------------+\n");
}

/**
 * Simula a batalha e resolve o resultado (Requisito: Uso de ponteiros).
 * @param atacante Ponteiro para o território atacante.
 * @param defensor Ponteiro para o território defensor.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    // Simulação de dados de ataque/defesa (1 a 6)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- SIMULACAO DE ATAQUE ---\n");
    printf("  %s (%s) ATACA %s (%s)\n", 
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("  Dados: Atacante rolou %d vs Defensor rolou %d\n", dadoAtacante, dadoDefensor);

    // Requisito: Empate favorece o atacante
    if (dadoAtacante >= dadoDefensor) {
        // Vitoria do Atacante
        printf("\nRESULTADO: VITORIA do %s!\n", atacante->nome);
        
        // Requisito: Se vencer, defensor perde 1 tropa
        defensor->tropas--;
        printf("  > %s perde 1 tropa (Tropas restantes: %d).\n", defensor->nome, defensor->tropas);
        
        // Requisito: Se defensor perde todos → território é conquistado
        if (defensor->tropas <= 0) {
            printf("  *** TERRITORIO CONQUISTADO! ***\n");
            
            // Defensor muda de cor/dono
            strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
            defensor->cor[sizeof(defensor->cor) - 1] = '\0';
            
            // Atacante move 1 tropa para o novo território (ou mais, dependendo da regra)
            // Simplificado: Atacante perde 1 tropa, que se torna a tropa do novo defensor
            if (atacante->tropas > 1) {
                atacante->tropas--;
                defensor->tropas = 1; // Nova tropa do conquistador
            } else {
                 // Atacante não pode perder sua última tropa
                 defensor->tropas = 1; 
            }
            printf("  > Novo dono: %s. %s agora tem 1 tropa.\n", defensor->cor, defensor->nome);
        }
        
    } else {
        // Derrota do Atacante (Defensor vence)
        printf("\nRESULTADO: DERROTA do %s. A defesa foi bem-sucedida.\n", atacante->nome);
        // Regra adicional comum: Atacante perde 1 tropa em caso de derrota.
        if (atacante->tropas > 1) {
            atacante->tropas--;
            printf("  > %s perde 1 tropa na investida.\n", atacante->nome);
        } else {
            printf("  > %s nao pode perder sua ultima tropa.\n", atacante->nome);
        }
    }
}

/**
 * Controla o loop interativo para o jogador escolher territórios.
 */
void simularAtaques(Territorio* mapa, int numTerritorios) {
    char continuar;
    int idxA, idxD;

    do {
        exibirMapa(mapa, numTerritorios);
        
        printf("\n--- FASE DE ATAQUE INTERATIVA ---\n");
        printf("Escolha os territorios pelo ID (1 a %d). Digite 0 para sair.\n", numTerritorios);
        
        printf("ID do Territorio ATACANTE: ");
        if (scanf("%d", &idxA) != 1 || idxA == 0) break;
        limpar_buffer();
        
        printf("ID do Territorio DEFENSOR: ");
        if (scanf("%d", &idxD) != 1) break;
        limpar_buffer();

        idxA--; // Ajuste para índice base 0
        idxD--; 

        // Validação básica de limites e auto-ataque
        if (idxA < 0 || idxA >= numTerritorios || idxD < 0 || idxD >= numTerritorios || idxA == idxD) {
            printf("Selecao invalida. Tente novamente.\n");
            continue;
        }

        // Validação de tropas e cores
        if (mapa[idxA].tropas < 2) {
             printf("Territorio atacante precisa de no minimo 2 tropas para atacar.\n");
             continue;
        }
        if (strcmp(mapa[idxA].cor, mapa[idxD].cor) == 0) {
            printf("Nao e possivel atacar um territorio da mesma cor (%s).\n", mapa[idxA].cor);
            continue;
        }
        
        // Chamada da função de ataque (passando ponteiros)
        atacar(&mapa[idxA], &mapa[idxD]);
        
        printf("\nDeseja realizar outro ataque? (S/N): ");
        scanf(" %c", &continuar);
        limpar_buffer(); 

    } while (continuar == 'S' || continuar == 's');
}

/**
 * Libera a memória alocada dinamicamente (Requisito: free).
 * @param mapa Ponteiro para a memória a ser liberada.
 */
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemoria alocada para o mapa liberada. Programa encerrado.\n");
    }
}