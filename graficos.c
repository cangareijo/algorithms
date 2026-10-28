#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t Cor;

typedef struct {
  int largura;
  int altura;
  Cor* pixeis;
} Tela;

typedef struct {
  float x, y;
} Vetor2D;

typedef struct {
  float x, y, z;
} Vetor3D;

typedef struct {
  int largura;
  int altura;
  float* profundidades;
} Profundidade;

constexpr float PI = 3.14159265f;

void destruir_tela(Tela* tela);
void limpar_tela(Tela* tela, Cor cor);
void desenhar_pixel(Tela* tela, int x, int y, Cor cor);
Tela criar_tela(int largura, int altura);

void criar_matriz_identidade(float matriz[4][4]);
void criar_matriz_translacao(float matriz[4][4], float x, float y, float z);
void criar_matriz_escala(float matriz[4][4], float sx, float sy, float sz);
void criar_matriz_projecao(float matriz[4][4], float fov, float aspeto, float perto, float longe);
void multiplicar_matrizes(float resultado[4][4], float a[4][4], float b[4][4]);

Cor multiplicar_cor(Cor cor, float intensidade);
Cor interpolar_cores(Cor c1, Cor c2, float fator);
Cor amostrar_textura(Tela* textura, float u, float v);

float absoluto(float x);
float truncar(float x);
float chao(float x);
float raiz(float x);
float reciproco_raiz(float x);
float seno(float x);
float cosseno(float x);
float tangente(float x);
float minimo(float x, float y);
float maximo(float x, float y);
float distancia(float x, float y);
float modulo(float x, float y);

void desenhar_linha(Tela* fb, Vetor2D p1, Vetor2D p2, Cor cor);
void desenhar_triangulo(Tela* fb, Vetor2D p1, Vetor2D p2, Vetor2D p3, Cor cor);

void criar_matriz_rotacao(float matriz[4][4], float radianos, Vetor3D eixo);
void criar_matriz_camara(float matriz[4][4], Vetor3D posicao, Vetor3D alvo, Vetor3D cima);
bool orientacao_traseira(Vetor3D v1, Vetor3D v2, Vetor3D v3, Vetor3D direcao_camara);
Cor calcular_iluminacao_difusa(Vetor3D normal, Vetor3D direcao_luz, Cor cor_objeto, Cor cor_luz);
Cor calcular_iluminacao_especular(Vetor3D normal, Vetor3D direcao_luz, Vetor3D direcao_olhar, float brilho);
float produto_escalar(Vetor3D u, Vetor3D v);
Vetor2D projetar_ponto(Vetor3D ponto_3d, int largura, int altura);
Vetor3D normalizar_vetor(Vetor3D v);
Vetor3D somar_vetores(Vetor3D u, Vetor3D v);
Vetor3D subtrair_vetores(Vetor3D u, Vetor3D v);
Vetor3D produto_vetorial(Vetor3D u, Vetor3D v);
Vetor3D transformar_vetor(Vetor3D v, float matriz[4][4]);
Vetor3D calcular_normal(Vetor3D u, Vetor3D v, Vetor3D w);



void destruir_tela(Tela* tela) {
  if (tela == nullptr) {
    return;
  }
  free(tela->pixeis);
  tela->largura = 0;
  tela->altura = 0;
  tela->pixeis = nullptr;
}

void limpar_tela(Tela* tela, Cor cor) {
  if (tela == nullptr || tela->pixeis == nullptr) {
    return;
  }
  int area = tela->largura * tela->altura;
  for (int i = 0; i < area; ++i) {
    tela->pixeis[i] = cor;
  }
}

void desenhar_pixel(Tela* tela, int x, int y, Cor cor) {
  if (tela == nullptr || tela->pixeis == nullptr) {
    return;
  }
  if (x < 0 || x >= tela->largura || y < 0 || y >= tela->altura) {
    return;
  }
  tela->pixeis[x + y * tela->largura] = cor;
}

Tela criar_tela(int largura, int altura) {
  Tela tela = {};
  if (largura <= 0 || altura <= 0) {
    return tela;
  }
  Cor* memoria_pixeis = malloc(largura * altura * sizeof(Cor));
  if (memoria_pixeis == nullptr) {
    return tela;
  }
  tela.largura = largura;
  tela.altura = altura;
  tela.pixeis = memoria_pixeis;
  return tela;
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

void criar_matriz_projecao(float matriz[4][4], float fov, float aspeto, float perto, float longe) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matriz[i][j] = 0.0f;
        }
    }
    float metade_fov = tangente(fov / 2.0f);
    if (metade_fov == 0.0f || aspeto == 0.0f || (longe - perto) == 0.0f) {
        return;
    }
    matriz[0][0] = 1.0f / (aspeto * metade_fov);
    matriz[1][1] = 1.0f / metade_fov;
    matriz[2][2] = -(longe + perto) / (longe - perto);
    matriz[2][3] = -1.0f;
    matriz[3][2] = -(2.0f * longe * perto) / (longe - perto);
}

void multiplicar_matrizes(float resultado[4][4], float a[4][4], float b[4][4]) {
    float temporario[4][4];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            float soma = 0.0f;
            for (int k = 0; k < 4; ++k) {
                soma += a[i][k] * b[k][j];
            }
            temporario[i][j] = soma;
        }
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            resultado[i][j] = temporario[i][j];
        }
    }
}



Cor multiplicar_cor(Cor cor, float intensidade) {
    if (intensidade < 0.0f) {
        intensidade = 0.0f;
    }
    if (intensidade > 1.0f) {
        intensidade = 1.0f;
    }
    uint8_t r = (cor >> 24) & 0xFF;
    uint8_t g = (cor >> 16) & 0xFF;
    uint8_t b = (cor >>  8) & 0xFF;
    uint8_t a =  cor        & 0xFF;
    uint32_t res_r = r * intensidade + 0.5f;
    uint32_t res_g = g * intensidade + 0.5f;
    uint32_t res_b = b * intensidade + 0.5f;
    return (res_r << 24) |
           (res_g << 16) |
           (res_b <<  8) |
           (uint32_t)a;
}

Cor interpolar_cores(Cor c1, Cor c2, float fator) {
    if (fator < 0.0f) {
        fator = 0.0f;
    }
    if (fator > 1.0f) {
        fator = 1.0f;
    }
    uint8_t r1 = (c1 >> 24) & 0xFF;
    uint8_t g1 = (c1 >> 16) & 0xFF;
    uint8_t b1 = (c1 >>  8) & 0xFF;
    uint8_t a1 =  c1        & 0xFF;
    uint8_t r2 = (c2 >> 24) & 0xFF;
    uint8_t g2 = (c2 >> 16) & 0xFF;
    uint8_t b2 = (c2 >>  8) & 0xFF;
    uint8_t a2 =  c2        & 0xFF;
    uint32_t r_final = r1 + fator * (r2 - r1) + 0.5f;
    uint32_t g_final = g1 + fator * (g2 - g1) + 0.5f;
    uint32_t b_final = b1 + fator * (b2 - b1) + 0.5f;
    uint32_t a_final = a1 + fator * (a2 - a1) + 0.5f;
    return (r_final << 24) |
           (g_final << 16) |
           (b_final <<  8) |
            a_final;
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
  if (y >= textura->altura)  y =  textura->altura - 1;
  if (x < 0) x = 0;
  if (y < 0) y = 0;
  return textura->pixeis[x + y * textura->largura];
}



float absoluto(float x) {
    return x >= 0.0f ? x : -x;
}

float truncar(float x) {
    if (x >= 16777216.0f || x <= -16777216.0f) {
        return x;
    }
    return (long long)x;
}

float chao(float x) {
    int y = truncar(x);
    return x < 0.0f && x != y ? y - 1 : y;
}

float raiz(float x) {
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

float seno(float x) {
    x = modulo(x, 2.0f * PI);
    if (x < 0.0f) {
        x += 2.0f * PI;
    }
    float sinal = 1.0f;
    if (x > PI) {
        x -= PI;
        sinal = -1.0f;
    }
    if (x > 0.5f * PI) {
        x = PI - x;
    }
    float termo = x;
    float soma = x;
    float quadrado = x * x;
    for (int i = 1; i <= 4; i++) {
        termo *= -quadrado / ((2 * i) * (2 * i + 1));
        soma += termo;
    }
    return sinal * soma;
}

float cosseno(float x) {
    return seno(x + 0.5f * PI);
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



void desenhar_linha(Tela* fb, Vetor2D p1, Vetor2D p2, Cor cor) {
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

void desenhar_triangulo(Tela* fb, Vetor2D p1, Vetor2D p2, Vetor2D p3, Cor cor) {
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



void criar_matriz_rotacao(float matriz[4][4], float radianos, Vetor3D eixo) {
    criar_matriz_identidade(matriz);
    float magnitude = raiz(eixo.x * eixo.x + eixo.y * eixo.y + eixo.z * eixo.z);
    if (magnitude == 0.0f) {
        return;
    }
    float ux = eixo.x / magnitude;
    float uy = eixo.y / magnitude;
    float uz = eixo.z / magnitude;
    float c = cosseno(radianos);
    float s = seno(radianos);
    float omc = 1.0f - c;
    matriz[0][0] = c + ux * ux * omc;
    matriz[0][1] = ux * uy * omc + uz * s;
    matriz[0][2] = ux * uz * omc - uy * s;
    matriz[1][0] = ux * uy * omc - uz * s;
    matriz[1][1] = c + uy * uy * omc;
    matriz[1][2] = uy * uz * omc + ux * s;
    matriz[2][0] = ux * uz * omc + uy * s;
    matriz[2][1] = uy * uz * omc - ux * s;
    matriz[2][2] = c + uz * uz * omc;
}

void criar_matriz_camara(float matriz[4][4], Vetor3D posicao, Vetor3D alvo, Vetor3D cima) {
    float zx = posicao.x - alvo.x;
    float zy = posicao.y - alvo.y;
    float zz = posicao.z - alvo.z;
    float magnitude_z = raiz(zx * zx + zy * zy + zz * zz);
    if (magnitude_z > 0.0f) {
        zx /= magnitude_z;
        zy /= magnitude_z;
        zz /= magnitude_z;
    }
    float xx = cima.y * zz - cima.z * zy;
    float xy = cima.z * zx - cima.x * zz;
    float xz = cima.x * zy - cima.y * zx;
    float magnitude_x = raiz(xx * xx + xy * xy + xz * xz);
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

Cor calcular_iluminacao_difusa(Vetor3D normal, Vetor3D direcao_luz, Cor cor_objeto, Cor cor_luz) {
    float tam_normal_qd = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
    float rec_tam_normal = reciproco_raiz(tam_normal_qd);
    Vetor3D n = {
        normal.x * rec_tam_normal,
        normal.y * rec_tam_normal,
        normal.z * rec_tam_normal
    };
    float tam_luz_qd = direcao_luz.x * direcao_luz.x + direcao_luz.y * direcao_luz.y + direcao_luz.z * direcao_luz.z;
    float rec_tam_luz = reciproco_raiz(tam_luz_qd);
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
    float inv_tam_normal = reciproco_raiz(tam_normal_qd);
    Vetor3D n = { normal.x * inv_tam_normal, normal.y * inv_tam_normal, normal.z * inv_tam_normal };
    float tam_luz_qd = direcao_luz.x * direcao_luz.x + direcao_luz.y * direcao_luz.y + direcao_luz.z * direcao_luz.z;
    float inv_tam_luz = reciproco_raiz(tam_luz_qd);
    Vetor3D l = { direcao_luz.x * inv_tam_luz, direcao_luz.y * inv_tam_luz, direcao_luz.z * inv_tam_luz };
    float tam_olhar_qd = direcao_olhar.x * direcao_olhar.x + direcao_olhar.y * direcao_olhar.y + direcao_olhar.z * direcao_olhar.z;
    float inv_tam_olhar = reciproco_raiz(tam_olhar_qd);
    Vetor3D v = { direcao_olhar.x * inv_tam_olhar, direcao_olhar.y * inv_tam_olhar, direcao_olhar.z * inv_tam_olhar };
    Vetor3D h_sub = { l.x + v.x, l.y + v.y, l.z + v.z };
    float tam_h_sq = h_sub.x * h_sub.x + h_sub.y * h_sub.y + h_sub.z * h_sub.z;
    float inv_tam_h = reciproco_raiz(tam_h_sq);
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

Vetor2D projetar_ponto(Vetor3D ponto_3d, int largura, int altura) {
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

Vetor3D normalizar_vetor(Vetor3D v) {
    float comprimento = raiz((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
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

Vetor3D transformar_vetor(Vetor3D v, float matriz[static 16]) {

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
    float magnitude = raiz((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));
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



int main() {
  return 0;
}
