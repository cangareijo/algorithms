// =============================================================================
// SECÇÃO 1 DE 4: CONFIGURAÇÕES, ESTRUTURAS DE DADOS E ISOLAMENTO DA SDL3
// =============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>

// Forçamos a inclusão da SDL3 apenas nesta secção para isolar o resto do motor
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>

#define MAT_PI 3.14159265358979323846f

// =============================================================================
// DEFINIÇÃO DAS ESTRUTURAS DE DADOS (Terminologia em Português Europeu)
// =============================================================================

typedef struct {
    float x;
    float y;
    float z;
} Vetor3D;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vetor4D;

typedef struct {
    int x;
    int y;
} Vetor2Di;

// Matriz 4x4 estruturada para a convenção estrita Row-Major (Vetores Linha)
typedef struct {
    float m[16];
} Matriz4x4;

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} CorRGBA;

typedef struct {
    Vetor3D posição;
    Vetor3D normal;
} Vértice;

typedef struct {
    unsigned int v0;
    unsigned int v1;
    unsigned int v2;
} Triângulo;

typedef struct {
    Vértice *vértices;
    Triângulo *triângulos;
    unsigned int contagem_vértices;
    unsigned int contagem_triângulos;
} MalhaPoligonal;

typedef struct {
    Vetor3D coeficiente_ambiente;
    Vetor3D coeficiente_difuso;
    Vetor3D coeficiente_especular;
    float brilho;
} Material;

typedef struct {
    Vetor3D posição;
    Vetor3D cor;
} LuzPontual;

typedef struct {
    Vetor3D posição;
    Vetor3D alvo;
    Vetor3D vetor_cima;
    float campo_de_visão_radianos;
    float plano_próximo;
    float plano_distante;
} Câmara3D;

typedef enum {
    FORMA_CUBO,
    FORMA_CILINDRO,
    FORMA_ESFERA
} TipoForma;

typedef struct {
    TipoForma tipo;
    Vetor3D posição;
    Vetor3D rotação;
    Vetor3D escala;
    Material material;
} Entidade3D;

typedef struct {
    CorRGBA *buffer_píxeis;
    float *buffer_profundidade;
    int largura;
    int altura;
} ContextoGráfico;

typedef struct {
    float alpha;
    float beta;
    float gamma;
} CoordenadasBaricêntricas;

typedef struct Ecrã Ecrã;

struct Ecrã {
    SDL_Window *janela;
    SDL_Renderer *renderizador;
    SDL_Texture *textura;
};

// =============================================================================
// CAMADA DE ABSTRAÇÃO DA BIBLIOTECA GRÁFICA (SDL3)
// =============================================================================

[[nodiscard]] Ecrã* criar_ecrã(const char *título, int largura, int altura) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return nullptr;
    }
    Ecrã *ecrã = malloc(sizeof(Ecrã));
    if (!ecrã) return nullptr;

    ecrã->janela = SDL_CreateWindow(título, largura, altura, 0);
    if (!ecrã->janela) {
        free(ecrã);
        return nullptr;
    }

    ecrã->renderizador = SDL_CreateRenderer(ecrã->janela, nullptr);
    if (!ecrã->renderizador) {
        SDL_DestroyWindow(ecrã->janela);
        free(ecrã);
        return nullptr;
    }

    ecrã->textura = SDL_CreateTexture(ecrã->renderizador, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, largura, altura);
    if (!ecrã->textura) {
        SDL_DestroyRenderer(ecrã->renderizador);
        SDL_DestroyWindow(ecrã->janela);
        free(ecrã);
        return nullptr;
    }

    return ecrã;
}

bool ecrã_deve_fechar(void) {
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_EVENT_QUIT) return true;
        if (evento.type == SDL_EVENT_KEY_DOWN) {
            if (evento.key.key == SDLK_ESCAPE) return true;
        }
    }
    return false;
}

void atualizar_ecrã(Ecrã *ecrã, const ContextoGráfico *ctx) {
    SDL_UpdateTexture(ecrã->textura, nullptr, ctx->buffer_píxeis, ctx->largura * (int)sizeof(CorRGBA));
    SDL_RenderClear(ecrã->renderizador);
    SDL_RenderTexture(ecrã->renderizador, ecrã->textura, nullptr, nullptr);
    SDL_RenderPresent(ecrã->renderizador);
}

void destruir_ecrã(Ecrã *ecrã) {
    if (!ecrã) return;
    SDL_DestroyTexture(ecrã->textura);
    SDL_DestroyRenderer(ecrã->renderizador);
    SDL_DestroyWindow(ecrã->janela);
    free(ecrã);
    SDL_Quit();
}

// =============================================================================
// SECÇÃO 2 DE 4: MATEMÁTICA VETORIAL E MATRICIAL ROW-MAJOR (C23 PURO)
// =============================================================================

// Operações Lineares Elementares de Vetores 3D
Vetor3D vetor3d_adicionar(Vetor3D a, Vetor3D b) { return (Vetor3D){a.x + b.x, a.y + b.y, a.z + b.z}; }
Vetor3D vetor3d_subtrair(Vetor3D a, Vetor3D b) { return (Vetor3D){a.x - b.x, a.y - b.y, a.z - b.z}; }
Vetor3D vetor3d_multiplicar_escalar(Vetor3D v, float escalar) { return (Vetor3D){v.x * escalar, v.y * escalar, v.z * escalar}; }
float vetor3d_produto_escalar(Vetor3D a, Vetor3D b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
float vetor3d_comprimento(Vetor3D v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }

Vetor3D vetor3d_normalizar(Vetor3D v) {
    float comp = vetor3d_comprimento(v);
    if (comp > 0.00001f) return (Vetor3D){v.x / comp, v.y / comp, v.z / comp};
    return (Vetor3D){0.0f, 0.0f, 0.0f};
}

Vetor3D vetor3d_produto_vetorial(Vetor3D a, Vetor3D b) {
    return (Vetor3D){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

// Inicialização da Matriz Identidade Linear
Matriz4x4 matriz4x4_identidade(void) {
    Matriz4x4 matriz = {0};
    matriz.m[0] = 1.0f;
    matriz.m[5] = 1.0f;
    matriz.m[10] = 1.0f;
    matriz.m[15] = 1.0f;
    return matriz;
}

// Construtor de Escala Linear
Matriz4x4 matriz4x4_criar_escala(float x, float y, float z) {
    Matriz4x4 matriz = matriz4x4_identidade();
    matriz.m[0] = x; 
    matriz.m[5] = y; 
    matriz.m[10] = z;
    return matriz;
}

// Construtor de Translação Row-Major Linear (Translação na última linha: índices 12, 13, 14)
Matriz4x4 matriz4x4_criar_translação(float x, float y, float z) {
    Matriz4x4 matriz = matriz4x4_identidade();
    matriz.m[12] = x; 
    matriz.m[13] = y; 
    matriz.m[14] = z;
    return matriz;
}

// Multiplicação Padrão Row-Major de Matrizes 4x4
Matriz4x4 matriz4x4_multiplicar(Matriz4x4 a, Matriz4x4 b) {
    Matriz4x4 resultado = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            resultado.m[i * 4 + j] = a.m[i * 4 + 0] * b.m[0 * 4 + j] +
                                     a.m[i * 4 + 1] * b.m[1 * 4 + j] +
                                     a.m[i * 4 + 2] * b.m[2 * 4 + j] +
                                     a.m[i * 4 + 3] * b.m[3 * 4 + j];
        }
    }
    return resultado;
}

// Rotação combinada XYZ em Radianos
Matriz4x4 matriz4x4_criar_rotação_xyz(float x, float y, float z) {
    Matriz4x4 rx = matriz4x4_identidade(), ry = matriz4x4_identidade(), rz = matriz4x4_identidade();
    float cx = cosf(x), sx = sinf(x);
    rx.m[5] = cx; rx.m[6] = sx; rx.m[9] = -sx; rx.m[10] = cx;
    
    float cy = cosf(y), sy = sinf(y);
    ry.m[0] = cy; ry.m[2] = -sy; ry.m[8] = sy; ry.m[10] = cy;
    
    float cz = cosf(z), sz = sinf(z);
    rz.m[0] = cz; rz.m[1] = sz; rz.m[4] = -sz; rz.m[5] = cz;
    
    return matriz4x4_multiplicar(matriz4x4_multiplicar(rx, ry), rz);
}

// Sistema de Visualização Row-Major (LookAt)
Matriz4x4 matriz4x4_olhar_para(Vetor3D posição, Vetor3D alvo, Vetor3D vetor_cima) {
    Vetor3D z = vetor3d_normalizar(vetor3d_subtrair(alvo, posição));
    Vetor3D x = vetor3d_normalizar(vetor3d_produto_vetorial(vetor_cima, z));
    Vetor3D y = vetor3d_produto_vetorial(z, x);
    
    Matriz4x4 vista = matriz4x4_identidade();
    vista.m[0] = x.x; vista.m[4] = x.y; vista.m[8] = x.z;
    vista.m[1] = y.x; vista.m[5] = y.y; vista.m[9] = y.z;
    vista.m[2] = z.x; vista.m[6] = z.y; vista.m[10] = z.z;
    
    vista.m[12] = -vetor3d_produto_escalar(x, posição);
    vista.m[13] = -vetor3d_produto_escalar(y, posição);
    vista.m[14] = -vetor3d_produto_escalar(z, posição);
    return vista;
}

// Matriz de Projeção Perspetiva alinhada para Row-Major (Coeficiente W na coluna 2, índice 11)
Matriz4x4 matriz4x4_projeção_perspetiva(float fov_radianos, float aspeto, float próximo, float distante) {
    Matriz4x4 proj = {0};
    float f = 1.0f / tanf(fov_radianos * 0.5f);
    proj.m[0] = f / aspeto;
    proj.m[5] = f;
    proj.m[10] = distante / (distante - próximo);
    proj.m[11] = 1.0f;
    proj.m[14] = -(distante * próximo) / (distante - próximo);
    return proj;
}

// Transformações lineares unificadas respeitando a multiplicação Vector-Matrix (V * M)
Vetor3D matriz4x4_transformar_posição(Matriz4x4 m, Vetor3D p) {
    return (Vetor3D){
        p.x * m.m[0] + p.y * m.m[4] + p.z * m.m[8] + m.m[12],
        p.x * m.m[1] + p.y * m.m[5] + p.z * m.m[9] + m.m[13],
        p.x * m.m[2] + p.y * m.m[6] + p.z * m.m[10] + m.m[14]
    };
}

Vetor3D matriz4x4_transformar_direção(Matriz4x4 m, Vetor3D d) {
    return (Vetor3D){
        d.x * m.m[0] + d.y * m.m[4] + d.z * m.m[8],
        d.x * m.m[1] + d.y * m.m[5] + d.z * m.m[9],
        d.x * m.m[2] + d.y * m.m[6] + d.z * m.m[10]
    };
}

Vetor4D matriz4x4_transformar_vetor4d(Matriz4x4 m, Vetor3D p) {
    return (Vetor4D){
        p.x * m.m[0] + p.y * m.m[4] + p.z * m.m[8] + m.m[12],
        p.x * m.m[1] + p.y * m.m[5] + p.z * m.m[9] + m.m[13],
        p.x * m.m[2] + p.y * m.m[6] + p.z * m.m[10] + m.m[14],
        p.x * m.m[3] + p.y * m.m[7] + p.z * m.m[11] + m.m[15]
    };
}

// =============================================================================
// SECÇÃO 3 DE 4: GERAÇÃO ALGORTÍMICA DE GEOMETRIA POLIGONAL
// =============================================================================

void libertar_memória_malha(MalhaPoligonal *malha) {
    if (malha) {
        free(malha->vértices);
        free(malha->triângulos);
        malha->vértices = nullptr;
        malha->triângulos = nullptr;
        malha->contagem_vértices = 0;
        malha->contagem_triângulos = 0;
    }
}

[[nodiscard]] MalhaPoligonal gerar_polígonos_cubo(float tamanho) {
    MalhaPoligonal malha = {0};
    malha.contagem_vértices = 24;
    malha.contagem_triângulos = 12;
    malha.vértices = malloc(sizeof(Vértice) * 24);
    malha.triângulos = malloc(sizeof(Triângulo) * 12);
    if (!malha.vértices || !malha.triângulos) {
        libertar_memória_malha(&malha);
        return (MalhaPoligonal){0};
    }
    
    float h = tamanho * 0.5f;
    Vetor3D p[8] = {
        {-h,-h, h}, { h,-h, h}, { h, h, h}, {-h, h, h},
        {-h,-h,-h}, { h,-h,-h}, { h, h,-h}, {-h, h,-h}
    };
    Vetor3D n[6] = {
        {0,0,1}, {1,0,0}, {0,0,-1}, {-1,0,0}, {0,1,0}, {0,-1,0}
    };
    int faces[6][4] = {
        {0,1,2,3}, {1,5,6,2}, {5,4,7,6}, {4,0,3,7}, {3,2,6,7}, {4,5,1,0}
    };

    int idx_v = 0, idx_t = 0;
    for(int f=0; f<6; f++) {
        for(int v=0; v<4; v++) {
            malha.vértices[idx_v].posição = p[faces[f][v]];
            malha.vértices[idx_v].normal = n[f];
            idx_v++;
        }
        int base = f * 4;
        // Winding order correto anti-horário
        malha.triângulos[idx_t++] = (Triângulo){(unsigned int)base, (unsigned int)(base+1), (unsigned int)(base+2)};
        malha.triângulos[idx_t++] = (Triângulo){(unsigned int)base, (unsigned int)(base+2), (unsigned int)(base+3)};
    }
    return malha;
}

[[nodiscard]] MalhaPoligonal gerar_polígonos_esfera(float raio, unsigned int anéis, unsigned int setores) {
    MalhaPoligonal malha = {0};
    malha.contagem_vértices = (anéis + 1) * (setores + 1);
    malha.contagem_triângulos = anéis * setores * 2;
    malha.vértices = malloc(sizeof(Vértice) * malha.contagem_vértices);
    malha.triângulos = malloc(sizeof(Triângulo) * malha.contagem_triângulos);
    if (!malha.vértices || !malha.triângulos) {
        libertar_memória_malha(&malha);
        return (MalhaPoligonal){0};
    }

    unsigned int idx_v = 0, idx_t = 0;
    for (unsigned int y = 0; y <= anéis; y++) {
        float fI = (float)y / (float)anéis;
        float phi = fI * MAT_PI;
        for (unsigned int x = 0; x <= setores; x++) {
            float fJ = (float)x / (float)setores;
            float theta = fJ * 2.0f * MAT_PI;
            Vetor3D norm = { sinf(phi)*cosf(theta), cosf(phi), sinf(phi)*sinf(theta) };
            malha.vértices[idx_v].posição = vetor3d_multiplicar_escalar(norm, raio);
            malha.vértices[idx_v].normal = norm;
            idx_v++;
        }
    }

    for (unsigned int y = 0; y < anéis; y++) {
        for (unsigned int x = 0; x < setores; x++) {
            unsigned int o0 = y * (setores + 1) + x;
            unsigned int o1 = o0 + 1;
            unsigned int o2 = (y + 1) * (setores + 1) + x;
            unsigned int o3 = o2 + 1;
            // Winding order invertido para bater certo com a rasterização por ecrã
            malha.triângulos[idx_t++] = (Triângulo){o0, o1, o2};
            malha.triângulos[idx_t++] = (Triângulo){o1, o3, o2};
        }
    }
    return malha;
}

[[nodiscard]] MalhaPoligonal gerar_polígonos_cilindro(float raio, float altura, unsigned int segmentos) {
    MalhaPoligonal malha = {0};
    unsigned int v_corpo = (segmentos + 1) * 2;
    unsigned int v_tampas = (segmentos + 1) * 2 + 2; 
    malha.contagem_vértices = v_corpo + v_tampas;
    malha.contagem_triângulos = segmentos * 2 + segmentos * 2; 
    malha.vértices = malloc(sizeof(Vértice) * malha.contagem_vértices);
    malha.triângulos = malloc(sizeof(Triângulo) * malha.contagem_triângulos);
    if (!malha.vértices || !malha.triângulos) {
        libertar_memória_malha(&malha);
        return (MalhaPoligonal){0};
    }

    unsigned int idx_v = 0, idx_t = 0;
    float h2 = altura * 0.5f;

    // Corpo lateral do cilindro
    for (unsigned int i = 0; i <= segmentos; i++) {
        float t = (float)i / (float)segmentos * 2.0f * MAT_PI;
        float cx = cosf(t), cz = sinf(t);
        Vetor3D n_radial = {cx, 0.0f, cz};
        malha.vértices[idx_v++] = (Vértice){{cx*raio,  h2, cz*raio}, n_radial};
        malha.vértices[idx_v++] = (Vértice){{cx*raio, -h2, cz*raio}, n_radial};
    }
    for (unsigned int i = 0; i < segmentos; i++) {
        unsigned int b = i * 2;
        malha.triângulos[idx_t++] = (Triângulo){b, b+2, b+1};
        malha.triângulos[idx_t++] = (Triângulo){b+2, b+3, b+1};
    }

    // Tampa Superior (Normais viradas para cima)
    unsigned int base_sup = idx_v;
    malha.vértices[idx_v++] = (Vértice){{0, h2, 0}, {0, 1, 0}};
    for(unsigned int i=0; i<=segmentos; i++){
        float t = (float)i / (float)segmentos * 2.0f * MAT_PI;
        malha.vértices[idx_v++] = (Vértice){{cosf(t)*raio, h2, sinf(t)*raio}, {0, 1, 0}};
    }
    for(unsigned int i=0; i<segmentos; i++){
        malha.triângulos[idx_t++] = (Triângulo){base_sup, base_sup + 2 + i, base_sup + 1 + i};
    }

    // Tampa Inferior (Normais viradas para baixo)
    unsigned int base_inf = idx_v;
    malha.vértices[idx_v++] = (Vértice){{0, -h2, 0}, {0, -1, 0}};
    for(unsigned int i=0; i<=segmentos; i++){
        float t = (float)i / (float)segmentos * 2.0f * MAT_PI;
        malha.vértices[idx_v++] = (Vértice){{cosf(t)*raio, -h2, sinf(t)*raio}, {0, -1, 0}};
    }
    for(unsigned int i=0; i<segmentos; i++){
        malha.triângulos[idx_t++] = (Triângulo){base_inf, base_inf + 1 + i, base_inf + 2 + i};
    }

    return malha;
}

// =============================================================================
// SECÇÃO 4 DE 5: PIPELINE DE RASTERIZAÇÃO E INTERPOLAÇÃO DE ATRIBUTOS
// =============================================================================

void limpar_contexto_gráfico(ContextoGráfico *ctx, CorRGBA cor_fundo) {
    int total = ctx->largura * ctx->altura;
    for (int i = 0; i < total; i++) {
        ctx->buffer_píxeis[i] = cor_fundo;
        ctx->buffer_profundidade[i] = 10000.0f;
    }
}

bool testar_abate_faces_ocultas(Vetor2Di v0, Vetor2Di v1, Vetor2Di v2) {
    return ((v1.x - v0.x) * (v2.y - v0.y) - (v1.y - v0.y) * (v2.x - v0.x)) <= 0;
}

CoordenadasBaricêntricas calcular_baricêntricas(Vetor2Di p, Vetor2Di v0, Vetor2Di v1, Vetor2Di v2) {
    float den = (float)((v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y));
    if (fabsf(den) < 0.00001f) return (CoordenadasBaricêntricas){-1.0f, -1.0f, -1.0f};
    float alpha = ((v1.y - v2.y) * ((float)p.x - (float)v2.x) + (v2.x - v1.x) * ((float)p.y - (float)v2.y)) / den;
    float beta  = ((v2.y - v0.y) * ((float)p.x - (float)v2.x) + (v0.x - v2.x) * ((float)p.y - (float)v2.y)) / den;
    return (CoordenadasBaricêntricas){alpha, beta, 1.0f - alpha - beta};
}

Vetor3D interpolar_atributo_3d(CoordenadasBaricêntricas bar, Vetor3D a0, Vetor3D a1, Vetor3D a2, float w0, float w1, float w2) {
    float n_alpha = bar.alpha * w0;
    float n_beta  = bar.beta * w1;
    float n_gamma = bar.gamma * w2;
    float soma = n_alpha + n_beta + n_gamma;
    if (soma < 0.00001f) return a0;
    return (Vetor3D){
        (a0.x * n_alpha + a1.x * n_beta + a2.x * n_gamma) / soma,
        (a0.y * n_alpha + a1.y * n_beta + a2.y * n_gamma) / soma,
        (a0.z * n_alpha + a1.z * n_beta + a2.z * n_gamma) / soma
    };
}

// =============================================================================
// SECÇÃO 5 DE 5: MODELO DE SOMBREAMENTO DE PHONG E CICLO PRINCIPAL (MAIN)
// =============================================================================

CorRGBA calcular_píxel_phong(Vetor3D pos_píxel, Vetor3D normal_píxel, Vetor3D pos_câmara, LuzPontual luz, Material mat) {
    Vetor3D N = vetor3d_normalizar(normal_píxel);
    Vetor3D L = vetor3d_normalizar(vetor3d_subtrair(luz.posição, pos_píxel));
    Vetor3D V = vetor3d_normalizar(vetor3d_subtrair(pos_câmara, pos_píxel));
    
    Vetor3D ambiente = mat.coeficiente_ambiente;

    float fator_difuso = fmaxf(vetor3d_produto_escalar(N, L), 0.0f);
    Vetor3D difusa = (Vetor3D){
        mat.coeficiente_difuso.x * luz.cor.x * fator_difuso,
        mat.coeficiente_difuso.y * luz.cor.y * fator_difuso,
        mat.coeficiente_difuso.z * luz.cor.z * fator_difuso
    };

    Vetor3D especular = {0.0f, 0.0f, 0.0f};
    if (fator_difuso > 0.0f) {
        float dotNL = vetor3d_produto_escalar(N, L);
        Vetor3D R = vetor3d_subtrair(vetor3d_multiplicar_escalar(N, 2.0f * dotNL), L);
        R = vetor3d_normalizar(R);
        
        float fator_especular = powf(fmaxf(vetor3d_produto_escalar(R, V), 0.0f), mat.brilho);
        especular = (Vetor3D){
            mat.coeficiente_especular.x * luz.cor.x * fator_especular,
            mat.coeficiente_especular.y * luz.cor.y * fator_especular,
            mat.coeficiente_especular.z * luz.cor.z * fator_especular
        };
    }

    Vetor3D final = vetor3d_adicionar(vetor3d_adicionar(ambiente, difusa), especular);
    return (CorRGBA){
        (unsigned char)fminf(fmaxf(final.x * 255.0f, 0.0f), 255.0f),
        (unsigned char)fminf(fmaxf(final.y * 255.0f, 0.0f), 255.0f),
        (unsigned char)fminf(fmaxf(final.z * 255.0f, 0.0f), 255.0f),
        255
    };
}

void desenhar_píxel_rastreado(ContextoGráfico *ctx, int x, int y, CoordenadasBaricêntricas bar, 
                             Vértice v0, Vértice v1, Vértice v2, float inv_w0, float inv_w1, float inv_w2,
                             Material mat, LuzPontual luz, Vetor3D pos_câmara) {
    if (x < 0 || x >= ctx->largura || y < 0 || y >= ctx->altura) return;
    
    float inv_z = bar.alpha * inv_w0 + bar.beta * inv_w1 + bar.gamma * inv_w2;
    if (inv_z <= 0.00001f) return;
    float z_atual = 1.0f / inv_z;

    int idx = y * ctx->largura + x;
    if (z_atual < ctx->buffer_profundidade[idx]) {
        ctx->buffer_profundidade[idx] = z_atual;
        
        Vetor3D p_mundo = interpolar_atributo_3d(bar, v0.posição, v1.posição, v2.posição, inv_w0, inv_w1, inv_w2);
        Vetor3D n_mundo = interpolar_atributo_3d(bar, v0.normal, v1.normal, v2.normal, inv_w0, inv_w1, inv_w2);
        
        ctx->buffer_píxeis[idx] = calcular_píxel_phong(p_mundo, n_mundo, pos_câmara, luz, mat);
    }
}

void rasterizar_triângulo_phong(ContextoGráfico *ctx, Vértice v0_m, Vértice v1_m, Vértice v2_m, 
                                Matriz4x4 mvp, Material mat, LuzPontual luz, Vetor3D pos_câmara) {
    Vetor4D p0_c = matriz4x4_transformar_vetor4d(mvp, v0_m.posição);
    Vetor4D p1_c = matriz4x4_transformar_vetor4d(mvp, v1_m.posição);
    Vetor4D p2_c = matriz4x4_transformar_vetor4d(mvp, v2_m.posição);

    if (p0_c.w <= 0.1f || p1_c.w <= 0.1f || p2_c.w <= 0.1f) return; 

    Vetor2Di s0 = {
        (int)(( (p0_c.x / p0_c.w) + 1.0f) * 0.5f * (float)ctx->largura), 
        (int)((-(p0_c.y / p0_c.w) + 1.0f) * 0.5f * (float)ctx->altura)
    };
    Vetor2Di s1 = {
        (int)(( (p1_c.x / p1_c.w) + 1.0f) * 0.5f * (float)ctx->largura), 
        (int)((-(p1_c.y / p1_c.w) + 1.0f) * 0.5f * (float)ctx->altura)
    };
    Vetor2Di s2 = {
        (int)(( (p2_c.x / p2_c.w) + 1.0f) * 0.5f * (float)ctx->largura), 
        (int)((-(p2_c.y / p2_c.w) + 1.0f) * 0.5f * (float)ctx->altura)
    };

    if (testar_abate_faces_ocultas(s0, s1, s2)) return;

    int min_x = (int)fmaxf(0.0f, floorf(fminf((float)s0.x, fminf((float)s1.x, (float)s2.x))));
    int max_x = (int)fminf((float)(ctx->largura - 1), ceilf(fmaxf((float)s0.x, fmaxf((float)s1.x, (float)s2.x))));
    int min_y = (int)fmaxf(0.0f, floorf(fminf((float)s0.y, fminf((float)s1.y, (float)s2.y))));
    int max_y = (int)fminf((float)(ctx->altura - 1), ceilf(fmaxf((float)s0.y, fmaxf((float)s1.y, (float)s2.y))));

    float inv_w0 = 1.0f / p0_c.w, inv_w1 = 1.0f / p1_c.w, inv_w2 = 1.0f / p2_c.w;

    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            Vetor2Di p = {x, y};
            CoordenadasBaricêntricas bar = calcular_baricêntricas(p, s0, s1, s2);
            if (bar.alpha >= 0.0f && bar.beta >= 0.0f && bar.gamma >= 0.0f) {
                desenhar_píxel_rastreado(ctx, x, y, bar, v0_m, v1_m, v2_m, inv_w0, inv_w1, inv_w2, mat, luz, pos_câmara);
            }
        }
    }
}

void renderizar_malha(ContextoGráfico *ctx, const MalhaPoligonal *malha, Matriz4x4 mundo, Câmara3D câmara, Material mat, LuzPontual luz) {
    Matriz4x4 vista = matriz4x4_olhar_para(câmara.posição, câmara.alvo, câmara.vetor_cima);
    Matriz4x4 proj = matriz4x4_projeção_perspetiva(câmara.campo_de_visão_radianos, (float)ctx->largura/(float)ctx->altura, câmara.plano_próximo, câmara.plano_distante);
    
    Matriz4x4 mundo_vista = matriz4x4_multiplicar(mundo, vista);
    Matriz4x4 mvp = matriz4x4_multiplicar(mundo_vista, proj);

    // Isolar a rotação pura anulando os índices de translação da última linha (12, 13, 14)
    Matriz4x4 rot_mundo = mundo;
    rot_mundo.m[12] = 0.0f; 
    rot_mundo.m[13] = 0.0f; 
    rot_mundo.m[14] = 0.0f;

    for (unsigned int i = 0; i < malha->contagem_triângulos; i++) {
        Triângulo t = malha->triângulos[i];
        Vértice v0 = malha->vértices[t.v0];
        Vértice v1 = malha->vértices[t.v1];
        Vértice v2 = malha->vértices[t.v2];

        v0.posição = matriz4x4_transformar_posição(mundo, v0.posição);
        v1.posição = matriz4x4_transformar_posição(mundo, v1.posição);
        v2.posição = matriz4x4_transformar_posição(mundo, v2.posição);
        
        v0.normal  = vetor3d_normalizar(matriz4x4_transformar_direção(rot_mundo, v0.normal));
        v1.normal  = vetor3d_normalizar(matriz4x4_transformar_direção(rot_mundo, v1.normal));
        v2.normal  = vetor3d_normalizar(matriz4x4_transformar_direção(rot_mundo, v2.normal));

        rasterizar_triângulo_phong(ctx, v0, v1, v2, mvp, mat, luz, câmara.posição);
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    const int LARGURA = 800;
    const int ALTURA = 600;

    Ecrã *ecrã = criar_ecrã("Demonstracao de Phong Shading (C23 + SDL3)", LARGURA, ALTURA);
    if (!ecrã) {
        printf("Erro ao inicializar o ecra SDL3.\n");
        return 1;
    }

    ContextoGráfico ctx = {0};
    ctx.largura = LARGURA;
    ctx.altura = ALTURA;
    ctx.buffer_píxeis = malloc(sizeof(CorRGBA) * (size_t)(LARGURA * ALTURA));
    ctx.buffer_profundidade = malloc(sizeof(float) * (size_t)(LARGURA * ALTURA));
    if (!ctx.buffer_píxeis || !ctx.buffer_profundidade) {
        free(ctx.buffer_píxeis);
        free(ctx.buffer_profundidade);
        destruir_ecrã(ecrã);
        return 1;
    }

    MalhaPoligonal cubo = gerar_polígonos_cubo(1.6f);
    MalhaPoligonal cilindro = gerar_polígonos_cilindro(0.9f, 2.0f, 24);
    MalhaPoligonal esfera = gerar_polígonos_esfera(1.1f, 20, 20);

    Material material_ouro = { {0.10f, 0.08f, 0.02f}, {0.60f, 0.50f, 0.15f}, {0.70f, 0.60f, 0.30f}, 40.0f };
    Material material_esmeralda = { {0.02f, 0.10f, 0.02f}, {0.05f, 0.70f, 0.05f}, {0.60f, 0.60f, 0.60f}, 25.0f };
    Material material_bronze = { {0.12f, 0.05f, 0.02f}, {0.65f, 0.30f, 0.10f}, {0.50f, 0.40f, 0.30f}, 15.0f };

    Entidade3D objetos[3] = {
        [0] = { FORMA_CUBO,     {-2.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, material_ouro },
        [1] = { FORMA_ESFERA,   { 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, material_esmeralda },
        [2] = { FORMA_CILINDRO, { 2.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, material_bronze }
    };

    Câmara3D câmara = { {0.0f, 2.5f, 7.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f * (MAT_PI / 180.0f), 0.1f, 100.0f };
    LuzPontual luz = { {2.0f, 4.0f, 4.0f}, {1.0f, 1.0f, 1.0f} }; 

    float tempo = 0.0f;
    bool a_correr = true;
    CorRGBA cor_fundo = {15, 15, 25, 255}; 

    while (a_correr) {
        if (ecrã_deve_fechar()) {
            a_correr = false;
        }

        limpar_contexto_gráfico(&ctx, cor_fundo);
        tempo += 0.02f;

        objetos[0].rotação = (Vetor3D){tempo * 0.5f, tempo * 0.8f, 0.0f};
        objetos[1].rotação = (Vetor3D){0.0f, tempo * 0.6f, tempo * 0.2f};
        objetos[2].rotação = (Vetor3D){tempo * 0.7f, 0.0f, tempo * 0.4f};

        luz.posição.x = cosf(tempo) * 4.0f;
        luz.posição.z = sinf(tempo) * 4.0f;

        for (int i = 0; i < 3; i++) {
            Matriz4x4 m_escala = matriz4x4_criar_escala(objetos[i].escala.x, objetos[i].escala.y, objetos[i].escala.z);
            Matriz4x4 m_rot    = matriz4x4_criar_rotação_xyz(objetos[i].rotação.x, objetos[i].rotação.y, objetos[i].rotação.z);
            Matriz4x4 m_trans  = matriz4x4_criar_translação(objetos[i].posição.x, objetos[i].posição.y, objetos[i].posição.z);
            
            Matriz4x4 m_mundo  = matriz4x4_multiplicar(matriz4x4_multiplicar(m_escala, m_rot), m_trans);

            const MalhaPoligonal *malha_alvo = nullptr;
            if (objetos[i].tipo == FORMA_CUBO) malha_alvo = &cubo;
            else if (objetos[i].tipo == FORMA_CILINDRO) malha_alvo = &cilindro;
            else if (objetos[i].tipo == FORMA_ESFERA) malha_alvo = &esfera;

            if (malha_alvo) {
                renderizar_malha(&ctx, malha_alvo, m_mundo, câmara, objetos[i].material, luz);
            }
        }

        atualizar_ecrã(ecrã, &ctx);
        SDL_Delay(16); 
    }

    libertar_memória_malha(&cubo);
    libertar_memória_malha(&cilindro);
    libertar_memória_malha(&esfera);
    free(ctx.buffer_píxeis);
    free(ctx.buffer_profundidade);
    destruir_ecrã(ecrã);

    return 0;
}
