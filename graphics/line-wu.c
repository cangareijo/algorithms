/*

O ALGORITMO DE XIAOLIN WU: EXPLICAÇÃO DETALHADA
===============================================

Este documento explica de forma exaustiva o funcionamento do Algoritmo de Linhas de Xiaolin Wu, 
um dos métodos mais elegantes e eficientes para a renderização de linhas retas com antialiasing 
(suavização de arestas) em computação gráfica de baixo nível.

1. O PROBLEMA ORIGINAL: SERRILHADO (ALIASING)
--------------------------------------------
Num ecrã digital composto por uma matriz discreta de píxeis, desenhar uma linha diagonal perfeita 
é impossível. Algoritmos tradicionais, como o Algoritmo de Linhas de Bresenham, utilizam apenas 
matemática inteira para determinar qual é o píxel mais próximo da trajetória teórica da reta, 
pintando-o a 100% da sua opacidade. Isto resulta num efeito visual conhecido como "serrilhado" 
ou "escada", especialmente visível em linhas com inclinações suaves.

O algoritmo de Xiaolin Wu resolve este problema ao reconhecer que uma linha passa, na maioria 
das vezes, "entre" dois píxeis. Em vez de escolher apenas um, o algoritmo pinta dois píxeis 
perpendiculares ao eixo principal da linha. A intensidade (ou opacidade) de cada píxel é 
diretamente proporcional à sua proximidade em relação à reta matemática ideal.

2. ARQUITETURA DE DADOS: O CANVAS E O PUT_PIXEL
-----------------------------------------------
Para suportar o antialiasing, o sistema gráfico não pode simplesmente substituir o valor do píxel 
no buffer de memória. É necessário efetuar uma mistura alfa (Alpha Blending) entre a nova cor da 
linha e a cor que já se encontra no fundo da tela.

A estrutura Canvas armazena a resolução e o ponteiro para o bloco de memória linear:

    typedef struct {
        int32_t width;
        int32_t height;
        uint32_t* pixels;
    } Canvas;

A função put_pixel recebe um argumento 'alpha' que varia entre 0.0f (totalmente transparente) 
e 1.0f (totalmente opaco). O processo segue três passos cruciais:
  A) Extração da cor de fundo: O valor ARGB de 32 bits é lido e os canais R, G e B são isolados 
     através de operações de deslocamento de bits (>>) e máscaras lógicas (& 0xFF).
  B) Interpolação linear: Calcula-se a cor final combinando os componentes:
     Cor_Final = (Cor_Linha * alpha) + (Cor_Fundo * (1.0f - alpha))
  C) Reempacotamento: Os novos componentes são unidos novamente num único inteiro de 32 bits 
     e escritos na posição correta do buffer (y * largura + x).

3. NORMALIZAÇÃO DO ESPAÇO GEOMÉTRICO
------------------------------------
Para evitar a replicação de lógica para todos os quadrantes possíveis, o algoritmo normaliza as 
coordenadas de entrada através de duas verificações fundamentais:

  A) Declividade (Steep): Se a variação vertical (dy) for maior do que a variação horizontal (dx), 
     a linha é considerada muito inclinada (steep). Nestas condições, se percorrêssemos o eixo X, 
     haveria colunas com múltiplos píxeis vazios, quebrando a continuidade da linha. Para resolver 
     isto, o algoritmo inverte temporariamente os eixos X e Y. Toda a matemática seguinte assume 
     que a linha é predominantemente horizontal. No momento de pintar o píxel através da função 
     put_pixel, a inversão é desfeita.
     
  B) Direção do Desenho: O ciclo principal assume que o desenho progride da esquerda para a direita. 
     Se o ponto inicial x0 for maior que x1, os dois pontos (x0, y0) e (x1, y1) são completamente 
     invertidos.

4. O CÁLCULO DO GRADIENTE E O CICLO PRINCIPAL
---------------------------------------------
Após a normalização, calcula-se o gradiente (declive) da reta:
    gradient = dy / dx

A variável mais importante do ciclo principal é a 'intery' (Y de interseção). Trata-se de um valor 
de vírgula flutuante que guarda a coordenada Y exata da reta para a coluna X atual. A cada iteração 
do ciclo (onde X avança uma unidade), a variável 'intery' é incrementada adicionando o 'gradient'.

Para cada coluna X, o algoritmo identifica os dois píxeis vizinhos no eixo vertical:
  * O píxel cuja coordenada Y é a parte inteira de intery: (int32_t)intery
  * O píxel imediatamente abaixo: (int32_t)intery + 1

A distância da reta real para estes píxeis é extraída da parte fracionária de 'intery':
  * Parte_Fracionaria = intery - (int32_t)intery
  * O píxel inferior recebe uma opacidade igual à Parte_Fracionaria.
  * O píxel superior recebe o complemento da opacidade, ou seja, (1.0f - Parte_Fracionaria).

Desta forma, se a reta passar exatamente no centro do píxel superior, a parte fracionária será 0, 
fazendo com que o píxel superior seja pintado com opacidade máxima (1.0f) e o inferior com 
opacidade nula (0.0f). Se a reta passar exatamente a meio caminho entre ambos, ambos receberão 
0.5f de opacidade, fundindo-se suavemente à vista do observador.

5. TRATAMENTO DAS EXTREMIDADES (XGAP)
-------------------------------------
Uma das grandes distinções do algoritmo de Xiaolin Wu em relação a outras abordagens de 
antialiasing é o tratamento rigoroso dos pontos terminais da linha. Os pontos de início e fim 
raramente coincidem com o centro exato de um píxel inteiro.

O algoritmo calcula uma variável chamada 'xgap', que representa o fator de ajuste horizontal para 
o primeiro e o último píxel. Este fator garante que a intensidade luminosa das extremidades da 
linha seja atenuada corretamente se a linha começar ou terminar a meio de um píxel, eliminando 
pontas quadradas ou artefactos visuais abruptos.

CONCLUSÃO
---------
O Algoritmo de Xiaolin Wu alcança um equilíbrio perfeito entre qualidade visual e desempenho. 
Embora a implementação apresentada utilize ponto flutuante (float) para fins educacionais e de 
clareza conceitual, a estrutura lógica é perfeitamente adaptável para aritmética de ponto fixo 
(Fixed-Point Arithmetic), o que permite a sua execução em processadores sem unidade de ponto 
flutuante (FPU), mantendo a suavidade e a precisão milimétrica nas aplicações gráficas modernas.

*/

#include <SDL3/SDL.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    int32_t width;
    int32_t height;
    uint32_t* pixels;
} Canvas;

void put_pixel(Canvas* canvas, int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b, float alpha) {
    if (x < 0 || x >= canvas->width || y < 0 || y >= canvas->height) return;

    uint32_t index = y * canvas->width + x;
    uint32_t bg = canvas->pixels[index];

    uint8_t bg_r = (bg >> 16) & 0xFF;
    uint8_t bg_g = (bg >> 8) & 0xFF;
    uint8_t bg_b = bg & 0xFF;

    uint8_t out_r = (uint8_t)(r * alpha + bg_r * (1.0f - alpha));
    uint8_t out_g = (uint8_t)(g * alpha + bg_g * (1.0f - alpha));
    uint8_t out_b = (uint8_t)(b * alpha + bg_b * (1.0f - alpha));

    canvas->pixels[index] = (0xFF << 24) | (out_r << 16) | (out_g << 8) | out_b;
}

void draw_line_wu(Canvas* canvas, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint8_t r, uint8_t g, uint8_t b) {
    bool steep = abs(y1 - y0) > abs(x1 - x0);

    if (steep) {
        int32_t temp = x0; x0 = y0; y0 = temp;
        temp = x1; x1 = y1; y1 = temp;
    }
    if (x0 > x1) {
        int32_t temp = x0; x0 = x1; x1 = temp;
        temp = y0; y0 = y1; y1 = temp;
    }

    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;
    float gradient = (dx == 0) ? 1.0f : (float)dy / (float)dx;

    int32_t xend = x0;
    float yend = y0 + gradient * (xend - x0);
    float xgap = 1.0f - (x0 + 0.5f - (int32_t)(x0 + 0.5f));
    int32_t xpxl1 = xend;
    int32_t ypxl1 = (int32_t)yend;

    if (steep) {
        put_pixel(canvas, ypxl1, xpxl1, r, g, b, (1.0f - (yend - (int32_t)yend)) * xgap);
        put_pixel(canvas, ypxl1 + 1, xpxl1, r, g, b, (yend - (int32_t)yend) * xgap);
    } else {
        put_pixel(canvas, xpxl1, ypxl1, r, g, b, (1.0f - (yend - (int32_t)yend)) * xgap);
        put_pixel(canvas, xpxl1, ypxl1 + 1, r, g, b, (yend - (int32_t)yend) * xgap);
    }

    float intery = yend + gradient;

    xend = x1;
    yend = y1 + gradient * (xend - x1);
    xgap = x1 + 0.5f - (int32_t)(x1 + 0.5f);
    int32_t xpxl2 = xend;
    int32_t ypxl2 = (int32_t)yend;

    if (steep) {
        put_pixel(canvas, ypxl2, xpxl2, r, g, b, (1.0f - (yend - (int32_t)yend)) * xgap);
        put_pixel(canvas, ypxl2 + 1, xpxl2, r, g, b, (yend - (int32_t)yend) * xgap);
    } else {
        put_pixel(canvas, xpxl2, ypxl2, r, g, b, (1.0f - (yend - (int32_t)yend)) * xgap);
        put_pixel(canvas, xpxl2, ypxl2 + 1, r, g, b, (yend - (int32_t)yend) * xgap);
    }

    if (steep) {
        for (int32_t x = xpxl1 + 1; x <= xpxl2 - 1; x++) {
            put_pixel(canvas, (int32_t)intery, x, r, g, b, 1.0f - (intery - (int32_t)intery));
            put_pixel(canvas, (int32_t)intery + 1, x, r, g, b, intery - (int32_t)intery);
            intery += gradient;
        }
    } else {
        for (int32_t x = xpxl1 + 1; x <= xpxl2 - 1; x++) {
            put_pixel(canvas, x, (int32_t)intery, r, g, b, 1.0f - (intery - (int32_t)intery));
            put_pixel(canvas, x, (int32_t)intery + 1, r, g, b, intery - (int32_t)intery);
            intery += gradient;
        }
    }
}

int main(int, char*[]) {
    const int32_t width = 800;
    const int32_t height = 600;

    if (!SDL_Init(SDL_INIT_VIDEO)) return -1;

    SDL_Window* window = SDL_CreateWindow("Wu's Line Algorithm", width, height, 0);
    if (!window) {
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    uint32_t* pixel_buffer = malloc(width * height * sizeof(uint32_t));
    Canvas canvas = { .width = width, .height = height, .pixels = pixel_buffer };

    for (int i = 0; i < width * height; i++) {
        canvas.pixels[i] = 0xFF000000;
    }

    srand((unsigned int)time(NULL));

    for (int i = 0; i < 20; i++) {
        int32_t x0 = rand() % width;
        int32_t y0 = rand() % height;
        int32_t x1 = rand() % width;
        int32_t y1 = rand() % height;

        uint8_t r = rand() % 256;
        uint8_t g = rand() % 256;
        uint8_t b = rand() % 256;

        draw_line_wu(&canvas, x0, y0, x1, y1, r, g, b);
    }

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        SDL_UpdateTexture(texture, NULL, canvas.pixels, width * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    free(pixel_buffer);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
