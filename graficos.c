/* row-major, RGBA */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t Cor;

typedef struct {
  int largura;
  int altura;
  Cor* pixeis;
} Framebuffer;

typedef struct {
  int largura;
  int altura;
  Cor* dados;
} Textura;

typedef struct {
  float x, y;
} Vetor2D;

typedef struct {
  float x, y, z;
} Vetor3D;

typedef struct {
  Vetor3D posicao;
  Cor cor;
} Vertice;

constexpr float PI = 3.14159265f;

void criar_matriz_projecao(float matriz[static 16], float fov, float aspeto, float perto, float longe);
void limpar_zbuffer(float* zbuffer, int largura, int altura);
float absoluto(float x);
float truncar(float x);
float raiz_quadrada(float x);
float reciproco_raiz_quadrada(float x);
float seno(float x);
float cosseno(float x);
float tangente(float x);
float minimo(float x, float y);
float maximo(float x, float y);
float distancia(float x, float y);
float modulo(float x, float y);
float* criar_zbuffer(int largura, int altura);

Cor aplicar_intensidade(Cor cor, float intensidade);
Cor interpolar_cores(Cor c1, Cor c2, float fator);

void destruir_framebuffer(Framebuffer* fb);
void limpar_framebuffer(Framebuffer* fb, Cor cor);
void desenhar_pixel(Framebuffer* fb, int x, int y, Cor cor);
Framebuffer criar_framebuffer(int largura, int altura);

Cor amostrar_textura(const Textura* textura, float u, float v);

void desenhar_linha(Framebuffer* fb, Vetor2D p1, Vetor2D p2, Cor cor);
void rasterizar_triangulo(Framebuffer* fb, Vetor2D p1, Vetor2D p2, Vetor2D p3, Cor cor);

void criar_matriz_camara(float matriz[static 16], Vetor3D posicao, Vetor3D alvo, Vetor3D cima);
bool orientacao_traseira(Vetor3D v1, Vetor3D v2, Vetor3D v3, Vetor3D direcao_camara);
int clipar_triangulo_plano(Vetor3D plano_ponto, Vetor3D plano_normal, Vertice triangulo_in[static 3], Vertice triangulos_out[static 6]);
Cor calcular_iluminacao_difusa(Vetor3D normal, Vetor3D direcao_luz, Cor cor_objeto, Cor cor_luz);
Cor calcular_iluminacao_especular(Vetor3D normal, Vetor3D direcao_luz, Vetor3D direcao_olhar, float brilho);
float produto_escalar(Vetor3D u, Vetor3D v);
Vetor3D normalizar_vetor(Vetor3D v);
Vetor3D somar_vetores(Vetor3D u, Vetor3D v);
Vetor3D subtrair_vetores(Vetor3D u, Vetor3D v);
Vetor3D produto_vetorial(Vetor3D u, Vetor3D v);
Vetor2D projetar_ponto(Vetor3D ponto_3d, int largura, int altura);
Vetor3D transformar_vetor(Vetor3D v, float matriz[static 16]);
Vetor3D calcular_normal(Vetor3D u, Vetor3D v, Vetor3D w);

void rasterizar_triangulo_interpolado(Framebuffer* fb, Vertice u, Vertice v, Vertice w);

void criar_matriz_projecao(float matriz[static 16], float fov, float aspeto, float perto, float longe) {
    for (int i = 0; i < 16; i++) {
        matriz[i] = 0.0f;
    }
    float fov_radianos = fov * (PI / 180.0f);
    float tan_metade_fov = tangente(fov_radianos / 2.0);
    if (tan_metade_fov == 0.0f || aspeto == 0.0f || longe - perto == 0.0f) {
        return;
    }
    matriz[0] = 1.0f / (aspeto * tan_metade_fov);
    matriz[5] = 1.0f / tan_metade_fov;
    matriz[10] = -(longe + perto) / (longe - perto);
    matriz[11] = -1.0f;
    matriz[14] = -(2.0f * longe * perto) / (longe - perto);
}

void limpar_zbuffer(float* zbuffer, int largura, int altura) {
    if (zbuffer == nullptr || largura <= 0 || altura <= 0) {
        return;
    }
    int total_pixeis = largura * altura;
    for (int i = 0; i < total_pixeis; ++i) {
        zbuffer[i] = 1.0f;
    }
}

void criar_matriz_identidade(float matriz[static 16]) {
    for (int i = 0; i < 16; ++i) {
        matriz[i] = 0.0f;
    }
    matriz[0]  = 1.0f;
    matriz[5]  = 1.0f;
    matriz[10] = 1.0f;
    matriz[15] = 1.0f;
}

void criar_matriz_translacao(float matriz[static 16], float x, float y, float z) {
    criar_matriz_identidade(matriz);
    matriz[12] = x;
    matriz[13] = y;
    matriz[14] = z;
}

void criar_matriz_escala(float matriz[static 16], float sx, float sy, float sz) {
    criar_matriz_identidade(matriz);
    matriz[0]  = sx;
    matriz[5]  = sy;
    matriz[10] = sz;
}

void criar_matriz_rotacao(float matriz[static 16], float angulo, float x, float y, float z) {
    criar_matriz_identidade(matriz);
    float magnitude = raiz_quadrada(x * x + y * y + z * z);
    if (magnitude == 0.0f) return;
    float ux = x / magnitude;
    float uy = y / magnitude;
    float uz = z / magnitude;
    float c = cosseno(angulo);
    float s = seno(angulo);
    float omc = 1.0f - c;
    matriz[0] = c + ux * ux * omc;
    matriz[1] = uy * ux * omc + uz * s;
    matriz[2] = uz * ux * omc - uy * s;
    matriz[3] = 0.0f;
    matriz[4] = ux * uy * omc - uz * s;
    matriz[5] = c + uy * uy * omc;
    matriz[6] = uz * uy * omc + ux * s;
    matriz[7] = 0.0f;
    matriz[8]  = ux * uz * omc + uy * s;
    matriz[9]  = uy * uz * omc - ux * s;
    matriz[10] = c + uz * uz * omc;
    matriz[11] = 0.0f;
    matriz[12] = 0.0f;
    matriz[13] = 0.0f;
    matriz[14] = 0.0f;
    matriz[15] = 1.0f;
}

void multiplicar_matrizes(float resultado[static 16], const float a[static 16], const float b[static 16]) {
    float temporario[16];
    for (int coluna = 0; coluna < 4; ++coluna) {
        for (int linha = 0; linha < 4; ++linha) {
            float soma = 0.0f;
            for (int k = 0; k < 4; ++k) {
                soma += a[k * 4 + linha] * b[coluna * 4 + k];
            }
            temporario[coluna * 4 + linha] = soma;
        }
    }
    for (size_t i = 0; i < 16; ++i) {
        resultado[i] = temporario[i];
    }
}

float absoluto(float x) {
    return x >= 0.0f ? x : -x;
}

float truncar(float x) {
    if (x >= 9223371487098961920.0f || x <= -9223371487098961920.0f) {
        return x;
    }
    return (long long)x;
}

float chao(float x) {
    int y = truncar(x);
    return x < 0.0f && x != y ? y - 1 : y;
}

float raiz_quadrada(float x) {
    if (x <= 0.0f) {
        return 0.0f;
    }
    float anterior;
    float raiz = x;
    do {
        anterior = raiz;
        raiz = 0.5f * (raiz + x / raiz);
    } while (distancia(raiz, anterior) >= 1e-6f);
    return raiz;
}

float reciproco_raiz_quadrada(float x) {
    if (x <= 0.0f) return 0.0f;
    float metade = 0.5f * x;
    union {
        float f;
        uint32_t i;
    } u;
    u.f = x;
    u.i = 0x5f3759df - (u.i >> 1);
    u.f = u.f * (1.5f - metade * u.f * u.f);
    u.f = u.f * (1.5f - metade * u.f * u.f);
    return u.f;
}

float seno(float x) {
    x = modulo(x, 2.0f * PI);
    if (x > PI)  x -= 2.0f * PI;
    if (x < -PI) x += 2.0f * PI;
    float termo = x;
    float soma = x;
    float x_quadrado = x * x;
    for (int i = 1; i <= 6; i++) {
        termo *= -x_quadrado / ((2 * i) * (2 * i + 1));
        soma += termo;
    }
    return soma;
}

float cosseno(float x) {
    x = modulo(x, 2.0f * PI);
    if (x > PI)  x -= 2.0f * PI;
    if (x < -PI) x += 2.0f * PI;
    float termo = 1.0f;
    float soma = 1.0f;
    float x_quadrado = x * x;
    for (int i = 1; i <= 6; i++) {
        termo *= -x_quadrado / ((2 * i - 1) * (2 * i));
        soma += termo;
    }
    return soma;
}

float tangente(float x) {
    return seno(x) / cosseno(x);
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

float modulo(float x, float y) {
    if (y == 0.0f) {
        return 0.0f;
    }
    return x - truncar(x / y) * y;
}

[[nodiscard]] float* criar_zbuffer(int largura, int altura) {
    if (largura <= 0 || altura <= 0) {
        fprintf(stderr, "Erro: Dimensões do Z-buffer inválidas (%dx%d).\n", largura, altura);
        return nullptr;
    }
    float* zbuffer = calloc(largura * altura, sizeof(float));
    if (zbuffer == nullptr) {
        fprintf(stderr, "Erro: Falha na alocação de memória para o Z-buffer.\n");
        return nullptr;
    }
    limpar_zbuffer(zbuffer, largura, altura);
    return zbuffer;
}



Cor aplicar_intensidade(Cor cor, float intensidade) {
    if (intensidade < 0.0f) {
        intensidade = 0.0f;
    }
    if (intensidade > 1.0f) {
        intensidade = 1.0f;
    }
    uint8_t a = (cor >> 24) & 0xFF;
    uint8_t r = (cor >> 16) & 0xFF;
    uint8_t g = (cor >>  8) & 0xFF;
    uint8_t b =  cor        & 0xFF;
    float res_r = r * intensidade;
    float res_g = g * intensidade;
    float res_b = b * intensidade;
    return ((Cor)a     << 24) |
           ((Cor)res_r << 16) |
           ((Cor)res_g <<  8) |
            (Cor)res_b;
}

Cor interpolar_cores(Cor c1, Cor c2, float fator) {
    if (fator < 0.0f) fator = 0.0f;
    if (fator > 1.0f) fator = 1.0f;
    uint8_t c1_b1 = (c1 >> 24) & 0xFF;
    uint8_t c1_b2 = (c1 >> 16) & 0xFF;
    uint8_t c1_b3 = (c1 >> 8)  & 0xFF;
    uint8_t c1_b4 =  c1        & 0xFF;
    uint8_t c2_b1 = (c2 >> 24) & 0xFF;
    uint8_t c2_b2 = (c2 >> 16) & 0xFF;
    uint8_t c2_b3 = (c2 >> 8)  & 0xFF;
    uint8_t c2_b4 =  c2        & 0xFF;
    uint8_t r_b1 = (uint8_t)(c1_b1 + fator * ((float)c2_b1 - (float)c1_b1));
    uint8_t r_b2 = (uint8_t)(c1_b2 + fator * ((float)c2_b2 - (float)c1_b2));
    uint8_t r_b3 = (uint8_t)(c1_b3 + fator * ((float)c2_b3 - (float)c1_b3));
    uint8_t r_b4 = (uint8_t)(c1_b4 + fator * ((float)c2_b4 - (float)c1_b4));
    return ((Cor)r_b1 << 24) |
           ((Cor)r_b2 << 16) |
           ((Cor)r_b3 << 8)  |
            (Cor)r_b4;
}



void destruir_framebuffer(Framebuffer* fb) {

  if (fb == nullptr) {
    return;
  }


  free(fb->pixeis);


  fb->pixeis = nullptr;
  fb->largura = 0;
  fb->altura = 0;
}

void limpar_framebuffer(Framebuffer* fb, Cor cor) {

  if (fb == nullptr || fb->pixeis == nullptr) {
    return;
  }


  int area = fb->largura * fb->altura;


  for (int i = 0; i < area; ++i) {
    fb->pixeis[i] = cor;
  }
}

void desenhar_pixel(Framebuffer* fb, int x, int y, Cor cor) {

  if (fb == nullptr || fb->pixeis == nullptr) {
    return;
  }


  if (x < 0 || x >= fb->largura || y < 0 || y >= fb->altura) {
    return;
  }


  fb->pixeis[x + y * fb->largura] = cor;
}

[[nodiscard]] Framebuffer criar_framebuffer(int largura, int altura) {
  Framebuffer fb = {};
  if (largura <= 0 || altura <= 0) {
    return fb;
  }
  Cor* memoria_pixeis = malloc(largura * altura * sizeof(Cor));
  if (memoria_pixeis == nullptr) {
    return fb;
  }
  fb.pixeis = memoria_pixeis;
  fb.largura = largura;
  fb.altura = altura;
  return fb;
}

[[nodiscard]] Cor amostrar_textura(const Textura* textura, float u, float v) {

  if (textura == nullptr || textura->dados == nullptr || textura->largura <= 0 || textura->altura <= 0) {
    return 0;
  }



  float u_repetido = u - chao(u);
  float v_repetido = v - chao(v);


  int x = (int)(u_repetido * (float)textura->largura);
  int y = (int)(v_repetido * (float)textura->altura);


  if (x >= textura->largura) x = textura->largura - 1;
  if (y >= textura->altura)  y =  textura->altura - 1;
  if (x < 0) x = 0;
  if (y < 0) y = 0;


  return textura->dados[x + y * textura->largura];
}

void desenhar_linha(Framebuffer* fb, Vetor2D p1, Vetor2D p2, Cor cor) {
    if (fb == nullptr || fb->pixeis == nullptr || fb->largura <= 0 || fb->altura <= 0) {
        return;
    }
    int x0 = p1.x;
    int y0 = p1.y;
    int x1 = p2.x;
    int y1 = p2.y;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int erro = dx - dy;
    while (true) {
        if (x0 >= 0 && x0 < fb->largura && y0 >= 0 && y0 < fb->altura) {
            fb->pixeis[y0 * fb->largura + x0] = cor;
        }
        if (x0 == x1 && y0 == y1) {
            break;
        }
        int erro2 = 2 * erro;
        if (erro2 > -dy) {
            erro -= dy;
            x0 += sx;
        }
        if (erro2 < dx) {
            erro += dx;
            y0 += sy;
        }
    }
}

void rasterizar_triangulo(Framebuffer* fb, Vetor2D p1, Vetor2D p2, Vetor2D p3, Cor cor) {
  if (fb == nullptr || fb->pixeis == nullptr) {
    return;
  }
  int min_x = minimo(minimo(p1.x, p2.x), p3.x);
  int max_x = maximo(maximo(p1.x, p2.x), p3.x);
  int min_y = minimo(minimo(p1.y, p2.y), p3.y);
  int max_y = maximo(maximo(p1.y, p2.y), p3.y);
  if (min_x < 0) min_x = 0;
  if (min_y < 0) min_y = 0;
  if (max_x >= fb->largura) max_x = fb->largura - 1;
  if (max_y >= fb->altura) max_y = fb->altura - 1;
  float dy12 = p2.y - p1.y; float dx12 = p2.x - p1.x;
  float dy23 = p3.y - p2.y; float dx23 = p3.x - p2.x;
  float dy31 = p1.y - p3.y; float dx31 = p1.x - p3.x;
  float area_dupla = dx12 * dy23 - dy12 * dx23;
  if (area_dupla == 0.0f) {
    return;
  }
  float sinal = (area_dupla < 0.0f) ? -1.0f : 1.0f;
  for (int y = min_y; y <= max_y; ++y) {
    for (int x = min_x; x <= max_x; ++x) {
      float px = (float)x + 0.5f;
      float py = (float)y + 0.5f;
      float w0 = ((px - p2.x) * dy23 - (py - p2.y) * dx23) * sinal;
      float w1 = ((px - p3.x) * dy31 - (py - p3.y) * dx31) * sinal;
      float w2 = ((px - p1.x) * dy12 - (py - p1.y) * dx12) * sinal;
      if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) {
        desenhar_pixel(fb, x, y, cor);
      }
    }
  }
}

void criar_matriz_camara(float matriz[static 16], Vetor3D posicao, Vetor3D alvo, Vetor3D cima) {
    float zx = posicao.x - alvo.x;
    float zy = posicao.y - alvo.y;
    float zz = posicao.z - alvo.z;
    float magnitude_z = raiz_quadrada(zx * zx + zy * zy + zz * zz);
    if (magnitude_z > 0.0f) {
        zx /= magnitude_z;
        zy /= magnitude_z;
        zz /= magnitude_z;
    }
    float xx = cima.y * zz - cima.z * zy;
    float xy = cima.z * zx - cima.x * zz;
    float xz = cima.x * zy - cima.y * zx;
    float magnitude_x = raiz_quadrada(xx * xx + xy * xy + xz * xz);
    if (magnitude_x > 0.0f) {
        xx /= magnitude_x;
        xy /= magnitude_x;
        xz /= magnitude_x;
    }
    float yx = zy * xz - zz * xy;
    float yy = zz * xx - zx * xz;
    float yz = zx * xy - zy * xx;
    matriz[0] = xx;
    matriz[1] = yx;
    matriz[2] = zx;
    matriz[3] = 0.0f;
    matriz[4] = xy;
    matriz[5] = yy;
    matriz[6] = zy;
    matriz[7] = 0.0f;
    matriz[8] = xz;
    matriz[9] = yz;
    matriz[10] = zz;
    matriz[11] = 0.0f;
    matriz[12] = -(xx * posicao.x + xy * posicao.y + xz * posicao.z);
    matriz[13] = -(yx * posicao.x + yy * posicao.y + yz * posicao.z);
    matriz[14] = -(zx * posicao.x + zy * posicao.y + zz * posicao.z);
    matriz[15] = 1.0f;
}

bool orientacao_traseira(Vetor3D v1, Vetor3D v2, Vetor3D v3, Vetor3D direcao_camara) {
    Vetor3D aresta1 = { v2.x - v1.x, v2.y - v1.y, v2.z - v1.z };
    Vetor3D aresta2 = { v3.x - v1.x, v3.y - v1.y, v3.z - v1.z };
    Vetor3D normal_face = {
        aresta1.y * aresta2.z - aresta1.z * aresta2.y,
        aresta1.z * aresta2.x - aresta1.x * aresta2.z,
        aresta1.x * aresta2.y - aresta1.y * aresta2.x
    };
    float produto_escalar = normal_face.x * direcao_camara.x +
                            normal_face.y * direcao_camara.y +
                            normal_face.z * direcao_camara.z;
    return produto_escalar >= 0.0f;
}

int clipar_triangulo_plano(Vetor3D plano_ponto, Vetor3D plano_normal, Vertice triangulo_in[static 3], Vertice triangulos_out[static 6]) {
    Vertice dentro[3];
    Vertice fora[3];
    int contagem_dentro = 0;
    int contagem_fora = 0;
    for (int i = 0; i < 3; i++) {
        Vetor3D v = triangulo_in[i].posicao;
        Vetor3D dir = { v.x - plano_ponto.x, v.y - plano_ponto.y, v.z - plano_ponto.z };
        float distancia = (dir.x * plano_normal.x) + (dir.y * plano_normal.y) + (dir.z * plano_normal.z);
        if (distancia >= 0.0f) {
            dentro[contagem_dentro++] = triangulo_in[i];
        } else {
            fora[contagem_fora++] = triangulo_in[i];
        }
    }
    if (contagem_dentro == 0) {
        return 0;
    }
    if (contagem_dentro == 3) {
        triangulos_out[0] = triangulo_in[0];
        triangulos_out[1] = triangulo_in[1];
        triangulos_out[2] = triangulo_in[2];
        return 1;
    }
    if (contagem_dentro == 1) {
        triangulos_out[0] = dentro[0];
        Vetor3D d1 = dentro[0].posicao;
        Vetor3D f1 = fora[0].posicao;
        float dist_d1 = ((d1.x - plano_ponto.x) * plano_normal.x) + ((d1.y - plano_ponto.y) * plano_normal.y) + ((d1.z - plano_ponto.z) * plano_normal.z);
        float dist_f1 = ((f1.x - plano_ponto.x) * plano_normal.x) + ((f1.y - plano_ponto.y) * plano_normal.y) + ((f1.z - plano_ponto.z) * plano_normal.z);
        float t1 = dist_d1 / (dist_d1 - dist_f1);
        triangulos_out[1].posicao.x = d1.x + t1 * (f1.x - d1.x);
        triangulos_out[1].posicao.y = d1.y + t1 * (f1.y - d1.y);
        triangulos_out[1].posicao.z = d1.z + t1 * (f1.z - d1.z);
        triangulos_out[1].cor = interpolar_cores(dentro[0].cor, fora[0].cor, t1);
        Vetor3D f2 = fora[1].posicao;
        float dist_f2 = ((f2.x - plano_ponto.x) * plano_normal.x) + ((f2.y - plano_ponto.y) * plano_normal.y) + ((f2.z - plano_ponto.z) * plano_normal.z);
        float t2 = dist_d1 / (dist_d1 - dist_f2);
        triangulos_out[2].posicao.x = d1.x + t2 * (f2.x - d1.x);
        triangulos_out[2].posicao.y = d1.y + t2 * (f2.y - d1.y);
        triangulos_out[2].posicao.z = d1.z + t2 * (f2.z - d1.z);
        triangulos_out[2].cor = interpolar_cores(dentro[0].cor, fora[1].cor, t2);
        return 1;
    }
    if (contagem_dentro == 2) {
        triangulos_out[0] = dentro[0];
        triangulos_out[1] = dentro[1];
        Vetor3D d1 = dentro[0].posicao;
        Vetor3D f1 = fora[0].posicao;
        float dist_d1 = ((d1.x - plano_ponto.x) * plano_normal.x) + ((d1.y - plano_ponto.y) * plano_normal.y) + ((d1.z - plano_ponto.z) * plano_normal.z);
        float dist_f1 = ((f1.x - plano_ponto.x) * plano_normal.x) + ((f1.y - plano_ponto.y) * plano_normal.y) + ((f1.z - plano_ponto.z) * plano_normal.z);
        float t1 = dist_d1 / (dist_d1 - dist_f1);
        triangulos_out[2].posicao.x = d1.x + t1 * (f1.x - d1.x);
        triangulos_out[2].posicao.y = d1.y + t1 * (f1.y - d1.y);
        triangulos_out[2].posicao.z = d1.z + t1 * (f1.z - d1.z);
        triangulos_out[2].cor = interpolar_cores(dentro[0].cor, fora[0].cor, t1);
        triangulos_out[3] = dentro[1];
        triangulos_out[4] = triangulos_out[2];
        Vetor3D d2 = dentro[1].posicao;
        float dist_d2 = ((d2.x - plano_ponto.x) * plano_normal.x) + ((d2.y - plano_ponto.y) * plano_normal.y) + ((d2.z - plano_ponto.z) * plano_normal.z);
        float t2 = dist_d2 / (dist_d2 - dist_f1);
        triangulos_out[5].posicao.x = d2.x + t2 * (f1.x - d2.x);
        triangulos_out[5].posicao.y = d2.y + t2 * (f1.y - d2.y);
        triangulos_out[5].posicao.z = d2.z + t2 * (f1.z - d2.z);
        triangulos_out[5].cor = interpolar_cores(dentro[1].cor, fora[0].cor, t2);
        return 2;
    }
    return 0;
}

Cor calcular_iluminacao_difusa(Vetor3D normal, Vetor3D direcao_luz, Cor cor_objeto, Cor cor_luz) {
    float tam_normal_qd = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
    float rec_tam_normal = reciproco_raiz_quadrada(tam_normal_qd);
    Vetor3D n = {
        normal.x * rec_tam_normal,
        normal.y * rec_tam_normal,
        normal.z * rec_tam_normal
    };
    float tam_luz_qd = direcao_luz.x * direcao_luz.x + direcao_luz.y * direcao_luz.y + direcao_luz.z * direcao_luz.z;
    float rec_tam_luz = reciproco_raiz_quadrada(tam_luz_qd);
    Vetor3D l = {
        direcao_luz.x * rec_tam_luz,
        direcao_luz.y * rec_tam_luz,
        direcao_luz.z * rec_tam_luz
    };
    float cosseno = n.x * l.x + n.y * l.y + n.z * l.z;
    if (cosseno < 0.0f) {
        cosseno = 0.0f;
    }
    uint8_t obj_a = (cor_objeto >> 24) & 0xFF;
    uint8_t obj_r = (cor_objeto >> 16) & 0xFF;
    uint8_t obj_g = (cor_objeto >> 8 ) & 0xFF;
    uint8_t obj_b =  cor_objeto        & 0xFF;
    uint8_t luz_a = (cor_luz >> 24) & 0xFF;
    uint8_t luz_r = (cor_luz >> 16) & 0xFF;
    uint8_t luz_g = (cor_luz >> 8 ) & 0xFF;
    uint8_t luz_b =  cor_luz        & 0xFF;
    float res_r = ((float)obj_r / 255.0f) * ((float)luz_r / 255.0f) * cosseno * 255.0f;
    float res_g = ((float)obj_g / 255.0f) * ((float)luz_g / 255.0f) * cosseno * 255.0f;
    float res_b = ((float)obj_b / 255.0f) * ((float)luz_b / 255.0f) * cosseno * 255.0f;
    float res_a = ((float)obj_a / 255.0f) * ((float)luz_a / 255.0f) * 255.0f;
    if (res_r > 255.0f) res_r = 255.0f;
    if (res_g > 255.0f) res_g = 255.0f;
    if (res_b > 255.0f) res_b = 255.0f;
    if (res_a > 255.0f) res_a = 255.0f;
    return ((Cor)(uint8_t)res_a << 24) |
           ((Cor)(uint8_t)res_r << 16) |
           ((Cor)(uint8_t)res_g << 8)  |
            (Cor)(uint8_t)res_b;
}

static inline float potencia_brilho(float base, float expoente) {
    if (base <= 0.0f) return 0.0f;
    if (expoente <= 0.0f) return 1.0f;
    int exp_int = expoente;
    float resultado = 1.0f;
    float atual = base;
    while (exp_int > 0) {
        if (exp_int % 2 == 1) {
            resultado *= atual;
        }
        atual *= atual;
        exp_int /= 2;
    }
    return resultado;
}

Cor calcular_iluminacao_especular(Vetor3D normal, Vetor3D direcao_luz, Vetor3D direcao_olhar, float brilho) {
    float tam_normal_qd = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
    float inv_tam_normal = reciproco_raiz_quadrada(tam_normal_qd);
    Vetor3D n = { normal.x * inv_tam_normal, normal.y * inv_tam_normal, normal.z * inv_tam_normal };
    float tam_luz_qd = direcao_luz.x * direcao_luz.x + direcao_luz.y * direcao_luz.y + direcao_luz.z * direcao_luz.z;
    float inv_tam_luz = reciproco_raiz_quadrada(tam_luz_qd);
    Vetor3D l = { direcao_luz.x * inv_tam_luz, direcao_luz.y * inv_tam_luz, direcao_luz.z * inv_tam_luz };
    float tam_olhar_qd = direcao_olhar.x * direcao_olhar.x + direcao_olhar.y * direcao_olhar.y + direcao_olhar.z * direcao_olhar.z;
    float inv_tam_olhar = reciproco_raiz_quadrada(tam_olhar_qd);
    Vetor3D v = { direcao_olhar.x * inv_tam_olhar, direcao_olhar.y * inv_tam_olhar, direcao_olhar.z * inv_tam_olhar };
    Vetor3D h_sub = { l.x + v.x, l.y + v.y, l.z + v.z };
    float tam_h_sq = h_sub.x * h_sub.x + h_sub.y * h_sub.y + h_sub.z * h_sub.z;
    float inv_tam_h = reciproco_raiz_quadrada(tam_h_sq);
    Vetor3D h = { h_sub.x * inv_tam_h, h_sub.y * inv_tam_h, h_sub.z * inv_tam_h };
    float dot_nh = n.x * h.x + n.y * h.y + n.z * h.z;
    if (dot_nh < 0.0f) {
        dot_nh = 0.0f;
    }
    float dot_nl = n.x * l.x + n.y * l.y + n.z * l.z;
    if (dot_nl <= 0.0f) {
        dot_nh = 0.0f;
    }
    float intensidade = potencia_brilho(dot_nh, brilho);
    float res_cor = intensidade * 255.0f;
    if (res_cor > 255.0f) res_cor = 255.0f;
    uint8_t especular = res_cor;
    return ((Cor)255       << 24) |
           ((Cor)especular << 16) |
           ((Cor)especular << 8 ) |
            (Cor)especular;
}

float produto_escalar(Vetor3D u, Vetor3D v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

Vetor3D normalizar_vetor(Vetor3D v) {
    float comprimento = raiz_quadrada((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    if (comprimento == 0.0f) {
        return (Vetor3D){0.0f, 0.0f, 0.0f};
    }
    return (Vetor3D){
        .x = v.x / comprimento,
        .y = v.y / comprimento,
        .z = v.z / comprimento
    };
}

Vetor3D somar_vetores(Vetor3D a, Vetor3D b) {
    return (Vetor3D){
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z
    };
}

Vetor3D subtrair_vetores(Vetor3D a, Vetor3D b) {
    return (Vetor3D){
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z
    };
}

Vetor3D produto_vetorial(Vetor3D a, Vetor3D b) {
    return (Vetor3D){
        .x = (a.y * b.z) - (a.z * b.y),
        .y = (a.z * b.x) - (a.x * b.z),
        .z = (a.x * b.y) - (a.y * b.x)
    };
}

[[nodiscard]] Vetor2D projetar_ponto(Vetor3D ponto_3d, int largura, int altura) {


  if (ponto_3d.z <= 0.1f) {
    return (Vetor2D){ .x = 0.0f, .y = 0.0f };
  }



  float fov_escala = (float)largura * 0.5f;


  float x_projetado = (ponto_3d.x * fov_escala) / ponto_3d.z;
  float y_projetado = (ponto_3d.y * fov_escala) / ponto_3d.z;




  float x_ecra = x_projetado + ((float)largura * 0.5f);
  float y_ecra = -y_projetado + ((float)altura * 0.5f);

  return (Vetor2D){ .x = x_ecra, .y = y_ecra };
}

[[nodiscard]] Vetor3D transformar_vetor(Vetor3D v, float matriz[static 16]) {

  float x = matriz[0] * v.x + matriz[1] * v.y + matriz[2] * v.z + matriz[3];
  float y = matriz[4] * v.x + matriz[5] * v.y + matriz[6] * v.z + matriz[7];
  float z = matriz[8] * v.x + matriz[9] * v.y + matriz[10] * v.z + matriz[11];
  float w = matriz[12] * v.x + matriz[13] * v.y + matriz[14] * v.z + matriz[15];



  if (w != 0.0f) {
    float inv_w = 1.0f / w;
    x *= inv_w;
    y *= inv_w;
    z *= inv_w;
  }

  return (Vetor3D){ .x = x, .y = y, .z = z };
}

[[nodiscard]] Vetor3D calcular_normal(Vetor3D v1, Vetor3D v2, Vetor3D v3) {
    Vetor3D aresta1 = { .x = v2.x - v1.x, .y = v2.y - v1.y, .z = v2.z - v1.z };
    Vetor3D aresta2 = { .x = v3.x - v1.x, .y = v3.y - v1.y, .z = v3.z - v1.z };
    Vetor3D normal = {
        .x = (aresta1.y * aresta2.z) - (aresta1.z * aresta2.y),
        .y = (aresta1.z * aresta2.x) - (aresta1.x * aresta2.z),
        .z = (aresta1.x * aresta2.y) - (aresta1.y * aresta2.x)
    };
    float magnitude = raiz_quadrada((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));
    if (magnitude > 0.000001f) {
        float inverso_magnitude = 1.0f / magnitude;
        normal.x *= inverso_magnitude;
        normal.y *= inverso_magnitude;
        normal.z *= inverso_magnitude;
    } else {
        normal = (Vetor3D){ .x = 0.0f, .y = 0.0f, .z = 0.0f };
    }
    return normal;
}

void rasterizar_triangulo_interpolado(Framebuffer* fb, Vertice v1, Vertice v2, Vertice v3) {
  if (fb == nullptr || fb->pixeis == nullptr) {
    return;
  }
  float x1 = v1.posicao.x; float y1 = v1.posicao.y;
  float x2 = v2.posicao.x; float y2 = v2.posicao.y;
  float x3 = v3.posicao.x; float y3 = v3.posicao.y;
  int min_x = minimo(minimo(x1, x2), x3);
  int max_x = maximo(maximo(x1, x2), x3);
  int min_y = minimo(minimo(y1, y2), y3);
  int max_y = maximo(maximo(y1, y2), y3);
  if (min_x < 0) min_x = 0;
  if (min_y < 0) min_y = 0;
  if (max_x >= fb->largura) max_x = fb->largura - 1;
  if (max_y >= fb->altura ) max_y = fb->altura  - 1;
  float det = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
  if (det == 0.0f) {
    return;
  }
  float inv_det = 1.0f / det;
  float r1 = (float)((v1.cor >> 24) & 0xFF);
  float g1 = (float)((v1.cor >> 16) & 0xFF);
  float b1 = (float)((v1.cor >> 8)  & 0xFF);
  float a1 = (float)(v1.cor         & 0xFF);
  float r2 = (float)((v2.cor >> 24) & 0xFF);
  float g2 = (float)((v2.cor >> 16) & 0xFF);
  float b2 = (float)((v2.cor >> 8)  & 0xFF);
  float a2 = (float)(v2.cor         & 0xFF);
  float r3 = (float)((v3.cor >> 24) & 0xFF);
  float g3 = (float)((v3.cor >> 16) & 0xFF);
  float b3 = (float)((v3.cor >> 8)  & 0xFF);
  float a3 = (float)(v3.cor         & 0xFF);
  for (int y = min_y; y <= max_y; ++y) {
    for (int x = min_x; x <= max_x; ++x) {
      float px = (float)x + 0.5f;
      float py = (float)y + 0.5f;
      float area1 = (x3 - x2) * (py - y2) - (y3 - y2) * (px - x2);
      float area2 = (x1 - x3) * (py - y3) - (y1 - y3) * (px - x3);
      float w1 = area1 * inv_det;
      float w2 = area2 * inv_det;
      float w3 = 1.0f - w1 - w2;
      if ((w1 >= 0.0f && w2 >= 0.0f && w3 >= 0.0f) || (w1 <= 0.0f && w2 <= 0.0f && w3 <= 0.0f)) {
        float r_interp = w1 * r1 + w2 * r2 + w3 * r3;
        float g_interp = w1 * g1 + w2 * g2 + w3 * g3;
        float b_interp = w1 * b1 + w2 * b2 + w3 * b3;
        float a_interp = w1 * a1 + w2 * a2 + w3 * a3;
        uint32_t r = minimo(maximo(r_interp, 0.0f), 255.0f);
        uint32_t g = minimo(maximo(g_interp, 0.0f), 255.0f);
        uint32_t b = minimo(maximo(b_interp, 0.0f), 255.0f);
        uint32_t a = minimo(maximo(a_interp, 0.0f), 255.0f);
        Cor cor_final = (r << 24) | (g << 16) | (b << 8) | a;
        desenhar_pixel(fb, x, y, cor_final);
      }
    }
  }
}



int main() {
  return 0;
}
