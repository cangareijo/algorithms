/*

ALGORITMO FLOOD FILL (PREENCHIMENTO POR ALAGAMENTO) ==================================================

1. INTRODUÇÃO
--------------
O algoritmo Flood Fill, frequentemente traduzido como "preenchimento por alagamento", é um dos métodos clássicos e mais
fundamentais da computação gráfica. O seu principal objetivo é determinar e alterar uma área conectada de nós (ou
píxeis) numa matriz multidimensional. É o algoritmo que serve de base à ferramenta "balde de tinta" (paint bucket)
presente em quase todos os softwares de edição de imagem digitais, como o Microsoft Paint, o Adobe Photoshop ou o
GIMP.

O algoritmo funciona de forma análoga a verter água ou tinta num ponto específico de uma superfície delimitada: o
líquido espalha-se em todas as direções possíveis até encontrar uma barreira ou fronteira de cor diferente, preenchendo
completamente o espaço disponível.


2. PARÂMETROS E INPUTS PRINCIPAIS
---------------------------------
Para que o algoritmo funcione corretamente, são necessários os seguintes elementos estruturais:
- Matriz/Array de Píxeis: A grelha bidimensional que representa a imagem ou o espaço de trabalho.
- Ponto de Partida (x, y): As coordenadas iniciais onde o utilizador clicou ou onde o preenchimento deve começar.
- Cor Alvo (Target Color): A cor original do píxel no ponto de partida. O algoritmo apenas se espalhará por píxeis que
  partilhem exatamente esta cor.
- Cor de Substituição (Replacement Color): A nova cor que será aplicada a todos os píxeis conectados da cor alvo.


3. PASSO A PASSO DO ALGORITMO (LÓGICA ABSTRATA)
-----------------------------------------------
Independentemente da abordagem de programação escolhida, a lógica central do Flood Fill segue estes passos rigorosos:

Passo 1 (Validação de Limites): Verificar se as coordenadas atuais (x, y) estão dentro dos limites válidos da matriz. Se
estiverem fora da imagem, o processo para imediatamente para esse caminho.

Passo 2 (Verificação de Cor): Olhar para a cor do píxel atual. Se a cor for diferente da Cor Alvo, ou se o píxel já
tiver sido alterado para a Cor de Substituição, o processo para.

Passo 3 (Pintura): Se o píxel passar nos testes anteriores, altera-se a sua cor para a Cor de Substituição.

Passo 4 (Propagação/Exploração): Repetir o processo para os píxeis vizinhos. Esta exploração pode ser feita de duas
formas principais:
- Conectividade de 4 vias (4-way): O algoritmo move-se apenas para cima, baixo, esquerda e direita.
- Conectividade de 8 vias (8-way): O algoritmo move-se para todas as direções cardeais e também na diagonal (noroeste,
  nordeste, sudoeste, sudeste).


4. ABORDAGENS DE IMPLEMENTAÇÃO: RECURSIVA VS ITERATIVA
------------------------------------------------------
Existem duas metodologias principais para traduzir este algoritmo em código de programação:

A) Abordagem Recursiva (Depth-First Search - DFS) Esta é a forma mais simples e intuitiva de programar o Flood Fill. O
algoritmo pinta o píxel atual e chama-se a si próprio imediatamente para o píxel vizinho. Ele explora uma linha/caminho
até ao limite máximo antes de retroceder (backtracking) para explorar os outros caminhos vizinhos.
- Vantagem: Código extremamente curto, limpo e fácil de compreender.
- Desvantagem Crítica (Stack Overflow): Como cada passo empilha uma nova chamada de função na memória Stack do sistema,
  imagens grandes ou áreas de preenchimento vastas esgotam rapidamente o espaço da Stack. Isto resulta num erro grave
  que encerra o programa abruptamente (Crash).

B) Abordagem Iterativa com Fila ou Pilha (Breadth-First Search ou Stack-DFS) Para contornar o problema de limites de
memória da Stack, utiliza-se uma estrutura de dados explícita (uma Fila ou uma Pilha) armazenada na memória Heap do
sistema (que é muito maior).
- Processo: O ponto de partida é colocado na fila. Enquanto a fila não estiver vazia, remove-se um píxel, pinta-se, e
  todos os seus vizinhos válidos são adicionados à fila para processamento posterior.
- Vantagem: Extremamente robusto. Consegue processar imagens com milhões de píxeis sem qualquer risco de Stack
  Overflow.
- Desvantagem: Requer mais linhas de código e a gestão manual da estrutura de dados.


5. APLICAÇÕES PRÁTICAS DO FLOOD FILL
------------------------------------
Para além do óbvio "balde de tinta" nos editores de imagem, este algoritmo tem uma enorme variedade de utilizações no
mundo real:
- Desenvolvimento de Videojogos: Criação de geradores de mapas e labirintos, inteligência artificial para verificar
  caminhos disponíveis ou preenchimento de áreas visíveis (Fog of War).
- Visão por Computador e Reconhecimento de Formas: Segmentação de objetos em imagens médicas ou fotografias de satélite,
  isolando uma região específica com base na sua assinatura de cor.
- Resolução de Quebra-Cabeças: Algoritmos para resolver e validar caminhos em jogos como o Campo Minado (Minesweeper),
  onde clicar num espaço vazio revela toda a área limpa adjacente.

*/

#include <stdio.h>
#include <stdbool.h>

#define WIDTH 20
#define HEIGHT 10

void flood_fill(int width, int height, unsigned int pixels[height][width], int x, int y, unsigned int target_color, unsigned int replacement_color) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
    auto current_color = pixels[y][x];
    if (current_color != target_color || current_color == replacement_color) {
        return;
    }
    pixels[y][x] = replacement_color;
    flood_fill(width, height, pixels, x + 1, y, target_color, replacement_color);
    flood_fill(width, height, pixels, x - 1, y, target_color, replacement_color);
    flood_fill(width, height, pixels, x, y + 1, target_color, replacement_color);
    flood_fill(width, height, pixels, x, y - 1, target_color, replacement_color);
}

void print_window(int width, int height, unsigned int pixels[height][width]) {
    for (auto i = 0; i < width + 2; ++i) {
        printf("-");
    }
    printf("\n");

    for (auto y = 0; y < height; ++y) {
        printf("|");
        for (auto x = 0; x < width; ++x) {
            if (pixels[y][x] == 0) {
                printf(" ");
            } else if (pixels[y][x] == 1) {
                printf("#");
            } else if (pixels[y][x] == 2) {
                printf(".");
            }
        }
        printf("|\n");
    }

    for (auto i = 0; i < width + 2; ++i) {
        printf("-");
    }
    printf("\n\n");
}

int main(void) {
    unsigned int window[HEIGHT][WIDTH] = {0};

    for (auto y = 0; y < HEIGHT; ++y) {
        window[y][0] = 1;
        window[y][WIDTH - 1] = 1;
    }
    for (auto x = 0; x < WIDTH; ++x) {
        window[0][x] = 1;
        window[HEIGHT - 1][x] = 1;
    }

    for (auto y = 2; y < HEIGHT - 2; ++y) {
        window[y][5] =  1;
        window[y][14] = 1;
    }
    for (auto x = 5; x < 15; ++x) {
        window[2][x] = 1;
        window[HEIGHT - 3][x] = 1;
    }

    printf("Initial Window Layout:\n");
    print_window(WIDTH, HEIGHT, window);

    auto start_x = 8;
    auto start_y = 4;
    auto target = window[start_y][start_x];
    auto replacement = 2;

    flood_fill(WIDTH, HEIGHT, window, start_x, start_y, target, replacement);

    printf("Window Layout After Flood Fill:\n");
    print_window(WIDTH, HEIGHT, window);

    return 0;
}
