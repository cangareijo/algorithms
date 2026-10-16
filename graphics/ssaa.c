/*

Escreve um artigo longo sobre o antisserrilhado por superamostragem.
Escreve em português europeu.
Escreve em formato de texto simples.
Escreve em Unicode.

*/

/*

O Antisserrilhado por Superamostragem (Super Sampling Anti-Aliasing ou SSAA) representa o auge da qualidade visual na
renderização de gráficos 3D por computador. Embora seja uma das técnicas mais antigas para combater o efeito
de "serrilhado" (aliasing), continua a ser a referência absoluta (gold standard) em termos de fidelidade de imagem.

Este artigo explora em detalhe o que é o SSAA, o problema matemático que visa resolver, o seu mecanismo de
funcionamento, o seu impacto no desempenho e o seu lugar no panorama atual da computação gráfica.

1. O Problema do Serrilhado (Aliasing)

Para compreender a superamostragem, é necessário primeiro entender o problema que ela combate. O serrilhado é um
artefacto visual que surge devido à natureza discreta dos ecrãs modernos. Os ecrãs são compostos por uma matriz de
pontos fixos chamados píxeis.

Quando uma placa gráfica (GPU) tenta desenhar uma linha diagonal ou uma curva suave usando uma grelha de píxeis
quadrados, ocorre uma perda de informação. A GPU precisa de decidir se um píxel individual pertence ao objeto ou ao
fundo. Se o centro do píxel estiver dentro do objeto, ele assume a cor do objeto; se estiver fora, assume a cor do
fundo.

O resultado desta decisão binária é o aparecimento de "escadas" visíveis nas arestas dos objetos, um efeito visual
geométrico desagradável conhecido como "serrilhado". Além disso, em objetos em movimento ou texturas complexas, este
fenómeno pode causar cintilação (shimmering) e ruído visual temporário.

2. O que é o Antisserrilhado por Superamostragem (SSAA)?

O SSAA resolve o serrilhado através da força bruta. A premissa subjacente é simples: se o serrilhado é causado por uma
resolução insuficiente para capturar os detalhes de uma cena, a solução é renderizar a cena a uma resolução muito
superior àquela que o ecrã consegue exibir.

Por exemplo, num ecrã com uma resolução nativa de 1920x1080 (Full HD), um fator de superamostragem de 4x (SSAA 4x) força
a placa gráfica a calcular a cena a uma resolução de 3840x2160 (4K Ultra HD). Cada píxel individual do ecrã final passa
a corresponder a um bloco de quatro subpíxeis na memória da GPU.

3. O Processo de Funcionamento: Passo a Passo

O processo do SSAA pode ser dividido em três etapas principais:

Passo 1: Superamostragem (Supersampling)

A cena 3D é renderizada internamente numa resolução artificialmente elevada. A GPU calcula a cor, a profundidade e a
iluminação para cada um dos subpíxeis desta grelha expandida.

Passo 2: Filtragem e Resolução (Downsampling / Resolving)

Antes de enviar a imagem para o ecrã, a GPU aplica um filtro de reconstrução para converter a imagem de alta resolução
de volta para a resolução nativa do monitor. O método mais comum é a média aritmética: as cores dos subpíxeis que
compõem um píxel nativo são somadas e divididas pelo número total de subpíxeis.

Passo 3: Exibição

O píxel no ecrã recebe a cor resultante desta média. Se uma aresta diagonal passava exatamente a meio de um píxel, os
subpíxeis capturaram essa transição. A cor final do píxel será uma mistura perfeita (um tom intermédio) entre a cor do
objeto e a cor do fundo, criando a ilusão ótica de uma linha perfeitamente suave para o olho humano.

4. Tipos de Grelhas de Amostragem

A eficácia do SSAA depende em grande parte da forma como os subpíxeis são dispostos. Existem diferentes padrões de
amostragem:

* Grelha Regular (Ordered Grid): Os subpíxeis são distribuídos uniformemente dentro do píxel. É o método mais fácil de
  implementar, mas é menos eficaz em linhas quase horizontais ou verticais.

* Grelha Rotativa (Rotated Grid): A grelha de subpíxeis é rodada num ângulo específico (geralmente próximo de 27 graus).
  Isto melhora drasticamente a captura de arestas em quase todas as inclinações.

* Amostragem Estocástica ou Aleatória (Stochastic/Random Sampling): Os subpíxeis são distribuídos de forma
  pseudoaleatória. Transforma o serrilhado em ruído visual difuso, o que é frequentemente mais tolerável para a
  perceção humana, sendo muito utilizado em renderizadores de cinema (Ray Tracing de produção).

5. Vantagens do SSAA

O SSAA oferece benefícios que outras técnicas modernas de antisserrilhado (como o MSAA, FXAA ou TAA) não conseguem
replicar na totalidade:

* Qualidade de Imagem Incomparável: Suaviza todas as arestas geométricas da cena de forma uniforme.

* Tratamento de Texturas: Ao contrário de técnicas que apenas atuam nas arestas dos polígonos, o SSAA melhora a nitidez
  de texturas inteiras e reduz o aliasing interno (como em grelhas, folhagem ou texturas repetitivas ao longe).

* Correção de Elementos Transparentes: Elementos que usam texturas alfa (transparências), como cabelos, folhas de
  árvores ou redes metálicas, beneficiam imenso do SSAA, eliminando o aspeto pixelizado que costumam apresentar.

6. O Custo do Desempenho: A Desvantagem Crítica

Apesar da sua perfeição visual, o SSAA caiu em desuso na maioria das aplicações em tempo real devido ao seu custo
computacional extremo.

* Consumo de Memória (VRAM): Armazenar buffers de cor e profundidade a resoluções multiplicadas requer uma quantidade
  massiva de memória de vídeo.

* Largura de Banda de Memória: Transferir este volume gigante de dados entre os componentes da GPU cria estrangulamentos
  severos.

* Processamento de Fragmentos: Os sombreadores de píxeis (pixel shaders) precisam de ser executados para cada subpíxel.
  Num modo SSAA 4x, a GPU trabalha quatro vezes mais por cada fotograma. Isto resulta numa queda drástica na taxa de
  fotogramas por segundo (FPS).

7. O Legado e o Cenário Atual

Devido à sua exigência de hardware, a indústria desenvolveu alternativas mais eficientes. O MSAA (Multi-Sampling
Anti-Aliasing) surgiu para aplicar a superamostragem apenas às arestas dos polígonos, poupando processamento no
interior dos objetos. Mais tarde, surgiram soluções de pós-processamento baseadas em filtros de imagem (FXAA, SMAA) e
soluções temporais (TAA) que utilizam informação de fotogramas anteriores.

Atualmente, o espírito do SSAA sobrevive através de tecnologias conhecidas como SSAA do Painel de Controlo,
especificamente o DSR (Dynamic Super Resolution) da NVIDIA e o VSR (Virtual Super Resolution) da AMD. Estas tecnologias
permitem que o utilizador force o sistema operativo e os jogos a renderizar a resoluções superiores (como 4K num
monitor 1080p), aplicando depois um filtro de redução de alta qualidade.

Conclusão

O Antisserrilhado por Superamostragem representa a abordagem ideal e sem compromissos para a qualidade de imagem
digital. Embora a computação gráfica moderna tenha migrado para algoritmos mais inteligentes e eficientes (incluindo a
reconstrução por Inteligência Artificial, como DLSS e FSR), o SSAA permanece como o padrão ouro teórico contra o qual
todas as outras técnicas de eliminação de serrilhado são medidas.

*/

/*

Escreve uma função que desenha um círculo preenchido numa tela.
Escreve uma função que aplica antisserrilhado por superamostragem a uma tela.
Escreve um programa que desenha 20 círculos preenchidos aleatórias numa janela.
Quero que seja possível ativar e destivar o antisserrilhado alterando um booleano.
Escreve o código em C23 usando a SDL3.
Usa bibliotecas o mínimo possível.
Escreve o código em português europeu.

*/

#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Configurações da Janela
#define JANELA_LARGURA 800
#define JANELA_ALTURA 600

// Ativar/Desativar Antisserrilhado (SSAA 4x) alterando este booleano
const bool ATIVAR_ANTISSERRILHADO = true;

// Estrutura para representar a tela de desenho (Buffer de Píxeis RGBA8888)
typedef struct {
    Uint32* pixeis;
    int largura;
    int altura;
} Tela;

// Função estática auxiliar para desenhar linhas horizontais de preenchimento do círculo
static inline void desenhar_linha_horizontal(Tela* tela, int x1, int x2, int linha_y, Uint32 cor) {
    if (linha_y < 0 || linha_y >= tela->altura) return;
    if (x1 < 0) x1 = 0;
    if (x2 >= tela->largura) x2 = tela->largura - 1;
    
    for (int i = x1; i <= x2; i++) {
        tela->pixeis[linha_y * tela->largura + i] = cor;
    }
}

// Função que desenha um círculo preenchido numa tela usando o algoritmo do ponto médio
void desenhar_circulo_preenchido(Tela* tela, int centro_x, int centro_y, int raio, Uint32 cor) {
    if (tela == nullptr || tela->pixeis == nullptr) return; // nullptr é nativo no C23

    int x = 0;
    int y = raio;
    int d = 3 - 2 * raio;

    while (y >= x) {
        // Desenha as linhas horizontais entre os pontos simétricos do círculo
        desenhar_linha_horizontal(tela, centro_x - x, centro_x + x, centro_y + y, cor);
        desenhar_linha_horizontal(tela, centro_x - x, centro_x + x, centro_y - y, cor);
        desenhar_linha_horizontal(tela, centro_x - y, centro_x + y, centro_y + x, cor);
        desenhar_linha_horizontal(tela, centro_x - y, centro_x + y, centro_y - x, cor);

        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

// Função que aplica antisserrilhado por superamostragem (Downsampling de SSAA 4x)
// Reduz uma tela de alta resolução para uma tela de destino tirando a média de blocos 2x2
void aplicar_antisserrilhado(const Tela* origem, Tela* destino) {
    if (origem == nullptr || destino == nullptr) return;
    if (origem->largura != destino->largura * 2 || origem->altura != destino->altura * 2) return;

    for (int y = 0; y < destino->altura; y++) {
        for (int x = 0; x < destino->largura; x++) {
            // Coordenadas do bloco 2x2 na tela de alta resolução
            int ox = x * 2;
            int oy = y * 2;

            Uint32 p1 = origem->pixeis[oy * origem->largura + ox];
            Uint32 p2 = origem->pixeis[oy * origem->largura + (ox + 1)];
            Uint32 p3 = origem->pixeis[(oy + 1) * origem->largura + ox];
            Uint32 p4 = origem->pixeis[(oy + 1) * origem->largura + (ox + 1)];

            // Extrair canais de cor e calcular a média aritmética simples
            Uint8 r = ((p1 >> 24 & 0xFF) + (p2 >> 24 & 0xFF) + (p3 >> 24 & 0xFF) + (p4 >> 24 & 0xFF)) / 4;
            Uint8 g = ((p1 >> 16 & 0xFF) + (p2 >> 16 & 0xFF) + (p3 >> 16 & 0xFF) + (p4 >> 16 & 0xFF)) / 4;
            Uint8 b = ((p1 >> 8 & 0xFF) + (p2 >> 8 & 0xFF) + (p3 >> 8 & 0xFF) + (p4 >> 8 & 0xFF)) / 4;
            Uint8 a = ((p1 & 0xFF) + (p2 & 0xFF) + (p3 & 0xFF) + (p4 & 0xFF)) / 4;

            // Recompor o píxel na tela de destino
            destino->pixeis[y * destino->largura + x] = (r << 24) | (g << 16) | (b << 8) | a;
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("Erro ao inicializar a SDL: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window* janela = SDL_CreateWindow("Circulos com Antisserrilhado (C23 + SDL3)", 
                                         JANELA_LARGURA, JANELA_ALTURA, 0);
    if (janela == nullptr) {
        SDL_Log("Erro ao criar janela: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderizador = SDL_CreateRenderer(janela, nullptr);
    if (renderizador == nullptr) {
        SDL_Log("Erro ao criar renderizador: %s", SDL_GetError());
        SDL_DestroyWindow(janela);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Criar a textura SDL final
    SDL_Texture* textura_final = SDL_CreateTexture(renderizador, SDL_PIXELFORMAT_RGBA8888, 
                                                   SDL_TEXTUREACCESS_STREAMING, 
                                                   JANELA_LARGURA, JANELA_ALTURA);

    // Definir dimensões com base na escolha do Antisserrilhado
    int e_largura = ATIVAR_ANTISSERRILHADO ? JANELA_LARGURA * 2 : JANELA_LARGURA;
    int e_altura = ATIVAR_ANTISSERRILHADO ? JANELA_ALTURA * 2 : JANELA_ALTURA;

    Tela tela_desenho = {
        .pixeis = malloc(e_largura * e_altura * sizeof(Uint32)),
        .largura = e_largura,
        .altura = e_altura
    };

    Tela tela_janela = { .pixeis = nullptr, .largura = JANELA_LARGURA, .altura = JANELA_ALTURA };
    if (ATIVAR_ANTISSERRILHADO) {
        tela_janela.pixeis = malloc(JANELA_LARGURA * JANELA_ALTURA * sizeof(Uint32));
    } else {
        tela_janela.pixeis = tela_desenho.pixeis;
    }

    // Limpar a tela de desenho (Fundo Preto)
    for (int i = 0; i < tela_desenho.largura * tela_desenho.altura; i++) {
        tela_desenho.pixeis[i] = 0x000000FF; // RGBA
    }

    // Desenhar 20 círculos aleatórios
    srand((unsigned int)time(nullptr));
    for (int i = 0; i < 20; i++) {
        int raio = 20 + rand() % 60;
        int escala = ATIVAR_ANTISSERRILHADO ? 2 : 1;
        int cx = (rand() % JANELA_LARGURA) * escala;
        int cy = (rand() % JANELA_ALTURA) * escala;
        raio *= escala;

        Uint8 r = rand() % 256;
        Uint8 g = rand() % 256;
        Uint8 b = rand() % 256;
        Uint32 cor = (r << 24) | (g << 16) | (b << 8) | 0xFF;

        desenhar_circulo_preenchido(&tela_desenho, cx, cy, raio, cor);
    }

    // Se ativo, faz o downsampling da superamostragem
    if (ATIVAR_ANTISSERRILHADO) {
        aplicar_antisserrilhado(&tela_desenho, &tela_janela);
    }

    // Atualizar a textura SDL com os píxeis
    SDL_UpdateTexture(textura_final, nullptr, tela_janela.pixeis, JANELA_LARGURA * sizeof(Uint32));

    bool a_correr = true;
    SDL_Event evento;

    while (a_correr) {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_EVENT_QUIT) {
                a_correr = false;
            }
        }

        SDL_RenderClear(renderizador);
        SDL_RenderTexture(renderizador, textura_final, nullptr, nullptr);
        SDL_RenderPresent(renderizador);
        SDL_Delay(16);
    }

    // Limpeza
    SDL_DestroyTexture(textura_final);
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(janela);
    
    free(tela_desenho.pixeis);
    if (ATIVAR_ANTISSERRILHADO) {
        free(tela_janela.pixeis);
    }
    
    SDL_Quit();
    return EXIT_SUCCESS;
}

/*

gcc -g -lSDL3 -std=c23 -Wall -Wextra -Wpedantic graphics/ssaa.c -o ssaa

*/
