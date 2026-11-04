#include <SDL3/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define LARGURA_ECRÃ 800
#define ALTURA_ECRÃ 600
#define PI 3.14159265358979323846f

// ============================================================================
// 1. ESTRUTURAS DE DADOS (Com Diacríticos)
// ============================================================================

typedef struct {
    float x, y, z;
} Vetor3D;

typedef struct {
    float m[4][4];
} Matriz4x4;

typedef struct {
    Vetor3D vértices[3];
} Triângulo;

typedef struct {
    Triângulo* triângulos;
    int total_triângulos;
} Malha3D;

typedef struct {
    Malha3D malha;
    Vetor3D posição;
    Vetor3D rotação;
} InstânciaObjeto;

// ============================================================================
// 2. ÁLGEBRA LINEAR E MATEMÁTICA 3D
// ============================================================================

Vetor3D vetor3d_subtrair(Vetor3D a, Vetor3D b) {
    return (Vetor3D){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vetor3D vetor3d_produto_vetorial(Vetor3D a, Vetor3D b) {
    return (Vetor3D){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

Vetor3D vetor3d_normalizar(Vetor3D v) {
    float comprimento = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (comprimento == 0.0f) return v;
    return (Vetor3D){v.x / comprimento, v.y / comprimento, v.z / comprimento};
}

float vetor3d_produto_escalar(Vetor3D a, Vetor3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Matriz4x4 matriz4x4_identidade(void) {
    Matriz4x4 mat = {0};
    mat.m[0][0] = 1.0f; mat.m[1][1] = 1.0f; mat.m[2][2] = 1.0f; mat.m[3][3] = 1.0f;
    return mat;
}

Matriz4x4 matriz4x4_multiplicar(Matriz4x4 a, Matriz4x4 b) {
    Matriz4x4 resultado = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            resultado.m[i][j] = a.m[i][0] * b.m[0][j] +
                                a.m[i][1] * b.m[1][j] +
                                a.m[i][2] * b.m[2][j] +
                                a.m[i][3] * b.m[3][j];
        }
    }
    return resultado;
}

Vetor3D matriz4x4_multiplicar_vetor3d(Matriz4x4 m, Vetor3D v) {
    float x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
    float y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
    float z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
    float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

    if (w != 0.0f) {
        x /= w; y /= w; z /= w;
    }
    return (Vetor3D){x, y, z};
}

Matriz4x4 matriz4x4_rotação_y(float ângulo) {
    Matriz4x4 mat = matriz4x4_identidade();
    mat.m[0][0] = cosf(ângulo);
    mat.m[0][2] = -sinf(ângulo);
    mat.m[2][0] = sinf(ângulo);
    mat.m[2][2] = cosf(ângulo);
    return mat;
}

Matriz4x4 matriz4x4_rotação_x(float ângulo) {
    Matriz4x4 mat = matriz4x4_identidade();
    mat.m[1][1] = cosf(ângulo);
    mat.m[1][2] = sinf(ângulo);
    mat.m[2][1] = -sinf(ângulo);
    mat.m[2][2] = cosf(ângulo);
    return mat;
}

Matriz4x4 matriz4x4_translação(float x, float y, float z) {
    Matriz4x4 mat = matriz4x4_identidade();
    mat.m[3][0] = x;
    mat.m[3][1] = y;
    mat.m[3][2] = z;
    return mat;
}

Matriz4x4 matriz4x4_perspetiva(float fov, float aspeto, float próximo, float distante) {
    Matriz4x4 mat = {0};
    float f = 1.0f / tanf((fov * PI / 180.0f) / 2.0f);
    mat.m[0][0] = f / aspeto;
    mat.m[1][1] = f;
    mat.m[2][2] = distante / (distante - próximo);
    mat.m[2][3] = 1.0f;
    mat.m[3][2] = (-distante * próximo) / (distante - próximo);
    return mat;
}

// ============================================================================
// 3. GERAÇÃO DA GEOMETRIA (Criação da Esfera)
// ============================================================================

Malha3D malha_criar_esfera(float raio, int anéis, int segmentos) {
    Malha3D malha;
    int max_triângulos = anéis * segmentos * 2;
    malha.triângulos = malloc(sizeof(Triângulo) * max_triângulos);
    malha.total_triângulos = 0;

    for (int i = 0; i < anéis; i++) {
        float phi1 = PI * (float)i / anéis;
        float phi2 = PI * (float)(i + 1) / anéis;

        for (int j = 0; j < segmentos; j++) {
            float theta1 = 2.0f * PI * (float)j / segmentos;
            float theta2 = 2.0f * PI * (float)(j + 1) / segmentos;

            Vetor3D v1 = {raio * sinf(phi1) * cosf(theta1), raio * cosf(phi1), raio * sinf(phi1) * sinf(theta1)};
            Vetor3D v2 = {raio * sinf(phi1) * cosf(theta2), raio * cosf(phi1), raio * sinf(phi1) * sinf(theta2)};
            Vetor3D v3 = {raio * sinf(phi2) * cosf(theta1), raio * cosf(phi2), raio * sinf(phi2) * sinf(theta1)};
            Vetor3D v4 = {raio * sinf(phi2) * cosf(theta2), raio * cosf(phi2), raio * sinf(phi2) * sinf(theta2)};

            if (i != 0) {
                malha.triângulos[malha.total_triângulos++] = (Triângulo){{v1, v2, v3}};
            }
            if (i != anéis - 1) {
                malha.triângulos[malha.total_triângulos++] = (Triângulo){{v2, v4, v3}};
            }
        }
    }
    return malha;
}

// ============================================================================
// 4. ILUMINAÇÃO E RASTERIZAÇÃO POR SOFTWARE (Píxel a Píxel)
// ============================================================================

SDL_Color calcular_iluminação_plana(Triângulo t, Vetor3D direção_luz, SDL_Color cor_base) {
    Vetor3D aresta1 = vetor3d_subtrair(t.vértices[1], t.vértices[0]);
    Vetor3D aresta2 = vetor3d_subtrair(t.vértices[2], t.vértices[0]);
    Vetor3D normal = vetor3d_normalizar(vetor3d_produto_vetorial(aresta1, aresta2));

    direção_luz = vetor3d_normalizar(direção_luz);
    float intensidade = vetor3d_produto_escalar(normal, direção_luz);
    if (intensidade < 0.0f) intensidade = 0.0f;

    float ambiente = 0.15f;
    float fator_total = ambiente + intensidade * (1.0f - ambiente);

    return (SDL_Color){
        (Uint8)SDL_clamp(cor_base.r * fator_total, 0, 255),
        (Uint8)SDL_clamp(cor_base.g * fator_total, 0, 255),
        (Uint8)SDL_clamp(cor_base.b * fator_total, 0, 255),
        255
    };
}

void desenhar_linha_z_buffer(SDL_Renderer* renderizador, int y, float x1, float z1, float x2, float z2, float* buffer_de_profundidade) {
    if (x1 > x2) {
        float temp_x = x1; x1 = x2; x2 = temp_x;
        float temp_z = z1; z1 = z2; z2 = temp_z;
    }

    int início_x = (int)ceilf(x1);
    int fim_x = (int)ceilf(x2);

    início_x = SDL_clamp(início_x, 0, LARGURA_ECRÃ);
    fim_x = SDL_clamp(fim_x, 0, LARGURA_ECRÃ);

    for (int x = início_x; x < fim_x; x++) {
        float t = (x1 == x2) ? 0.0f : (float)(x - x1) / (x2 - x1);
        float z = z1 + t * (z2 - z1);

        int indice = y * LARGURA_ECRÃ + x;
        if (z < buffer_de_profundidade[indice]) {
            buffer_de_profundidade[indice] = z;
            SDL_RenderPoint(renderizador, (float)x, (float)y);
        }
    }
}

void desenhar_triângulo_plano(SDL_Renderer* renderizador, Triângulo t, SDL_Color cor, float* buffer_de_profundidade) {
    Vetor3D v0 = t.vértices[0]; Vetor3D v1 = t.vértices[1]; Vetor3D v2 = t.vértices[2];

    // Ordenar os vértices por Y (v0 mais acima, v2 mais abaixo)
    if (v0.y > v1.y) { Vetor3D tmp = v0; v0 = v1; v1 = tmp; }
    if (v0.y > v2.y) { Vetor3D tmp = v0; v0 = v2; v2 = tmp; }
    if (v1.y > v2.y) { Vetor3D tmp = v1; v1 = v2; v2 = tmp; }

    if (v0.y == v2.y) return;

    SDL_SetRenderDrawColor(renderizador, cor.r, cor.g, cor.b, 255);

    int início_y = (int)ceilf(v0.y);
    int fim_y = (int)ceilf(v2.y);
    início_y = SDL_clamp(início_y, 0, ALTURA_ECRÃ);
    fim_y = SDL_clamp(fim_y, 0, ALTURA_ECRÃ);

    for (int y = início_y; y < fim_y; y++) {
        bool na_metade_superior = y < v1.y;

        float t_total = (float)(y - v0.y) / (v2.y - v0.y);
        float t_segmento = na_metade_superior ? (float)(y - v0.y) / (v1.y - v0.y) : (float)(y - v1.y) / (v2.y - v1.y);

        if (na_metade_superior && v0.y == v1.y) continue;
        if (!na_metade_superior && v1.y == v2.y) continue;

        float ax = v0.x + t_total * (v2.x - v0.x);
        float az = v0.z + t_total * (v2.z - v0.z);

        float bx = na_metade_superior ? v0.x + t_segmento * (v1.x - v0.x) : v1.x + t_segmento * (v2.x - v1.x);
        float bz = na_metade_superior ? v0.z + t_segmento * (v1.z - v0.z) : v1.z + t_segmento * (v2.z - v1.z);

        desenhar_linha_z_buffer(renderizador, y, ax, az, bx, bz, buffer_de_profundidade);
    }
}

// ============================================================================
// 5. CICLO PRINCIPAL DA APLICAÇÃO
// ============================================================================

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return -1;
    }

    SDL_Window* janela = SDL_CreateWindow("Demonstração de Flat Shading em C23 (Esfera)", LARGURA_ECRÃ, ALTURA_ECRÃ, 0);
    SDL_Renderer* renderizador = SDL_CreateRenderer(janela, NULL);

    float* buffer_de_profundidade = malloc(sizeof(float) * LARGURA_ECRÃ * ALTURA_ECRÃ);

    // Criação do objeto (Esfera com poucos segmentos para realçar o Flat Shading)
    InstânciaObjeto objeto = {
        .malha = malha_criar_esfera(2.0f, 16, 24),
        .posição = {0.0f, 0.0f, 6.0f},
        .rotação = {0.0f, 0.0f, 0.0f}
    };

    Vetor3D direção_luz = {1.0f, 1.0f, -1.0f};
    SDL_Color cor_esfera = {0, 150, 255, 255};
    Matriz4x4 mat_projeção = matriz4x4_perspetiva(60.0f, (float)LARGURA_ECRÃ / ALTURA_ECRÃ, 0.1f, 100.0f);

    bool em_execução = true;
    SDL_Event evento;

    while (em_execução) {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_EVENT_QUIT) {
                em_execução = false;
            }
        }

        // Atualização da física/rotação
        objeto.rotação.y += 0.02f;
        objeto.rotação.x += 0.01f;

        // Limpeza dos ecrãs (Preto) e Reset do Z-Buffer
				SDL_SetRenderDrawColor(renderizador, 15, 15, 15, 255);
				SDL_RenderClear(renderizador);
				for (int i = 0; i < LARGURA_ECRÃ * ALTURA_ECRÃ; i++) {
						buffer_de_profundidade[i] = INFINITY;
				}

				// Construção das transformações do objeto (Matriz Mundo)
				Matriz4x4 mat_rot_y = matriz4x4_rotação_y(objeto.rotação.y);
				Matriz4x4 mat_rot_x = matriz4x4_rotação_x(objeto.rotação.x);
				Matriz4x4 mat_trans = matriz4x4_translação(objeto.posição.x, objeto.posição.y, objeto.posição.z);
				Matriz4x4 mat_mundo = matriz4x4_multiplicar(mat_rot_x, mat_rot_y);
				mat_mundo = matriz4x4_multiplicar(mat_mundo, mat_trans);

				// Processar e desenhar cada face
				for (int i = 0; i < objeto.malha.total_triângulos; i++) {
						Triângulo t_original = objeto.malha.triângulos[i];
						Triângulo t_transformado, t_projetado;

						// 1. Transformação para o Espaço do Mundo
						for (int v = 0; v < 3; v++) {
								t_transformado.vértices[v] = matriz4x4_multiplicar_vetor3d(mat_mundo, t_original.vértices[v]);
						}

						// Backface Culling (Evita desenhar triângulos virados de costas)
						Vetor3D a1 = vetor3d_subtrair(t_transformado.vértices[1], t_transformado.vértices[0]);
						Vetor3D a2 = vetor3d_subtrair(t_transformado.vértices[2], t_transformado.vértices[0]);
						Vetor3D normal_mundo = vetor3d_produto_vetorial(a1, a2);
						Vetor3D raio_visão = vetor3d_subtrair(t_transformado.vértices[0], (Vetor3D){0, 0, 0});
						if (vetor3d_produto_escalar(normal_mundo, raio_visão) < 0.0f) {
								// 2. Calcular Iluminação Flat baseada na face do Mundo
								SDL_Color cor_iluminada = calcular_iluminação_plana(t_transformado, direção_luz, cor_esfera);

								// 3. Aplicar Projeção Perspetiva e Escalar para o Ecrã
								for (int v = 0; v < 3; v++) {
										t_projetado.vértices[v] = matriz4x4_multiplicar_vetor3d(mat_projeção, t_transformado.vértices[v]);

										// Escalar Coordenadas Normalizadas para Píxeis do Ecrã
										t_projetado.vértices[v].x = (t_projetado.vértices[v].x + 1.0f) * 0.5f * LARGURA_ECRÃ;
										t_projetado.vértices[v].y = (1.0f - (t_projetado.vértices[v].y + 1.0f) * 0.5f) * ALTURA_ECRÃ;
								}

						// 4. Desenhar Face Rasterizada
						desenhar_triângulo_plano(renderizador, t_projetado, cor_iluminada, buffer_de_profundidade);
						}
				}

				SDL_RenderPresent(renderizador);
				SDL_Delay(16); // Manter próximo dos 60 FPS
		}

		// Libertação de recursos
		free(objeto.malha.triângulos);
		free(buffer_de_profundidade);
		SDL_DestroyRenderer(renderizador);
		SDL_DestroyWindow(janela);
		SDL_Quit();
		return 0;
}
