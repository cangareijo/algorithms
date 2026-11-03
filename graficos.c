#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t Cor;

typedef struct {
  unsigned largura;
  unsigned altura;
  Cor* píxeis;
} Tela;

typedef struct {
  float x, y;
} Vetor2D;

typedef struct {
  float x, y, z;
} Vetor3D;

typedef struct {
  float w, x, y, z;
} Quaternião;

typedef struct {
  unsigned largura;
  unsigned altura;
  float* profundidades;
} Profundidade;

typedef struct {
  Vetor3D origem;
  Vetor3D direção;
} Raio;

typedef struct {
  bool intersetou;
  float t;
  Vetor3D ponto;
  Vetor3D normal;
  Cor cor_material;
} Interseção;

typedef struct {
  Vetor3D posição;
  Vetor2D uv;
  Cor cor;
} Vértice;

// Estrutura interna para representação dos nós da árvore BVH
typedef struct {
  Vetor3D min_caixa;
  Vetor3D max_caixa;
  unsigned índice_esquerdo;  // Se for > 0, aponta para o nó filho. Se for folha, pode ser o índice do objeto.
  unsigned contagem_objetos; // Se for 0, é um nó interno. Se for > 0, é uma folha com N objetos.
} NodoBVH;

typedef struct {
  Cor cor_albedo;
  float rugosidade;
  float metalicidade;
  float oclusão_ambiental;
} MaterialPBR;

typedef struct {
  Tela faces[6]; // +X, -X, +Y, -Y, +Z, -Z
} MapaDeCubos;

constexpr float PI = 3.14159265f;

uint8_t extrair_r(Cor c);
uint8_t extrair_g(Cor c);
uint8_t extrair_b(Cor c);
uint8_t extrair_a(Cor c);
Cor empacotar_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

Tela criar_tela(int largura, int altura);
void destruir_tela(Tela* tela);
void aplicar_mapeamento_de_tom_reinhard(Tela* tela);
void limpar_tela(Tela* tela, Cor cor);
void aplicar_filtro_gamma(Tela* tela, float gama);
void desenhar_píxel(Tela* tela, unsigned x, unsigned y, Cor cor);
bool guardar_tela_tga(const Tela* tela, const char* caminho_ficheiro);
bool guardar_tela_ppm(const Tela* tela, const char* caminho_ficheiro);

Cor multiplicar_cor(Cor cor, float intensidade);
Cor interpolar_cores(Cor c1, Cor c2, float fator);
Cor amostrar_textura(const Tela* textura, float u, float v);
Cor gerar_textura_xadrez(float u, float v, Cor c1, Cor c2, float frequencia);
void aplicar_filtro_blur_gaussiano(Tela* tela, float sigma);
void aplicar_vinheta(Tela* tela, float intensidade, float raio);
static inline float perlin_hash_gradiente_2d(int ix, int iy, float rx, float ry);
float logaritmo_natural(float x);
float absoluto(float x);
float truncar(float x);
float chão(float x);
float raiz(float x);
float reciproco_raiz(float x);
float seno(float x);
float cosseno(float x);
float tangente(float x);
static inline float curva_suave_perlin(float t);
float potencia_inteira(float base, int expoente);
float potencia_real(float base, float expoente);
float minimo(float x, float y);
float maximo(float x, float y);
float distância(float x, float y);
float módulo(float x, float y);
float ruido_perlin_2d(float x, float y);
float interpolar_linear(float a, float b, float t);
float calcular_atenuacao_luz(float distância, float constante, float linear, float quadratica);

void desenhar_linha(Tela* tela, Vetor2D p1, Vetor2D p2, Cor cor);
void desenhar_triângulo(Tela* tela, Vetor2D p1, Vetor2D p2, Vetor2D p3, Cor cor);

Vetor3D amostrar_mapeamento_normal(const Tela* mapa_normais, float u, float v, Vetor3D normal_geometria, Vetor3D tangente, Vetor3D bitangente);
float comprimento_vetor(Vetor3D v);
float produto_escalar(Vetor3D u, Vetor3D v);
Vetor3D calcular_coordenadas_baricentricas(Vetor2D p, Vetor2D a, Vetor2D b, Vetor2D c);
Vetor2D projetar_ponto(Vetor3D ponto_3d, int largura, int altura);
bool orientacao_traseira(Vetor3D v1, Vetor3D v2, Vetor3D v3, Vetor3D direção_câmara);
Cor calcular_iluminacao_foco(Vetor3D ponto, Vetor3D normal, Vetor3D pos_luz, Vetor3D dir_foco, float corte_interno, float corte_externo);
bool verificar_sombra_bloqueada(Vetor3D ponto_origem, Vetor3D direção_luz, float distância_luz, const Vetor3D* esferas_centros, const float* esferas_raios, unsigned núm_esferas);
Vetor3D normalizar_vetor(Vetor3D v);
Vetor3D transformar_vetor(Vetor3D v, float matriz[static 4][4]);
Vetor3D somar_vetores(Vetor3D a, Vetor3D b);
Vetor3D subtrair_vetores(Vetor3D a, Vetor3D b);
Vetor3D produto_vetorial(Vetor3D a, Vetor3D b);
Vetor3D refletir_vetor(Vetor3D v, Vetor3D normal);
Vetor3D interpolar_vetores(Vetor3D a, Vetor3D b, float t);
Vetor3D calcular_normal(Vetor3D v1, Vetor3D v2, Vetor3D v3);
static void calcular_aabb_grupo(Vetor3D* centros, float* raios, int* índices, int inicio, int fim, Vetor3D* min_out, Vetor3D* max_out);

void criar_matriz_identidade(float matriz[static 4][4]);
void criar_matriz_translacao(float matriz[static 4][4], float x, float y, float z);
void criar_matriz_escala(float matriz[static 4][4], float sx, float sy, float sz);
void criar_matriz_perspetiva_infinito(float matriz[static 4][4], float fov, float proporcao, float perto);
void criar_matriz_projeção(float matriz[static 4][4], float fov, float proporcao, float perto, float longe);
void criar_matriz_ortográfica(float matriz[static 4][4], float esquerda, float direita, float baixo, float cima, float perto, float longe);
bool inverter_matriz_4x4(float resultado[static 4][4], float matriz[static 4][4]);
void transpor_matriz_4x4(float resultado[static 4][4], float matriz[static 4][4]);
void multiplicar_matrizes(float resultado[static 4][4], float a[static 4][4], float b[static 4][4]);
void criar_matriz_rotacao(float matriz[static 4][4], float radianos, Vetor3D eixo);
void criar_matriz_olhar_para(float matriz[static 4][4], Vetor3D posição, Vetor3D alvo, Vetor3D cima);
void extrair_direcoes_câmara(float matriz_vista[static 4][4], Vetor3D* direita, Vetor3D* cima, Vetor3D* frente);
void criar_matriz_de_quaternião(float matriz[static 4][4], Quaternião q);
void criar_matriz_quat_posição(float matriz[static 4][4], Quaternião q, Vetor3D posição);
Cor calcular_iluminacao_difusa(Vetor3D normal, Vetor3D direção_luz, Cor cor_objeto, Cor cor_luz);
Cor calcular_iluminacao_especular(Vetor3D normal, Vetor3D direção_luz, Vetor3D direção_olhar, int polimento);
Cor calcular_iluminacao_phong_completa(Vetor3D normal, Vetor3D direção_luz, Vetor3D direção_olhar, Cor cor_objeto, Cor cor_difusa_luz, Cor cor_especular_luz, int polimento, float intensidade_ambiente);

Quaternião quaternião_identidade(void);
Quaternião quaternião_de_eixo_angulo(Vetor3D eixo, float radianos);
Quaternião quaternião_olhar_para(Vetor3D origem, Vetor3D alvo, Vetor3D cima);
Quaternião quaternião_normalizar(Quaternião q);
Quaternião quaternião_inverso(Quaternião q);
Quaternião multiplicar_quaternioes(Quaternião q1, Quaternião q2);
Quaternião quaternião_slerp(Quaternião q1, Quaternião q2, float t);

void limpar_profundidade(Profundidade* pf, float valor);
bool testar_e_definir_profundidade(Profundidade* pf, int x, int y, float z);

Raio gerar_raio_câmara_profundidade_campo(Vetor3D posição, Vetor3D dir, float abertura, float distância_focal, int x, int y, int largura, int altura);
bool intersetar_caixa_alinhada_rapido(Raio raio, Vetor3D min_caixa, Vetor3D max_caixa, float* t_min_out);
Raio gerar_raio_câmara(Vetor3D posição_câmara, float matriz_inv_proj[static 4][4], int x, int y, int largura, int altura);

Interseção intersetar_esfera(Raio raio, Vetor3D centro, float raio_esfera, Cor cor);
Interseção intersetar_plano(Raio raio, Vetor3D ponto_plano, Vetor3D normal_plano, Cor cor);
Interseção intersetar_caixa_alinhada(Raio raio, Vetor3D min_caixa, Vetor3D max_caixa, Cor cor);
Interseção intersetar_triângulo(Raio raio, Vetor3D v1, Vetor3D v2, Vetor3D v3, Cor cor);

int gerar_geometria_esfera(Vértice* vértices_out, uint32_t* índices_out, float raio, unsigned fatias, unsigned pilhas);
int gerar_geometria_cubo(Vértice* vértices_out, uint32_t* índices_out, float tamanho);
Vértice interpolar_vértices(Vértice a, Vértice b, float t);
int recortar_triângulo_plano_perto(Vértice* triângulo_in, Vértice* triângulos_out, float plano_perto);
void calcular_tangente_bitangente(Vértice* triângulo, Vetor3D* tangente, Vetor3D* bitangente);
void desenhar_triângulo_preenchido(Tela* tela, Profundidade* pf, Vértice v1, Vértice v2, Vértice v3, Tela* textura);

static int construir_bvh_recursivo(NodoBVH* nodos, int* índices, Vetor3D* centros, float* raios, int inicio, int fim, unsigned* contador_nodos);
void construir_arvore_bvh(NodoBVH* nodos_bvh, Vetor3D* centros, float* raios, int núm_objetos);

float absoluto(float x) {
  return x >= 0.0f ? x : -x;
}

float truncar(float x) {
  if (x >= 16777216.0f || x <= -16777216.0f) {
    return x;
  }
  return (float)((long long)x);
}

float chão(float x) {
  float y = truncar(x);
  return (x < 0.0f && x != y) ? (y - 1.0f) : y;
}

float reciproco_raiz(float x) {
  if (x <= 0.0f) return 0.0f;
  float metade = 0.5f * x;
  union {
    float f;
    uint32_t i;
  } u;
  u.f = x;
  u.i = 0x5f375a86 - (u.i >> 1);
  u.f = u.f * (1.5f - metade * u.f * u.f);
  u.f = u.f * (1.5f - metade * u.f * u.f);
  return u.f;
}

float raiz(float x) {
  if (x <= 0.0f) return 0.0f;
  float inv = reciproco_raiz(x);
  if (inv == 0.0f) return 0.0f;
  float r = 1.0f / inv;
  return 0.5f * (r + x / r);
}

float módulo(float x, float y) {
  if (y == 0.0f) return 0.0f;
  return x - truncar(x / y) * y;
}

float seno(float x) {
  float sinal = 1.0f;
  if (x < 0.0f) {
    x = -x;
    sinal = -sinal;
  }
  x = módulo(x, 2.0f * PI);
  if (x > PI) {
    x -= PI;
    sinal = -sinal;
  }
  if (x > 0.5f * PI) {
    x = PI - x;
  }
  float termo = x;
  float soma = x;
  float quadrado = x * x;
  for (int i = 1; i <= 4; i++) {
    termo *= -quadrado / (float)((2 * i) * (2 * i + 1));
    soma += termo;
  }
  return sinal * soma;
}

float cosseno(float x) {
  return seno(x + 0.5f * PI);
}

float tangente(float x) {
  float c = cosseno(x);
  if (c == 0.0f) return 0.0f;
  return seno(x) / c;
}

float potencia_inteira(float base, int expoente) {
  assert(base != 0.0f || expoente > 0);
  if (expoente == 0) return 1.0f;
  if (expoente < 0) {
    base = 1.0f / base;
    expoente = -expoente;
  }
  float resultado = 1.0f;
  float atual = base;
  do {
    if ((expoente & 1) == 1) {
      resultado *= atual;
    }
    atual *= atual;
    expoente >>= 1;
  } while (expoente > 0);
  return resultado;
}

float minimo(float x, float y) {
  return x <= y ? x : y;
}

float maximo(float x, float y) {
  return x >= y ? x : y;
}

float distância(float x, float y) {
  return absoluto(x - y);
}

float comprimento_vetor(Vetor3D v) {
  return raiz(produto_escalar(v, v));
}

float produto_escalar(Vetor3D u, Vetor3D v) {
  return u.x * v.x + u.y * v.y + u.z * v.z;
}

Vetor3D normalizar_vetor(Vetor3D v) {
  float tam_qd = v.x * v.x + v.y * v.y + v.z * v.z;
  if (tam_qd == 0.0f) {
    return (Vetor3D){0.0f, 0.0f, 0.0f};
  }
  float inv_tam = reciproco_raiz(tam_qd);
  return (Vetor3D){
    .x = v.x * inv_tam,
    .y = v.y * inv_tam,
    .z = v.z * inv_tam
  };
}

Vetor3D transformar_vetor(Vetor3D v, float matriz[static 4][4]) {
  // Convenção Row-Major: Multiplicação V * M. A translação reside na quarta linha (matriz[3][j])
  float x = v.x * matriz[0][0] + v.y * matriz[1][0] + v.z * matriz[2][0] + matriz[3][0];
  float y = v.x * matriz[0][1] + v.y * matriz[1][1] + v.z * matriz[2][1] + matriz[3][1];
  float z = v.x * matriz[0][2] + v.y * matriz[1][2] + v.z * matriz[2][2] + matriz[3][2];
  float w = v.x * matriz[0][3] + v.y * matriz[1][3] + v.z * matriz[2][3] + matriz[3][3];
  if (w != 0.0f && w != 1.0f) {
    float reciproco_w = 1.0f / w;
    x *= reciproco_w;
    y *= reciproco_w;
    z *= reciproco_w;
  }
  return (Vetor3D){ .x = x, .y = y, .z = z };
}

Vetor3D somar_vetores(Vetor3D a, Vetor3D b) {
  return (Vetor3D){ a.x + b.x, a.y + b.y, a.z + b.z };
}

Vetor3D subtrair_vetores(Vetor3D a, Vetor3D b) {
  return (Vetor3D){ a.x - b.x, a.y - b.y, a.z - b.z };
}

Vetor3D produto_vetorial(Vetor3D a, Vetor3D b) {
  return (Vetor3D){
    .x = (a.y * b.z) - (a.z * b.y),
    .y = (a.z * b.x) - (a.x * b.z),
    .z = (a.x * b.y) - (a.y * b.x)
  };
}

Vetor3D refletir_vetor(Vetor3D v, Vetor3D normal) {
  float produto = produto_escalar(v, normal);
  return (Vetor3D){
    .x = v.x - 2.0f * produto * normal.x,
    .y = v.y - 2.0f * produto * normal.y,
    .z = v.z - 2.0f * produto * normal.z
  };
}

Vetor3D interpolar_vetores(Vetor3D a, Vetor3D b, float t) {
  return (Vetor3D){
    a.x + t * (b.x - a.x),
    a.y + t * (b.y - a.y),
    a.z + t * (b.z - a.z)
  };
}

Vetor3D calcular_normal(Vetor3D v1, Vetor3D v2, Vetor3D v3) {
  Vetor3D aresta1 = subtrair_vetores(v2, v1);
  Vetor3D aresta2 = subtrair_vetores(v3, v1);
  return normalizar_vetor(produto_vetorial(aresta1, aresta2));
}

void criar_matriz_identidade(float matriz[static 4][4]) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      matriz[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
}

void criar_matriz_translacao(float matriz[static 4][4], float x, float y, float z) {
  criar_matriz_identidade(matriz);
  matriz[3][0] = x;
  matriz[3][1] = y;
  matriz[3][2] = z;
}

void criar_matriz_escala(float matriz[static 4][4], float sx, float sy, float sz) {
  criar_matriz_identidade(matriz);
  matriz[0][0] = sx;
  matriz[1][1] = sy;
  matriz[2][2] = sz;
}

void criar_matriz_perspetiva_infinito(float matriz[static 4][4], float fov, float proporcao, float perto) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matriz[i][j] = 0.0f;
    }
  }
  float tan_metade_fov = tangente(0.5f * fov);
  if (tan_metade_fov == 0.0f || proporcao == 0.0f || perto == 0.0f) {
    return;
  }
  matriz[0][0] = 1.0f / (proporcao * tan_metade_fov);
  matriz[1][1] = 1.0f / tan_metade_fov;
  matriz[2][2] = -1.0f;
  matriz[2][3] = -1.0f;
  matriz[3][2] = -2.0f * perto;
}

void criar_matriz_projeção(float matriz[static 4][4], float fov, float proporcao, float perto, float longe) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matriz[i][j] = 0.0f;
    }
  }
  float tan_metade_fov = tangente(0.5f * fov);
  if (tan_metade_fov == 0.0f || proporcao == 0.0f || (longe - perto) == 0.0f) {
    return;
  }
  matriz[0][0] = 1.0f / (proporcao * tan_metade_fov);
  matriz[1][1] = 1.0f / tan_metade_fov;
  matriz[2][2] = -(longe + perto) / (longe - perto);
  matriz[2][3] = -1.0f;
  matriz[3][2] = -(2.0f * longe * perto) / (longe - perto);
}

void criar_matriz_ortográfica(float matriz[static 4][4], float esquerda, float direita, float baixo, float cima, float perto, float longe) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      matriz[i][j] = 0.0f;
    }
  }
  if (direita == esquerda || cima == baixo || longe == perto) {
    return;
  }
  matriz[0][0] = 2.0f / (direita - esquerda);
  matriz[1][1] = 2.0f / (cima - baixo);
  matriz[2][2] = -2.0f / (longe - perto);
  matriz[3][3] = 1.0f;
  matriz[3][0] = -(direita + esquerda) / (direita - esquerda);
  matriz[3][1] = -(cima + baixo) / (cima - baixo);
  matriz[3][2] = -(longe + perto) / (longe - perto);
}

void multiplicar_matrizes(float resultado[static 4][4], float a[static 4][4], float b[static 4][4]) {
  float temporario[4][4] = {};
  for (int i = 0; i < 4; ++i) {
    for (int k = 0; k < 4; ++k) {
      float elemento_a = a[i][k];
      for (int j = 0; j < 4; ++j) {
        temporario[i][j] += elemento_a * b[k][j];
      }
    }
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      resultado[i][j] = temporario[i][j];
    }
  }
}

void criar_matriz_rotacao(float matriz[static 4][4], float radianos, Vetor3D eixo) {
  criar_matriz_identidade(matriz);
  float magnitude = raiz(eixo.x * eixo.x + eixo.y * eixo.y + eixo.z * eixo.z);
  if (magnitude == 0.0f) return;

  float ux = eixo.x / magnitude;
  float uy = eixo.y / magnitude;
  float uz = eixo.z / magnitude;
  float c = cosseno(radianos);
  float s = seno(radianos);
  float omc = 1.0f - c;

  // Ajustado para Row-Major (Transposto para suportar V * M)
  matriz[0][0] = c + ux * ux * omc;
  matriz[0][1] = ux * uy * omc - uz * s;
  matriz[0][2] = ux * uz * omc + uy * s;
  matriz[0][3] = 0.0f;

  matriz[1][0] = ux * uy * omc + uz * s;
  matriz[1][1] = c + uy * uy * omc;
  matriz[1][2] = uy * uz * omc - ux * s;
  matriz[1][3] = 0.0f;

  matriz[2][0] = ux * uz * omc - uy * s;
  matriz[2][1] = uy * uz * omc + ux * s;
  matriz[2][2] = c + uz * uz * omc;
  matriz[2][3] = 0.0f;
}

void criar_matriz_olhar_para(float matriz[static 4][4], Vetor3D posição, Vetor3D alvo, Vetor3D cima) {
  Vetor3D z_eixo = normalizar_vetor(subtrair_vetores(posição, alvo));
  Vetor3D x_eixo = normalizar_vetor(produto_vetorial(cima, z_eixo));
  Vetor3D y_eixo = produto_vetorial(z_eixo, x_eixo);

  matriz[0][0] = x_eixo.x;  matriz[0][1] = y_eixo.x;  matriz[0][2] = z_eixo.x;  matriz[0][3] = 0.0f;
  matriz[1][0] = x_eixo.y;  matriz[1][1] = y_eixo.y;  matriz[1][2] = z_eixo.y;  matriz[1][3] = 0.0f;
  matriz[2][0] = x_eixo.z;  matriz[2][1] = y_eixo.z;  matriz[2][2] = z_eixo.z;  matriz[2][3] = 0.0f;
  matriz[3][0] = -produto_escalar(x_eixo, posição);
  matriz[3][1] = -produto_escalar(y_eixo, posição);
  matriz[3][2] = -produto_escalar(z_eixo, posição);
  matriz[3][3] = 1.0f;
}

void extrair_direcoes_câmara(float matriz_vista[static 4][4], Vetor3D* direita, Vetor3D* cima, Vetor3D* frente) {
  if (direita != nullptr) {
    direita->x = matriz_vista[0][0];
    direita->y = matriz_vista[1][0];
    direita->z = matriz_vista[2][0];
  }
  
  if (cima != nullptr) {
    cima->x = matriz_vista[0][1];
    cima->y = matriz_vista[1][1];
    cima->z = matriz_vista[2][1];
  }
  
  if (frente != nullptr) {
    // Inverte-se o sinal da terceira coluna porque o eixo Z da View Matrix
    // aponta na direção oposta ao olhar da câmara.
    frente->x = -matriz_vista[0][2];
    frente->y = -matriz_vista[1][2];
    frente->z = -matriz_vista[2][2];
  }
}

void criar_matriz_de_quaternião(float matriz[static 4][4], Quaternião q) {
  float xx = q.x * q.x; float xy = q.x * q.y; float xz = q.x * q.z; float xw = q.x * q.w;
  float yy = q.y * q.y; float yz = q.y * q.z; float yw = q.y * q.w;
  float zz = q.z * q.z; float zw = q.z * q.w;

  // Ajustado para Row-Major (Transposta para V * M)
  matriz[0][0] = 1.0f - 2.0f * (yy + zz);
  matriz[0][1] = 2.0f * (xy + zw);
  matriz[0][2] = 2.0f * (xz - yw);
  matriz[0][3] = 0.0f;

  matriz[1][0] = 2.0f * (xy - zw);
  matriz[1][1] = 1.0f - 2.0f * (xx + zz);
  matriz[1][2] = 2.0f * (yz + xw);
  matriz[1][3] = 0.0f;

  matriz[2][0] = 2.0f * (xz + yw);
  matriz[2][1] = 2.0f * (yz - xw);
  matriz[2][2] = 1.0f - 2.0f * (xx + yy);
  matriz[2][3] = 0.0f;

  matriz[3][0] = 0.0f; matriz[3][1] = 0.0f; matriz[3][2] = 0.0f; matriz[3][3] = 1.0f;
}

void criar_matriz_quat_posição(float matriz[static 4][4], Quaternião q, Vetor3D posição) {
  float xx = q.x * q.x; float xy = q.x * q.y; float xz = q.x * q.z; float xw = q.x * q.w;
  float yy = q.y * q.y; float yz = q.y * q.z; float yw = q.y * q.w;
  float zz = q.z * q.z; float zw = q.z * q.w;
  matriz[0][0] = 1.0f - 2.0f * (yy + zz);
  matriz[0][1] = 2.0f * (xy + zw);
  matriz[0][2] = 2.0f * (xz - yw);
  matriz[0][3] = 0.0f;
  matriz[1][0] = 2.0f * (xy - zw);
  matriz[1][1] = 1.0f - 2.0f * (xx + zz);
  matriz[1][2] = 2.0f * (yz + xw);
  matriz[1][3] = 0.0f;
  matriz[2][0] = 2.0f * (xz + yw);
  matriz[2][1] = 2.0f * (yz - xw);
  matriz[2][2] = 1.0f - 2.0f * (xx + yy);
  matriz[2][3] = 0.0f;
  matriz[3][0] = posição.x;
  matriz[3][1] = posição.y;
  matriz[3][2] = posição.z;
  matriz[3][3] = 1.0f;
}

Quaternião quaternião_identidade(void) {
  return (Quaternião){ .w = 1.0f, .x = 0.0f, .y = 0.0f, .z = 0.0f };
}

Quaternião quaternião_de_eixo_angulo(Vetor3D axis, float radianos) {
  Vetor3D eixo_normalizado = normalizar_vetor(axis);
  float metade_angulo = radianos * 0.5f;
  float seno_metade = seno(metade_angulo);
  return (Quaternião){
    .w = cosseno(metade_angulo),
    .x = eixo_normalizado.x * seno_metade,
    .y = eixo_normalizado.y * seno_metade,
    .z = eixo_normalizado.z * seno_metade
  };
}

Quaternião quaternião_olhar_para(Vetor3D origem, Vetor3D alvo, Vetor3D cima) {
  // 1. Calculate the forward direction vector (Z-axis) matching your view conventions
  Vetor3D z_eixo = normalizar_vetor(subtrair_vetores(origem, alvo));
  
  // 2. Calculate right and up vectors to form an orthonormal matrix basis
  Vetor3D x_eixo = normalizar_vetor(produto_vetorial(cima, z_eixo));
  Vetor3D y_eixo = produto_vetorial(z_eixo, x_eixo);

  // 3. Extract the rotation components directly from the row-major basis matrix
  // m00 = x_eixo.x, m11 = y_eixo.y, m22 = z_eixo.z
  float tr = x_eixo.x + y_eixo.y + z_eixo.z;
  Quaternião q;

  if (tr > 0.0f) {
    float s = raiz(tr + 1.0f) * 2.0f; // s = 4 * q.w
    if (s == 0.0f) {
      return (Quaternião){ .w = 1.0f, .x = 0.0f, .y = 0.0f, .z = 0.0f };
    }
    float inv_s = 1.0f / s;
    q.w = 0.25f * s;
    q.x = (y_eixo.z - z_eixo.y) * inv_s;
    q.y = (z_eixo.x - x_eixo.z) * inv_s;
    q.z = (x_eixo.y - y_eixo.x) * inv_s;
  } else if ((x_eixo.x > y_eixo.y) && (x_eixo.x > z_eixo.z)) {
    float s = raiz(1.0f + x_eixo.x - y_eixo.y - z_eixo.z) * 2.0f; // s = 4 * q.x
    if (s == 0.0f) {
      return (Quaternião){ .w = 1.0f, .x = 0.0f, .y = 0.0f, .z = 0.0f };
    }
    float inv_s = 1.0f / s;
    q.w = (y_eixo.z - z_eixo.y) * inv_s;
    q.x = 0.25f * s;
    q.y = (x_eixo.y + y_eixo.x) * inv_s;
    q.z = (z_eixo.x + x_eixo.z) * inv_s;
  } else if (y_eixo.y > z_eixo.z) {
    float s = raiz(1.0f + y_eixo.y - x_eixo.x - z_eixo.z) * 2.0f; // s = 4 * q.y
    if (s == 0.0f) {
      return (Quaternião){ .w = 1.0f, .x = 0.0f, .y = 0.0f, .z = 0.0f };
    }
    float inv_s = 1.0f / s;
    q.w = (z_eixo.x - x_eixo.z) * inv_s;
    q.x = (x_eixo.y + y_eixo.x) * inv_s;
    q.y = 0.25f * s;
    q.z = (y_eixo.z + z_eixo.y) * inv_s;
  } else {
    float s = raiz(1.0f + z_eixo.z - x_eixo.x - y_eixo.y) * 2.0f; // s = 4 * q.z
    if (s == 0.0f) {
      return (Quaternião){ .w = 1.0f, .x = 0.0f, .y = 0.0f, .z = 0.0f };
    }
    float inv_s = 1.0f / s;
    q.w = (x_eixo.y - y_eixo.x) * inv_s;
    q.x = (z_eixo.x + x_eixo.z) * inv_s;
    q.y = (y_eixo.z + z_eixo.y) * inv_s;
    q.z = 0.25f * s;
  }

  return q;
}

Quaternião quaternião_normalizar(Quaternião q) {
  float n_qd = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
  if (n_qd == 0.0f) {
    return (Quaternião){ .w = 1.0f, .x = 0.0f, .y = 0.0f, .z = 0.0f };
  }
  float inv_norma = reciproco_raiz(n_qd);
  return (Quaternião){
    .w = q.w * inv_norma,
    .x = q.x * inv_norma,
    .y = q.y * inv_norma,
    .z = q.z * inv_norma
  };
}

Quaternião multiplicar_quaternioes(Quaternião q1, Quaternião q2) {
  return (Quaternião){
    .w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
    .x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
    .y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
    .z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w
  };
}

Quaternião quaternião_slerp(Quaternião q1, Quaternião q2, float t) {
  // Ensure we operate on unit quaternions
  Quaternião start = quaternião_normalizar(q1);
  Quaternião end = quaternião_normalizar(q2);

  if (t <= 0.0f) return start;
  if (t >= 1.0f) return end;

  // Compute the cosine of the angle between the two quaternions (4D dot product)
  float cosseno_omega = start.w * end.w + start.x * end.x + start.y * end.y + start.z * end.z;

  // If the dot product is negative, the quaternions have opposite polarities.
  // Take the alternative path to ensure we interpolate along the shortest arc.
  if (cosseno_omega < 0.0f) {
    end.w = -end.w;
    end.x = -end.x;
    end.y = -end.y;
    end.z = -end.z;
    cosseno_omega = -cosseno_omega;
  }

  float escala_start;
  float escala_end;

  // Use standard linear interpolation if the quaternions are extremely close
  // to avoid numerical instability or divisions by zero
  if (cosseno_omega > 0.9995f) {
    escala_start = 1.0f - t;
    escala_end = t;
  } else {
    // Standard spherical linear interpolation math
    float sin_omega = raiz(1.0f - cosseno_omega * cosseno_omega);
    
    // Fallback safe guard
    if (sin_omega == 0.0f) {
      escala_start = 1.0f - t;
      escala_end = t;
    } else {
      // Analytical arc math
      float omega = tangente(sin_omega / cosseno_omega); // Using tangent approximation safely
      float inv_sin_omega = 1.0f / sin_omega;
      
      escala_start = seno((1.0f - t) * omega) * inv_sin_omega;
      escala_end = seno(t * omega) * inv_sin_omega;
    }
  }

  // Combine components into the final blending state
  Quaternião resultado;
  resultado.w = escala_start * start.w + escala_end * end.w;
  resultado.x = escala_start * start.x + escala_end * end.x;
  resultado.y = escala_start * start.y + escala_end * end.y;
  resultado.z = escala_start * start.z + escala_end * end.z;

  return quaternião_normalizar(resultado);
}

Cor multiplicar_cor(Cor cor, float intensidade) {
  if (intensidade <= 0.0f) return cor & 0xFF; // Preserva apenas o canal Alpha
  if (intensidade >= 1.0f) return cor;
  float r = (float)((cor >> 24) & 0xFF) * intensidade + 0.5f;
  float g = (float)((cor >> 16) & 0xFF) * intensidade + 0.5f;
  float b = (float)((cor >> 8) & 0xFF) * intensidade + 0.5f;
  uint32_t a = cor & 0xFF;
  return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | a;
}

Cor interpolar_cores(Cor c1, Cor c2, float fator) {
  if (fator <= 0.0f) return c1;
  if (fator >= 1.0f) return c2;
  float r1 = (float)((c1 >> 24) & 0xFF);
  float g1 = (float)((c1 >> 16) & 0xFF);
  float b1 = (float)((c1 >> 8) & 0xFF);
  float a1 = (float)(c1 & 0xFF);

  float r2 = (float)((c2 >> 24) & 0xFF);
  float g2 = (float)((c2 >> 16) & 0xFF);
  float b2 = (float)((c2 >> 8) & 0xFF);
  float a2 = (float)(c2 & 0xFF);

  uint32_t r_final = (uint32_t)(r1 + fator * (r2 - r1) + 0.5f);
  uint32_t g_final = (uint32_t)(g1 + fator * (g2 - g1) + 0.5f);
  uint32_t b_final = (uint32_t)(b1 + fator * (b2 - b1) + 0.5f);
  uint32_t a_final = (uint32_t)(a1 + fator * (a2 - a1) + 0.5f);

  return (r_final << 24) | (g_final << 16) | (b_final << 8) | a_final;
}

Cor calcular_iluminacao_difusa(Vetor3D normal, Vetor3D direção_luz, Cor cor_objeto, Cor cor_luz) {
  Vetor3D n = normalizar_vetor(normal);
  Vetor3D l = normalizar_vetor(direção_luz);
  float cosseno_fator = produto_escalar(n, l);
  if (cosseno_fator < 0.0f) cosseno_fator = 0.0f;

  float obj_r = (float)((cor_objeto >> 24) & 0xFF);
  float obj_g = (float)((cor_objeto >> 16) & 0xFF);
  float obj_b = (float)((cor_objeto >> 8) & 0xFF);
  float obj_a = (float)(cor_objeto & 0xFF);

  float luz_r = (float)((cor_luz >> 24) & 0xFF);
  float luz_g = (float)((cor_luz >> 16) & 0xFF);
  float luz_b = (float)((cor_luz >> 8) & 0xFF);
  float luz_a = (float)(cor_luz & 0xFF);

  float fator = cosseno_fator / 255.0f;
  float res_r = obj_r * luz_r * fator;
  float res_g = obj_g * luz_g * fator;
  float res_b = obj_b * luz_b * fator;
  float res_a = (obj_a * luz_a) / 255.0f;

  if (res_r > 255.0f) res_r = 255.0f;
  if (res_g > 255.0f) res_g = 255.0f;
  if (res_b > 255.0f) res_b = 255.0f;
  if (res_a > 255.0f) res_a = 255.0f;

  return ((Cor)(uint8_t)res_r << 24) | ((Cor)(uint8_t)res_g << 16) | ((Cor)(uint8_t)res_b << 8) | (Cor)(uint8_t)res_a;
}

Cor calcular_iluminacao_especular(Vetor3D normal, Vetor3D direção_luz, Vetor3D direção_olhar, int polimento) {
  Vetor3D n = normalizar_vetor(normal);
  Vetor3D l = normalizar_vetor(direção_luz);
  Vetor3D v = normalizar_vetor(direção_olhar);
  Vetor3D h = normalizar_vetor(somar_vetores(l, v));

  float dot_nh = produto_escalar(n, h);
  if (dot_nh < 0.0f) dot_nh = 0.0f;
  if (produto_escalar(n, l) <= 0.0f) dot_nh = 0.0f;

  float intensidade = potencia_inteira(dot_nh, polimento);
  float res_cor = intensidade * 255.0f;
  if (res_cor > 255.0f) res_cor = 255.0f;

  uint8_t especular = (uint8_t)res_cor;
  return ((Cor)especular << 24) | ((Cor)especular << 16) | ((Cor)especular << 8) | (Cor)255;
}

Cor calcular_iluminacao_phong_completa(Vetor3D normal, Vetor3D direção_luz, Vetor3D direção_olhar, Cor cor_objeto, Cor cor_difusa_luz, Cor cor_especular_luz, int polimento, float intensidade_ambiente) {
  Vetor3D n = normalizar_vetor(normal);
  Vetor3D l = normalizar_vetor(direção_luz);
  Vetor3D v = normalizar_vetor(direção_olhar);

  // Extração dos canais do objeto
  float obj_r = (float)((cor_objeto >> 24) & 0xFF);
  float obj_g = (float)((cor_objeto >> 16) & 0xFF);
  float obj_b = (float)((cor_objeto >> 8) & 0xFF);
  float obj_a = (float)(cor_objeto & 0xFF);

  // Extração dos canais da luz difusa
  float dif_r = (float)((cor_difusa_luz >> 24) & 0xFF);
  float dif_g = (float)((cor_difusa_luz >> 16) & 0xFF);
  float dif_b = (float)((cor_difusa_luz >> 8) & 0xFF);

  // Extração dos canais da luz especular
  float esp_r = (float)((cor_especular_luz >> 24) & 0xFF);
  float esp_g = (float)((cor_especular_luz >> 16) & 0xFF);
  float esp_b = (float)((cor_especular_luz >> 8) & 0xFF);

  // 1. Componente Ambiente
  float amb_r = obj_r * intensidade_ambiente;
  float amb_g = obj_g * intensidade_ambiente;
  float amb_b = obj_b * intensidade_ambiente;

  // 2. Componente Difusa
  float cosseno_difuso = produto_escalar(n, l);
  if (cosseno_difuso < 0.0f) cosseno_difuso = 0.0f; // Caso a luz venha de trás

  float res_dif_r = (obj_r * dif_r * (cosseno_difuso / 255.0f));
  float res_dif_g = (obj_g * dif_g * (cosseno_difuso / 255.0f));
  float res_dif_b = (obj_b * dif_b * (cosseno_difuso / 255.0f));

  // 3. Componente Especular (Phong clássico: reflexão da luz vs olhar)
  float res_esp_r = 0.0f;
  float res_esp_g = 0.0f;
  float res_esp_b = 0.0f;

  if (cosseno_difuso > 0.0f) {
    Vetor3D r_vetor = refletir_vetor((Vetor3D){-l.x, -l.y, -l.z}, n);
    float cosseno_especular = produto_escalar(r_vetor, v);
    if (cosseno_especular < 0.0f) cosseno_especular = 0.0f;

    float intensidade_especular = potencia_inteira(cosseno_especular, polimento);
    res_esp_r = esp_r * intensidade_especular;
    res_esp_g = esp_g * intensidade_especular;
    res_esp_b = esp_b * intensidade_especular;
  }

  // Acumulação final com teto de segurança
  float final_r = amb_r + res_dif_r + res_esp_r;
  float final_g = amb_g + res_dif_g + res_esp_g;
  float final_b = amb_b + res_dif_b + res_esp_b;

  if (final_r > 255.0f) final_r = 255.0f;
  if (final_g > 255.0f) final_g = 255.0f;
  if (final_b > 255.0f) final_b = 255.0f;

  return ((Cor)(uint8_t)final_r << 24) |
         ((Cor)(uint8_t)final_g << 16) |
         ((Cor)(uint8_t)final_b << 8)  |
         (Cor)(uint8_t)obj_a;
}

Cor amostrar_textura(const Tela* const textura, const float u, const float v) {
  if (textura == nullptr || textura->largura <= 0 || textura->altura <= 0 || textura->píxeis == nullptr) {
    return 0;
  }
  const float módulo_u_1 = módulo(u, 1);
  const float módulo_v_1 = módulo(1 - v, 1);
  const float u_repetente = módulo_u_1 >= 0 ? módulo_u_1 : módulo_u_1 + 1;
  const float v_repetente = módulo_v_1 >= 0 ? módulo_v_1 : módulo_v_1 + 1;
  const unsigned x = u_repetente * textura->largura;
  const unsigned y = v_repetente * textura->altura;
  const unsigned x_seguro = x < textura->largura ? x : textura->largura - 1;
  const unsigned y_seguro = y < textura->altura ? y : textura->altura - 1;
  return textura->píxeis[x_seguro + y_seguro * textura->largura];
}

Tela criar_tela(int largura, int altura) {
  Tela tela = {};
  if (largura <= 0 || altura <= 0) return tela;
  Cor* memoria_píxeis = malloc((size_t)largura * (size_t)altura * sizeof(Cor));
  if (memoria_píxeis == nullptr) return tela;
  tela.largura = largura;
  tela.altura = altura;
  tela.píxeis = memoria_píxeis;
  return tela;
}

void destruir_tela(Tela* tela) {
  if (tela == nullptr) return;
  free(tela->píxeis);
  tela->largura = 0;
  tela->altura = 0;
  tela->píxeis = nullptr;
}

void limpar_tela(Tela* tela, Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  int area = tela->largura * tela->altura;
  for (int i = 0; i < area; ++i) {
    tela->píxeis[i] = cor;
  }
}

void desenhar_píxel(Tela* tela, unsigned x, unsigned y, Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  if (tela->largura <= 0 || tela->altura <= 0) return;
  if (x >= tela->largura || y >= tela->altura) return;
  tela->píxeis[x + y * tela->largura] = cor;
}

void limpar_profundidade(Profundidade* pf, float valor) {
  if (pf == NULL || pf->profundidades == NULL || pf->largura <= 0 || pf->altura <= 0) return;
  size_t total_píxeis = (size_t)pf->largura * (size_t)pf->altura;
  for (size_t i = 0; i < total_píxeis; ++i) {
    pf->profundidades[i] = valor;
  }
}

bool testar_e_definir_profundidade(Profundidade* pf, int x, int y, float z) {
  if (pf == nullptr || pf->profundidades == nullptr) return false;
  if (x < 0 || x >= pf->largura || y < 0 || y >= pf->altura) return false;
  size_t índice = (size_t)y * (size_t)pf->largura + (size_t)x;
  if (z < pf->profundidades[índice]) {
    pf->profundidades[índice] = z;
    return true;
  }
  return false;
}

void desenhar_linha(Tela* tela, Vetor2D p1, Vetor2D p2, Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura <= 0 || tela->altura <= 0) return;
  int x0 = (int)p1.x; int y0 = (int)p1.y;
  int x1 = (int)p2.x; int y1 = (int)p2.y;
  int dx = (int)absoluto((float)(x1 - x0));
  int dy = (int)absoluto((float)(y1 - y0));
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int erro = dx - dy;
  int largura = tela->largura;
  int altura = tela->altura;
  while (true) {
    if (x0 >= 0 && x0 < largura && y0 >= 0 && y0 < altura) {
      tela->píxeis[y0 * largura + x0] = cor;
    }
    if (x0 == x1 && y0 == y1) break;
    int erro2 = 2 * erro;
    if (erro2 > -dy) { erro -= dy; x0 += sx; }
    if (erro2 < dx) { erro += dx; y0 += sy; }
  }
}

void desenhar_triângulo(Tela* tela, Vetor2D p1, Vetor2D p2, Vetor2D p3, Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura <= 0 || tela->altura <= 0) return;
  int min_x = (int)minimo(minimo(p1.x, p2.x), p3.x);
  int max_x = (int)maximo(maximo(p1.x, p2.x), p3.x);
  int min_y = (int)minimo(minimo(p1.y, p2.y), p3.y);
  int max_y = (int)maximo(maximo(p1.y, p2.y), p3.y);
  if (min_x < 0) min_x = 0;
  if (min_y < 0) min_y = 0;
  if (max_x >= tela->largura) max_x = tela->largura - 1;
  if (max_y >= tela->altura) max_y = tela->altura - 1;

  float dy12 = p2.y - p1.y; float dx12 = p2.x - p1.x;
  float dy23 = p3.y - p2.y; float dx23 = p3.x - p2.x;
  float dy31 = p1.y - p3.y; float dx31 = p1.x - p3.x;
  float area_dupla = dx12 * dy23 - dy12 * dx23;
  if (area_dupla == 0.0f) return;

  float sinal = (area_dupla < 0.0f) ? -1.0f : 1.0f;
  int largura = tela->largura;
  for (int y = min_y; y <= max_y; ++y) {
    float py = (float)y + 0.5f;
    int offset_linha = y * largura;
    for (int x = min_x; x <= max_x; ++x) {
      float px = (float)x + 0.5f;
      float w0 = ((px - p2.x) * dy23 - (py - p2.y) * dx23) * sinal;
      float w1 = ((px - p3.x) * dy31 - (py - p3.y) * dx31) * sinal;
      float w2 = ((px - p1.x) * dy12 - (py - p1.y) * dx12) * sinal;
      if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) {
        tela->píxeis[offset_linha + x] = cor;
      }
    }
  }
}

Vetor3D calcular_coordenadas_baricentricas(Vetor2D p, Vetor2D a, Vetor2D b, Vetor2D c) {
  float det = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
  if (det > -0.000001f && det < 0.000001f) {
    return (Vetor3D){-1.0f, -1.0f, -1.0f};
  }
  float inv_det = 1.0f / det;
  float alpha = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) * inv_det;
  float beta = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) * inv_det;
  return (Vetor3D){alpha, beta, 1.0f - alpha - beta};
}

Vetor2D projetar_ponto(Vetor3D ponto_3d, int largura, int altura) {
  if (ponto_3d.z <= 0.1f) return (Vetor2D){0.0f, 0.0f};
  float fov_escala = (float)largura * 0.5f;
  float inv_z = 1.0f / ponto_3d.z;
  return (Vetor2D){
    .x = (ponto_3d.x * fov_escala) * inv_z + ((float)largura * 0.5f),
    .y = -(ponto_3d.y * fov_escala) * inv_z + ((float)altura * 0.5f)
  };
}

bool orientacao_traseira(Vetor3D v1, Vetor3D v2, Vetor3D v3, Vetor3D direção_câmara) {
  Vetor3D normal_face = calcular_normal(v1, v2, v3);
  return produto_escalar(normal_face, direção_câmara) >= 0.0f;
}

Vértice interpolar_vértices(Vértice a, Vértice b, float t) {
  return (Vértice){
    .posição = interpolar_vetores(a.posição, b.posição, t),
    .uv = { a.uv.x + t * (b.uv.x - a.uv.x), a.uv.y + t * (b.uv.y - a.uv.y) },
    .cor = interpolar_cores(a.cor, b.cor, t)
  };
}

int recortar_triângulo_plano_perto(Vértice* triângulo_in, Vértice* triângulos_out, float plano_perto) {
  Vértice vértices_dentro[3];
  Vértice vértices_fora[3];
  int contagem_dentro = 0;
  int contagem_fora = 0;
  
  for (int i = 0; i < 3; ++i) {
    if (triângulo_in[i].posição.z >= plano_perto) {
      vértices_dentro[contagem_dentro++] = triângulo_in[i];
    } else {
      vértices_fora[contagem_fora++] = triângulo_in[i];
    }
  }
  
  if (contagem_dentro == 0) return 0;
  
  if (contagem_dentro == 3) {
    triângulos_out[0] = triângulo_in[0];
    triângulos_out[1] = triângulo_in[1];
    triângulos_out[2] = triângulo_in[2];
    return 1;
  }
  
  if (contagem_dentro == 1) {
    triângulos_out[0] = vértices_dentro[0];
    float t1 = (plano_perto - vértices_dentro[0].posição.z) / (vértices_fora[0].posição.z - vértices_dentro[0].posição.z);
    triângulos_out[1] = interpolar_vértices(vértices_dentro[0], vértices_fora[0], t1);
    float t2 = (plano_perto - vértices_dentro[0].posição.z) / (vértices_fora[1].posição.z - vértices_dentro[0].posição.z);
    triângulos_out[2] = interpolar_vértices(vértices_dentro[0], vértices_fora[1], t2);
    return 1;
  }
  
  if (contagem_dentro == 2) {
    int idx_fora = 0;
    for (int i = 0; i < 3; ++i) {
      if (triângulo_in[i].posição.z < plano_perto) { 
        idx_fora = i; 
        break; 
      }
    }
    Vértice v0 = triângulo_in[idx_fora];
    Vértice v1 = triângulo_in[(idx_fora + 1) % 3];
    Vértice v2 = triângulo_in[(idx_fora + 2) % 3];

    float t1 = (plano_perto - v0.posição.z) / (v1.posição.z - v0.posição.z);
    Vértice i1 = interpolar_vértices(v0, v1, t1);
    float t2 = (plano_perto - v2.posição.z) / (v0.posição.z - v2.posição.z);
    Vértice i2 = interpolar_vértices(v2, v0, t2);

    triângulos_out[0] = i1;   triângulos_out[1] = v1;   triângulos_out[2] = v2;
    triângulos_out[3] = i1;   triângulos_out[4] = v2;   triângulos_out[5] = i2;
    return 2;
  }
  
  return 0;
}

void calcular_tangente_bitangente(Vértice* triângulo, Vetor3D* tangente, Vetor3D* bitangente) {
  Vetor3D delta_pos1 = subtrair_vetores(triângulo[1].posição, triângulo[0].posição);
  Vetor3D delta_pos2 = subtrair_vetores(triângulo[2].posição, triângulo[0].posição);

  float delta_u1 = triângulo[1].uv.x - triângulo[0].uv.x;
  float delta_v1 = triângulo[1].uv.y - triângulo[0].uv.y;
  float delta_u2 = triângulo[2].uv.x - triângulo[0].uv.x;
  float delta_v2 = triângulo[2].uv.y - triângulo[0].uv.y;

  float det = delta_u1 * delta_v2 - delta_u2 * delta_v1;
  float r = 1.0f;
  
  if (det > -0.000001f && det < 0.000001f) {
    r = 1.0f;
  } else {
    r = 1.0f / det;
  }

  if (tangente != nullptr) {
    tangente->x = r * (delta_v2 * delta_pos1.x - delta_v1 * delta_pos2.x);
    tangente->y = r * (delta_v2 * delta_pos1.y - delta_v1 * delta_pos2.y);
    tangente->z = r * (delta_v2 * delta_pos1.z - delta_v1 * delta_pos2.z);
    *tangente = normalizar_vetor(*tangente);
  }

  if (bitangente != nullptr) {
    bitangente->x = r * (-delta_u2 * delta_pos1.x + delta_u1 * delta_pos2.x);
    bitangente->y = r * (-delta_u2 * delta_pos1.y + delta_u1 * delta_pos2.y);
    bitangente->z = r * (-delta_u2 * delta_pos1.z + delta_u1 * delta_pos2.z);
    *bitangente = normalizar_vetor(*bitangente);
  }
}

void desenhar_triângulo_preenchido(Tela* tela, Profundidade* pf, Vértice v1, Vértice v2, Vértice v3, Tela* textura) {
  if (tela == nullptr || pf == nullptr) return;

  Vetor2D p1 = { v1.posição.x, v1.posição.y };
  Vetor2D p2 = { v2.posição.x, v2.posição.y };
  Vetor2D p3 = { v3.posição.x, v3.posição.y };

  int inicio_x = (int)minimo(minimo(p1.x, p2.x), p3.x);
  int fim_x = (int)maximo(maximo(p1.x, p2.x), p3.x) + 1;
  int inicio_y = (int)minimo(minimo(p1.y, p2.y), p3.y);
  int fim_y = (int)maximo(maximo(p1.y, p2.y), p3.y) + 1;

  if (inicio_x < 0) inicio_x = 0;
  if (fim_x >= tela->largura) fim_x = tela->largura - 1;
  if (inicio_y < 0) inicio_y = 0;
  if (fim_y >= tela->altura) fim_y = tela->altura - 1;

  float inv_w1 = 1.0f / v1.posição.z;
  float inv_w2 = 1.0f / v2.posição.z;
  float inv_w3 = 1.0f / v3.posição.z;

  float r1 = (float)((v1.cor >> 24) & 0xFF) * inv_w1;
  float g1 = (float)((v1.cor >> 16) & 0xFF) * inv_w1;
  float b1 = (float)((v1.cor >> 8) & 0xFF) * inv_w1;
  float a1 = (float)(v1.cor & 0xFF) * inv_w1;

  float r2 = (float)((v2.cor >> 24) & 0xFF) * inv_w2;
  float g2 = (float)((v2.cor >> 16) & 0xFF) * inv_w2;
  float b2 = (float)((v2.cor >> 8) & 0xFF) * inv_w2;
  float a2 = (float)(v2.cor & 0xFF) * inv_w2;

  float r3 = (float)((v3.cor >> 24) & 0xFF) * inv_w3;
  float g3 = (float)((v3.cor >> 16) & 0xFF) * inv_w3;
  float b3 = (float)((v3.cor >> 8) & 0xFF) * inv_w3;
  float a3 = (float)(v3.cor & 0xFF) * inv_w3;

  for (int y = inicio_y; y <= fim_y; ++y) {
    for (int x = inicio_x; x <= fim_x; ++x) {
      Vetor2D p = { (float)x + 0.5f, (float)y + 0.5f };
      Vetor3D baricentricas = calcular_coordenadas_baricentricas(p, p1, p2, p3);
      if (baricentricas.x >= 0.0f && baricentricas.y >= 0.0f && baricentricas.z >= 0.0f) {
        float interpolado_inv_z = baricentricas.x * inv_w1 + baricentricas.y * inv_w2 + baricentricas.z * inv_w3;
        float z_atual = 1.0f / interpolado_inv_z;
        if (testar_e_definir_profundidade(pf, x, y, z_atual)) {
          Cor cor_final;
          if (textura != nullptr && textura->píxeis != nullptr) {
            float u_correto = (baricentricas.x * v1.uv.x * inv_w1 + baricentricas.y * v2.uv.x * inv_w2 + baricentricas.z * v3.uv.x * inv_w3) * z_atual;
            float v_correto = (baricentricas.x * v1.uv.y * inv_w1 + baricentricas.y * v2.uv.y * inv_w2 + baricentricas.z * v3.uv.y * inv_w3) * z_atual;
            cor_final = amostrar_textura(textura, u_correto, v_correto);
          } else {
            float r_c = (baricentricas.x * r1 + baricentricas.y * r2 + baricentricas.z * r3) * z_atual;
            float g_c = (baricentricas.x * g1 + baricentricas.y * g2 + baricentricas.z * g3) * z_atual;
            float b_c = (baricentricas.x * b1 + baricentricas.y * b2 + baricentricas.z * b3) * z_atual;
            float a_c = (baricentricas.x * a1 + baricentricas.y * a2 + baricentricas.z * a3) * z_atual;
            cor_final = ((uint32_t)(r_c + 0.5f) << 24) | ((uint32_t)(g_c + 0.5f) << 16) | ((uint32_t)(b_c + 0.5f) << 8) | (uint32_t)(a_c + 0.5f);
          }
          desenhar_píxel(tela, x, y, cor_final);
        }
      }
    }
  }
}

Vetor3D amostrar_mapeamento_normal(const Tela* mapa_normais, float u, float v, Vetor3D normal_geometria, Vetor3D tangente, Vetor3D bitangente) {
  // If no normal map is available, fallback safely to the vertex geometry normal
  if (mapa_normais == nullptr || mapa_normais->píxeis == nullptr) {
    return normalizar_vetor(normal_geometria);
  }

  // 1. Sample the color from the normal map texture at the given UV coordinates
  Cor cor_amostrada = amostrar_textura(mapa_normais, u, v);

  // 2. Extract channels assuming standard RGBA/RGB byte ordering
  float r = (float)((cor_amostrada >> 24) & 0xFF);
  float g = (float)((cor_amostrada >> 16) & 0xFF);
  float b = (float)((cor_amostrada >> 8)  & 0xFF);

  // 3. Unpack RGB values from [0, 255] to tangent-space vector components in [-1.0, 1.0]
  Vetor3D normal_tangente;
  normal_tangente.x = (r / 255.0f) * 2.0f - 1.0f;
  normal_tangente.y = (g / 255.0f) * 2.0f - 1.0f;
  normal_tangente.z = (b / 255.0f) * 2.0f - 1.0f;

  // Ensure the sampled vector is unit-length
  normal_tangente = normalizar_vetor(normal_tangente);

  // Ensure the geometric basis vectors are properly normalized
  Vetor3D t = normalizar_vetor(tangente);
  Vetor3D b_vec = normalizar_vetor(bitangente);
  Vetor3D n = normalizar_vetor(normal_geometria);

  // 4. Transform to target coordinate space using the TBN basis matrix
  // Row-Major layout: Vector * Matrix multiplication where T, B, N are the matrix rows.
  Vetor3D normal_final;
  normal_final.x = normal_tangente.x * t.x + normal_tangente.y * b_vec.x + normal_tangente.z * n.x;
  normal_final.y = normal_tangente.x * t.y + normal_tangente.y * b_vec.y + normal_tangente.z * n.y;
  normal_final.z = normal_tangente.x * t.z + normal_tangente.y * b_vec.z + normal_tangente.z * n.z;

  return normalizar_vetor(normal_final);
}

bool verificar_sombra_bloqueada(Vetor3D ponto_origem, Vetor3D direção_luz, float distância_luz, const Vetor3D* esferas_centros, const float* esferas_raios, unsigned núm_esferas) {
  // Safe validation check for arrays and range boundaries
  if (esferas_centros == nullptr || esferas_raios == nullptr) {
    return false;
  }

  // Offset origin slightly along light ray vector direction to prevent self-shadowing acne
  Vetor3D origem_raio;
  float epsilon = 0.001f;
  origem_raio.x = ponto_origem.x + direção_luz.x * epsilon;
  origem_raio.y = ponto_origem.y + direção_luz.y * epsilon;
  origem_raio.z = ponto_origem.z + direção_luz.z * epsilon;

  // Loop through all spatial primitives inside the cluster array
  for (int i = 0; i < núm_esferas; ++i) {
    Vetor3D centro_esfera = esferas_centros[i];
    float raio_esfera = esferas_raios[i];

    // Analytical geometric check for ray-sphere contact
    Vetor3D l = subtrair_vetores(centro_esfera, origem_raio);
    float tca = produto_escalar(l, direção_luz);
    
    // If center projects behind ray origin, it cannot block the path forward
    if (tca < 0.0001f) {
      continue;
    }

    float d2 = produto_escalar(l, l) - tca * tca;
    float raio2 = raio_esfera * raio_esfera;
    
    // If the closest distance squared is larger than radius squared, ray misses sphere
    if (d2 > raio2) {
      continue;
    }

    float thc = raiz(raio2 - d2);
    float t0 = tca - thc;

    // Check if the intersection point is located between the origin and the light source
    if (t0 > 0.0001f && t0 < distância_luz) {
      return true; // Light visibility path is completely blocked
    }
  }

  return false; // The visibility lane is unoccluded
}

Raio gerar_raio_câmara(Vetor3D posição_câmara, float matriz_inv_proj[static 4][4], int x, int y, int largura, int altura) {
  Raio raio;
  raio.origem = posição_câmara;
  if (largura <= 0 || altura <= 0) {
    raio.direção = (Vetor3D){0.0f, 0.0f, 1.0f};
    return raio;
  }
  float ndc_x = (2.0f * ((float)x + 0.5f) / (float)largura) - 1.0f;
  float ndc_y = 1.0f - (2.0f * ((float)y + 0.5f) / (float)altura);
  Vetor3D ponto_clip = { ndc_x, ndc_y, 1.0f };
  Vetor3D direção_câmara = transformar_vetor(ponto_clip, matriz_inv_proj);
  raio.direção = normalizar_vetor(direção_câmara);
  return raio;
}

Interseção intersetar_esfera(Raio raio, Vetor3D centro, float raio_esfera, Cor cor) {
  Interseção resultado = { .intersetou = false, .t = -1.0f, .ponto = {0.0f, 0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f}, .cor_material = cor };
  Vetor3D l = subtrair_vetores(centro, raio.origem);
  float tca = produto_escalar(l, raio.direção);
  float d2 = produto_escalar(l, l) - tca * tca;
  float raio2 = raio_esfera * raio_esfera;
  if (d2 > raio2) return resultado;

  float thc = raiz(raio2 - d2);
  float t0 = tca - thc;
  float t1 = tca + thc;
  float t_escolhido = -1.0f;
  if (t0 > 0.0001f) {
    t_escolhido = t0;
  } else if (t1 > 0.0001f) {
    t_escolhido = t1;
  } else {
    return resultado;
  }
  resultado.intersetou = true;
  resultado.t = t_escolhido;
  resultado.ponto = somar_vetores(raio.origem, (Vetor3D){raio.direção.x * t_escolhido, raio.direção.y * t_escolhido, raio.direção.z * t_escolhido});
  resultado.normal = normalizar_vetor(subtrair_vetores(resultado.ponto, centro));
  return resultado;
}

Interseção intersetar_plano(Raio raio, Vetor3D ponto_plano, Vetor3D normal_plano, Cor cor) {
  Interseção resultado = { .intersetou = false, .t = -1.0f, .ponto = {0.0f, 0.0f, 0.0f}, .normal = normal_plano, .cor_material = cor };
  float denominador = produto_escalar(raio.direção, normal_plano);
  if (denominador > -0.0001f && denominador < 0.0001f) return resultado;

  float t_calculado = produto_escalar(subtrair_vetores(ponto_plano, raio.origem), normal_plano) / denominador;
  if (t_calculado < 0.0001f) return resultado;

  resultado.intersetou = true;
  resultado.t = t_calculado;
  if (denominador > 0.0f) {
    resultado.normal = (Vetor3D){-normal_plano.x, -normal_plano.y, -normal_plano.z};
  }
  resultado.ponto = somar_vetores(raio.origem, (Vetor3D){raio.direção.x * t_calculado, raio.direção.y * t_calculado, raio.direção.z * t_calculado});
  return resultado;
}

Interseção intersetar_caixa_alinhada(Raio raio, Vetor3D min_caixa, Vetor3D max_caixa, Cor cor) {
  Interseção resultado = {
    .intersetou = false,
    .t = -1.0f,
    .ponto = {0.0f, 0.0f, 0.0f},
    .normal = {0.0f, 0.0f, 1.0f},
    .cor_material = cor
  };

  float t_min = -1e30f; // Approximated negative infinity
  float t_max =  1e30f; // Approximated positive infinity

  // --- X-Axis Slab Check ---
  if (raio.direção.x != 0.0f) {
    float inv_dx = 1.0f / raio.direção.x;
    float t1 = (min_caixa.x - raio.origem.x) * inv_dx;
    float t2 = (max_caixa.x - raio.origem.x) * inv_dx;
    
    float t_corte_min = minimo(t1, t2);
    float t_corte_max = maximo(t1, t2);
    
    t_min = maximo(t_min, t_corte_min);
    t_max = minimo(t_max, t_corte_max);
  } else if (raio.origem.x < min_caixa.x || raio.origem.x > max_caixa.x) {
    return resultado; // Parallel and outside the box boundaries
  }

  // --- Y-Axis Slab Check ---
  if (raio.direção.y != 0.0f) {
    float inv_dy = 1.0f / raio.direção.y;
    float t1 = (min_caixa.y - raio.origem.y) * inv_dy;
    float t2 = (max_caixa.y - raio.origem.y) * inv_dy;
    
    float t_corte_min = minimo(t1, t2);
    float t_corte_max = maximo(t1, t2);
    
    t_min = maximo(t_min, t_corte_min);
    t_max = minimo(t_max, t_corte_max);
  } else if (raio.origem.y < min_caixa.y || raio.origem.y > max_caixa.y) {
    return resultado;
  }

  // --- Z-Axis Slab Check ---
  if (raio.direção.z != 0.0f) {
    float inv_dz = 1.0f / raio.direção.z;
    float t1 = (min_caixa.z - raio.origem.z) * inv_dz;
    float t2 = (max_caixa.z - raio.origem.z) * inv_dz;
    
    float t_corte_min = minimo(t1, t2);
    float t_corte_max = maximo(t1, t2);
    
    t_min = maximo(t_min, t_corte_min);
    t_max = minimo(t_max, t_corte_max);
  } else if (raio.origem.z < min_caixa.z || raio.origem.z > max_caixa.z) {
    return resultado;
  }

  // If the slab intersection intervals do not overlap or the box is behind the ray
  if (t_min > t_max || t_max < 0.0001f) {
    return resultado;
  }

  // Determine the final intersection distance parameter t
  float t_escolhido = (t_min < 0.0001f) ? t_max : t_min;

  resultado.intersetou = true;
  resultado.t = t_escolhido;

  // Compute precise 3D point of contact on the box plane
  resultado.ponto.x = raio.origem.x + raio.direção.x * t_escolhido;
  resultado.ponto.y = raio.origem.y + raio.direção.y * t_escolhido;
  resultado.ponto.z = raio.origem.z + raio.direção.z * t_escolhido;

  // Derive structural surface normals dynamically depending on which slab plane was hit
  float epsilon = 0.0005f;
  if (resultado.ponto.x < min_caixa.x + epsilon)      resultado.normal = (Vetor3D){-1.0f,  0.0f,  0.0f};
  else if (resultado.ponto.x > max_caixa.x - epsilon) resultado.normal = (Vetor3D){ 1.0f,  0.0f,  0.0f};
  else if (resultado.ponto.y < min_caixa.y + epsilon) resultado.normal = (Vetor3D){ 0.0f, -1.0f,  0.0f};
  else if (resultado.ponto.y > max_caixa.y - epsilon) resultado.normal = (Vetor3D){ 0.0f,  1.0f,  0.0f};
  else if (resultado.ponto.z < min_caixa.z + epsilon) resultado.normal = (Vetor3D){ 0.0f,  0.0f, -1.0f};
  else                                                resultado.normal = (Vetor3D){ 0.0f,  0.0f,  1.0f};

  return resultado;
}

Interseção intersetar_triângulo(Raio raio, Vetor3D v1, Vetor3D v2, Vetor3D v3, Cor cor) {
  Interseção resultado = {
    .intersetou = false,
    .t = -1.0f,
    .ponto = {0.0f, 0.0f, 0.0f},
    .normal = {0.0f, 0.0f, 1.0f},
    .cor_material = cor
  };

  // 1. Calculate edge vectors of the triangle
  Vetor3D aresta1 = subtrair_vetores(v2, v1);
  Vetor3D aresta2 = subtrair_vetores(v3, v1);

  // 2. Prepare determinants to calculate barycentric coordinates
  Vetor3D p_vetor = produto_vetorial(raio.direção, aresta2);
  float det = produto_escalar(aresta1, p_vetor);

  // If determinant is close to zero, ray is parallel to the triangle plane
  if (det > -0.0001f && det < 0.0001f) {
    return resultado;
  }
  float inv_det = 1.0f / det;

  // 3. Compute U barycentric coordinate and validate bounds
  Vetor3D t_vetor = subtrair_vetores(raio.origem, v1);
  float u = produto_escalar(t_vetor, p_vetor) * inv_det;
  if (u < 0.0f || u > 1.0f) {
    return resultado;
  }

  // 4. Compute V barycentric coordinate and validate bounds
  Vetor3D q_vetor = produto_vetorial(t_vetor, aresta1);
  float v = produto_escalar(raio.direção, q_vetor) * inv_det;
  if (v < 0.0f || (u + v) > 1.0f) {
    return resultado;
  }

  // 5. Compute ray parameter distance t
  float t_calculado = produto_escalar(aresta2, q_vetor) * inv_det;
  if (t_calculado < 0.0001f) {
    return resultado; // Intersection behind the ray origin
  }

  // 6. Complete intersection logging
  resultado.intersetou = true;
  resultado.t = t_calculado;
  
  // Compute hit point: origin + direction * t
  resultado.ponto.x = raio.origem.x + raio.direção.x * t_calculado;
  resultado.ponto.y = raio.origem.y + raio.direção.y * t_calculado;
  resultado.ponto.z = raio.origem.z + raio.direção.z * t_calculado;

  // Calculate face normal and ensure it opposes the ray trajectory
  Vetor3D normal_face = calcular_normal(v1, v2, v3);
  if (produto_escalar(raio.direção, normal_face) > 0.0f) {
    resultado.normal = (Vetor3D){-normal_face.x, -normal_face.y, -normal_face.z};
  } else {
    resultado.normal = normal_face;
  }

  return resultado;
}

int main(void) {
  float identidade[4][4];
  criar_matriz_identidade(identidade);
  Vetor3D ponto = {1.0f, 2.0f, 3.0f};
  Vetor3D transformado = transformar_vetor(ponto, identidade);
  printf("Pipeline C23 Row-Major Unificado e Funcional!\n");
  printf("Teste de Ponto Transformado: (%.1f, %.1f, %.1f)\n", transformado.x, transformado.y, transformado.z);
  return 0;
}

float interpolar_linear(float a, float b, float t) {
  return a + t * (b - a);
}

void aplicar_filtro_gamma(Tela* tela, float gama) {
  if (tela == nullptr || tela->píxeis == nullptr || gama <= 0.0f) {
    return;
  }

  float inverso_gama = 1.0f / gama;
  int total_píxeis = tela->largura * tela->altura;

  for (int i = 0; i < total_píxeis; i++) {
    Cor cor_atual = tela->píxeis[i];

    // Extração dos canais assumindo o formato RGBA de 32 bits
    float r = (float)((cor_atual >> 24) & 0xFF) / 255.0f;
    float g = (float)((cor_atual >> 16) & 0xFF) / 255.0f;
    float b = (float)((cor_atual >> 8)  & 0xFF) / 255.0f;
    unsigned int a = cor_atual & 0xFF;

    // Aplicação da correção de gama exponencial
    float r_corrigido = potencia_real(r, inverso_gama) * 255.0f;
    float g_corrigido = potencia_real(g, inverso_gama) * 255.0f;
    float b_corrigido = potencia_real(b, inverso_gama) * 255.0f;

    // Teto de segurança para os limites de precisão de vírgula flutuante
    if (r_corrigido > 255.0f) r_corrigido = 255.0f;
    if (g_corrigido > 255.0f) g_corrigido = 255.0f;
    if (b_corrigido > 255.0f) b_corrigido = 255.0f;
    if (r_corrigido < 0.0f) r_corrigido = 0.0f;
    if (g_corrigido < 0.0f) g_corrigido = 0.0f;
    if (b_corrigido < 0.0f) b_corrigido = 0.0f;

    // Reconstrução da cor final no formato RGBA
    tela->píxeis[i] = ((Cor)(unsigned char)r_corrigido << 24) |
                      ((Cor)(unsigned char)g_corrigido << 16) |
                      ((Cor)(unsigned char)b_corrigido << 8)  |
                      (Cor)a;
  }
}

float logaritmo_natural(float x) {
  if (x <= 0.0f) {
    return 0.0f;
  }

  float n_log = 0.0f;

  // Redução de gama para aproximar o valor ao intervalo ideal perto de 1.0f
  while (x > 1.41421356f) {
    x *= 0.5f;
    n_log += 0.69314718f;
  }
  while (x < 0.70710678f) {
    x *= 2.0f;
    n_log -= 0.69314718f;
  }

  // Série polinomial para ln(x) no intervalo reduzido
  float num = x - 1.0f;
  float den = x + 1.0f;
  float fracao = num / den;
  float fracao_quadrada = fracao * fracao;
  
  float termo_ln = fracao;
  float soma_ln = fracao;
  int i_ln = 3;

  for (int j = 0; j < 4; j++) {
    termo_ln *= fracao_quadrada;
    soma_ln += termo_ln / (float)i_ln;
    i_ln += 2;
  }

  return n_log + (2.0f * soma_ln);
}

float potencia_real(float base, float expoente) {
  if (base == 0.0f) {
    if (expoente > 0.0f) {
      return 0.0f;
    }
    return 1.0f; 
  }

  if (base < 0.0f) {
    return 0.0f;
  }

  if (expoente == 0.0f) {
    return 1.0f;
  }

  float ln_final = logaritmo_natural(base);
  float valor_exp = expoente * ln_final;
  
  // Redução de gama para exp(f) aproximado a 0
  int k_exp = (int)(valor_exp * 1.44269504f);
  float f_exp = valor_exp - ((float)k_exp * 0.69314718f);

  float termo_exp = 1.0f;
  float soma_exp = 1.0f;

  // Expansão de Taylor para e^f_exp
  for (int i = 1; i < 10; i++) {
    termo_exp *= f_exp / (float)i;
    soma_exp += termo_exp;
  }

  // Aplicação do expoente binário remanescente
  if (k_exp > 0) {
    for (int i = 0; i < k_exp; i++) {
      soma_exp *= 2.0f;
    }
  } else if (k_exp < 0) {
    for (int i = 0; i < -k_exp; i++) {
      soma_exp *= 0.5f;
    }
  }

  return soma_exp;
}

void aplicar_mapeamento_de_tom_reinhard(Tela* tela) {
  if (tela == nullptr || tela->píxeis == nullptr) {
    return;
  }

  int total_píxeis = tela->largura * tela->altura;

  for (int i = 0; i < total_píxeis; i++) {
    Cor cor_atual = tela->píxeis[i];

    // Extração dos canais normalizados [0.0, 1.0] assumindo o formato RGBA de 32 bits
    float r = (float)((cor_atual >> 24) & 0xFF) / 255.0f;
    float g = (float)((cor_atual >> 16) & 0xFF) / 255.0f;
    float b = (float)((cor_atual >> 8)  & 0xFF) / 255.0f;
    unsigned int a = cor_atual & 0xFF;

    // Aplicação do operador de Reinhard independente por canal
    float r_mapeado = r / (1.0f + r);
    float g_mapeado = g / (1.0f + g);
    float b_mapeado = b / (1.0f + b);

    // Conversão de volta para a escala inteira [0, 255]
    float r_final = r_mapeado * 255.0f;
    float g_final = g_mapeado * 255.0f;
    float b_final = b_mapeado * 255.0f;

    // Salvaguardas estruturais para limites numéricos
    if (r_final > 255.0f) r_final = 255.0f;
    if (g_final > 255.0f) g_final = 255.0f;
    if (b_final > 255.0f) b_final = 255.0f;
    if (r_final < 0.0f) r_final = 0.0f;
    if (g_final < 0.0f) g_final = 0.0f;
    if (b_final < 0.0f) b_final = 0.0f;

    // Recomposição do píxel no espaço binário RGBA
    tela->píxeis[i] = ((Cor)(unsigned char)r_final << 24) |
                      ((Cor)(unsigned char)g_final << 16) |
                      ((Cor)(unsigned char)b_final << 8)  |
                      (Cor)a;
  }
}

bool guardar_tela_tga(const Tela* tela, const char* caminho_ficheiro) {
  if (tela == nullptr || tela->píxeis == nullptr || caminho_ficheiro == nullptr) {
    return false;
  }

  FILE* ficheiro = fopen(caminho_ficheiro, "wb");
  if (ficheiro == nullptr) {
    return false;
  }

  // Cabeçalho estrutural do formato TGA (18 bytes)
  unsigned char cabecalho[18];
  for (int i = 0; i < 18; i++) {
    cabecalho[i] = 0;
  }
  
  cabecalho[2] = 2; // Tipo de imagem: True-color não comprimida
  
  // Largura (Bytes 12 e 13)
  cabecalho[12] = (unsigned char)(tela->largura & 0xFF);
  cabecalho[13] = (unsigned char)((tela->largura >> 8) & 0xFF);
  
  // Altura (Bytes 14 e 15)
  cabecalho[14] = (unsigned char)(tela->altura & 0xFF);
  cabecalho[15] = (unsigned char)((tela->altura >> 8) & 0xFF);
  
  cabecalho[16] = 32; // Profundidade de cor: 32 bits por píxel (BGRA)
  cabecalho[17] = 8;  // Atributos do descritor: 8 bits de canal alfa

  // Escrita do cabeçalho no disco
  unsigned long itens_escritos = fwrite(cabecalho, 1, 18, ficheiro);
  if (itens_escritos != 18) {
    fclose(ficheiro);
    return false;
  }

  // Conversão de RGBA (Top-Down) para BGRA (Bottom-Up) e escrita linha a linha
  for (int y = tela->altura - 1; y >= 0; y--) {
    for (int x = 0; x < tela->largura; x++) {
      int índice_origem = y * tela->largura + x;
      Cor píxel_rgba = tela->píxeis[índice_origem];

      // Extração analítica dos componentes RGBA
      unsigned char r = (unsigned char)((píxel_rgba >> 24) & 0xFF);
      unsigned char g = (unsigned char)((píxel_rgba >> 16) & 0xFF);
      unsigned char b = (unsigned char)((píxel_rgba >> 8)  & 0xFF);
      unsigned char a = (unsigned char)(píxel_rgba & 0xFF);

      // Reorganização física para o formato binário BGRA do TGA
      unsigned char píxel_bgra[4];
      píxel_bgra[0] = b;
      píxel_bgra[1] = g;
      píxel_bgra[2] = r;
      píxel_bgra[3] = a;

      unsigned long bytes_píxel = fwrite(píxel_bgra, 1, 4, ficheiro);
      if (bytes_píxel != 4) {
        fclose(ficheiro);
        return false;
      }
    }
  }

  fclose(ficheiro);
  return true;
}

bool guardar_tela_ppm(const Tela* tela, const char* caminho_ficheiro) {
  if (tela == nullptr || tela->píxeis == nullptr || caminho_ficheiro == nullptr) {
    return false;
  }

  FILE* ficheiro = fopen(caminho_ficheiro, "wb");
  if (ficheiro == nullptr) {
    return false;
  }

  // Escrita do cabeçalho PPM (Formato P6, Largura, Altura, Valor Máximo de Cor)
  // O formato PPM é lido de cima para baixo (Top-Down), igual à tua estrutura de Tela.
  int caracteres_escritos = fprintf(ficheiro, "P6\n%d %d\n255\n", tela->largura, tela->altura);
  if (caracteres_escritos < 0) {
    fclose(ficheiro);
    return false;
  }

  int total_píxeis = tela->largura * tela->altura;

  for (int i = 0; i < total_píxeis; i++) {
    Cor píxel_rgba = tela->píxeis[i];

    // Extração analítica dos componentes de cor a partir do formato RGBA de 32 bits
    unsigned char componentes;
    componentes = (unsigned char)((píxel_rgba >> 24) & 0xFF); // R
    componentes = (unsigned char)((píxel_rgba >> 16) & 0xFF); // G
    componentes = (unsigned char)((píxel_rgba >> 8)  & 0xFF); // B

    // Escrita direta do bloco RGB de 3 bytes no ficheiro
    unsigned long bytes_escritos = fwrite(&componentes, 1, 3, ficheiro);
    if (bytes_escritos != 3) {
      fclose(ficheiro);
      return false;
    }
  }

  fclose(ficheiro);
  return true;
}

float calcular_atenuacao_luz(float distância, float constante, float linear, float quadratica) {
  float denominador = constante + (linear * distância) + (quadratica * distância * distância);
  
  if (denominador <= 0.0f) {
    return 1.0f;
  }
  
  return 1.0f / denominador;
}

Cor calcular_iluminacao_foco(Vetor3D ponto, Vetor3D normal, Vetor3D pos_luz, Vetor3D dir_foco, float corte_interno, float corte_externo) {
  // Calcular o vetor do ponto até à luz
  Vetor3D direção_para_luz = subtrair_vetores(pos_luz, ponto);
  direção_para_luz = normalizar_vetor(direção_para_luz);
  
  // Normalizar a direção do foco da luz (garantia)
  Vetor3D dir_foco_normalizada = normalizar_vetor(dir_foco);
  
  // O vetor do raio luminoso que sai da luz vai na direção oposta a direção_para_luz
  Vetor3D direção_raio_luz = subtrair_vetores(ponto, pos_luz);
  direção_raio_luz = normalizar_vetor(direção_raio_luz);
  
  // Calcular o cosseno do ângulo entre o raio da luz e a direção principal do foco
  float cosseno_atual = produto_escalar(direção_raio_luz, dir_foco_normalizada);
  
  // No modelo row-major/padrão, os cortes recebidos já devem ser os cossenos dos ângulos.
  // Se forem ângulos em radianos, seriam calculados previamente com cosseno(angulo).
  // Assumindo que corte_interno e corte_externo são os cossenos:
  float diferenca = corte_interno - corte_externo;
  
  // Evitar divisão por zero caso os ângulos sejam idênticos
  if (diferenca <= 0.0001f) {
    if (cosseno_atual >= corte_interno) {
      // Fora do cone de transição, totalmente iluminado pelo foco
      float n_dot_l = maximo(0.0f, produto_escalar(normal, direção_para_luz));
      return multiplicar_cor(0xFFFFFFFF, n_dot_l); // Retorna branco com intensidade difusa
    }
    return 0x00000000; // Totalmente escuro fora do foco
  }
  
  // Calcular a intensidade da transição (fator de atenuação angular)
  float intensidade_foco = (cosseno_atual - corte_externo) / diferenca;
  intensidade_foco = minimo(maximo(intensidade_foco, 0.0f), 1.0f);
  
  // Calcular o fator Lambertiano (difuso básico)
  float fator_difuso = maximo(0.0f, produto_escalar(normal, direção_para_luz));
  
  // Intensidade final combinando a orientação da superfície e o cone do foco
  float intensidade_final = fator_difuso * intensidade_foco;
  
  // Retorna a cor branca modulada pela intensidade (RGBA: 0xFFFFFFFF)
  return multiplicar_cor(0xFFFFFFFF, intensidade_final);
}

float calcular_fator_sombra_raster(Vetor3D ponto_espaco_luz, Profundidade* mapa_sombras, float bias) {
  // Se o mapa de sombras não existir, não há sombra projetada
  if (mapa_sombras == nullptr || mapa_sombras->profundidades == nullptr) {
    return 1.0f;
  }

  // Converter as coordenadas de textura [0.0, 1.0] para índices de píxeis inteiros
  int tex_x = (int)(ponto_espaco_luz.x * (float)mapa_sombras->largura);
  int tex_y = (int)(ponto_espaco_luz.y * (float)mapa_sombras->altura);

  // Se o ponto estiver fora dos limites do mapa de sombras, 
  // assumimos que está fora do cone/frustum da luz (portanto, iluminado)
  if (tex_x < 0 || tex_x >= mapa_sombras->largura || tex_y < 0 || tex_y >= mapa_sombras->altura) {
    return 1.0f;
  }

  // Obter a profundidade linearizada do fragmento atual
  float profundidade_atual = ponto_espaco_luz.z;

  // Se o fragmento estiver além do plano distante da luz, não recebe sombra projetada
  if (profundidade_atual > 1.0f) {
    return 1.0f;
  }

  // Mapeamento Row-Major para encontrar o índice no buffer de profundidade
  int índice = (tex_y * mapa_sombras->largura) + tex_x;
  float profundidade_armazenada = mapa_sombras->profundidades[índice];

  // Teste de sombra tradicional: se a profundidade atual com bias for maior
  // do que a profundidade mais próxima vista pela luz, o ponto está na sombra
  if (profundidade_atual - bias > profundidade_armazenada) {
    return 0.0f; // Totalmente na sombra
  }

  return 1.0f; // Totalmente iluminado
}

bool intersetar_caixa_alinhada_rapido(Raio raio, Vetor3D min_caixa, Vetor3D max_caixa, float* t_min_out) {
  float t_minimo = -1e30f; // Representação prática de infinito negativo
  float t_maximo =  1e30f; // Representação prática de infinito positivo

  // Processamento do Eixo X
  if (raio.direção.x != 0.0f) {
    float inverso_dir_x = 1.0f / raio.direção.x;
    float t1 = (min_caixa.x - raio.origem.x) * inverso_dir_x;
    float t2 = (max_caixa.x - raio.origem.x) * inverso_dir_x;

    t_minimo = maximo(t_minimo, minimo(t1, t2));
    t_maximo = minimo(t_maximo, maximo(t1, t2));
  } else if (raio.origem.x < min_caixa.x || raio.origem.x > max_caixa.x) {
    return false;
  }

  // Processamento do Eixo Y
  if (raio.direção.y != 0.0f) {
    float inverso_dir_y = 1.0f / raio.direção.y;
    float t1 = (min_caixa.y - raio.origem.y) * inverso_dir_y;
    float t2 = (max_caixa.y - raio.origem.y) * inverso_dir_y;

    t_minimo = maximo(t_minimo, minimo(t1, t2));
    t_maximo = minimo(t_maximo, maximo(t1, t2));
  } else if (raio.origem.y < min_caixa.y || raio.origem.y > max_caixa.y) {
    return false;
  }

  // Processamento do Eixo Z
  if (raio.direção.z != 0.0f) {
    float inverso_dir_z = 1.0f / raio.direção.z;
    float t1 = (min_caixa.z - raio.origem.z) * inverso_dir_z;
    float t2 = (max_caixa.z - raio.origem.z) * inverso_dir_z;

    t_minimo = maximo(t_minimo, minimo(t1, t2));
    t_maximo = minimo(t_maximo, maximo(t1, t2));
  } else if (raio.origem.z < min_caixa.z || raio.origem.z > max_caixa.z) {
    return false;
  }

  // Verificar se houve uma interseção válida no espaço-tempo do raio
  if (t_minimo > t_maximo || t_maximo < 0.0f) {
    return false;
  }

  // Se o raio começar dentro da caixa, o t_minimo será negativo.
  // Nesse caso, o primeiro ponto de interseção à frente do raio será em t_minimo = 0.0f ou t_maximo.
  // Para volumes envolventes de aceleração, queremos saber se intersecta à frente.
  if (t_minimo < 0.0f) {
    t_minimo = 0.0f; 
  }

  if (t_min_out != nullptr) {
    *t_min_out = t_minimo;
  }

  return true;
}

// Função auxiliar interna para computar o volume envolvente (AABB) de um conjunto de esferas
static void calcular_aabb_grupo(Vetor3D* centros, float* raios, int* índices, int inicio, int fim, Vetor3D* min_out, Vetor3D* max_out) {
  Vetor3D minimo_ = { .x = 1e30f, .y = 1e30f, .z = 1e30f };
  Vetor3D maximo_ = { .x = -1e30f, .y = -1e30f, .z = -1e30f };

  for (int i = inicio; i < fim; ++i) {
    int idx = índices[i];
    Vetor3D c = centros[idx];
    float r = raios[idx];

    minimo_.x = minimo(minimo_.x, c.x - r);
    minimo_.y = minimo(minimo_.y, c.y - r);
    minimo_.z = minimo(minimo_.z, c.z - r);

    maximo_.x = maximo(maximo_.x, c.x + r);
    maximo_.y = maximo(maximo_.y, c.y + r);
    maximo_.z = maximo(maximo_.z, c.z + r);
  }

  *min_out = minimo_;
  *max_out = maximo_;
}

// Função recursiva interna para construir os nós da árvore sequencialmente na memória alocada
static int construir_bvh_recursivo(NodoBVH* nodos, int* índices, Vetor3D* centros, float* raios, int inicio, int fim, unsigned* contador_nodos) {
  unsigned no_atual_idx = *contador_nodos;
  (*contador_nodos)++;

  NodoBVH* no = &nodos[no_atual_idx];
  
  // Calcular a caixa envolvente para os objetos deste nó
  calcular_aabb_grupo(centros, raios, índices, inicio, fim, &no->min_caixa, &no->max_caixa);

  int contagem = fim - inicio;

  // Condição de paragem: se tivermos 2 ou menos objetos, transformamos o nó numa folha
  if (contagem <= 2) {
    no->índice_esquerdo = índices[inicio]; // Armazena o índice do primeiro objeto no array original
    no->contagem_objetos = contagem;       // Indica que é um nó folha
    return no_atual_idx;
  }

  // Determinar o eixo mais longo da caixa envolvente para fazer a divisão (Split)
  Vetor3D extensao = subtrair_vetores(no->max_caixa, no->min_caixa);
  int eixo = 0;
  if (extensao.y > extensao.x) eixo = 1;
  if (extensao.z > (eixo == 0 ? extensao.x : extensao.y)) eixo = 2;

  // Dividir a meio com base na posição dos centros dos objetos ao longo do eixo escolhido
  float divisao = 0.5f * (
    (eixo == 0 ? no->min_caixa.x + no->max_caixa.x : 
    (eixo == 1 ? no->min_caixa.y + no->max_caixa.y : no->min_caixa.z + no->max_caixa.z))
  );

  // Reordenar os índices (particionamento rápido semelhante ao Quicksort)
  int i = inicio;
  int j = fim - 1;
  while (i <= j) {
    float pos_centro = 0.0f;
    if (eixo == 0) pos_centro = centros[índices[i]].x;
    else if (eixo == 1) pos_centro = centros[índices[i]].y;
    else pos_centro = centros[índices[i]].z;

    if (pos_centro < divisao) {
      i++;
    } else {
      int temporario = índices[i];
      índices[i] = índices[j];
      índices[j] = temporario;
      j--;
    }
  }

  // Se a divisão falhar e colocar todos os objetos de um único lado, força a divisão ao meio exato do array
  int meio = i;
  if (meio == inicio || meio == fim) {
    meio = inicio + contagem / 2;
  }

  // Construir os nós filhos recursivamente
  no->contagem_objetos = 0; // 0 indica que é um nó interno
  no->índice_esquerdo = construir_bvh_recursivo(nodos, índices, centros, raios, inicio, meio, contador_nodos);
  // O filho direito estará localizado implicitamente no nó gerado logo a seguir ao ramo esquerdo
  construir_bvh_recursivo(nodos, índices, centros, raios, meio, fim, contador_nodos);

  return no_atual_idx;
}

void construir_arvore_bvh(NodoBVH* nodos_bvh, Vetor3D* centros, float* raios, int núm_objetos) {
  if (nodos_bvh == nullptr || centros == nullptr || raios == nullptr || núm_objetos <= 0) {
    return;
  }

  // Alocar dinamicamente uma tabela temporária de índices para mapeamento e ordenação espacial
  int* índices = malloc(sizeof(int) * (size_t)núm_objetos);
  if (índices == nullptr) {
    return;
  }

  for (int i = 0; i < núm_objetos; ++i) {
    índices[i] = i;
  }

  unsigned contador_nodos = 0;
  NodoBVH* array_nodos = (NodoBVH*)nodos_bvh;

  // Iniciar a construção recursiva a partir da raiz (índice 0)
  construir_bvh_recursivo(array_nodos, índices, centros, raios, 0, núm_objetos, &contador_nodos);

  // Libertar a tabela temporária de índices
  free(índices);
}

void aplicar_filtro_blur_gaussiano(Tela* tela, float sigma) {
  if (tela == nullptr || tela->píxeis == nullptr || sigma <= 0.0f) {
    return;
  }

  // Determinar o raio do núcleo com base no desvio padrão (sigma)
  int raio = (int)truncar(sigma * 3.0f);
  if (raio < 1) raio = 1;
  if (raio > 15) raio = 15; // Limite prático para estabilidade de memória estática

  int tamanho_nucleo = (raio * 2) + 1;
  float nucleo[31]; // Suporta raio até 15
  float soma_pesos = 0.0f;

  // Calcular os pesos unidimensionais do núcleo Gaussiano
  float dois_sigma_quadrado = 2.0f * sigma * sigma;
  for (int i = -raio; i <= raio; ++i) {
    float x = (float)i;
    float peso = (1.0f / (raiz(PI * dois_sigma_quadrado))) * (potencia_real(2.71828182f, -(x * x) / dois_sigma_quadrado));
    nucleo[i + raio] = peso;
    soma_pesos += peso;
  }

  // Normalizar o núcleo para evitar que a imagem escureça ou clareie
  for (int i = 0; i < tamanho_nucleo; ++i) {
    nucleo[i] /= soma_pesos;
  }

  // Alocar uma tela temporária para armazenar a primeira passagem (Horizontal)
  Cor* buffer_temporario = malloc(sizeof(Cor) * (size_t)tela->largura * (size_t)tela->altura);
  if (buffer_temporario == nullptr) {
    return;
  }

  // Passagem 1: Desfoque Horizontal (Lê de tela->píxeis, escreve em buffer_temporario)
  for (int y = 0; y < tela->altura; ++y) {
    for (int x = 0; x < tela->largura; ++x) {
      float r_acumulado = 0.0f, g_acumulado = 0.0f, b_acumulado = 0.0f, a_acumulado = 0.0f;

      for (int k = -raio; k <= raio; ++k) {
        // Amostragem com fixação nos limites (Clamp) para as bordas da tela
        int vizinho_x = x + k;
        if (vizinho_x < 0) vizinho_x = 0;
        if (vizinho_x >= tela->largura) vizinho_x = tela->largura - 1;

        int índice_vizinho = (y * tela->largura) + vizinho_x;
        Cor cor_vizinho = tela->píxeis[índice_vizinho];
        float peso = nucleo[k + raio];

        r_acumulado += extrair_r(cor_vizinho) * peso;
        g_acumulado += extrair_g(cor_vizinho) * peso;
        b_acumulado += extrair_b(cor_vizinho) * peso;
        a_acumulado += extrair_a(cor_vizinho) * peso;
      }

      int índice_destino = (y * tela->largura) + x;
      buffer_temporario[índice_destino] = empacotar_rgba(r_acumulado, g_acumulado, b_acumulado, a_acumulado);
    }
  }

  // Passagem 2: Desfoque Vertical (Lê de buffer_temporario, escreve de volta em tela->píxeis)
  for (int y = 0; y < tela->altura; ++y) {
    for (int x = 0; x < tela->largura; ++x) {
      float r_acumulado = 0.0f, g_acumulado = 0.0f, b_acumulado = 0.0f, a_acumulado = 0.0f;

      for (int k = -raio; k <= raio; ++k) {
        // Amostragem com fixação nos limites (Clamp) para as bordas verticais
        int vizinho_y = y + k;
        if (vizinho_y < 0) vizinho_y = 0;
        if (vizinho_y >= tela->altura) vizinho_y = tela->altura - 1;

        int índice_vizinho = (vizinho_y * tela->largura) + x;
        Cor cor_vizinho = buffer_temporario[índice_vizinho];
        float peso = nucleo[k + raio];

        r_acumulado += extrair_r(cor_vizinho) * peso;
        g_acumulado += extrair_g(cor_vizinho) * peso;
        b_acumulado += extrair_b(cor_vizinho) * peso;
        a_acumulado += extrair_a(cor_vizinho) * peso;
      }

      int índice_destino = (y * tela->largura) + x;
      tela->píxeis[índice_destino] = empacotar_rgba(r_acumulado, g_acumulado, b_acumulado, a_acumulado);
    }
  }

  // Libertar a memória do buffer temporário alocado
  free(buffer_temporario);
}

// Funções auxiliares para manipulação dos canais RGBA de uma Cor (uint32_t)
uint8_t extrair_r(Cor c) { return c >> 24; }
uint8_t extrair_g(Cor c) { return c >> 16; }
uint8_t extrair_b(Cor c) { return c >>  8; }
uint8_t extrair_a(Cor c) { return c      ; }

Cor empacotar_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  return ((Cor)r << 24) | ((Cor)g << 16) | ((Cor)b << 8) | (Cor)a;
}

void aplicar_vinheta(Tela* tela, float intensidade, float raio) {
  if (tela == nullptr || tela->píxeis == nullptr || intensidade <= 0.0f || raio <= 0.0f) {
    return;
  }

  // Determinar o centro geométrico da tela
  float centro_x = (float)tela->largura * 0.5f;
  float centro_y = (float)tela->altura * 0.5f;

  for (int y = 0; y < tela->altura; ++y) {
    for (int x = 0; x < tela->largura; ++x) {
      // Calcular a distância do píxel atual ao centro da tela
      float dx = (float)x - centro_x;
      float dy = (float)y - centro_y;
      float dist = raiz((dx * dx) + (dy * dy));

      // Normalizar a distância com base no raio da vinheta
      float dist_normalizada = dist / raio;

      // Calcular o fator de atenuação (quanto maior a distância, menor o fator)
      // O decaimento usa uma curva suave para evitar transições abruptas
      float fator_escurecimento = 1.0f - (dist_normalizada * dist_normalizada * intensidade);
      fator_escurecimento = minimo(maximo(fator_escurecimento, 0.0f), 1.0f);

      // Obter o índice do píxel na estrutura Row-Major
      int índice = (y * tela->largura) + x;
      Cor cor_original = tela->píxeis[índice];

      // Extrair os canais de cor originais
      float r = extrair_r(cor_original);
      float g = extrair_g(cor_original);
      float b = extrair_b(cor_original);
      float a = extrair_a(cor_original);

      // Aplicar o efeito de vinheta apenas aos canais cromáticos (RGB)
      r *= fator_escurecimento;
      g *= fator_escurecimento;
      b *= fator_escurecimento;

      // Gravar o píxel processado de volta na tela
      tela->píxeis[índice] = empacotar_rgba(r, g, b, a);
    }
  }
}

Raio gerar_raio_câmara_profundidade_campo(Vetor3D posição, Vetor3D dir, float abertura, float distância_focal, int x, int y, int largura, int altura) {
  // 1. Configurar o sistema de coordenadas locais da câmara (Camera Frame)
  Vetor3D frente = normalizar_vetor(dir);
  Vetor3D cima_global = { .x = 0.0f, .y = 1.0f, .z = 0.0f };
  
  // Evitar singularidade se a câmara olhar exatamente para cima ou para baixo
  if (absoluto(frente.y) > 0.99f) {
    cima_global = (Vetor3D){ .x = 1.0f, .y = 0.0f, .z = 0.0f };
  }
  
  Vetor3D direita = normalizar_vetor(produto_vetorial(frente, cima_global));
  Vetor3D cima_local = produto_vetorial(direita, frente);

  // 2. Calcular as coordenadas normalizadas do ecrã [-1, 1]
  float proporcao = (float)largura / (float)altura;
  float fov = 0.785398f; // Assumindo um FOV padrão de 45 graus (PI / 4) em radianos
  float escala_tangente = tangente(fov * 0.5f);

  float px = (((float)x + 0.5f) / (float)largura * 2.0f - 1.0f) * proporcao * escala_tangente;
  float py = (1.0f - ((float)y + 0.5f) / (float)altura * 2.0f) * escala_tangente;

  // Direção primária calculada diretamente somando as componentes no espaço da câmara
  Vetor3D dir_raio_primario;
  dir_raio_primario.x = (direita.x * px) + (cima_local.x * py) + frente.x;
  dir_raio_primario.y = (direita.y * px) + (cima_local.y * py) + frente.y;
  dir_raio_primario.z = (direita.z * px) + (cima_local.z * py) + frente.z;
  dir_raio_primario = normalizar_vetor(dir_raio_primario);

  // 3. Determinar o ponto de focagem no plano focal
  float t_foco = distância_focal;
  Vetor3D ponto_foco = {
    .x = posição.x + dir_raio_primario.x * t_foco,
    .y = posição.y + dir_raio_primario.y * t_foco,
    .z = posição.z + dir_raio_primario.z * t_foco
  };

  // 4. Gerar amostragem pseudoaleatória determinista baseada no píxel para o disco da lente
  uint32_t s = (uint32_t)(x * 73856093 ^ y * 19349663);
  s = (s ^ 61) ^ (s >> 16);
  s *= 9;
  s = s ^ (s >> 4);
  s *= 0x27d4eb2d;
  s = s ^ (s >> 15);
  
  float r1 = (float)(s & 0xFFFF) / 65535.0f;
  float r2 = (float)((s >> 16) & 0xFFFF) / 65535.0f;

  // Mapeamento para um disco circular (Coordenadas Polares)
  float raio_lente = (abertura * 0.5f) * raiz(r1);
  float angulo = r2 * 2.0f * PI;

  float deslocamento_u = raio_lente * cosseno(angulo);
  float deslocamento_v = raio_lente * seno(angulo);

  // 5. Calcular a nova origem deslocada sobre a superfície da lente
  Vetor3D nova_origem = {
    .x = posição.x + (direita.x * deslocamento_u) + (cima_local.x * deslocamento_v),
    .y = posição.y + (direita.y * deslocamento_u) + (cima_local.y * deslocamento_v),
    .z = posição.z + (direita.z * deslocamento_u) + (cima_local.z * deslocamento_v)
  };

  // 6. A nova direção aponta da lente para o ponto de focagem
  Vetor3D nova_direção = normalizar_vetor(subtrair_vetores(ponto_foco, nova_origem));

  Raio raio_câmara = {
    .origem = nova_origem,
    .direção = nova_direção
  };

  return raio_câmara;
}

int gerar_geometria_esfera(Vértice* vértices_out, uint32_t* índices_out, float raio, unsigned fatias, unsigned pilhas) {
  if (fatias < 3 || pilhas < 2) {
    return 0;
  }

  int total_índices = pilhas * fatias * 6;

  // Se apenas pretendermos calcular a dimensão dos buffers para alocação externa
  if (vértices_out == nullptr || índices_out == nullptr) {
    return total_índices;
  }

  int contagem_vértices = 0;

  // 1. Gerar os Vértices, Normais e Coordenadas UV
  for (int p = 0; p <= pilhas; ++p) {
    float fator_p = (float)p / (float)pilhas;
    // phi varia de 0 a PI (do polo norte ao polo sul)
    float phi = fator_p * PI;

    for (int f = 0; f <= fatias; ++f) {
      float fator_f = (float)f / (float)fatias;
      // theta varia de 0 a 2*PI (uma volta completa)
      float theta = fator_f * 2.0f * PI;

      float sen_phi = seno(phi);
      float cos_phi = cosseno(phi);
      float sen_theta = seno(theta);
      float cos_theta = cosseno(theta);

      // Calcular as coordenadas cartesianas na superfície da esfera
      float nx = sen_phi * cos_theta;
      float ny = cos_phi; // Eixo Y como eixo vertical polar
      float nz = sen_phi * sen_theta;

      Vértice v;
      v.posição.x = nx * raio;
      v.posição.y = ny * raio;
      v.posição.z = nz * raio;

      v.uv.x = fator_f;
      v.uv.y = 1.0f - fator_p; // Inverter o eixo V para mapeamento padrão

      // Embalar a normal diretamente na cor como representação visual
      uint32_t r_norm = (uint32_t)((nx * 0.5f + 0.5f) * 255.0f);
      uint32_t g_norm = (uint32_t)((ny * 0.5f + 0.5f) * 255.0f);
      uint32_t b_norm = (uint32_t)((nz * 0.5f + 0.5f) * 255.0f);
      v.cor = (r_norm << 24) | (g_norm << 16) | (b_norm << 8) | 0xFF;

      vértices_out[contagem_vértices] = v;
      contagem_vértices++;
    }
  }

  // 2. Gerar a Topologia da Malha (Índices dos Triângulos)
  int contagem_índices = 0;
  for (int p = 0; p < pilhas; ++p) {
    for (int f = 0; f < fatias; ++f) {
      // Determinar os quatro cantos de cada quadrilátero (quad) da malha
      uint32_t canto_noroeste = (uint32_t)((p * (fatias + 1)) + f);
      uint32_t canto_nordeste = canto_noroeste + 1;
      uint32_t canto_sudoeste = canto_noroeste + (uint32_t)(fatias + 1);
      uint32_t canto_sudeste = canto_sudoeste + 1;

      // Primeiro Triângulo (Sentido dos ponteiros do relógio / Row-major convencional)
      índices_out[contagem_índices++] = canto_noroeste;
      índices_out[contagem_índices++] = canto_nordeste;
      índices_out[contagem_índices++] = canto_sudoeste;

      // Segundo Triângulo
      índices_out[contagem_índices++] = canto_nordeste;
      índices_out[contagem_índices++] = canto_sudeste;
      índices_out[contagem_índices++] = canto_sudoeste;
    }
  }

  return total_índices;
}

int gerar_geometria_cubo(Vértice* vértices_out, uint32_t* índices_out, float tamanho) {
  int total_índices = 36; // 6 faces * 2 triângulos * 3 índices

  if (vértices_out == nullptr || índices_out == nullptr) {
    return total_índices;
  }

  float metade = tamanho * 0.5f;

  // Definição das posições dos 8 cantos únicos do cubo
  Vetor3D c0 = { -metade, -metade,  metade };
  Vetor3D c1 = {  metade, -metade,  metade };
  Vetor3D c2 = {  metade,  metade,  metade };
  Vetor3D c3 = { -metade,  metade,  metade };
  Vetor3D c4 = { -metade, -metade, -metade };
  Vetor3D c5 = {  metade, -metade, -metade };
  Vetor3D c6 = {  metade,  metade, -metade };
  Vetor3D c7 = { -metade,  metade, -metade };

  // Definição das coordenadas UV padrão para um quadrilátero
  Vetor2D uv00 = { 0.0f, 0.0f };
  Vetor2D uv10 = { 1.0f, 0.0f };
  Vetor2D uv11 = { 1.0f, 1.0f };
  Vetor2D uv01 = { 0.0f, 1.0f };

  // Cores codificadas para representar a iluminação ou normais por face
  Cor cor_padrao = 0xFFFFFFFF;

  int v_idx = 0;
  int i_idx = 0;

  // Face 1: Frente (+Z)
  vértices_out[v_idx++] = (Vértice){ .posição = c0, .uv = uv00, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c1, .uv = uv10, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c2, .uv = uv11, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c3, .uv = uv01, .cor = cor_padrao };

  // Face 2: Trás (-Z)
  vértices_out[v_idx++] = (Vértice){ .posição = c5, .uv = uv00, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c4, .uv = uv10, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c7, .uv = uv11, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c6, .uv = uv01, .cor = cor_padrao };

  // Face 3: Cima (+Y)
  vértices_out[v_idx++] = (Vértice){ .posição = c3, .uv = uv00, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c2, .uv = uv10, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c6, .uv = uv11, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c7, .uv = uv01, .cor = cor_padrao };

  // Face 4: Baixo (-Y)
  vértices_out[v_idx++] = (Vértice){ .posição = c4, .uv = uv00, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c5, .uv = uv10, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c1, .uv = uv11, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c0, .uv = uv01, .cor = cor_padrao };

  // Face 5: Direita (+X)
  vértices_out[v_idx++] = (Vértice){ .posição = c1, .uv = uv00, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c5, .uv = uv10, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c6, .uv = uv11, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c2, .uv = uv01, .cor = cor_padrao };

  // Face 6: Esquerda (-X)
  vértices_out[v_idx++] = (Vértice){ .posição = c4, .uv = uv00, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c0, .uv = uv10, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c3, .uv = uv11, .cor = cor_padrao };
  vértices_out[v_idx++] = (Vértice){ .posição = c7, .uv = uv01, .cor = cor_padrao };

  // Gerar a topologia indexada para as 6 faces
  for (uint32_t face = 0; face < 6; ++face) {
    uint32_t base = face * 4;

    índices_out[i_idx++] = base + 0;
    índices_out[i_idx++] = base + 2;
    índices_out[i_idx++] = base + 1;

    índices_out[i_idx++] = base + 0;
    índices_out[i_idx++] = base + 3;
    índices_out[i_idx++] = base + 2;
  }

  return total_índices;
}

// Função interna para gerar um valor pseudoaleatório determinista (Hash) a partir de coordenadas inteiras
static inline float perlin_hash_gradiente_2d(int ix, int iy, float rx, float ry) {
  // Algoritmo de hash linear simples para misturar os bits das coordenadas discretas
  uint32_t hash = ((uint32_t)ix * 73856093U) ^ ((uint32_t)iy * 19349663U);
  hash = (hash ^ 61) ^ (hash >> 16);
  hash *= 9;
  hash = hash ^ (hash >> 4);
  hash *= 0x27d4eb2d;
  hash = hash ^ (hash >> 15);

  // Mapear o hash para 8 direções de vetores de gradiente unitários espalhados uniformemente
  float angulo = (float)(hash & 0x7) * (2.0f * PI / 8.0f);
  
  // Calcular as componentes do vetor gradiente
  float gx = cosseno(angulo);
  float gy = seno(angulo);

  // Produto escalar entre o vetor gradiente e o vetor de distância até ao canto
  return (gx * rx) + (gy * ry);
}

// Curva de interpolação quíntica suave de Ken Perlin: 6t^5 - 15t^4 + 10t^3
static inline float curva_suave_perlin(float t) {
  return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float ruido_perlin_2d(float x, float y) {
  // Determinar as coordenadas inteiras da célula da grelha (Bounding Box)
  float x_chão = chão(x);
  float y_chão = chão(y);

  int x0 = (int)x_chão;
  int y0 = (int)y_chão;
  int x1 = x0 + 1;
  int y1 = y0 + 1;

  // Calcular as distâncias locais relativas do ponto aos quatro cantos da célula
  float rx0 = x - x_chão;
  float ry0 = y - y_chão;
  float rx1 = rx0 - 1.0f;
  float ry1 = ry0 - 1.0f;

  // Calcular a influência (produto escalar) de cada um dos 4 vetores de gradiente
  float canto_noroeste = perlin_hash_gradiente_2d(x0, y0, rx0, ry0);
  float canto_nordeste = perlin_hash_gradiente_2d(x1, y0, rx1, ry0);
  float canto_sudoeste = perlin_hash_gradiente_2d(x0, y1, rx0, ry1);
  float canto_sudeste  = perlin_hash_gradiente_2d(x1, y1, rx1, ry1);

  // Calcular os fatores de atenuação/suavização para a interpolação final
  float tx = curva_suave_perlin(rx0);
  float ty = curva_suave_perlin(ry0);

  // Interpolação linear bi-direcional (Bilinear) baseada nas curvas quânticas
  float topo = interpolar_linear(canto_noroeste, canto_nordeste, tx);
  float base = interpolar_linear(canto_sudoeste, canto_sudeste, tx);
  float resultado_ruido = interpolar_linear(topo, base, ty);

  // Mapear o intervalo matemático nativo [-1.0, 1.0] para o intervalo padrão [0.0, 1.0]
  return (resultado_ruido + 1.0f) * 0.5f;
}

Cor gerar_textura_xadrez(float u, float v, Cor c1, Cor c2, float frequencia) {
  // Multiplicar as coordenadas UV pela frequência para definir o tamanho dos quadrados
  float u_escalado = u * frequencia;
  float v_escalado = v * frequencia;

  // Obter os índices inteiros das células da grelha tridimensional mapeada
  int u_int = (int)chão(u_escalado);
  int v_int = (int)chão(v_escalado);

  // O padrão de xadrez alterna com base na paridade da soma dos índices discretos
  if ((u_int + v_int) % 2 == 0) {
    return c1;
  }
  
  return c2;
}

bool inverter_matriz_4x4(float resultado[static 4][4], float matriz[static 4][4]) {
  float adjunta[4][4];

  // Elementos da matriz original organizados de forma legível por conveniência
  float m00 = matriz[0][0], m01 = matriz[0][1], m02 = matriz[0][2], m03 = matriz[0][3];
  float m10 = matriz[1][0], m11 = matriz[1][1], m12 = matriz[1][2], m13 = matriz[1][3];
  float m20 = matriz[2][0], m21 = matriz[2][1], m22 = matriz[2][2], m23 = matriz[2][3];
  float m30 = matriz[3][0], m31 = matriz[3][1], m32 = matriz[3][2], m33 = matriz[3][3];

  // Calcular os cofactores para construir a matriz adjunta (já transposta)
  adjunta[0][0] = m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31) + m13 * (m21 * m32 - m22 * m31);
  adjunta[1][0] = -(m10 * (m22 * m33 - m23 * m32) - m12 * (m20 * m33 - m23 * m32) + m13 * (m20 * m32 - m22 * m30));
  adjunta[2][0] = m10 * (m21 * m33 - m23 * m31) - m11 * (m20 * m33 - m23 * m30) + m13 * (m20 * m31 - m21 * m30);
  adjunta[3][0] = -(m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30) + m12 * (m20 * m31 - m21 * m30));

  adjunta[0][1] = -(m01 * (m22 * m33 - m23 * m32) - m02 * (m21 * m33 - m23 * m31) + m03 * (m21 * m32 - m22 * m31));
  adjunta[1][1] = m00 * (m22 * m33 - m23 * m32) - m02 * (m20 * m33 - m23 * m30) + m03 * (m20 * m32 - m22 * m30);
  adjunta[2][1] = -(m00 * (m21 * m33 - m23 * m31) - m01 * (m20 * m33 - m23 * m30) + m03 * (m20 * m31 - m21 * m30));
  adjunta[3][1] = m00 * (m21 * m32 - m22 * m31) - m01 * (m20 * m32 - m22 * m30) + m02 * (m20 * m31 - m21 * m30);

  adjunta[0][2] = m01 * (m12 * m33 - m13 * m32) - m02 * (m11 * m33 - m13 * m31) + m03 * (m11 * m32 - m12 * m31);
  adjunta[1][2] = -(m00 * (m12 * m33 - m13 * m32) - m02 * (m10 * m33 - m13 * m30) + m03 * (m10 * m32 - m12 * m30));
  adjunta[2][2] = m00 * (m11 * m33 - m13 * m31) - m01 * (m10 * m33 - m13 * m30) + m03 * (m10 * m31 - m11 * m30);
  adjunta[3][2] = -(m00 * (m11 * m32 - m12 * m31) - m01 * (m10 * m32 - m12 * m30) + m02 * (m10 * m31 - m11 * m30));

  adjunta[0][3] = -(m01 * (m12 * m23 - m13 * m22) - m02 * (m11 * m23 - m13 * m21) + m03 * (m11 * m22 - m12 * m21));
  adjunta[1][3] = m00 * (m12 * m23 - m13 * m22) - m02 * (m10 * m23 - m13 * m20) + m03 * (m10 * m22 - m12 * m20);
  adjunta[2][3] = -(m00 * (m11 * m23 - m13 * m21) - m01 * (m10 * m23 - m13 * m20) + m03 * (m10 * m21 - m11 * m20));
  adjunta[3][3] = m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) + m02 * (m10 * m21 - m11 * m20);

  // Calcular o determinante usando a primeira linha e a primeira coluna de cofactores
  float determinante = m00 * adjunta[0][0] + m01 * adjunta[1][0] + m02 * adjunta[2][0] + m03 * adjunta[3][0];

  // Verificar se o determinante é nulo ou excessivamente próximo de zero
  if (absoluto(determinante) < 1e-7f) {
    return false;
  }

  // Multiplicar a matriz adjunta pelo recíproco do determinante para obter a matriz inversa
  float determinante_inverso = 1.0f / determinante;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      resultado[i][j] = adjunta[i][j] * determinante_inverso;
    }
  }

  return true;
}

void transpor_matriz_4x4(float resultado[static 4][4], float matriz[static 4][4]) {
  // Criar uma cópia estável num buffer local para permitir operações in-place com segurança
  float temporaria[4][4];

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      temporaria[i][j] = matriz[i][j];
    }
  }

  // Permutar as linhas pelas colunas (Row-Major) no buffer de destino
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      resultado[i][j] = temporaria[j][i];
    }
  }
}

Quaternião quaternião_inverso(Quaternião q) {
  // Calcular a norma quadrada do quatérnio (o quadrado do seu comprimento)
  float norma_quadrada = (q.w * q.w) + (q.x * q.x) + (q.y * q.y) + (q.z * q.z);

  // Evitar divisão por zero caso o quatérnio seja nulo
  if (norma_quadrada <= 0.00001f) {
    return (Quaternião){ .w = 1.0f, .x = 0.0f, .y = 0.0f, .z = 0.0f };
  }

  float inverso_norma = 1.0f / norma_quadrada;

  // O inverso é o conjugado (sinal oposto no eixo imaginário) dividido pela norma quadrada
  Quaternião resultado;
  resultado.w = q.w * inverso_norma;
  resultado.x = -q.x * inverso_norma;
  resultado.y = -q.y * inverso_norma;
  resultado.z = -q.z * inverso_norma;

  return resultado;
}
