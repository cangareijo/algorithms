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
bool triângulo_voltado_para_câmara(Flut3D v1, Flut3D v2, Flut3D v3);

typedef struct {
  Flut3D posição;
  Flut3D normal;
  Flut2D uv;
  Cor cor;
} Vértice3D;

size_t recortar_triângulo_plano_próximo(const Vértice3D entrada[3], Vértice3D saída[4], float plano_próximo);

typedef struct {
  Flut3D mínimo;
  Flut3D máximo;
} CaixaDelimitadora3D;

typedef struct {
  float x;
  float y;
  float z;
  float w;
} Flut4D;

Flut3D projetar_para_ecrã(Flut4D ponto_recorte, uint32_t largura_tela, uint32_t altura_tela);

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

void criar_matriz3x3_identidade(Matriz3x3 *destino);
void criar_matriz3x3_translação(Matriz3x3 *destino, float tx, float ty);
void criar_matriz3x3_rotação(Matriz3x3 *destino, float radianos);
void criar_matriz3x3_escala(Matriz3x3 *destino, float sx, float sy);
void multiplicar_matrizes3x3(Matriz3x3 *destino, const Matriz3x3 *a, const Matriz3x3 *b);
bool inverter_matriz3x3(Matriz3x3 *destino, const Matriz3x3 *m);
Int2D transformar_int2d_matriz3x3(Int2D ponto, const Matriz3x3 *m);
Flut2D transformar_flut2d_matriz3x3(Flut2D ponto, const Matriz3x3 *m);

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
void transpor_matriz4x4(Matriz4x4 *destino, const Matriz4x4 *m);
Flut3D transformar_flut3d_matriz4x4(Flut3D ponto, const Matriz4x4 *m);
Flut4D transformar_flut4d_matriz4x4(Flut4D ponto, const Matriz4x4 *m);
void quaternião_para_matriz4x4(Matriz4x4 *destino, Quaternião q);
bool caixa_dentro_frusto(CaixaDelimitadora3D caixa, const Matriz4x4 *visão_projeção);

typedef enum {
  MISTURA_NORMAL,
  MISTURA_ADITIVA,
  MISTURA_MULTIPLICAR,
  MISTURA_ECRÃ
} Mistura;

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
void desenhar_linha_suavizada(Tela *tela, Flut2D início, Flut2D fim, Cor cor);
void desenhar_polilinha(Tela *tela, const Int2D vértices[], size_t núm_vértices, Cor cor, bool fechada);

void desenhar_curva_bezier_quadrática(Tela *tela, Int2D p0, Int2D p1, Int2D p2, Cor cor);
void desenhar_curva_bezier_cúbica(Tela *tela, Int2D p0, Int2D p1, Int2D p2, Int2D p3, Cor cor);
void desenhar_curva_bezier_adaptativa(Tela *tela, Flut2D p0, Flut2D p1, Flut2D p2, Flut2D p3, Cor cor, float tolerância_píxeis);
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
void aplicar_matriz_convolução(Tela * restrict destino, const Tela * restrict origem, Matriz3x3 núcleo, float divisor, float viés);
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
  Vértice3D *vértices;
  size_t núm_vértices;
  uint32_t *índices;
  size_t núm_índices;
} Malha3D;

void destruir_malha3d(Malha3D *malha);
Malha3D criar_malha_cubo(float lado);
Malha3D criar_malha_esfera(float raio, uint32_t fatias, uint32_t pilhas);
Malha3D criar_malha_plano(float largura, float profundidade, uint32_t subdivisões);
bool carregar_malha_obj(Malha3D *destino, const char *caminho);
void calcular_normais_malha(Malha3D *malha);
CaixaDelimitadora3D calcular_caixa_delimitadora_malha(const Malha3D *malha);

typedef struct {
  uint32_t largura;
  uint32_t altura;
  float *valores;
} BufferProfundidade;

BufferProfundidade criar_buffer_profundidade(uint32_t largura, uint32_t altura);
void destruir_buffer_profundidade(BufferProfundidade *buffer);
void limpar_buffer_profundidade(BufferProfundidade *buffer, float valor_inicial);
void desenhar_linha_3d(Tela *tela, BufferProfundidade *zbuffer, Flut3D início, Flut3D fim, const Matriz4x4 *mvp, Cor cor);

typedef struct {
  Cor cor_ambiente;
  Cor cor_difusa;
  Cor cor_especular;
  float brilho_especular;
  const Tela *textura_difusa;
} Material3D;

typedef enum {
  SOMBREAMENTO_ARAME,
  SOMBREAMENTO_PLANO,
  SOMBREAMENTO_GOURAUD,
  SOMBREAMENTO_PHONG
} ModoSombreamento;

typedef struct {
  Flut3D posição;
  Flut3D direção;
  Flut3D cima;
  float fov_radianos;
  float plano_próximo;
  float plano_distante;
} Câmara3D;

typedef enum {
  LUZ_DIRECIONAL,
  LUZ_PONTUAL,
  LUZ_HOLOFOTE
} TipoLuz;

typedef struct {
  TipoLuz tipo;
  Flut3D posição;
  Flut3D direção;
  Cor cor;
  float intensidade;
  float alcance;
  float ângulo_cone_radianos;
} Luz3D;

void desenhar_triângulo_3d_zbuffer(Tela *tela, BufferProfundidade *zbuffer, Vértice3D v1, Vértice3D v2, Vértice3D v3, const Material3D *material, ModoSombreamento modo);
void desenhar_malha_3d(Tela *tela, BufferProfundidade *zbuffer, const Malha3D *malha, const Matriz4x4 *modelo, const Câmara3D *câmara, const Material3D *material, const Luz3D luzes[], size_t núm_luzes, ModoSombreamento modo);
void aplicar_nevoeiro_profundidade(Tela *tela, const BufferProfundidade *zbuffer, Cor cor_nevoeiro, float início, float fim);



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

bool triângulo_voltado_para_câmara(Flut3D v1, Flut3D v2, Flut3D v3) {
  /* Calcula a área com sinal (produto vetorial 2D) no espaço do ecrã.
     Com Y a apontar para baixo no ecrã, triângulos definidos em sentido
     anti-horário (CCW) no espaço 3D ficam com área negativa quando estão
     voltados para a câmara. */
  const float área_dupla =
    ((v2.x - v1.x) * (v3.y - v1.y)) -
    ((v2.y - v1.y) * (v3.x - v1.x));

  return área_dupla < 0.0f;
}



static Vértice3D interpolar_vértice3d(Vértice3D a, Vértice3D b, float t) {
  return (Vértice3D){
    .posição = interpolar_flut3d(a.posição, b.posição, t),
    .normal = normalizar_flut3d(interpolar_flut3d(a.normal, b.normal, t)),
    .uv = {
      .x = a.uv.x + ((b.uv.x - a.uv.x) * t),
      .y = a.uv.y + ((b.uv.y - a.uv.y) * t)
    },
    .cor = {
      .vermelho = (uint8_t)((float)a.cor.vermelho + (((float)b.cor.vermelho - (float)a.cor.vermelho) * t)),
      .verde = (uint8_t)((float)a.cor.verde + (((float)b.cor.verde - (float)a.cor.verde) * t)),
      .azul = (uint8_t)((float)a.cor.azul + (((float)b.cor.azul - (float)a.cor.azul) * t)),
      .alfa = (uint8_t)((float)a.cor.alfa + (((float)b.cor.alfa - (float)a.cor.alfa) * t))
    }
  };
}

size_t recortar_triângulo_plano_próximo(const Vértice3D entrada[3], Vértice3D saída[4], float plano_próximo) {
  assert(entrada != nullptr && saída != nullptr);
  assert(plano_próximo > 0.0f);

  /* Em espaço de visão mão direita (câmara olha para -Z), o plano próximo fica em z = -plano_próximo */
  const float limite_z = -plano_próximo;
  size_t núm_saída = 0;

  for (size_t i = 0; i < 3; ++i) {
    const Vértice3D atual = entrada[i];
    const Vértice3D seguinte = entrada[(i + 1) % 3];

    /* Distância com sinal ao plano próximo (>= 0.0f significa à frente do plano / visível) */
    const float dist_atual = limite_z - atual.posição.z;
    const float dist_seguinte = limite_z - seguinte.posição.z;

    const bool atual_dentro = (dist_atual >= 0.0f);
    const bool seguinte_dentro = (dist_seguinte >= 0.0f);

    if (atual_dentro && seguinte_dentro) {
      /* Ambos os vértices visíveis: mantém o vértice seguinte */
      saída[núm_saída++] = seguinte;
    } else if (atual_dentro && !seguinte_dentro) {
      /* A sair do volume visível: adiciona apenas o ponto de interseção */
      const float t = dist_atual / (dist_atual - dist_seguinte);
      saída[núm_saída++] = interpolar_vértice3d(atual, seguinte, t);
    } else if (!atual_dentro && seguinte_dentro) {
      /* A entrar no volume visível: adiciona a interseção e o vértice seguinte */
      const float t = dist_atual / (dist_atual - dist_seguinte);
      saída[núm_saída++] = interpolar_vértice3d(atual, seguinte, t);
      saída[núm_saída++] = seguinte;
    }
  }

  return núm_saída;
}



Flut3D projetar_para_ecrã(Flut4D ponto_recorte, uint32_t largura_tela, uint32_t altura_tela) {
  if (ponto_recorte.w == 0.0f) {
    return (Flut3D){ .x = 0.0f, .y = 0.0f, .z = 0.0f };
  }

  /* Divisão perspética para obter Coordenadas Normalizadas do Dispositivo (NDC) */
  const float inv_w = 1.0f / ponto_recorte.w;
  const float ndc_x = ponto_recorte.x * inv_w;
  const float ndc_y = ponto_recorte.y * inv_w;
  const float ndc_z = ponto_recorte.z * inv_w;

  /* Mapeia X de [-1, 1] para [0, largura_tela] e inverte Y de [-1, 1] para [altura_tela, 0] */
  return (Flut3D){
    .x = (ndc_x + 1.0f) * 0.5f * (float)largura_tela,
    .y = (1.0f - ndc_y) * 0.5f * (float)altura_tela,
    .z = ndc_z
  };
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

void criar_matriz3x3_identidade(Matriz3x3 *destino) {
  if (!destino) return;
  *destino = (Matriz3x3){
    .m = {
      { 1.0f, 0.0f, 0.0f },
      { 0.0f, 1.0f, 0.0f },
      { 0.0f, 0.0f, 1.0f }
    }
  };
}

void criar_matriz3x3_translação(Matriz3x3 *destino, float tx, float ty) {
  if (!destino) return;
  criar_matriz3x3_identidade(destino);
  destino->m[0][2] = tx;
  destino->m[1][2] = ty;
}

void criar_matriz3x3_rotação(Matriz3x3 *destino, float radianos) {
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

void criar_matriz3x3_escala(Matriz3x3 *destino, float sx, float sy) {
  if (!destino) return;
  *destino = (Matriz3x3){
    .m = {
      { sx,   0.0f, 0.0f },
      { 0.0f, sy,   0.0f },
      { 0.0f, 0.0f, 1.0f }
    }
  };
}

void multiplicar_matrizes3x3(Matriz3x3 *destino, const Matriz3x3 *a, const Matriz3x3 *b) {
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

bool inverter_matriz3x3(Matriz3x3 *destino, const Matriz3x3 *m) {
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

Flut2D transformar_flut2d_matriz3x3(Flut2D ponto, const Matriz3x3 *m) {
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

bool caixa_dentro_frusto(CaixaDelimitadora3D caixa, const Matriz4x4 *visão_projeção) {
  assert(visão_projeção != nullptr);

  /* Os 8 cantos da caixa delimitadora (AABB) */
  const Flut4D cantos[8] = {
    { caixa.mínimo.x, caixa.mínimo.y, caixa.mínimo.z, 1.0f },
    { caixa.máximo.x, caixa.mínimo.y, caixa.mínimo.z, 1.0f },
    { caixa.mínimo.x, caixa.máximo.y, caixa.mínimo.z, 1.0f },
    { caixa.máximo.x, caixa.máximo.y, caixa.mínimo.z, 1.0f },
    { caixa.mínimo.x, caixa.mínimo.y, caixa.máximo.z, 1.0f },
    { caixa.máximo.x, caixa.mínimo.y, caixa.máximo.z, 1.0f },
    { caixa.mínimo.x, caixa.máximo.y, caixa.máximo.z, 1.0f },
    { caixa.máximo.x, caixa.máximo.y, caixa.máximo.z, 1.0f }
  };

  /* Contadores de cantos que ficam fora de cada um dos 6 planos do frustum */
  size_t fora_esquerda = 0;
  size_t fora_direita = 0;
  size_t fora_baixo = 0;
  size_t fora_cima = 0;
  size_t fora_próximo = 0;
  size_t fora_distante = 0;

  for (size_t i = 0; i < 8; ++i) {
    const Flut4D p = transformar_flut4d_matriz4x4(cantos[i], visão_projeção);

    if (p.x < -p.w) ++fora_esquerda;
    if (p.x >  p.w) ++fora_direita;
    if (p.y < -p.w) ++fora_baixo;
    if (p.y >  p.w) ++fora_cima;
    if (p.z < 0.0f) ++fora_próximo; /* Limite próximo: z >= 0 (estilo DirectX/Vulkan) */
    if (p.z >  p.w) ++fora_distante;
  }

  /* Se todos os 8 cantos estiverem fora do mesmo plano, a caixa é invisível */
  if (fora_esquerda == 8 || fora_direita == 8 ||
      fora_baixo == 8    || fora_cima == 8    ||
      fora_próximo == 8  || fora_distante == 8) {
    return false;
  }

  return true;
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



Int2D transformar_int2d_matriz3x3(Int2D ponto, const Matriz3x3 *m) {
  Flut2D res = transformar_flut2d_matriz3x3((Flut2D){ (float)ponto.x, (float)ponto.y }, m);
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

static inline void desenhar_píxel_wu(Tela *tela, int32_t x, int32_t y, Cor cor, float intensidade) {
  if (intensidade <= 0.0f) {
    return;
  }
  if (intensidade > 1.0f) {
    intensidade = 1.0f;
  }
  Cor cor_final = cor;
  cor_final.alfa = (uint8_t)lroundf((float)cor.alfa * intensidade);
  if (cor_final.alfa > 0) {
    desenhar_píxel_alfa(tela, (Int2D){ .x = x, .y = y }, cor_final);
  }
}

void desenhar_linha_suavizada(Tela *tela, Flut2D início, Flut2D fim, Cor cor) {
  assert(tela != nullptr);

  if (tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0 || cor.alfa == 0) {
    return;
  }

  float x0 = início.x;
  float y0 = início.y;
  float x1 = fim.x;
  float y1 = fim.y;

  const bool inclinada = fabsf(y1 - y0) > fabsf(x1 - x0);
  if (inclinada) {
    float temp_x0 = x0;
    x0 = y0;
    y0 = temp_x0;

    float temp_x1 = x1;
    x1 = y1;
    y1 = temp_x1;
  }

  if (x0 > x1) {
    float temp_x = x0;
    x0 = x1;
    x1 = temp_x;

    float temp_y = y0;
    y0 = y1;
    y1 = temp_y;
  }

  const float dx = x1 - x0;
  const float dy = y1 - y0;
  const float gradiente = (dx < 1e-6f) ? 1.0f : (dy / dx);

  /* Primeiro extremo */
  float x_fim_1 = roundf(x0);
  float y_fim_1 = y0 + gradiente * (x_fim_1 - x0);
  float x_lacuna_1 = 1.0f - ((x0 + 0.5f) - floorf(x0 + 0.5f));
  const int32_t x_píxel_1 = (int32_t)x_fim_1;
  const int32_t y_píxel_1 = (int32_t)floorf(y_fim_1);
  const float frac_y1 = y_fim_1 - floorf(y_fim_1);

  if (inclinada) {
    desenhar_píxel_wu(tela, y_píxel_1, x_píxel_1, cor, (1.0f - frac_y1) * x_lacuna_1);
    desenhar_píxel_wu(tela, y_píxel_1 + 1, x_píxel_1, cor, frac_y1 * x_lacuna_1);
  } else {
    desenhar_píxel_wu(tela, x_píxel_1, y_píxel_1, cor, (1.0f - frac_y1) * x_lacuna_1);
    desenhar_píxel_wu(tela, x_píxel_1, y_píxel_1 + 1, cor, frac_y1 * x_lacuna_1);
  }

  float intery = y_fim_1 + gradiente;

  /* Segundo extremo */
  float x_fim_2 = roundf(x1);
  float y_fim_2 = y1 + gradiente * (x_fim_2 - x1);
  float x_lacuna_2 = (x1 + 0.5f) - floorf(x1 + 0.5f);
  const int32_t x_píxel_2 = (int32_t)x_fim_2;
  const int32_t y_píxel_2 = (int32_t)floorf(y_fim_2);
  const float frac_y2 = y_fim_2 - floorf(y_fim_2);

  if (inclinada) {
    desenhar_píxel_wu(tela, y_píxel_2, x_píxel_2, cor, (1.0f - frac_y2) * x_lacuna_2);
    desenhar_píxel_wu(tela, y_píxel_2 + 1, x_píxel_2, cor, frac_y2 * x_lacuna_2);
  } else {
    desenhar_píxel_wu(tela, x_píxel_2, y_píxel_2, cor, (1.0f - frac_y2) * x_lacuna_2);
    desenhar_píxel_wu(tela, x_píxel_2, y_píxel_2 + 1, cor, frac_y2 * x_lacuna_2);
  }

  /* Ciclo principal entre os dois extremos */
  if (inclinada) {
    for (int32_t x = x_píxel_1 + 1; x < x_píxel_2; ++x) {
      const int32_t y_int = (int32_t)floorf(intery);
      const float frac = intery - (float)y_int;
      desenhar_píxel_wu(tela, y_int, x, cor, 1.0f - frac);
      desenhar_píxel_wu(tela, y_int + 1, x, cor, frac);
      intery += gradiente;
    }
  } else {
    for (int32_t x = x_píxel_1 + 1; x < x_píxel_2; ++x) {
      const int32_t y_int = (int32_t)floorf(intery);
      const float frac = intery - (float)y_int;
      desenhar_píxel_wu(tela, x, y_int, cor, 1.0f - frac);
      desenhar_píxel_wu(tela, x, y_int + 1, cor, frac);
      intery += gradiente;
    }
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

static inline bool bezier_cúbica_plana(Flut2D p0, Flut2D p1, Flut2D p2, Flut2D p3, float tolerância_quadrada) {
  const float ux = 3.0f * p1.x - 2.0f * p0.x - p3.x;
  const float uy = 3.0f * p1.y - 2.0f * p0.y - p3.y;
  const float vx = 3.0f * p2.x - 2.0f * p3.x - p0.x;
  const float vy = 3.0f * p2.y - 2.0f * p3.y - p0.y;

  const float max_x = (ux * ux > vx * vx) ? (ux * ux) : (vx * vx);
  const float max_y = (uy * uy > vy * vy) ? (uy * uy) : (vy * vy);

  return (max_x + max_y) <= 16.0f * tolerância_quadrada;
}

void desenhar_curva_bezier_adaptativa(Tela *tela, Flut2D p0, Flut2D p1, Flut2D p2, Flut2D p3, Cor cor, float tolerância_píxeis) {
  assert(tela != nullptr);

  if (tela->píxeis == nullptr || tela->largura == 0 || tela->altura == 0 || cor.alfa == 0) {
    return;
  }

  const float tol = (tolerância_píxeis > 0.05f) ? tolerância_píxeis : 0.05f;
  const float tol_quadrada = tol * tol;

  typedef struct {
    Flut2D p0;
    Flut2D p1;
    Flut2D p2;
    Flut2D p3;
    uint8_t profundidade;
  } SegmentoBezier;

  constexpr size_t CAPACIDADE_PILHA = 64;
  constexpr uint8_t PROFUNDIDADE_MÁXIMA = 16;

  SegmentoBezier pilha[CAPACIDADE_PILHA];
  size_t topo = 0;

  pilha[topo++] = (SegmentoBezier){
    .p0 = p0,
    .p1 = p1,
    .p2 = p2,
    .p3 = p3,
    .profundidade = 0
  };

  while (topo > 0) {
    const SegmentoBezier seg = pilha[--topo];

    if (seg.profundidade >= PROFUNDIDADE_MÁXIMA ||
        topo + 2 > CAPACIDADE_PILHA ||
        bezier_cúbica_plana(seg.p0, seg.p1, seg.p2, seg.p3, tol_quadrada)) {
      desenhar_linha_suavizada(tela, seg.p0, seg.p3, cor);
      continue;
    }

    /* Subdivisão de De Casteljau no parâmetro t = 0.5 */
    const Flut2D p01   = interpolar_flut2d(seg.p0, seg.p1, 0.5f);
    const Flut2D p12   = interpolar_flut2d(seg.p1, seg.p2, 0.5f);
    const Flut2D p23   = interpolar_flut2d(seg.p2, seg.p3, 0.5f);
    const Flut2D p012  = interpolar_flut2d(p01, p12, 0.5f);
    const Flut2D p123  = interpolar_flut2d(p12, p23, 0.5f);
    const Flut2D meio  = interpolar_flut2d(p012, p123, 0.5f);

    const uint8_t próxima_prof = seg.profundidade + 1;

    /* Empilha primeiro a metade direita para processar a esquerda primeiro */
    pilha[topo++] = (SegmentoBezier){
      .p0 = meio,
      .p1 = p123,
      .p2 = p23,
      .p3 = seg.p3,
      .profundidade = próxima_prof
    };

    pilha[topo++] = (SegmentoBezier){
      .p0 = seg.p0,
      .p1 = p01,
      .p2 = p012,
      .p3 = meio,
      .profundidade = próxima_prof
    };
  }
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
  criar_matriz3x3_translação(&t_origem, -(float)pivô.x, -(float)pivô.y);
  criar_matriz3x3_rotação(&rot, radianos);
  criar_matriz3x3_translação(&t_destino, (float)posição.x, (float)posição.y);

  multiplicar_matrizes3x3(&temp, &rot, &t_origem);
  multiplicar_matrizes3x3(&m, &t_destino, &temp);
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
  if (!inverter_matriz3x3(&inv, &m)) return;

  Flut2D cantos[4] = {
    transformar_flut2d_matriz3x3((Flut2D){ 0.0f, 0.0f }, &m),
    transformar_flut2d_matriz3x3((Flut2D){ (float)origem->largura, 0.0f }, &m),
    transformar_flut2d_matriz3x3((Flut2D){ 0.0f, (float)origem->altura }, &m),
    transformar_flut2d_matriz3x3((Flut2D){ (float)origem->largura, (float)origem->altura }, &m)
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
      Flut2D src = transformar_flut2d_matriz3x3((Flut2D){ (float)dx + 0.5f, (float)dy + 0.5f }, &inv);
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

static inline uint8_t limitar_uint8(float valor) {
  if (valor <= 0.0f) {
    return 0;
  }
  if (valor >= 255.0f) {
    return 255;
  }
  return (uint8_t)lroundf(valor);
}

void aplicar_matriz_convolução(Tela * restrict destino, const Tela * restrict origem, Matriz3x3 núcleo, float divisor, float viés) {
  assert(destino != nullptr);
  assert(origem != nullptr);
  assert(destino->píxeis != origem->píxeis);
  assert(destino->largura == origem->largura && destino->altura == origem->altura);

  if (origem->píxeis == nullptr || destino->píxeis == nullptr || origem->largura == 0 || origem->altura == 0) {
    return;
  }

  const float fator_div = (fabsf(divisor) < 1e-6f) ? 1.0f : (1.0f / divisor);
  const int32_t larg = (int32_t)origem->largura;
  const int32_t alt = (int32_t)origem->altura;
  const size_t passo_orig = (origem->passo > 0) ? origem->passo : origem->largura;
  const size_t passo_dest = (destino->passo > 0) ? destino->passo : destino->largura;

  for (int32_t y = 0; y < alt; ++y) {
    Cor * restrict linha_dest = destino->píxeis + (size_t)y * passo_dest;
    const Cor * restrict linha_orig_atual = origem->píxeis + (size_t)y * passo_orig;

    for (int32_t x = 0; x < larg; ++x) {
      float soma_r = 0.0f;
      float soma_g = 0.0f;
      float soma_b = 0.0f;

      for (int32_t ky = -1; ky <= 1; ++ky) {
        int32_t py = y + ky;
        if (py < 0) {
          py = 0;
        } else if (py >= alt) {
          py = alt - 1;
        }

        const Cor * restrict linha_amostra = origem->píxeis + (size_t)py * passo_orig;

        for (int32_t kx = -1; kx <= 1; ++kx) {
          int32_t px = x + kx;
          if (px < 0) {
            px = 0;
          } else if (px >= larg) {
            px = larg - 1;
          }

          const float peso = núcleo.m[ky + 1][kx + 1];
          const Cor amostra = linha_amostra[px];

          soma_r += (float)amostra.vermelho * peso;
          soma_g += (float)amostra.verde * peso;
          soma_b += (float)amostra.azul * peso;
        }
      }

      linha_dest[x] = (Cor){
        .vermelho = limitar_uint8(soma_r * fator_div + viés),
        .verde    = limitar_uint8(soma_g * fator_div + viés),
        .azul     = limitar_uint8(soma_b * fator_div + viés),
        .alfa     = linha_orig_atual[x].alfa
      };
    }
  }
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



void destruir_malha3d(Malha3D *malha) {
  if (malha == nullptr) {
    return;
  }

  free(malha->vértices);
  free(malha->índices);
  malha->vértices = nullptr;
  malha->índices = nullptr;
  malha->núm_vértices = 0;
  malha->núm_índices = 0;
}

Malha3D criar_malha_cubo(float lado) {
  constexpr size_t núm_vértices = 24;
  constexpr size_t núm_índices = 36;

  Vértice3D *vértices = malloc(núm_vértices * sizeof(Vértice3D));
  uint32_t *índices = malloc(núm_índices * sizeof(uint32_t));
  if (vértices == nullptr || índices == nullptr) {
    free(vértices);
    free(índices);
    return (Malha3D){ 0 };
  }

  const float m = lado * 0.5f;

  /* 6 faces * 4 vértices cada (para normais e UVs nítidos por face) */
  const Vértice3D dados_vértices[24] = {
    /* Frente (+Z) */
    { .posição = { -m, -m,  m }, .normal = {  0.0f,  0.0f,  1.0f }, .uv = { 0.0f, 1.0f }, .cor = COR_BRANCA },
    { .posição = {  m, -m,  m }, .normal = {  0.0f,  0.0f,  1.0f }, .uv = { 1.0f, 1.0f }, .cor = COR_BRANCA },
    { .posição = {  m,  m,  m }, .normal = {  0.0f,  0.0f,  1.0f }, .uv = { 1.0f, 0.0f }, .cor = COR_BRANCA },
    { .posição = { -m,  m,  m }, .normal = {  0.0f,  0.0f,  1.0f }, .uv = { 0.0f, 0.0f }, .cor = COR_BRANCA },
    /* Trás (-Z) */
    { .posição = {  m, -m, -m }, .normal = {  0.0f,  0.0f, -1.0f }, .uv = { 0.0f, 1.0f }, .cor = COR_BRANCA },
    { .posição = { -m, -m, -m }, .normal = {  0.0f,  0.0f, -1.0f }, .uv = { 1.0f, 1.0f }, .cor = COR_BRANCA },
    { .posição = { -m,  m, -m }, .normal = {  0.0f,  0.0f, -1.0f }, .uv = { 1.0f, 0.0f }, .cor = COR_BRANCA },
    { .posição = {  m,  m, -m }, .normal = {  0.0f,  0.0f, -1.0f }, .uv = { 0.0f, 0.0f }, .cor = COR_BRANCA },
    /* Direita (+X) */
    { .posição = {  m, -m,  m }, .normal = {  1.0f,  0.0f,  0.0f }, .uv = { 0.0f, 1.0f }, .cor = COR_BRANCA },
    { .posição = {  m, -m, -m }, .normal = {  1.0f,  0.0f,  0.0f }, .uv = { 1.0f, 1.0f }, .cor = COR_BRANCA },
    { .posição = {  m,  m, -m }, .normal = {  1.0f,  0.0f,  0.0f }, .uv = { 1.0f, 0.0f }, .cor = COR_BRANCA },
    { .posição = {  m,  m,  m }, .normal = {  1.0f,  0.0f,  0.0f }, .uv = { 0.0f, 0.0f }, .cor = COR_BRANCA },
    /* Esquerda (-X) */
    { .posição = { -m, -m, -m }, .normal = { -1.0f,  0.0f,  0.0f }, .uv = { 0.0f, 1.0f }, .cor = COR_BRANCA },
    { .posição = { -m, -m,  m }, .normal = { -1.0f,  0.0f,  0.0f }, .uv = { 1.0f, 1.0f }, .cor = COR_BRANCA },
    { .posição = { -m,  m,  m }, .normal = { -1.0f,  0.0f,  0.0f }, .uv = { 1.0f, 0.0f }, .cor = COR_BRANCA },
    { .posição = { -m,  m, -m }, .normal = { -1.0f,  0.0f,  0.0f }, .uv = { 0.0f, 0.0f }, .cor = COR_BRANCA },
    /* Topo (+Y) */
    { .posição = { -m,  m,  m }, .normal = {  0.0f,  1.0f,  0.0f }, .uv = { 0.0f, 1.0f }, .cor = COR_BRANCA },
    { .posição = {  m,  m,  m }, .normal = {  0.0f,  1.0f,  0.0f }, .uv = { 1.0f, 1.0f }, .cor = COR_BRANCA },
    { .posição = {  m,  m, -m }, .normal = {  0.0f,  1.0f,  0.0f }, .uv = { 1.0f, 0.0f }, .cor = COR_BRANCA },
    { .posição = { -m,  m, -m }, .normal = {  0.0f,  1.0f,  0.0f }, .uv = { 0.0f, 0.0f }, .cor = COR_BRANCA },
    /* Base (-Y) */
    { .posição = { -m, -m, -m }, .normal = {  0.0f, -1.0f,  0.0f }, .uv = { 0.0f, 1.0f }, .cor = COR_BRANCA },
    { .posição = {  m, -m, -m }, .normal = {  0.0f, -1.0f,  0.0f }, .uv = { 1.0f, 1.0f }, .cor = COR_BRANCA },
    { .posição = {  m, -m,  m }, .normal = {  0.0f, -1.0f,  0.0f }, .uv = { 1.0f, 0.0f }, .cor = COR_BRANCA },
    { .posição = { -m, -m,  m }, .normal = {  0.0f, -1.0f,  0.0f }, .uv = { 0.0f, 0.0f }, .cor = COR_BRANCA }
  };

  memcpy(vértices, dados_vértices, sizeof(dados_vértices));

  for (uint32_t face = 0; face < 6; ++face) {
    const uint32_t base_v = face * 4;
    const uint32_t base_i = face * 6;
    índices[base_i + 0] = base_v + 0;
    índices[base_i + 1] = base_v + 1;
    índices[base_i + 2] = base_v + 2;
    índices[base_i + 3] = base_v + 0;
    índices[base_i + 4] = base_v + 2;
    índices[base_i + 5] = base_v + 3;
  }

  return (Malha3D){
    .vértices = vértices,
    .núm_vértices = núm_vértices,
    .índices = índices,
    .núm_índices = núm_índices
  };
}

Malha3D criar_malha_esfera(float raio, uint32_t fatias, uint32_t pilhas) {
  if (fatias < 3 || pilhas < 2) {
    return (Malha3D){ 0 };
  }

  constexpr float pi = 3.14159265358979323846f;
  const size_t núm_vértices = (size_t)(pilhas + 1) * (size_t)(fatias + 1);
  const size_t núm_índices = (size_t)pilhas * (size_t)fatias * 6;

  Vértice3D *vértices = malloc(núm_vértices * sizeof(Vértice3D));
  uint32_t *índices = malloc(núm_índices * sizeof(uint32_t));
  if (vértices == nullptr || índices == nullptr) {
    free(vértices);
    free(índices);
    return (Malha3D){ 0 };
  }

  size_t idx_v = 0;
  for (uint32_t p = 0; p <= pilhas; ++p) {
    const float v = (float)p / (float)pilhas;
    const float phi = v * pi;
    const float sin_phi = sinf(phi);
    const float cos_phi = cosf(phi);

    for (uint32_t f = 0; f <= fatias; ++f) {
      const float u = (float)f / (float)fatias;
      const float theta = u * 2.0f * pi;
      const float sin_theta = sinf(theta);
      const float cos_theta = cosf(theta);

      const Flut3D normal = {
        .x = cos_theta * sin_phi,
        .y = cos_phi,
        .z = sin_theta * sin_phi
      };

      vértices[idx_v++] = (Vértice3D){
        .posição = escalar_flut3d(normal, raio),
        .normal = normal,
        .uv = { .x = u, .y = v },
        .cor = COR_BRANCA
      };
    }
  }

  size_t idx_i = 0;
  const uint32_t passo_anel = fatias + 1;
  for (uint32_t p = 0; p < pilhas; ++p) {
    for (uint32_t f = 0; f < fatias; ++f) {
      const uint32_t a = (p * passo_anel) + f;
      const uint32_t b = a + passo_anel;
      const uint32_t c = a + 1;
      const uint32_t d = b + 1;

      /* Sentido anti-horário (CCW) visto de fora */
      índices[idx_i++] = a;
      índices[idx_i++] = c;
      índices[idx_i++] = b;

      índices[idx_i++] = c;
      índices[idx_i++] = d;
      índices[idx_i++] = b;
    }
  }

  return (Malha3D){
    .vértices = vértices,
    .núm_vértices = núm_vértices,
    .índices = índices,
    .núm_índices = núm_índices
  };
}

Malha3D criar_malha_plano(float largura, float profundidade, uint32_t subdivisões) {
  const uint32_t segs = (subdivisões == 0) ? 1 : subdivisões;
  const size_t núm_vértices = (size_t)(segs + 1) * (size_t)(segs + 1);
  const size_t núm_índices = (size_t)segs * (size_t)segs * 6;

  Vértice3D *vértices = malloc(núm_vértices * sizeof(Vértice3D));
  uint32_t *índices = malloc(núm_índices * sizeof(uint32_t));
  if (vértices == nullptr || índices == nullptr) {
    free(vértices);
    free(índices);
    return (Malha3D){ 0 };
  }

  const float meia_larg = largura * 0.5f;
  const float meia_prof = profundidade * 0.5f;

  size_t idx_v = 0;
  for (uint32_t z = 0; z <= segs; ++z) {
    const float v = (float)z / (float)segs;
    const float pos_z = -meia_prof + (v * profundidade);

    for (uint32_t x = 0; x <= segs; ++x) {
      const float u = (float)x / (float)segs;
      const float pos_x = -meia_larg + (u * largura);

      vértices[idx_v++] = (Vértice3D){
        .posição = { .x = pos_x, .y = 0.0f, .z = pos_z },
        .normal = { .x = 0.0f, .y = 1.0f, .z = 0.0f },
        .uv = { .x = u, .y = v },
        .cor = COR_BRANCA
      };
    }
  }

  size_t idx_i = 0;
  const uint32_t passo_linha = segs + 1;
  for (uint32_t z = 0; z < segs; ++z) {
    for (uint32_t x = 0; x < segs; ++x) {
      const uint32_t sup_esq = (z * passo_linha) + x;
      const uint32_t sup_dir = sup_esq + 1;
      const uint32_t inf_esq = sup_esq + passo_linha;
      const uint32_t inf_dir = inf_esq + 1;

      índices[idx_i++] = sup_esq;
      índices[idx_i++] = inf_esq;
      índices[idx_i++] = sup_dir;

      índices[idx_i++] = sup_dir;
      índices[idx_i++] = inf_esq;
      índices[idx_i++] = inf_dir;
    }
  }

  return (Malha3D){
    .vértices = vértices,
    .núm_vértices = núm_vértices,
    .índices = índices,
    .núm_índices = núm_índices
  };
}

bool carregar_malha_obj(Malha3D *destino, const char *caminho) {
  if (destino == nullptr || caminho == nullptr) {
    return false;
  }

  FILE *ficheiro = fopen(caminho, "r");
  if (ficheiro == nullptr) {
    return false;
  }

  Flut3D *posições = nullptr;
  size_t núm_pos = 0, cap_pos = 0;

  Flut2D *uvs = nullptr;
  size_t núm_uvs = 0, cap_uvs = 0;

  Flut3D *normais = nullptr;
  size_t núm_norm = 0, cap_norm = 0;

  Vértice3D *vértices = nullptr;
  size_t núm_vértices = 0, cap_vértices = 0;

  uint32_t *índices = nullptr;
  size_t núm_índices = 0, cap_índices = 0;

  bool sucesso = true;
  bool tem_normais = false;
  char linha[512];

  while (fgets(linha, sizeof(linha), ficheiro) != nullptr && sucesso) {
    char *p = linha;
    while (*p == ' ' || *p == '\t') {
      ++p;
    }

    if (strncmp(p, "v ", 2) == 0) {
      Flut3D pos = { 0 };
      if (sscanf(p + 2, "%f %f %f", &pos.x, &pos.y, &pos.z) == 3) {
        if (núm_pos == cap_pos) {
          const size_t nova_cap = (cap_pos == 0) ? 128 : cap_pos * 2;
          Flut3D *tmp = realloc(posições, nova_cap * sizeof(Flut3D));
          if (tmp == nullptr) { sucesso = false; break; }
          posições = tmp;
          cap_pos = nova_cap;
        }
        posições[núm_pos++] = pos;
      }
    } else if (strncmp(p, "vt ", 3) == 0) {
      Flut2D uv = { 0 };
      if (sscanf(p + 3, "%f %f", &uv.x, &uv.y) >= 2) {
        uv.y = 1.0f - uv.y; /* Converte UV OpenGL (baixo=0) para Tela (topo=0) */
        if (núm_uvs == cap_uvs) {
          const size_t nova_cap = (cap_uvs == 0) ? 128 : cap_uvs * 2;
          Flut2D *tmp = realloc(uvs, nova_cap * sizeof(Flut2D));
          if (tmp == nullptr) { sucesso = false; break; }
          uvs = tmp;
          cap_uvs = nova_cap;
        }
        uvs[núm_uvs++] = uv;
      }
    } else if (strncmp(p, "vn ", 3) == 0) {
      Flut3D norm = { 0 };
      if (sscanf(p + 3, "%f %f %f", &norm.x, &norm.y, &norm.z) == 3) {
        if (núm_norm == cap_norm) {
          const size_t nova_cap = (cap_norm == 0) ? 128 : cap_norm * 2;
          Flut3D *tmp = realloc(normais, nova_cap * sizeof(Flut3D));
          if (tmp == nullptr) { sucesso = false; break; }
          normais = tmp;
          cap_norm = nova_cap;
        }
        normais[núm_norm++] = normalizar_flut3d(norm);
        tem_normais = true;
      }
    } else if (strncmp(p, "f ", 2) == 0) {
      uint32_t vértices_face[32];
      size_t count_face = 0;

      char *token = strtok(p + 2, " \t\r\n");
      while (token != nullptr && count_face < 32) {
        int32_t iv = 0, ivt = 0, ivn = 0;

        if (sscanf(token, "%d/%d/%d", &iv, &ivt, &ivn) == 3) {
          /* v/vt/vn */
        } else if (sscanf(token, "%d//%d", &iv, &ivn) == 2) {
          ivt = 0;
        } else if (sscanf(token, "%d/%d", &iv, &ivt) == 2) {
          ivn = 0;
        } else if (sscanf(token, "%d", &iv) == 1) {
          ivt = 0;
          ivn = 0;
        } else {
          token = strtok(nullptr, " \t\r\n");
          continue;
        }

        if (iv < 0) iv = (int32_t)núm_pos + iv + 1;
        if (ivt < 0) ivt = (int32_t)núm_uvs + ivt + 1;
        if (ivn < 0) ivn = (int32_t)núm_norm + ivn + 1;

        if (iv <= 0 || (size_t)iv > núm_pos) {
          sucesso = false;
          break;
        }

        Vértice3D novo_v = {
          .posição = posições[iv - 1],
          .normal = (ivn > 0 && (size_t)ivn <= núm_norm) ? normais[ivn - 1] : (Flut3D){ 0.0f, 1.0f, 0.0f },
          .uv = (ivt > 0 && (size_t)ivt <= núm_uvs) ? uvs[ivt - 1] : (Flut2D){ 0.0f, 0.0f },
          .cor = COR_BRANCA
        };

        if (núm_vértices == cap_vértices) {
          const size_t nova_cap = (cap_vértices == 0) ? 256 : cap_vértices * 2;
          Vértice3D *tmp = realloc(vértices, nova_cap * sizeof(Vértice3D));
          if (tmp == nullptr) { sucesso = false; break; }
          vértices = tmp;
          cap_vértices = nova_cap;
        }

        vértices_face[count_face++] = (uint32_t)núm_vértices;
        vértices[núm_vértices++] = novo_v;

        token = strtok(nullptr, " \t\r\n");
      }

      /* Triangulação em leque (triangle fan) para faces com 3 ou mais vértices */
      for (size_t i = 1; i + 1 < count_face && sucesso; ++i) {
        if (núm_índices + 3 > cap_índices) {
          const size_t nova_cap = (cap_índices == 0) ? 512 : cap_índices * 2;
          uint32_t *tmp = realloc(índices, nova_cap * sizeof(uint32_t));
          if (tmp == nullptr) { sucesso = false; break; }
          índices = tmp;
          cap_índices = nova_cap;
        }
        índices[núm_índices++] = vértices_face[0];
        índices[núm_índices++] = vértices_face[i];
        índices[núm_índices++] = vértices_face[i + 1];
      }
    }
  }

  fclose(ficheiro);
  free(posições);
  free(uvs);
  free(normais);

  if (!sucesso || núm_vértices == 0 || núm_índices == 0) {
    free(vértices);
    free(índices);
    return false;
  }

  *destino = (Malha3D){
    .vértices = vértices,
    .núm_vértices = núm_vértices,
    .índices = índices,
    .núm_índices = núm_índices
  };

  if (!tem_normais) {
    calcular_normais_malha(destino);
  }

  return true;
}

void calcular_normais_malha(Malha3D *malha) {
  if (malha == nullptr || malha->vértices == nullptr || malha->índices == nullptr) {
    return;
  }

  for (size_t i = 0; i < malha->núm_vértices; ++i) {
    malha->vértices[i].normal = (Flut3D){ 0.0f, 0.0f, 0.0f };
  }

  for (size_t i = 0; i + 2 < malha->núm_índices; i += 3) {
    const uint32_t i0 = malha->índices[i + 0];
    const uint32_t i1 = malha->índices[i + 1];
    const uint32_t i2 = malha->índices[i + 2];

    if (i0 >= malha->núm_vértices || i1 >= malha->núm_vértices || i2 >= malha->núm_vértices) {
      continue;
    }

    const Flut3D p0 = malha->vértices[i0].posição;
    const Flut3D p1 = malha->vértices[i1].posição;
    const Flut3D p2 = malha->vértices[i2].posição;

    const Flut3D aresta1 = subtrair_flut3d(p1, p0);
    const Flut3D aresta2 = subtrair_flut3d(p2, p0);
    const Flut3D normal_face = produto_vetorial_flut3d(aresta1, aresta2);

    malha->vértices[i0].normal = somar_flut3d(malha->vértices[i0].normal, normal_face);
    malha->vértices[i1].normal = somar_flut3d(malha->vértices[i1].normal, normal_face);
    malha->vértices[i2].normal = somar_flut3d(malha->vértices[i2].normal, normal_face);
  }

  for (size_t i = 0; i < malha->núm_vértices; ++i) {
    malha->vértices[i].normal = normalizar_flut3d(malha->vértices[i].normal);
  }
}



CaixaDelimitadora3D calcular_caixa_delimitadora_malha(const Malha3D *malha) {
  if (malha == nullptr || malha->vértices == nullptr || malha->núm_vértices == 0) {
    return (CaixaDelimitadora3D){
      .mínimo = { .x = 0.0f, .y = 0.0f, .z = 0.0f },
      .máximo = { .x = 0.0f, .y = 0.0f, .z = 0.0f }
    };
  }

  Flut3D mínimo = malha->vértices[0].posição;
  Flut3D máximo = malha->vértices[0].posição;

  for (size_t i = 1; i < malha->núm_vértices; ++i) {
    const Flut3D pos = malha->vértices[i].posição;

    if (pos.x < mínimo.x) mínimo.x = pos.x;
    if (pos.y < mínimo.y) mínimo.y = pos.y;
    if (pos.z < mínimo.z) mínimo.z = pos.z;

    if (pos.x > máximo.x) máximo.x = pos.x;
    if (pos.y > máximo.y) máximo.y = pos.y;
    if (pos.z > máximo.z) máximo.z = pos.z;
  }

  return (CaixaDelimitadora3D){
    .mínimo = mínimo,
    .máximo = máximo
  };
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

void desenhar_linha_3d(Tela *tela, BufferProfundidade *zbuffer, Flut3D início, Flut3D fim, const Matriz4x4 *mvp, Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || zbuffer == nullptr || zbuffer->valores == nullptr || mvp == nullptr) {
    return;
  }

  /* Transforma para o espaço de recorte (Clip Space) */
  Flut4D c0 = transformar_flut4d_matriz4x4((Flut4D){ início.x, início.y, início.z, 1.0f }, mvp);
  Flut4D c1 = transformar_flut4d_matriz4x4((Flut4D){ fim.x, fim.y, fim.z, 1.0f }, mvp);

  /* Rejeição trivial se ambos os pontos estiverem atrás do plano próximo (z < 0) */
  if (c0.z < 0.0f && c1.z < 0.0f) {
    return;
  }

  /* Recorte da linha contra o plano próximo (z >= 0) */
  if (c0.z < 0.0f) {
    const float t = c0.z / (c0.z - c1.z);
    c0 = (Flut4D){
      .x = c0.x + ((c1.x - c0.x) * t),
      .y = c0.y + ((c1.y - c0.y) * t),
      .z = 0.0f,
      .w = c0.w + ((c1.w - c0.w) * t)
    };
  } else if (c1.z < 0.0f) {
    const float t = c1.z / (c1.z - c0.z);
    c1 = (Flut4D){
      .x = c1.x + ((c0.x - c1.x) * t),
      .y = c1.y + ((c0.y - c1.y) * t),
      .z = 0.0f,
      .w = c1.w + ((c0.w - c1.w) * t)
    };
  }

  const Flut3D p0 = projetar_para_ecrã(c0, tela->largura, tela->altura);
  const Flut3D p1 = projetar_para_ecrã(c1, tela->largura, tela->altura);

  /* Algoritmo DDA com interpolação linear de profundidade Z */
  const float dx = p1.x - p0.x;
  const float dy = p1.y - p0.y;
  const float dz = p1.z - p0.z;

  const float passos = fmaxf(fabsf(dx), fabsf(dy));
  if (passos < 1.0f) {
    const int32_t x = (int32_t)roundf(p0.x);
    const int32_t y = (int32_t)roundf(p0.y);
    if (x >= 0 && (uint32_t)x < tela->largura && y >= 0 && (uint32_t)y < tela->altura) {
      const size_t idx = ((size_t)y * tela->largura) + (size_t)x;
      if (p0.z >= 0.0f && p0.z <= 1.0f && p0.z < zbuffer->valores[idx]) {
        zbuffer->valores[idx] = p0.z;
        tela->píxeis[idx] = cor;
      }
    }
    return;
  }

  const float inc_x = dx / passos;
  const float inc_y = dy / passos;
  const float inc_z = dz / passos;

  float x = p0.x;
  float y = p0.y;
  float z = p0.z;

  const uint32_t total_passos = (uint32_t)passos;
  for (uint32_t i = 0; i <= total_passos; ++i) {
    const int32_t px = (int32_t)roundf(x);
    const int32_t py = (int32_t)roundf(y);

    if (px >= 0 && (uint32_t)px < tela->largura && py >= 0 && (uint32_t)py < tela->altura) {
      const size_t idx = ((size_t)py * tela->largura) + (size_t)px;
      if (z >= 0.0f && z <= 1.0f && z < zbuffer->valores[idx]) {
        zbuffer->valores[idx] = z;
        tela->píxeis[idx] = cor;
      }
    }

    x += inc_x;
    y += inc_y;
    z += inc_z;
  }
}



static void desenhar_aresta_2d_zbuffer(Tela *tela, BufferProfundidade *zbuffer, Flut3D p0, Flut3D p1, Cor cor) {
  const float dx = p1.x - p0.x;
  const float dy = p1.y - p0.y;
  const float dz = p1.z - p0.z;

  const float passos = fmaxf(fabsf(dx), fabsf(dy));
  if (passos < 1.0f) {
    const int32_t x = (int32_t)roundf(p0.x);
    const int32_t y = (int32_t)roundf(p0.y);
    if (x >= 0 && (uint32_t)x < tela->largura && y >= 0 && (uint32_t)y < tela->altura) {
      const size_t idx = ((size_t)y * tela->largura) + (size_t)x;
      if (p0.z >= 0.0f && p0.z <= 1.0f && p0.z <= zbuffer->valores[idx]) {
        zbuffer->valores[idx] = p0.z;
        tela->píxeis[idx] = cor;
      }
    }
    return;
  }

  const float inc_x = dx / passos;
  const float inc_y = dy / passos;
  const float inc_z = dz / passos;

  float x = p0.x;
  float y = p0.y;
  float z = p0.z;

  const uint32_t total_passos = (uint32_t)passos;
  for (uint32_t i = 0; i <= total_passos; ++i) {
    const int32_t px = (int32_t)roundf(x);
    const int32_t py = (int32_t)roundf(y);

    if (px >= 0 && (uint32_t)px < tela->largura && py >= 0 && (uint32_t)py < tela->altura) {
      const size_t idx = ((size_t)py * tela->largura) + (size_t)px;
      if (z >= 0.0f && z <= 1.0f && z <= zbuffer->valores[idx]) {
        zbuffer->valores[idx] = z;
        tela->píxeis[idx] = cor;
      }
    }

    x += inc_x;
    y += inc_y;
    z += inc_z;
  }
}

static float função_aresta_2d(Flut3D a, Flut3D b, float px, float py) {
  return ((px - a.x) * (b.y - a.y)) - ((py - a.y) * (b.x - a.x));
}

static Cor amostrar_textura_uv(const Tela *textura, float u, float v) {
  if (textura == nullptr || textura->píxeis == nullptr || textura->largura == 0 || textura->altura == 0) {
    return COR_BRANCA;
  }

  u = u - floorf(u);
  v = v - floorf(v);

  const uint32_t tx = (uint32_t)(u * (float)(textura->largura - 1));
  const uint32_t ty = (uint32_t)(v * (float)(textura->altura - 1));
  return textura->píxeis[((size_t)ty * textura->largura) + (size_t)tx];
}

static Cor modular_cores(Cor a, Cor b) {
  return (Cor){
    .vermelho = (uint8_t)(((uint32_t)a.vermelho * (uint32_t)b.vermelho) / 255U),
    .verde = (uint8_t)(((uint32_t)a.verde * (uint32_t)b.verde) / 255U),
    .azul = (uint8_t)(((uint32_t)a.azul * (uint32_t)b.azul) / 255U),
    .alfa = (uint8_t)(((uint32_t)a.alfa * (uint32_t)b.alfa) / 255U)
  };
}

static Cor calcular_iluminação_vértice_v2(Flut3D pos_mundo, Flut3D normal_mundo, Cor cor_vértice, const Luz3D luzes[], size_t núm_luzes) {
  if (luzes == nullptr || núm_luzes == 0) {
    return cor_vértice;
  }

  /* Luz ambiente base suave */
  float acum_r = 0.15f;
  float acum_g = 0.15f;
  float acum_b = 0.15f;

  for (size_t i = 0; i < núm_luzes; ++i) {
    const Luz3D luz = luzes[i];
    Flut3D dir_luz = { 0 };
    float atenuação = 1.0f;

    if (luz.tipo == LUZ_DIRECIONAL) {
      dir_luz = normalizar_flut3d(escalar_flut3d(luz.direção, -1.0f));
    } else {
      const Flut3D vetor_luz = subtrair_flut3d(luz.posição, pos_mundo);
      const float dist = comprimento_flut3d(vetor_luz);

      if (dist <= 1e-4f || (luz.alcance > 0.0f && dist > luz.alcance)) {
        continue;
      }

      dir_luz = escalar_flut3d(vetor_luz, 1.0f / dist);

      /* Atenuação suave baseada no alcance da luz */
      if (luz.alcance > 0.0f) {
        const float razão = dist / luz.alcance;
        const float queda = fmaxf(0.0f, 1.0f - (razão * razão));
        atenuação = queda * queda;
      } else {
        atenuação = 1.0f / (1.0f + (0.09f * dist) + (0.032f * dist * dist));
      }

      /* Cálculo do cone se for um holofote (Spotlight) */
      if (luz.tipo == LUZ_HOLOFOTE) {
        const Flut3D dir_foco = normalizar_flut3d(luz.direção);
        const Flut3D luz_para_ponto = escalar_flut3d(dir_luz, -1.0f);
        const float cos_ângulo = produto_escalar_flut3d(dir_foco, luz_para_ponto);
        const float cos_limite = cosf(luz.ângulo_cone_radianos * 0.5f);

        if (cos_ângulo < cos_limite) {
          continue;
        }

        /* Suavização da borda do cone nos últimos 15% do ângulo */
        const float cos_interior = cosf(luz.ângulo_cone_radianos * 0.425f);
        if (cos_ângulo < cos_interior && (cos_interior - cos_limite) > 1e-5f) {
          const float borda = (cos_ângulo - cos_limite) / (cos_interior - cos_limite);
          atenuação *= borda;
        }
      }
    }

    const float fator_difuso = fmaxf(0.0f, produto_escalar_flut3d(normal_mundo, dir_luz)) * luz.intensidade * atenuação;

    acum_r += ((float)luz.cor.vermelho / 255.0f) * fator_difuso;
    acum_g += ((float)luz.cor.verde / 255.0f) * fator_difuso;
    acum_b += ((float)luz.cor.azul / 255.0f) * fator_difuso;
  }

  const float r = fminf(255.0f, (float)cor_vértice.vermelho * acum_r);
  const float g = fminf(255.0f, (float)cor_vértice.verde * acum_g);
  const float b = fminf(255.0f, (float)cor_vértice.azul * acum_b);

  return (Cor){
    .vermelho = (uint8_t)r,
    .verde = (uint8_t)g,
    .azul = (uint8_t)b,
    .alfa = cor_vértice.alfa
  };
}

static void desenhar_triângulo_3d_interno(
  Tela *tela,
  BufferProfundidade *zbuffer,
  Vértice3D v1,
  Vértice3D v2,
  Vértice3D v3,
  const Flut3D pos_mundo[3],
  const Material3D *material,
  const Luz3D luzes[],
  size_t núm_luzes,
  ModoSombreamento modo
) {
  if (tela == nullptr || tela->píxeis == nullptr || zbuffer == nullptr || zbuffer->valores == nullptr) {
    return;
  }

  if (modo == SOMBREAMENTO_ARAME) {
    const Cor cor_arame = (material != nullptr) ? material->cor_difusa : v1.cor;
    desenhar_aresta_2d_zbuffer(tela, zbuffer, v1.posição, v2.posição, cor_arame);
    desenhar_aresta_2d_zbuffer(tela, zbuffer, v2.posição, v3.posição, cor_arame);
    desenhar_aresta_2d_zbuffer(tela, zbuffer, v3.posição, v1.posição, cor_arame);
    return;
  }

  const float área_total = função_aresta_2d(v1.posição, v2.posição, v3.posição.x, v3.posição.y);
  if (fabsf(área_total) < 1e-5f) {
    return;
  }

  const float min_x_f = fmaxf(0.0f, fminf(v1.posição.x, fminf(v2.posição.x, v3.posição.x)));
  const float min_y_f = fmaxf(0.0f, fminf(v1.posição.y, fminf(v2.posição.y, v3.posição.y)));
  const float max_x_f = fminf((float)(tela->largura - 1), fmaxf(v1.posição.x, fmaxf(v2.posição.x, v3.posição.x)));
  const float max_y_f = fminf((float)(tela->altura - 1), fmaxf(v1.posição.y, fmaxf(v2.posição.y, v3.posição.y)));

  const int32_t min_x = (int32_t)floorf(min_x_f);
  const int32_t min_y = (int32_t)floorf(min_y_f);
  const int32_t max_x = (int32_t)ceilf(max_x_f);
  const int32_t max_y = (int32_t)ceilf(max_y_f);

  const float inv_área = 1.0f / área_total;
  const bool sentido_positivo = (área_total > 0.0f);

  for (int32_t y = min_y; y <= max_y; ++y) {
    const float py = (float)y + 0.5f;

    for (int32_t x = min_x; x <= max_x; ++x) {
      const float px = (float)x + 0.5f;

      const float w1 = função_aresta_2d(v2.posição, v3.posição, px, py);
      const float w2 = função_aresta_2d(v3.posição, v1.posição, px, py);
      const float w3 = função_aresta_2d(v1.posição, v2.posição, px, py);

      const bool dentro = sentido_positivo
        ? (w1 >= 0.0f && w2 >= 0.0f && w3 >= 0.0f)
        : (w1 <= 0.0f && w2 <= 0.0f && w3 <= 0.0f);

      if (!dentro) {
        continue;
      }

      const float b1 = w1 * inv_área;
      const float b2 = w2 * inv_área;
      const float b3 = w3 * inv_área;

      const float z = (b1 * v1.posição.z) + (b2 * v2.posição.z) + (b3 * v3.posição.z);
      const size_t idx = ((size_t)y * tela->largura) + (size_t)x;

      if (z < 0.0f || z > 1.0f || z >= zbuffer->valores[idx]) {
        continue;
      }

      zbuffer->valores[idx] = z;

      Cor cor_base = v1.cor;

      if (modo == SOMBREAMENTO_GOURAUD || modo == SOMBREAMENTO_PHONG) {
        cor_base = (Cor){
          .vermelho = (uint8_t)((b1 * (float)v1.cor.vermelho) + (b2 * (float)v2.cor.vermelho) + (b3 * (float)v3.cor.vermelho)),
          .verde = (uint8_t)((b1 * (float)v1.cor.verde) + (b2 * (float)v2.cor.verde) + (b3 * (float)v3.cor.verde)),
          .azul = (uint8_t)((b1 * (float)v1.cor.azul) + (b2 * (float)v2.cor.azul) + (b3 * (float)v3.cor.azul)),
          .alfa = 255
        };
      }

      if (material != nullptr) {
        cor_base = modular_cores(cor_base, material->cor_difusa);
        if (material->textura_difusa != nullptr) {
          const float u = (b1 * v1.uv.x) + (b2 * v2.uv.x) + (b3 * v3.uv.x);
          const float v = (b1 * v1.uv.y) + (b2 * v2.uv.y) + (b3 * v3.uv.y);
          const Cor texel = amostrar_textura_uv(material->textura_difusa, u, v);
          cor_base = modular_cores(cor_base, texel);
        }
      }

      if (modo == SOMBREAMENTO_PHONG && pos_mundo != nullptr && luzes != nullptr && núm_luzes > 0) {
        const Flut3D pos_píxel = {
          .x = (b1 * pos_mundo[0].x) + (b2 * pos_mundo[1].x) + (b3 * pos_mundo[2].x),
          .y = (b1 * pos_mundo[0].y) + (b2 * pos_mundo[1].y) + (b3 * pos_mundo[2].y),
          .z = (b1 * pos_mundo[0].z) + (b2 * pos_mundo[1].z) + (b3 * pos_mundo[2].z)
        };
        const Flut3D norm_píxel = normalizar_flut3d((Flut3D){
          .x = (b1 * v1.normal.x) + (b2 * v2.normal.x) + (b3 * v3.normal.x),
          .y = (b1 * v1.normal.y) + (b2 * v2.normal.y) + (b3 * v3.normal.y),
          .z = (b1 * v1.normal.z) + (b2 * v2.normal.z) + (b3 * v3.normal.z)
        });
        cor_base = calcular_iluminação_vértice_v2(pos_píxel, norm_píxel, cor_base, luzes, núm_luzes);
      }

      tela->píxeis[idx] = cor_base;
    }
  }
}

void desenhar_triângulo_3d_zbuffer(Tela *tela, BufferProfundidade *zbuffer, Vértice3D v1, Vértice3D v2, Vértice3D v3, const Material3D *material, ModoSombreamento modo) {
  const ModoSombreamento modo_efetivo = (modo == SOMBREAMENTO_PHONG) ? SOMBREAMENTO_GOURAUD : modo;
  desenhar_triângulo_3d_interno(tela, zbuffer, v1, v2, v3, nullptr, material, nullptr, 0, modo_efetivo);
}

static inline uint8_t flut_para_u8(float valor) {
  float limitado = limitar_flut(valor, 0.0f, 1.0f);
  return (uint8_t)lroundf(limitado * 255.0f);
}

void transpor_matriz4x4(Matriz4x4 *destino, const Matriz4x4 *origem) {
  Matriz4x4 temp;
  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 4; ++j) {
      temp.m[i][j] = origem->m[j][i];
    }
  }
  *destino = temp;
}

static Flut3D transformar_normal_matriz4x4(Flut3D normal, const Matriz4x4 *matriz_normal) {
  Flut3D resultado = {
    .x = normal.x * matriz_normal->m[0][0] + normal.y * matriz_normal->m[1][0] + normal.z * matriz_normal->m[2][0],
    .y = normal.x * matriz_normal->m[0][1] + normal.y * matriz_normal->m[1][1] + normal.z * matriz_normal->m[2][1],
    .z = normal.x * matriz_normal->m[0][2] + normal.y * matriz_normal->m[1][2] + normal.z * matriz_normal->m[2][2]
  };
  return normalizar_flut3d(resultado);
}

static Cor calcular_iluminação_vértice(
  Flut3D pos_mundo,
  Flut3D normal_mundo,
  Cor cor_vértice,
  Flut3D pos_câmara,
  const Material3D *material,
  const Luz3D luzes[],
  size_t núm_luzes
) {
  float mat_amb_r = (float)material->cor_ambiente.vermelho / 255.0f;
  float mat_amb_g = (float)material->cor_ambiente.verde    / 255.0f;
  float mat_amb_b = (float)material->cor_ambiente.azul     / 255.0f;

  float mat_dif_r = ((float)material->cor_difusa.vermelho / 255.0f) * ((float)cor_vértice.vermelho / 255.0f);
  float mat_dif_g = ((float)material->cor_difusa.verde    / 255.0f) * ((float)cor_vértice.verde    / 255.0f);
  float mat_dif_b = ((float)material->cor_difusa.azul     / 255.0f) * ((float)cor_vértice.azul     / 255.0f);

  float mat_esp_r = (float)material->cor_especular.vermelho / 255.0f;
  float mat_esp_g = (float)material->cor_especular.verde    / 255.0f;
  float mat_esp_b = (float)material->cor_especular.azul     / 255.0f;

  float acum_r = mat_amb_r * 0.15f;
  float acum_g = mat_amb_g * 0.15f;
  float acum_b = mat_amb_b * 0.15f;

  Flut3D dir_vista = normalizar_flut3d(subtrair_flut3d(pos_câmara, pos_mundo));

  for (size_t i = 0; i < núm_luzes; ++i) {
    const Luz3D *luz = &luzes[i];
    if (luz->intensidade <= 0.0f) continue;

    Flut3D dir_luz = { 0.0f, 0.0f, 0.0f };
    float atenuação = luz->intensidade;

    if (luz->tipo == LUZ_DIRECIONAL) {
      dir_luz = normalizar_flut3d(escalar_flut3d(luz->direção, -1.0f));
    } else {
      Flut3D vetor_para_luz = subtrair_flut3d(luz->posição, pos_mundo);
      float dist = comprimento_flut3d(vetor_para_luz);

      if (luz->alcance > 0.0f && dist > luz->alcance) {
        continue;
      }

      dir_luz = (dist > 1e-6f) ? escalar_flut3d(vetor_para_luz, 1.0f / dist) : (Flut3D){ 0.0f, 1.0f, 0.0f };

      if (luz->alcance > 0.0f) {
        float fator_dist = limitar_flut(1.0f - (dist / luz->alcance), 0.0f, 1.0f);
        atenuação *= fator_dist * fator_dist;
      }

      if (luz->tipo == LUZ_HOLOFOTE) {
        Flut3D dir_foco = normalizar_flut3d(luz->direção);
        Flut3D luz_para_ponto = escalar_flut3d(dir_luz, -1.0f);
        float cosseno_ângulo = produto_escalar_flut3d(dir_foco, luz_para_ponto);
        float cosseno_limite = cosf(luz->ângulo_cone_radianos * 0.5f);

        if (cosseno_ângulo < cosseno_limite) {
          continue;
        }
        float borda_suave = limitar_flut((cosseno_ângulo - cosseno_limite) / (1.0f - cosseno_limite + 1e-5f), 0.0f, 1.0f);
        atenuação *= borda_suave;
      }
    }

    float fator_difuso = fmaxf(produto_escalar_flut3d(normal_mundo, dir_luz), 0.0f);
    if (fator_difuso <= 0.0f) continue;

    float luz_r = ((float)luz->cor.vermelho / 255.0f) * atenuação;
    float luz_g = ((float)luz->cor.verde    / 255.0f) * atenuação;
    float luz_b = ((float)luz->cor.azul     / 255.0f) * atenuação;

    acum_r += mat_dif_r * luz_r * fator_difuso;
    acum_g += mat_dif_g * luz_g * fator_difuso;
    acum_b += mat_dif_b * luz_b * fator_difuso;

    if (material->brilho_especular > 0.0f) {
      Flut3D meio_vetor = normalizar_flut3d(somar_flut3d(dir_luz, dir_vista));
      float n_dot_h = fmaxf(produto_escalar_flut3d(normal_mundo, meio_vetor), 0.0f);
      float fator_especular = powf(n_dot_h, material->brilho_especular);

      acum_r += mat_esp_r * luz_r * fator_especular;
      acum_g += mat_esp_g * luz_g * fator_especular;
      acum_b += mat_esp_b * luz_b * fator_especular;
    }
  }

  float alfa_final = ((float)material->cor_difusa.alfa / 255.0f) * ((float)cor_vértice.alfa / 255.0f);

  return (Cor){
    .vermelho = flut_para_u8(acum_r),
    .verde    = flut_para_u8(acum_g),
    .azul     = flut_para_u8(acum_b),
    .alfa     = flut_para_u8(alfa_final)
  };
}

void desenhar_malha_3d(
  Tela *tela,
  BufferProfundidade *zbuffer,
  const Malha3D *malha,
  const Matriz4x4 *modelo,
  const Câmara3D *câmara,
  const Material3D *material,
  const Luz3D luzes[],
  size_t núm_luzes,
  ModoSombreamento modo
) {
  if (!tela || !tela->píxeis || !malha || !malha->vértices || !malha->índices || !câmara) {
    return;
  }
  if (tela->largura == 0 || tela->altura == 0 || malha->núm_índices < 3) {
    return;
  }

  Matriz4x4 mat_modelo;
  if (modelo) {
    mat_modelo = *modelo;
  } else {
    criar_matriz4x4_identidade(&mat_modelo);
  }

  Matriz4x4 mat_inversa;
  Matriz4x4 mat_normal;
  if (inverter_matriz4x4(&mat_inversa, &mat_modelo)) {
    transpor_matriz4x4(&mat_normal, &mat_inversa);
  } else {
    mat_normal = mat_modelo;
  }

  CaixaDelimitadora3D caixa_local = calcular_caixa_delimitadora_malha(malha);
  Flut3D cantos_locais[8] = {
    { caixa_local.mínimo.x, caixa_local.mínimo.y, caixa_local.mínimo.z },
    { caixa_local.máximo.x, caixa_local.mínimo.y, caixa_local.mínimo.z },
    { caixa_local.mínimo.x, caixa_local.máximo.y, caixa_local.mínimo.z },
    { caixa_local.máximo.x, caixa_local.máximo.y, caixa_local.mínimo.z },
    { caixa_local.mínimo.x, caixa_local.mínimo.y, caixa_local.máximo.z },
    { caixa_local.máximo.x, caixa_local.mínimo.y, caixa_local.máximo.z },
    { caixa_local.mínimo.x, caixa_local.máximo.y, caixa_local.máximo.z },
    { caixa_local.máximo.x, caixa_local.máximo.y, caixa_local.máximo.z }
  };

  CaixaDelimitadora3D caixa_mundo = {
    .mínimo = {  INFINITY,  INFINITY,  INFINITY },
    .máximo = { -INFINITY, -INFINITY, -INFINITY }
  };

  for (size_t i = 0; i < 8; ++i) {
    Flut3D canto_mundo = transformar_flut3d_matriz4x4(cantos_locais[i], &mat_modelo);
    caixa_mundo.mínimo.x = fminf(caixa_mundo.mínimo.x, canto_mundo.x);
    caixa_mundo.mínimo.y = fminf(caixa_mundo.mínimo.y, canto_mundo.y);
    caixa_mundo.mínimo.z = fminf(caixa_mundo.mínimo.z, canto_mundo.z);
    caixa_mundo.máximo.x = fmaxf(caixa_mundo.máximo.x, canto_mundo.x);
    caixa_mundo.máximo.y = fmaxf(caixa_mundo.máximo.y, canto_mundo.y);
    caixa_mundo.máximo.z = fmaxf(caixa_mundo.máximo.z, canto_mundo.z);
  }

  Matriz4x4 mat_visão;
  Flut3D alvo_câmara = somar_flut3d(câmara->posição, câmara->direção);
  criar_matriz4x4_visão(&mat_visão, câmara->posição, alvo_câmara, câmara->cima);

  Matriz4x4 mat_projeção;
  float aspeto = (float)tela->largura / (float)tela->altura;
  criar_matriz4x4_perspetiva(&mat_projeção, câmara->fov_radianos, aspeto, câmara->plano_próximo, câmara->plano_distante);

  Matriz4x4 mat_visão_projeção;
  multiplicar_matrizes4x4(&mat_visão_projeção, &mat_visão, &mat_projeção);

  if (!caixa_dentro_frusto(caixa_mundo, &mat_visão_projeção)) {
    return;
  }

  Material3D material_padrão = {
    .cor_ambiente     = COR_BRANCA,
    .cor_difusa       = COR_BRANCA,
    .cor_especular    = COR_PRETA,
    .brilho_especular = 0.0f,
    .textura_difusa   = nullptr
  };
  const Material3D *mat_ativo = material ? material : &material_padrão;

  for (size_t i = 0; i + 2 < malha->núm_índices; i += 3) {
    uint32_t idx0 = malha->índices[i];
    uint32_t idx1 = malha->índices[i + 1];
    uint32_t idx2 = malha->índices[i + 2];

    if (idx0 >= malha->núm_vértices || idx1 >= malha->núm_vértices || idx2 >= malha->núm_vértices) {
      continue;
    }

    const Vértice3D *orig[3] = {
      &malha->vértices[idx0],
      &malha->vértices[idx1],
      &malha->vértices[idx2]
    };

    Flut3D pos_mundo[3];
    Flut3D norm_mundo[3];
    for (size_t v = 0; v < 3; ++v) {
      pos_mundo[v]  = transformar_flut3d_matriz4x4(orig[v]->posição, &mat_modelo);
      norm_mundo[v] = transformar_normal_matriz4x4(orig[v]->normal, &mat_normal);
    }

    Flut3D aresta1 = subtrair_flut3d(pos_mundo[1], pos_mundo[0]);
    Flut3D aresta2 = subtrair_flut3d(pos_mundo[2], pos_mundo[0]);
    Flut3D normal_face = normalizar_flut3d(produto_vetorial_flut3d(aresta1, aresta2));

    Flut3D centroide = escalar_flut3d(somar_flut3d(somar_flut3d(pos_mundo[0], pos_mundo[1]), pos_mundo[2]), 1.0f / 3.0f);
    Flut3D dir_para_câmara = subtrair_flut3d(câmara->posição, centroide);

    if (modo != SOMBREAMENTO_ARAME && produto_escalar_flut3d(normal_face, dir_para_câmara) <= 0.0f) {
      continue;
    }

    Vértice3D tri_mundo[3];
    for (size_t v = 0; v < 3; ++v) {
      tri_mundo[v] = (Vértice3D){
        .posição = pos_mundo[v],
        .normal  = norm_mundo[v],
        .uv      = orig[v]->uv,
        .cor     = orig[v]->cor
      };
    }

    if (modo == SOMBREAMENTO_PLANO) {
      Cor cor_média = {
        .vermelho = (uint8_t)(((uint32_t)orig[0]->cor.vermelho + orig[1]->cor.vermelho + orig[2]->cor.vermelho) / 3),
        .verde    = (uint8_t)(((uint32_t)orig[0]->cor.verde    + orig[1]->cor.verde    + orig[2]->cor.verde)    / 3),
        .azul     = (uint8_t)(((uint32_t)orig[0]->cor.azul     + orig[1]->cor.azul     + orig[2]->cor.azul)     / 3),
        .alfa     = (uint8_t)(((uint32_t)orig[0]->cor.alfa     + orig[1]->cor.alfa     + orig[2]->cor.alfa)     / 3)
      };
      Cor cor_iluminada = calcular_iluminação_vértice(
        centroide, normal_face, cor_média, câmara->posição, mat_ativo, luzes, núm_luzes
      );
      for (size_t v = 0; v < 3; ++v) {
        tri_mundo[v].normal = normal_face;
        tri_mundo[v].cor    = cor_iluminada;
      }
    } else if (modo == SOMBREAMENTO_GOURAUD || modo == SOMBREAMENTO_PHONG) {
      for (size_t v = 0; v < 3; ++v) {
        tri_mundo[v].cor = calcular_iluminação_vértice(
          pos_mundo[v], norm_mundo[v], orig[v]->cor, câmara->posição, mat_ativo, luzes, núm_luzes
        );
      }
    }

    Vértice3D tri_vista[3];
    for (size_t v = 0; v < 3; ++v) {
      tri_vista[v]         = tri_mundo[v];
      tri_vista[v].posição = transformar_flut3d_matriz4x4(tri_mundo[v].posição, &mat_visão);
    }

    Vértice3D recortados[4];
    size_t núm_recortados = recortar_triângulo_plano_próximo(tri_vista, recortados, câmara->plano_próximo);
    if (núm_recortados < 3) {
      continue;
    }

    Vértice3D projetados[4];
    for (size_t v = 0; v < núm_recortados; ++v) {
      Flut4D homogéneo = {
        .x = recortados[v].posição.x,
        .y = recortados[v].posição.y,
        .z = recortados[v].posição.z,
        .w = 1.0f
      };
      Flut4D recorte = transformar_flut4d_matriz4x4(homogéneo, &mat_projeção);
      Flut3D ecrã    = projetar_para_ecrã(recorte, tela->largura, tela->altura);

      projetados[v] = (Vértice3D){
        .posição = ecrã,
        .normal  = recortados[v].normal,
        .uv      = recortados[v].uv,
        .cor     = recortados[v].cor
      };
    }

    for (size_t t = 1; t + 1 < núm_recortados; ++t) {
      desenhar_triângulo_3d_zbuffer(
        tela, zbuffer, projetados[0], projetados[t], projetados[t + 1], mat_ativo, modo
      );
    }
  }
}

void aplicar_nevoeiro_profundidade(
  Tela *tela,
  const BufferProfundidade *zbuffer,
  Cor cor_nevoeiro,
  float início,
  float fim
) {
  if (!tela || !tela->píxeis || !zbuffer || !zbuffer->valores) {
    return;
  }

  uint32_t largura = (tela->largura < zbuffer->largura) ? tela->largura : zbuffer->largura;
  uint32_t altura  = (tela->altura  < zbuffer->altura)  ? tela->altura  : zbuffer->altura;

  if (largura == 0 || altura == 0) {
    return;
  }

  float intervalo = fim - início;
  bool intervalo_degenerado = fabsf(intervalo) < 1e-6f;

  for (uint32_t y = 0; y < altura; ++y) {
    size_t linha_tela = (size_t)y * (size_t)tela->largura;
    size_t linha_z    = (size_t)y * (size_t)zbuffer->largura;

    for (uint32_t x = 0; x < largura; ++x) {
      float profundidade = zbuffer->valores[linha_z + x];

      if (!isfinite(profundidade) || profundidade <= início) {
        continue;
      }

      float fator;
      if (profundidade >= fim || intervalo_degenerado) {
        fator = 1.0f;
      } else {
        fator = (profundidade - início) / intervalo;
        if (fator < 0.0f) fator = 0.0f;
        if (fator > 1.0f) fator = 1.0f;
      }

      Cor original = tela->píxeis[linha_tela + x];
      Cor misturada = interpolar_cor(original, cor_nevoeiro, fator);
      misturada.alfa = original.alfa;

      tela->píxeis[linha_tela + x] = misturada;
    }
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
  criar_matriz3x3_translação(&m_trans, 10.0f, 20.0f);
  criar_matriz3x3_escala(&m_esc, 2.0f, 2.0f);
  multiplicar_matrizes3x3(&m_comb, &m_trans, &m_esc);
  assert(inverter_matriz3x3(&m_inv, &m_comb));

  Flut2D p = transformar_flut2d_matriz3x3((Flut2D){ 5.0f, 5.0f }, &m_comb);
  Flut2D p_volta = transformar_flut2d_matriz3x3(p, &m_inv);
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
  desenhar_linha_suavizada(&tela, (Flut2D){ 10, 10 }, (Flut2D){ 200, 30 }, COR_CIANO);
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
