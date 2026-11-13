#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  MISTURA_NORMAL,
  MISTURA_ADITIVA,
  MISTURA_MULTIPLICAR,
  MISTURA_ECRÃ
} Mistura;

typedef struct {
  uint8_t vermelho;
  uint8_t verde;
  uint8_t azul;
  uint8_t alfa;
} Cor;

typedef struct {
  float matiz;
  float saturação;
  float valor;
  float alfa;
} CorHSV;

typedef struct {
  int32_t x;
  int32_t y;
} Int2D;

typedef struct {
  float x;
  float y;
} Flut2D;

typedef struct {
  float m[3][3];
} Matriz3x3;

typedef struct {
  uint32_t largura;
  uint32_t altura;
  Cor *píxeis;
} Tela;

[[maybe_unused]] constexpr Cor COR_BRANCA   = { .vermelho = 255, .verde = 255, .azul = 255, .alfa = 255 };
[[maybe_unused]] constexpr Cor COR_PRETA    = { .vermelho = 0,   .verde = 0,   .azul = 0,   .alfa = 255 };
[[maybe_unused]] constexpr Cor COR_VERMELHA = { .vermelho = 255, .verde = 0,   .azul = 0,   .alfa = 255 };
[[maybe_unused]] constexpr Cor COR_VERDE    = { .vermelho = 0,   .verde = 255, .azul = 0,   .alfa = 255 };
[[maybe_unused]] constexpr Cor COR_AZUL     = { .vermelho = 0,   .verde = 0,   .azul = 255, .alfa = 255 };
[[maybe_unused]] constexpr Cor COR_CIANO    = { .vermelho = 0,   .verde = 255, .azul = 255, .alfa = 255 };
[[maybe_unused]] constexpr Cor COR_MAGENTA  = { .vermelho = 255, .verde = 0,   .azul = 255, .alfa = 255 };
[[maybe_unused]] constexpr Cor COR_AMARELA  = { .vermelho = 255, .verde = 255, .azul = 0,   .alfa = 255 };

int comparar_int32(const void *a, const void *b);

static inline float normalizar_radianos(float ângulo);

bool cores_iguais(Cor a, Cor b);
Cor interpolar_cor(Cor a, Cor b, float fator);

Cor cor_de_hsv(CorHSV hsv);
CorHSV cor_para_hsv(Cor cor);

void transformar_pontos(const Int2D pontos_origem[], Int2D pontos_destino[], size_t núm_pontos, Int2D translação, float radianos, float escala);
Int2D rodar_ponto(Int2D ponto, Int2D origem, float radianos);
Cor obter_píxel(const Tela *tela, Int2D ponto);

void criar_matriz_identidade(Matriz3x3 *destino);
void criar_matriz_translação(Matriz3x3 *destino, float tx, float ty);
void criar_matriz_rotação(Matriz3x3 *destino, float radianos);
void criar_matriz_escala(Matriz3x3 *destino, float sx, float sy);
void multiplicar_matrizes(Matriz3x3 *destino, const Matriz3x3 *a, const Matriz3x3 *b);
bool inverter_matriz(Matriz3x3 *destino, const Matriz3x3 *m);
Int2D transformar_int2d_matriz(Int2D ponto, const Matriz3x3 *m);
Flut2D transformar_flut2d_matriz(Flut2D ponto, const Matriz3x3 *m);

Tela criar_tela(uint32_t largura, uint32_t altura);
void destruir_tela(Tela *tela);
void limpar_tela(Tela *tela, Cor fundo);
void desenhar_píxel(Tela *tela, Int2D ponto, Cor cor);
void desenhar_píxel_alfa(Tela *tela, Int2D ponto, Cor cor_fonte);
void desenhar_píxel_mistura(Tela *tela, Int2D ponto, Cor cor_fonte, Mistura modo);
void desenhar_linha(Tela *tela, Int2D início, Int2D fim, Cor cor);
void desenhar_linha_horizontal(Tela *tela, int32_t y, int32_t x_início, int32_t x_fim, Cor cor);
void desenhar_linha_espessa(Tela *tela, Int2D início, Int2D fim, uint32_t espessura, Cor cor);
void desenhar_linha_suavizada(Tela *tela, Int2D início, Int2D fim, Cor cor);
void desenhar_polilinha(Tela *tela, const Int2D vértices[], size_t núm_vértices, Cor cor, bool fechada);
void desenhar_curva_bezier_quadrática(Tela *tela, Int2D p0, Int2D p1, Int2D p2, Cor cor);
void desenhar_curva_bezier_cúbica(Tela *tela, Int2D p0, Int2D p1, Int2D p2, Int2D p3, Cor cor);
void desenhar_curva_bezier_adaptativa(Tela *tela, Int2D p0, Int2D p1, Int2D p2, Int2D p3, Cor cor, float tolerância_píxeis);
void desenhar_spline_catmull_rom(Tela *tela, const Int2D pontos[], size_t núm_pontos, uint32_t subdivisões_por_segmento, Cor cor);
void desenhar_triângulo(Tela *tela, Int2D p1, Int2D p2, Int2D p3, Cor cor);
void desenhar_triângulo_preenchido(Tela *tela, Int2D p1, Int2D p2, Int2D p3, Cor cor);
void desenhar_retângulo(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, Cor cor);
void desenhar_retângulo_preenchido(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, Cor cor);
void desenhar_retângulo_arredondado(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, uint32_t raio, Cor cor);
void desenhar_retângulo_arredondado_preenchido(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, uint32_t raio, Cor cor);
void desenhar_gradiente_linear(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, Cor cor_início, Cor cor_fim, bool vertical);
void desenhar_polígono(Tela *tela, const Int2D vértices[], size_t núm_vértices, Cor cor);
void desenhar_polígono_preenchido(Tela *tela, const Int2D vértices[], size_t núm_vértices, Cor cor);
void desenhar_círculo(Tela *tela, Int2D centro, uint32_t raio, Cor cor);
void desenhar_círculo_preenchido(Tela *tela, Int2D centro, uint32_t raio, Cor cor);
void desenhar_gradiente_radial(Tela *tela, Int2D centro, uint32_t raio, Cor cor_centro, Cor cor_periferia);
void desenhar_arco(Tela *tela, Int2D centro, uint32_t raio, float ângulo_início, float ângulo_fim, Cor cor);
void desenhar_setor_circular(Tela *tela, Int2D centro, uint32_t raio, float ângulo_início, float ângulo_fim, Cor cor);
void desenhar_elipse(Tela *tela, Int2D centro, uint32_t rx, uint32_t ry, Cor cor);
void desenhar_elipse_preenchida(Tela *tela, Int2D centro, uint32_t rx, uint32_t ry, Cor cor);
void preencher_área(Tela *tela, Int2D semente, Cor nova_cor);
void desenhar_imagem(Tela * destino, const Tela * origem, Int2D posição);
void desenhar_imagem_escala(Tela * destino, const Tela * origem, Int2D posição, uint32_t largura_nova, uint32_t altura_nova);
void desenhar_imagem_rodada(Tela * destino,  const Tela * origem,  Int2D posição,  float radianos,  Int2D pivô);
void desenhar_subimagem(Tela * destino, const Tela * origem, Int2D pos_destino, Int2D pos_origem, uint32_t larg_sub, uint32_t alt_sub);
void desenhar_imagem_mistura(Tela * destino, const Tela * origem, Int2D posição, Mistura modo);
void desenhar_imagem_matriz(Tela * destino, const Tela * origem, Matriz3x3 m);
void aplicar_matriz_convolução(Tela *tela, Matriz3x3 núcleo, float divisor, float viés);
void aplicar_desfoque_gaussiano(Tela *tela, uint32_t raio);
void converter_para_tons_de_cinzento(Tela *tela);
void aplicar_modulação_alfa(Tela *tela, uint8_t alfa);
void ajustar_brilho_contraste(Tela *tela, float brilho, float contraste);
void inverter_cores(Tela *tela);
void espelhar_tela_horizontal(Tela *tela);
void espelhar_tela_vertical(Tela *tela);

bool carregar_de_ficheiro(Tela *destino, const char *caminho);
bool exportar_para_ficheiro_ppm(const Tela *tela, const char *caminho);
bool exportar_para_ficheiro_bmp(const Tela *tela, const char *caminho);
bool exportar_para_ficheiro_tga(const Tela *tela, const char *caminho);



#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static inline float limitar_flut(float valor, float min, float max) {
  if (valor < min) return min;
  if (valor > max) return max;
  return valor;
}

static inline uint8_t limitar_u8(int32_t valor) {
  if (valor < 0) return 0;
  if (valor > 255) return 255;
  return (uint8_t)valor;
}

int comparar_int32(const void *a, const void *b) {
  int32_t va = *(const int32_t *)a;
  int32_t vb = *(const int32_t *)b;
  return (va > vb) - (va < vb);
}

static inline float normalizar_radianos(float ângulo) {
  const float dois_pi = (float)(2.0 * M_PI);
  ângulo = fmodf(ângulo, dois_pi);
  if (ângulo < 0.0f) {
    ângulo += dois_pi;
  }
  return ângulo;
}

bool cores_iguais(Cor a, Cor b) {
  return a.vermelho == b.vermelho &&
         a.verde == b.verde &&
         a.azul == b.azul &&
         a.alfa == b.alfa;
}

Cor interpolar_cor(Cor a, Cor b, float fator) {
  float t = limitar_flut(fator, 0.0f, 1.0f);
  return (Cor){
    .vermelho = (uint8_t)roundf((float)a.vermelho + ((float)b.vermelho - (float)a.vermelho) * t),
    .verde    = (uint8_t)roundf((float)a.verde    + ((float)b.verde    - (float)a.verde)    * t),
    .azul     = (uint8_t)roundf((float)a.azul     + ((float)b.azul     - (float)a.azul)     * t),
    .alfa     = (uint8_t)roundf((float)a.alfa     + ((float)b.alfa     - (float)a.alfa)     * t)
  };
}

Cor cor_de_hsv(CorHSV hsv) {
  float h = normalizar_radianos(hsv.matiz) * (180.0f / (float)M_PI);
  float s = limitar_flut(hsv.saturação, 0.0f, 1.0f);
  float v = limitar_flut(hsv.valor, 0.0f, 1.0f);
  float a = limitar_flut(hsv.alfa, 0.0f, 1.0f);

  float c = v * s;
  float x = c * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f));
  float m = v - c;

  float r1 = 0.0f, g1 = 0.0f, b1 = 0.0f;
  if (h < 60.0f)       { r1 = c; g1 = x; b1 = 0; }
  else if (h < 120.0f) { r1 = x; g1 = c; b1 = 0; }
  else if (h < 180.0f) { r1 = 0; g1 = c; b1 = x; }
  else if (h < 240.0f) { r1 = 0; g1 = x; b1 = c; }
  else if (h < 300.0f) { r1 = x; g1 = 0; b1 = c; }
  else                 { r1 = c; g1 = 0; b1 = x; }

  return (Cor){
    .vermelho = (uint8_t)roundf((r1 + m) * 255.0f),
    .verde    = (uint8_t)roundf((g1 + m) * 255.0f),
    .azul     = (uint8_t)roundf((b1 + m) * 255.0f),
    .alfa     = (uint8_t)roundf(a * 255.0f)
  };
}

CorHSV cor_para_hsv(Cor cor) {
  float r = (float)cor.vermelho / 255.0f;
  float g = (float)cor.verde / 255.0f;
  float b = (float)cor.azul / 255.0f;

  float max = fmaxf(r, fmaxf(g, b));
  float min = fminf(r, fminf(g, b));
  float delta = max - min;

  float h_graus = 0.0f;
  if (delta > 1e-6f) {
    if (max == r) {
      h_graus = 60.0f * fmodf(((g - b) / delta), 6.0f);
    } else if (max == g) {
      h_graus = 60.0f * (((b - r) / delta) + 2.0f);
    } else {
      h_graus = 60.0f * (((r - g) / delta) + 4.0f);
    }
  }
  if (h_graus < 0.0f) h_graus += 360.0f;

  float s = (max > 1e-6f) ? (delta / max) : 0.0f;

  return (CorHSV){
    .matiz     = h_graus * ((float)M_PI / 180.0f),
    .saturação = s,
    .valor     = max,
    .alfa      = (float)cor.alfa / 255.0f
  };
}

Int2D rodar_ponto(Int2D ponto, Int2D origem, float radianos) {
  float s = sinf(radianos);
  float c = cosf(radianos);
  float dx = (float)(ponto.x - origem.x);
  float dy = (float)(ponto.y - origem.y);

  return (Int2D){
    .x = (int32_t)roundf(dx * c - dy * s) + origem.x,
    .y = (int32_t)roundf(dx * s + dy * c) + origem.y
  };
}

void transformar_pontos(const Int2D pontos_origem[], Int2D pontos_destino[], size_t núm_pontos, Int2D translação, float radianos, float escala) {
  if (!pontos_origem || !pontos_destino) return;
  float s = sinf(radianos) * escala;
  float c = cosf(radianos) * escala;

  for (size_t i = 0; i < núm_pontos; i++) {
    float x = (float)pontos_origem[i].x;
    float y = (float)pontos_origem[i].y;
    pontos_destino[i].x = (int32_t)roundf(x * c - y * s) + translação.x;
    pontos_destino[i].y = (int32_t)roundf(x * s + y * c) + translação.y;
  }
}

void criar_matriz_identidade(Matriz3x3 *destino) {
  if (!destino) return;
  *destino = (Matriz3x3){
    .m = {
      { 1.0f, 0.0f, 0.0f },
      { 0.0f, 1.0f, 0.0f },
      { 0.0f, 0.0f, 1.0f }
    }
  };
}

void criar_matriz_translação(Matriz3x3 *destino, float tx, float ty) {
  if (!destino) return;
  criar_matriz_identidade(destino);
  destino->m[0][2] = tx;
  destino->m[1][2] = ty;
}

void criar_matriz_rotação(Matriz3x3 *destino, float radianos) {
  if (!destino) return;
  float c = cosf(radianos);
  float s = sinf(radianos);
  *destino = (Matriz3x3){
    .m = {
      {  c,   -s,   0.0f },
      {  s,    c,   0.0f },
      { 0.0f, 0.0f, 1.0f }
    }
  };
}

void criar_matriz_escala(Matriz3x3 *destino, float sx, float sy) {
  if (!destino) return;
  *destino = (Matriz3x3){
    .m = {
      { sx,   0.0f, 0.0f },
      { 0.0f, sy,   0.0f },
      { 0.0f, 0.0f, 1.0f }
    }
  };
}

void multiplicar_matrizes(Matriz3x3 *destino, const Matriz3x3 *a, const Matriz3x3 *b) {
  if (!destino || !a || !b) return;
  Matriz3x3 res = {0};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      res.m[i][j] = a->m[i][0] * b->m[0][j] +
                    a->m[i][1] * b->m[1][j] +
                    a->m[i][2] * b->m[2][j];
    }
  }
  *destino = res;
}

bool inverter_matriz(Matriz3x3 *destino, const Matriz3x3 *m) {
  if (!destino || !m) return false;
  float det = m->m[0][0] * (m->m[1][1] * m->m[2][2] - m->m[2][1] * m->m[1][2]) -
              m->m[0][1] * (m->m[1][0] * m->m[2][2] - m->m[1][2] * m->m[2][0]) +
              m->m[0][2] * (m->m[1][0] * m->m[2][1] - m->m[1][1] * m->m[2][0]);

  if (fabsf(det) < 1e-8f) return false;
  float inv_det = 1.0f / det;

  Matriz3x3 inv;
  inv.m[0][0] = (m->m[1][1] * m->m[2][2] - m->m[2][1] * m->m[1][2]) * inv_det;
  inv.m[0][1] = (m->m[0][2] * m->m[2][1] - m->m[0][1] * m->m[2][2]) * inv_det;
  inv.m[0][2] = (m->m[0][1] * m->m[1][2] - m->m[0][2] * m->m[1][1]) * inv_det;
  inv.m[1][0] = (m->m[1][2] * m->m[2][0] - m->m[1][0] * m->m[2][2]) * inv_det;
  inv.m[1][1] = (m->m[0][0] * m->m[2][2] - m->m[0][2] * m->m[2][0]) * inv_det;
  inv.m[1][2] = (m->m[1][0] * m->m[0][2] - m->m[0][0] * m->m[1][2]) * inv_det;
  inv.m[2][0] = (m->m[1][0] * m->m[2][1] - m->m[2][0] * m->m[1][1]) * inv_det;
  inv.m[2][1] = (m->m[2][0] * m->m[0][1] - m->m[0][0] * m->m[2][1]) * inv_det;
  inv.m[2][2] = (m->m[0][0] * m->m[1][1] - m->m[1][0] * m->m[0][1]) * inv_det;

  *destino = inv;
  return true;
}

Flut2D transformar_flut2d_matriz(Flut2D ponto, const Matriz3x3 *m) {
  if (!m) return ponto;
  float x = m->m[0][0] * ponto.x + m->m[0][1] * ponto.y + m->m[0][2];
  float y = m->m[1][0] * ponto.x + m->m[1][1] * ponto.y + m->m[1][2];
  float w = m->m[2][0] * ponto.x + m->m[2][1] * ponto.y + m->m[2][2];

  if (fabsf(w) > 1e-6f && fabsf(w - 1.0f) > 1e-6f) {
    x /= w;
    y /= w;
  }
  return (Flut2D){ .x = x, .y = y };
}

Int2D transformar_int2d_matriz(Int2D ponto, const Matriz3x3 *m) {
  Flut2D res = transformar_flut2d_matriz((Flut2D){ (float)ponto.x, (float)ponto.y }, m);
  return (Int2D){ .x = (int32_t)roundf(res.x), .y = (int32_t)roundf(res.y) };
}

Tela criar_tela(uint32_t largura, uint32_t altura) {
  Tela tela = { .largura = 0, .altura = 0, .píxeis = nullptr };
  if (largura == 0 || altura == 0) return tela;

  size_t total = (size_t)largura * (size_t)altura;
  Cor *píxeis = calloc(total, sizeof(Cor));
  if (!píxeis) return tela;

  tela.largura = largura;
  tela.altura = altura;
  tela.píxeis = píxeis;
  return tela;
}

void destruir_tela(Tela *tela) {
  if (!tela) return;
  free(tela->píxeis);
  tela->píxeis = nullptr;
  tela->largura = 0;
  tela->altura = 0;
}

void limpar_tela(Tela *tela, Cor fundo) {
  if (!tela || !tela->píxeis) return;
  size_t total = (size_t)tela->largura * (size_t)tela->altura;
  for (size_t i = 0; i < total; i++) {
    tela->píxeis[i] = fundo;
  }
}

static inline bool ponto_dentro_tela(const Tela *tela, Int2D p) {
  return tela && tela->píxeis &&
         p.x >= 0 && p.y >= 0 &&
         (uint32_t)p.x < tela->largura &&
         (uint32_t)p.y < tela->altura;
}

Cor obter_píxel(const Tela *tela, Int2D ponto) {
  if (!ponto_dentro_tela(tela, ponto)) {
    return (Cor){ 0, 0, 0, 0 };
  }
  return tela->píxeis[(size_t)ponto.y * tela->largura + (size_t)ponto.x];
}

void desenhar_píxel(Tela *tela, Int2D ponto, Cor cor) {
  if (!ponto_dentro_tela(tela, ponto)) return;
  tela->píxeis[(size_t)ponto.y * tela->largura + (size_t)ponto.x] = cor;
}

void desenhar_píxel_alfa(Tela *tela, Int2D ponto, Cor cor_fonte) {
  if (!ponto_dentro_tela(tela, ponto) || cor_fonte.alfa == 0) return;
  if (cor_fonte.alfa == 255) {
    desenhar_píxel(tela, ponto, cor_fonte);
    return;
  }

  size_t idx = (size_t)ponto.y * tela->largura + (size_t)ponto.x;
  Cor dst = tela->píxeis[idx];

  float sa = (float)cor_fonte.alfa / 255.0f;
  float da = (float)dst.alfa / 255.0f;
  float out_a = sa + da * (1.0f - sa);

  if (out_a <= 1e-6f) {
    tela->píxeis[idx] = (Cor){ 0, 0, 0, 0 };
    return;
  }

  float r = ((float)cor_fonte.vermelho * sa + (float)dst.vermelho * da * (1.0f - sa)) / out_a;
  float g = ((float)cor_fonte.verde    * sa + (float)dst.verde    * da * (1.0f - sa)) / out_a;
  float b = ((float)cor_fonte.azul     * sa + (float)dst.azul     * da * (1.0f - sa)) / out_a;

  tela->píxeis[idx] = (Cor){
    .vermelho = (uint8_t)roundf(r),
    .verde    = (uint8_t)roundf(g),
    .azul     = (uint8_t)roundf(b),
    .alfa     = (uint8_t)roundf(out_a * 255.0f)
  };
}

void desenhar_píxel_mistura(Tela *tela, Int2D ponto, Cor cor_fonte, Mistura modo) {
  if (!ponto_dentro_tela(tela, ponto)) return;
  if (modo == MISTURA_NORMAL) {
    desenhar_píxel_alfa(tela, ponto, cor_fonte);
    return;
  }

  size_t idx = (size_t)ponto.y * tela->largura + (size_t)ponto.x;
  Cor dst = tela->píxeis[idx];
  float sa = (float)cor_fonte.alfa / 255.0f;

  uint8_t mr = cor_fonte.vermelho;
  uint8_t mg = cor_fonte.verde;
  uint8_t mb = cor_fonte.azul;

  switch (modo) {
    case MISTURA_ADITIVA:
      mr = limitar_u8((int32_t)dst.vermelho + (int32_t)roundf((float)cor_fonte.vermelho * sa));
      mg = limitar_u8((int32_t)dst.verde    + (int32_t)roundf((float)cor_fonte.verde    * sa));
      mb = limitar_u8((int32_t)dst.azul     + (int32_t)roundf((float)cor_fonte.azul     * sa));
      tela->píxeis[idx] = (Cor){ mr, mg, mb, dst.alfa };
      break;

    case MISTURA_MULTIPLICAR: {
      uint8_t prod_r = (uint8_t)(((uint32_t)cor_fonte.vermelho * dst.vermelho) / 255U);
      uint8_t prod_g = (uint8_t)(((uint32_t)cor_fonte.verde    * dst.verde)    / 255U);
      uint8_t prod_b = (uint8_t)(((uint32_t)cor_fonte.azul     * dst.azul)     / 255U);
      Cor misturada = { prod_r, prod_g, prod_b, cor_fonte.alfa };
      desenhar_píxel_alfa(tela, ponto, misturada);
      break;
    }

    case MISTURA_ECRÃ: {
      uint8_t scr_r = (uint8_t)(255U - (((255U - cor_fonte.vermelho) * (255U - dst.vermelho)) / 255U));
      uint8_t scr_g = (uint8_t)(255U - (((255U - cor_fonte.verde)    * (255U - dst.verde))    / 255U));
      uint8_t scr_b = (uint8_t)(255U - (((255U - cor_fonte.azul)     * (255U - dst.azul))     / 255U));
      Cor misturada = { scr_r, scr_g, scr_b, cor_fonte.alfa };
      desenhar_píxel_alfa(tela, ponto, misturada);
      break;
    }

    default:
      desenhar_píxel_alfa(tela, ponto, cor_fonte);
      break;
  }
}

void desenhar_linha_horizontal(Tela *tela, int32_t y, int32_t x_início, int32_t x_fim, Cor cor) {
  if (!tela || !tela->píxeis || y < 0 || (uint32_t)y >= tela->altura) return;
  if (x_início > x_fim) {
    int32_t tmp = x_início;
    x_início = x_fim;
    x_fim = tmp;
  }
  if (x_fim < 0 || (uint32_t)x_início >= tela->largura) return;
  if (x_início < 0) x_início = 0;
  if ((uint32_t)x_fim >= tela->largura) x_fim = (int32_t)tela->largura - 1;

  Cor *linha = &tela->píxeis[(size_t)y * tela->largura];
  for (int32_t x = x_início; x <= x_fim; x++) {
    linha[x] = cor;
  }
}

void desenhar_linha(Tela *tela, Int2D início, Int2D fim, Cor cor) {
  if (!tela || !tela->píxeis) return;
  int32_t x0 = início.x, y0 = início.y;
  int32_t x1 = fim.x,    y1 = fim.y;

  int32_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int32_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int32_t err = dx + dy;

  while (true) {
    desenhar_píxel(tela, (Int2D){ x0, y0 }, cor);
    if (x0 == x1 && y0 == y1) break;
    int32_t e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; }
    if (e2 <= dx) { err += dx; y0 += sy; }
  }
}

void desenhar_linha_espessa(Tela *tela, Int2D início, Int2D fim, uint32_t espessura, Cor cor) {
  if (!tela || !tela->píxeis || espessura == 0) return;
  if (espessura == 1) {
    desenhar_linha(tela, início, fim, cor);
    return;
  }

  float dx = (float)(fim.x - início.x);
  float dy = (float)(fim.y - início.y);
  float comp = hypotf(dx, dy);
  uint32_t raio = espessura / 2;

  if (comp < 1e-4f) {
    desenhar_círculo_preenchido(tela, início, raio, cor);
    return;
  }

  float nx = -dy / comp;
  float ny =  dx / comp;
  float metade = (float)espessura * 0.5f;

  Int2D quad[4] = {
    { (int32_t)roundf((float)início.x + nx * metade), (int32_t)roundf((float)início.y + ny * metade) },
    { (int32_t)roundf((float)fim.x    + nx * metade), (int32_t)roundf((float)fim.y    + ny * metade) },
    { (int32_t)roundf((float)fim.x    - nx * metade), (int32_t)roundf((float)fim.y    - ny * metade) },
    { (int32_t)roundf((float)início.x - nx * metade), (int32_t)roundf((float)início.y - ny * metade) }
  };

  desenhar_polígono_preenchido(tela, quad, 4, cor);
  desenhar_círculo_preenchido(tela, início, raio, cor);
  desenhar_círculo_preenchido(tela, fim, raio, cor);
}

static void desenhar_píxel_wu(Tela *tela, int32_t x, int32_t y, Cor cor, float brilho) {
  if (brilho <= 0.0f) return;
  Cor c = cor;
  c.alfa = (uint8_t)roundf((float)cor.alfa * limitar_flut(brilho, 0.0f, 1.0f));
  desenhar_píxel_alfa(tela, (Int2D){ x, y }, c);
}

void desenhar_linha_suavizada(Tela *tela, Int2D início, Int2D fim, Cor cor) {
  if (!tela || !tela->píxeis) return;
  float x0 = (float)início.x, y0 = (float)início.y;
  float x1 = (float)fim.x,    y1 = (float)fim.y;

  bool íngreme = fabsf(y1 - y0) > fabsf(x1 - x0);
  if (íngreme) {
    float t;
    t = x0; x0 = y0; y0 = t;
    t = x1; x1 = y1; y1 = t;
  }
  if (x0 > x1) {
    float t;
    t = x0; x0 = x1; x1 = t;
    t = y0; y0 = y1; y1 = t;
  }

  float dx = x1 - x0;
  float dy = y1 - y0;
  float gradiente = (fabsf(dx) < 1e-6f) ? 1.0f : (dy / dx);

  int32_t xpxl1 = (int32_t)roundf(x0);
  int32_t xpxl2 = (int32_t)roundf(x1);
  float intery = y0 + gradiente * ((float)xpxl1 - x0);

  for (int32_t x = xpxl1; x <= xpxl2; x++) {
    int32_t iy = (int32_t)floorf(intery);
    float frac = intery - (float)iy;
    if (íngreme) {
      desenhar_píxel_wu(tela, iy,     x, cor, 1.0f - frac);
      desenhar_píxel_wu(tela, iy + 1, x, cor, frac);
    } else {
      desenhar_píxel_wu(tela, x, iy,     cor, 1.0f - frac);
      desenhar_píxel_wu(tela, x, iy + 1, cor, frac);
    }
    intery += gradiente;
  }
}

void desenhar_polilinha(Tela *tela, const Int2D vértices[], size_t núm_vértices, Cor cor, bool fechada) {
  if (!tela || !vértices || núm_vértices < 2) return;
  for (size_t i = 0; i + 1 < núm_vértices; i++) {
    desenhar_linha(tela, vértices[i], vértices[i + 1], cor);
  }
  if (fechada) {
    desenhar_linha(tela, vértices[núm_vértices - 1], vértices[0], cor);
  }
}

void desenhar_curva_bezier_quadrática(Tela *tela, Int2D p0, Int2D p1, Int2D p2, Cor cor) {
  if (!tela) return;
  float comp = hypotf((float)(p1.x - p0.x), (float)(p1.y - p0.y)) +
               hypotf((float)(p2.x - p1.x), (float)(p2.y - p1.y));
  uint32_t passos = (uint32_t)fmaxf(16.0f, comp * 0.5f);

  Int2D anterior = p0;
  for (uint32_t i = 1; i <= passos; i++) {
    float t = (float)i / (float)passos;
    float u = 1.0f - t;
    float x = u * u * (float)p0.x + 2.0f * u * t * (float)p1.x + t * t * (float)p2.x;
    float y = u * u * (float)p0.y + 2.0f * u * t * (float)p1.y + t * t * (float)p2.y;
    Int2D atual = { (int32_t)roundf(x), (int32_t)roundf(y) };
    desenhar_linha(tela, anterior, atual, cor);
    anterior = atual;
  }
}

void desenhar_curva_bezier_cúbica(Tela *tela, Int2D p0, Int2D p1, Int2D p2, Int2D p3, Cor cor) {
  if (!tela) return;
  float comp = hypotf((float)(p1.x - p0.x), (float)(p1.y - p0.y)) +
               hypotf((float)(p2.x - p1.x), (float)(p2.y - p1.y)) +
               hypotf((float)(p3.x - p2.x), (float)(p3.y - p2.y));
  uint32_t passos = (uint32_t)fmaxf(24.0f, comp * 0.5f);

  Int2D anterior = p0;
  for (uint32_t i = 1; i <= passos; i++) {
    float t = (float)i / (float)passos;
    float u = 1.0f - t;
    float u2 = u * u, u3 = u2 * u;
    float t2 = t * t, t3 = t2 * t;
    float x = u3 * (float)p0.x + 3.0f * u2 * t * (float)p1.x + 3.0f * u * t2 * (float)p2.x + t3 * (float)p3.x;
    float y = u3 * (float)p0.y + 3.0f * u2 * t * (float)p1.y + 3.0f * u * t2 * (float)p2.y + t3 * (float)p3.y;
    Int2D atual = { (int32_t)roundf(x), (int32_t)roundf(y) };
    desenhar_linha(tela, anterior, atual, cor);
    anterior = atual;
  }
}

static void subdividir_bezier_recursivo(Tela *tela, Flut2D p0, Flut2D p1, Flut2D p2, Flut2D p3, Cor cor, float tol_sq, int profundidade) {
  float dx = p3.x - p0.x;
  float dy = p3.y - p0.y;
  float d2 = fabsf((p1.x - p3.x) * dy - (p1.y - p3.y) * dx);
  float d3 = fabsf((p2.x - p3.x) * dy - (p2.y - p3.y) * dx);

  if ((d2 + d3) * (d2 + d3) <= tol_sq * (dx * dx + dy * dy) || profundidade >= 12) {
    desenhar_linha(tela,
      (Int2D){ (int32_t)roundf(p0.x), (int32_t)roundf(p0.y) },
      (Int2D){ (int32_t)roundf(p3.x), (int32_t)roundf(p3.y) },
      cor);
    return;
  }

  Flut2D p01   = { (p0.x + p1.x) * 0.5f, (p0.y + p1.y) * 0.5f };
  Flut2D p12   = { (p1.x + p2.x) * 0.5f, (p1.y + p2.y) * 0.5f };
  Flut2D p23   = { (p2.x + p3.x) * 0.5f, (p2.y + p3.y) * 0.5f };
  Flut2D p012  = { (p01.x + p12.x) * 0.5f, (p01.y + p12.y) * 0.5f };
  Flut2D p123  = { (p12.x + p23.x) * 0.5f, (p12.y + p23.y) * 0.5f };
  Flut2D p0123 = { (p012.x + p123.x) * 0.5f, (p012.y + p123.y) * 0.5f };

  subdividir_bezier_recursivo(tela, p0, p01, p012, p0123, cor, tol_sq, profundidade + 1);
  subdividir_bezier_recursivo(tela, p0123, p123, p23, p3, cor, tol_sq, profundidade + 1);
}

void desenhar_curva_bezier_adaptativa(Tela *tela, Int2D p0, Int2D p1, Int2D p2, Int2D p3, Cor cor, float tolerância_píxeis) {
  if (!tela) return;
  float tol = fmaxf(0.1f, tolerância_píxeis);
  subdividir_bezier_recursivo(tela,
    (Flut2D){ (float)p0.x, (float)p0.y },
    (Flut2D){ (float)p1.x, (float)p1.y },
    (Flut2D){ (float)p2.x, (float)p2.y },
    (Flut2D){ (float)p3.x, (float)p3.y },
    cor, tol * tol, 0);
}

void desenhar_spline_catmull_rom(Tela *tela, const Int2D pontos[], size_t núm_pontos, uint32_t subdivisões_por_segmento, Cor cor) {
  if (!tela || !pontos || núm_pontos < 2 || subdivisões_por_segmento == 0) return;

  for (size_t i = 0; i + 1 < núm_pontos; i++) {
    Int2D p0 = (i == 0) ? pontos[0] : pontos[i - 1];
    Int2D p1 = pontos[i];
    Int2D p2 = pontos[i + 1];
    Int2D p3 = (i + 2 < núm_pontos) ? pontos[i + 2] : pontos[núm_pontos - 1];

    Int2D anterior = p1;
    for (uint32_t s = 1; s <= subdivisões_por_segmento; s++) {
      float t = (float)s / (float)subdivisões_por_segmento;
      float t2 = t * t;
      float t3 = t2 * t;

      float x = 0.5f * ((2.0f * (float)p1.x) +
                       (-(float)p0.x + (float)p2.x) * t +
                       (2.0f * (float)p0.x - 5.0f * (float)p1.x + 4.0f * (float)p2.x - (float)p3.x) * t2 +
                       (-(float)p0.x + 3.0f * (float)p1.x - 3.0f * (float)p2.x + (float)p3.x) * t3);

      float y = 0.5f * ((2.0f * (float)p1.y) +
                       (-(float)p0.y + (float)p2.y) * t +
                       (2.0f * (float)p0.y - 5.0f * (float)p1.y + 4.0f * (float)p2.y - (float)p3.y) * t2 +
                       (-(float)p0.y + 3.0f * (float)p1.y - 3.0f * (float)p2.y + (float)p3.y) * t3);

      Int2D atual = { (int32_t)roundf(x), (int32_t)roundf(y) };
      desenhar_linha(tela, anterior, atual, cor);
      anterior = atual;
    }
  }
}

void desenhar_triângulo(Tela *tela, Int2D p1, Int2D p2, Int2D p3, Cor cor) {
  desenhar_linha(tela, p1, p2, cor);
  desenhar_linha(tela, p2, p3, cor);
  desenhar_linha(tela, p3, p1, cor);
}

void desenhar_triângulo_preenchido(Tela *tela, Int2D p1, Int2D p2, Int2D p3, Cor cor) {
  if (!tela || !tela->píxeis) return;

  if (p1.y > p2.y) { Int2D t = p1; p1 = p2; p2 = t; }
  if (p1.y > p3.y) { Int2D t = p1; p1 = p3; p3 = t; }
  if (p2.y > p3.y) { Int2D t = p2; p2 = p3; p3 = t; }

  int32_t altura_total = p3.y - p1.y;
  if (altura_total == 0) {
    int32_t min_x = p1.x < p2.x ? (p1.x < p3.x ? p1.x : p3.x) : (p2.x < p3.x ? p2.x : p3.x);
    int32_t max_x = p1.x > p2.x ? (p1.x > p3.x ? p1.x : p3.x) : (p2.x > p3.x ? p2.x : p3.x);
    desenhar_linha_horizontal(tela, p1.y, min_x, max_x, cor);
    return;
  }

  for (int32_t i = 0; i <= altura_total; i++) {
    bool segunda_metade = (i > p2.y - p1.y) || (p2.y == p1.y);
    int32_t altura_seg = segunda_metade ? (p3.y - p2.y) : (p2.y - p1.y);
    if (altura_seg == 0) continue;

    float alfa = (float)i / (float)altura_total;
    float beta = (float)(i - (segunda_metade ? (p2.y - p1.y) : 0)) / (float)altura_seg;

    int32_t ax = p1.x + (int32_t)roundf((float)(p3.x - p1.x) * alfa);
    int32_t bx = segunda_metade
      ? p2.x + (int32_t)roundf((float)(p3.x - p2.x) * beta)
      : p1.x + (int32_t)roundf((float)(p2.x - p1.x) * beta);

    desenhar_linha_horizontal(tela, p1.y + i, ax, bx, cor);
  }
}

void desenhar_retângulo(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, Cor cor) {
  if (!tela || largura == 0 || altura == 0) return;
  int32_t x0 = superior_esquerdo.x;
  int32_t y0 = superior_esquerdo.y;
  int32_t x1 = x0 + (int32_t)largura - 1;
  int32_t y1 = y0 + (int32_t)altura - 1;

  desenhar_linha_horizontal(tela, y0, x0, x1, cor);
  desenhar_linha_horizontal(tela, y1, x0, x1, cor);
  desenhar_linha(tela, (Int2D){ x0, y0 }, (Int2D){ x0, y1 }, cor);
  desenhar_linha(tela, (Int2D){ x1, y0 }, (Int2D){ x1, y1 }, cor);
}

void desenhar_retângulo_preenchido(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, Cor cor) {
  if (!tela || !tela->píxeis || largura == 0 || altura == 0) return;
  int32_t x_ini = superior_esquerdo.x;
  int32_t y_ini = superior_esquerdo.y;
  int32_t x_fim = x_ini + (int32_t)largura - 1;
  int32_t y_fim = y_ini + (int32_t)altura - 1;

  for (int32_t y = y_ini; y <= y_fim; y++) {
    desenhar_linha_horizontal(tela, y, x_ini, x_fim, cor);
  }
}

void desenhar_retângulo_arredondado(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, uint32_t raio, Cor cor) {
  if (!tela || largura == 0 || altura == 0) return;
  uint32_t max_r = (largura < altura ? largura : altura) / 2;
  if (raio > max_r) raio = max_r;
  if (raio == 0) {
    desenhar_retângulo(tela, superior_esquerdo, largura, altura, cor);
    return;
  }

  int32_t x0 = superior_esquerdo.x;
  int32_t y0 = superior_esquerdo.y;
  int32_t x1 = x0 + (int32_t)largura - 1;
  int32_t y1 = y0 + (int32_t)altura - 1;
  int32_t r  = (int32_t)raio;

  desenhar_linha_horizontal(tela, y0, x0 + r, x1 - r, cor);
  desenhar_linha_horizontal(tela, y1, x0 + r, x1 - r, cor);
  desenhar_linha(tela, (Int2D){ x0, y0 + r }, (Int2D){ x0, y1 - r }, cor);
  desenhar_linha(tela, (Int2D){ x1, y0 + r }, (Int2D){ x1, y1 - r }, cor);

  int32_t x = r, y = 0, err = 1 - r;
  while (x >= y) {
    desenhar_píxel(tela, (Int2D){ x0 + r - x, y0 + r - y }, cor);
    desenhar_píxel(tela, (Int2D){ x0 + r - y, y0 + r - x }, cor);
    desenhar_píxel(tela, (Int2D){ x1 - r + x, y0 + r - y }, cor);
    desenhar_píxel(tela, (Int2D){ x1 - r + y, y0 + r - x }, cor);
    desenhar_píxel(tela, (Int2D){ x0 + r - x, y1 - r + y }, cor);
    desenhar_píxel(tela, (Int2D){ x0 + r - y, y1 - r + x }, cor);
    desenhar_píxel(tela, (Int2D){ x1 - r + x, y1 - r + y }, cor);
    desenhar_píxel(tela, (Int2D){ x1 - r + y, y1 - r + x }, cor);
    y++;
    if (err < 0) {
      err += 2 * y + 1;
    } else {
      x--;
      err += 2 * (y - x) + 1;
    }
  }
}

void desenhar_retângulo_arredondado_preenchido(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, uint32_t raio, Cor cor) {
  if (!tela || largura == 0 || altura == 0) return;
  uint32_t max_r = (largura < altura ? largura : altura) / 2;
  if (raio > max_r) raio = max_r;
  if (raio == 0) {
    desenhar_retângulo_preenchido(tela, superior_esquerdo, largura, altura, cor);
    return;
  }

  int32_t x0 = superior_esquerdo.x;
  int32_t y0 = superior_esquerdo.y;
  int32_t x1 = x0 + (int32_t)largura - 1;
  int32_t y1 = y0 + (int32_t)altura - 1;
  int32_t r  = (int32_t)raio;

  for (int32_t y = y0 + r; y <= y1 - r; y++) {
    desenhar_linha_horizontal(tela, y, x0, x1, cor);
  }

  int32_t x = r, y = 0, err = 1 - r;
  while (x >= y) {
    desenhar_linha_horizontal(tela, y0 + r - y, x0 + r - x, x1 - r + x, cor);
    desenhar_linha_horizontal(tela, y0 + r - x, x0 + r - y, x1 - r + y, cor);
    desenhar_linha_horizontal(tela, y1 - r + y, x0 + r - x, x1 - r + x, cor);
    desenhar_linha_horizontal(tela, y1 - r + x, x0 + r - y, x1 - r + y, cor);
    y++;
    if (err < 0) {
      err += 2 * y + 1;
    } else {
      x--;
      err += 2 * (y - x) + 1;
    }
  }
}

void desenhar_gradiente_linear(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, Cor cor_início, Cor cor_fim, bool vertical) {
  if (!tela || !tela->píxeis || largura == 0 || altura == 0) return;

  if (vertical) {
    for (uint32_t dy = 0; dy < altura; dy++) {
      float t = (altura > 1) ? ((float)dy / (float)(altura - 1)) : 0.0f;
      Cor c = interpolar_cor(cor_início, cor_fim, t);
      desenhar_linha_horizontal(tela, superior_esquerdo.y + (int32_t)dy, superior_esquerdo.x, superior_esquerdo.x + (int32_t)largura - 1, c);
    }
  } else {
    for (uint32_t dx = 0; dx < largura; dx++) {
      float t = (largura > 1) ? ((float)dx / (float)(largura - 1)) : 0.0f;
      Cor c = interpolar_cor(cor_início, cor_fim, t);
      int32_t x = superior_esquerdo.x + (int32_t)dx;
      for (uint32_t dy = 0; dy < altura; dy++) {
        desenhar_píxel(tela, (Int2D){ x, superior_esquerdo.y + (int32_t)dy }, c);
      }
    }
  }
}

void desenhar_polígono(Tela *tela, const Int2D vértices[], size_t núm_vértices, Cor cor) {
  desenhar_polilinha(tela, vértices, núm_vértices, cor, true);
}

void desenhar_polígono_preenchido(Tela *tela, const Int2D vértices[], size_t núm_vértices, Cor cor) {
  if (!tela || !tela->píxeis || !vértices || núm_vértices < 3) return;

  int32_t min_y = vértices[0].y, max_y = vértices[0].y;
  for (size_t i = 1; i < núm_vértices; i++) {
    if (vértices[i].y < min_y) min_y = vértices[i].y;
    if (vértices[i].y > max_y) max_y = vértices[i].y;
  }

  if (min_y < 0) min_y = 0;
  if (max_y >= (int32_t)tela->altura) max_y = (int32_t)tela->altura - 1;

  int32_t *nós = malloc(núm_vértices * sizeof(int32_t));
  if (!nós) return;

  for (int32_t y = min_y; y <= max_y; y++) {
    size_t num_nós = 0;
    size_t j = núm_vértices - 1;
    for (size_t i = 0; i < núm_vértices; i++) {
      if ((vértices[i].y < y && vértices[j].y >= y) ||
          (vértices[j].y < y && vértices[i].y >= y)) {
        float x = (float)vértices[i].x +
                  ((float)(y - vértices[i].y) / (float)(vértices[j].y - vértices[i].y)) *
                  (float)(vértices[j].x - vértices[i].x);
        nós[num_nós++] = (int32_t)roundf(x);
      }
      j = i;
    }

    qsort(nós, num_nós, sizeof(int32_t), comparar_int32);

    for (size_t i = 0; i + 1 < num_nós; i += 2) {
      desenhar_linha_horizontal(tela, y, nós[i], nós[i + 1], cor);
    }
  }

  free(nós);
}

void desenhar_círculo(Tela *tela, Int2D centro, uint32_t raio, Cor cor) {
  if (!tela || !tela->píxeis) return;
  int32_t x = (int32_t)raio;
  int32_t y = 0;
  int32_t err = 1 - x;

  while (x >= y) {
    desenhar_píxel(tela, (Int2D){ centro.x + x, centro.y + y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x + y, centro.y + x }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - y, centro.y + x }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - x, centro.y + y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - x, centro.y - y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - y, centro.y - x }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x + y, centro.y - x }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x + x, centro.y - y }, cor);

    y++;
    if (err < 0) {
      err += 2 * y + 1;
    } else {
      x--;
      err += 2 * (y - x) + 1;
    }
  }
}

void desenhar_círculo_preenchido(Tela *tela, Int2D centro, uint32_t raio, Cor cor) {
  if (!tela || !tela->píxeis) return;
  int32_t x = (int32_t)raio;
  int32_t y = 0;
  int32_t err = 1 - x;

  while (x >= y) {
    desenhar_linha_horizontal(tela, centro.y + y, centro.x - x, centro.x + x, cor);
    desenhar_linha_horizontal(tela, centro.y - y, centro.x - x, centro.x + x, cor);
    desenhar_linha_horizontal(tela, centro.y + x, centro.x - y, centro.x + y, cor);
    desenhar_linha_horizontal(tela, centro.y - x, centro.x - y, centro.x + y, cor);

    y++;
    if (err < 0) {
      err += 2 * y + 1;
    } else {
      x--;
      err += 2 * (y - x) + 1;
    }
  }
}

void desenhar_gradiente_radial(Tela *tela, Int2D centro, uint32_t raio, Cor cor_centro, Cor cor_periferia) {
  if (!tela || !tela->píxeis) return;
  if (raio == 0) {
    desenhar_píxel(tela, centro, cor_centro);
    return;
  }

  int32_t r = (int32_t)raio;
  float r_flut = (float)raio;

  for (int32_t dy = -r; dy <= r; dy++) {
    for (int32_t dx = -r; dx <= r; dx++) {
      float dist = hypotf((float)dx, (float)dy);
      if (dist <= r_flut) {
        Cor c = interpolar_cor(cor_centro, cor_periferia, dist / r_flut);
        desenhar_píxel(tela, (Int2D){ centro.x + dx, centro.y + dy }, c);
      }
    }
  }
}

static bool ângulo_no_intervalo(float ang, float inicio, float fim) {
  ang    = normalizar_radianos(ang);
  inicio = normalizar_radianos(inicio);
  fim    = normalizar_radianos(fim);
  if (inicio <= fim) {
    return ang >= inicio && ang <= fim;
  }
  return ang >= inicio || ang <= fim;
}

void desenhar_arco(Tela *tela, Int2D centro, uint32_t raio, float ângulo_início, float ângulo_fim, Cor cor) {
  if (!tela || !tela->píxeis || raio == 0) return;

  float diff = normalizar_radianos(ângulo_fim - ângulo_início);
  if (diff < 1e-5f && fabsf(ângulo_fim - ângulo_início) > 1e-5f) {
    desenhar_círculo(tela, centro, raio, cor);
    return;
  }

  uint32_t passos = (uint32_t)fmaxf(16.0f, diff * (float)raio * 1.5f);
  Int2D anterior = {
    centro.x + (int32_t)roundf(cosf(ângulo_início) * (float)raio),
    centro.y + (int32_t)roundf(sinf(ângulo_início) * (float)raio)
  };

  for (uint32_t i = 1; i <= passos; i++) {
    float t = (float)i / (float)passos;
    float a = ângulo_início + diff * t;
    Int2D atual = {
      centro.x + (int32_t)roundf(cosf(a) * (float)raio),
      centro.y + (int32_t)roundf(sinf(a) * (float)raio)
    };
    desenhar_linha(tela, anterior, atual, cor);
    anterior = atual;
  }
}

void desenhar_setor_circular(Tela *tela, Int2D centro, uint32_t raio, float ângulo_início, float ângulo_fim, Cor cor) {
  if (!tela || !tela->píxeis) return;
  if (raio == 0) {
    desenhar_píxel(tela, centro, cor);
    return;
  }

  float diff = normalizar_radianos(ângulo_fim - ângulo_início);
  if (diff < 1e-5f && fabsf(ângulo_fim - ângulo_início) > 1e-5f) {
    desenhar_círculo_preenchido(tela, centro, raio, cor);
    return;
  }

  int32_t r = (int32_t)raio;
  int32_t r_sq = r * r;

  for (int32_t dy = -r; dy <= r; dy++) {
    for (int32_t dx = -r; dx <= r; dx++) {
      if (dx * dx + dy * dy <= r_sq) {
        if (dx == 0 && dy == 0) {
          desenhar_píxel(tela, centro, cor);
        } else {
          float ang = atan2f((float)dy, (float)dx);
          if (ângulo_no_intervalo(ang, ângulo_início, ângulo_fim)) {
            desenhar_píxel(tela, (Int2D){ centro.x + dx, centro.y + dy }, cor);
          }
        }
      }
    }
  }
}

void desenhar_elipse(Tela *tela, Int2D centro, uint32_t rx, uint32_t ry, Cor cor) {
  if (!tela || !tela->píxeis) return;
  if (rx == 0 && ry == 0) { desenhar_píxel(tela, centro, cor); return; }
  if (rx == 0) { desenhar_linha(tela, (Int2D){ centro.x, centro.y - (int32_t)ry }, (Int2D){ centro.x, centro.y + (int32_t)ry }, cor); return; }
  if (ry == 0) { desenhar_linha_horizontal(tela, centro.y, centro.x - (int32_t)rx, centro.x + (int32_t)rx, cor); return; }

  int64_t rx2 = (int64_t)rx * rx;
  int64_t ry2 = (int64_t)ry * ry;
  int64_t x = 0, y = (int64_t)ry;
  int64_t px = 0, py = 2 * rx2 * y;

  int64_t p = (int64_t)llround((double)ry2 - (double)(rx2 * ry) + 0.25 * (double)rx2);
  while (px < py) {
    desenhar_píxel(tela, (Int2D){ centro.x + (int32_t)x, centro.y + (int32_t)y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - (int32_t)x, centro.y + (int32_t)y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x + (int32_t)x, centro.y - (int32_t)y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - (int32_t)x, centro.y - (int32_t)y }, cor);
    x++;
    px += 2 * ry2;
    if (p < 0) {
      p += ry2 + px;
    } else {
      y--;
      py -= 2 * rx2;
      p += ry2 + px - py;
    }
  }

  p = (int64_t)llround((double)ry2 * ((double)x + 0.5) * ((double)x + 0.5) + (double)rx2 * ((double)y - 1) * ((double)y - 1) - (double)(rx2 * ry2));
  while (y >= 0) {
    desenhar_píxel(tela, (Int2D){ centro.x + (int32_t)x, centro.y + (int32_t)y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - (int32_t)x, centro.y + (int32_t)y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x + (int32_t)x, centro.y - (int32_t)y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - (int32_t)x, centro.y - (int32_t)y }, cor);
    y--;
    py -= 2 * rx2;
    if (p > 0) {
      p += rx2 - py;
    } else {
      x++;
      px += 2 * ry2;
      p += rx2 - py + px;
    }
  }
}

void desenhar_elipse_preenchida(Tela *tela, Int2D centro, uint32_t rx, uint32_t ry, Cor cor) {
  if (!tela || !tela->píxeis) return;
  if (rx == 0 && ry == 0) { desenhar_píxel(tela, centro, cor); return; }
  if (rx == 0) { desenhar_linha(tela, (Int2D){ centro.x, centro.y - (int32_t)ry }, (Int2D){ centro.x, centro.y + (int32_t)ry }, cor); return; }
  if (ry == 0) { desenhar_linha_horizontal(tela, centro.y, centro.x - (int32_t)rx, centro.x + (int32_t)rx, cor); return; }

  int64_t rx2 = (int64_t)rx * rx;
  int64_t ry2 = (int64_t)ry * ry;
  int64_t x = 0, y = (int64_t)ry;
  int64_t px = 0, py = 2 * rx2 * y;

  int64_t p = (int64_t)llround((double)ry2 - (double)(rx2 * ry) + 0.25 * (double)rx2);
  while (px < py) {
    desenhar_linha_horizontal(tela, centro.y + (int32_t)y, centro.x - (int32_t)x, centro.x + (int32_t)x, cor);
    desenhar_linha_horizontal(tela, centro.y - (int32_t)y, centro.x - (int32_t)x, centro.x + (int32_t)x, cor);
    x++;
    px += 2 * ry2;
    if (p < 0) {
      p += ry2 + px;
    } else {
      y--;
      py -= 2 * rx2;
      p += ry2 + px - py;
    }
  }

  p = (int64_t)llround((double)ry2 * ((double)x + 0.5) * ((double)x + 0.5) + (double)rx2 * ((double)y - 1) * ((double)y - 1) - (double)(rx2 * ry2));
  while (y >= 0) {
    desenhar_linha_horizontal(tela, centro.y + (int32_t)y, centro.x - (int32_t)x, centro.x + (int32_t)x, cor);
    desenhar_linha_horizontal(tela, centro.y - (int32_t)y, centro.x - (int32_t)x, centro.x + (int32_t)x, cor);
    y--;
    py -= 2 * rx2;
    if (p > 0) {
      p += rx2 - py;
    } else {
      x++;
      px += 2 * ry2;
      p += rx2 - py + px;
    }
  }
}

void preencher_área(Tela *tela, Int2D semente, Cor nova_cor) {
  if (!ponto_dentro_tela(tela, semente)) return;
  Cor cor_alvo = obter_píxel(tela, semente);
  if (cores_iguais(cor_alvo, nova_cor)) return;

  size_t cap = (size_t)tela->largura * (size_t)tela->altura;
  Int2D *pilha = malloc(cap * sizeof(Int2D));
  if (!pilha) return;

  size_t topo = 0;
  pilha[topo++] = semente;

  while (topo > 0) {
    Int2D p = pilha[--topo];
    if (!ponto_dentro_tela(tela, p)) continue;
    if (!cores_iguais(obter_píxel(tela, p), cor_alvo)) continue;

    desenhar_píxel(tela, p, nova_cor);

    if (p.x + 1 < (int32_t)tela->largura)  pilha[topo++] = (Int2D){ p.x + 1, p.y };
    if (p.x > 0)                           pilha[topo++] = (Int2D){ p.x - 1, p.y };
    if (p.y + 1 < (int32_t)tela->altura)   pilha[topo++] = (Int2D){ p.x, p.y + 1 };
    if (p.y > 0)                           pilha[topo++] = (Int2D){ p.x, p.y - 1 };
  }

  free(pilha);
}

void desenhar_imagem(Tela *destino, const Tela *origem, Int2D posição) {
  if (!destino || !destino->píxeis || !origem || !origem->píxeis) return;

  for (uint32_t sy = 0; sy < origem->altura; sy++) {
    int32_t dy = posição.y + (int32_t)sy;
    if (dy < 0 || (uint32_t)dy >= destino->altura) continue;

    for (uint32_t sx = 0; sx < origem->largura; sx++) {
      int32_t dx = posição.x + (int32_t)sx;
      if (dx < 0 || (uint32_t)dx >= destino->largura) continue;

      Cor c = origem->píxeis[(size_t)sy * origem->largura + (size_t)sx];
      desenhar_píxel_alfa(destino, (Int2D){ dx, dy }, c);
    }
  }
}

void desenhar_imagem_escala(Tela *destino, const Tela *origem, Int2D posição, uint32_t largura_nova, uint32_t altura_nova) {
  if (!destino || !destino->píxeis || !origem || !origem->píxeis || largura_nova == 0 || altura_nova == 0) return;

  for (uint32_t y = 0; y < altura_nova; y++) {
    int32_t dy = posição.y + (int32_t)y;
    if (dy < 0 || (uint32_t)dy >= destino->altura) continue;
    uint32_t sy = (uint32_t)(((uint64_t)y * origem->altura) / altura_nova);

    for (uint32_t x = 0; x < largura_nova; x++) {
      int32_t dx = posição.x + (int32_t)x;
      if (dx < 0 || (uint32_t)dx >= destino->largura) continue;
      uint32_t sx = (uint32_t)(((uint64_t)x * origem->largura) / largura_nova);

      Cor c = origem->píxeis[(size_t)sy * origem->largura + (size_t)sx];
      desenhar_píxel_alfa(destino, (Int2D){ dx, dy }, c);
    }
  }
}

void desenhar_imagem_rodada(Tela *destino, const Tela *origem, Int2D posição, float radianos, Int2D pivô) {
  if (!destino || !destino->píxeis || !origem || !origem->píxeis) return;

  Matriz3x3 t_origem, rot, t_destino, temp, m;
  criar_matriz_translação(&t_origem, -(float)pivô.x, -(float)pivô.y);
  criar_matriz_rotação(&rot, radianos);
  criar_matriz_translação(&t_destino, (float)posição.x, (float)posição.y);

  multiplicar_matrizes(&temp, &rot, &t_origem);
  multiplicar_matrizes(&m, &t_destino, &temp);
  desenhar_imagem_matriz(destino, origem, m);
}

void desenhar_subimagem(Tela *destino, const Tela *origem, Int2D pos_destino, Int2D pos_origem, uint32_t larg_sub, uint32_t alt_sub) {
  if (!destino || !destino->píxeis || !origem || !origem->píxeis) return;

  for (uint32_t y = 0; y < alt_sub; y++) {
    int32_t sy = pos_origem.y + (int32_t)y;
    int32_t dy = pos_destino.y + (int32_t)y;
    if (sy < 0 || (uint32_t)sy >= origem->altura) continue;
    if (dy < 0 || (uint32_t)dy >= destino->altura) continue;

    for (uint32_t x = 0; x < larg_sub; x++) {
      int32_t sx = pos_origem.x + (int32_t)x;
      int32_t dx = pos_destino.x + (int32_t)x;
      if (sx < 0 || (uint32_t)sx >= origem->largura) continue;
      if (dx < 0 || (uint32_t)dx >= destino->largura) continue;

      Cor c = origem->píxeis[(size_t)sy * origem->largura + (size_t)sx];
      desenhar_píxel_alfa(destino, (Int2D){ dx, dy }, c);
    }
  }
}

void desenhar_imagem_mistura(Tela *destino, const Tela *origem, Int2D posição, Mistura modo) {
  if (!destino || !destino->píxeis || !origem || !origem->píxeis) return;

  for (uint32_t sy = 0; sy < origem->altura; sy++) {
    int32_t dy = posição.y + (int32_t)sy;
    if (dy < 0 || (uint32_t)dy >= destino->altura) continue;

    for (uint32_t sx = 0; sx < origem->largura; sx++) {
      int32_t dx = posição.x + (int32_t)sx;
      if (dx < 0 || (uint32_t)dx >= destino->largura) continue;

      Cor c = origem->píxeis[(size_t)sy * origem->largura + (size_t)sx];
      desenhar_píxel_mistura(destino, (Int2D){ dx, dy }, c, modo);
    }
  }
}

void desenhar_imagem_matriz(Tela *destino, const Tela *origem, Matriz3x3 m) {
  if (!destino || !destino->píxeis || !origem || !origem->píxeis) return;

  Matriz3x3 inv;
  if (!inverter_matriz(&inv, &m)) return;

  Flut2D cantos[4] = {
    transformar_flut2d_matriz((Flut2D){ 0.0f, 0.0f }, &m),
    transformar_flut2d_matriz((Flut2D){ (float)origem->largura, 0.0f }, &m),
    transformar_flut2d_matriz((Flut2D){ 0.0f, (float)origem->altura }, &m),
    transformar_flut2d_matriz((Flut2D){ (float)origem->largura, (float)origem->altura }, &m)
  };

  float min_x = cantos[0].x, max_x = cantos[0].x;
  float min_y = cantos[0].y, max_y = cantos[0].y;
  for (int i = 1; i < 4; i++) {
    if (cantos[i].x < min_x) min_x = cantos[i].x;
    if (cantos[i].x > max_x) max_x = cantos[i].x;
    if (cantos[i].y < min_y) min_y = cantos[i].y;
    if (cantos[i].y > max_y) max_y = cantos[i].y;
  }

  int32_t x0 = (int32_t)floorf(min_x);
  int32_t y0 = (int32_t)floorf(min_y);
  int32_t x1 = (int32_t)ceilf(max_x);
  int32_t y1 = (int32_t)ceilf(max_y);

  if (x0 < 0) x0 = 0;
  if (y0 < 0) y0 = 0;
  if (x1 >= (int32_t)destino->largura) x1 = (int32_t)destino->largura - 1;
  if (y1 >= (int32_t)destino->altura)  y1 = (int32_t)destino->altura - 1;

  for (int32_t dy = y0; dy <= y1; dy++) {
    for (int32_t dx = x0; dx <= x1; dx++) {
      Flut2D src = transformar_flut2d_matriz((Flut2D){ (float)dx + 0.5f, (float)dy + 0.5f }, &inv);
      int32_t sx = (int32_t)floorf(src.x);
      int32_t sy = (int32_t)floorf(src.y);

      if (sx >= 0 && (uint32_t)sx < origem->largura &&
          sy >= 0 && (uint32_t)sy < origem->altura) {
        Cor c = origem->píxeis[(size_t)sy * origem->largura + (size_t)sx];
        desenhar_píxel_alfa(destino, (Int2D){ dx, dy }, c);
      }
    }
  }
}

void aplicar_matriz_convolução(Tela *tela, Matriz3x3 núcleo, float divisor, float viés) {
  if (!tela || !tela->píxeis) return;
  if (fabsf(divisor) < 1e-6f) divisor = 1.0f;

  size_t total = (size_t)tela->largura * (size_t)tela->altura;
  Cor *cópia = malloc(total * sizeof(Cor));
  if (!cópia) return;
  memcpy(cópia, tela->píxeis, total * sizeof(Cor));

  for (uint32_t y = 0; y < tela->altura; y++) {
    for (uint32_t x = 0; x < tela->largura; x++) {
      float soma_r = 0.0f, soma_g = 0.0f, soma_b = 0.0f;

      for (int ky = -1; ky <= 1; ky++) {
        int32_t py = (int32_t)y + ky;
        if (py < 0) py = 0;
        if (py >= (int32_t)tela->altura) py = (int32_t)tela->altura - 1;

        for (int kx = -1; kx <= 1; kx++) {
          int32_t px = (int32_t)x + kx;
          if (px < 0) px = 0;
          if (px >= (int32_t)tela->largura) px = (int32_t)tela->largura - 1;

          float peso = núcleo.m[ky + 1][kx + 1];
          Cor c = cópia[(size_t)py * tela->largura + (size_t)px];
          soma_r += (float)c.vermelho * peso;
          soma_g += (float)c.verde    * peso;
          soma_b += (float)c.azul     * peso;
        }
      }

      size_t idx = (size_t)y * tela->largura + (size_t)x;
      tela->píxeis[idx] = (Cor){
        .vermelho = limitar_u8((int32_t)roundf(soma_r / divisor + viés)),
        .verde    = limitar_u8((int32_t)roundf(soma_g / divisor + viés)),
        .azul     = limitar_u8((int32_t)roundf(soma_b / divisor + viés)),
        .alfa     = cópia[idx].alfa
      };
    }
  }

  free(cópia);
}

void aplicar_desfoque_gaussiano(Tela *tela, uint32_t raio) {
  if (!tela || !tela->píxeis || raio == 0) return;

  int32_t r = (int32_t)raio;
  size_t tam_núcleo = (size_t)(2 * r + 1);
  float *núcleo = malloc(tam_núcleo * sizeof(float));
  size_t total = (size_t)tela->largura * (size_t)tela->altura;
  Cor *temp = malloc(total * sizeof(Cor));

  if (!núcleo || !temp) {
    free(núcleo);
    free(temp);
    return;
  }

  float sigma = fmaxf((float)raio / 2.0f, 0.5f);
  float soma_pesos = 0.0f;
  for (int32_t i = -r; i <= r; i++) {
    float peso = expf(-((float)(i * i)) / (2.0f * sigma * sigma));
    núcleo[i + r] = peso;
    soma_pesos += peso;
  }
  for (size_t i = 0; i < tam_núcleo; i++) {
    núcleo[i] /= soma_pesos;
  }

  /* Passagem horizontal */
  for (uint32_t y = 0; y < tela->altura; y++) {
    for (uint32_t x = 0; x < tela->largura; x++) {
      float sr = 0.0f, sg = 0.0f, sb = 0.0f, sa = 0.0f;
      for (int32_t k = -r; k <= r; k++) {
        int32_t px = (int32_t)x + k;
        if (px < 0) px = 0;
        if (px >= (int32_t)tela->largura) px = (int32_t)tela->largura - 1;

        Cor c = tela->píxeis[(size_t)y * tela->largura + (size_t)px];
        float w = núcleo[k + r];
        sr += (float)c.vermelho * w;
        sg += (float)c.verde    * w;
        sb += (float)c.azul     * w;
        sa += (float)c.alfa     * w;
      }
      temp[(size_t)y * tela->largura + (size_t)x] = (Cor){
        (uint8_t)roundf(sr), (uint8_t)roundf(sg), (uint8_t)roundf(sb), (uint8_t)roundf(sa)
      };
    }
  }

  /* Passagem vertical */
  for (uint32_t y = 0; y < tela->altura; y++) {
    for (uint32_t x = 0; x < tela->largura; x++) {
      float sr = 0.0f, sg = 0.0f, sb = 0.0f, sa = 0.0f;
      for (int32_t k = -r; k <= r; k++) {
        int32_t py = (int32_t)y + k;
        if (py < 0) py = 0;
        if (py >= (int32_t)tela->altura) py = (int32_t)tela->altura - 1;

        Cor c = temp[(size_t)py * tela->largura + (size_t)x];
        float w = núcleo[k + r];
        sr += (float)c.vermelho * w;
        sg += (float)c.verde    * w;
        sb += (float)c.azul     * w;
        sa += (float)c.alfa     * w;
      }
      tela->píxeis[(size_t)y * tela->largura + (size_t)x] = (Cor){
        (uint8_t)roundf(sr), (uint8_t)roundf(sg), (uint8_t)roundf(sb), (uint8_t)roundf(sa)
      };
    }
  }

  free(núcleo);
  free(temp);
}

void converter_para_tons_de_cinzento(Tela *tela) {
  if (!tela || !tela->píxeis) return;
  size_t total = (size_t)tela->largura * (size_t)tela->altura;
  for (size_t i = 0; i < total; i++) {
    Cor c = tela->píxeis[i];
    uint8_t lum = (uint8_t)roundf(0.2126f * (float)c.vermelho + 0.7152f * (float)c.verde + 0.0722f * (float)c.azul);
    tela->píxeis[i] = (Cor){ lum, lum, lum, c.alfa };
  }
}

void aplicar_modulação_alfa(Tela *tela, uint8_t alfa) {
  if (!tela || !tela->píxeis) return;
  size_t total = (size_t)tela->largura * (size_t)tela->altura;
  for (size_t i = 0; i < total; i++) {
    tela->píxeis[i].alfa = (uint8_t)(((uint32_t)tela->píxeis[i].alfa * alfa) / 255U);
  }
}

void ajustar_brilho_contraste(Tela *tela, float brilho, float contraste) {
  if (!tela || !tela->píxeis) return;
  size_t total = (size_t)tela->largura * (size_t)tela->altura;
  for (size_t i = 0; i < total; i++) {
    Cor c = tela->píxeis[i];
    float r = (((float)c.vermelho - 128.0f) * contraste) + 128.0f + brilho;
    float g = (((float)c.verde    - 128.0f) * contraste) + 128.0f + brilho;
    float b = (((float)c.azul     - 128.0f) * contraste) + 128.0f + brilho;
    tela->píxeis[i] = (Cor){
      limitar_u8((int32_t)roundf(r)),
      limitar_u8((int32_t)roundf(g)),
      limitar_u8((int32_t)roundf(b)),
      c.alfa
    };
  }
}

void inverter_cores(Tela *tela) {
  if (!tela || !tela->píxeis) return;
  size_t total = (size_t)tela->largura * (size_t)tela->altura;
  for (size_t i = 0; i < total; i++) {
    tela->píxeis[i].vermelho = 255 - tela->píxeis[i].vermelho;
    tela->píxeis[i].verde    = 255 - tela->píxeis[i].verde;
    tela->píxeis[i].azul     = 255 - tela->píxeis[i].azul;
  }
}

void espelhar_tela_horizontal(Tela *tela) {
  if (!tela || !tela->píxeis || tela->largura < 2) return;
  for (uint32_t y = 0; y < tela->altura; y++) {
    Cor *linha = &tela->píxeis[(size_t)y * tela->largura];
    for (uint32_t x = 0; x < tela->largura / 2; x++) {
      uint32_t oposto = tela->largura - 1 - x;
      Cor tmp = linha[x];
      linha[x] = linha[oposto];
      linha[oposto] = tmp;
    }
  }
}

void espelhar_tela_vertical(Tela *tela) {
  if (!tela || !tela->píxeis || tela->altura < 2) return;
  size_t bytes_linha = (size_t)tela->largura * sizeof(Cor);
  Cor *buffer_linha = malloc(bytes_linha);
  if (!buffer_linha) return;

  for (uint32_t y = 0; y < tela->altura / 2; y++) {
    uint32_t oposta = tela->altura - 1 - y;
    Cor *l1 = &tela->píxeis[(size_t)y * tela->largura];
    Cor *l2 = &tela->píxeis[(size_t)oposta * tela->largura];
    memcpy(buffer_linha, l1, bytes_linha);
    memcpy(l1, l2, bytes_linha);
    memcpy(l2, buffer_linha, bytes_linha);
  }

  free(buffer_linha);
}

bool carregar_de_ficheiro(Tela *destino, const char *caminho) {
  if (!destino || !caminho) {
    return false;
  }

  FILE *ficheiro = fopen(caminho, "rb");
  if (!ficheiro) {
    return false;
  }

  uint8_t cabeçalho[18];
  if (fread(cabeçalho, 1, sizeof(cabeçalho), ficheiro) != sizeof(cabeçalho)) {
    fclose(ficheiro);
    return false;
  }

  uint8_t tamanho_id = cabeçalho[0];
  uint8_t tipo_mapa_cores = cabeçalho[1];
  uint8_t tipo_imagem = cabeçalho[2];
  uint16_t largura = (uint16_t)(cabeçalho[12] | (cabeçalho[13] << 8));
  uint16_t altura = (uint16_t)(cabeçalho[14] | (cabeçalho[15] << 8));
  uint8_t bits_por_píxel = cabeçalho[16];
  uint8_t descritor = cabeçalho[17];

  /* Validar: sem paleta, RGB não comprimido (tipo 2), 24 ou 32 bits */
  if (tipo_mapa_cores != 0 || tipo_imagem != 2 || largura == 0 || altura == 0 ||
      (bits_por_píxel != 24 && bits_por_píxel != 32)) {
    fclose(ficheiro);
    return false;
  }

  /* Saltar o campo de identificação da imagem, se existir */
  if (tamanho_id > 0 && fseek(ficheiro, tamanho_id, SEEK_CUR) != 0) {
    fclose(ficheiro);
    return false;
  }

  size_t total_píxeis = (size_t)largura * (size_t)altura;
  Cor *píxeis = malloc(total_píxeis * sizeof(Cor));
  if (!píxeis) {
    fclose(ficheiro);
    return false;
  }

  size_t bytes_por_píxel = bits_por_píxel / 8;
  size_t tamanho_linha = (size_t)largura * bytes_por_píxel;
  uint8_t *linha_temp = malloc(tamanho_linha);
  if (!linha_temp) {
    free(píxeis);
    fclose(ficheiro);
    return false;
  }

  /* Bit 5 do descritor indica origem no topo (1) ou na base (0) */
  bool origem_no_topo = (descritor & 0x20) != 0;

  for (uint32_t y = 0; y < altura; ++y) {
    if (fread(linha_temp, 1, tamanho_linha, ficheiro) != tamanho_linha) {
      free(linha_temp);
      free(píxeis);
      fclose(ficheiro);
      return false;
    }

    uint32_t y_destino = origem_no_topo ? y : (altura - 1 - y);
    Cor *linha_destino = &píxeis[(size_t)y_destino * largura];

    for (uint32_t x = 0; x < largura; ++x) {
      size_t índice = (size_t)x * bytes_por_píxel;
      /* O formato TGA armazena os canais por ordem BGRA */
      linha_destino[x].azul = linha_temp[índice + 0];
      linha_destino[x].verde = linha_temp[índice + 1];
      linha_destino[x].vermelho = linha_temp[índice + 2];
      linha_destino[x].alfa = (bytes_por_píxel == 4) ? linha_temp[índice + 3] : 255;
    }
  }

  free(linha_temp);
  fclose(ficheiro);

  destino->largura = largura;
  destino->altura = altura;
  destino->píxeis = píxeis;

  return true;
}

bool exportar_para_ficheiro_tga(const Tela *tela, const char *caminho) {
  if (!tela || !tela->píxeis || !caminho || tela->largura == 0 || tela->altura == 0) {
    return false;
  }

  /* O formato TGA usa inteiros de 16 bits sem sinal para as dimensões */
  if (tela->largura > UINT16_MAX || tela->altura > UINT16_MAX) {
    return false;
  }

  FILE *ficheiro = fopen(caminho, "wb");
  if (!ficheiro) {
    return false;
  }

  uint16_t largura = (uint16_t)tela->largura;
  uint16_t altura = (uint16_t)tela->altura;

  /* Cabeçalho TGA de 18 bytes: 32 bits BGRA, sem compressão, origem no topo */
  uint8_t cabeçalho[18] = {
    0,                                  /* Tamanho do ID */
    0,                                  /* Sem mapa de cores */
    2,                                  /* Imagem RGB(A) não comprimida */
    0, 0, 0, 0, 0,                      /* Especificação do mapa de cores (5 bytes) */
    0, 0,                               /* Origem X */
    0, 0,                               /* Origem Y */
    (uint8_t)(largura & 0xFF),          /* Largura (byte menos significativo) */
    (uint8_t)((largura >> 8) & 0xFF),   /* Largura (byte mais significativo) */
    (uint8_t)(altura & 0xFF),           /* Altura (byte menos significativo) */
    (uint8_t)((altura >> 8) & 0xFF),    /* Altura (byte mais significativo) */
    32,                                 /* 32 bits por píxel */
    0x28                                /* 8 bits de alfa (0x08) + origem no topo (0x20) */
  };

  if (fwrite(cabeçalho, 1, sizeof(cabeçalho), ficheiro) != sizeof(cabeçalho)) {
    fclose(ficheiro);
    return false;
  }

  size_t tamanho_linha = (size_t)largura * 4;
  uint8_t *linha_temp = malloc(tamanho_linha);
  if (!linha_temp) {
    fclose(ficheiro);
    return false;
  }

  for (uint32_t y = 0; y < altura; ++y) {
    const Cor *linha_origem = &tela->píxeis[(size_t)y * largura];

    for (uint32_t x = 0; x < largura; ++x) {
      size_t índice = (size_t)x * 4;
      /* Converter de RGBA para BGRA */
      linha_temp[índice + 0] = linha_origem[x].azul;
      linha_temp[índice + 1] = linha_origem[x].verde;
      linha_temp[índice + 2] = linha_origem[x].vermelho;
      linha_temp[índice + 3] = linha_origem[x].alfa;
    }

    if (fwrite(linha_temp, 1, tamanho_linha, ficheiro) != tamanho_linha) {
      free(linha_temp);
      fclose(ficheiro);
      return false;
    }
  }

  free(linha_temp);
  fclose(ficheiro);

  return true;
}

int main(void) {
  printf("A iniciar testes essenciais da biblioteca gráfica...\n");

  /* 1. Utilitários e Cores */
  int32_t nums[] = { 30, -5, 12 };
  qsort(nums, 3, sizeof(int32_t), comparar_int32);
  assert(nums[0] == -5 && nums[2] == 30);

  CorHSV hsv = cor_para_hsv(COR_VERMELHA);
  assert(cores_iguais(COR_VERMELHA, cor_de_hsv(hsv)));
  assert(interpolar_cor(COR_PRETA, COR_BRANCA, 0.5f).vermelho >= 127);

  /* 2. Matrizes e Transformações 2D */
  Matriz3x3 m_trans, m_esc, m_comb, m_inv;
  criar_matriz_translação(&m_trans, 10.0f, 20.0f);
  criar_matriz_escala(&m_esc, 2.0f, 2.0f);
  multiplicar_matrizes(&m_comb, &m_trans, &m_esc);
  assert(inverter_matriz(&m_inv, &m_comb));

  Flut2D p = transformar_flut2d_matriz((Flut2D){ 5.0f, 5.0f }, &m_comb);
  Flut2D p_volta = transformar_flut2d_matriz(p, &m_inv);
  assert(fabsf(p_volta.x - 5.0f) < 1e-3f && fabsf(p_volta.y - 5.0f) < 1e-3f);

  Int2D rodado = rodar_ponto((Int2D){ 10, 0 }, (Int2D){ 0, 0 }, 1.5707963f);
  assert(abs(rodado.x) <= 1 && abs(rodado.y - 10) <= 1);

  /* 3. Tela, Píxeis e Primitivas Geométricas */
  Tela tela = criar_tela(256, 256);
  assert(tela.píxeis != nullptr);
  limpar_tela(&tela, COR_PRETA);

  desenhar_píxel(&tela, (Int2D){ 5, 5 }, COR_BRANCA);
  assert(cores_iguais(obter_píxel(&tela, (Int2D){ 5, 5 }), COR_BRANCA));

  desenhar_píxel_mistura(&tela, (Int2D){ 5, 5 }, COR_VERMELHA, MISTURA_MULTIPLICAR);
  desenhar_linha_suavizada(&tela, (Int2D){ 10, 10 }, (Int2D){ 200, 30 }, COR_CIANO);
  desenhar_curva_bezier_cúbica(&tela, (Int2D){ 10, 50 }, (Int2D){ 80, 10 }, (Int2D){ 160, 90 }, (Int2D){ 240, 50 }, COR_MAGENTA);

  desenhar_triângulo_preenchido(&tela, (Int2D){ 30, 80 }, (Int2D){ 70, 80 }, (Int2D){ 50, 120 }, COR_VERDE);
  assert(cores_iguais(obter_píxel(&tela, (Int2D){ 50, 95 }), COR_VERDE));

  desenhar_retângulo_arredondado_preenchido(&tela, (Int2D){ 90, 80 }, 60, 40, 8, COR_AZUL);
  desenhar_círculo_preenchido(&tela, (Int2D){ 200, 100 }, 25, COR_AMARELA);
  desenhar_gradiente_linear(&tela, (Int2D){ 10, 140 }, 100, 30, COR_VERMELHA, COR_AZUL, false);

  desenhar_retângulo(&tela, (Int2D){ 130, 140 }, 40, 30, COR_BRANCA);
  preencher_área(&tela, (Int2D){ 150, 155 }, COR_CIANO);
  assert(cores_iguais(obter_píxel(&tela, (Int2D){ 150, 155 }), COR_CIANO));

  /* 4. Imagens (Blitting) e Pós-Processamento */
  Tela sprite = criar_tela(32, 32);
  limpar_tela(&sprite, COR_VERMELHA);
  desenhar_imagem_rodada(&tela, &sprite, (Int2D){ 60, 210 }, 0.785398f, (Int2D){ 16, 16 });
  desenhar_imagem_escala(&tela, &sprite, (Int2D){ 120, 190 }, 48, 48);

  aplicar_desfoque_gaussiano(&tela, 1);
  espelhar_tela_horizontal(&tela);
  inverter_cores(&tela);

  /* 5. Exportação e Carregamento de Ficheiro */
  const char *ficheiro = "resultado_teste.tga";
  assert(exportar_para_ficheiro_tga(&tela, ficheiro));

  Tela carregada = { 0 };
  assert(carregar_de_ficheiro(&carregada, ficheiro));
  assert(carregada.largura == tela.largura && carregada.altura == tela.altura);
  assert(cores_iguais(obter_píxel(&carregada, (Int2D){ 50, 50 }), obter_píxel(&tela, (Int2D){ 50, 50 })));

  destruir_tela(&sprite);
  destruir_tela(&tela);
  destruir_tela(&carregada);

  printf("Testes concluídos com sucesso!\n");
  return EXIT_SUCCESS;
}
