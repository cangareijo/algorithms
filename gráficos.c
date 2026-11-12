#include <math.h>
#include <stdbool.h>
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
} ModoMistura;

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
  uint8_t alfa;
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

int comparar_inteiros_32(const void *a, const void *b);

static inline float normalizar_radianos(float ângulo);

bool cores_iguais(Cor a, Cor b);
Cor interpolar_cor(Cor a, Cor b, float fator);

Cor cor_de_hsv(CorHSV hsv);
CorHSV cor_para_hsv(Cor cor);

void transformar_pontos(const Int2D pontos_origem[], Int2D pontos_destino[], size_t núm_pontos, Int2D translação, float radianos, float escala);
Int2D rodar_ponto(Int2D ponto, Int2D origem, float radianos);

Matriz3x3 criar_matriz_identidade(void);
Matriz3x3 criar_matriz_translação(float tx, float ty);
Matriz3x3 criar_matriz_rotação(float radianos);
Matriz3x3 criar_matriz_escala(float sx, float sy);
Matriz3x3 multiplicar_matrizes(Matriz3x3 a, Matriz3x3 b);
Matriz3x3 inverter_matriz(Matriz3x3 m);
Int2D transformar_int2d_matriz(Int2D ponto, Matriz3x3 m);
Flut2D transformar_flut2d_matriz(Flut2D ponto, Matriz3x3 m);

Tela criar_tela(uint32_t largura, uint32_t altura);
void destruir_tela(Tela *tela);
void limpar_tela(Tela *tela, Cor fundo);
void desenhar_píxel(Tela *tela, Int2D ponto, Cor cor);
void desenhar_píxel_alfa(Tela *tela, Int2D ponto, Cor cor_fonte);
void desenhar_píxel_mistura(Tela *tela, Int2D ponto, Cor cor_fonte, ModoMistura modo);
void desenhar_linha(Tela *tela, Int2D início, Int2D fim, Cor cor);
void desenhar_linha_horizontal(Tela *tela, int32_t y, int32_t x_início, int32_t x_fim, Cor cor);
void desenhar_linha_espessa(Tela *tela, Int2D início, Int2D fim, uint32_t espessura, Cor cor);
void desenhar_linha_suavizada(Tela *tela, Int2D início, Int2D fim, Cor cor);
void desenhar_polilinha(Tela *tela, const Int2D vértices[], size_t núm_vértices, Cor cor, bool fechada);
void desenhar_curva_bezier_quadrática(Tela *tela, Int2D p0, Int2D p1, Int2D p2, Cor cor);
void desenhar_curva_bezier_cúbica(Tela *tela, Int2D p0, Int2D p1, Int2D p2, Int2D p3, Cor cor);
void desenhar_curva_bezier_subdivisão(Tela *tela, Int2D p0, Int2D p1, Int2D p2, Int2D p3, Cor cor, float tolerância_píxeis);
void desenhar_spline_catmull_rom(Tela *tela, const Int2D pontos[], size_t núm_pontos, uint32_t subdivisões_por_segmento, Cor cor);
void desenhar_triângulo(Tela *tela, Int2D p1, Int2D p2, Int2D p3, Cor cor);
void desenhar_triângulo_preenchido(Tela *tela, Int2D p1, Int2D p2, Int2D p3, Cor cor);
void desenhar_triângulo_preenchido_base_plana(Tela *tela, Int2D p1, Int2D p2, Int2D p3, Cor cor);
void desenhar_triângulo_preenchido_topo_plano(Tela *tela, Int2D p1, Int2D p2, Int2D p3, Cor cor);
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
void desenhar_elipse(Tela *tela, Int2D centro, int32_t rx, int32_t ry, Cor cor);
void desenhar_elipse_preenchida(Tela *tela, Int2D centro, int32_t rx, int32_t ry, Cor cor);
void preencher_área(Tela *tela, Int2D semente, Cor nova_cor);
void desenhar_imagem(Tela *destino, const Tela *origem, Int2D posição);
void desenhar_imagem_escala(Tela *destino, const Tela *origem, Int2D posição, int32_t largura_nova, int32_t altura_nova);
void desenhar_imagem_rodada(Tela *destino,  const Tela *origem,  Int2D posição,  float radianos,  Int2D pivô);
void desenhar_subimagem(Tela *destino, const Tela *origem, Int2D pos_destino, Int2D pos_origem, uint32_t larg_sub, uint32_t alt_sub);
void aplicar_desfoque_gaussiano(Tela *tela, uint32_t raio);
void converter_para_tons_de_cinzento(Tela *tela);
void aplicar_modulação_alfa(Tela *tela, uint8_t alfa);
void desenhar_imagem_matriz(Tela *destino, const Tela *origem, Matriz3x3 m);

bool exportar_para_ficheiro(const Tela *tela, const char *caminho);

int comparar_inteiros_32(const void *const a, const void *const b) {
  const int32_t valor_a = *(const int32_t *)a;
  const int32_t valor_b = *(const int32_t *)b;
  return (valor_a > valor_b) - (valor_a < valor_b);
}

static inline float normalizar_radianos(float ângulo) {
  constexpr float DOIS_PI = 6.28318530717958647692f;
  ângulo = fmodf(ângulo, DOIS_PI);
  if (ângulo < 0.0f) ângulo += DOIS_PI;
  return ângulo;
}

bool cores_iguais(const Cor a, const Cor b) {
  return a.vermelho == b.vermelho &&
         a.verde    == b.verde    &&
         a.azul     == b.azul     &&
         a.alfa     == b.alfa;
}

Cor interpolar_cor(Cor a, Cor b, float fator) {
  if (fator <= 0.0f) return a;
  if (fator >= 1.0f) return b;
  const float inv = 1.0f - fator;
  return (Cor){
    .vermelho = (uint8_t)roundf((float)a.vermelho * inv + (float)b.vermelho * fator),
    .verde    = (uint8_t)roundf((float)a.verde    * inv + (float)b.verde    * fator),
    .azul     = (uint8_t)roundf((float)a.azul     * inv + (float)b.azul     * fator),
    .alfa     = (uint8_t)roundf((float)a.alfa     * inv + (float)b.alfa     * fator)
  };
}

Cor cor_de_hsv(CorHSV hsv) {
  constexpr float DOIS_PI = 6.28318530717958647692f;
  constexpr float TERÇO_PI = 1.04719755119659774615f;
  const float s = (hsv.saturação < 0.0f) ? 0.0f : (hsv.saturação > 1.0f ? 1.0f : hsv.saturação);
  const float v = (hsv.valor     < 0.0f) ? 0.0f : (hsv.valor     > 1.0f ? 1.0f : hsv.valor);
  if (s <= 0.0f) {
    const uint8_t cinza = (uint8_t)roundf(v * 255.0f);
    return (Cor){ .vermelho = cinza, .verde = cinza, .azul = cinza, .alfa = hsv.alfa };
  }
  float h = fmodf(hsv.matiz, DOIS_PI);
  if (h < 0.0f) {
    h += DOIS_PI;
  }
  const float c = v * s;
  const float setor = h / TERÇO_PI;
  const float x = c * (1.0f - fabsf(fmodf(setor, 2.0f) - 1.0f));
  const float m = v - c;
  float r_flut = 0.0f;
  float g_flut = 0.0f;
  float b_flut = 0.0f;
  if (setor < 1.0f) {
    r_flut = c; g_flut = x; b_flut = 0.0f;
  } else if (setor < 2.0f) {
    r_flut = x; g_flut = c; b_flut = 0.0f;
  } else if (setor < 3.0f) {
    r_flut = 0.0f; g_flut = c; b_flut = x;
  } else if (setor < 4.0f) {
    r_flut = 0.0f; g_flut = x; b_flut = c;
  } else if (setor < 5.0f) {
    r_flut = x; g_flut = 0.0f; b_flut = c;
  } else {
    r_flut = c; g_flut = 0.0f; b_flut = x;
  }
  return (Cor){
    .vermelho = (uint8_t)roundf((r_flut + m) * 255.0f),
    .verde    = (uint8_t)roundf((g_flut + m) * 255.0f),
    .azul     = (uint8_t)roundf((b_flut + m) * 255.0f),
    .alfa     = hsv.alfa
  };
}

CorHSV cor_para_hsv(Cor cor) {
  constexpr float DOIS_PI = 6.28318530717958647692f;
  constexpr float TERÇO_PI = 1.04719755119659774615f;
  const float r = (float)cor.vermelho / 255.0f;
  const float g = (float)cor.verde    / 255.0f;
  const float b = (float)cor.azul     / 255.0f;
  const float c_máx = fmaxf(r, fmaxf(g, b));
  const float c_mín = fminf(r, fminf(g, b));
  const float delta = c_máx - c_mín;
  float h = 0.0f;
  if (delta > 0.0f) {
    if (c_máx == r) {
      h = TERÇO_PI * fmodf((g - b) / delta, 6.0f);
    } else if (c_máx == g) {
      h = TERÇO_PI * (((b - r) / delta) + 2.0f);
    } else {
      h = TERÇO_PI * (((r - g) / delta) + 4.0f);
    }
    if (h < 0.0f) {
      h += DOIS_PI;
    }
  }
  const float s = (c_máx <= 0.0f) ? 0.0f : (delta / c_máx);
  return (CorHSV){
    .matiz     = h,
    .saturação = s,
    .valor     = c_máx,
    .alfa      = cor.alfa
  };
}

void transformar_pontos(
  const Int2D pontos_origem[],
  Int2D pontos_destino[],
  const size_t núm_pontos,
  const Int2D translação,
  const float radianos,
  const float escala)
{
  if (pontos_origem == nullptr || pontos_destino == nullptr || núm_pontos == 0) return;
  const float cosseno = cosf(radianos);
  const float seno = sinf(radianos);
  for (size_t i = 0; i < núm_pontos; ++i) {
    const float x_escalado = (float)pontos_origem[i].x * escala;
    const float y_escalado = (float)pontos_origem[i].y * escala;
    const float x_rodado = (x_escalado * cosseno) - (y_escalado * seno);
    const float y_rodado = (x_escalado * seno) + (y_escalado * cosseno);
    pontos_destino[i] = (Int2D){
      .x = (int32_t)roundf(x_rodado + (float)translação.x),
      .y = (int32_t)roundf(y_rodado + (float)translação.y)
    };
  }
}

[[nodiscard]] Int2D rodar_ponto(const Int2D ponto, const Int2D origem, const float radianos) {
  if (radianos == 0.0f) return ponto;
  const float cosseno = cosf(radianos);
  const float seno = sinf(radianos);
  const float delta_x = (float)(ponto.x - origem.x);
  const float delta_y = (float)(ponto.y - origem.y);
  const float novo_x = origem.x + (delta_x * cosseno) - (delta_y * seno);
  const float novo_y = origem.y + (delta_x * seno) + (delta_y * cosseno);
  return (Int2D){
    .x = (int32_t)roundf(novo_x),
    .y = (int32_t)roundf(novo_y)
  };
}

Matriz3x3 criar_matriz_identidade(void) {
  return (Matriz3x3){
    .m = {
      { 1.0f, 0.0f, 0.0f },
      { 0.0f, 1.0f, 0.0f },
      { 0.0f, 0.0f, 1.0f }
    }
  };
}

Matriz3x3 criar_matriz_translação(float tx, float ty) {
  return (Matriz3x3){
    .m = {
      { 1.0f, 0.0f, 0.0f },
      { 0.0f, 1.0f, 0.0f },
      {   tx,   ty, 1.0f }
    }
  };
}

Matriz3x3 criar_matriz_rotação(float radianos) {
  const float c = cosf(radianos);
  const float s = sinf(radianos);
  return (Matriz3x3){
    .m = {
      {    c,    s, 0.0f },
      {   -s,    c, 0.0f },
      { 0.0f, 0.0f, 1.0f }
    }
  };
}

Matriz3x3 criar_matriz_escala(float sx, float sy) {
  return (Matriz3x3){
    .m = {
      {   sx, 0.0f, 0.0f },
      { 0.0f,   sy, 0.0f },
      { 0.0f, 0.0f, 1.0f }
    }
  };
}

Matriz3x3 multiplicar_matrizes(Matriz3x3 a, Matriz3x3 b) {
  Matriz3x3 resultado = {};
  for (size_t linha = 0; linha < 3; linha++) {
    for (size_t coluna = 0; coluna < 3; coluna++) {
      resultado.m[linha][coluna] =
        a.m[linha][0] * b.m[0][coluna] +
        a.m[linha][1] * b.m[1][coluna] +
        a.m[linha][2] * b.m[2][coluna];
    }
  }
  return resultado;
}

Matriz3x3 inverter_matriz(Matriz3x3 m) {
  const float c00 =  (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]);
  const float c01 = -(m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]);
  const float c02 =  (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);
  const float c10 = -(m.m[0][1] * m.m[2][2] - m.m[0][2] * m.m[2][1]);
  const float c11 =  (m.m[0][0] * m.m[2][2] - m.m[0][2] * m.m[2][0]);
  const float c12 = -(m.m[0][0] * m.m[2][1] - m.m[0][1] * m.m[2][0]);
  const float c20 =  (m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1]);
  const float c21 = -(m.m[0][0] * m.m[1][2] - m.m[0][2] * m.m[1][0]);
  const float c22 =  (m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0]);
  const float det = m.m[0][0] * c00 + m.m[0][1] * c01 + m.m[0][2] * c02;
  if (fabsf(det) < 1e-7f) return criar_matriz_identidade();
  const float inv_det = 1.0f / det;
  return (Matriz3x3){
    .m = {
      { c00 * inv_det, c10 * inv_det, c20 * inv_det },
      { c01 * inv_det, c11 * inv_det, c21 * inv_det },
      { c02 * inv_det, c12 * inv_det, c22 * inv_det }
    }
  };
}

Int2D transformar_int2d_matriz(Int2D ponto, Matriz3x3 m) {
  const Flut2D p_flut = { .x = (float)ponto.x, .y = (float)ponto.y };
  const Flut2D res = transformar_flut2d_matriz(p_flut, m);
  return (Int2D){
    .x = (int32_t)roundf(res.x),
    .y = (int32_t)roundf(res.y)
  };
}

Flut2D transformar_flut2d_matriz(Flut2D ponto, Matriz3x3 m) {
  const float x = ponto.x * m.m[0][0] + ponto.y * m.m[1][0] + m.m[2][0];
  const float y = ponto.x * m.m[0][1] + ponto.y * m.m[1][1] + m.m[2][1];
  const float w = ponto.x * m.m[0][2] + ponto.y * m.m[1][2] + m.m[2][2];
  if (w != 0.0f && w != 1.0f) {
    const float inv_w = 1.0f / w;
    return (Flut2D){ .x = x * inv_w, .y = y * inv_w };
  }
  return (Flut2D){ .x = x, .y = y };
}

[[nodiscard]] Tela criar_tela(const uint32_t largura, const uint32_t altura) {
  if (largura == 0 || altura == 0) return (Tela){};
  Cor *const píxeis = malloc(largura * altura * sizeof(Cor));
  if (píxeis == nullptr) return (Tela){};
  Tela nova_tela = {
    .largura = largura,
    .altura = altura,
    .píxeis = píxeis
  };
  return nova_tela;
}

void destruir_tela(Tela *const tela) {
  if (tela == nullptr) return;
  free(tela->píxeis);
  tela->píxeis = nullptr;
  tela->largura = 0;
  tela->altura = 0;
}

void limpar_tela(Tela *const tela, const Cor fundo) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  const uint32_t área = tela->largura * tela->altura;
  for (uint32_t i = 0; i < área; ++i) tela->píxeis[i] = fundo;
}

void desenhar_píxel(Tela *const tela, const Int2D ponto, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  if (ponto.x < 0 || ponto.x >= (int32_t)tela->largura) return;
  if (ponto.y < 0 || ponto.y >= (int32_t)tela->altura) return;
  tela->píxeis[ponto.x + ponto.y * tela->largura] = cor;
}

void desenhar_píxel_alfa(Tela *const tela, const Int2D ponto, const Cor cor_fonte) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  if (ponto.x < 0 || ponto.x >= (int32_t)tela->largura) return;
  if (ponto.y < 0 || ponto.y >= (int32_t)tela->altura) return;
  if (cor_fonte.alfa == 0) return;
  const size_t índice = (size_t)ponto.x + (size_t)ponto.y * tela->largura;
  const Cor cor_destino = tela->píxeis[índice];
  if (cor_fonte.alfa == 255) {
    tela->píxeis[índice] = cor_fonte;
    return;
  }
  const uint8_t alfa = cor_fonte.alfa;
  const uint8_t inv_alfa = 255 - alfa;
  const uint32_t verm_misturado = ((uint32_t)cor_fonte.vermelho * alfa) + ((uint32_t)cor_destino.vermelho * inv_alfa);
  const uint32_t verd_misturado = ((uint32_t)cor_fonte.verde * alfa) + ((uint32_t)cor_destino.verde * inv_alfa);
  const uint32_t azul_misturado = ((uint32_t)cor_fonte.azul * alfa) + ((uint32_t)cor_destino.azul * inv_alfa);
  tela->píxeis[índice] = (Cor){
    .vermelho = (uint8_t)((verm_misturado + 1 + (verm_misturado >> 8)) >> 8),
    .verde = (uint8_t)((verd_misturado + 1 + (verd_misturado >> 8)) >> 8),
    .azul = (uint8_t)((azul_misturado + 1 + (azul_misturado >> 8)) >> 8),
    .alfa = 255
  };
}

void desenhar_píxel_mistura(Tela *tela, Int2D ponto, Cor cor_fonte, ModoMistura modo) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  if (ponto.x < 0 || ponto.y < 0 || (uint32_t)ponto.x >= tela->largura || (uint32_t)ponto.y >= tela->altura) return;
  if (cor_fonte.alfa == 0) return;
  Cor *const destino = &tela->píxeis[(size_t)ponto.y * tela->largura + (size_t)ponto.x];
  if (modo == MISTURA_NORMAL && cor_fonte.alfa == 255) {
    *destino = cor_fonte;
    return;
  }
  uint32_t r_mistura = cor_fonte.vermelho;
  uint32_t g_mistura = cor_fonte.verde;
  uint32_t b_mistura = cor_fonte.azul;
  switch (modo) {
    case MISTURA_NORMAL:
      break;
    case MISTURA_ADITIVA: {
      r_mistura = (uint32_t)destino->vermelho + r_mistura;
      g_mistura = (uint32_t)destino->verde    + g_mistura;
      b_mistura = (uint32_t)destino->azul     + b_mistura;
      if (r_mistura > 255) r_mistura = 255;
      if (g_mistura > 255) g_mistura = 255;
      if (b_mistura > 255) b_mistura = 255;
      break;
    }
    case MISTURA_MULTIPLICAR: {
      r_mistura = ((uint32_t)destino->vermelho * r_mistura + 127) / 255;
      g_mistura = ((uint32_t)destino->verde    * g_mistura + 127) / 255;
      b_mistura = ((uint32_t)destino->azul     * b_mistura + 127) / 255;
      break;
    }
    case MISTURA_ECRÃ: {
      r_mistura = 255 - (((255 - (uint32_t)destino->vermelho) * (255 - r_mistura) + 127) / 255);
      g_mistura = 255 - (((255 - (uint32_t)destino->verde)    * (255 - g_mistura) + 127) / 255);
      b_mistura = 255 - (((255 - (uint32_t)destino->azul)     * (255 - b_mistura) + 127) / 255);
      break;
    }
  }
  const uint32_t alfa = cor_fonte.alfa;
  const uint32_t inv_alfa = 255 - alfa;
  destino->vermelho = (uint8_t)((r_mistura * alfa + (uint32_t)destino->vermelho * inv_alfa + 127) / 255);
  destino->verde    = (uint8_t)((g_mistura * alfa + (uint32_t)destino->verde    * inv_alfa + 127) / 255);
  destino->azul     = (uint8_t)((b_mistura * alfa + (uint32_t)destino->azul     * inv_alfa + 127) / 255);
  const uint32_t novo_alfa = alfa + (((uint32_t)destino->alfa * inv_alfa + 127) / 255);
  destino->alfa = (uint8_t)(novo_alfa > 255 ? 255 : novo_alfa);
}

void desenhar_linha(Tela *const tela, const Int2D início, const Int2D fim, const Cor cor) {
  const int32_t delta_x = abs(fim.x - início.x);
  const int32_t delta_y = abs(fim.y - início.y);
  const int32_t passo_x = (início.x < fim.x) ? 1 : -1;
  const int32_t passo_y = (início.y < fim.y) ? 1 : -1;
  int32_t erro = delta_x - delta_y;
  Int2D atual = início;
  while (true) {
    desenhar_píxel(tela, atual, cor);
    if (atual.x == fim.x && atual.y == fim.y) break;
    const int32_t erro2 = 2 * erro;
    if (erro2 > -delta_y) {
      erro -= delta_y;
      atual.x += passo_x;
    }
    if (erro2 < delta_x) {
      erro += delta_x;
      atual.y += passo_y;
    }
  }
}

void desenhar_linha_horizontal(Tela *const tela, const int32_t y, int32_t x_início, int32_t x_fim, const Cor cor) {
  if (y < 0 || y >= (int32_t)tela->altura) return;
  if (x_início > x_fim) {
    const int32_t temp = x_início;
    x_início = x_fim;
    x_fim = temp;
  }
  if (x_fim < 0 || x_início >= (int32_t)tela->largura) return;
  if (x_início < 0) x_início = 0;
  if (x_fim >= (int32_t)tela->largura) x_fim = (int32_t)tela->largura - 1;
  Cor *const linha_tela = &tela->píxeis[(size_t)y * tela->largura];
  for (int32_t x = x_início; x <= x_fim; x++) linha_tela[x] = cor;
}

void desenhar_linha_espessa(
  Tela *const tela,
  const Int2D início,
  const Int2D fim,
  const uint32_t espessura,
  const Cor cor)
{
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) return;
  if (espessura == 0) return;
  if (espessura == 1) {
    desenhar_linha(tela, início, fim, cor);
    return;
  }
  const float raio = (float)espessura * 0.5f;
  const float raio_quadrado = raio * raio;
  const int32_t margem = (int32_t)ceilf(raio);
  int32_t x_mín = (início.x < fim.x ? início.x : fim.x) - margem;
  int32_t x_máx = (início.x > fim.x ? início.x : fim.x) + margem;
  int32_t y_mín = (início.y < fim.y ? início.y : fim.y) - margem;
  int32_t y_máx = (início.y > fim.y ? início.y : fim.y) + margem;
  if (x_mín < 0) x_mín = 0;
  if (x_máx >= (int32_t)tela->largura) x_máx = (int32_t)tela->largura - 1;
  if (y_mín < 0) y_mín = 0;
  if (y_máx >= (int32_t)tela->altura) y_máx = (int32_t)tela->altura - 1;
  if (x_mín > x_máx || y_mín > y_máx) return;
  const float ax = (float)início.x;
  const float ay = (float)início.y;
  const float abx = (float)fim.x - ax;
  const float aby = (float)fim.y - ay;
  const float comp_quadrado = abx * abx + aby * aby;
  if (comp_quadrado == 0.0f) {
    desenhar_círculo_preenchido(tela, início, (uint32_t)raio, cor);
    return;
  }
  const float inv_comp_quadrado = 1.0f / comp_quadrado;
  for (int32_t y = y_mín; y <= y_máx; y++) {
    const float py = (float)y - ay;
    Cor *const linha_tela = &tela->píxeis[(size_t)y * tela->largura];
    for (int32_t x = x_mín; x <= x_máx; x++) {
      const float px = (float)x - ax;
      float t = (px * abx + py * aby) * inv_comp_quadrado;
      if (t < 0.0f)
        t = 0.0f;
      else if (t > 1.0f)
        t = 1.0f;
      const float dx = px - t * abx;
      const float dy = py - t * aby;
      if (dx * dx + dy * dy <= raio_quadrado) linha_tela[x] = cor;
    }
  }
}

static inline void desenhar_píxel_wu(
  Tela *const tela,
  const int32_t x,
  const int32_t y,
  const Cor cor,
  const float intensidade)
{
  if (intensidade <= 0.0f) return;
  Cor cor_ajustada = cor;
  cor_ajustada.alfa = (uint8_t)roundf((float)cor.alfa * intensidade);
  if (cor_ajustada.alfa > 0) desenhar_píxel_alfa(tela, (Int2D){ .x = x, .y = y }, cor_ajustada);
}

void desenhar_linha_suavizada(
  Tela *const tela,
  const Int2D início,
  const Int2D fim,
  const Cor cor)
{
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) return;
  if (cor.alfa == 0) return;
  float x0 = (float)início.x;
  float y0 = (float)início.y;
  float x1 = (float)fim.x;
  float y1 = (float)fim.y;
  const bool íngreme = fabsf(y1 - y0) > fabsf(x1 - x0);
  if (íngreme) {
    float temp = x0; x0 = y0; y0 = temp;
    temp = x1; x1 = y1; y1 = temp;
  }
  if (x0 > x1) {
    float temp = x0; x0 = x1; x1 = temp;
    temp = y0; y0 = y1; y1 = temp;
  }
  const float dx = x1 - x0;
  const float dy = y1 - y0;
  const float declive = dx == 0.0f ? 1.0f : dy / dx;
  float x_extremo = roundf(x0);
  float y_extremo = y0 + declive * (x_extremo - x0);
  float folga_x = 1.0f - ((x0 + 0.5f) - floorf(x0 + 0.5f));
  const int32_t x_píxel_1 = (int32_t)x_extremo;
  const int32_t y_píxel_1 = (int32_t)floorf(y_extremo);
  const float frac_1 = y_extremo - floorf(y_extremo);
  if (íngreme) {
    desenhar_píxel_wu(tela, y_píxel_1,     x_píxel_1, cor, (1.0f - frac_1) * folga_x);
    desenhar_píxel_wu(tela, y_píxel_1 + 1, x_píxel_1, cor, frac_1 * folga_x);
  } else {
    desenhar_píxel_wu(tela, x_píxel_1, y_píxel_1,     cor, (1.0f - frac_1) * folga_x);
    desenhar_píxel_wu(tela, x_píxel_1, y_píxel_1 + 1, cor, frac_1 * folga_x);
  }
  float interseção_y = y_extremo + declive;
  x_extremo = roundf(x1);
  y_extremo = y1 + declive * (x_extremo - x1);
  folga_x = (x1 + 0.5f) - floorf(x1 + 0.5f);
  const int32_t x_píxel_2 = (int32_t)x_extremo;
  const int32_t y_píxel_2 = (int32_t)floorf(y_extremo);
  const float frac_2 = y_extremo - floorf(y_extremo);
  if (íngreme) {
    desenhar_píxel_wu(tela, y_píxel_2,     x_píxel_2, cor, (1.0f - frac_2) * folga_x);
    desenhar_píxel_wu(tela, y_píxel_2 + 1, x_píxel_2, cor, frac_2 * folga_x);
  } else {
    desenhar_píxel_wu(tela, x_píxel_2, y_píxel_2,     cor, (1.0f - frac_2) * folga_x);
    desenhar_píxel_wu(tela, x_píxel_2, y_píxel_2 + 1, cor, frac_2 * folga_x);
  }
  if (íngreme) {
    for (int32_t x = x_píxel_1 + 1; x < x_píxel_2; x++) {
      const int32_t y_inteiro = (int32_t)floorf(interseção_y);
      const float frac = interseção_y - (float)y_inteiro;
      desenhar_píxel_wu(tela, y_inteiro,     x, cor, 1.0f - frac);
      desenhar_píxel_wu(tela, y_inteiro + 1, x, cor, frac);
      interseção_y += declive;
    }
  } else {
    for (int32_t x = x_píxel_1 + 1; x < x_píxel_2; x++) {
      const int32_t y_inteiro = (int32_t)floorf(interseção_y);
      const float frac = interseção_y - (float)y_inteiro;
      desenhar_píxel_wu(tela, x, y_inteiro,     cor, 1.0f - frac);
      desenhar_píxel_wu(tela, x, y_inteiro + 1, cor, frac);
      interseção_y += declive;
    }
  }
}

void desenhar_polilinha(Tela *const tela, const Int2D vértices[], const size_t núm_vértices, const Cor cor, const bool fechada)
{
  if (tela == nullptr || tela->píxeis == nullptr || vértices == nullptr) return;
  if (tela->largura == 0 || tela->altura == 0 || núm_vértices == 0) return;
  if (núm_vértices == 1) {
    desenhar_píxel(tela, vértices[0], cor);
    return;
  }
  for (size_t i = 0; i + 1 < núm_vértices; i++) desenhar_linha(tela, vértices[i], vértices[i + 1], cor);
  if (fechada && núm_vértices >= 3) desenhar_linha(tela, vértices[núm_vértices - 1], vértices[0], cor);
}

void desenhar_curva_bezier_quadrática(Tela *const tela, const Int2D p0, const Int2D p1, const Int2D p2, const Cor cor)
{
  if (tela == nullptr || tela->píxeis == nullptr) return;
  const uint32_t comprimento_estimado = abs(p1.x - p0.x) + abs(p1.y - p0.y) + abs(p2.x - p1.x) + abs(p2.y - p1.y);
  const uint32_t passos = comprimento_estimado > 10 ? comprimento_estimado : 10;
  const float delta_t = 1.0f / passos;
  Int2D anterior = p0;
  for (uint32_t i = 1; i <= passos; ++i) {
    const float t = i * delta_t;
    const float inv_t = 1.0f - t;
    const float c0 = inv_t * inv_t;
    const float c1 = 2.0f * inv_t * t;
    const float c2 = t * t;
    const float fx = (c0 * p0.x) + (c1 * p1.x) + (c2 * p2.x);
    const float fy = (c0 * p0.y) + (c1 * p1.y) + (c2 * p2.y);
    const Int2D atual = { .x = roundf(fx), .y = roundf(fy) };
    desenhar_linha(tela, anterior, atual, cor);
    anterior = atual;
  }
}

void desenhar_curva_bezier_cúbica(Tela *const tela, const Int2D p0, const Int2D p1, const Int2D p2, const Int2D p3, const Cor cor)
{
  if (tela == nullptr || tela->píxeis == nullptr) return;
  const uint32_t comprimento_estimado =
    abs(p1.x - p0.x) + abs(p1.y - p0.y) +
    abs(p2.x - p1.x) + abs(p2.y - p1.y) +
    abs(p3.x - p2.x) + abs(p3.y - p2.y);
  const uint32_t passos = comprimento_estimado > 10 ? comprimento_estimado : 10;
  const float delta_t = 1.0f / passos;
  Int2D anterior = p0;
  for (uint32_t i = 1; i <= passos; ++i) {
    const float t = i * delta_t;
    const float inv_t = 1.0f - t;
    const float c0 = inv_t * inv_t * inv_t;
    const float c1 = 3.0f * inv_t * inv_t * t;
    const float c2 = 3.0f * inv_t * t * t;
    const float c3 = t * t * t;
    const float fx = (c0 * p0.x) + (c1 * p1.x) + (c2 * p2.x) + (c3 * p3.x);
    const float fy = (c0 * p0.y) + (c1 * p1.y) + (c2 * p2.y) + (c3 * p3.y);
    const Int2D atual = { .x = roundf(fx), .y = roundf(fy) };
    desenhar_linha(tela, anterior, atual, cor);
    anterior = atual;
  }
}

static void subdividir_bezier_recursivo(
  Tela *const tela,
  const Flut2D p0,
  const Flut2D p1,
  const Flut2D p2,
  const Flut2D p3,
  const Cor cor,
  const float tolerância_quadrada,
  const uint32_t profundidade
) {
  const float dx = p3.x - p0.x;
  const float dy = p3.y - p0.y;
  const float d1 = fabsf(((p1.x - p3.x) * dy) - ((p1.y - p3.y) * dx));
  const float d2 = fabsf(((p2.x - p3.x) * dy) - ((p2.y - p3.y) * dx));
  const float desvio_total = d1 + d2;
  const float comprimento_quadrado = (dx * dx) + (dy * dy);
  if ((desvio_total * desvio_total) <= (tolerância_quadrada * comprimento_quadrado) || profundidade >= 16) {
    const Int2D início = { .x = roundf(p0.x), .y = roundf(p0.y) };
    const Int2D fim    = { .x = roundf(p3.x), .y = roundf(p3.y) };
    desenhar_linha(tela, início, fim, cor);
    return;
  }
  const Flut2D p01 = { (p0.x + p1.x) * 0.5f, (p0.y + p1.y) * 0.5f };
  const Flut2D p12 = { (p1.x + p2.x) * 0.5f, (p1.y + p2.y) * 0.5f };
  const Flut2D p23 = { (p2.x + p3.x) * 0.5f, (p2.y + p3.y) * 0.5f };
  const Flut2D p012 = { (p01.x + p12.x) * 0.5f, (p01.y + p12.y) * 0.5f };
  const Flut2D p123 = { (p12.x + p23.x) * 0.5f, (p12.y + p23.y) * 0.5f };
  const Flut2D p0123 = { (p012.x + p123.x) * 0.5f, (p012.y + p123.y) * 0.5f };
  subdividir_bezier_recursivo(tela, p0, p01, p012, p0123, cor, tolerância_quadrada, profundidade + 1);
  subdividir_bezier_recursivo(tela, p0123, p123, p23, p3, cor, tolerância_quadrada, profundidade + 1);
}

void desenhar_curva_bezier_subdivisão(
  Tela *const tela,
  const Int2D p0,
  const Int2D p1,
  const Int2D p2,
  const Int2D p3,
  const Cor cor,
  const float tolerância_píxeis)
{
  if (tela == nullptr || tela->píxeis == nullptr) return;
  const float tol = tolerância_píxeis > 0.05f ? tolerância_píxeis : 0.05f;
  const float tolerância_quadrada = tol * tol;
  const Flut2D f0 = { (float)p0.x, (float)p0.y };
  const Flut2D f1 = { (float)p1.x, (float)p1.y };
  const Flut2D f2 = { (float)p2.x, (float)p2.y };
  const Flut2D f3 = { (float)p3.x, (float)p3.y };
  subdividir_bezier_recursivo(tela, f0, f1, f2, f3, cor, tolerância_quadrada, 0);
}

static inline Int2D avaliar_catmull_rom(
  const Int2D p0,
  const Int2D p1,
  const Int2D p2,
  const Int2D p3,
  const float t)
{
  const float t2 = t * t;
  const float t3 = t2 * t;
  const float b0 = -0.5f * t3 + t2 - 0.5f * t;
  const float b1 =  1.5f * t3 - 2.5f * t2 + 1.0f;
  const float b2 = -1.5f * t3 + 2.0f * t2 + 0.5f * t;
  const float b3 =  0.5f * t3 - 0.5f * t2;
  const float x = b0 * (float)p0.x + b1 * (float)p1.x + b2 * (float)p2.x + b3 * (float)p3.x;
  const float y = b0 * (float)p0.y + b1 * (float)p1.y + b2 * (float)p2.y + b3 * (float)p3.y;
  return (Int2D){
    .x = (int32_t)roundf(x),
    .y = (int32_t)roundf(y)
  };
}

void desenhar_spline_catmull_rom(
  Tela *const tela,
  const Int2D pontos[],
  const size_t núm_pontos,
  const uint32_t subdivisões_por_segmento,
  const Cor cor)
{
  if (tela == nullptr || tela->píxeis == nullptr || pontos == nullptr) return;
  if (tela->largura == 0 || tela->altura == 0 || núm_pontos == 0) return;
  if (núm_pontos == 1) {
    desenhar_píxel(tela, pontos[0], cor);
    return;
  }
  if (núm_pontos == 2) {
    desenhar_linha(tela, pontos[0], pontos[1], cor);
    return;
  }
  const uint32_t passos = (subdivisões_por_segmento > 0) ? subdivisões_por_segmento : 16;
  const float passo_t = 1.0f / (float)passos;
  for (size_t i = 0; i + 1 < núm_pontos; i++) {
    const Int2D p0 = i == 0 ? pontos[i] : pontos[i - 1];
    const Int2D p1 = pontos[i];
    const Int2D p2 = pontos[i + 1];
    const Int2D p3 = i + 2 < núm_pontos ? pontos[i + 2] : pontos[núm_pontos - 1];
    Int2D anterior = p1;
    for (uint32_t s = 1; s <= passos; s++) {
      const float t = s == passos ? 1.0f : (float)s * passo_t;
      const Int2D atual = avaliar_catmull_rom(p0, p1, p2, p3, t);
      desenhar_linha(tela, anterior, atual, cor);
      anterior = atual;
    }
  }
}

void desenhar_triângulo(Tela *const tela, const Int2D p1, const Int2D p2, const Int2D p3, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  desenhar_linha(tela, p1, p2, cor);
  desenhar_linha(tela, p2, p3, cor);
  desenhar_linha(tela, p3, p1, cor);
}

static inline void ordenar_pontos_y(Int2D *const p1, Int2D *const p2) {
  if (p1->y > p2->y) {
    const Int2D temporário = *p1;
    *p1 = *p2;
    *p2 = temporário;
  }
}

void desenhar_triângulo_preenchido(Tela *const tela, const Int2D p1, const Int2D p2, const Int2D p3, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  Int2D v1 = p1;
  Int2D v2 = p2;
  Int2D v3 = p3;
  ordenar_pontos_y(&v1, &v2);
  ordenar_pontos_y(&v1, &v3);
  ordenar_pontos_y(&v2, &v3);
  if (v2.y == v3.y) desenhar_triângulo_preenchido_base_plana(tela, v1, v2, v3, cor);
  else if (v1.y == v2.y) desenhar_triângulo_preenchido_topo_plano(tela, v1, v2, v3, cor);
  else {
    const int32_t altura_total = v3.y - v1.y;
    const int32_t dx = v3.x - v1.x;
    const int32_t dy = v2.y - v1.y;
    const Int2D v4 = {
      .x = v1.x + (dy * dx) / altura_total,
      .y = v2.y
    };
    desenhar_triângulo_preenchido_base_plana(tela, v1, v2, v4, cor);
    desenhar_triângulo_preenchido_topo_plano(tela, v2, v4, v3, cor);
  }
}

void desenhar_triângulo_preenchido_base_plana(Tela *const tela, const Int2D p1, const Int2D p2, const Int2D p3, const Cor cor) {
  const int32_t altura = p2.y - p1.y;
  if (altura == 0) return;
  const int32_t dx1 = p2.x - p1.x;
  const int32_t dx2 = p3.x - p1.x;
  for (int32_t y = p1.y; y <= p2.y; ++y) {
    const int32_t dy = y - p1.y;
    int32_t x1 = p1.x + (dy * dx1) / altura;
    int32_t x2 = p1.x + (dy * dx2) / altura;
    if (x1 > x2) { const int32_t t = x1; x1 = x2; x2 = t; }
    desenhar_linha(tela, (Int2D){ x1, y }, (Int2D){ x2, y }, cor);
  }
}

void desenhar_triângulo_preenchido_topo_plano(Tela *const tela, const Int2D p1, const Int2D p2, const Int2D p3, const Cor cor) {
  const int32_t altura = p3.y - p1.y;
  if (altura == 0) return;
  const int32_t dx1 = p3.x - p1.x;
  const int32_t dx2 = p3.x - p2.x;
  for (int32_t y = p1.y; y <= p3.y; ++y) {
    const int32_t dy = p3.y - y;
    int32_t x1 = p3.x - (dy * dx1) / altura;
    int32_t x2 = p3.x - (dy * dx2) / altura;
    if (x1 > x2) { const int32_t t = x1; x1 = x2; x2 = t; }
    desenhar_linha(tela, (Int2D){ x1, y }, (Int2D){ x2, y }, cor);
  }
}

void desenhar_retângulo(Tela *const tela, const Int2D superior_esquerdo, const uint32_t largura, const uint32_t altura, const Cor cor) {
  if (largura == 0 || altura == 0) return;
  const int32_t inferior_direito_x = superior_esquerdo.x + (int32_t)largura - 1;
  const int32_t inferior_direito_y = superior_esquerdo.y + (int32_t)altura  - 1;
  const Int2D superior_direito  = { .x =  inferior_direito_x, .y = superior_esquerdo.y };
  const Int2D inferior_esquerdo = { .x = superior_esquerdo.x, .y = inferior_direito_y  };
  const Int2D inferior_direito  = { .x =  inferior_direito_x, .y = inferior_direito_y  };
  desenhar_linha(tela, superior_esquerdo,  superior_direito, cor);
  desenhar_linha(tela, inferior_esquerdo,  inferior_direito, cor);
  desenhar_linha(tela, superior_esquerdo, inferior_esquerdo, cor);
  desenhar_linha(tela,  superior_direito,  inferior_direito, cor);
}

void desenhar_retângulo_preenchido(
  Tela *const tela, const Int2D superior_esquerdo, const uint32_t largura, const uint32_t altura, const Cor cor)
{
  if (tela == nullptr || tela->píxeis == nullptr || largura == 0 || altura == 0) return;
  int32_t início_x = superior_esquerdo.x;
  int32_t início_y = superior_esquerdo.y;
  int32_t fim_x = início_x + (int32_t)largura;
  int32_t fim_y = início_y + (int32_t)altura;
  if (fim_x <= 0 || fim_y <= 0 || início_x >= (int32_t)tela->largura || início_y >= (int32_t)tela->altura) return;
  if (início_x < 0) início_x = 0;
  if (início_y < 0) início_y = 0;
  if (fim_x > (int32_t)tela->largura) fim_x = (int32_t)tela->largura;
  if (fim_y > (int32_t)tela->altura) fim_y = (int32_t)tela->altura;
  for (int32_t y = início_y; y < fim_y; ++y) {
    const uint32_t índice_linha = (uint32_t)y * tela->largura;
    for (int32_t x = início_x; x < fim_x; ++x) tela->píxeis[índice_linha + (uint32_t)x] = cor;
  }
}

void desenhar_retângulo_arredondado(
  Tela *const tela,
  const Int2D superior_esquerdo,
  const uint32_t largura,
  const uint32_t altura,
  uint32_t raio,
  const Cor cor)
{
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) return;
  if (largura == 0 || altura == 0) return;
  if (raio == 0) {
    desenhar_retângulo(tela, superior_esquerdo, largura, altura, cor);
    return;
  }
  const uint32_t metade_menor = (largura < altura ? largura : altura) / 2;
  const int32_t r = (int32_t)(raio > metade_menor ? metade_menor : raio);
  const int32_t x0 = superior_esquerdo.x;
  const int32_t y0 = superior_esquerdo.y;
  const int32_t x1 = x0 + (int32_t)largura - 1;
  const int32_t y1 = y0 + (int32_t)altura - 1;
  const int32_t cx_esq = x0 + r;
  const int32_t cx_dir = x1 - r;
  const int32_t cy_sup = y0 + r;
  const int32_t cy_inf = y1 - r;
  if (cx_esq <= cx_dir)
    for (int32_t x = cx_esq; x <= cx_dir; x++) {
      desenhar_píxel(tela, (Int2D){ .x = x, .y = y0 }, cor);
      desenhar_píxel(tela, (Int2D){ .x = x, .y = y1 }, cor);
    }
  if (cy_sup <= cy_inf)
    for (int32_t y = cy_sup; y <= cy_inf; y++) {
      desenhar_píxel(tela, (Int2D){ .x = x0, .y = y }, cor);
      desenhar_píxel(tela, (Int2D){ .x = x1, .y = y }, cor);
    }
  int32_t x = 0;
  int32_t y = r;
  int32_t d = 1 - r;
  while (x <= y) {
    desenhar_píxel(tela, (Int2D){ .x = cx_esq - x, .y = cy_sup - y }, cor);
    desenhar_píxel(tela, (Int2D){ .x = cx_esq - y, .y = cy_sup - x }, cor);
    desenhar_píxel(tela, (Int2D){ .x = cx_dir + x, .y = cy_sup - y }, cor);
    desenhar_píxel(tela, (Int2D){ .x = cx_dir + y, .y = cy_sup - x }, cor);
    desenhar_píxel(tela, (Int2D){ .x = cx_esq - x, .y = cy_inf + y }, cor);
    desenhar_píxel(tela, (Int2D){ .x = cx_esq - y, .y = cy_inf + x }, cor);
    desenhar_píxel(tela, (Int2D){ .x = cx_dir + x, .y = cy_inf + y }, cor);
    desenhar_píxel(tela, (Int2D){ .x = cx_dir + y, .y = cy_inf + x }, cor);
    if (d < 0) {
      d += 2 * x + 3;
    } else {
      d += 2 * (x - y) + 5;
      y--;
    }
    x++;
  }
}

void desenhar_retângulo_arredondado_preenchido(
  Tela *const tela,
  const Int2D superior_esquerdo,
  const uint32_t largura,
  const uint32_t altura,
  const uint32_t raio,
  const Cor cor)
{
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) return;
  if (largura == 0 || altura == 0) return;
  if (raio == 0) {
    desenhar_retângulo_preenchido(tela, superior_esquerdo, largura, altura, cor);
    return;
  }
  const uint32_t metade_menor = (largura < altura ? largura : altura) / 2;
  const int32_t r = (int32_t)(raio > metade_menor ? metade_menor : raio);
  const int32_t x0 = superior_esquerdo.x;
  const int32_t y0 = superior_esquerdo.y;
  const int32_t x1 = x0 + (int32_t)largura - 1;
  const int32_t y1 = y0 + (int32_t)altura - 1;
  const int32_t cx_esq = x0 + r;
  const int32_t cx_dir = x1 - r;
  const int32_t cy_sup = y0 + r;
  const int32_t cy_inf = y1 - r;
  if (cy_sup <= cy_inf)
    desenhar_retângulo_preenchido(
      tela,
      (Int2D){ .x = x0, .y = cy_sup },
      largura,
      (uint32_t)(cy_inf - cy_sup + 1),
      cor
    );
  int32_t x = 0;
  int32_t y = r;
  int32_t d = 1 - r;
  while (x <= y) {
    if (x > 0 && x != y) {
      const uint32_t larg_larga = (uint32_t)((cx_dir + y) - (cx_esq - y) + 1);
      desenhar_retângulo_preenchido(tela, (Int2D){ .x = cx_esq - y, .y = cy_sup - x }, larg_larga, 1, cor);
      desenhar_retângulo_preenchido(tela, (Int2D){ .x = cx_esq - y, .y = cy_inf + x }, larg_larga, 1, cor);
    }
    if (d < 0) {
      d += 2 * x + 3;
    } else {
      if (y > 0) {
        const uint32_t larg_estreita = (uint32_t)((cx_dir + x) - (cx_esq - x) + 1);
        desenhar_retângulo_preenchido(tela, (Int2D){ .x = cx_esq - x, .y = cy_sup - y }, larg_estreita, 1, cor);
        desenhar_retângulo_preenchido(tela, (Int2D){ .x = cx_esq - x, .y = cy_inf + y }, larg_estreita, 1, cor);
      }
      d += 2 * (x - y) + 5;
      y--;
    }
    x++;
  }
}

void desenhar_gradiente_linear(
  Tela *const tela,
  const Int2D superior_esquerdo,
  const uint32_t largura,
  const uint32_t altura,
  const Cor cor_início,
  const Cor cor_fim,
  const bool vertical)
{
  if (tela == nullptr || tela->píxeis == nullptr || largura == 0 || altura == 0) return;
  int32_t início_x = superior_esquerdo.x;
  int32_t início_y = superior_esquerdo.y;
  int32_t fim_x = início_x + (int32_t)largura;
  int32_t fim_y = início_y + (int32_t)altura;
  if (fim_x <= 0 || fim_y <= 0 || início_x >= (int32_t)tela->largura || início_y >= (int32_t)tela->altura) return;
  int32_t clip_início_x = (início_x < 0) ? 0 : início_x;
  int32_t clip_início_y = (início_y < 0) ? 0 : início_y;
  int32_t clip_fim_x = (fim_x > (int32_t)tela->largura) ? (int32_t)tela->largura : fim_x;
  int32_t clip_fim_y = (fim_y > (int32_t)tela->altura) ? (int32_t)tela->altura : fim_y;
  const float comprimento_x = (float)(largura - 1);
  const float comprimento_y = (float)(altura - 1);
  if (vertical) {
    const float divisor_y = (comprimento_y > 0.0f) ? comprimento_y : 1.0f;
    for (int32_t y = clip_início_y; y < clip_fim_y; ++y) {
      const float t = (float)(y - início_y) / divisor_y;
      const float inv_t = 1.0f - t;
      const Cor cor_linha = (Cor){
        .vermelho = (uint8_t)((float)cor_início.vermelho * inv_t + (float)cor_fim.vermelho * t),
        .verde    = (uint8_t)((float)cor_início.verde    * inv_t + (float)cor_fim.verde    * t),
        .azul     = (uint8_t)((float)cor_início.azul     * inv_t + (float)cor_fim.azul     * t),
        .alfa     = (uint8_t)((float)cor_início.alfa     * inv_t + (float)cor_fim.alfa     * t)
      };
      const uint32_t índice_linha = (uint32_t)y * tela->largura;
      for (int32_t x = clip_início_x; x < clip_fim_x; ++x) {
        tela->píxeis[índice_linha + (uint32_t)x] = cor_linha;
      }
    }
  } else {
    const float divisor_x = (comprimento_x > 0.0f) ? comprimento_x : 1.0f;
    for (int32_t y = clip_início_y; y < clip_fim_y; ++y) {
      const uint32_t índice_linha = (uint32_t)y * tela->largura;
      for (int32_t x = clip_início_x; x < clip_fim_x; ++x) {
        const float t = (float)(x - início_x) / divisor_x;
        const float inv_t = 1.0f - t;
        tela->píxeis[índice_linha + (uint32_t)x] = (Cor){
          .vermelho = (uint8_t)((float)cor_início.vermelho * inv_t + (float)cor_fim.vermelho * t),
          .verde    = (uint8_t)((float)cor_início.verde    * inv_t + (float)cor_fim.verde    * t),
          .azul     = (uint8_t)((float)cor_início.azul     * inv_t + (float)cor_fim.azul     * t),
          .alfa     = (uint8_t)((float)cor_início.alfa     * inv_t + (float)cor_fim.alfa     * t)
        };
      }
    }
  }
}

void desenhar_polígono(Tela *const tela, const Int2D vértices[], const size_t núm_vértices, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || vértices == nullptr || núm_vértices < 3) return;
  for (size_t i = 1; i < núm_vértices; ++i) desenhar_linha(tela, vértices[i - 1], vértices[i], cor);
  desenhar_linha(tela, vértices[núm_vértices - 1], vértices[0], cor);
}

void desenhar_polígono_preenchido(Tela *const tela, const Int2D vértices[], const size_t núm_vértices, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  if (tela->largura == 0 || tela->altura == 0) return;
  if (vértices == nullptr || núm_vértices < 3) return;
  int32_t y_mín = vértices[0].y;
  int32_t y_máx = vértices[0].y;
  for (size_t i = 1; i < núm_vértices; i++) {
    if (vértices[i].y < y_mín) y_mín = vértices[i].y;
    if (vértices[i].y > y_máx) y_máx = vértices[i].y;
  }
  if (y_mín < 0) y_mín = 0;
  if (y_máx >= (int32_t)tela->altura) y_máx = (int32_t)tela->altura - 1;
  if (y_mín > y_máx) return;
  int32_t *interseções_x = calloc(núm_vértices, sizeof(int32_t));
  if (interseções_x == nullptr) return;
  for (int32_t y = y_mín; y <= y_máx; y++) {
    size_t núm_interseções = 0;
    size_t j = núm_vértices - 1;
    for (size_t i = 0; i < núm_vértices; i++) {
      const int32_t yi = vértices[i].y;
      const int32_t yj = vértices[j].y;
      if ((yi < y && yj >= y) || (yj < y && yi >= y)) {
        const float xi = (float)vértices[i].x;
        const float xj = (float)vértices[j].x;
        const float fator = (float)(y - yi) / (float)(yj - yi);
        interseções_x[núm_interseções++] = (int32_t)roundf(xi + fator * (xj - xi));
      }
      j = i;
    }
    if (núm_interseções > 1) qsort(interseções_x, núm_interseções, sizeof(int32_t), comparar_inteiros_32);
    for (size_t k = 0; k + 1 < núm_interseções; k += 2) {
      int32_t x_início = interseções_x[k];
      int32_t x_fim = interseções_x[k + 1];
      if (x_fim < 0 || x_início >= (int32_t)tela->largura) continue;
      if (x_início < 0) x_início = 0;
      if (x_fim >= (int32_t)tela->largura) x_fim = (int32_t)tela->largura - 1;
      Cor *const linha_tela = &tela->píxeis[(size_t)y * tela->largura];
      for (int32_t x = x_início; x <= x_fim; x++) linha_tela[x] = cor;
    }
  }
  free(interseções_x);
}

void desenhar_círculo(Tela *const tela, const Int2D centro, const uint32_t raio, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  if (raio == 0) {
    desenhar_píxel(tela, centro, cor);
    return;
  }
  int32_t x = 0;
  int32_t y = (int32_t)raio;
  int32_t decisão = 3 - 2 * (int32_t)raio;
  while (x <= y) {
    desenhar_píxel(tela, (Int2D){ centro.x + x, centro.y + y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - x, centro.y + y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x + x, centro.y - y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - x, centro.y - y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x + y, centro.y + x }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - y, centro.y + x }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x + y, centro.y - x }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - y, centro.y - x }, cor);
    if (decisão < 0) {
      decisão += 4 * x + 6;
    } else {
      decisão += 4 * (x - y) + 10;
      y--;
    }
    x++;
  }
}

void desenhar_círculo_preenchido(Tela *const tela, const Int2D centro, const uint32_t raio, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  if (raio == 0) {
    desenhar_píxel(tela, centro, cor);
    return;
  }
  int32_t x = 0;
  int32_t y = (int32_t)raio;
  int32_t decisão = 3 - 2 * (int32_t)raio;
  while (x <= y) {
    desenhar_linha(tela, (Int2D){ centro.x - x, centro.y + y }, (Int2D){ centro.x + x, centro.y + y }, cor);
    desenhar_linha(tela, (Int2D){ centro.x - x, centro.y - y }, (Int2D){ centro.x + x, centro.y - y }, cor);
    desenhar_linha(tela, (Int2D){ centro.x - y, centro.y + x }, (Int2D){ centro.x + y, centro.y + x }, cor);
    desenhar_linha(tela, (Int2D){ centro.x - y, centro.y - x }, (Int2D){ centro.x + y, centro.y - x }, cor);
    if (decisão < 0) {
      decisão += 4 * x + 6;
    } else {
      decisão += 4 * (x - y) + 10;
      y--;
    }
    x++;
  }
}

void desenhar_gradiente_radial( Tela *const tela, const Int2D centro, const uint32_t raio, const Cor cor_centro, const Cor cor_periferia)
{
  if (tela == nullptr || tela->píxeis == nullptr || raio == 0) return;
  if (raio == 1) {
    desenhar_píxel(tela, centro, cor_centro);
    return;
  }
  int32_t início_x = centro.x - (int32_t)raio;
  int32_t início_y = centro.y - (int32_t)raio;
  int32_t fim_x = centro.x + (int32_t)raio;
  int32_t fim_y = centro.y + (int32_t)raio;
  if (fim_x <= 0 || fim_y <= 0 || início_x >= (int32_t)tela->largura || início_y >= (int32_t)tela->altura) return;
  if (início_x < 0) início_x = 0;
  if (início_y < 0) início_y = 0;
  if (fim_x > (int32_t)tela->largura) fim_x = (int32_t)tela->largura;
  if (fim_y > (int32_t)tela->altura) fim_y = (int32_t)tela->altura;
  const float raio_f = (float)raio;
  const float raio_quadrado = raio_f * raio_f;
  for (int32_t y = início_y; y < fim_y; ++y) {
    const float dy = (float)(y - centro.y);
    const float dy_quadrado = dy * dy;
    const uint32_t índice_linha = (uint32_t)y * tela->largura;
    for (int32_t x = início_x; x < fim_x; ++x) {
      const float dx = (float)(x - centro.x);
      const float distância_quadrada = (dx * dx) + dy_quadrado;
      if (distância_quadrada > raio_quadrado) continue;
      const float distância = sqrtf(distância_quadrada);
      const float t = distância / raio_f;
      const float inv_t = 1.0f - t;
      tela->píxeis[índice_linha + (uint32_t)x] = (Cor){
        .vermelho = (uint8_t)((float)cor_centro.vermelho * inv_t + (float)cor_periferia.vermelho * t),
        .verde    = (uint8_t)((float)cor_centro.verde    * inv_t + (float)cor_periferia.verde    * t),
        .azul     = (uint8_t)((float)cor_centro.azul     * inv_t + (float)cor_periferia.azul     * t),
        .alfa     = (uint8_t)((float)cor_centro.alfa     * inv_t + (float)cor_periferia.alfa     * t)
      };
    }
  }
}

static inline bool ponto_no_arco(const int32_t dx, const int32_t dy, const float início_norm, const float amplitude, const bool cruza_zero) {
  constexpr float DOIS_PI = 6.28318530717958647692f;
  if (amplitude >= DOIS_PI) return true;
  float ângulo = atan2f((float)dy, (float)dx);
  if (ângulo < 0.0f) ângulo += DOIS_PI;
  const float fim_norm = início_norm + amplitude;
  if (!cruza_zero) return ângulo >= início_norm && ângulo <= fim_norm;
  return ângulo >= início_norm || ângulo <= fim_norm - DOIS_PI;
}

void desenhar_arco(Tela *const tela, const Int2D centro, const uint32_t raio, const float ângulo_início, const float ângulo_fim, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) return;
  constexpr float DOIS_PI = 6.28318530717958647692f;
  const float amplitude = ângulo_fim - ângulo_início;
  if (amplitude <= 0.0f) return;
  if (raio == 0) {
    desenhar_píxel(tela, centro, cor);
    return;
  }
  const float início_norm = normalizar_radianos(ângulo_início);
  const bool cruza_zero = início_norm + amplitude >= DOIS_PI;
  int32_t x = 0;
  int32_t y = (int32_t)raio;
  int32_t d = 1 - (int32_t)raio;
  while (x <= y) {
    const Int2D deslocamentos[8] = {
      {  x,  y }, { -x,  y }, {  x, -y }, { -x, -y },
      {  y,  x }, { -y,  x }, {  y, -x }, { -y, -x }
    };
    for (size_t i = 0; i < 8; i++) {
      const int32_t dx = deslocamentos[i].x;
      const int32_t dy = deslocamentos[i].y;
      if (i == 1 && x == 0) continue;
      if (i == 3 && x == 0) continue;
      if (i >= 4 && x == y) continue;
      if (i == 5 && y == 0) continue;
      if (i == 7 && y == 0) continue;
      if (ponto_no_arco(dx, dy, início_norm, amplitude, cruza_zero))
        desenhar_píxel(tela, (Int2D){ .x = centro.x + dx, .y = centro.y + dy }, cor);
    }
    if (d < 0) {
      d += 2 * x + 3;
    } else {
      d += 2 * (x - y) + 5;
      y--;
    }
    x++;
  }
}

void desenhar_setor_circular(Tela *const tela, const Int2D centro, const uint32_t raio, const float ângulo_início, const float ângulo_fim, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura <= 0 || tela->altura <= 0) return;
  constexpr float DOIS_PI = 6.28318530717958647692f;
  const float amplitude = ângulo_fim - ângulo_início;
  if (amplitude <= 0.0f) return;
  if (raio == 0) {
    desenhar_píxel(tela, centro, cor);
    return;
  }
  if (amplitude >= DOIS_PI) {
    desenhar_círculo_preenchido(tela, centro, raio, cor);
    return;
  }
  const int32_t r = (int32_t)raio;
  const int64_t raio_quadrado = (int64_t)r * (int64_t)r;
  int32_t y_mín = centro.y - r;
  int32_t y_máx = centro.y + r;
  int32_t x_mín = centro.x - r;
  int32_t x_máx = centro.x + r;
  if (y_mín < 0) y_mín = 0;
  if (y_máx >= (int32_t)tela->altura) y_máx = (int32_t)tela->altura - 1;
  if (x_mín < 0) x_mín = 0;
  if (x_máx >= (int32_t)tela->largura) x_máx = (int32_t)tela->largura - 1;
  if (y_mín > y_máx || x_mín > x_máx) return;
  const float início_norm = normalizar_radianos(ângulo_início);
  const float fim_norm = início_norm + amplitude;
  const bool cruza_zero = fim_norm >= DOIS_PI;
  const float fim_ajustado = fim_norm - DOIS_PI;
  for (int32_t y = y_mín; y <= y_máx; y++) {
    const int64_t dy = (int64_t)y - (int64_t)centro.y;
    const int64_t dy2 = dy * dy;
    Cor *const linha_tela = &tela->píxeis[(size_t)y * tela->largura];
    for (int32_t x = x_mín; x <= x_máx; x++) {
      const int64_t dx = (int64_t)x - (int64_t)centro.x;
      if ((dx * dx + dy2) > raio_quadrado) continue;
      if (dx == 0 && dy == 0) {
        linha_tela[x] = cor;
        continue;
      }
      float ângulo = atan2f((float)dy, (float)dx);
      if (ângulo < 0.0f) ângulo += DOIS_PI;
      const bool dentro = !cruza_zero
        ? (ângulo >= início_norm && ângulo <= fim_norm)
        : (ângulo >= início_norm || ângulo <= fim_ajustado);
      if (dentro) linha_tela[x] = cor;
    }
  }
}

void desenhar_elipse(Tela *const tela, const Int2D centro, const int32_t rx, const int32_t ry, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || rx <= 0 || ry <= 0) return;
  const int32_t rx_quadrado = rx * rx;
  const int32_t ry_quadrado = ry * ry;
  int32_t x = 0;
  int32_t y = ry;
  int32_t dx = 2 * ry_quadrado * x;
  int32_t dy = 2 * rx_quadrado * y;
  int32_t decisão = ry_quadrado - (rx_quadrado * ry) + (rx_quadrado / 4);
  while (dx < dy) {
    desenhar_píxel(tela, (Int2D){ centro.x + x, centro.y + y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - x, centro.y + y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x + x, centro.y - y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - x, centro.y - y }, cor);
    if (decisão < 0) {
      x++;
      dx += 2 * ry_quadrado;
      decisão += dx + ry_quadrado;
    } else {
      x++;
      y--;
      dx += 2 * ry_quadrado;
      dy -= 2 * rx_quadrado;
      decisão += dx - dy + ry_quadrado;
    }
  }
  decisão = (ry_quadrado * (x * x + x) + rx_quadrado * (y - 1) * (y - 1) - rx_quadrado * ry_quadrado);
  while (y >= 0) {
    desenhar_píxel(tela, (Int2D){ centro.x + x, centro.y + y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - x, centro.y + y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x + x, centro.y - y }, cor);
    desenhar_píxel(tela, (Int2D){ centro.x - x, centro.y - y }, cor);
    if (decisão > 0) {
      y--;
      dy -= 2 * rx_quadrado;
      decisão += rx_quadrado - dy;
    } else {
      y--;
      x++;
      dx += 2 * ry_quadrado;
      dy -= 2 * rx_quadrado;
      decisão += dx - dy + rx_quadrado;
    }
  }
}

void desenhar_elipse_preenchida(Tela *const tela, const Int2D centro, const int32_t rx, const int32_t ry, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) return;
  if (rx < 0 || ry < 0) return;
  if (rx == 0 && ry == 0) {
    desenhar_linha_horizontal(tela, centro.y, centro.x, centro.x, cor);
    return;
  }
  if (rx == 0) {
    for (int32_t y = centro.y - ry; y <= centro.y + ry; y++) {
      desenhar_linha_horizontal(tela, y, centro.x, centro.x, cor);
    }
    return;
  }
  if (ry == 0) {
    desenhar_linha_horizontal(tela, centro.y, centro.x - rx, centro.x + rx, cor);
    return;
  }
  const int64_t rx2 = (int64_t)rx * (int64_t)rx;
  const int64_t ry2 = (int64_t)ry * (int64_t)ry;
  const int64_t dois_rx2 = 2 * rx2;
  const int64_t dois_ry2 = 2 * ry2;
  int64_t x = 0;
  int64_t y = ry;
  int64_t px = 0;
  int64_t py = dois_rx2 * y;
  int64_t p1 = ry2 - (rx2 * ry) + (rx2 / 4);
  while (px < py) {
    x++;
    px += dois_ry2;
    if (p1 < 0) {
      p1 += ry2 + px;
    } else {
      desenhar_linha_horizontal(tela, centro.y + (int32_t)y, centro.x - (int32_t)(x - 1), centro.x + (int32_t)(x - 1), cor);
      desenhar_linha_horizontal(tela, centro.y - (int32_t)y, centro.x - (int32_t)(x - 1), centro.x + (int32_t)(x - 1), cor);
      y--;
      py -= dois_rx2;
      p1 += ry2 + px - py;
    }
  }
  desenhar_linha_horizontal(tela, centro.y + (int32_t)y, centro.x - (int32_t)x, centro.x + (int32_t)x, cor);
  desenhar_linha_horizontal(tela, centro.y - (int32_t)y, centro.x - (int32_t)x, centro.x + (int32_t)x, cor);
  int64_t p2 = ry2 * (x * x + x) + (ry2 / 4) + rx2 * ((y - 1) * (y - 1)) - rx2 * ry2;
  while (y > 0) {
    y--;
    py -= dois_rx2;
    if (p2 > 0) {
      p2 += rx2 - py;
    } else {
      x++;
      px += dois_ry2;
      p2 += rx2 - py + px;
    }

    if (y > 0) {
      desenhar_linha_horizontal(tela, centro.y + (int32_t)y, centro.x - (int32_t)x, centro.x + (int32_t)x, cor);
      desenhar_linha_horizontal(tela, centro.y - (int32_t)y, centro.x - (int32_t)x, centro.x + (int32_t)x, cor);
    }
  }
  desenhar_linha_horizontal(tela, centro.y, centro.x - rx, centro.x + rx, cor);
}

void preencher_área(Tela *const tela, const Int2D semente, const Cor nova_cor) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) return;
  if (semente.x < 0 || semente.y < 0 || (uint32_t)semente.x >= tela->largura || (uint32_t)semente.y >= tela->altura) return;
  const size_t largura = (size_t)tela->largura;
  const size_t altura = (size_t)tela->altura;
  const Cor cor_alvo = tela->píxeis[(size_t)semente.y * largura + (size_t)semente.x];
  if (cores_iguais(cor_alvo, nova_cor)) return;
  size_t capacidade = 1024;
  size_t topo = 0;
  Int2D *pilha = malloc(capacidade * sizeof(Int2D));
  if (pilha == nullptr) return;
  pilha[topo++] = (Int2D){ .x = semente.x, .y = semente.y };
  while (topo > 0) {
    const Int2D atual = pilha[--topo];
    const int32_t y = atual.y;
    Cor *const linha = &tela->píxeis[(size_t)y * largura];
    if (!cores_iguais(linha[atual.x], cor_alvo)) continue;
    int32_t x_esq = atual.x;
    while (x_esq > 0 && cores_iguais(linha[x_esq - 1], cor_alvo)) x_esq--;
    int32_t x_dir = atual.x;
    while ((size_t)(x_dir + 1) < largura && cores_iguais(linha[x_dir + 1], cor_alvo)) x_dir++;
    for (int32_t x = x_esq; x <= x_dir; x++) linha[x] = nova_cor;
    const int32_t vizinhos_y[2] = { y - 1, y + 1 };
    for (size_t v = 0; v < 2; v++) {
      const int32_t vy = vizinhos_y[v];
      if (vy < 0 || (size_t)vy >= altura) continue;
      const Cor *const linha_vizinha = &tela->píxeis[(size_t)vy * largura];
      bool em_segmento = false;
      for (int32_t x = x_esq; x <= x_dir; x++) {
        if (cores_iguais(linha_vizinha[x], cor_alvo)) {
          if (!em_segmento) {
            if (topo == capacidade) {
              const size_t nova_capacidade = capacidade * 2;
              Int2D *const nova_pilha = realloc(pilha, nova_capacidade * sizeof(Int2D));
              if (nova_pilha == nullptr) {
                free(pilha);
                return;
              }
              pilha = nova_pilha;
              capacidade = nova_capacidade;
            }
            pilha[topo++] = (Int2D){ .x = x, .y = vy };
            em_segmento = true;
          }
        } else {
          em_segmento = false;
        }
      }
    }
  }
  free(pilha);
}

void desenhar_imagem(Tela *const destino, const Tela *const origem, const Int2D posição) {
  if (destino == nullptr || destino->píxeis == nullptr || origem == nullptr || origem->píxeis == nullptr) return;
  for (int32_t y_origem = 0; y_origem < (int32_t)origem->altura; ++y_origem) {
    const int32_t y_destino = posição.y + y_origem;
    if (y_destino < 0 || y_destino >= (int32_t)destino->altura) continue;
    for (int32_t x_origem = 0; x_origem < (int32_t)origem->largura; ++x_origem) {
      const int32_t x_destino = posição.x + x_origem;
      if (x_destino < 0 || x_destino >= (int32_t)destino->largura) continue;
      const size_t índice_origem = (size_t)x_origem + (size_t)y_origem * origem->largura;
      const Cor cor_píxel = origem->píxeis[índice_origem];
      desenhar_píxel_alfa(destino, (Int2D){ x_destino, y_destino }, cor_píxel);
    }
  }
}

void desenhar_imagem_escala(
  Tela *const destino, const Tela *const origem, const Int2D posição, const int32_t largura_nova, const int32_t altura_nova)
{
  if (destino == nullptr || destino->píxeis == nullptr) return;
  if (origem == nullptr || origem->píxeis == nullptr) return;
  if (largura_nova <= 0 || altura_nova <= 0) return;
  const int32_t escala_x = ((int32_t)origem->largura << 16) / largura_nova;
  const int32_t escala_y = ((int32_t)origem->altura << 16) / altura_nova;
  for (int32_t y_dest = 0; y_dest < altura_nova; ++y_dest) {
    const int32_t y_destino_real = posição.y + y_dest;
    if (y_destino_real < 0 || y_destino_real >= (int32_t)destino->altura) continue;
    const int32_t y_origem = (y_dest * escala_y) >> 16;
    for (int32_t x_dest = 0; x_dest < largura_nova; ++x_dest) {
      const int32_t x_destino_real = posição.x + x_dest;
      if (x_destino_real < 0 || x_destino_real >= (int32_t)destino->largura) continue;
      const int32_t x_origem = (x_dest * escala_x) >> 16;
      const size_t índice_origem = (size_t)y_origem * origem->largura + (size_t)x_origem;
      const Cor cor_píxel = origem->píxeis[índice_origem];
      desenhar_píxel_alfa(destino, (Int2D){ x_destino_real, y_destino_real }, cor_píxel);
    }
  }
}

void desenhar_imagem_rodada(Tela *const destino, const Tela *const origem, const Int2D posição, const float radianos, const Int2D pivô)
{
  if (destino == nullptr || destino->píxeis == nullptr) return;
  if (origem == nullptr || origem->píxeis == nullptr) return;
  if (radianos == 0.0f) {
    desenhar_imagem(destino, origem, posição);
    return;
  }
  const float cosseno = cosf(radianos);
  const float seno = sinf(radianos);
  const float dimensão_máxima = sqrtf((float)(origem->largura * origem->largura + origem->altura * origem->altura));
  const int32_t metade_caixa = (int32_t)ceilf(dimensão_máxima / 2.0f);
  const int32_t centro_x = posição.x + pivô.x;
  const int32_t centro_y = posição.y + pivô.y;
  int32_t min_x = centro_x - metade_caixa;
  int32_t max_x = centro_x + metade_caixa;
  int32_t min_y = centro_y - metade_caixa;
  int32_t max_y = centro_y + metade_caixa;
  if (min_x < 0) min_x = 0;
  if (min_y < 0) min_y = 0;
  if (max_x > (int32_t)destino->largura) max_x = (int32_t)destino->largura;
  if (max_y > (int32_t)destino->altura) max_y = (int32_t)destino->altura;
  for (int32_t y_dest = min_y; y_dest < max_y; ++y_dest) {
    const float dy = (float)(y_dest - centro_y);
    for (int32_t x_dest = min_x; x_dest < max_x; ++x_dest) {
      const float dx = (float)(x_dest - centro_x);
      const float fx_origem = (float)pivô.x + (dx * cosseno) + (dy * seno);
      const float fy_origem = (float)pivô.y - (dx * seno) + (dy * cosseno);
      const int32_t x_origem = (int32_t)roundf(fx_origem);
      const int32_t y_origem = (int32_t)roundf(fy_origem);
      if (x_origem >= 0 && x_origem < (int32_t)origem->largura && y_origem >= 0 && y_origem < (int32_t)origem->altura) {
        const size_t índice_origem = (size_t)x_origem + (size_t)y_origem * origem->largura;
        const Cor cor_píxel = origem->píxeis[índice_origem];
        desenhar_píxel_alfa(destino, (Int2D){ x_dest, y_dest }, cor_píxel);
      }
    }
  }
}

void desenhar_subimagem(
  Tela *const destino,
  const Tela *const origem,
  const Int2D pos_destino,
  const Int2D pos_origem,
  const uint32_t larg_sub,
  const uint32_t alt_sub)
{
  if (destino == nullptr || destino->píxeis == nullptr) return;
  if (origem == nullptr || origem->píxeis == nullptr) return;
  if (larg_sub == 0 || alt_sub == 0) return;
  int32_t orig_início_x = pos_origem.x;
  int32_t orig_início_y = pos_origem.y;
  int32_t orig_fim_x = orig_início_x + (int32_t)larg_sub;
  int32_t orig_fim_y = orig_início_y + (int32_t)alt_sub;
  if (orig_início_x < 0) orig_início_x = 0;
  if (orig_início_y < 0) orig_início_y = 0;
  if (orig_fim_x > (int32_t)origem->largura) orig_fim_x = (int32_t)origem->largura;
  if (orig_fim_y > (int32_t)origem->altura) orig_fim_y = (int32_t)origem->altura;
  int32_t larg_efetiva = orig_fim_x - orig_início_x;
  int32_t alt_efetiva = orig_fim_y - orig_início_y;
  if (larg_efetiva <= 0 || alt_efetiva <= 0) return;
  int32_t dest_início_x = pos_destino.x + (orig_início_x - pos_origem.x);
  int32_t dest_início_y = pos_destino.y + (orig_início_y - pos_origem.y);
  int32_t dest_fim_x = dest_início_x + larg_efetiva;
  int32_t dest_fim_y = dest_início_y + alt_efetiva;
  if (dest_início_x < 0) {
    orig_início_x += (0 - dest_início_x);
    dest_início_x = 0;
  }
  if (dest_início_y < 0) {
    orig_início_y += (0 - dest_início_y);
    dest_início_y = 0;
  }
  if (dest_fim_x > (int32_t)destino->largura) dest_fim_x = (int32_t)destino->largura;
  if (dest_fim_y > (int32_t)destino->altura) dest_fim_y = (int32_t)destino->altura;
  larg_efetiva = dest_fim_x - dest_início_x;
  alt_efetiva = dest_fim_y - dest_início_y;
  if (larg_efetiva <= 0 || alt_efetiva <= 0) return;
  for (int32_t dy = 0; dy < alt_efetiva; ++dy) {
    const int32_t y_origem = orig_início_y + dy;
    const int32_t y_destino = dest_início_y + dy;
    const size_t índice_linha_origem = (size_t)y_origem * origem->largura;
    for (int32_t dx = 0; dx < larg_efetiva; ++dx) {
      const int32_t x_origem = orig_início_x + dx;
      const int32_t x_destino = dest_início_x + dx;
      const size_t índice_origem = índice_linha_origem + (size_t)x_origem;
      const Cor cor_píxel = origem->píxeis[índice_origem];
      desenhar_píxel_alfa(destino, (Int2D){ x_destino, y_destino }, cor_píxel);
    }
  }
}

bool exportar_para_ficheiro(const Tela *const tela, const char *const caminho) {
  if (tela == nullptr || tela->píxeis == nullptr || caminho == nullptr) return false;
  FILE *const ficheiro = fopen(caminho, "wb");
  if (ficheiro == nullptr) return false;
  const int resultado_cabeçalho = fprintf(ficheiro, "P6\n%u %u\n255\n", tela->largura, tela->altura);
  if (resultado_cabeçalho < 0) {
    fclose(ficheiro);
    return false;
  }
  const uint32_t área = tela->largura * tela->altura;
  for (uint32_t i = 0; i < área; ++i) {
    const Cor atual = tela->píxeis[i];
    const uint8_t componentes_rgb[3] = {
      atual.vermelho,
      atual.verde,
      atual.azul
    };
    const size_t elementos_escritos = fwrite(componentes_rgb, sizeof(uint8_t), 3, ficheiro);
    if (elementos_escritos != 3) {
      fclose(ficheiro);
      return false;
    }
  }
  fclose(ficheiro);
  return true;
}

void aplicar_desfoque_gaussiano(Tela *const tela, const uint32_t raio) {
  if (tela == nullptr || tela->píxeis == nullptr || raio == 0) return;
  const uint32_t largura = tela->largura;
  const uint32_t altura = tela->altura;
  const uint32_t área = largura * altura;
  const uint32_t tamanho_kernel = 2 * raio + 1;
  float *const kernel = malloc(tamanho_kernel * sizeof(float));
  if (kernel == nullptr) return;
  Cor *const buffer_horizontal = malloc(área * sizeof(Cor));
  if (buffer_horizontal == nullptr) {
    free(kernel);
    return;
  }
  const float sigma = (float)raio / 2.0f;
  const float dois_sigma_quadrado = 2.0f * sigma * sigma;
  constexpr float PI = 3.14159265358979323846f;
  const float pi_raiz = sqrtf(2.0f * PI) * sigma;
  float soma_pesos = 0.0f;
  for (int32_t i = -(int32_t)raio; i <= (int32_t)raio; ++i) {
    const float x = (float)i;
    const size_t índice_k = (size_t)(i + (int32_t)raio);
    kernel[índice_k] = expf(-(x * x) / dois_sigma_quadrado) / pi_raiz;
    soma_pesos += kernel[índice_k];
  }
  for (uint32_t i = 0; i < tamanho_kernel; ++i) {
    kernel[i] /= soma_pesos;
  }
  for (uint32_t y = 0; y < altura; ++y) {
    const size_t índice_linha = (size_t)y * largura;
    for (uint32_t x = 0; x < largura; ++x) {
      float r_acumulado = 0.0f, g_acumulado = 0.0f, b_acumulado = 0.0f;
      for (int32_t k = -(int32_t)raio; k <= (int32_t)raio; ++k) {
        int32_t x_píxel = (int32_t)x + k;
        if (x_píxel < 0) x_píxel = 0;
        if (x_píxel >= (int32_t)largura) x_píxel = (int32_t)largura - 1;
        const float peso = kernel[k + (int32_t)raio];
        const Cor píxel_atual = tela->píxeis[índice_linha + (size_t)x_píxel];
        r_acumulado += (float)píxel_atual.vermelho * peso;
        g_acumulado += (float)píxel_atual.verde * peso;
        b_acumulado += (float)píxel_atual.azul * peso;
      }
      buffer_horizontal[índice_linha + x] = (Cor){
        .vermelho = (uint8_t)roundf(r_acumulado),
        .verde    = (uint8_t)roundf(g_acumulado),
        .azul     = (uint8_t)roundf(b_acumulado),
        .alfa     = tela->píxeis[índice_linha + x].alfa
      };
    }
  }
  for (uint32_t x = 0; x < largura; ++x) {
    for (uint32_t y = 0; y < altura; ++y) {
      float r_acumulado = 0.0f, g_acumulado = 0.0f, b_acumulado = 0.0f;
      for (int32_t k = -(int32_t)raio; k <= (int32_t)raio; ++k) {
        int32_t y_píxel = (int32_t)y + k;
        if (y_píxel < 0) y_píxel = 0;
        if (y_píxel >= (int32_t)altura) y_píxel = (int32_t)altura - 1;
        const float peso = kernel[k + (int32_t)raio];
        const Cor píxel_atual = buffer_horizontal[(size_t)y_píxel * largura + x];
        r_acumulado += (float)píxel_atual.vermelho * peso;
        g_acumulado += (float)píxel_atual.verde * peso;
        b_acumulado += (float)píxel_atual.azul * peso;
      }
      const size_t índice_final = (size_t)y * largura + x;
      tela->píxeis[índice_final] = (Cor){
        .vermelho = (uint8_t)roundf(r_acumulado),
        .verde    = (uint8_t)roundf(g_acumulado),
        .azul     = (uint8_t)roundf(b_acumulado),
        .alfa     = buffer_horizontal[índice_final].alfa
      };
    }
  }
  free(buffer_horizontal);
  free(kernel);
}

void converter_para_tons_de_cinzento(Tela *const tela) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  const uint32_t área = tela->largura * tela->altura;
  for (uint32_t i = 0; i < área; ++i) {
    const Cor atual = tela->píxeis[i];
    const float luminância = (0.2126f * (float)atual.vermelho) +
                             (0.7152f * (float)atual.verde) +
                             (0.0722f * (float)atual.azul);
    const uint8_t tom_cinza = (uint8_t)roundf(luminância);
    tela->píxeis[i].vermelho = tom_cinza;
    tela->píxeis[i].verde    = tom_cinza;
    tela->píxeis[i].azul     = tom_cinza;
  }
}

void aplicar_modulação_alfa(Tela *const tela, const uint8_t alfa) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  if (alfa == 255) return;
  const uint32_t área = tela->largura * tela->altura;
  if (alfa == 0) {
    for (uint32_t i = 0; i < área; ++i) tela->píxeis[i].alfa = 0;
    return;
  }
  for (uint32_t i = 0; i < área; ++i) {
    const uint32_t alfa_atual = tela->píxeis[i].alfa;
    const uint32_t alfa_temporário = alfa_atual * (uint32_t)alfa;
    tela->píxeis[i].alfa = (uint8_t)((alfa_temporário + 1 + (alfa_temporário >> 8)) >> 8);
  }
}

void desenhar_imagem_matriz(Tela *destino, const Tela *origem, Matriz3x3 m) {
  if (destino == nullptr || destino->píxeis == nullptr || origem == nullptr || origem->píxeis == nullptr) return;
  if (destino->largura == 0 || destino->altura == 0 || origem->largura == 0 || origem->altura == 0) return;
  const float larg_orig = (float)origem->largura;
  const float alt_orig  = (float)origem->altura;
  const Flut2D cantos[4] = {
    transformar_flut2d_matriz((Flut2D){ 0.0f,      0.0f     }, m),
    transformar_flut2d_matriz((Flut2D){ larg_orig, 0.0f     }, m),
    transformar_flut2d_matriz((Flut2D){ larg_orig, alt_orig }, m),
    transformar_flut2d_matriz((Flut2D){ 0.0f,      alt_orig }, m)
  };
  float min_x = cantos[0].x;
  float max_x = cantos[0].x;
  float min_y = cantos[0].y;
  float max_y = cantos[0].y;
  for (size_t i = 1; i < 4; i++) {
    if (cantos[i].x < min_x) min_x = cantos[i].x;
    if (cantos[i].x > max_x) max_x = cantos[i].x;
    if (cantos[i].y < min_y) min_y = cantos[i].y;
    if (cantos[i].y > max_y) max_y = cantos[i].y;
  }
  int32_t x_início = (int32_t)floorf(min_x);
  int32_t x_fim    = (int32_t)ceilf(max_x);
  int32_t y_início = (int32_t)floorf(min_y);
  int32_t y_fim    = (int32_t)ceilf(max_y);
  if (x_início < 0) x_início = 0;
  if (x_fim >= (int32_t)destino->largura) x_fim = (int32_t)destino->largura - 1;
  if (y_início < 0) y_início = 0;
  if (y_fim >= (int32_t)destino->altura) y_fim = (int32_t)destino->altura - 1;
  if (x_início > x_fim || y_início > y_fim) return;
  const Matriz3x3 inv_m = inverter_matriz(m);
  const float dux = inv_m.m[0][0];
  const float dvx = inv_m.m[0][1];
  const float duy = inv_m.m[1][0];
  const float dvy = inv_m.m[1][1];
  float linha_u = (float)x_início * inv_m.m[0][0] + (float)y_início * inv_m.m[1][0] + inv_m.m[2][0];
  float linha_v = (float)x_início * inv_m.m[0][1] + (float)y_início * inv_m.m[1][1] + inv_m.m[2][1];
  for (int32_t y = y_início; y <= y_fim; y++) {
    float u = linha_u;
    float v = linha_v;
    for (int32_t x = x_início; x <= x_fim; x++) {
      const int32_t orig_x = (int32_t)floorf(u);
      const int32_t orig_y = (int32_t)floorf(v);
      if (orig_x >= 0 && (uint32_t)orig_x < origem->largura && orig_y >= 0 && (uint32_t)orig_y < origem->altura) {
        const Cor cor_fonte = origem->píxeis[(size_t)orig_y * origem->largura + (size_t)orig_x];
        if (cor_fonte.alfa > 0) desenhar_píxel_alfa(destino, (Int2D){ .x = x, .y = y }, cor_fonte);
      }
      u += dux;
      v += dvx;
    }
    linha_u += duy;
    linha_v += dvy;
  }
}

int main() {
  Tela tela = criar_tela(500, 500);
  if (tela.píxeis == nullptr) return 1;
  limpar_tela(&tela, COR_BRANCA);
  desenhar_píxel(&tela, (Int2D){ 250, 250 }, COR_PRETA);
  desenhar_linha(&tela, (Int2D){ 10, 10 }, (Int2D){ 490, 490 }, COR_VERMELHA);
  desenhar_retângulo(&tela, (Int2D){ 30, 50 }, 120, 80, COR_AZUL);
  desenhar_retângulo_preenchido(&tela, (Int2D){ 30, 350 }, 100, 100, COR_VERDE);
  desenhar_círculo(&tela, (Int2D){ 400, 100 }, 50, COR_AMARELA);
  desenhar_círculo_preenchido(&tela, (Int2D){ 400, 380 }, 60, COR_MAGENTA);
  desenhar_curva_bezier_quadrática(&tela, (Int2D){ 200, 100 }, (Int2D){ 300, 50 }, (Int2D){ 400, 100 }, COR_VERDE);
  // desenhar_curva_bezier_cúbica(&tela, (Int2D){ 100, 150 }, (Int2D){ 150, 50 }, (Int2D){ 250, 250 }, (Int2D){ 300, 150 }, COR_VERMELHA);
  desenhar_curva_bezier_subdivisão(&tela, (Int2D){ 100, 150 }, (Int2D){ 150, 50 }, (Int2D){ 250, 250 }, (Int2D){ 300, 150 }, COR_VERDE, 1);
  if (exportar_para_ficheiro(&tela, "exemplo.ppm")) {
    printf("Imagem guardada com sucesso em 'exemplo.ppm'!\n");
  } else {
    printf("Erro ao exportar o ficheiro.\n");
  }
  destruir_tela(&tela);
  return 0;
}
