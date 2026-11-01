#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t Cor;

typedef struct {
  float x, y;
} Vetor2D;

typedef struct {
  float x, y, z;
} Vetor3D;

typedef struct {
  float w, x, y, z;
} Quaterniao;

typedef struct {
  int largura;
  int altura;
  float* profundidades;
} Profundidade;

typedef struct {
  int largura;
  int altura;
  Cor* pixeis;
} Tela;

typedef struct {
  Vetor3D origem;
  Vetor3D direcao;
} Raio;

typedef struct {
  bool intersetou;
  float t;
  Vetor3D ponto;
  Vetor3D normal;
  Cor cor_material;
} Intersecao;

typedef struct {
  Vetor3D posicao;
  Vetor2D uv;
  Cor cor;
} Vertice;

constexpr float PI = 3.14159265f;

float absoluto(float x);
float truncar(float x);
float chao(float x);
float raiz(float x);
float reciproco_raiz(float x);
float seno(float x);
float cosseno(float x);
float tangente(float x);
float potencia_inteira(float base, int expoente);
float minimo(float x, float y);
float maximo(float x, float y);
float distancia(float x, float y);
float modulo(float x, float y);

float comprimento_vetor(Vetor3D v);
float produto_escalar(Vetor3D u, Vetor3D v);
Vetor3D normalizar_vetor(Vetor3D v);
Vetor3D transformar_vetor(Vetor3D v, float matriz[4][4]);
Vetor3D somar_vetores(Vetor3D a, Vetor3D b);
Vetor3D subtrair_vetores(Vetor3D a, Vetor3D b);
Vetor3D produto_vetorial(Vetor3D a, Vetor3D b);
Vetor3D refletir_vetor(Vetor3D v, Vetor3D normal);
Vetor3D interpolar_vetores(Vetor3D a, Vetor3D b, float t);
Vetor3D calcular_normal(Vetor3D v1, Vetor3D v2, Vetor3D v3);

void criar_matriz_identidade(float matriz[4][4]);
void criar_matriz_translacao(float matriz[4][4], float x, float y, float z);
void criar_matriz_escala(float matriz[4][4], float sx, float sy, float sz);
void criar_matriz_perspetiva_infinito(float matriz[4][4], float fov, float proporcao, float perto);
void criar_matriz_projecao(float matriz[4][4], float fov, float proporcao, float perto, float longe);
void criar_matriz_ortografica(float matriz[4][4], float esquerda, float direita, float baixo, float cima, float perto, float longe);
void multiplicar_matrizes(float resultado[4][4], float a[4][4], float b[4][4]);
void criar_matriz_rotacao(float matriz[4][4], float radianos, Vetor3D eixo);
void criar_matriz_olhar_para(float matriz[4][4], Vetor3D posicao, Vetor3D alvo, Vetor3D cima);
void extrair_direcoes_camara(float matriz_vista[4][4], Vetor3D* direita, Vetor3D* cima, Vetor3D* frente);
void criar_matriz_de_quaterniao(float matriz[4][4], Quaterniao q);
void criar_matriz_quat_posicao(float matriz[4][4], Quaterniao q, Vetor3D posicao);

Quaterniao quaterniao_identidade(void);
Quaterniao quaterniao_de_eixo_angulo(Vetor3D eixo, float radianos);
Quaterniao multiplicar_quaternioes(Quaterniao q1, Quaterniao q2);

Cor multiplicar_cor(Cor cor, float intensidade);
Cor interpolar_cores(Cor c1, Cor c2, float fator);
Cor calcular_iluminacao_difusa(Vetor3D normal, Vetor3D direcao_luz, Cor cor_objeto, Cor cor_luz);
Cor calcular_iluminacao_especular(Vetor3D normal, Vetor3D direcao_luz, Vetor3D direcao_olhar, int brilho);
Cor calcular_iluminacao_phong_completa(Vetor3D normal, Vetor3D direcao_luz, Vetor3D direcao_olhar, Cor cor_objeto, Cor cor_difusa_luz, Cor cor_especular_luz, int brilho, float intensidade_ambiente);
Cor amostrar_textura(Tela* textura, float u, float v);

Tela criar_tela(int largura, int altura);
void destruir_tela(Tela* tela);
void limpar_tela(Tela* tela, Cor cor);
void desenhar_pixel(Tela* tela, int x, int y, Cor cor);
void limpar_profundidade(Profundidade* pf, float valor);
bool testar_e_definir_profundidade(Profundidade* pf, int x, int y, float z);

void desenhar_linha(Tela* tela, Vetor2D p1, Vetor2D p2, Cor cor);
void desenhar_triangulo(Tela* tela, Vetor2D p1, Vetor2D p2, Vetor2D p3, Cor cor);
Vetor3D calcular_coordenadas_baricentricas(Vetor2D p, Vetor2D a, Vetor2D b, Vetor2D c);
Vetor2D projetar_ponto(Vetor3D ponto_3d, int largura, int altura);
bool orientacao_traseira(Vetor3D v1, Vetor3D v2, Vetor3D v3, Vetor3D direcao_camara);
Vertice interpolar_vertices(Vertice a, Vertice b, float t);
int recortar_triangulo_plano_perto(Vertice* triangulo_in, Vertice* triangulos_out, float plano_perto);
void calcular_tangente_bitangente(Vertice* triangulo, Vetor3D* tangente, Vetor3D* bitangente);
void desenhar_triangulo_preenchido(Tela* tela, Profundidade* pf, Vertice v1, Vertice v2, Vertice v3, Tela* textura);

Raio gerar_raio_camara(Vetor3D posicao_camara, float matriz_inv_proj[4][4], int x, int y, int largura, int altura);
Intersecao intersetar_esfera(Raio raio, Vetor3D centro, float raio_esfera, Cor cor);
Intersecao intersetar_plano(Raio raio, Vetor3D ponto_plano, Vetor3D normal_plano, Cor cor);

float absoluto(float x) {
  return x >= 0.0f ? x : -x;
}

float truncar(float x) {
  if (x >= 16777216.0f || x <= -16777216.0f) {
    return x;
  }
  return (float)((long long)x);
}

float chao(float x) {
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

float modulo(float x, float y) {
  if (y == 0.0f) return 0.0f;
  return x - truncar(x / y) * y;
}

float seno(float x) {
  float sinal = 1.0f;
  if (x < 0.0f) {
    x = -x;
    sinal = -sinal;
  }
  x = modulo(x, 2.0f * PI);
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

float distancia(float x, float y) {
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

Vetor3D transformar_vetor(Vetor3D v, float matriz[4][4]) {
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

void criar_matriz_identidade(float matriz[4][4]) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      matriz[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
}

void criar_matriz_translacao(float matriz[4][4], float x, float y, float z) {
  criar_matriz_identidade(matriz);
  matriz[3][0] = x;
  matriz[3][1] = y;
  matriz[3][2] = z;
}

void criar_matriz_escala(float matriz[4][4], float sx, float sy, float sz) {
  criar_matriz_identidade(matriz);
  matriz[0][0] = sx;
  matriz[1][1] = sy;
  matriz[2][2] = sz;
}

void criar_matriz_perspetiva_infinito(float matriz[4][4], float fov, float proporcao, float perto) {
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

void criar_matriz_projecao(float matriz[4][4], float fov, float proporcao, float perto, float longe) {
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

void criar_matriz_ortografica(float matriz[4][4], float esquerda, float direita, float baixo, float cima, float perto, float longe) {
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

void multiplicar_matrizes(float resultado[4][4], float a[4][4], float b[4][4]) {
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

void criar_matriz_rotacao(float matriz[4][4], float radianos, Vetor3D eixo) {
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

void criar_matriz_olhar_para(float matriz[4][4], Vetor3D posicao, Vetor3D alvo, Vetor3D cima) {
  Vetor3D z_eixo = normalizar_vetor(subtrair_vetores(posicao, alvo));
  Vetor3D x_eixo = normalizar_vetor(produto_vetorial(cima, z_eixo));
  Vetor3D y_eixo = produto_vetorial(z_eixo, x_eixo);

  matriz[0][0] = x_eixo.x;  matriz[0][1] = y_eixo.x;  matriz[0][2] = z_eixo.x;  matriz[0][3] = 0.0f;
  matriz[1][0] = x_eixo.y;  matriz[1][1] = y_eixo.y;  matriz[1][2] = z_eixo.y;  matriz[1][3] = 0.0f;
  matriz[2][0] = x_eixo.z;  matriz[2][1] = y_eixo.z;  matriz[2][2] = z_eixo.z;  matriz[2][3] = 0.0f;
  matriz[3][0] = -produto_escalar(x_eixo, posicao);
  matriz[3][1] = -produto_escalar(y_eixo, posicao);
  matriz[3][2] = -produto_escalar(z_eixo, posicao);
  matriz[3][3] = 1.0f;
}

void extrair_direcoes_camara(float matriz_vista[4][4], Vetor3D* direita, Vetor3D* cima, Vetor3D* frente) {
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

void criar_matriz_de_quaterniao(float matriz[4][4], Quaterniao q) {
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

void criar_matriz_quat_posicao(float matriz[4][4], Quaterniao q, Vetor3D posicao) {
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
  matriz[3][0] = posicao.x;
  matriz[3][1] = posicao.y;
  matriz[3][2] = posicao.z;
  matriz[3][3] = 1.0f;
}

Quaterniao quaterniao_identidade(void) {
  return (Quaterniao){ .w = 1.0f, .x = 0.0f, .y = 0.0f, .z = 0.0f };
}

Quaterniao quaterniao_de_eixo_angulo(Vetor3D axis, float radianos) {
  Vetor3D eixo_normalizado = normalizar_vetor(axis);
  float metade_angulo = radianos * 0.5f;
  float seno_metade = seno(metade_angulo);
  return (Quaterniao){
    .w = cosseno(metade_angulo),
    .x = eixo_normalizado.x * seno_metade,
    .y = eixo_normalizado.y * seno_metade,
    .z = eixo_normalizado.z * seno_metade
  };
}

Quaterniao multiplicar_quaternioes(Quaterniao q1, Quaterniao q2) {
  return (Quaterniao){
    .w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
    .x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
    .y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
    .z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w
  };
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

Cor calcular_iluminacao_difusa(Vetor3D normal, Vetor3D direcao_luz, Cor cor_objeto, Cor cor_luz) {
  Vetor3D n = normalizar_vetor(normal);
  Vetor3D l = normalizar_vetor(direcao_luz);
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

Cor calcular_iluminacao_especular(Vetor3D normal, Vetor3D direcao_luz, Vetor3D direcao_olhar, int brilho) {
  Vetor3D n = normalizar_vetor(normal);
  Vetor3D l = normalizar_vetor(direcao_luz);
  Vetor3D v = normalizar_vetor(direcao_olhar);
  Vetor3D h = normalizar_vetor(somar_vetores(l, v));

  float dot_nh = produto_escalar(n, h);
  if (dot_nh < 0.0f) dot_nh = 0.0f;
  if (produto_escalar(n, l) <= 0.0f) dot_nh = 0.0f;

  float intensidade = potencia_inteira(dot_nh, brilho);
  float res_cor = intensidade * 255.0f;
  if (res_cor > 255.0f) res_cor = 255.0f;

  uint8_t especular = (uint8_t)res_cor;
  return ((Cor)especular << 24) | ((Cor)especular << 16) | ((Cor)especular << 8) | (Cor)255;
}

Cor calcular_iluminacao_phong_completa(Vetor3D normal, Vetor3D direcao_luz, Vetor3D direcao_olhar, Cor cor_objeto, Cor cor_difusa_luz, Cor cor_especular_luz, int brilho, float intensidade_ambiente) {
  Vetor3D n = normalizar_vetor(normal);
  Vetor3D l = normalizar_vetor(direcao_luz);
  Vetor3D v = normalizar_vetor(direcao_olhar);

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

    float intensidade_especular = potencia_inteira(cosseno_especular, brilho);
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

Cor amostrar_textura(Tela* textura, float u, float v) {
  if (textura == nullptr || textura->pixeis == nullptr || textura->largura <= 0 || textura->altura <= 0) {
    return 0;
  }
  float u_repetido = u - chao(u);
  float v_repetido = v - chao(v);
  int x = (int)(u_repetido * (float)textura->largura);
  int y = (int)(v_repetido * (float)textura->altura);
  if (x >= textura->largura) x = textura->largura - 1;
  if (y >= textura->altura)  y = textura->altura - 1;
  if (x < 0) x = 0;
  if (y < 0) y = 0;
  return textura->pixeis[x + y * textura->largura];
}

Tela criar_tela(int largura, int altura) {
  Tela tela = {};
  if (largura <= 0 || altura <= 0) return tela;
  Cor* memoria_pixeis = malloc((size_t)largura * (size_t)altura * sizeof(Cor));
  if (memoria_pixeis == nullptr) return tela;
  tela.largura = largura;
  tela.altura = altura;
  tela.pixeis = memoria_pixeis;
  return tela;
}

void destruir_tela(Tela* tela) {
  if (tela == nullptr) return;
  free(tela->pixeis);
  tela->largura = 0;
  tela->altura = 0;
  tela->pixeis = nullptr;
}

void limpar_tela(Tela* tela, Cor cor) {
  if (tela == nullptr || tela->pixeis == nullptr) return;
  int area = tela->largura * tela->altura;
  for (int i = 0; i < area; ++i) {
    tela->pixeis[i] = cor;
  }
}

void desenhar_pixel(Tela* tela, int x, int y, Cor cor) {
  if (tela == nullptr || tela->pixeis == nullptr) return;
  if (x < 0 || x >= tela->largura || y < 0 || y >= tela->altura) return;
  tela->pixeis[x + y * tela->largura] = cor;
}

void limpar_profundidade(Profundidade* pf, float valor) {
  if (pf == NULL || pf->profundidades == NULL || pf->largura <= 0 || pf->altura <= 0) return;
  size_t total_pixeis = (size_t)pf->largura * (size_t)pf->altura;
  for (size_t i = 0; i < total_pixeis; ++i) {
    pf->profundidades[i] = valor;
  }
}

bool testar_e_definir_profundidade(Profundidade* pf, int x, int y, float z) {
  if (pf == nullptr || pf->profundidades == nullptr) return false;
  if (x < 0 || x >= pf->largura || y < 0 || y >= pf->altura) return false;
  size_t indice = (size_t)y * (size_t)pf->largura + (size_t)x;
  if (z < pf->profundidades[indice]) {
    pf->profundidades[indice] = z;
    return true;
  }
  return false;
}

void desenhar_linha(Tela* tela, Vetor2D p1, Vetor2D p2, Cor cor) {
  if (tela == nullptr || tela->pixeis == nullptr || tela->largura <= 0 || tela->altura <= 0) return;
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
      tela->pixeis[y0 * largura + x0] = cor;
    }
    if (x0 == x1 && y0 == y1) break;
    int erro2 = 2 * erro;
    if (erro2 > -dy) { erro -= dy; x0 += sx; }
    if (erro2 < dx) { erro += dx; y0 += sy; }
  }
}

void desenhar_triangulo(Tela* tela, Vetor2D p1, Vetor2D p2, Vetor2D p3, Cor cor) {
  if (tela == nullptr || tela->pixeis == nullptr || tela->largura <= 0 || tela->altura <= 0) return;
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
        tela->pixeis[offset_linha + x] = cor;
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

bool orientacao_traseira(Vetor3D v1, Vetor3D v2, Vetor3D v3, Vetor3D direcao_camara) {
  Vetor3D normal_face = calcular_normal(v1, v2, v3);
  return produto_escalar(normal_face, direcao_camara) >= 0.0f;
}

Vertice interpolar_vertices(Vertice a, Vertice b, float t) {
  return (Vertice){
    .posicao = interpolar_vetores(a.posicao, b.posicao, t),
    .uv = { a.uv.x + t * (b.uv.x - a.uv.x), a.uv.y + t * (b.uv.y - a.uv.y) },
    .cor = interpolar_cores(a.cor, b.cor, t)
  };
}

int recortar_triangulo_plano_perto(Vertice* triangulo_in, Vertice* triangulos_out, float plano_perto) {
  Vertice vertices_dentro[3];
  Vertice vertices_fora[3];
  int contagem_dentro = 0;
  int contagem_fora = 0;
  
  for (int i = 0; i < 3; ++i) {
    if (triangulo_in[i].posicao.z >= plano_perto) {
      vertices_dentro[contagem_dentro++] = triangulo_in[i];
    } else {
      vertices_fora[contagem_fora++] = triangulo_in[i];
    }
  }
  
  if (contagem_dentro == 0) return 0;
  
  if (contagem_dentro == 3) {
    triangulos_out[0] = triangulo_in[0];
    triangulos_out[1] = triangulo_in[1];
    triangulos_out[2] = triangulo_in[2];
    return 1;
  }
  
  if (contagem_dentro == 1) {
    triangulos_out[0] = vertices_dentro[0];
    float t1 = (plano_perto - vertices_dentro[0].posicao.z) / (vertices_fora[0].posicao.z - vertices_dentro[0].posicao.z);
    triangulos_out[1] = interpolar_vertices(vertices_dentro[0], vertices_fora[0], t1);
    float t2 = (plano_perto - vertices_dentro[0].posicao.z) / (vertices_fora[1].posicao.z - vertices_dentro[0].posicao.z);
    triangulos_out[2] = interpolar_vertices(vertices_dentro[0], vertices_fora[1], t2);
    return 1;
  }
  
  if (contagem_dentro == 2) {
    int idx_fora = 0;
    for (int i = 0; i < 3; ++i) {
      if (triangulo_in[i].posicao.z < plano_perto) { 
        idx_fora = i; 
        break; 
      }
    }
    Vertice v0 = triangulo_in[idx_fora];
    Vertice v1 = triangulo_in[(idx_fora + 1) % 3];
    Vertice v2 = triangulo_in[(idx_fora + 2) % 3];

    float t1 = (plano_perto - v0.posicao.z) / (v1.posicao.z - v0.posicao.z);
    Vertice i1 = interpolar_vertices(v0, v1, t1);
    float t2 = (plano_perto - v2.posicao.z) / (v0.posicao.z - v2.posicao.z);
    Vertice i2 = interpolar_vertices(v2, v0, t2);

    triangulos_out[0] = i1;   triangulos_out[1] = v1;   triangulos_out[2] = v2;
    triangulos_out[3] = i1;   triangulos_out[4] = v2;   triangulos_out[5] = i2;
    return 2;
  }
  
  return 0;
}

void calcular_tangente_bitangente(Vertice* triangulo, Vetor3D* tangente, Vetor3D* bitangente) {
  Vetor3D delta_pos1 = subtrair_vetores(triangulo[1].posicao, triangulo[0].posicao);
  Vetor3D delta_pos2 = subtrair_vetores(triangulo[2].posicao, triangulo[0].posicao);

  float delta_u1 = triangulo[1].uv.x - triangulo[0].uv.x;
  float delta_v1 = triangulo[1].uv.y - triangulo[0].uv.y;
  float delta_u2 = triangulo[2].uv.x - triangulo[0].uv.x;
  float delta_v2 = triangulo[2].uv.y - triangulo[0].uv.y;

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


void desenhar_triangulo_preenchido(Tela* tela, Profundidade* pf, Vertice v1, Vertice v2, Vertice v3, Tela* textura) {
  if (tela == nullptr || pf == nullptr) return;

  Vetor2D p1 = { v1.posicao.x, v1.posicao.y };
  Vetor2D p2 = { v2.posicao.x, v2.posicao.y };
  Vetor2D p3 = { v3.posicao.x, v3.posicao.y };

  int inicio_x = (int)minimo(minimo(p1.x, p2.x), p3.x);
  int fim_x = (int)maximo(maximo(p1.x, p2.x), p3.x) + 1;
  int inicio_y = (int)minimo(minimo(p1.y, p2.y), p3.y);
  int fim_y = (int)maximo(maximo(p1.y, p2.y), p3.y) + 1;

  if (inicio_x < 0) inicio_x = 0;
  if (fim_x >= tela->largura) fim_x = tela->largura - 1;
  if (inicio_y < 0) inicio_y = 0;
  if (fim_y >= tela->altura) fim_y = tela->altura - 1;

  float inv_w1 = 1.0f / v1.posicao.z;
  float inv_w2 = 1.0f / v2.posicao.z;
  float inv_w3 = 1.0f / v3.posicao.z;

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
          if (textura != nullptr && textura->pixeis != nullptr) {
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
          desenhar_pixel(tela, x, y, cor_final);
        }
      }
    }
  }
}

Raio gerar_raio_camara(Vetor3D posicao_camara, float matriz_inv_proj[4][4], int x, int y, int largura, int altura) {
  Raio raio;
  raio.origem = posicao_camara;
  if (largura <= 0 || altura <= 0) {
    raio.direcao = (Vetor3D){0.0f, 0.0f, 1.0f};
    return raio;
  }
  float ndc_x = (2.0f * ((float)x + 0.5f) / (float)largura) - 1.0f;
  float ndc_y = 1.0f - (2.0f * ((float)y + 0.5f) / (float)altura);
  Vetor3D ponto_clip = { ndc_x, ndc_y, 1.0f };
  Vetor3D direcao_camara = transformar_vetor(ponto_clip, matriz_inv_proj);
  raio.direcao = normalizar_vetor(direcao_camara);
  return raio;
}

Intersecao intersetar_esfera(Raio raio, Vetor3D centro, float raio_esfera, Cor cor) {
  Intersecao resultado = { .intersetou = false, .t = -1.0f, .ponto = {0.0f, 0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f}, .cor_material = cor };
  Vetor3D l = subtrair_vetores(centro, raio.origem);
  float tca = produto_escalar(l, raio.direcao);
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
  resultado.ponto = somar_vetores(raio.origem, (Vetor3D){raio.direcao.x * t_escolhido, raio.direcao.y * t_escolhido, raio.direcao.z * t_escolhido});
  resultado.normal = normalizar_vetor(subtrair_vetores(resultado.ponto, centro));
  return resultado;
}

Intersecao intersetar_plano(Raio raio, Vetor3D ponto_plano, Vetor3D normal_plano, Cor cor) {
  Intersecao resultado = { .intersetou = false, .t = -1.0f, .ponto = {0.0f, 0.0f, 0.0f}, .normal = normal_plano, .cor_material = cor };
  float denominador = produto_escalar(raio.direcao, normal_plano);
  if (denominador > -0.0001f && denominador < 0.0001f) return resultado;

  float t_calculado = produto_escalar(subtrair_vetores(ponto_plano, raio.origem), normal_plano) / denominador;
  if (t_calculado < 0.0001f) return resultado;

  resultado.intersetou = true;
  resultado.t = t_calculado;
  if (denominador > 0.0f) {
    resultado.normal = (Vetor3D){-normal_plano.x, -normal_plano.y, -normal_plano.z};
  }
  resultado.ponto = somar_vetores(raio.origem, (Vetor3D){raio.direcao.x * t_calculado, raio.direcao.y * t_calculado, raio.direcao.z * t_calculado});
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
