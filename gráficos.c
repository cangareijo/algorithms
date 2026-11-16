#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>

int comparar_int32(const void *a, const void *b);

static inline float normalizar_radianos(float ângulo);

typedef struct {
  uint8_t vermelho;
  uint8_t verde;
  uint8_t azul;
  uint8_t alfa;
} Cor;

[[maybe_unused]] constexpr Cor COR_BRANCA   = { .vermelho = 255, .verde = 255, .azul = 255, .alfa = 255 };
[[maybe_unused]] constexpr Cor COR_PRETA    = { .vermelho = 0,   .verde = 0,   .azul = 0,   .alfa = 255 };
[[maybe_unused]] constexpr Cor COR_VERMELHA = { .vermelho = 255, .verde = 0,   .azul = 0,   .alfa = 255 };
[[maybe_unused]] constexpr Cor COR_VERDE    = { .vermelho = 0,   .verde = 255, .azul = 0,   .alfa = 255 };
[[maybe_unused]] constexpr Cor COR_AZUL     = { .vermelho = 0,   .verde = 0,   .azul = 255, .alfa = 255 };
[[maybe_unused]] constexpr Cor COR_CIANO    = { .vermelho = 0,   .verde = 255, .azul = 255, .alfa = 255 };
[[maybe_unused]] constexpr Cor COR_MAGENTA  = { .vermelho = 255, .verde = 0,   .azul = 255, .alfa = 255 };
[[maybe_unused]] constexpr Cor COR_AMARELA  = { .vermelho = 255, .verde = 255, .azul = 0,   .alfa = 255 };

bool cores_iguais(Cor a, Cor b);
Cor interpolar_cor(Cor a, Cor b, float fator);

typedef struct {
  float matiz;
  float saturação;
  float valor;
  float alfa;
} CorHSV;

Cor cor_de_hsv(CorHSV hsv);
CorHSV cor_para_hsv(Cor cor);

typedef struct {
  int32_t x;
  int32_t y;
} Int2D;

void transformar_pontos(const Int2D pontos_origem[], Int2D pontos_destino[], size_t núm_pontos, Int2D translação, float radianos, float escala);
Int2D rodar_ponto(Int2D ponto, Int2D origem, float radianos);
bool ponto_dentro_círculo(Int2D ponto, Int2D centro, uint32_t raio);
bool ponto_dentro_triângulo(Int2D ponto, Int2D p1, Int2D p2, Int2D p3);
bool ponto_dentro_polígono(Int2D ponto, const Int2D vértices[], size_t núm_vértices);
size_t calcular_envoltória_convexa(const Int2D pontos[], size_t núm_pontos, Int2D saída[]);

typedef struct {
  float x;
  float y;
} Flut2D;

Flut2D somar_flut2d(Flut2D a, Flut2D b);
Flut2D subtrair_flut2d(Flut2D a, Flut2D b);
Flut2D escalar_flut2d(Flut2D v, float fator);
float comprimento_flut2d(Flut2D v);
float comprimento_quadrado_flut2d(Flut2D v);
Flut2D normalizar_flut2d(Flut2D v);
float produto_escalar_flut2d(Flut2D a, Flut2D b);
float produto_vetorial_flut2d(Flut2D a, Flut2D b);
float distância_flut2d(Flut2D a, Flut2D b);
float ângulo_entre_flut2d(Flut2D a, Flut2D b);
Flut2D interpolar_flut2d(Flut2D a, Flut2D b, float fator);
Flut2D rodar_flut2d(Flut2D ponto, Flut2D origem, float radianos);
Flut2D int2d_para_flut2d(Int2D ponto);
Int2D flut2d_para_int2d(Flut2D ponto);
bool intersetar_segmentos(Flut2D a1, Flut2D a2, Flut2D b1, Flut2D b2, Flut2D *interseção);

typedef struct {
  Flut2D posição;
  Flut2D uv;
  Cor cor;
} Vértice2D;

typedef struct {
  float x;
  float y;
  float z;
} Flut3D;

Flut3D somar_flut3d(Flut3D a, Flut3D b);
Flut3D subtrair_flut3d(Flut3D a, Flut3D b);
Flut3D escalar_flut3d(Flut3D v, float fator);
float comprimento_flut3d(Flut3D v);
float comprimento_quadrado_flut3d(Flut3D v);
Flut3D normalizar_flut3d(Flut3D v);
float produto_escalar_flut3d(Flut3D a, Flut3D b);
Flut3D produto_vetorial_flut3d(Flut3D a, Flut3D b);
float distância_flut3d(Flut3D a, Flut3D b);
Flut3D interpolar_flut3d(Flut3D a, Flut3D b, float fator);
Flut3D refletir_flut3d(Flut3D incidente, Flut3D normal);

typedef struct {
  float x;
  float y;
  float z;
  float w;
} Flut4D;

typedef struct {
  float x;
  float y;
  float z;
  float w;
} Quaternião;

Quaternião criar_quaternião_identidade(void);
Quaternião criar_quaternião_eixo_ângulo(Flut3D eixo, float radianos);
Quaternião multiplicar_quaterniões(Quaternião a, Quaternião b);
Quaternião normalizar_quaternião(Quaternião q);
Quaternião interpolar_quaternião_slerp(Quaternião a, Quaternião b, float fator);
Flut3D rodar_flut3d_quaternião(Flut3D v, Quaternião q);

typedef struct {
  float m[3][3];
} Matriz3x3;

void criar_matriz_identidade(Matriz3x3 *destino);
void criar_matriz_translação(Matriz3x3 *destino, float tx, float ty);
void criar_matriz_rotação(Matriz3x3 *destino, float radianos);
void criar_matriz_escala(Matriz3x3 *destino, float sx, float sy);
void multiplicar_matrizes(Matriz3x3 *destino, const Matriz3x3 *a, const Matriz3x3 *b);
bool inverter_matriz(Matriz3x3 *destino, const Matriz3x3 *m);
Int2D transformar_int2d_matriz(Int2D ponto, const Matriz3x3 *m);
Flut2D transformar_flut2d_matriz(Flut2D ponto, const Matriz3x3 *m);

typedef struct {
  float m[4][4];
} Matriz4x4;

void criar_matriz4x4_identidade(Matriz4x4 *destino);
void criar_matriz4x4_translação(Matriz4x4 *destino, float tx, float ty, float tz);
void criar_matriz4x4_escala(Matriz4x4 *destino, float sx, float sy, float sz);
void criar_matriz4x4_rotação_x(Matriz4x4 *destino, float radianos);
void criar_matriz4x4_rotação_y(Matriz4x4 *destino, float radianos);
void criar_matriz4x4_rotação_z(Matriz4x4 *destino, float radianos);
void criar_matriz4x4_rotação_eixo(Matriz4x4 *destino, Flut3D eixo, float radianos);
void criar_matriz4x4_visão(Matriz4x4 *destino, Flut3D posição_olho, Flut3D alvo, Flut3D cima);
void criar_matriz4x4_perspetiva(Matriz4x4 *destino, float fov_radianos, float aspeto, float plano_próximo, float plano_distante);
void criar_matriz4x4_ortográfica(Matriz4x4 *destino, float esquerda, float direita, float baixo, float cima, float plano_próximo, float plano_distante);
void multiplicar_matrizes4x4(Matriz4x4 *destino, const Matriz4x4 *a, const Matriz4x4 *b);
bool inverter_matriz4x4(Matriz4x4 *destino, const Matriz4x4 *m);
Flut3D transformar_flut3d_matriz4x4(Flut3D ponto, const Matriz4x4 *m);
Flut4D transformar_flut4d_matriz4x4(Flut4D ponto, const Matriz4x4 *m);
void quaternião_para_matriz4x4(Matriz4x4 *destino, Quaternião q);

typedef enum {
  MISTURA_NORMAL,
  MISTURA_ADITIVA,
  MISTURA_MULTIPLICAR,
  MISTURA_ECRÃ
} Mistura;

typedef struct {
  int32_t x;
  int32_t y;
  uint32_t largura;
  uint32_t altura;
} Retângulo;

bool ponto_dentro_retângulo(Int2D ponto, Retângulo ret);
bool recortar_linha_retângulo(Int2D *início, Int2D *fim, Retângulo limites);
size_t recortar_polígono_retângulo(const Int2D entrada[], size_t núm_entrada, Int2D saída[], size_t cap_saída, Retângulo limites);
bool retângulos_intersetam(Retângulo a, Retângulo b);
bool obter_interseção_retângulos(Retângulo a, Retângulo b, Retângulo *resultado);

typedef struct {
  uint32_t largura;
  uint32_t altura;
  Cor *píxeis;
} Tela;

Tela criar_tela(uint32_t largura, uint32_t altura);
void destruir_tela(Tela *tela);
void limpar_tela(Tela *tela, Cor fundo);

Cor obter_píxel(const Tela *tela, Int2D ponto);
void desenhar_píxel(Tela *tela, Int2D ponto, Cor cor);
void desenhar_píxel_alfa(Tela *tela, Int2D ponto, Cor cor_fonte);
void desenhar_píxel_mistura(Tela *tela, Int2D ponto, Cor cor_fonte, Mistura modo);

void desenhar_linha(Tela *tela, Int2D início, Int2D fim, Cor cor);
void desenhar_linha_horizontal(Tela *tela, int32_t y, int32_t x_início, int32_t x_fim, Cor cor);
void desenhar_linha_espessa(Tela *tela, Int2D início, Int2D fim, uint32_t espessura, Cor cor);
void desenhar_cápsula(Tela *tela, Int2D p1, Int2D p2, uint32_t raio, Cor cor);
void desenhar_cápsula_sdf(Tela *tela, Int2D p1, Int2D p2, uint32_t raio, Cor cor);
void desenhar_cápsula_preenchida(Tela *tela, Int2D p1, Int2D p2, uint32_t raio, Cor cor);
void desenhar_linha_suavizada(Tela *tela, Int2D início, Int2D fim, Cor cor);
void desenhar_polilinha(Tela *tela, const Int2D vértices[], size_t núm_vértices, Cor cor, bool fechada);

void desenhar_curva_bezier_quadrática(Tela *tela, Int2D p0, Int2D p1, Int2D p2, Cor cor);
void desenhar_curva_bezier_cúbica(Tela *tela, Int2D p0, Int2D p1, Int2D p2, Int2D p3, Cor cor);
void desenhar_curva_bezier_adaptativa(Tela *tela, Int2D p0, Int2D p1, Int2D p2, Int2D p3, Cor cor, float tolerância_píxeis);
void desenhar_spline_catmull_rom(Tela *tela, const Int2D pontos[], size_t núm_pontos, uint32_t subdivisões_por_segmento, Cor cor);

void desenhar_triângulo(Tela *tela, Int2D p1, Int2D p2, Int2D p3, Cor cor);
void desenhar_triângulo_preenchido(Tela *tela, Int2D p1, Int2D p2, Int2D p3, Cor cor);
void desenhar_triângulo_gouraud(Tela *tela, Vértice2D v1, Vértice2D v2, Vértice2D v3);
void desenhar_triângulo_texturizado(Tela *tela, const Tela *textura, Vértice2D v1, Vértice2D v2, Vértice2D v3);

void desenhar_retângulo(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, Cor cor);
void desenhar_retângulo_preenchido(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, Cor cor);
void desenhar_retângulo_arredondado(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, uint32_t raio, Cor cor);
void desenhar_retângulo_arredondado_preenchido(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, uint32_t raio, Cor cor);
void desenhar_gradiente_linear(Tela *tela, Int2D superior_esquerdo, uint32_t largura, uint32_t altura, Cor cor_início, Cor cor_fim, bool vertical);
void desenhar_retângulo_mosaico(Tela *tela, Retângulo área, const Tela *padrão, Int2D deslocamento);

void desenhar_polígono(Tela *tela, const Int2D vértices[], size_t núm_vértices, Cor cor);
void desenhar_polígono_preenchido(Tela *tela, const Int2D vértices[], size_t núm_vértices, Cor cor);
void desenhar_polígono_texturizado(Tela *tela, const Tela *textura, const Vértice2D vértices[], size_t núm_vértices);

void desenhar_círculo(Tela *tela, Int2D centro, uint32_t raio, Cor cor);
void desenhar_círculo_preenchido(Tela *tela, Int2D centro, uint32_t raio, Cor cor);
void desenhar_gradiente_radial(Tela *tela, Int2D centro, uint32_t raio, Cor cor_centro, Cor cor_periferia);
void desenhar_arco(Tela *tela, Int2D centro, uint32_t raio, float ângulo_início, float ângulo_fim, Cor cor);
void desenhar_setor_circular(Tela *tela, Int2D centro, uint32_t raio, float ângulo_início, float ângulo_fim, Cor cor);
void desenhar_gradiente_angular(Tela *tela, Int2D centro, uint32_t raio, Cor cor_início, Cor cor_fim, float ângulo_início);
void desenhar_anel_preenchido(Tela *tela, Int2D centro, uint32_t raio_interno, uint32_t raio_externo, Cor cor);

void desenhar_elipse(Tela *tela, Int2D centro, uint32_t rx, uint32_t ry, Cor cor);
void desenhar_elipse_preenchida(Tela *tela, Int2D centro, uint32_t rx, uint32_t ry, Cor cor);

void desenhar_imagem(Tela * destino, const Tela * origem, Int2D posição);
void desenhar_imagem_escala(Tela * destino, const Tela * origem, Int2D posição, uint32_t largura_nova, uint32_t altura_nova);
void desenhar_imagem_rodada(Tela * destino,  const Tela * origem,  Int2D posição,  float radianos,  Int2D pivô);
void desenhar_subimagem(Tela * destino, const Tela * origem, Int2D pos_destino, Int2D pos_origem, uint32_t larg_sub, uint32_t alt_sub);
void desenhar_imagem_mistura(Tela * destino, const Tela * origem, Int2D posição, Mistura modo);
void desenhar_imagem_matriz(Tela * destino, const Tela * origem, Matriz3x3 m);

void preencher_área(Tela *tela, Int2D semente, Cor cor);
void aplicar_matriz_convolução(Tela *tela, Matriz3x3 núcleo, float divisor, float viés);
void aplicar_desfoque_gaussiano(Tela *tela, uint32_t raio);
void converter_para_tons_de_cinzento(Tela *tela);
void aplicar_modulação_alfa(Tela *tela, uint8_t alfa);
void ajustar_brilho_contraste(Tela *tela, float brilho, float contraste);
void inverter_cores(Tela *tela);
void espelhar_tela_horizontal(Tela *tela);
void espelhar_tela_vertical(Tela *tela);
void aplicar_sépia(Tela *tela);
void aplicar_limiar(Tela *tela, uint8_t limiar);
void aplicar_pontilhismo_bayer(Tela *tela, uint8_t níveis_por_canal);
void aplicar_deteção_bordas_sobel(Tela *tela);
void aplicar_nitidez(Tela *tela, float intensidade);
void aplicar_máscara_alfa(Tela *destino, const Tela *máscara, Int2D posição);
Tela extrair_canal_vermelho(const Tela *origem);
Tela extrair_canal_verde(const Tela *origem);
Tela extrair_canal_azul(const Tela *origem);
Tela extrair_canal_alfa(const Tela *origem);
void substituir_cor(Tela *tela, Cor alvo, Cor nova, uint8_t tolerância);

bool atualizar_textura_sdl(SDL_Texture *textura, const Tela *tela);
SDL_Texture *criar_textura_sdl_de_tela(SDL_Renderer *renderizador, const Tela *tela);
bool capturar_renderizador_sdl(SDL_Renderer *renderizador, Tela *destino);

bool carregar_de_ficheiro(Tela *destino, const char *caminho);
bool exportar_para_ficheiro_ppm(const Tela *tela, const char *caminho);
bool exportar_para_ficheiro_bmp(const Tela *tela, const char *caminho);
bool exportar_para_ficheiro_tga(const Tela *tela, const char *caminho);

typedef struct {
  uint32_t largura;
  uint32_t altura;
  float *valores;
} BufferProfundidade;

BufferProfundidade criar_buffer_profundidade(uint32_t largura, uint32_t altura);
void destruir_buffer_profundidade(BufferProfundidade *buffer);
void limpar_buffer_profundidade(BufferProfundidade *buffer, float valor_inicial);



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

bool ponto_dentro_círculo(Int2D ponto, Int2D centro, uint32_t raio) {
  const int64_t dx = (int64_t)ponto.x - (int64_t)centro.x;
  const int64_t dy = (int64_t)ponto.y - (int64_t)centro.y;
  const uint64_t dist_quadrada = (uint64_t)(dx * dx + dy * dy);
  const uint64_t raio_quadrado = (uint64_t)raio * (uint64_t)raio;

  return dist_quadrada <= raio_quadrado;
}

bool ponto_dentro_triângulo(Int2D ponto, Int2D p1, Int2D p2, Int2D p3) {
  const int64_t d1 = ((int64_t)ponto.x - (int64_t)p2.x) * ((int64_t)p1.y - (int64_t)p2.y) -
                     ((int64_t)p1.x - (int64_t)p2.x) * ((int64_t)ponto.y - (int64_t)p2.y);

  const int64_t d2 = ((int64_t)ponto.x - (int64_t)p3.x) * ((int64_t)p2.y - (int64_t)p3.y) -
                     ((int64_t)p2.x - (int64_t)p3.x) * ((int64_t)ponto.y - (int64_t)p3.y);

  const int64_t d3 = ((int64_t)ponto.x - (int64_t)p1.x) * ((int64_t)p3.y - (int64_t)p1.y) -
                     ((int64_t)p3.x - (int64_t)p1.x) * ((int64_t)ponto.y - (int64_t)p1.y);

  const bool tem_negativo = (d1 < 0) || (d2 < 0) || (d3 < 0);
  const bool tem_positivo = (d1 > 0) || (d2 > 0) || (d3 > 0);

  return !(tem_negativo && tem_positivo);
}

bool ponto_dentro_polígono(Int2D ponto, const Int2D vértices[], size_t núm_vértices) {
  if (vértices == nullptr || núm_vértices < 3) {
    return false;
  }

  bool dentro = false;
  const int64_t px = ponto.x;
  const int64_t py = ponto.y;

  for (size_t i = 0, j = núm_vértices - 1; i < núm_vértices; j = i++) {
    const int64_t xi = vértices[i].x;
    const int64_t yi = vértices[i].y;
    const int64_t xj = vértices[j].x;
    const int64_t yj = vértices[j].y;

    const int64_t dx = xj - xi;
    const int64_t dy = yj - yi;
    const int64_t prod_vetorial = (px - xi) * dy - (py - yi) * dx;

    if (prod_vetorial == 0) {
      const int64_t min_x = (xi < xj) ? xi : xj;
      const int64_t max_x = (xi > xj) ? xi : xj;
      const int64_t min_y = (yi < yj) ? yi : yj;
      const int64_t max_y = (yi > yj) ? yi : yj;

      if (px >= min_x && px <= max_x && py >= min_y && py <= max_y) {
        return true;
      }
    }

    const bool cruza_y = (yi > py) != (yj > py);
    if (cruza_y) {
      const int64_t lado_esquerdo = (px - xi) * dy;
      const int64_t lado_direito = (py - yi) * dx;

      if (dy > 0) {
        if (lado_esquerdo < lado_direito) {
          dentro = !dentro;
        }
      } else {
        if (lado_esquerdo > lado_direito) {
          dentro = !dentro;
        }
      }
    }
  }

  return dentro;
}

static int comparar_int2d_lexicográfico(const void *a, const void *b) {
  const Int2D *pa = (const Int2D *)a;
  const Int2D *pb = (const Int2D *)b;

  if (pa->x < pb->x) return -1;
  if (pa->x > pb->x) return 1;
  if (pa->y < pb->y) return -1;
  if (pa->y > pb->y) return 1;
  return 0;
}

static int64_t produto_vetorial_3pontos(Int2D o, Int2D a, Int2D b) {
  return ((int64_t)a.x - (int64_t)o.x) * ((int64_t)b.y - (int64_t)o.y) - ((int64_t)a.y - (int64_t)o.y) * ((int64_t)b.x - (int64_t)o.x);
}

size_t calcular_envoltória_convexa(const Int2D pontos[], size_t núm_pontos, Int2D saída[]) {
  if (pontos == nullptr || saída == nullptr || núm_pontos == 0) {
    return 0;
  }

  if (núm_pontos == 1) {
    saída[0] = pontos[0];
    return 1;
  }

  Int2D *ordenados = malloc(núm_pontos * sizeof(Int2D));
  if (ordenados == nullptr) {
    return 0;
  }

  memcpy(ordenados, pontos, núm_pontos * sizeof(Int2D));
  qsort(ordenados, núm_pontos, sizeof(Int2D), comparar_int2d_lexicográfico);

  size_t k = 0;

  /* Construir a cadeia inferior */
  for (size_t i = 0; i < núm_pontos; ++i) {
    while (k >= 2 && produto_vetorial_3pontos(saída[k - 2], saída[k - 1], ordenados[i]) <= 0) {
      k--;
    }
    saída[k++] = ordenados[i];
  }

  /* Construir a cadeia superior */
  const size_t t = k + 1;
  for (size_t i = núm_pontos - 1; i > 0; --i) {
    const size_t idx = i - 1;
    while (k >= t && produto_vetorial_3pontos(saída[k - 2], saída[k - 1], ordenados[idx]) <= 0) {
      k--;
    }
    saída[k++] = ordenados[idx];
  }

  free(ordenados);

  /* Remover o último ponto pois é duplicado do primeiro */
  return (k > 1) ? (k - 1) : k;
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



Flut3D somar_flut3d(Flut3D a, Flut3D b) {
  return (Flut3D){
    .x = a.x + b.x,
    .y = a.y + b.y,
    .z = a.z + b.z
  };
}

Flut3D subtrair_flut3d(Flut3D a, Flut3D b) {
  return (Flut3D){
    .x = a.x - b.x,
    .y = a.y - b.y,
    .z = a.z - b.z
  };
}

Flut3D escalar_flut3d(Flut3D v, float fator) {
  return (Flut3D){
    .x = v.x * fator,
    .y = v.y * fator,
    .z = v.z * fator
  };
}

float comprimento_flut3d(Flut3D v) {
  return sqrtf(comprimento_quadrado_flut3d(v));
}

float comprimento_quadrado_flut3d(Flut3D v) {
  return produto_escalar_flut3d(v, v);
}

Flut3D normalizar_flut3d(Flut3D v) {
  const float comprimento = comprimento_flut3d(v);
  if (comprimento <= 0.0f) {
    return (Flut3D){ .x = 0.0f, .y = 0.0f, .z = 0.0f };
  }

  const float inverso = 1.0f / comprimento;
  return (Flut3D){
    .x = v.x * inverso,
    .y = v.y * inverso,
    .z = v.z * inverso
  };
}

float produto_escalar_flut3d(Flut3D a, Flut3D b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Flut3D produto_vetorial_flut3d(Flut3D a, Flut3D b) {
  return (Flut3D){
    .x = (a.y * b.z) - (a.z * b.y),
    .y = (a.z * b.x) - (a.x * b.z),
    .z = (a.x * b.y) - (a.y * b.x)
  };
}

float distância_flut3d(Flut3D a, Flut3D b) {
  return comprimento_flut3d(subtrair_flut3d(b, a));
}

Flut3D interpolar_flut3d(Flut3D a, Flut3D b, float fator) {
  return (Flut3D){
    .x = a.x + ((b.x - a.x) * fator),
    .y = a.y + ((b.y - a.y) * fator),
    .z = a.z + ((b.z - a.z) * fator)
  };
}

Flut3D refletir_flut3d(Flut3D incidente, Flut3D normal) {
  const float escalar = 2.0f * produto_escalar_flut3d(incidente, normal);
  return subtrair_flut3d(incidente, escalar_flut3d(normal, escalar));
}



Quaternião criar_quaternião_identidade(void) {
  return (Quaternião){
    .x = 0.0f,
    .y = 0.0f,
    .z = 0.0f,
    .w = 1.0f
  };
}

Quaternião criar_quaternião_eixo_ângulo(Flut3D eixo, float radianos) {
  const Flut3D eixo_norm = normalizar_flut3d(eixo);
  const float meio_ângulo = radianos * 0.5f;
  const float seno = sinf(meio_ângulo);
  const float cosseno = cosf(meio_ângulo);

  return (Quaternião){
    .x = eixo_norm.x * seno,
    .y = eixo_norm.y * seno,
    .z = eixo_norm.z * seno,
    .w = cosseno
  };
}

Quaternião multiplicar_quaterniões(Quaternião a, Quaternião b) {
  return (Quaternião){
    .x = (a.w * b.x) + (a.x * b.w) + (a.y * b.z) - (a.z * b.y),
    .y = (a.w * b.y) - (a.x * b.z) + (a.y * b.w) + (a.z * b.x),
    .z = (a.w * b.z) + (a.x * b.y) - (a.y * b.x) + (a.z * b.w),
    .w = (a.w * b.w) - (a.x * b.x) - (a.y * b.y) - (a.z * b.z)
  };
}

Quaternião normalizar_quaternião(Quaternião q) {
  const float comprimento_quad = (q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
  if (comprimento_quad <= 0.0f) {
    return criar_quaternião_identidade();
  }

  const float inverso = 1.0f / sqrtf(comprimento_quad);
  return (Quaternião){
    .x = q.x * inverso,
    .y = q.y * inverso,
    .z = q.z * inverso,
    .w = q.w * inverso
  };
}

Quaternião interpolar_quaternião_slerp(Quaternião a, Quaternião b, float fator) {
  float produto = (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);

  /* Garante o caminho mais curto na hiperesfera */
  if (produto < 0.0f) {
    b.x = -b.x;
    b.y = -b.y;
    b.z = -b.z;
    b.w = -b.w;
    produto = -produto;
  }

  /* Se os quaterniões estiverem muito próximos, usa interpolação linear normalizada (NLERP) */
  if (produto > 0.9995f) {
    const Quaternião resultado = {
      .x = a.x + ((b.x - a.x) * fator),
      .y = a.y + ((b.y - a.y) * fator),
      .z = a.z + ((b.z - a.z) * fator),
      .w = a.w + ((b.w - a.w) * fator)
    };
    return normalizar_quaternião(resultado);
  }

  const float ângulo = acosf(produto);
  const float seno_ângulo = sinf(ângulo);
  const float peso_a = sinf((1.0f - fator) * ângulo) / seno_ângulo;
  const float peso_b = sinf(fator * ângulo) / seno_ângulo;

  return (Quaternião){
    .x = (a.x * peso_a) + (b.x * peso_b),
    .y = (a.y * peso_a) + (b.y * peso_b),
    .z = (a.z * peso_a) + (b.z * peso_b),
    .w = (a.w * peso_a) + (b.w * peso_b)
  };
}

Flut3D rodar_flut3d_quaternião(Flut3D v, Quaternião q) {
  /* Fórmula otimizada de Rodrigues para quaterniões unitários: v' = v + 2 * (q_xyz × (q_xyz × v + w * v)) */
  const Flut3D q_vec = { .x = q.x, .y = q.y, .z = q.z };
  const Flut3D t = escalar_flut3d(produto_vetorial_flut3d(q_vec, v), 2.0f);

  const Flut3D parcela_w = escalar_flut3d(t, q.w);
  const Flut3D parcela_vetorial = produto_vetorial_flut3d(q_vec, t);

  return somar_flut3d(v, somar_flut3d(parcela_w, parcela_vetorial));
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



void criar_matriz4x4_identidade(Matriz4x4 *destino) {
  assert(destino != nullptr);
  *destino = (Matriz4x4){
    .m = {
      { 1.0f, 0.0f, 0.0f, 0.0f },
      { 0.0f, 1.0f, 0.0f, 0.0f },
      { 0.0f, 0.0f, 1.0f, 0.0f },
      { 0.0f, 0.0f, 0.0f, 1.0f }
    }
  };
}

void criar_matriz4x4_translação(Matriz4x4 *destino, float tx, float ty, float tz) {
  assert(destino != nullptr);
  *destino = (Matriz4x4){
    .m = {
      { 1.0f, 0.0f, 0.0f, 0.0f },
      { 0.0f, 1.0f, 0.0f, 0.0f },
      { 0.0f, 0.0f, 1.0f, 0.0f },
      { tx,   ty,   tz,   1.0f }
    }
  };
}

void criar_matriz4x4_escala(Matriz4x4 *destino, float sx, float sy, float sz) {
  assert(destino != nullptr);
  *destino = (Matriz4x4){
    .m = {
      { sx,   0.0f, 0.0f, 0.0f },
      { 0.0f, sy,   0.0f, 0.0f },
      { 0.0f, 0.0f, sz,   0.0f },
      { 0.0f, 0.0f, 0.0f, 1.0f }
    }
  };
}

void criar_matriz4x4_rotação_x(Matriz4x4 *destino, float radianos) {
  assert(destino != nullptr);
  const float c = cosf(radianos);
  const float s = sinf(radianos);

  *destino = (Matriz4x4){
    .m = {
      { 1.0f, 0.0f, 0.0f, 0.0f },
      { 0.0f, c,    s,    0.0f },
      { 0.0f, -s,   c,    0.0f },
      { 0.0f, 0.0f, 0.0f, 1.0f }
    }
  };
}

void criar_matriz4x4_rotação_y(Matriz4x4 *destino, float radianos) {
  assert(destino != nullptr);
  const float c = cosf(radianos);
  const float s = sinf(radianos);

  *destino = (Matriz4x4){
    .m = {
      { c,    0.0f, -s,   0.0f },
      { 0.0f, 1.0f, 0.0f, 0.0f },
      { s,    0.0f, c,    0.0f },
      { 0.0f, 0.0f, 0.0f, 1.0f }
    }
  };
}

void criar_matriz4x4_rotação_z(Matriz4x4 *destino, float radianos) {
  assert(destino != nullptr);
  const float c = cosf(radianos);
  const float s = sinf(radianos);

  *destino = (Matriz4x4){
    .m = {
      { c,    s,    0.0f, 0.0f },
      { -s,   c,    0.0f, 0.0f },
      { 0.0f, 0.0f, 1.0f, 0.0f },
      { 0.0f, 0.0f, 0.0f, 1.0f }
    }
  };
}

void criar_matriz4x4_rotação_eixo(Matriz4x4 *destino, Flut3D eixo, float radianos) {
  assert(destino != nullptr);
  const Flut3D u = normalizar_flut3d(eixo);
  const float c = cosf(radianos);
  const float s = sinf(radianos);
  const float t = 1.0f - c;

  *destino = (Matriz4x4){
    .m = {
      { (t * u.x * u.x) + c,         (t * u.x * u.y) + (s * u.z), (t * u.x * u.z) - (s * u.y), 0.0f },
      { (t * u.x * u.y) - (s * u.z), (t * u.y * u.y) + c,         (t * u.y * u.z) + (s * u.x), 0.0f },
      { (t * u.x * u.z) + (s * u.y), (t * u.y * u.z) - (s * u.x), (t * u.z * u.z) + c,         0.0f },
      { 0.0f,                        0.0f,                        0.0f,                        1.0f }
    }
  };
}

void criar_matriz4x4_visão(Matriz4x4 *destino, Flut3D posição_olho, Flut3D alvo, Flut3D cima) {
  assert(destino != nullptr);
  /* Sistema de coordenadas mão direita (Right-Handed): câmara olha para -Z */
  const Flut3D frente = normalizar_flut3d(subtrair_flut3d(alvo, posição_olho));
  const Flut3D direita = normalizar_flut3d(produto_vetorial_flut3d(frente, cima));
  const Flut3D cima_real = produto_vetorial_flut3d(direita, frente);

  *destino = (Matriz4x4){
    .m = {
      { direita.x,  cima_real.x, -frente.x, 0.0f },
      { direita.y,  cima_real.y, -frente.y, 0.0f },
      { direita.z,  cima_real.z, -frente.z, 0.0f },
      {
        -produto_escalar_flut3d(direita, posição_olho),
        -produto_escalar_flut3d(cima_real, posição_olho),
        produto_escalar_flut3d(frente, posição_olho),
        1.0f
      }
    }
  };
}

void criar_matriz4x4_perspetiva(Matriz4x4 *destino, float fov_radianos, float aspeto, float plano_próximo, float plano_distante) {
  assert(destino != nullptr);
  assert(aspeto > 0.0f);
  assert(plano_próximo > 0.0f && plano_distante > plano_próximo);

  const float escala_y = 1.0f / tanf(fov_radianos * 0.5f);
  const float escala_x = escala_y / aspeto;
  const float inv_profundidade = 1.0f / (plano_próximo - plano_distante);

  /* Mapeia Z para [0, 1] (estilo DirectX/Vulkan) em sistema mão direita */
  *destino = (Matriz4x4){
    .m = {
      { escala_x, 0.0f,     0.0f,                                         0.0f  },
      { 0.0f,     escala_y, 0.0f,                                         0.0f  },
      { 0.0f,     0.0f,     plano_distante * inv_profundidade,            -1.0f },
      { 0.0f,     0.0f,     plano_próximo * plano_distante * inv_profundidade, 0.0f }
    }
  };
}

void criar_matriz4x4_ortográfica(Matriz4x4 *destino, float esquerda, float direita, float baixo, float cima, float plano_próximo, float plano_distante) {
  assert(destino != nullptr);
  const float inv_largura = 1.0f / (direita - esquerda);
  const float inv_altura = 1.0f / (cima - baixo);
  const float inv_profundidade = 1.0f / (plano_próximo - plano_distante);

  *destino = (Matriz4x4){
    .m = {
      { 2.0f * inv_largura,                 0.0f,                                 0.0f,                             0.0f },
      { 0.0f,                               2.0f * inv_altura,                    0.0f,                             0.0f },
      { 0.0f,                               0.0f,                                 inv_profundidade,                 0.0f },
      { -(direita + esquerda) * inv_largura, -(cima + baixo) * inv_altura,        plano_próximo * inv_profundidade, 1.0f }
    }
  };
}

void multiplicar_matrizes4x4(Matriz4x4 *destino, const Matriz4x4 *a, const Matriz4x4 *b) {
  assert(destino != nullptr && a != nullptr && b != nullptr);
  Matriz4x4 temp = { 0 };

  for (size_t linha = 0; linha < 4; ++linha) {
    for (size_t coluna = 0; coluna < 4; ++coluna) {
      temp.m[linha][coluna] =
        (a->m[linha][0] * b->m[0][coluna]) +
        (a->m[linha][1] * b->m[1][coluna]) +
        (a->m[linha][2] * b->m[2][coluna]) +
        (a->m[linha][3] * b->m[3][coluna]);
    }
  }

  *destino = temp;
}

bool inverter_matriz4x4(Matriz4x4 *destino, const Matriz4x4 *m) {
  assert(destino != nullptr && m != nullptr);
  const float *src = &m->m[0][0];
  float inv[16];

  inv[0] =  src[5] * src[10] * src[15] - src[5] * src[11] * src[14] - src[9] * src[6] * src[15] +
            src[9] * src[7]  * src[14] + src[13] * src[6] * src[11] - src[13] * src[7] * src[10];
  inv[4] = -src[4] * src[10] * src[15] + src[4] * src[11] * src[14] + src[8] * src[6] * src[15] -
            src[8] * src[7]  * src[14] - src[12] * src[6] * src[11] + src[12] * src[7] * src[10];
  inv[8] =  src[4] * src[9]  * src[15] - src[4] * src[11] * src[13] - src[8] * src[5] * src[15] +
            src[8] * src[7]  * src[13] + src[12] * src[5] * src[11] - src[12] * src[7] * src[9];
  inv[12] = -src[4] * src[9] * src[14] + src[4] * src[10] * src[13] + src[8] * src[5] * src[14] -
             src[8] * src[6] * src[13] - src[12] * src[5] * src[10] + src[12] * src[6] * src[9];

  inv[1] = -src[1] * src[10] * src[15] + src[1] * src[11] * src[14] + src[9] * src[2] * src[15] -
            src[9] * src[3]  * src[14] - src[13] * src[2] * src[11] + src[13] * src[3] * src[10];
  inv[5] =  src[0] * src[10] * src[15] - src[0] * src[11] * src[14] - src[8] * src[2] * src[15] +
            src[8] * src[3]  * src[14] + src[12] * src[2] * src[11] - src[12] * src[3] * src[10];
  inv[9] = -src[0] * src[9]  * src[15] + src[0] * src[11] * src[13] + src[8] * src[1] * src[15] -
            src[8] * src[3]  * src[13] - src[12] * src[1] * src[11] + src[12] * src[3] * src[9];
  inv[13] =  src[0] * src[9] * src[14] - src[0] * src[10] * src[13] - src[8] * src[1] * src[14] +
             src[8] * src[2] * src[13] + src[12] * src[1] * src[10] - src[12] * src[2] * src[9];

  inv[2] =  src[1] * src[6] * src[15] - src[1] * src[7] * src[14] - src[5] * src[2] * src[15] +
            src[5] * src[3] * src[14] + src[13] * src[2] * src[7] - src[13] * src[3] * src[6];
  inv[6] = -src[0] * src[6] * src[15] + src[0] * src[7] * src[14] + src[4] * src[2] * src[15] -
            src[4] * src[3] * src[14] - src[12] * src[2] * src[7] + src[12] * src[3] * src[6];
  inv[10] =  src[0] * src[5] * src[15] - src[0] * src[7] * src[13] - src[4] * src[1] * src[15] +
             src[4] * src[3] * src[13] + src[12] * src[1] * src[7] - src[12] * src[3] * src[5];
  inv[14] = -src[0] * src[5] * src[14] + src[0] * src[6] * src[13] + src[4] * src[1] * src[14] -
             src[4] * src[2] * src[13] - src[12] * src[1] * src[6] + src[12] * src[2] * src[5];

  inv[3] = -src[1] * src[6] * src[11] + src[1] * src[7] * src[10] + src[5] * src[2] * src[11] -
            src[5] * src[3] * src[10] - src[9] * src[2] * src[7] + src[9] * src[3] * src[6];
  inv[7] =  src[0] * src[6] * src[11] - src[0] * src[7] * src[10] - src[4] * src[2] * src[11] +
            src[4] * src[3] * src[10] + src[8] * src[2] * src[7] - src[8] * src[3] * src[6];
  inv[11] = -src[0] * src[5] * src[11] + src[0] * src[7] * src[9] + src[4] * src[1] * src[11] -
             src[4] * src[3] * src[9] - src[8] * src[1] * src[7] + src[8] * src[3] * src[5];
  inv[15] =  src[0] * src[5] * src[10] - src[0] * src[6] * src[9] - src[4] * src[1] * src[10] +
             src[4] * src[2] * src[9] + src[8] * src[1] * src[6] - src[8] * src[2] * src[5];

  const float det = (src[0] * inv[0]) + (src[1] * inv[4]) + (src[2] * inv[8]) + (src[3] * inv[12]);
  if (fabsf(det) < 1e-8f) {
    return false;
  }

  const float inv_det = 1.0f / det;
  float *dst = &destino->m[0][0];
  for (size_t i = 0; i < 16; ++i) {
    dst[i] = inv[i] * inv_det;
  }

  return true;
}

Flut3D transformar_flut3d_matriz4x4(Flut3D ponto, const Matriz4x4 *m) {
  assert(m != nullptr);
  const float x = (ponto.x * m->m[0][0]) + (ponto.y * m->m[1][0]) + (ponto.z * m->m[2][0]) + m->m[3][0];
  const float y = (ponto.x * m->m[0][1]) + (ponto.y * m->m[1][1]) + (ponto.z * m->m[2][1]) + m->m[3][1];
  const float z = (ponto.x * m->m[0][2]) + (ponto.y * m->m[1][2]) + (ponto.z * m->m[2][2]) + m->m[3][2];
  const float w = (ponto.x * m->m[0][3]) + (ponto.y * m->m[1][3]) + (ponto.z * m->m[2][3]) + m->m[3][3];

  if (w != 0.0f && w != 1.0f) {
    const float inv_w = 1.0f / w;
    return (Flut3D){ .x = x * inv_w, .y = y * inv_w, .z = z * inv_w };
  }

  return (Flut3D){ .x = x, .y = y, .z = z };
}

Flut4D transformar_flut4d_matriz4x4(Flut4D ponto, const Matriz4x4 *m) {
  assert(m != nullptr);
  return (Flut4D){
    .x = (ponto.x * m->m[0][0]) + (ponto.y * m->m[1][0]) + (ponto.z * m->m[2][0]) + (ponto.w * m->m[3][0]),
    .y = (ponto.x * m->m[0][1]) + (ponto.y * m->m[1][1]) + (ponto.z * m->m[2][1]) + (ponto.w * m->m[3][1]),
    .z = (ponto.x * m->m[0][2]) + (ponto.y * m->m[1][2]) + (ponto.z * m->m[2][2]) + (ponto.w * m->m[3][2]),
    .w = (ponto.x * m->m[0][3]) + (ponto.y * m->m[1][3]) + (ponto.z * m->m[2][3]) + (ponto.w * m->m[3][3])
  };
}

void quaternião_para_matriz4x4(Matriz4x4 *destino, Quaternião q) {
  assert(destino != nullptr);
  const float xx = q.x * q.x;
  const float yy = q.y * q.y;
  const float zz = q.z * q.z;
  const float xy = q.x * q.y;
  const float xz = q.x * q.z;
  const float yz = q.y * q.z;
  const float wx = q.w * q.x;
  const float wy = q.w * q.y;
  const float wz = q.w * q.z;

  *destino = (Matriz4x4){
    .m = {
      { 1.0f - 2.0f * (yy + zz), 2.0f * (xy + wz),        2.0f * (xz - wy),        0.0f },
      { 2.0f * (xy - wz),        1.0f - 2.0f * (xx + zz), 2.0f * (yz + wx),        0.0f },
      { 2.0f * (xz + wy),        2.0f * (yz - wx),        1.0f - 2.0f * (xx + yy), 0.0f },
      { 0.0f,                    0.0f,                    0.0f,                    1.0f }
    }
  };
}



bool ponto_dentro_retângulo(Int2D ponto, Retângulo ret) {
  if (ret.largura == 0 || ret.altura == 0) {
    return false;
  }

  const int64_t px = ponto.x;
  const int64_t py = ponto.y;
  const int64_t rx = ret.x;
  const int64_t ry = ret.y;

  return (px >= rx) && (px < rx + (int64_t)ret.largura) && (py >= ry) && (py < ry + (int64_t)ret.altura);
}

static uint8_t calcular_código_região(double x, double y, double x_min, double y_min, double x_max, double y_max) {
  uint8_t código = 0;

  if (x < x_min) {
    código |= 1u << 0;
  } else if (x > x_max) {
    código |= 1u << 1;
  }

  if (y > y_max) {
    código |= 1u << 2;
  } else if (y < y_min) {
    código |= 1u << 3;
  }

  return código;
}

bool recortar_linha_retângulo(Int2D *início, Int2D *fim, Retângulo limites) {
  if (início == nullptr || fim == nullptr || limites.largura == 0 || limites.altura == 0) {
    return false;
  }

  constexpr uint8_t REG_ESQUERDA = 1u << 0;
  constexpr uint8_t REG_DIREITA  = 1u << 1;
  constexpr uint8_t REG_INFERIOR = 1u << 2;
  constexpr uint8_t REG_SUPERIOR = 1u << 3;

  const double x_min = (double)limites.x;
  const double y_min = (double)limites.y;
  const double x_max = (double)((int64_t)limites.x + (int64_t)limites.largura - 1);
  const double y_max = (double)((int64_t)limites.y + (int64_t)limites.altura - 1);

  double x0 = (double)início->x;
  double y0 = (double)início->y;
  double x1 = (double)fim->x;
  double y1 = (double)fim->y;

  uint8_t cod0 = calcular_código_região(x0, y0, x_min, y_min, x_max, y_max);
  uint8_t cod1 = calcular_código_região(x1, y1, x_min, y_min, x_max, y_max);
  bool aceite = false;

  while (true) {
    if ((cod0 | cod1) == 0) {
      aceite = true;
      break;
    }

    if ((cod0 & cod1) != 0) {
      break;
    }

    const uint8_t cod_fora = (cod0 != 0) ? cod0 : cod1;
    double x = 0.0;
    double y = 0.0;

    if ((cod_fora & REG_INFERIOR) != 0) {
      x = x0 + (x1 - x0) * (y_max - y0) / (y1 - y0);
      y = y_max;
    } else if ((cod_fora & REG_SUPERIOR) != 0) {
      x = x0 + (x1 - x0) * (y_min - y0) / (y1 - y0);
      y = y_min;
    } else if ((cod_fora & REG_DIREITA) != 0) {
      y = y0 + (y1 - y0) * (x_max - x0) / (x1 - x0);
      x = x_max;
    } else if ((cod_fora & REG_ESQUERDA) != 0) {
      y = y0 + (y1 - y0) * (x_min - x0) / (x1 - x0);
      x = x_min;
    }

    if (cod_fora == cod0) {
      x0 = x;
      y0 = y;
      cod0 = calcular_código_região(x0, y0, x_min, y_min, x_max, y_max);
    } else {
      x1 = x;
      y1 = y;
      cod1 = calcular_código_região(x1, y1, x_min, y_min, x_max, y_max);
    }
  }

  if (aceite) {
    início->x = (int32_t)lround(x0);
    início->y = (int32_t)lround(y0);
    fim->x = (int32_t)lround(x1);
    fim->y = (int32_t)lround(y1);
  }

  return aceite;
}

typedef enum {
  BORDA_ESQUERDA,
  BORDA_DIREITA,
  BORDA_SUPERIOR,
  BORDA_INFERIOR
} BordaRecorte;

static bool ponto_dentro_borda(Flut2D p, BordaRecorte borda, float x_min, float x_max, float y_min, float y_max) {
  switch (borda) {
    case BORDA_ESQUERDA: return p.x >= x_min;
    case BORDA_DIREITA:  return p.x <= x_max;
    case BORDA_SUPERIOR: return p.y >= y_min;
    case BORDA_INFERIOR: return p.y <= y_max;
  }
  return false;
}

static Flut2D intersetar_borda(Flut2D a, Flut2D b, BordaRecorte borda, float x_min, float x_max, float y_min, float y_max) {
  const float dx = b.x - a.x;
  const float dy = b.y - a.y;

  switch (borda) {
    case BORDA_ESQUERDA:
      return (Flut2D){ .x = x_min, .y = a.y + dy * (x_min - a.x) / dx };
    case BORDA_DIREITA:
      return (Flut2D){ .x = x_max, .y = a.y + dy * (x_max - a.x) / dx };
    case BORDA_SUPERIOR:
      return (Flut2D){ .x = a.x + dx * (y_min - a.y) / dy, .y = y_min };
    case BORDA_INFERIOR:
      return (Flut2D){ .x = a.x + dx * (y_max - a.y) / dy, .y = y_max };
  }
  return a;
}

size_t recortar_polígono_retângulo(const Int2D entrada[], size_t núm_entrada, Int2D saída[], size_t cap_saída, Retângulo limites) {
  if (entrada == nullptr || saída == nullptr || núm_entrada < 3 || cap_saída == 0 || limites.largura == 0 || limites.altura == 0) {
    return 0;
  }

  /* Um polígono recortado contra 4 arestas pode ganhar no máximo 1 vértice por aresta */
  const size_t cap_máx = núm_entrada + 4;
  Flut2D *buf_a = malloc(cap_máx * sizeof(Flut2D));
  Flut2D *buf_b = malloc(cap_máx * sizeof(Flut2D));

  if (buf_a == nullptr || buf_b == nullptr) {
    free(buf_a);
    free(buf_b);
    return 0;
  }

  for (size_t i = 0; i < núm_entrada; ++i) {
    buf_a[i] = (Flut2D){ .x = (float)entrada[i].x, .y = (float)entrada[i].y };
  }

  const float x_min = (float)limites.x;
  const float y_min = (float)limites.y;
  const float x_max = (float)((int64_t)limites.x + (int64_t)limites.largura - 1);
  const float y_max = (float)((int64_t)limites.y + (int64_t)limites.altura - 1);

  constexpr BordaRecorte bordas[4] = {
    BORDA_ESQUERDA,
    BORDA_DIREITA,
    BORDA_SUPERIOR,
    BORDA_INFERIOR
  };

  size_t contagem = núm_entrada;
  Flut2D *atual = buf_a;
  Flut2D *próximo = buf_b;

  for (size_t b = 0; b < 4; ++b) {
    if (contagem == 0) {
      break;
    }

    const BordaRecorte borda = bordas[b];
    size_t nova_contagem = 0;

    for (size_t i = 0; i < contagem; ++i) {
      const Flut2D p1 = atual[i];
      const Flut2D p2 = atual[(i + 1) % contagem];

      const bool p1_dentro = ponto_dentro_borda(p1, borda, x_min, x_max, y_min, y_max);
      const bool p2_dentro = ponto_dentro_borda(p2, borda, x_min, x_max, y_min, y_max);

      if (p1_dentro && p2_dentro) {
        próximo[nova_contagem++] = p2;
      } else if (p1_dentro && !p2_dentro) {
        próximo[nova_contagem++] = intersetar_borda(p1, p2, borda, x_min, x_max, y_min, y_max);
      } else if (!p1_dentro && p2_dentro) {
        próximo[nova_contagem++] = intersetar_borda(p1, p2, borda, x_min, x_max, y_min, y_max);
        próximo[nova_contagem++] = p2;
      }
    }

    contagem = nova_contagem;
    Flut2D *temp = atual;
    atual = próximo;
    próximo = temp;
  }

  const size_t total_final = (contagem < cap_saída) ? contagem : cap_saída;
  for (size_t i = 0; i < total_final; ++i) {
    saída[i] = (Int2D){
      .x = (int32_t)lroundf(atual[i].x),
      .y = (int32_t)lroundf(atual[i].y)
    };
  }

  free(buf_a);
  free(buf_b);

  return total_final;
}

bool retângulos_intersetam(Retângulo a, Retângulo b) {
  if (a.largura == 0 || a.altura == 0 || b.largura == 0 || b.altura == 0) {
    return false;
  }

  const int64_t a_esq = a.x;
  const int64_t a_dir = a_esq + (int64_t)a.largura;
  const int64_t a_sup = a.y;
  const int64_t a_inf = a_sup + (int64_t)a.altura;

  const int64_t b_esq = b.x;
  const int64_t b_dir = b_esq + (int64_t)b.largura;
  const int64_t b_sup = b.y;
  const int64_t b_inf = b_sup + (int64_t)b.altura;

  return (a_esq < b_dir) && (a_dir > b_esq) && (a_sup < b_inf) && (a_inf > b_sup);
}

bool obter_interseção_retângulos(Retângulo a, Retângulo b, Retângulo *resultado) {
  if (a.largura == 0 || a.altura == 0 || b.largura == 0 || b.altura == 0) {
    return false;
  }

  const int64_t a_esq = a.x;
  const int64_t a_dir = a_esq + (int64_t)a.largura;
  const int64_t a_sup = a.y;
  const int64_t a_inf = a_sup + (int64_t)a.altura;

  const int64_t b_esq = b.x;
  const int64_t b_dir = b_esq + (int64_t)b.largura;
  const int64_t b_sup = b.y;
  const int64_t b_inf = b_sup + (int64_t)b.altura;

  const int64_t int_esq = (a_esq > b_esq) ? a_esq : b_esq;
  const int64_t int_dir = (a_dir < b_dir) ? a_dir : b_dir;
  const int64_t int_sup = (a_sup > b_sup) ? a_sup : b_sup;
  const int64_t int_inf = (a_inf < b_inf) ? a_inf : b_inf;

  if (int_esq >= int_dir || int_sup >= int_inf) {
    return false;
  }

  if (resultado != nullptr) {
    resultado->x = (int32_t)int_esq;
    resultado->y = (int32_t)int_sup;
    resultado->largura = (uint32_t)(int_dir - int_esq);
    resultado->altura = (uint32_t)(int_inf - int_sup);
  }

  return true;
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



Flut2D somar_flut2d(Flut2D a, Flut2D b) {
  return (Flut2D){ .x = a.x + b.x, .y = a.y + b.y };
}

Flut2D subtrair_flut2d(Flut2D a, Flut2D b) {
  return (Flut2D){ .x = a.x - b.x, .y = a.y - b.y };
}

Flut2D escalar_flut2d(Flut2D v, float fator) {
  return (Flut2D){ .x = v.x * fator, .y = v.y * fator };
}

float comprimento_flut2d(Flut2D v) {
  return sqrtf(v.x * v.x + v.y * v.y);
}

float comprimento_quadrado_flut2d(Flut2D v) {
  return v.x * v.x + v.y * v.y;
}

Flut2D normalizar_flut2d(Flut2D v) {
  const float comp = sqrtf(v.x * v.x + v.y * v.y);
  if (comp <= 1e-6f) {
    return (Flut2D){ .x = 0.0f, .y = 0.0f };
  }
  const float inv = 1.0f / comp;
  return (Flut2D){ .x = v.x * inv, .y = v.y * inv };
}

float produto_escalar_flut2d(Flut2D a, Flut2D b) {
  return a.x * b.x + a.y * b.y;
}

float produto_vetorial_flut2d(Flut2D a, Flut2D b) {
  return a.x * b.y - a.y * b.x;
}

float distância_flut2d(Flut2D a, Flut2D b) {
  const float dx = b.x - a.x;
  const float dy = b.y - a.y;
  return sqrtf(dx * dx + dy * dy);
}

float ângulo_entre_flut2d(Flut2D a, Flut2D b) {
  const float vetorial = a.x * b.y - a.y * b.x;
  const float escalar = a.x * b.x + a.y * b.y;
  return atan2f(vetorial, escalar);
}

Flut2D interpolar_flut2d(Flut2D a, Flut2D b, float fator) {
  return (Flut2D){
    .x = a.x + (b.x - a.x) * fator,
    .y = a.y + (b.y - a.y) * fator
  };
}

Flut2D rodar_flut2d(Flut2D ponto, Flut2D origem, float radianos) {
  const float seno = sinf(radianos);
  const float cosseno = cosf(radianos);
  const float dx = ponto.x - origem.x;
  const float dy = ponto.y - origem.y;

  return (Flut2D){
    .x = origem.x + (dx * cosseno - dy * seno),
    .y = origem.y + (dx * seno + dy * cosseno)
  };
}

Flut2D int2d_para_flut2d(Int2D ponto) {
  return (Flut2D){ .x = (float)ponto.x, .y = (float)ponto.y };
}

Int2D flut2d_para_int2d(Flut2D ponto) {
  return (Int2D){ .x = (int32_t)lroundf(ponto.x), .y = (int32_t)lroundf(ponto.y) };
}

bool intersetar_segmentos(Flut2D a1, Flut2D a2, Flut2D b1, Flut2D b2, Flut2D *interseção) {
  const float r_x = a2.x - a1.x;
  const float r_y = a2.y - a1.y;
  const float s_x = b2.x - b1.x;
  const float s_y = b2.y - b1.y;

  const float denominador = r_x * s_y - r_y * s_x;

  /* Segmentos paralelos ou colineares */
  if (fabsf(denominador) <= 1e-6f) {
    return false;
  }

  const float qp_x = b1.x - a1.x;
  const float qp_y = b1.y - a1.y;

  const float inv_denom = 1.0f / denominador;
  const float t = (qp_x * s_y - qp_y * s_x) * inv_denom;
  const float u = (qp_x * r_y - qp_y * r_x) * inv_denom;

  if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
    if (interseção != nullptr) {
      interseção->x = a1.x + t * r_x;
      interseção->y = a1.y + t * r_y;
    }
    return true;
  }

  return false;
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

void desenhar_cápsula(Tela *tela, Int2D p1, Int2D p2, uint32_t raio, Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return;
  }

  /* Se o raio for nulo, a cápsula degenera num segmento de reta simples */
  if (raio == 0) {
    desenhar_linha(tela, p1, p2, cor);
    return;
  }

  const float dx = (float)((int64_t)p2.x - (int64_t)p1.x);
  const float dy = (float)((int64_t)p2.y - (int64_t)p1.y);
  const float comprimento = hypotf(dx, dy);

  /* Se os dois centros coincidirem, a cápsula degenera num círculo */
  if (comprimento <= 1e-6f) {
    desenhar_círculo(tela, p1, raio, cor);
    return;
  }

  constexpr float MEIO_PI = 1.57079632679489661923f;
  const float r = (float)raio;

  /* Vetor normal unitário perpendicular ao segmento (rodado -90 graus) */
  const float nx = dy / comprimento;
  const float ny = -dx / comprimento;

  const int32_t desl_x = (int32_t)lroundf(nx * r);
  const int32_t desl_y = (int32_t)lroundf(ny * r);

  /* Desenhar as duas arestas laterais paralelas ao eixo central */
  const Int2D lado1_início = { .x = p1.x + desl_x, .y = p1.y + desl_y };
  const Int2D lado1_fim    = { .x = p2.x + desl_x, .y = p2.y + desl_y };

  const Int2D lado2_início = { .x = p1.x - desl_x, .y = p1.y - desl_y };
  const Int2D lado2_fim    = { .x = p2.x - desl_x, .y = p2.y - desl_y };

  desenhar_linha(tela, lado1_início, lado1_fim, cor);
  desenhar_linha(tela, lado2_início, lado2_fim, cor);

  /* Desenhar os dois semicírculos nas extremidades em torno de p1 e p2 */
  const float ângulo_eixo = atan2f(dy, dx);

  desenhar_arco(tela, p2, raio, ângulo_eixo - MEIO_PI, ângulo_eixo + MEIO_PI, cor);
  desenhar_arco(tela, p1, raio, ângulo_eixo + MEIO_PI, ângulo_eixo + 3.0f * MEIO_PI, cor);
}

static inline bool píxel_dentro_cápsula(float px, float py, float ax, float ay, float ab_x, float ab_y, float inv_ab_comp_quad, float raio_quadrado) {
  const float ap_x = px - ax;
  const float ap_y = py - ay;

  float t = (ap_x * ab_x + ap_y * ab_y) * inv_ab_comp_quad;
  if (t < 0.0f) {
    t = 0.0f;
  } else if (t > 1.0f) {
    t = 1.0f;
  }

  const float dx = ap_x - t * ab_x;
  const float dy = ap_y - t * ab_y;

  return (dx * dx + dy * dy) <= raio_quadrado;
}

void desenhar_cápsula_sdf(Tela *tela, Int2D p1, Int2D p2, uint32_t raio, Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return;
  }

  if (raio == 0) {
    desenhar_linha(tela, p1, p2, cor);
    return;
  }

  const int64_t r = (int64_t)raio;
  const float raio_quadrado = (float)((uint64_t)raio * (uint64_t)raio);

  const int64_t min_x_seg = (p1.x < p2.x) ? p1.x : p2.x;
  const int64_t max_x_seg = (p1.x > p2.x) ? p1.x : p2.x;
  const int64_t min_y_seg = (p1.y < p2.y) ? p1.y : p2.y;
  const int64_t max_y_seg = (p1.y > p2.y) ? p1.y : p2.y;

  const int64_t x0 = (min_x_seg - r > 0) ? (min_x_seg - r) : 0;
  const int64_t y0 = (min_y_seg - r > 0) ? (min_y_seg - r) : 0;
  const int64_t x1 = (max_x_seg + r < (int64_t)tela->largura - 1) ? (max_x_seg + r) : ((int64_t)tela->largura - 1);
  const int64_t y1 = (max_y_seg + r < (int64_t)tela->altura - 1)  ? (max_y_seg + r) : ((int64_t)tela->altura - 1);

  if (x0 > x1 || y0 > y1) {
    return;
  }

  const float ax = (float)p1.x;
  const float ay = (float)p1.y;
  const float ab_x = (float)((int64_t)p2.x - (int64_t)p1.x);
  const float ab_y = (float)((int64_t)p2.y - (int64_t)p1.y);
  const float ab_comp_quad = ab_x * ab_x + ab_y * ab_y;
  const float inv_ab_comp_quad = (ab_comp_quad > 1e-6f) ? (1.0f / ab_comp_quad) : 0.0f;

  for (int64_t y = y0; y <= y1; ++y) {
    const float fy = (float)y;
    Cor *linha = &tela->píxeis[(size_t)y * tela->largura];

    for (int64_t x = x0; x <= x1; ++x) {
      const float fx = (float)x;

      if (!píxel_dentro_cápsula(fx, fy, ax, ay, ab_x, ab_y, inv_ab_comp_quad, raio_quadrado)) {
        continue;
      }

      /* Se algum vizinho ortogonal estiver fora, este píxel faz parte da fronteira exata */
      const bool esq_fora = !píxel_dentro_cápsula(fx - 1.0f, fy, ax, ay, ab_x, ab_y, inv_ab_comp_quad, raio_quadrado);
      const bool dir_fora = !píxel_dentro_cápsula(fx + 1.0f, fy, ax, ay, ab_x, ab_y, inv_ab_comp_quad, raio_quadrado);
      const bool cim_fora = !píxel_dentro_cápsula(fx, fy - 1.0f, ax, ay, ab_x, ab_y, inv_ab_comp_quad, raio_quadrado);
      const bool bai_fora = !píxel_dentro_cápsula(fx, fy + 1.0f, ax, ay, ab_x, ab_y, inv_ab_comp_quad, raio_quadrado);

      if (esq_fora || dir_fora || cim_fora || bai_fora) {
        linha[x] = cor;
      }
    }
  }
}

void desenhar_cápsula_preenchida(Tela *tela, Int2D p1, Int2D p2, uint32_t raio, Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return;
  }

  /* Se o raio for nulo, a cápsula degenera numa linha simples */
  if (raio == 0) {
    desenhar_linha(tela, p1, p2, cor);
    return;
  }

  const int64_t r = (int64_t)raio;
  const float raio_quadrado = (float)((uint64_t)raio * (uint64_t)raio);

  /* Calcular caixa envolvente dos dois círculos extremos e recortar à tela */
  const int64_t min_x_seg = (p1.x < p2.x) ? p1.x : p2.x;
  const int64_t max_x_seg = (p1.x > p2.x) ? p1.x : p2.x;
  const int64_t min_y_seg = (p1.y < p2.y) ? p1.y : p2.y;
  const int64_t max_y_seg = (p1.y > p2.y) ? p1.y : p2.y;

  const int64_t x0 = (min_x_seg - r > 0) ? (min_x_seg - r) : 0;
  const int64_t y0 = (min_y_seg - r > 0) ? (min_y_seg - r) : 0;
  const int64_t x1 = (max_x_seg + r < (int64_t)tela->largura - 1) ? (max_x_seg + r) : ((int64_t)tela->largura - 1);
  const int64_t y1 = (max_y_seg + r < (int64_t)tela->altura - 1)  ? (max_y_seg + r) : ((int64_t)tela->altura - 1);

  if (x0 > x1 || y0 > y1) {
    return;
  }

  const float ax = (float)p1.x;
  const float ay = (float)p1.y;
  const float ab_x = (float)((int64_t)p2.x - (int64_t)p1.x);
  const float ab_y = (float)((int64_t)p2.y - (int64_t)p1.y);
  const float ab_comp_quad = ab_x * ab_x + ab_y * ab_y;
  const float inv_ab_comp_quad = (ab_comp_quad > 1e-6f) ? (1.0f / ab_comp_quad) : 0.0f;

  for (int64_t y = y0; y <= y1; ++y) {
    const float ap_y = (float)y - ay;
    Cor *linha = &tela->píxeis[(size_t)y * tela->largura];

    for (int64_t x = x0; x <= x1; ++x) {
      const float ap_x = (float)x - ax;

      /* Projetar o píxel sobre o segmento e limitar t ao intervalo [0.0f, 1.0f] */
      float t = (ap_x * ab_x + ap_y * ab_y) * inv_ab_comp_quad;
      if (t < 0.0f) {
        t = 0.0f;
      } else if (t > 1.0f) {
        t = 1.0f;
      }

      const float proj_x = ax + t * ab_x;
      const float proj_y = ay + t * ab_y;

      const float dx = (float)x - proj_x;
      const float dy = (float)y - proj_y;

      if ((dx * dx + dy * dy) <= raio_quadrado) {
        linha[x] = cor;
      }
    }
  }
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

static float produto_vetorial_arestas(Flut2D a, Flut2D b, Flut2D c) {
  return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

void desenhar_triângulo_gouraud(Tela *tela, Vértice2D v1, Vértice2D v2, Vértice2D v3) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return;
  }

  const float área = produto_vetorial_arestas(v1.posição, v2.posição, v3.posição);
  if (fabsf(área) <= 1e-6f) {
    return;
  }

  const float inv_área = 1.0f / área;
  const float sinal = (área > 0.0f) ? 1.0f : -1.0f;

  /* Calcular caixa envolvente e recortar aos limites da tela */
  const float min_xf = fminf(v1.posição.x, fminf(v2.posição.x, v3.posição.x));
  const float max_xf = fmaxf(v1.posição.x, fmaxf(v2.posição.x, v3.posição.x));
  const float min_yf = fminf(v1.posição.y, fminf(v2.posição.y, v3.posição.y));
  const float max_yf = fmaxf(v1.posição.y, fmaxf(v2.posição.y, v3.posição.y));

  int32_t min_x = (int32_t)floorf(min_xf);
  int32_t max_x = (int32_t)ceilf(max_xf);
  int32_t min_y = (int32_t)floorf(min_yf);
  int32_t max_y = (int32_t)ceilf(max_yf);

  if (min_x < 0) min_x = 0;
  if (min_y < 0) min_y = 0;
  if (max_x >= (int32_t)tela->largura) max_x = (int32_t)tela->largura - 1;
  if (max_y >= (int32_t)tela->altura)  max_y = (int32_t)tela->altura - 1;

  if (min_x > max_x || min_y > max_y) {
    return;
  }

  for (int32_t y = min_y; y <= max_y; ++y) {
    Cor *linha = &tela->píxeis[(size_t)y * tela->largura];

    for (int32_t x = min_x; x <= max_x; ++x) {
      /* Amostragem no centro do píxel */
      const Flut2D p = { .x = (float)x + 0.5f, .y = (float)y + 0.5f };

      const float e1 = produto_vetorial_arestas(v2.posição, v3.posição, p);
      const float e2 = produto_vetorial_arestas(v3.posição, v1.posição, p);
      const float e3 = produto_vetorial_arestas(v1.posição, v2.posição, p);

      /* Verifica se o ponto está dentro do triângulo (independentemente da orientação) */
      if ((e1 * sinal >= 0.0f) && (e2 * sinal >= 0.0f) && (e3 * sinal >= 0.0f)) {
        const float w1 = e1 * inv_área;
        const float w2 = e2 * inv_área;
        const float w3 = e3 * inv_área;

        const float r = w1 * (float)v1.cor.vermelho + w2 * (float)v2.cor.vermelho + w3 * (float)v3.cor.vermelho;
        const float g = w1 * (float)v1.cor.verde    + w2 * (float)v2.cor.verde    + w3 * (float)v3.cor.verde;
        const float b = w1 * (float)v1.cor.azul     + w2 * (float)v2.cor.azul     + w3 * (float)v3.cor.azul;
        const float a = w1 * (float)v1.cor.alfa     + w2 * (float)v2.cor.alfa     + w3 * (float)v3.cor.alfa;

        linha[x] = (Cor){
          .vermelho = (uint8_t)lroundf(fminf(fmaxf(r, 0.0f), 255.0f)),
          .verde    = (uint8_t)lroundf(fminf(fmaxf(g, 0.0f), 255.0f)),
          .azul     = (uint8_t)lroundf(fminf(fmaxf(b, 0.0f), 255.0f)),
          .alfa     = (uint8_t)lroundf(fminf(fmaxf(a, 0.0f), 255.0f))
        };
      }
    }
  }
}

void desenhar_triângulo_texturizado(Tela *tela, const Tela *textura, Vértice2D v1, Vértice2D v2, Vértice2D v3) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return;
  }

  if (textura == nullptr || textura->píxeis == nullptr || textura->largura == 0 || textura->altura == 0) {
    return;
  }

  const float área = produto_vetorial_arestas(v1.posição, v2.posição, v3.posição);
  if (fabsf(área) <= 1e-6f) {
    return;
  }

  const float inv_área = 1.0f / área;
  const float sinal = (área > 0.0f) ? 1.0f : -1.0f;

  /* Calcular caixa envolvente e recortar aos limites da tela */
  const float min_xf = fminf(v1.posição.x, fminf(v2.posição.x, v3.posição.x));
  const float max_xf = fmaxf(v1.posição.x, fmaxf(v2.posição.x, v3.posição.x));
  const float min_yf = fminf(v1.posição.y, fminf(v2.posição.y, v3.posição.y));
  const float max_yf = fmaxf(v1.posição.y, fmaxf(v2.posição.y, v3.posição.y));

  int32_t min_x = (int32_t)floorf(min_xf);
  int32_t max_x = (int32_t)ceilf(max_xf);
  int32_t min_y = (int32_t)floorf(min_yf);
  int32_t max_y = (int32_t)ceilf(max_yf);

  if (min_x < 0) min_x = 0;
  if (min_y < 0) min_y = 0;
  if (max_x >= (int32_t)tela->largura) max_x = (int32_t)tela->largura - 1;
  if (max_y >= (int32_t)tela->altura)  max_y = (int32_t)tela->altura - 1;

  if (min_x > max_x || min_y > max_y) {
    return;
  }

  const float max_u_tex = (float)(textura->largura - 1);
  const float max_v_tex = (float)(textura->altura - 1);

  for (int32_t y = min_y; y <= max_y; ++y) {
    Cor *linha = &tela->píxeis[(size_t)y * tela->largura];

    for (int32_t x = min_x; x <= max_x; ++x) {
      /* Amostragem no centro do píxel */
      const Flut2D p = { .x = (float)x + 0.5f, .y = (float)y + 0.5f };

      const float e1 = produto_vetorial_arestas(v2.posição, v3.posição, p);
      const float e2 = produto_vetorial_arestas(v3.posição, v1.posição, p);
      const float e3 = produto_vetorial_arestas(v1.posição, v2.posição, p);

      if ((e1 * sinal >= 0.0f) && (e2 * sinal >= 0.0f) && (e3 * sinal >= 0.0f)) {
        const float w1 = e1 * inv_área;
        const float w2 = e2 * inv_área;
        const float w3 = e3 * inv_área;

        /* Interpolar coordenadas UV e limitar ao intervalo [0.0f, 1.0f] */
        const float u = fminf(fmaxf(w1 * v1.uv.x + w2 * v2.uv.x + w3 * v3.uv.x, 0.0f), 1.0f);
        const float v = fminf(fmaxf(w1 * v1.uv.y + w2 * v2.uv.y + w3 * v3.uv.y, 0.0f), 1.0f);

        const uint32_t tex_x = (uint32_t)lroundf(u * max_u_tex);
        const uint32_t tex_y = (uint32_t)lroundf(v * max_v_tex);
        const Cor texel = textura->píxeis[(size_t)tex_y * textura->largura + tex_x];

        /* Interpolar cor dos vértices para modulação (tinting) */
        const float r_vert = w1 * (float)v1.cor.vermelho + w2 * (float)v2.cor.vermelho + w3 * (float)v3.cor.vermelho;
        const float g_vert = w1 * (float)v1.cor.verde    + w2 * (float)v2.cor.verde    + w3 * (float)v3.cor.verde;
        const float b_vert = w1 * (float)v1.cor.azul     + w2 * (float)v2.cor.azul     + w3 * (float)v3.cor.azul;
        const float a_vert = w1 * (float)v1.cor.alfa     + w2 * (float)v2.cor.alfa     + w3 * (float)v3.cor.alfa;

        constexpr float inv_255 = 1.0f / 255.0f;
        const float r_final = (float)texel.vermelho * (r_vert * inv_255);
        const float g_final = (float)texel.verde    * (g_vert * inv_255);
        const float b_final = (float)texel.azul     * (b_vert * inv_255);
        const float a_final = (float)texel.alfa     * (a_vert * inv_255);

        linha[x] = (Cor){
          .vermelho = (uint8_t)lroundf(fminf(fmaxf(r_final, 0.0f), 255.0f)),
          .verde    = (uint8_t)lroundf(fminf(fmaxf(g_final, 0.0f), 255.0f)),
          .azul     = (uint8_t)lroundf(fminf(fmaxf(b_final, 0.0f), 255.0f)),
          .alfa     = (uint8_t)lroundf(fminf(fmaxf(a_final, 0.0f), 255.0f))
        };
      }
    }
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
  int32_t x_iní = superior_esquerdo.x;
  int32_t y_iní = superior_esquerdo.y;
  int32_t x_fim = x_iní + (int32_t)largura - 1;
  int32_t y_fim = y_iní + (int32_t)altura - 1;

  for (int32_t y = y_iní; y <= y_fim; y++) {
    desenhar_linha_horizontal(tela, y, x_iní, x_fim, cor);
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

static inline uint32_t módulo_positivo(int64_t valor, uint32_t divisor) {
  const int64_t d = (int64_t)divisor;
  int64_t resto = valor % d;
  if (resto < 0) {
    resto += d;
  }
  return (uint32_t)resto;
}

void desenhar_retângulo_mosaico(Tela *tela, Retângulo área, const Tela *padrão, Int2D deslocamento) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return;
  }

  if (padrão == nullptr || padrão->píxeis == nullptr || padrão->largura == 0 || padrão->altura == 0) {
    return;
  }

  if (área.largura == 0 || área.altura == 0) {
    return;
  }

  /* Calcular limites recortados à tela em 64 bits para evitar overflows */
  const int64_t x0 = (área.x > 0) ? (int64_t)área.x : 0;
  const int64_t y0 = (área.y > 0) ? (int64_t)área.y : 0;

  const int64_t área_dir = (int64_t)área.x + (int64_t)área.largura;
  const int64_t área_inf = (int64_t)área.y + (int64_t)área.altura;

  const int64_t x1 = (área_dir < (int64_t)tela->largura) ? área_dir : (int64_t)tela->largura;
  const int64_t y1 = (área_inf < (int64_t)tela->altura)  ? área_inf : (int64_t)tela->altura;

  if (x0 >= x1 || y0 >= y1) {
    return;
  }

  for (int64_t y = y0; y < y1; ++y) {
    const uint32_t pat_y = módulo_positivo((y - (int64_t)área.y) + (int64_t)deslocamento.y, padrão->altura);
    const Cor *linha_padrão = &padrão->píxeis[(size_t)pat_y * padrão->largura];
    Cor *linha_destino = &tela->píxeis[(size_t)y * tela->largura];

    for (int64_t x = x0; x < x1; ++x) {
      const uint32_t pat_x = módulo_positivo((x - (int64_t)área.x) + (int64_t)deslocamento.x, padrão->largura);
      linha_destino[x] = linha_padrão[pat_x];
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

static float área_assinada_polígono_v2d(const Vértice2D vértices[], size_t núm_vértices) {
  float soma = 0.0f;
  for (size_t i = 0; i < núm_vértices; ++i) {
    const Flut2D atual = vértices[i].posição;
    const Flut2D seg = vértices[(i + 1) % núm_vértices].posição;
    soma += (atual.x * seg.y) - (seg.x * atual.y);
  }
  return soma * 0.5f;
}

static bool ponto_flut_em_triângulo(Flut2D p, Flut2D a, Flut2D b, Flut2D c) {
  const float d1 = (p.x - b.x) * (a.y - b.y) - (a.x - b.x) * (p.y - b.y);
  const float d2 = (p.x - c.x) * (b.y - c.y) - (b.x - c.x) * (p.y - c.y);
  const float d3 = (p.x - a.x) * (c.y - a.y) - (c.x - a.x) * (p.y - a.y);

  const bool tem_neg = (d1 < 0.0f) || (d2 < 0.0f) || (d3 < 0.0f);
  const bool tem_pos = (d1 > 0.0f) || (d2 > 0.0f) || (d3 > 0.0f);

  return !(tem_neg && tem_pos);
}

void desenhar_polígono_texturizado(Tela *tela, const Tela *textura, const Vértice2D vértices[], size_t núm_vértices) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return;
  }

  if (textura == nullptr || textura->píxeis == nullptr || textura->largura == 0 || textura->altura == 0) {
    return;
  }

  if (vértices == nullptr || núm_vértices < 3) {
    return;
  }

  /* Caso trivial de 3 vértices */
  if (núm_vértices == 3) {
    desenhar_triângulo_texturizado(tela, textura, vértices[0], vértices[1], vértices[2]);
    return;
  }

  const float área_total = área_assinada_polígono_v2d(vértices, núm_vértices);
  if (fabsf(área_total) <= 1e-6f) {
    return;
  }

  const float sinal_orientação = (área_total > 0.0f) ? 1.0f : -1.0f;

  size_t *índices = malloc(núm_vértices * sizeof(size_t));
  if (índices == nullptr) {
    return;
  }

  for (size_t i = 0; i < núm_vértices; ++i) {
    índices[i] = i;
  }

  size_t restantes = núm_vértices;
  size_t tentativas_sem_corte = 0;
  size_t i = 0;

  while (restantes > 3 && tentativas_sem_corte < restantes) {
    const size_t idx_ant = índices[(i + restantes - 1) % restantes];
    const size_t idx_cur = índices[i % restantes];
    const size_t idx_seg = índices[(i + 1) % restantes];

    const Vértice2D v_ant = vértices[idx_ant];
    const Vértice2D v_cur = vértices[idx_cur];
    const Vértice2D v_seg = vértices[idx_seg];

    const float prod_vetorial = (v_cur.posição.x - v_ant.posição.x) * (v_seg.posição.y - v_ant.posição.y) -
                                (v_cur.posição.y - v_ant.posição.y) * (v_seg.posição.x - v_ant.posição.x);

    /* Verificar se o vértice atual forma um ângulo convexo */
    if (prod_vetorial * sinal_orientação > 1e-6f) {
      bool contém_outro_vértice = false;

      for (size_t j = 0; j < restantes; ++j) {
        const size_t idx_teste = índices[j];
        if (idx_teste == idx_ant || idx_teste == idx_cur || idx_teste == idx_seg) {
          continue;
        }

        if (ponto_flut_em_triângulo(vértices[idx_teste].posição, v_ant.posição, v_cur.posição, v_seg.posição)) {
          contém_outro_vértice = true;
          break;
        }
      }

      if (!contém_outro_vértice) {
        desenhar_triângulo_texturizado(tela, textura, v_ant, v_cur, v_seg);

        /* Remover o vértice atual da lista de índices ativos */
        const size_t pos_remover = i % restantes;
        for (size_t k = pos_remover; k + 1 < restantes; ++k) {
          índices[k] = índices[k + 1];
        }

        restantes--;
        tentativas_sem_corte = 0;
        continue;
      }
    }

    i++;
    tentativas_sem_corte++;
  }

  /* Desenhar o último triângulo restante */
  if (restantes == 3) {
    desenhar_triângulo_texturizado(
      tela,
      textura,
      vértices[índices[0]],
      vértices[índices[1]],
      vértices[índices[2]]
    );
  }

  free(índices);
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

static bool ângulo_no_intervalo(float âng, float início, float fim) {
  âng    = normalizar_radianos(âng);
  início = normalizar_radianos(início);
  fim    = normalizar_radianos(fim);
  if (início <= fim) {
    return âng >= início && âng <= fim;
  }
  return âng >= início || âng <= fim;
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
          float âng = atan2f((float)dy, (float)dx);
          if (ângulo_no_intervalo(âng, ângulo_início, ângulo_fim)) {
            desenhar_píxel(tela, (Int2D){ centro.x + dx, centro.y + dy }, cor);
          }
        }
      }
    }
  }
}

void desenhar_gradiente_angular(Tela *tela, Int2D centro, uint32_t raio, Cor cor_início, Cor cor_fim, float ângulo_início) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0 || raio == 0) {
    return;
  }

  constexpr float DOIS_PI = 6.28318530717958647692f;
  constexpr float INV_DOIS_PI = 1.0f / DOIS_PI;

  const int64_t cx = centro.x;
  const int64_t cy = centro.y;
  const int64_t r = (int64_t)raio;
  const uint64_t raio_quadrado = (uint64_t)raio * (uint64_t)raio;

  /* Recortar caixa envolvente aos limites da tela */
  const int64_t x0 = (cx - r > 0) ? (cx - r) : 0;
  const int64_t y0 = (cy - r > 0) ? (cy - r) : 0;
  const int64_t x1 = (cx + r < (int64_t)tela->largura - 1) ? (cx + r) : ((int64_t)tela->largura - 1);
  const int64_t y1 = (cy + r < (int64_t)tela->altura - 1)  ? (cy + r) : ((int64_t)tela->altura - 1);

  if (x0 > x1 || y0 > y1) {
    return;
  }

  for (int64_t y = y0; y <= y1; ++y) {
    const int64_t dy = y - cy;
    const uint64_t dy2 = (uint64_t)(dy * dy);
    Cor *linha = &tela->píxeis[(size_t)y * tela->largura];

    for (int64_t x = x0; x <= x1; ++x) {
      const int64_t dx = x - cx;
      const uint64_t dist2 = (uint64_t)(dx * dx) + dy2;

      if (dist2 <= raio_quadrado) {
        float ângulo = atan2f((float)dy, (float)dx) - ângulo_início;
        ângulo = fmodf(ângulo, DOIS_PI);
        if (ângulo < 0.0f) {
          ângulo += DOIS_PI;
        }

        const float fator = ângulo * INV_DOIS_PI;

        linha[x] = (Cor){
          .vermelho = (uint8_t)lroundf((float)cor_início.vermelho + ((float)cor_fim.vermelho - (float)cor_início.vermelho) * fator),
          .verde    = (uint8_t)lroundf((float)cor_início.verde    + ((float)cor_fim.verde    - (float)cor_início.verde)    * fator),
          .azul     = (uint8_t)lroundf((float)cor_início.azul     + ((float)cor_fim.azul     - (float)cor_início.azul)     * fator),
          .alfa     = (uint8_t)lroundf((float)cor_início.alfa     + ((float)cor_fim.alfa     - (float)cor_início.alfa)     * fator)
        };
      }
    }
  }
}

void desenhar_anel_preenchido(Tela *tela, Int2D centro, uint32_t raio_interno, uint32_t raio_externo, Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return;
  }

  if (raio_externo == 0 || raio_interno > raio_externo) {
    return;
  }

  const int64_t cx = centro.x;
  const int64_t cy = centro.y;
  const int64_t r_ext = (int64_t)raio_externo;

  const uint64_t r_int_quad = (uint64_t)raio_interno * (uint64_t)raio_interno;
  const uint64_t r_ext_quad = (uint64_t)raio_externo * (uint64_t)raio_externo;

  /* Recortar caixa envolvente do raio externo aos limites da tela */
  const int64_t x0 = (cx - r_ext > 0) ? (cx - r_ext) : 0;
  const int64_t y0 = (cy - r_ext > 0) ? (cy - r_ext) : 0;
  const int64_t x1 = (cx + r_ext < (int64_t)tela->largura - 1) ? (cx + r_ext) : ((int64_t)tela->largura - 1);
  const int64_t y1 = (cy + r_ext < (int64_t)tela->altura - 1)  ? (cy + r_ext) : ((int64_t)tela->altura - 1);

  if (x0 > x1 || y0 > y1) {
    return;
  }

  for (int64_t y = y0; y <= y1; ++y) {
    const int64_t dy = y - cy;
    const uint64_t dy2 = (uint64_t)(dy * dy);
    Cor *linha = &tela->píxeis[(size_t)y * tela->largura];

    for (int64_t x = x0; x <= x1; ++x) {
      const int64_t dx = x - cx;
      const uint64_t dist2 = (uint64_t)(dx * dx) + dy2;

      if (dist2 >= r_int_quad && dist2 <= r_ext_quad) {
        linha[x] = cor;
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

void aplicar_sépia(Tela *tela) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return;
  }

  const size_t total_píxeis = (size_t)tela->largura * (size_t)tela->altura;

  for (size_t i = 0; i < total_píxeis; ++i) {
    const Cor orig = tela->píxeis[i];
    const float r = (float)orig.vermelho;
    const float g = (float)orig.verde;
    const float b = (float)orig.azul;

    const float novo_r = (r * 0.393f) + (g * 0.769f) + (b * 0.189f);
    const float novo_g = (r * 0.349f) + (g * 0.686f) + (b * 0.168f);
    const float novo_b = (r * 0.272f) + (g * 0.534f) + (b * 0.131f);

    tela->píxeis[i] = (Cor){
      .vermelho = (novo_r > 255.0f) ? 255u : (uint8_t)lroundf(novo_r),
      .verde    = (novo_g > 255.0f) ? 255u : (uint8_t)lroundf(novo_g),
      .azul     = (novo_b > 255.0f) ? 255u : (uint8_t)lroundf(novo_b),
      .alfa     = orig.alfa
    };
  }
}

void aplicar_limiar(Tela *tela, uint8_t limiar) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return;
  }

  const size_t total_píxeis = (size_t)tela->largura * (size_t)tela->altura;

  for (size_t i = 0; i < total_píxeis; ++i) {
    const Cor orig = tela->píxeis[i];

    /* Luminância perceptual aproximada (0.299 R + 0.587 G + 0.114 B) em inteiros */
    const uint32_t luminância = ((uint32_t)orig.vermelho * 77u +
                                 (uint32_t)orig.verde    * 150u +
                                 (uint32_t)orig.azul     * 29u) >> 8;

    const uint8_t valor = (luminância >= (uint32_t)limiar) ? 255u : 0u;

    tela->píxeis[i] = (Cor){
      .vermelho = valor,
      .verde    = valor,
      .azul     = valor,
      .alfa     = orig.alfa
    };
  }
}

static inline uint8_t quantizar_canal_bayer(uint8_t valor, float deslocamento_bayer, float escala_degrau, float fator_nível) {
  const float norm = (float)valor / 255.0f;
  const float ajustado = norm + (deslocamento_bayer * escala_degrau);
  const float limitado = fminf(fmaxf(ajustado, 0.0f), 1.0f);
  const float quantizado = roundf(limitado * fator_nível) / fator_nível;

  return (uint8_t)lroundf(quantizado * 255.0f);
}

void aplicar_pontilhismo_bayer(Tela *tela, uint8_t níveis_por_canal) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return;
  }

  /* São necessários pelo menos 2 níveis por canal (ex.: 0 e 255) */
  if (níveis_por_canal < 2) {
    níveis_por_canal = 2;
  }

  /* Matriz de Bayer 4x4 (valores de 0 a 15) */
  static constexpr float MATRIZ_BAYER_4X4[4][4] = {
    {  0.0f / 16.0f,  8.0f / 16.0f,  2.0f / 16.0f, 10.0f / 16.0f },
    { 12.0f / 16.0f,  4.0f / 16.0f, 14.0f / 16.0f,  6.0f / 16.0f },
    {  3.0f / 16.0f, 11.0f / 16.0f,  1.0f / 16.0f,  9.0f / 16.0f },
    { 15.0f / 16.0f,  7.0f / 16.0f, 13.0f / 16.0f,  5.0f / 16.0f }
  };

  const float fator_nível = (float)(níveis_por_canal - 1);
  const float escala_degrau = 1.0f / fator_nível;

  for (uint32_t y = 0; y < tela->altura; ++y) {
    Cor *linha = &tela->píxeis[(size_t)y * tela->largura];
    const uint32_t by = y & 3u;

    for (uint32_t x = 0; x < tela->largura; ++x) {
      const uint32_t bx = x & 3u;
      /* Centrar o limiar em torno de zero [-0.5f, +0.4375f] para não escurecer nem aclarar a imagem */
      const float deslocamento = MATRIZ_BAYER_4X4[by][bx] - 0.5f;
      const Cor orig = linha[x];

      linha[x] = (Cor){
        .vermelho = quantizar_canal_bayer(orig.vermelho, deslocamento, escala_degrau, fator_nível),
        .verde    = quantizar_canal_bayer(orig.verde,    deslocamento, escala_degrau, fator_nível),
        .azul     = quantizar_canal_bayer(orig.azul,     deslocamento, escala_degrau, fator_nível),
        .alfa     = orig.alfa
      };
    }
  }
}

void aplicar_deteção_bordas_sobel(Tela *tela) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return;
  }

  const size_t largura = (size_t)tela->largura;
  const size_t altura = (size_t)tela->altura;
  const size_t total_píxeis = largura * altura;

  uint8_t *luminância = malloc(total_píxeis * sizeof(uint8_t));
  if (luminância == nullptr) {
    return;
  }

  /* Pré-calcular luminância perceptual em inteiros (0.299 R + 0.587 G + 0.114 B) */
  for (size_t i = 0; i < total_píxeis; ++i) {
    const Cor c = tela->píxeis[i];
    luminância[i] = (uint8_t)(((uint32_t)c.vermelho * 77u +
                               (uint32_t)c.verde    * 150u +
                               (uint32_t)c.azul     * 29u) >> 8);
  }

  for (size_t y = 0; y < altura; ++y) {
    const size_t y_ant = (y > 0) ? (y - 1) : 0;
    const size_t y_seg = (y + 1 < altura) ? (y + 1) : (altura - 1);

    const uint8_t *linha_sup = &luminância[y_ant * largura];
    const uint8_t *linha_med = &luminância[y * largura];
    const uint8_t *linha_inf = &luminância[y_seg * largura];
    Cor *linha_destino = &tela->píxeis[y * largura];

    for (size_t x = 0; x < largura; ++x) {
      const size_t x_ant = (x > 0) ? (x - 1) : 0;
      const size_t x_seg = (x + 1 < largura) ? (x + 1) : (largura - 1);

      const int32_t p00 = linha_sup[x_ant];
      const int32_t p01 = linha_sup[x];
      const int32_t p02 = linha_sup[x_seg];

      const int32_t p10 = linha_med[x_ant];
      const int32_t p12 = linha_med[x_seg];

      const int32_t p20 = linha_inf[x_ant];
      const int32_t p21 = linha_inf[x];
      const int32_t p22 = linha_inf[x_seg];

      /* Gradiente horizontal Gx: [-1 0 +1; -2 0 +2; -1 0 +1] */
      const int32_t gx = -p00 + p02 - (2 * p10) + (2 * p12) - p20 + p22;

      /* Gradiente vertical Gy: [-1 -2 -1; 0 0 0; +1 +2 +1] */
      const int32_t gy = -p00 - (2 * p01) - p02 + p20 + (2 * p21) + p22;

      const float mag = hypotf((float)gx, (float)gy);
      const uint8_t intensidade = (mag > 255.0f) ? 255u : (uint8_t)lroundf(mag);

      linha_destino[x] = (Cor){
        .vermelho = intensidade,
        .verde    = intensidade,
        .azul     = intensidade,
        .alfa     = linha_destino[x].alfa
      };
    }
  }

  free(luminância);
}

static inline uint8_t limitar_u8_flut(float valor) {
  if (valor <= 0.0f) {
    return 0u;
  }
  if (valor >= 255.0f) {
    return 255u;
  }
  return (uint8_t)lroundf(valor);
}

void aplicar_nitidez(Tela *tela, float intensidade) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return;
  }

  if (intensidade <= 0.0f) {
    return;
  }

  const size_t largura = (size_t)tela->largura;
  const size_t altura = (size_t)tela->altura;
  const size_t total_bytes = largura * altura * sizeof(Cor);

  Cor *cópia = malloc(total_bytes);
  if (cópia == nullptr) {
    return;
  }

  memcpy(cópia, tela->píxeis, total_bytes);

  /* Núcleo 3x3: centro = 1 + 4 * intensidade, vizinhos ortogonais = -intensidade */
  const float peso_centro = 1.0f + 4.0f * intensidade;
  const float peso_vizinho = -intensidade;

  for (size_t y = 0; y < altura; ++y) {
    const size_t y_ant = (y > 0) ? (y - 1) : 0;
    const size_t y_seg = (y + 1 < altura) ? (y + 1) : (altura - 1);

    const Cor *linha_sup = &cópia[y_ant * largura];
    const Cor *linha_med = &cópia[y * largura];
    const Cor *linha_inf = &cópia[y_seg * largura];
    Cor *linha_destino = &tela->píxeis[y * largura];

    for (size_t x = 0; x < largura; ++x) {
      const size_t x_ant = (x > 0) ? (x - 1) : 0;
      const size_t x_seg = (x + 1 < largura) ? (x + 1) : (largura - 1);

      const Cor c_centro = linha_med[x];
      const Cor c_cima   = linha_sup[x];
      const Cor c_baixo  = linha_inf[x];
      const Cor c_esq    = linha_med[x_ant];
      const Cor c_dir    = linha_med[x_seg];

      const float soma_viz_r = (float)c_cima.vermelho + (float)c_baixo.vermelho + (float)c_esq.vermelho + (float)c_dir.vermelho;
      const float soma_viz_g = (float)c_cima.verde    + (float)c_baixo.verde    + (float)c_esq.verde    + (float)c_dir.verde;
      const float soma_viz_b = (float)c_cima.azul     + (float)c_baixo.azul     + (float)c_esq.azul     + (float)c_dir.azul;

      const float novo_r = (float)c_centro.vermelho * peso_centro + soma_viz_r * peso_vizinho;
      const float novo_g = (float)c_centro.verde    * peso_centro + soma_viz_g * peso_vizinho;
      const float novo_b = (float)c_centro.azul     * peso_centro + soma_viz_b * peso_vizinho;

      linha_destino[x] = (Cor){
        .vermelho = limitar_u8_flut(novo_r),
        .verde    = limitar_u8_flut(novo_g),
        .azul     = limitar_u8_flut(novo_b),
        .alfa     = c_centro.alfa
      };
    }
  }

  free(cópia);
}

void aplicar_máscara_alfa(Tela *destino, const Tela *máscara, Int2D posição) {
  if (destino == nullptr || destino->píxeis == nullptr || destino->largura == 0 || destino->altura == 0) {
    return;
  }

  if (máscara == nullptr || máscara->píxeis == nullptr || máscara->largura == 0 || máscara->altura == 0) {
    return;
  }

  /* Calcular limites da interseção no destino em 64 bits para evitar overflows */
  const int64_t dest_x0 = (posição.x > 0) ? (int64_t)posição.x : 0;
  const int64_t dest_y0 = (posição.y > 0) ? (int64_t)posição.y : 0;

  const int64_t masc_dir = (int64_t)posição.x + (int64_t)máscara->largura;
  const int64_t masc_inf = (int64_t)posição.y + (int64_t)máscara->altura;

  const int64_t dest_x1 = (masc_dir < (int64_t)destino->largura) ? masc_dir : (int64_t)destino->largura;
  const int64_t dest_y1 = (masc_inf < (int64_t)destino->altura)  ? masc_inf : (int64_t)destino->altura;

  if (dest_x0 >= dest_x1 || dest_y0 >= dest_y1) {
    return;
  }

  for (int64_t dy = dest_y0; dy < dest_y1; ++dy) {
    const size_t my = (size_t)(dy - (int64_t)posição.y);
    const Cor *linha_máscara = &máscara->píxeis[my * máscara->largura];
    Cor *linha_destino = &destino->píxeis[(size_t)dy * destino->largura];

    for (int64_t dx = dest_x0; dx < dest_x1; ++dx) {
      const size_t mx = (size_t)(dx - (int64_t)posição.x);
      const Cor m = linha_máscara[mx];

      /* Luminância perceptual aproximada (0.299 R + 0.587 G + 0.114 B) em inteiros */
      const uint32_t lum = ((uint32_t)m.vermelho * 77u +
                            (uint32_t)m.verde    * 150u +
                            (uint32_t)m.azul     * 29u) >> 8;

      /* Combinar luminância com o próprio canal alfa da máscara */
      const uint32_t fator_máscara = (lum * (uint32_t)m.alfa + 127u) / 255u;

      /* Modular o alfa atual do destino preservando a transparência prévia */
      const uint32_t novo_alfa = ((uint32_t)linha_destino[dx].alfa * fator_máscara + 127u) / 255u;
      linha_destino[dx].alfa = (uint8_t)novo_alfa;
    }
  }
}

Tela extrair_canal_vermelho(const Tela *origem) {
  if (origem == nullptr || origem->píxeis == nullptr || origem->largura == 0 || origem->altura == 0) {
    return (Tela){ .largura = 0, .altura = 0, .píxeis = nullptr };
  }

  Tela resultado = criar_tela(origem->largura, origem->altura);
  if (resultado.píxeis == nullptr) {
    return resultado;
  }

  const size_t total_píxeis = (size_t)origem->largura * (size_t)origem->altura;

  for (size_t i = 0; i < total_píxeis; ++i) {
    const uint8_t valor = origem->píxeis[i].vermelho;
    resultado.píxeis[i] = (Cor){
      .vermelho = valor,
      .verde    = valor,
      .azul     = valor,
      .alfa     = 255u
    };
  }

  return resultado;
}

Tela extrair_canal_verde(const Tela *origem) {
  if (origem == nullptr || origem->píxeis == nullptr || origem->largura == 0 || origem->altura == 0) {
    return (Tela){ .largura = 0, .altura = 0, .píxeis = nullptr };
  }

  Tela resultado = criar_tela(origem->largura, origem->altura);
  if (resultado.píxeis == nullptr) {
    return resultado;
  }

  const size_t total_píxeis = (size_t)origem->largura * (size_t)origem->altura;

  for (size_t i = 0; i < total_píxeis; ++i) {
    const uint8_t valor = origem->píxeis[i].verde;
    resultado.píxeis[i] = (Cor){
      .vermelho = valor,
      .verde    = valor,
      .azul     = valor,
      .alfa     = 255u
    };
  }

  return resultado;
}

Tela extrair_canal_azul(const Tela *origem) {
  if (origem == nullptr || origem->píxeis == nullptr || origem->largura == 0 || origem->altura == 0) {
    return (Tela){ .largura = 0, .altura = 0, .píxeis = nullptr };
  }

  Tela resultado = criar_tela(origem->largura, origem->altura);
  if (resultado.píxeis == nullptr) {
    return resultado;
  }

  const size_t total_píxeis = (size_t)origem->largura * (size_t)origem->altura;

  for (size_t i = 0; i < total_píxeis; ++i) {
    const uint8_t valor = origem->píxeis[i].azul;
    resultado.píxeis[i] = (Cor){
      .vermelho = valor,
      .verde    = valor,
      .azul     = valor,
      .alfa     = 255u
    };
  }

  return resultado;
}

Tela extrair_canal_alfa(const Tela *origem) {
  if (origem == nullptr || origem->píxeis == nullptr || origem->largura == 0 || origem->altura == 0) {
    return (Tela){ .largura = 0, .altura = 0, .píxeis = nullptr };
  }

  Tela resultado = criar_tela(origem->largura, origem->altura);
  if (resultado.píxeis == nullptr) {
    return resultado;
  }

  const size_t total_píxeis = (size_t)origem->largura * (size_t)origem->altura;

  for (size_t i = 0; i < total_píxeis; ++i) {
    const uint8_t valor = origem->píxeis[i].alfa;
    resultado.píxeis[i] = (Cor){
      .vermelho = valor,
      .verde    = valor,
      .azul     = valor,
      .alfa     = 255u
    };
  }

  return resultado;
}

void substituir_cor(Tela *tela, Cor alvo, Cor nova, uint8_t tolerância) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return;
  }

  const size_t total_píxeis = (size_t)tela->largura * (size_t)tela->altura;

  for (size_t i = 0; i < total_píxeis; ++i) {
    const Cor atual = tela->píxeis[i];

    const int32_t dr = abs((int32_t)atual.vermelho - (int32_t)alvo.vermelho);
    const int32_t dg = abs((int32_t)atual.verde    - (int32_t)alvo.verde);
    const int32_t db = abs((int32_t)atual.azul     - (int32_t)alvo.azul);
    const int32_t da = abs((int32_t)atual.alfa     - (int32_t)alvo.alfa);

    if (dr <= (int32_t)tolerância &&
        dg <= (int32_t)tolerância &&
        db <= (int32_t)tolerância &&
        da <= (int32_t)tolerância) {
      tela->píxeis[i] = nova;
    }
  }
}



bool atualizar_textura_sdl(SDL_Texture *textura, const Tela *tela) {
  if (textura == nullptr || tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return false;
  }

  const uint64_t bytes_por_linha = (uint64_t)tela->largura * sizeof(Cor);
  if (bytes_por_linha > (uint64_t)INT_MAX) {
    return false;
  }

  return SDL_UpdateTexture(textura, nullptr, tela->píxeis, (int)bytes_por_linha);
}

SDL_Texture *criar_textura_sdl_de_tela(SDL_Renderer *renderizador, const Tela *tela) {
  if (renderizador == nullptr || tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0) {
    return nullptr;
  }

  if (tela->largura > (uint32_t)INT_MAX || tela->altura > (uint32_t)INT_MAX) {
    return nullptr;
  }

  SDL_Texture *textura = SDL_CreateTexture(
    renderizador,
    SDL_PIXELFORMAT_RGBA32,
    SDL_TEXTUREACCESS_STREAMING,
    (int)tela->largura,
    (int)tela->altura
  );

  if (textura == nullptr) {
    return nullptr;
  }

  if (!atualizar_textura_sdl(textura, tela)) {
    SDL_DestroyTexture(textura);
    return nullptr;
  }

  return textura;
}

bool capturar_renderizador_sdl(SDL_Renderer *renderizador, Tela *destino) {
  if (renderizador == nullptr || destino == nullptr) {
    return false;
  }

  SDL_Surface *superfície = SDL_RenderReadPixels(renderizador, nullptr);
  if (superfície == nullptr) {
    return false;
  }

  if (superfície->format != SDL_PIXELFORMAT_RGBA32) {
    SDL_Surface *convertida = SDL_ConvertSurface(superfície, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(superfície);
    if (convertida == nullptr) {
      return false;
    }
    superfície = convertida;
  }

  if (superfície->w <= 0 || superfície->h <= 0 || superfície->pixels == nullptr) {
    SDL_DestroySurface(superfície);
    return false;
  }

  const uint32_t largura = (uint32_t)superfície->w;
  const uint32_t altura = (uint32_t)superfície->h;

  /* Reutilizar a memória se o destino já tiver as dimensões exatas */
  if (destino->píxeis == nullptr || destino->largura != largura || destino->altura != altura) {
    destruir_tela(destino);
    *destino = criar_tela(largura, altura);
    if (destino->píxeis == nullptr) {
      SDL_DestroySurface(superfície);
      return false;
    }
  }

  const size_t bytes_por_linha = (size_t)largura * sizeof(Cor);
  const uint8_t *origem_bytes = (const uint8_t *)superfície->pixels;

  for (uint32_t y = 0; y < altura; ++y) {
    memcpy(
      &destino->píxeis[(size_t)y * largura],
      origem_bytes + ((size_t)y * (size_t)superfície->pitch),
      bytes_por_linha
    );
  }

  SDL_DestroySurface(superfície);
  return true;
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

bool exportar_para_ficheiro_ppm(const Tela *tela, const char *caminho) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0 || caminho == nullptr) {
    return false;
  }

  FILE *ficheiro = fopen(caminho, "wb");
  if (ficheiro == nullptr) {
    return false;
  }

  if (fprintf(ficheiro, "P6\n%u %u\n255\n", tela->largura, tela->altura) < 0) {
    fclose(ficheiro);
    return false;
  }

  const size_t bytes_por_linha = (size_t)tela->largura * 3;
  uint8_t *linha_rgb = malloc(bytes_por_linha);
  if (linha_rgb == nullptr) {
    fclose(ficheiro);
    return false;
  }

  bool sucesso = true;
  for (uint32_t y = 0; y < tela->altura; ++y) {
    const Cor *linha_píxeis = &tela->píxeis[(size_t)y * tela->largura];

    for (uint32_t x = 0; x < tela->largura; ++x) {
      const size_t idx = (size_t)x * 3;
      linha_rgb[idx + 0] = linha_píxeis[x].vermelho;
      linha_rgb[idx + 1] = linha_píxeis[x].verde;
      linha_rgb[idx + 2] = linha_píxeis[x].azul;
    }

    if (fwrite(linha_rgb, 1, bytes_por_linha, ficheiro) != bytes_por_linha) {
      sucesso = false;
      break;
    }
  }

  free(linha_rgb);
  if (fclose(ficheiro) != 0) {
    sucesso = false;
  }

  return sucesso;
}

static void escrever_u16_le(uint8_t *destino, uint16_t valor) {
  destino[0] = (uint8_t)(valor & 0xFFu);
  destino[1] = (uint8_t)((valor >> 8) & 0xFFu);
}

static void escrever_u32_le(uint8_t *destino, uint32_t valor) {
  destino[0] = (uint8_t)(valor & 0xFFu);
  destino[1] = (uint8_t)((valor >> 8) & 0xFFu);
  destino[2] = (uint8_t)((valor >> 16) & 0xFFu);
  destino[3] = (uint8_t)((valor >> 24) & 0xFFu);
}

bool exportar_para_ficheiro_bmp(const Tela *tela, const char *caminho) {
  if (tela == nullptr || tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0 || caminho == nullptr) {
    return false;
  }

  constexpr uint32_t TAM_CABEÇALHO_FICHEIRO = 14;
  constexpr uint32_t TAM_CABEÇALHO_DIB = 108; /* BITMAPV4HEADER para suporte RGBA */
  constexpr uint32_t DESLOCAMENTO_DADOS = TAM_CABEÇALHO_FICHEIRO + TAM_CABEÇALHO_DIB;

  const uint64_t tamanho_imagem_64 = (uint64_t)tela->largura * (uint64_t)tela->altura * 4u;
  if (tamanho_imagem_64 > (uint64_t)UINT32_MAX - DESLOCAMENTO_DADOS) {
    return false;
  }

  const uint32_t tamanho_imagem = (uint32_t)tamanho_imagem_64;
  const uint32_t tamanho_ficheiro = DESLOCAMENTO_DADOS + tamanho_imagem;

  FILE *ficheiro = fopen(caminho, "wb");
  if (ficheiro == nullptr) {
    return false;
  }

  uint8_t cabeçalho[DESLOCAMENTO_DADOS] = { 0 };

  /* Cabeçalho do ficheiro BMP (14 bytes) */
  cabeçalho[0] = 'B';
  cabeçalho[1] = 'M';
  escrever_u32_le(&cabeçalho[2], tamanho_ficheiro);
  escrever_u32_le(&cabeçalho[10], DESLOCAMENTO_DADOS);

  /* Cabeçalho DIB BITMAPV4HEADER (108 bytes) */
  escrever_u32_le(&cabeçalho[14], TAM_CABEÇALHO_DIB);
  escrever_u32_le(&cabeçalho[18], tela->largura);
  /* Altura negativa indica orientação top-down (de cima para baixo) */
  escrever_u32_le(&cabeçalho[22], (uint32_t)(-(int32_t)tela->altura));
  escrever_u16_le(&cabeçalho[26], 1);          /* Planos de cor */
  escrever_u16_le(&cabeçalho[28], 32);         /* 32 bits por píxel */
  escrever_u32_le(&cabeçalho[30], 3);          /* BI_BITFIELDS */
  escrever_u32_le(&cabeçalho[34], tamanho_imagem);
  escrever_u32_le(&cabeçalho[38], 2835);       /* Resolução horizontal (~72 DPI) */
  escrever_u32_le(&cabeçalho[42], 2835);       /* Resolução vertical (~72 DPI) */

  /* Máscaras de canais BGRA (little-endian) */
  escrever_u32_le(&cabeçalho[54], 0x00FF0000u); /* Máscara Vermelho */
  escrever_u32_le(&cabeçalho[58], 0x0000FF00u); /* Máscara Verde */
  escrever_u32_le(&cabeçalho[62], 0x000000FFu); /* Máscara Azul */
  escrever_u32_le(&cabeçalho[66], 0xFF000000u); /* Máscara Alfa */

  /* Espaço de cor LCS_sRGB ("sRGB") */
  cabeçalho[70] = 'B';
  cabeçalho[71] = 'G';
  cabeçalho[72] = 'R';
  cabeçalho[73] = 's';

  if (fwrite(cabeçalho, 1, DESLOCAMENTO_DADOS, ficheiro) != DESLOCAMENTO_DADOS) {
    fclose(ficheiro);
    return false;
  }

  const size_t bytes_por_linha = (size_t)tela->largura * 4;
  uint8_t *linha_bgra = malloc(bytes_por_linha);
  if (linha_bgra == nullptr) {
    fclose(ficheiro);
    return false;
  }

  bool sucesso = true;
  for (uint32_t y = 0; y < tela->altura; ++y) {
    const Cor *linha_píxeis = &tela->píxeis[(size_t)y * tela->largura];

    for (uint32_t x = 0; x < tela->largura; ++x) {
      const size_t idx = (size_t)x * 4;
      linha_bgra[idx + 0] = linha_píxeis[x].azul;
      linha_bgra[idx + 1] = linha_píxeis[x].verde;
      linha_bgra[idx + 2] = linha_píxeis[x].vermelho;
      linha_bgra[idx + 3] = linha_píxeis[x].alfa;
    }

    if (fwrite(linha_bgra, 1, bytes_por_linha, ficheiro) != bytes_por_linha) {
      sucesso = false;
      break;
    }
  }

  free(linha_bgra);
  if (fclose(ficheiro) != 0) {
    sucesso = false;
  }

  return sucesso;
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



BufferProfundidade criar_buffer_profundidade(uint32_t largura, uint32_t altura) {
  if (largura == 0 || altura == 0) {
    return (BufferProfundidade){ .largura = 0, .altura = 0, .valores = nullptr };
  }

  const size_t total_píxeis = (size_t)largura * (size_t)altura;
  if (total_píxeis > SIZE_MAX / sizeof(float)) {
    return (BufferProfundidade){ .largura = 0, .altura = 0, .valores = nullptr };
  }

  float *valores = malloc(total_píxeis * sizeof(float));
  if (valores == nullptr) {
    return (BufferProfundidade){ .largura = 0, .altura = 0, .valores = nullptr };
  }

  return (BufferProfundidade){
    .largura = largura,
    .altura = altura,
    .valores = valores
  };
}

void destruir_buffer_profundidade(BufferProfundidade *buffer) {
  if (buffer == nullptr) {
    return;
  }

  free(buffer->valores);
  buffer->valores = nullptr;
  buffer->largura = 0;
  buffer->altura = 0;
}

void limpar_buffer_profundidade(BufferProfundidade *buffer, float valor_inicial) {
  if (buffer == nullptr || buffer->valores == nullptr) {
    return;
  }

  const size_t total_píxeis = (size_t)buffer->largura * (size_t)buffer->altura;
  for (size_t i = 0; i < total_píxeis; ++i) {
    buffer->valores[i] = valor_inicial;
  }
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
