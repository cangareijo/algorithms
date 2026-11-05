#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define LARGURA_ECRÃ 800
#define ALTURA_ECRÃ 600
#define PI 3.14159265358979323846f

// ============================================================================
// 1. ESTRUTURAS DE DADOS (Motor Gráfico Autónomo)
// ============================================================================

typedef struct {
    float x;
    float y;
    float z;
} Vetor3D;

typedef struct {
    float m[4][4];
} Matriz4x4;

typedef struct {
    Vetor3D vértices[3];
} Triângulo;

typedef struct {
    Triângulo* triângulos;
    int        total_triângulos;
} Malha3D;

typedef struct {
    Malha3D malha;
    Vetor3D posição;
    Vetor3D rotação;
} InstânciaObjeto;

typedef struct {
    uint32_t* píxeis;
    float*    buffer_de_profundidade;
    int       largura;
    int       altura;
} Tela;

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
    mat.m[0][0] = 1.0f;
    mat.m[1][1] = 1.0f;
    mat.m[2][2] = 1.0f;
    mat.m[3][3] = 1.0f;
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

Matriz4x4 matriz4x4_rotação_x(float ângulo) {
    Matriz4x4 mat = matriz4x4_identidade();
    mat.m[1][1] = cosf(ângulo);
    mat.m[1][2] = sinf(ângulo);
    mat.m[2][1] = -sinf(ângulo);
    mat.m[2][2] = cosf(ângulo);
    return mat;
}

Matriz4x4 matriz4x4_rotação_y(float ângulo) {
    Matriz4x4 mat = matriz4x4_identidade();
    mat.m[0][0] = cosf(ângulo);
    mat.m[0][2] = -sinf(ângulo);
    mat.m[2][0] = sinf(ângulo);
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
// 3. GESTÃO DA TELA (Superfície de Desenho Autónoma)
// ============================================================================

Tela tela_criar(int largura, int altura) {
    Tela tela;
    tela.largura = largura;
    tela.altura = altura;
    tela.píxeis = malloc(sizeof(uint32_t) * largura * altura);
    tela.buffer_de_profundidade = malloc(sizeof(float) * largura * altura);
    return tela;
}

void tela_limpar(Tela* tela, uint32_t cor_fundo) {
    int total_píxeis = tela->largura * tela->altura;
    for (int i = 0; i < total_píxeis; i++) {
        tela->píxeis[i] = cor_fundo;
        tela->buffer_de_profundidade[i] = INFINITY;
    }
}

void tela_destruir(Tela* tela) {
    free(tela->píxeis);
    free(tela->buffer_de_profundidade);
    tela->píxeis = NULL;
    tela->buffer_de_profundidade = NULL;
}

// ============================================================================
// 4. PIPELINE DE VÉRTICES, GEOMETRIA E ILUMINAÇÃO
// ============================================================================

Vetor3D transformar_e_projetar_vértice(Vetor3D vértice, Matriz4x4 mat_mundo, Matriz4x4 mat_projeção, int largura_tela, int altura_tela) {
    // 1. Transforma o vértice original para o Espaço do Mundo
    Vetor3D v_mundo = matriz4x4_multiplicar_vetor3d(mat_mundo, vértice);
    
    // 2. Transforma o vértice do Mundo para o Espaço de Projeção Perspetiva
    Vetor3D v_projetado = matriz4x4_multiplicar_vetor3d(mat_projeção, v_mundo);
    
    // 3. Converte as Coordenadas Normalizadas para Píxeis do Ecrã (Viewport Transform)
    v_projetado.x = (v_projetado.x + 1.0f) * 0.5f * (float)largura_tela;
    v_projetado.y = (1.0f - (v_projetado.y + 1.0f) * 0.5f) * (float)altura_tela;
    
    return v_projetado;
}

bool verificar_face_visível(Triângulo t_transformado) {
    Vetor3D a1 = vetor3d_subtrair(t_transformado.vértices[1], t_transformado.vértices[0]);
    Vetor3D a2 = vetor3d_subtrair(t_transformado.vértices[2], t_transformado.vértices[0]);
    Vetor3D normal_mundo = vetor3d_produto_vetorial(a1, a2);
    Vetor3D raio_visão = vetor3d_subtrair(t_transformado.vértices[0], (Vetor3D){0.0f, 0.0f, 0.0f});
    
    return (vetor3d_produto_escalar(normal_mundo, raio_visão) < 0.0f);
}

uint32_t calcular_iluminação_plana(Triângulo t, Vetor3D direção_luz, uint32_t cor_base) {
    Vetor3D a1 = vetor3d_subtrair(t.vértices[1], t.vértices[0]);
    Vetor3D a2 = vetor3d_subtrair(t.vértices[2], t.vértices[0]);
    Vetor3D normal = vetor3d_normalizar(vetor3d_produto_vetorial(a1, a2));

    direção_luz = vetor3d_normalizar(direção_luz);
    float intensidade = vetor3d_produto_escalar(normal, direção_luz);
    if (intensidade < 0.0f) intensidade = 0.0f;

    float ambiente = 0.15f;
    float fator_total = ambiente + intensidade * (1.0f - ambiente);

    // Desempacotar componentes ARGB de 32 bits
    uint8_t r = (cor_base >> 16) & 0xFF;
    uint8_t g = (cor_base >> 8) & 0xFF;
    uint8_t b = cor_base & 0xFF;

    // Aplicar a intensidade de flat shading com limites corretos
    float r_final = (float)r * fator_total;
    float g_final = (float)g * fator_total;
    float b_final = (float)b * fator_total;

    r = (r_final > 255.0f) ? 255 : (uint8_t)r_final;
    g = (g_final > 255.0f) ? 255 : (uint8_t)g_final;
    b = (b_final > 255.0f) ? 255 : (uint8_t)b_final;

    // Reempacotar em formato ARGB de 32 bits para a Tela
    return (0xFF000000 | (r << 16) | (g << 8) | b);
}

Malha3D malha_criar_esfera(float raio, int anéis, int segmentos) {
    Malha3D malha;
    int max_triângulos = anéis * segmentos * 2;
    malha.triângulos = malloc(sizeof(Triângulo) * max_triângulos);
    malha.total_triângulos = 0;

    for (int i = 0; i < anéis; i++) {
        float phi1 = PI * (float)i / (float)anéis;
        float phi2 = PI * (float)(i + 1) / (float)anéis;

        for (int j = 0; j < segmentos; j++) {
            float theta1 = 2.0f * PI * (float)j / (float)segmentos;
            float theta2 = 2.0f * PI * (float)(j + 1) / (float)segmentos;

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
// 5. RASTERIZAÇÃO E PREENCHIMENTO (Autónomo por Software)
// ============================================================================

void desenhar_linha_z_buffer(Tela* tela, int y, float x1, float z1, float x2, float z2, uint32_t cor) {
    if (x1 > x2) {
        float temp_x = x1; x1 = x2; x2 = temp_x;
        float temp_z = z1; z1 = z2; z2 = temp_z;
    }

    // Garante que o preenchimento segue as regras de grelha de píxeis inteiros
    int início_x = (int)ceilf(x1);
    int fim_x = (int)ceilf(x2);

    // Recorte básico (Clipping) contra os limites horizontais da tela
    if (início_x < 0) início_x = 0;
    if (fim_x > tela->largura) fim_x = tela->largura;

    for (int x = início_x; x < fim_x; x++) {
        float t = (x1 == x2) ? 0.0f : (float)(x - x1) / (x2 - x1);
        float z = z1 + t * (z2 - z1);

        int indice = y * tela->largura + x;
        
        // Teste de Profundidade: desenha apenas se o fragmento estiver mais próximo da câmara
        if (z < tela->buffer_de_profundidade[indice]) {
            tela->buffer_de_profundidade[indice] = z;
            tela->píxeis[indice] = cor;
        }
    }
}

void desenhar_triângulo_plano(Tela* tela, Triângulo t, uint32_t cor) {
    Vetor3D v0 = t.vértices[0]; 
    Vetor3D v1 = t.vértices[1]; 
    Vetor3D v2 = t.vértices[2];

    // Ordenação dos vértices por altura (Y) de forma ascendente (v0 no topo, v2 na base)
    if (v0.y > v1.y) { Vetor3D tmp = v0; v0 = v1; v1 = tmp; }
    if (v0.y > v2.y) { Vetor3D tmp = v0; v0 = v2; v2 = tmp; }
    if (v1.y > v2.y) { Vetor3D tmp = v1; v1 = v2; v2 = tmp; }

    // Ignora triângulos degenerados sem altura
    if (v0.y == v2.y) return;

    int início_y = (int)ceilf(v0.y);
    int fim_y = (int)ceilf(v2.y);
    
    // Recorte básico (Clipping) contra os limites verticais da tela
    if (início_y < 0) início_y = 0;
    if (fim_y > tela->altura) fim_y = tela->altura;

    // Varredura por linhas horizontais (Scanline Rasterization)
    for (int y = início_y; y < fim_y; y++) {
        // Determina se estamos na metade superior ou inferior do triângulo
        bool na_metade_superior = y < v1.y;

        float t_total = (float)(y - v0.y) / (v2.y - v0.y);
        float t_segmento = na_metade_superior ? (float)(y - v0.y) / (v1.y - v0.y) : (float)(y - v1.y) / (v2.y - v1.y);

        // Prevenção de divisões por zero em arestas horizontais internas
        if (na_metade_superior && v0.y == v1.y) continue;
        if (!na_metade_superior && v1.y == v2.y) continue;

        // Aresta A: Vai sempre do topo (v0) à base (v2)
        float ax = v0.x + t_total * (v2.x - v0.x);
        float az = v0.z + t_total * (v2.z - v0.z);

        // Aresta B: Divide-se a meio (vai de v0 a v1, ou de v1 a v2)
        float bx = na_metade_superior ? v0.x + t_segmento * (v1.x - v0.x) : v1.x + t_segmento * (v2.x - v1.x);
        float bz = na_metade_superior ? v0.z + t_segmento * (v1.z - v0.z) : v1.z + t_segmento * (v2.z - v1.z);

        // Rasteriza o segmento horizontal entre a Aresta A e a Aresta B
        desenhar_linha_z_buffer(tela, y, ax, az, bx, bz, cor);
    }
}

#include <SDL3/SDL.h>

// ============================================================================
// 6. SUBSISTEMA DE JANELA E PERIFERIA (Exclusivos SDL3)
// ============================================================================

typedef struct {
    SDL_Window*   janela;
    SDL_Renderer* renderizador;
    SDL_Texture*  textura;
} SubsistemaJanela;

bool inicializar_janela(SubsistemaJanela* sj, int largura, int altura) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return false;
    }

    sj->janela = SDL_CreateWindow(
        "Demonstração de Flat Shading Autónoma em C23 (Esfera)", 
        largura, 
        altura, 
        0
    );
    if (!sj->janela) return false;

    sj->renderizador = SDL_CreateRenderer(sj->janela, NULL);
    if (!sj->renderizador) return false;

    // Criar uma textura nativa para streaming de píxeis em formato ARGB8888
    sj->textura = SDL_CreateTexture(
        sj->renderizador, 
        SDL_PIXELFORMAT_ARGB8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        largura, 
        altura
    );
    if (!sj->textura) return false;

    return true;
}

void processar_eventos(bool* em_execução) {
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_EVENT_QUIT) {
            *em_execução = false;
        }
    }
}

void atualizar_janela_com_tela(SubsistemaJanela* sj, const Tela* tela) {
    // Copiar o nosso array bruto de píxeis na RAM diretamente para a VRAM
    SDL_UpdateTexture(
        sj->textura, 
        NULL, 
        tela->píxeis, 
        tela->largura * (int)sizeof(uint32_t)
    );

    // Limpar o buffer nativo do renderizador
    SDL_RenderClear(sj->renderizador);

    // Enviar a nossa textura renderizada por software para o ecrã completo
    SDL_RenderTexture(sj->renderizador, sj->textura, NULL, NULL);

    // Apresentar o fotograma ao utilizador
    SDL_RenderPresent(sj->renderizador);
}

void destruir_janela(SubsistemaJanela* sj) {
    SDL_DestroyTexture(sj->textura);
    SDL_DestroyRenderer(sj->renderizador);
    SDL_DestroyWindow(sj->janela);
    SDL_Quit();
}

// ============================================================================
// 7. MAESTRO PRINCIPAL (Função Main)
// ============================================================================

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    SubsistemaJanela sistema_visual;
    if (!inicializar_janela(&sistema_visual, LARGURA_ECRÃ, ALTURA_ECRÃ)) {
        return -1;
    }

    // Criar a tela abstrata em memória
    Tela tela = tela_criar(LARGURA_ECRÃ, ALTURA_ECRÃ);

    // Instanciar a esfera com poucos anéis e segmentos para notar o Flat Shading
    InstânciaObjeto objeto = {
        .malha = malha_criar_esfera(2.0f, 16, 24),
        .posição = {0.0f, 0.0f, 6.0f},
        .rotação = {0.0f, 0.0f, 0.0f}
    };

    Vetor3D direção_luz = {1.0f, 1.0f, -1.0f};
    
    // Cor azul em hexadecimal (Formato ARGB: 0xFF0096FF)
    uint32_t cor_base_objeto = 0xFF0096FF; 
    uint32_t cor_fundo_tela   = 0xFF0F0F0F; // Cinzento escuro

    Matriz4x4 mat_projeção = matriz4x4_perspetiva(
        60.0f, 
        (float)LARGURA_ECRÃ / ALTURA_ECRÃ, 
        0.1f, 
        100.0f
    );

    bool em_execução = true;

    while (em_execução) {
        processar_eventos(&em_execução);

        // Atualizar rotação angular do modelo 3D
        objeto.rotação.y += 0.02f;
        objeto.rotação.x += 0.01f;

        // Limpar a nossa estrutura genérica (RAM)
        tela_limpar(&tela, cor_fundo_tela);

        // Construir matrizes de transformação geométrica
        Matriz4x4 mat_rot_y = matriz4x4_rotação_y(objeto.rotação.y);
        Matriz4x4 mat_rot_x = matriz4x4_rotação_x(objeto.rotação.x);
        Matriz4x4 mat_trans = matriz4x4_translação(objeto.posição.x, objeto.posição.y, objeto.posição.z);
        
        Matriz4x4 mat_mundo = matriz4x4_multiplicar(mat_rot_x, mat_rot_y);
        mat_mundo = matriz4x4_multiplicar(mat_mundo, mat_trans);

        // Processar geometricamente as faces e desenhar
        for (int i = 0; i < objeto.malha.total_triângulos; i++) {
            Triângulo t_original = objeto.malha.triângulos[i];
            Triângulo t_transformado, t_projetado;

            // 1. Mapear triângulo para o Espaço do Mundo
            for (int v = 0; v < 3; v++) {
                t_transformado.vértices[v] = matriz4x4_multiplicar_vetor3d(mat_mundo, t_original.vértices[v]);
            }

            // 2. Filtrar faces traseiras (Backface Culling)
            if (verificar_face_visível(t_transformado)) {
                
                // 3. Iluminar a face com base na normal do Espaço do Mundo
                uint32_t cor_iluminada = calcular_iluminação_plana(t_transformado, direção_luz, cor_base_objeto);

                // 4. Projetar e converter os 3 vértices para píxeis
                for (int v = 0; v < 3; v++) {
                    t_projetado.vértices[v] = transformar_e_projetar_vértice(
                        t_original.vértices[v], 
                        mat_mundo, 
                        mat_projeção, 
                        tela.largura, 
                        tela.altura
                    );
                }

                // 5. Rasterizar diretamente na nossa estrutura genérica Tela
                desenhar_triângulo_plano(&tela, t_projetado, cor_iluminada);
            }
        }

        // Fim da renderização: Enviar a Tela atualizada para a Janela SDL3
        atualizar_janela_com_tela(&sistema_visual, &tela);
        
        SDL_Delay(16); // Manter ~60 FPS
    }

    // Limpeza profunda de memória
    free(objeto.malha.triângulos);
    tela_destruir(&tela);
    destruir_janela(&sistema_visual);

    return 0;
}
