#include <stdio.h>
#include <stdlib.h>

// Definição das dimensões da imagem de saída
#define LARG 600
#define ALT  400

// Estrutura minimalista para um vértice 3D com uma cor sólida associada
typedef struct {
    float x, y, z;
    unsigned char r, g, b;
} Vertice;

// Função de Aresta: Determina se um ponto (px, py) está à direita ou à esquerda 
// da linha que une 'a' e 'b'. O sinal indica o lado, e o valor é proporcional à área.
static inline float aresta(Vertice a, Vertice b, float px, float py) {
    return (px - a.x) * (b.y - a.y) - (py - a.y) * (b.x - a.x);
}

int main(void) {
    // C23: Alocação e inicialização automática a zero (fundo preto) com chaves {}
    unsigned char *imagem = malloc(LARG * ALT * 3 * sizeof(unsigned char));
    float *z_buffer = malloc(LARG * ALT * sizeof(float));

    if (!imagem || !z_buffer) return 1;

    // Inicializa o Z-buffer com um valor de profundidade muito distante (infinito)
    for (int i = 0; i < LARG * ALT; ++i) {
        z_buffer[i] = 1e10f; 
        imagem[i * 3 + 0] = imagem[i * 3 + 1] = imagem[i * 3 + 2] = 0;
    }

    // Array de vértices: cada grupo de 3 vértices forma um triângulo intersetado
    Vertice v[] = {
        // Triângulo 1 (Vermelho): inclina-se de Z=10 (perto) até Z=50 (longe)
        {100, 100, 10, 255, 0, 0}, {500, 150, 30, 255, 0, 0}, {300, 350, 50, 255, 0, 0},
        // Triângulo 2 (Azul): cruza o primeiro, indo de Z=40 (longe) até Z=15 (perto)
        {150, 320, 40, 0, 0, 255}, {450, 280, 30, 0, 0, 255}, {250, 50, 15, 0, 0, 255}
    };

    // C23: Constante de tempo de compilação para determinar o número de triângulos
    constexpr size_t total_triangulos = sizeof(v) / (sizeof(Vertice) * 3);

    // Loop principal: Processa um triângulo de cada vez
    for (size_t t = 0; t < total_triangulos; ++t) {
        // Identifica os 3 vértices do triângulo atual no array linear
        Vertice v0 = v[t * 3 + 0], v1 = v[t * 3 + 1], v2 = v[t * 3 + 2];

        // Calcula a área dupla do triângulo completo para usar na interpolação
        float area_total = aresta(v0, v1, v2.x, v2.y);
        if (area_total == 0.0f) continue; // Ignora triângulos sem área (linhas/pontos)

        // Limites simplificados de desenho (caixa delimitadora básica)
        int min_x = 0, max_x = LARG - 1, min_y = 0, max_y = ALT - 1;

        // Varrimento pixel a pixel dentro da área da imagem
        for (int y = min_y; y <= max_y; ++y) {
            for (int x = min_x; x <= max_x; ++x) {
                // Alvo: Centro do pixel atual para maior precisão matemática
                float px = (float)x + 0.5f;
                float py = (float)y + 0.5f;

                // Pesos baricêntricos: calculam a influência de cada vértice neste pixel
                float w2 = aresta(v0, v1, px, py) / area_total;
                float w0 = aresta(v1, v2, px, py) / area_total;
                float w1 = aresta(v2, v0, px, py) / area_total;

                // Teste de Inclusão: Se todos os pesos forem positivos, o pixel está dentro do triângulo
                if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) {
                    // Interpolação do Z: Calcula a profundidade exata (Z) neste pixel específico
                    float z_pixel = w0 * v0.z + w1 * v1.z + w2 * v2.z;
                    int idx = y * LARG + x;

                    // --- O TESTE DO Z-BUFFER ---
                    // Se o Z do triângulo atual for menor (mais perto) do que o guardado no buffer:
                    if (z_pixel < z_buffer[idx]) {
                        z_buffer[idx] = z_pixel; // 1. Atualiza o buffer com a nova proximidade

                        // 2. Pinta o pixel na imagem com a cor do triângulo vencedor
                        imagem[idx * 3 + 0] = v0.r;
                        imagem[idx * 3 + 1] = v0.g;
                        imagem[idx * 3 + 2] = v0.b;
                    }
                }
            }
        }
    }

    // Gravação direta do resultado no formato de imagem PPM (P6)
    FILE *f = fopen("zbuffer.ppm", "wb");
    if (f) {
        fprintf(f, "P6\n%d %d\n255\n", LARG, ALT);
        fwrite(imagem, 1, LARG * ALT * 3, f);
        fclose(f);
    }

    // Libertação da memória alocada
    free(imagem);
    free(z_buffer);
    return 0;
}
