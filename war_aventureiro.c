// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:
Territorio* alocarMapa(int qtd);
void cadastrarTerritorios(Territorio* territorios, int qtd);
void exibirMapa(const Territorio* territorios, int qtd);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* territorios);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.
    srand(time(NULL)); // Inicializa aleatoriedade

    int qtd;
    printf("Digite o numero de territorios: ");
    scanf("%d", &qtd);

    // Alocação dinamica do mapa
    Territorio* territorios = alocarMapa(qtd);
    if (!territorios) {
        printf("Erro ao alocar memoria para o mapa.\n");
        return 1;
    }

    cadastrarTerritorios(territorios, qtd);
    exibirMapa(territorios, qtd);

    int iAtacante, iDefensor;
    printf("\nEscolha o territorio atacante (1-%d): ", qtd);
    scanf("%d", &iAtacante);
    printf("Escolha o territorio defensor (1-%d): ", qtd);
    scanf("%d", &iDefensor);

    atacar(&territorios[iAtacante - 1], &territorios[iDefensor - 1]);
    exibirMapa(territorios, qtd);

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(territorios);

    return 0;
}

// --- Implementação das Funções ---


// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
Territorio* alocarMapa(int qtd) {
    Territorio* mapa = (Territorio*)calloc(qtd, sizeof(Territorio));
    return mapa;
}

// cadastrarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void cadastrarTerritorios(Territorio* territorios, int qtd) {
  for (int i = 0; i < qtd; i++) {
      printf("============ Cadastro de Territorios ============\n");
      printf("Territorio %d:\n", i + 1);

      printf("Nome: ");
      scanf(" %29[^\n]", territorios[i].nome); // lê string com espaços

      printf("Cor do Exercito: ");
      scanf(" %9[^\n]", territorios[i].cor); // lê string com espaços

      printf("Numero de Tropas: "); 
      scanf("%d", &territorios[i].tropas);

      printf("\n--- Territorio %d ---\n", i + 1);
  }
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(Territorio* territorios) {
    free(territorios);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio* territorios, int qtd) {
    printf("\n============ Mapa Atual ============\n");
    for (int i = 0; i < qtd; i++) {
        printf("Territorio: %d\n", i + 1);
        printf(" - Nome: %s\n", territorios[i].nome);
        printf(" - Cor do Exercito: %s\n", territorios[i].cor);
        printf(" - Numero de Tropas: %d\n", territorios[i].tropas);
        printf("==============================\n");
    }
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque invalido: Ambos os territorios pertencem ao mesmo jogador.\n");
        return;
    }
    if (atacante->tropas < 2) {
        printf("Ataque invalido: Territorio atacante nao possui tropas suficientes.\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n%s (dado: %d) x %s (dado: %d)\n", atacante->nome, dadoAtacante, defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
      printf("Vitoria de %s!\n", atacante->nome);
      strcpy(defensor->cor, atacante->cor);
      defensor->tropas = atacante->tropas / 2; // Move metade das tropas
    } else {
      printf("%s defendeu com sucesso!\n", defensor->nome);
      atacante->tropas--; // Perde uma tropa
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
