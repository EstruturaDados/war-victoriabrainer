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
/**
 * @struct Territorio
 * @brief Define a estrutura para o território, contendo seu nome, a cor do exército que o domina e o número de tropas.
 */
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa(int qtd);
void liberarMemoria(Territorio* territorios);

// Funções de interface com o usuário:
void cadastrarTerritorios(Territorio* territorios, int qtd);
void exibirMapa(const Territorio* territorios, int qtd);
void exibirMissao(const char* missao);
void exibirMenuPrincipal();

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio* territorios, int qtd);
void atacar(Territorio* atacante, Territorio* defensor);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho);

// Função utilitária:
void limparBufferEntrada();

// --- Vetor de Missões ---
// Contém diferentes objetivos possíveis para o jogador, que serão sorteados no início do jogo.
const char* missoes[] = {
    "Conquistar 3 territorios seguidos",
    "Eliminar todas as tropas da cor Vermelha",
    "Dominar todos os territorios de cor Azul",
    "Manter pelo menos 5 tropas em todos os seus territorios",
    "Conquistar o territorio inimigo com mais tropas"
};
const int TOTAL_MISSOES = 5;


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
    limparBufferEntrada();

    // Alocação dinamica do mapa
    Territorio* territorios = alocarMapa(qtd);
    if (!territorios) {
        printf("Erro ao alocar memoria para o mapa.\n");
        return 1;
    }

    cadastrarTerritorios(territorios, qtd);
    exibirMapa(territorios, qtd);

    char missao[100];
    atribuirMissao(missao, (char**)missoes, TOTAL_MISSOES);
    exibirMissao(missao);


    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
    int opcao;
    do {
        exibirMenuPrincipal();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                // Fase de ataque
                faseDeAtaque(territorios, qtd);
                exibirMapa(territorios, qtd);
                break;
            case 2:
                if (verificarMissao(missao, territorios, qtd)) {
                    printf("Parabens! Voce cumpriu sua missao!\n");
                } else {
                    printf("Missao ainda nao cumprida. Continue tentando!\n");
                }
                break;
            case 0:
                printf("Encerrando o jogo. Obrigado por jogar!\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

        // Pausa para o jogador ler os resultados
        printf("Pressione Enter para continuar...");
        limparBufferEntrada();
        getchar();
    } while (opcao != 0);


    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(territorios);

    return 0;
}

// --- Implementação das Funções ---
/**
 * @brief Aloca dinamicamente a memória para o mapa de territórios.
 * @param qtd A quantidade de territórios a serem alocados.
 * @return Um ponteiro para o mapa de territórios alocado ou NULL em caso de falha.
 */
Territorio* alocarMapa(int qtd) {
    Territorio* mapa = (Territorio*)calloc(qtd, sizeof(Territorio));
    return mapa;
}
    

/**
 * @brief Cadastra os territórios preenchendo seus dados iniciais.
 * @param territorios Ponteiro para o array de territórios a serem cadastrados.
 * @param qtd A quantidade de territórios a serem cadastrados.
 */
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
      limparBufferEntrada();

      printf("\n--- Territorio %d ---\n", i + 1);
  }
}


/**
 * @brief Libera a memória alocada para o mapa de territórios.
 * @param territorios Ponteiro para o array de territórios a ser liberado.
 */
void liberarMemoria(Territorio* territorios) {
    free(territorios);
}


/**
 * @brief Exibe o menu principal do jogo com as opções disponíveis.
 */
void exibirMenuPrincipal() {
    printf("\n============ Menu Principal ============\n");
    printf("1. Atacar um territorio\n");
    printf("2. Verificar missao\n");
    printf("0. Sair do jogo\n");
    printf("========================================\n");
}


/**
 * @brief Exibe o mapa atual com os detalhes de cada território.
 * @param territorios Ponteiro para o array de territórios a serem exibidos.
 * @param qtd A quantidade de territórios no mapa.
 */
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


/**
 * @brief Exibe a missão atual do jogador.
 * @param missao A descrição da missão a ser exibida.
 */
void exibirMissao(const char* missao) {
    printf("\n============ Missao Atual ============\n");
    printf("%s\n", missao);
    printf("=====================================\n");
}


/**
 * @brief Gerencia a fase de ataque, solicitando os territórios de origem e destino ao jogador.
 * @param territorios Ponteiro para o array de territórios.
 * @param qtd A quantidade de territórios no mapa.
 */
void faseDeAtaque(Territorio* territorios, int qtd) {
    int iAtacante, iDefensor;
    printf("\nEscolha o territorio atacante (1-%d): ", qtd);
    scanf("%d", &iAtacante);
    printf("Escolha o territorio defensor (1-%d): ", qtd);
    scanf("%d", &iDefensor);

    atacar(&territorios[iAtacante - 1], &territorios[iDefensor - 1]);
}


/**
 * @brief Executa a lógica de ataque entre dois territórios.
 * @param atacante Ponteiro para o território atacante.
 * @param defensor Ponteiro para o território defensor.
 */
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


/**
 * @brief Atribui uma missão aleatória ao jogador.
 * @param destino Ponteiro para a string onde a missão será armazenada.
 * @param missoes Array de strings contendo as missões disponíveis.
 * @param totalMissoes A quantidade total de missões disponíveis.
 */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
}


/**
 * @brief Verifica se a missão do jogador foi cumprida com base no estado atual do mapa.
 * @param missao A descrição da missão a ser verificada.
 * @param mapa Ponteiro para o array de territórios representando o mapa atual.
 * @param tamanho A quantidade de territórios no mapa.
 * @return 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho) {
    // --- Missão 1: Conquistar 3 territórios ---
    if (strcmp(missao, "Conquistar 3 territorios seguidos") == 0) {
        int conquistados = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Jogador") == 0)
                conquistados++;
        }
        return conquistados >= 3; // 1 se cumpriu, 0 se não
    }

    // --- Missão 2: Eliminar todas as tropas da cor Vermelha ---
    else if (strcmp(missao, "Eliminar todas as tropas da cor Vermelha") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelha") == 0 && mapa[i].tropas > 0)
                return 0; // Ainda há tropas vermelhas
        }
        return 1; // Missão cumprida
    }

    // --- Missão 3: Dominar todos os territórios originalmente Azuis ---
    else if (strcmp(missao, "Dominar todos os territorios de cor Azul") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0 && strcmp(mapa[i].cor, "Jogador") != 0)
                return 0; // Ainda não domina todos
        }
        return 1; // Cumpriu a missão
    }

    // --- Missão 4: Manter pelo menos 5 tropas em todos os territórios ---
    else if (strcmp(missao, "Manter pelo menos 5 tropas em todos os seus territorios") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Jogador") == 0 && mapa[i].tropas < 5)
                return 0; // Algum território com menos de 5 tropas
        }
        return 1; // Todos os territórios têm >= 5 tropas
    }

    // --- Missão 5: Conquistar o território inimigo com mais tropas ---
    else if (strcmp(missao, "Conquistar o territorio inimigo com mais tropas") == 0) {
        static int indiceAlvo = -1;

        // Se ainda não há alvo definido, escolhe o inimigo com mais tropas
        if (indiceAlvo == -1) {
            int maxTropas = -1;
            for (int i = 0; i < tamanho; i++) {
                if (strcmp(mapa[i].cor, "Jogador") != 0 && mapa[i].tropas > maxTropas) {
                    maxTropas = mapa[i].tropas;
                    indiceAlvo = i;
                }
            }
        }

        // Se o jogador passou a dominar o território-alvo, missão cumprida
        if (indiceAlvo != -1 && strcmp(mapa[indiceAlvo].cor, "Jogador") == 0)
            return 1;
        return 0;
    }

    // --- Caso não seja uma missão reconhecida ---
    return 0;
}


/**
 * @brief Limpa o buffer de entrada do teclado (stdin).
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}