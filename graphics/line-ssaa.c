/*
===============================================================================
O ALGORITMO DE SUPERSAMPLE ANTI-ALIASING (SSAA)
Uma Análise Profunda sobre a Suavização de Primitivas Gráficas
===============================================================================

1. INTRODUÇÃO E O FENÓMENO DE ALIASING
-------------------------------------------------------------------------------
Na computação gráfica, a renderização de elementos geométricos bidimensionais,
como linhas retas ou curvas, sobre uma matriz discreta de píxeis apresenta um
desafio clássico conhecido como "aliasing" ou efeito de serrilhado. Uma linha
matemática é uma entidade contínua e infinitamente divisível. No entanto, os
ecrãs modernos são constituídos por uma grelha rígida e finita de píxeis.

Quando um algoritmo tradicional de rasterização (como o Algoritmo de Linha de
Bresenham) é executado, ele avalia cada píxel de forma binária. Se a linha
passa pelo centro geométrico de um determinado píxel, esse píxel é totalmente
preenchido com a cor da linha. Se a linha não intersetar o centro, o píxel
permanece com a cor de fundo original. Esta amostragem digital insuficiente de
uma frequência espacial alta resulta em transições de contraste severas. O olho
humano interpreta estas descontinuidades abruptas como "degraus de escada", que
quebram a ilusão de fluidez e continuidade geométrica.

O Supersample Anti-Aliasing (SSAA), historicamente apelidado de Full-Scene
Anti-Aliasing (FSAA), surge como a solução conceptual mais direta, robusta e
matematicamente pura para mitigar este artefacto visual.


2. O PRINCÍPIO FUNDAMENTAL DO SUPERSAMPLING
-------------------------------------------------------------------------------
O princípio do SSAA baseia-se diretamente na teoria de amostragem de Nyquist-
Shannon: para evitar o aliasing, a taxa de amostragem deve ser substancialmente
superior à maior frequência presente no sinal original. No contexto gráfico,
isto traduz-se em aumentar drasticamente a taxa de amostragem no domínio espacial.

Em vez de calcular o traçado da linha diretamente na resolução final de saída
(a Janela Real), o algoritmo projeta e aloca uma tela virtual intermédia com
dimensões significativamente ampliadas, denominada Grelha de Supersampling ou
"Sample Buffer". Esta ampliação é ditada por um parametro crucial: o Fator SSAA
(doravante designado por N).

Se estabelecermos um fator N = 4, uma janela com uma resolução real de 800 × 600
píxeis passará a dispor de uma tela virtual interna com uma resolução de 
2400 × 1800 subpíxeis. Consequentemente, cada píxel singular da resolução de 
saída passa a ser representado internamente por uma subgrelha de 4 × 4 (ou seja,
16 subpíxeis independentes no total).


3. AS TRÊS ETAPAS LÓGICAS DO ALGORITMO
-------------------------------------------------------------------------------
O ciclo de vida da renderização de uma linha com suporte a SSAA divide-se
rigidamente em três etapas sequenciais e interdependentes:

Etapa 1: Multiplicação e Escalonamento das Coordenadas

Antes de iniciar qualquer cálculo de traçado, as coordenadas bidimensionais de
início (x0, y0) e fim (x1, y1) da linha recebem uma transformação linear simples.
Multiplicam-se ambos os eixos pelo fator de escala N:

    x_super = x * N
    y_super = y * N

Esta operação projeta com precisão matemática a geometria pretendida para o
domínio da alta resolução.

Etapa 2: Rasterização Binária em Alta Resolução

Com as coordenadas devidamente escalonadas, a linha é desenhada na tela virtual
de supersampling. Nesta fase, recorre-se a um algoritmo de rasterização standard
(como o de Bresenham). Importa realçar que, nesta etapa intermédia, NÃO EXISTE
QUALQUER CÁLCULO DE GRADIENTES OU TRANSPARÊNCIAS. A linha é desenhada com cores
totalmente opacas e cantos puramente serrilhados. No entanto, por estar a ser
desenhada numa escala microscopicamente menor em relação ao espaco final, o erro
de discretização é severamente reduzido.

Etapa 3: Filtragem por Downsampling (Média de Caixa)

A magia da suavização visual manifesta-se nesta fase final, frequentemente
designada por "Resolve Pass". Para reconstruir a imagem e mapeiá-la de volta à
resolução real da janela, o algoritmo percorre cada píxel do destino e analisa
os seus N x N subpíxeis correspondentes na tela virtual.

A técnica mais comum e linearmente eficiente é a Filtração por Média de Caixa
(Box Filter). O algoritmo efetua a soma aritmética dos canais de cor (Vermelho,
Verde e Azul) de todos os subpíxeis daquela vizinhança e divide o resultado pelo
total de subpíxeis (N^2).

Se a linha de alta resolução intersetar apenas 4 dos 16 subpíxeis disponíveis
numa determinada vizinhança, o píxel final na janela não será puramente a cor
da linha nem puramente a cor de fundo. Será uma mistura ponderada: 25% da
intensidade da cor da linha fundida com 75% da intensidade da cor de fundo
previamente existente. Ao nével percetivo do olho humano, esta transição suave
de tonalidades atua como um filtro passa-baixo, eliminando as arestas vivas e
conferindo à linha uma aparência perfeitamente retilínea e contínua.


4. A IMPORTÂNCIA DA PRESERVAÇÃO DO FUNDO (BLENDING)
-------------------------------------------------------------------------------
Uma das falhas mais comuns em implementações ingénuas de SSAA é assumir que a
tela de alta resolução começa completamente limpa a preto (com valores RGB a 
zero). Se essa assunção for feita, uma simples divisão aritmética dos píxeis
da linha funcionará, mas criará contornos escuros artificiais se a cor de fundo
real da janela for clara ou colorida.

Para que o algoritmo seja semanticamente correto e preciso, a linha deve ser
rasterizada diretamente por cima do conteúdo que já existia na supertela. No
momento em que o Downsampling é aplicado, a média calculada estará a misturar
a cor da linha com a cor real do fundo subjacente. Isto garante transições de
cor impecáveis, independentemente de a linha cruzar um fundo sólido, uma imagem
de textura complexa ou outros elementos geométricos pré-existentes.


5. EQUILÍBRIO ENTRE DESEMPENHO E QUALIDADE (TRADE-OFFS)
-------------------------------------------------------------------------------
O SSAA continua a ser a métrica de ouro no que diz respeito à qualidade de 
imagem, servindo de referência comparativa para todos os outros algoritmos da
indústria moderna (como MSAA, FXAA ou TAA). Contudo, possui custos associados:

Vantagens:

- Fidelidade Visual Absoluta: Trata toda a cena de forma uniforme. Suaviza não
  apenas as arestas geométricas de linhas e polígonos, mas também quaisquer
  descontinuidades de frequência dentro das texturas e sombreamentos.
- Simplicidade Concetual: Não requer equações complexas de interseção
  geométrica área-píxel, dependendo apenas de rasterização básica e médias.

Desvantagens:
- Custo de Memória Exponencial: O consumo de largura de banda e memoria cresce
  à taxa de N^2. Uma imagem 4K com SSAA 4x exige o processamento interno de uma
  resolução equivalente a 8K.
- Desperdício Computacional: O algoritmo gasta ciclos valiosos a processar e
  a calcular a média de áreas estáticas da tela onde não existem linhas ou
  transições geométricas, tornando-o proibitivo para aplicações gráficas de
  grande escala em tempo real sem otimizações espaciais localizadas (como o
  downsampling incremental restrito à caixa delimitadora da primitiva).

*/

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600
#define FATOR_SSAA 4

#define LARGURA_SUPER (LARGURA_JANELA * FATOR_SSAA)
#define ALTURA_SUPER (ALTURA_JANELA * FATOR_SSAA)

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} Cor;

typedef struct {
    int largura;
    int altura;
    Cor *pixeis;
} Tela;

Tela criar_tela(int largura, int altura, Cor cor_fundo) {
    Tela t = {
        .largura = largura,
        .altura = altura,
        .pixeis = malloc(largura * altura * sizeof(Cor))
    };
    for (int i = 0; i < largura * altura; i++) {
        t.pixeis[i] = cor_fundo;
    }
    return t;
}

void libertar_tela(Tela *t) {
    free(t->pixeis);
    t->pixeis = nullptr;
}

void desenhar_pixel(Tela *t, int x, int y, Cor cor) {
    if (x >= 0 && x < t->largura && y >= 0 && y < t->altura) {
        t->pixeis[y * t->largura + x] = cor;
    }
}

void rasterizar_linha_bresenham(Tela *t, int x0, int y0, int x1, int y1, Cor cor) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int erro = dx - dy;

    while (true) {
        desenhar_pixel(t, x0, y0, cor);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * erro;
        if (e2 > -dy) {
            erro -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            erro += dx;
            y0 += sy;
        }
    }
}

void desenhar_linha_ssaa(Tela *tela_super, int x0, int y0, int x1, int y1, Cor cor) {
    int sx0 = x0 * FATOR_SSAA;
    int sy0 = y0 * FATOR_SSAA;
    int sx1 = x1 * FATOR_SSAA;
    int sy1 = y1 * FATOR_SSAA;
    rasterizar_linha_bresenham(tela_super, sx0, sy0, sx1, sy1, cor);
}

void aplicar_downsampling(Tela *origem, Tela *destino) {
    for (int y = 0; y < destino->altura; y++) {
        for (int x = 0; x < destino->largura; x++) {
            uint32_t soma_r = 0, soma_g = 0, soma_b = 0;
            for (int sy = 0; sy < FATOR_SSAA; sy++) {
                for (int sx = 0; sx < FATOR_SSAA; sx++) {
                    int ox = x * FATOR_SSAA + sx;
                    int oy = y * FATOR_SSAA + sy;
                    Cor c = origem->pixeis[oy * origem->largura + ox];
                    soma_r += c.r;
                    soma_g += c.g;
                    soma_b += c.b;
                }
            }
            uint32_t total_subpixeis = FATOR_SSAA * FATOR_SSAA;
            Cor cor_final = {
                .r = (uint8_t)(soma_r / total_subpixeis),
                .g = (uint8_t)(soma_g / total_subpixeis),
                .b = (uint8_t)(soma_b / total_subpixeis),
                .a = 255
            };
            destino->pixeis[y * destino->largura + x] = cor_final;
        }
    }
}

int main(int argc, char *argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return 1;
    }

    SDL_Window *janela = SDL_CreateWindow("SSAA com SDL3", LARGURA_JANELA, ALTURA_JANELA, 0);
    if (!janela) {
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderizador = SDL_CreateRenderer(janela, NULL);
    if (!renderizador) {
        SDL_DestroyWindow(janela);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *textura = SDL_CreateTexture(renderizador, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, LARGURA_JANELA, ALTURA_JANELA);
    if (!textura) {
        SDL_DestroyRenderer(renderizador);
        SDL_DestroyWindow(janela);
        SDL_Quit();
        return 1;
    }

    Cor cor_fundo = { .r = 40, .g = 44, .b = 52, .a = 255 };
    Tela super_tela = criar_tela(LARGURA_SUPER, ALTURA_SUPER, cor_fundo);
    Tela janela_final = criar_tela(LARGURA_JANELA, ALTURA_JANELA, cor_fundo);

    bool a_executar = true;
    SDL_Event evento;

    while (a_executar) {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_EVENT_QUIT) {
                a_executar = false;
            }
        }

        int x0 = rand() % LARGURA_JANELA;
        int y0 = rand() % ALTURA_JANELA;
        int x1 = rand() % LARGURA_JANELA;
        int y1 = rand() % ALTURA_JANELA;
        Cor cor_linha = {
            .r = (uint8_t)(rand() % 256),
            .g = (uint8_t)(rand() % 256),
            .b = (uint8_t)(rand() % 256),
            .a = 255
        };

        desenhar_linha_ssaa(&super_tela, x0, y0, x1, y1, cor_linha);
        aplicar_downsampling(&super_tela, &janela_final);

        SDL_UpdateTexture(textura, NULL, janela_final.pixeis, LARGURA_JANELA * sizeof(Cor));
        SDL_SetRenderDrawColor(renderizador, 0, 0, 0, 255);
        SDL_RenderClear(renderizador);
        SDL_RenderTexture(renderizador, textura, NULL, NULL);
        SDL_RenderPresent(renderizador);

        SDL_Delay(100);
    }

    libertar_tela(&super_tela);
    libertar_tela(&janela_final);
    SDL_DestroyTexture(textura);
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(janela);
    SDL_Quit();

    return 0;
}
