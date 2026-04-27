#include <stdio.h>
#include <stdlib.h>

// Estrutura para tratar colisoes na Tabela Hash (Encadeamento/Chaining)
typedef struct Node {
    int page_number;
    int frame_number;
    struct Node *next;
} Node;

// --- 1. CONFIGURACOES E VARIAVEIS GLOBAIS ---
#define TAM_HASH 7
#define FRAMES_FISICOS 10
#define N1 5 // Nivel 1 da Hierarquica
#define N2 5 // Nivel 2 da Hierarquica

// Estruturas de Dados
int hierarquica[N1][N2];      // Matriz para Paginacao Hierarquica
Node* tabela_hash[TAM_HASH];  // Array de listas para Hash
int tabela_invertida[FRAMES_FISICOS]; // Indice e o Frame, valor e a Pagina

// --- 2. INICIALIZACAO ---
void inicializar_sistemas() {
    int i, j;
    // Inicializa Hierarquica com -1 (indicando vazio/Page Fault)
    for(i = 0; i < N1; i++)
        for(j = 0; j < N2; j++) hierarquica[i][j] = -1;

    // Inicializa Hash
    for(i = 0; i < TAM_HASH; i++) tabela_hash[i] = NULL;

    // Inicializa Invertida (Mapeia RAM fisica) 
    for(i = 0; i < FRAMES_FISICOS; i++) tabela_invertida[i] = -1;
}

// --- 3. IMPLEMENTACAO: PAGINACAO HIERARQUICA ---
// Divide a pagina em dois niveis para economizar espaco
void inserir_hierarquica(int pagina, int frame) {
    int idx1 = pagina / N2; 
    int idx2 = pagina % N2;
    if (idx1 < N1) {
        hierarquica[idx1][idx2] = frame;
        printf("[Hierarquica] Mapeado: L1[%d] L2[%d] -> Frame %d\n", idx1, idx2, frame);
    } else {
        printf("[Hierarquica] Erro: Endereco virtual fora do limite simulado.\n");
    }
}

// --- 4. IMPLEMENTACAO: TABELA HASH ---
// Usa funcao de espalhamento e trata colisoes com lista encadeada
void inserir_hash(int pagina, int frame) {
    int indice = pagina % TAM_HASH;
    Node* novo = (Node*)malloc(sizeof(Node));
    novo->page_number = pagina;
    novo->frame_number = frame;
    novo->next = tabela_hash[indice]; // Insere no topo da lista
    tabela_hash[indice] = novo;
    printf("[Hash] Mapeado: Indice %d -> Pagina %d no Frame %d\n", indice, pagina, frame);
}

int buscar_hash(int pagina) {
    int indice = pagina % TAM_HASH;
    Node* atual = tabela_hash[indice];
    while (atual != NULL) {
        if (atual->page_number == pagina) return atual->frame_number;
        atual = atual->next; // Percorre a lista de colisao 
    }
    return -1; // Page Fault
}

// --- 5. IMPLEMENTACAO: TABELA INVERTIDA ---
// Mapeia a memoria fisica. Tamanho proporcional a RAM
void inserir_invertida(int pagina, int frame) {
    if (frame < FRAMES_FISICOS) {
        tabela_invertida[frame] = pagina;
        printf("[Invertida] Mapeado: Frame %d contem Pagina %d\n", frame, pagina);
    }
}

int buscar_invertida(int pagina) {
    // Busca puramente sequencial no pior caso: O(f) 
    for (int i = 0; i < FRAMES_FISICOS; i++) {
        if (tabela_invertida[i] == pagina) return i;
    }
    return -1; // Page Fault
}

// --- 6. MENU PRINCIPAL ---
int main() {
    int opcao, pg, fr, busca;
    inicializar_sistemas();

    do {
        printf("\n--- EMULADOR DE MEMORIA (UNIVALI) ---");
        printf("\n1. Inserir Mapeamento (Simular Insercao)");
        printf("\n2. Buscar Pagina (Simular Traducao)");
        printf("\n0. Sair\nOpcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            printf("N. Pagina Logica: "); scanf("%d", &pg);
            printf("N. Frame Fisico: "); scanf("%d", &fr);
            inserir_hierarquica(pg, fr);
            inserir_hash(pg, fr);
            inserir_invertida(pg, fr);
        } 
        else if (opcao == 2) {
            printf("Digite a Pagina para busca: "); scanf("%d", &pg);
            
            // Demonstra a busca nas diferentes estruturas
            busca = buscar_hash(pg);
            printf("\n[Resultado Hash]: ");
            if(busca != -1) printf("Frame %d\n", busca); else printf("PAGE FAULT!\n");

            busca = buscar_invertida(pg);
            printf("[Resultado Invertida]: ");
            if(busca != -1) printf("Frame %d\n", busca); else printf("PAGE FAULT!\n");
        }
    } while (opcao != 0);

    return 0;
}