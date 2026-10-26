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
float seno(float x);
float cosseno(float x);
float tangente(float x);
float minimo(float x, float y);
float maximo(float x, float y);
float distancia(float x, float y);
float modulo(float x, float y);
float* criar_zbuffer(int largura, int altura);

void destruir_framebuffer(Framebuffer* fb);
void limpar_framebuffer(Framebuffer* fb, Cor cor);
void desenhar_pixel(Framebuffer* fb, int x, int y, Cor cor);
Framebuffer criar_framebuffer(int largura, int altura);

Cor amostrar_textura(const Textura* textura, float u, float v);

void desenhar_linha(Framebuffer* fb, Vetor2D p1, Vetor2D p2, Cor cor);
void rasterizar_triangulo(Framebuffer* fb, Vetor2D p1, Vetor2D p2, Vetor2D p3, Cor cor);

Vetor3D normalizar_vetor(Vetor3D v);
Vetor3D somar_vetores(Vetor3D a, Vetor3D b);
Vetor3D subtrair_vetores(Vetor3D a, Vetor3D b);
Vetor3D produto_vetorial(Vetor3D a, Vetor3D b);
float produto_escalar(Vetor3D a, Vetor3D b);
Vetor2D projetar_ponto(Vetor3D ponto_3d, int largura, int altura);
Vetor3D transformar_vetor(Vetor3D v, float matriz[static 16]);
Vetor3D calcular_normal(Vetor3D v1, Vetor3D v2, Vetor3D v3);

void rasterizar_triangulo_interpolado(Framebuffer* fb, Vertice v1, Vertice v2, Vertice v3);

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


    int total_pixeis = largura * altura;


    float* zbuffer = calloc(total_pixeis, sizeof(float));

    if (zbuffer == nullptr) {
        fprintf(stderr, "Erro: Falha na alocação de memória para o Z-buffer.\n");
        return nullptr;
    }


    limpar_zbuffer(zbuffer, largura, altura);

    return zbuffer;
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

float produto_escalar(Vetor3D a, Vetor3D b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
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


  int min_x = (int)minimo(minimo(x1, x2), x3);
  int max_x = (int)maximo(maximo(x1, x2), x3);
  int min_y = (int)minimo(minimo(y1, y2), y3);
  int max_y = (int)maximo(maximo(y1, y2), y3);


  if (min_x < 0) min_x = 0;
  if (min_y < 0) min_y = 0;
  if (max_x >= fb->largura) max_x = fb->largura - 1;
  if (max_y >= fb->altura) max_y = fb->altura - 1;



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
