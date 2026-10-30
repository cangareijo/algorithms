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

void destruir_tela(Tela* tela);
void limpar_tela(Tela* tela, Cor cor);
void desenhar_pixel(Tela* tela, int x, int y, Cor cor);
void desenhar_linha(Tela* tela, Vetor2D p1, Vetor2D p2, Cor cor);
void desenhar_triangulo(Tela* tela, Vetor2D p1, Vetor2D p2, Vetor2D p3, Cor cor);
void desenhar_triangulo_preenchido(Tela* tela, Profundidade* pf, Vertice v1, Vertice v2, Vertice v3, Tela* textura);
void criar_matriz_identidade(float matriz[4][4]);
void criar_matriz_translacao(float matriz[4][4], float x, float y, float z);
void criar_matriz_escala(float matriz[4][4], float sx, float sy, float sz);
void criar_matriz_projecao(float matriz[4][4], float fov, float proporcao, float perto, float longe);
void criar_matriz_ortografica(float matriz[4][4], float esquerda, float direita, float baixo, float cima, float perto, float longe);
void multiplicar_matrizes(float resultado[4][4], float a[4][4], float b[4][4]);
void criar_matriz_rotacao(float matriz[4][4], float radianos, Vetor3D eixo);
void criar_matriz_olhar_para(float matriz[4][4], Vetor3D posicao, Vetor3D direcao, Vetor3D cima);
void limpar_profundidade(Profundidade* pf, float valor);

bool orientacao_traseira(Vetor3D v1, Vetor3D v2, Vetor3D v3, Vetor3D direcao_camara);
bool testar_e_definir_profundidade(Profundidade* pf, int x, int y, float z);

int recortar_triangulo_plano_perto(Vertice triangulo_in[3], Vertice triangulos_out[2], float plano_perto);

float absoluto(float x);
float truncar(float x);
float chao(float x);
float raiz(float x);
float reciproco_raiz(float x);
float seno(float x);
float cosseno(float x);
float tangente(float x);
float potencia_inteira(float x, int n);
float minimo(float x, float y);
float maximo(float x, float y);
float distancia(float x, float y);
float modulo(float x, float y);
float produto_escalar(Vetor3D u, Vetor3D v);

Cor multiplicar_cor(Cor cor, float intensidade);
Cor interpolar_cores(Cor c1, Cor c2, float fator);
Cor calcular_iluminacao_difusa(Vetor3D normal, Vetor3D direcao_luz, Cor cor_objeto, Cor cor_luz);
Cor calcular_iluminacao_especular(Vetor3D normal, Vetor3D direcao_luz, Vetor3D direcao_olhar, int brilho);
Cor amostrar_textura(Tela* textura, float u, float v);

Tela criar_tela(int largura, int altura);

Vetor2D projetar_ponto(Vetor3D ponto_3d, int largura, int altura);

Vetor3D calcular_coordenadas_baricentricas(Vetor2D p, Vetor2D a, Vetor2D b, Vetor2D c);
Vetor3D normalizar_vetor(Vetor3D v);
Vetor3D transformar_vetor(Vetor3D v, float matriz[4][4]);
Vetor3D somar_vetores(Vetor3D u, Vetor3D v);
Vetor3D subtrair_vetores(Vetor3D u, Vetor3D v);
Vetor3D produto_vetorial(Vetor3D u, Vetor3D v);
Vetor3D interpolar_vetores(Vetor3D a, Vetor3D b, float t);
Vetor3D calcular_normal(Vetor3D u, Vetor3D v, Vetor3D w);

Quaterniao quaterniao_identidade(void);

Raio gerar_raio_camara(Vetor3D posicao_camara, float matriz_inv_proj[4][4], int x, int y, int largura, int altura);

Intersecao intersetar_esfera(Raio raio, Vetor3D centro, float raio_esfera, Cor cor);
Intersecao intersetar_plano(Raio raio, Vetor3D ponto_plano, Vetor3D normal_plano, Cor cor);

Vertice interpolar_vertices(Vertice a, Vertice b, float t);



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

void desenhar_linha(Tela* tela, Vetor2D p1, Vetor2D p2, Cor cor) {
    if (tela == nullptr || tela->pixeis == nullptr || tela->largura <= 0 || tela->altura <= 0) {
        return;
    }
    int x0 = (int)p1.x;
    int y0 = (int)p1.y;
    int x1 = (int)p2.x;
    int y1 = (int)p2.y;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int erro = dx - dy;
    int largura = tela->largura;
    int altura = tela->altura;
    while (true) {
        if (x0 >= 0 && x0 < largura && y0 >= 0 && y0 < altura) {
            tela->pixeis[y0 * largura + x0] = cor;
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

void desenhar_triangulo(Tela* tela, Vetor2D p1, Vetor2D p2, Vetor2D p3, Cor cor) {
    if (tela == nullptr || tela->pixeis == nullptr || tela->largura <= 0 || tela->altura <= 0) {
        return;
    }
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
    if (area_dupla == 0.0f) {
        return;
    }
    float sinal = (area_dupla < 0.0f) ? -1.0f : 1.0f;
    int largura = tela->largura;
    Cor* pixeis = tela->pixeis;
    for (int y = min_y; y <= max_y; ++y) {
        float py = (float)y + 0.5f;
        int offset_linha = y * largura;
        for (int x = min_x; x <= max_x; ++x) {
            float px = (float)x + 0.5f;
            float w0 = ((px - p2.x) * dy23 - (py - p2.y) * dx23) * sinal;
            float w1 = ((px - p3.x) * dy31 - (py - p3.y) * dx31) * sinal;
            float w2 = ((px - p1.x) * dy12 - (py - p1.y) * dx12) * sinal;
            if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) {
                pixeis[offset_linha + x] = cor;
            }
        }
    }
}

void desenhar_triangulo_preenchido(Tela* tela, Profundidade* pf, Vertice v1, Vertice v2, Vertice v3, Tela* textura) {
  if (tela == NULL || pf == NULL) {
    return;
  }
  Vetor2D p1; p1.x = v1.posicao.x; p1.y = v1.posicao.y;
  Vetor2D p2; p2.x = v2.posicao.x; p2.y = v2.posicao.y;
  Vetor2D p3; p3.x = v3.posicao.x; p3.y = v3.posicao.y;
  float min_x = p1.x;
  if (p2.x < min_x) min_x = p2.x;
  if (p3.x < min_x) min_x = p3.x;
  float max_x = p1.x;
  if (p2.x > max_x) max_x = p2.x;
  if (p3.x > max_x) max_x = p3.x;
  float min_y = p1.y;
  if (p2.y < min_y) min_y = p2.y;
  if (p3.y < min_y) min_y = p3.y;
  float max_y = p1.y;
  if (p2.y > max_y) max_y = p2.y;
  if (p3.y > max_y) max_y = p3.y;
  int inicio_x = (int)min_x;
  if (inicio_x < 0) inicio_x = 0;
  int fim_x = (int)max_x + 1;
  if (fim_x >= tela->largura) fim_x = tela->largura - 1;
  int inicio_y = (int)min_y;
  if (inicio_y < 0) inicio_y = 0;
  int fim_y = (int)max_y + 1;
  if (fim_y >= tela->altura) fim_y = tela->altura - 1;
  float inv_w1 = 1.0f / v1.posicao.z;
  float inv_w2 = 1.0f / v2.posicao.z;
  float inv_w3 = 1.0f / v3.posicao.z;
  for (int y = inicio_y; y <= fim_y; ++y) {
    for (int x = inicio_x; x <= fim_x; ++x) {
      Vetor2D p;
      p.x = (float)x + 0.5f;
      p.y = (float)y + 0.5f;
      Vetor3D baricentricas = calcular_coordenadas_baricentricas(p, p1, p2, p3);
      if (baricentricas.x >= 0.0f && baricentricas.y >= 0.0f && baricentricas.z >= 0.0f) {
        float interpolado_inv_z = baricentricas.x * inv_w1 + baricentricas.y * inv_w2 + baricentricas.z * inv_w3;
        float z_atual = 1.0f / interpolado_inv_z;
        if (testar_e_definir_profundidade(pf, x, y, z_atual)) {
          Cor cor_final;
          if (textura != NULL && textura->pixeis != NULL) {
            float u_correto = (baricentricas.x * v1.uv.x * inv_w1 +
                               baricentricas.y * v2.uv.x * inv_w2 +
                               baricentricas.z * v3.uv.x * inv_w3) * z_atual;
            float v_correto = (baricentricas.x * v1.uv.y * inv_w1 +
                               baricentricas.y * v2.uv.y * inv_w2 +
                               baricentricas.z * v3.uv.y * inv_w3) * z_atual;
            cor_final = amostrar_textura(textura, u_correto, v_correto);
          } else {
            Cor c12 = interpolar_cores(v1.cor, v2.cor, baricentricas.y / (baricentricas.x + baricentricas.y + 0.000001f));
            cor_final = interpolar_cores(c12, v3.cor, baricentricas.z);
          }
          desenhar_pixel(tela, x, y, cor_final);
        }
      }
    }
  }
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

void criar_matriz_olhar_para(float matriz[4][4], Vetor3D posicao, Vetor3D alvo, Vetor3D cima) {
    Vetor3D z_eixo = normalizar_vetor(subtrair_vetores(posicao, alvo));
    Vetor3D x_eixo = normalizar_vetor(produto_vetorial(cima, z_eixo));
    Vetor3D y_eixo = produto_vetorial(z_eixo, x_eixo);
    matriz[0][0] = x_eixo.x;
    matriz[0][1] = y_eixo.x;
    matriz[0][2] = z_eixo.x;
    matriz[0][3] = 0.0f;
    matriz[1][0] = x_eixo.y;
    matriz[1][1] = y_eixo.y;
    matriz[1][2] = z_eixo.y;
    matriz[1][3] = 0.0f;
    matriz[2][0] = x_eixo.z;
    matriz[2][1] = y_eixo.z;
    matriz[2][2] = z_eixo.z;
    matriz[2][3] = 0.0f;
    matriz[3][0] = -produto_escalar(x_eixo, posicao);
    matriz[3][1] = -produto_escalar(y_eixo, posicao);
    matriz[3][2] = -produto_escalar(z_eixo, posicao);
    matriz[3][3] = 1.0f;
}

void limpar_profundidade(Profundidade* pf, float valor) {
  if (pf == NULL || pf->profundidades == NULL) {
    return;
  }
  if (pf->largura <= 0 || pf->altura <= 0) {
    return;
  }
  size_t total_pixeis = (size_t)pf->largura * (size_t)pf->altura;
  for (size_t i = 0; i < total_pixeis; ++i) {
    pf->profundidades[i] = valor;
  }
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

bool testar_e_definir_profundidade(Profundidade* pf, int x, int y, float z) {
  if (pf == NULL || pf->profundidades == NULL) {
    return false;
  }
  if (x < 0 || x >= pf->largura || y < 0 || y >= pf->altura) {
    return false;
  }
  size_t indice = (size_t)y * (size_t)pf->largura + (size_t)x;
  if (z < pf->profundidades[indice]) {
    pf->profundidades[indice] = z;
    return true;
  }
  return false;
}



int recortar_triangulo_plano_perto(Vertice triangulo_in[3], Vertice triangulos_out[2], float plano_perto) {
  Vertice vertices_dentro[3];
  Vertice vertices_fora[3];
  int contagem_dentro = 0;
  int contagem_fora = 0;
  for (int i = 0; i < 3; ++i) {
    if (triangulo_in[i].posicao.z >= plano_perto) {
      vertices_dentro[contagem_dentro] = triangulo_in[i];
      contagem_dentro = contagem_dentro + 1;
    } else {
      vertices_fora[contagem_fora] = triangulo_in[i];
      contagem_fora = contagem_fora + 1;
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
    triangulos_out[0] = vertices_dentro[0];
    float t1 = (plano_perto - vertices_dentro[0].posicao.z) / (vertices_fora[0].posicao.z - vertices_dentro[0].posicao.z);
    triangulos_out[1] = interpolar_vertices(vertices_dentro[0], vertices_fora[0], t1);
    float t2 = (plano_perto - vertices_dentro[0].posicao.z) / (vertices_fora[1].posicao.z - vertices_dentro[0].posicao.z);
    triangulos_out[2] = interpolar_vertices(vertices_dentro[0], vertices_fora[1], t2);
    return 1;
  }
  if (contagem_dentro == 2) {
    triangulos_out[0] = vertices_dentro[0];
    triangulos_out[1] = vertices_dentro[1];
    float t1 = (plano_perto - vertices_dentro[0].posicao.z) / (vertices_fora[0].posicao.z - vertices_dentro[0].posicao.z);
    triangulos_out[2] = interpolar_vertices(vertices_dentro[0], vertices_fora[0], t1);
    triangulos_out[3] = vertices_dentro[1];
    triangulos_out[4] = triangulos_out[2];
    float t2 = (plano_perto - vertices_dentro[1].posicao.z) / (vertices_fora[0].posicao.z - vertices_dentro[1].posicao.z);
    triangulos_out[5] = interpolar_vertices(vertices_dentro[1], vertices_fora[0], t2);
    return 2;
  }
  return 0;
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
    if (x <= 0.0f) return 0.0f;
    float inv = reciproco_raiz(x);
    if (inv == 0.0f) return 0.0f;
    float r = 1.0f / inv;
    return 0.5f * (r + x / r);
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

float modulo(float x, float y) {
    if (y == 0.0f) {
        return 0.0f;
    }
    return x - truncar(x / y) * y;
}

float produto_escalar(Vetor3D u, Vetor3D v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}



Cor multiplicar_cor(Cor cor, float intensidade) {
    if (intensidade <= 0.0f) return cor & 0xFF;
    if (intensidade >= 1.0f) return cor;
    uint8_t r = (cor >> 24) & 0xFF;
    uint8_t g = (cor >> 16) & 0xFF;
    uint8_t b = (cor >>  8) & 0xFF;
    uint8_t a =  cor        & 0xFF;
    uint32_t res_r = (uint32_t)(r * intensidade + 0.5f);
    uint32_t res_g = (uint32_t)(g * intensidade + 0.5f);
    uint32_t res_b = (uint32_t)(b * intensidade + 0.5f);
    return (res_r << 24) | (res_g << 16) | (res_b <<  8) | a;
}

Cor interpolar_cores(Cor c1, Cor c2, float fator) {
    if (fator <= 0.0f) return c1;
    if (fator >= 1.0f) return c2;
    uint8_t r1 = (c1 >> 24) & 0xFF;
    uint8_t g1 = (c1 >> 16) & 0xFF;
    uint8_t b1 = (c1 >>  8) & 0xFF;
    uint8_t a1 =  c1        & 0xFF;
    uint8_t r2 = (c2 >> 24) & 0xFF;
    uint8_t g2 = (c2 >> 16) & 0xFF;
    uint8_t b2 = (c2 >>  8) & 0xFF;
    uint8_t a2 =  c2        & 0xFF;
    uint32_t r_final = (uint32_t)(r1 + fator * (r2 - r1) + 0.5f);
    uint32_t g_final = (uint32_t)(g1 + fator * (g2 - g1) + 0.5f);
    uint32_t b_final = (uint32_t)(b1 + fator * (b2 - b1) + 0.5f);
    uint32_t a_final = (uint32_t)(a1 + fator * (a2 - a1) + 0.5f);
    return (r_final << 24) | (g_final << 16) | (b_final <<  8) | a_final;
}

Cor calcular_iluminacao_difusa(Vetor3D normal, Vetor3D direcao_luz, Cor cor_objeto, Cor cor_luz) {
    Vetor3D n = normalizar_vetor(normal);
    Vetor3D l = normalizar_vetor(direcao_luz);
    float cosseno = produto_escalar(n, l);
    if (cosseno < 0.0f) {
        cosseno = 0.0f;
    }
    uint8_t obj_r = (cor_objeto >> 24) & 0xFF;
    uint8_t obj_g = (cor_objeto >> 16) & 0xFF;
    uint8_t obj_b = (cor_objeto >> 8)  & 0xFF;
    uint8_t obj_a =  cor_objeto        & 0xFF;
    uint8_t luz_r = (cor_luz >> 24) & 0xFF;
    uint8_t luz_g = (cor_luz >> 16) & 0xFF;
    uint8_t luz_b = (cor_luz >> 8)  & 0xFF;
    uint8_t luz_a =  cor_luz        & 0xFF;
    float fator = cosseno / 255.0f;
    float res_r = (float)obj_r * (float)luz_r * fator;
    float res_g = (float)obj_g * (float)luz_g * fator;
    float res_b = (float)obj_b * (float)luz_b * fator;
    float res_a = ((float)obj_a * (float)luz_a) / 255.0f;
    if (res_r > 255.0f) res_r = 255.0f;
    if (res_g > 255.0f) res_g = 255.0f;
    if (res_b > 255.0f) res_b = 255.0f;
    if (res_a > 255.0f) res_a = 255.0f;
    return ((Cor)(uint8_t)res_r << 24) |
           ((Cor)(uint8_t)res_g << 16) |
           ((Cor)(uint8_t)res_b << 8)  |
            (Cor)(uint8_t)res_a;
}

Cor calcular_iluminacao_especular(Vetor3D normal, Vetor3D direcao_luz, Vetor3D direcao_olhar, int brilho) {
    Vetor3D n = normalizar_vetor(normal);
    Vetor3D l = normalizar_vetor(direcao_luz);
    Vetor3D v = normalizar_vetor(direcao_olhar);
    Vetor3D h = normalizar_vetor(somar_vetores(l, v));
    float dot_nh = produto_escalar(n, h);
    if (dot_nh < 0.0f) {
        dot_nh = 0.0f;
    }
    float dot_nl = produto_escalar(n, l);
    if (dot_nl <= 0.0f) {
        dot_nh = 0.0f;
    }
    float intensidade = potencia_inteira(dot_nh, brilho);
    float res_cor = intensidade * 255.0f;
    if (res_cor > 255.0f) {
        res_cor = 255.0f;
    }
    uint8_t especular = (uint8_t)res_cor;
    return ((Cor)especular << 24) |
           ((Cor)especular << 16) |
           ((Cor)especular << 8)  |
            (Cor)255;
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



Vetor2D projetar_ponto(Vetor3D ponto_3d, int largura, int height) {
    if (ponto_3d.z <= 0.1f) {
        return (Vetor2D){ .x = 0.0f, .y = 0.0f };
    }
    float fov_escala = (float)largura * 0.5f;
    float inv_z = 1.0f / ponto_3d.z;
    float x_projetado = (ponto_3d.x * fov_escala) * inv_z;
    float y_projetado = (ponto_3d.y * fov_escala) * inv_z;
    float x_ecra = x_projetado + ((float)largura * 0.5f);
    float y_ecra = -y_projetado + ((float)height * 0.5f);
    return (Vetor2D){ .x = x_ecra, .y = y_ecra };
}



Vetor3D calcular_coordenadas_baricentricas(Vetor2D p, Vetor2D a, Vetor2D b, Vetor2D c) {
  float det = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
  Vetor3D resultado;
  if (det > -0.000001f && det < 0.000001f) {
    resultado.x = -1.0f;
    resultado.y = -1.0f;
    resultado.z = -1.0f;
    return resultado;
  }
  float inv_det = 1.0f / det;
  resultado.x = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) * inv_det;
  resultado.y = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) * inv_det;
  resultado.z = 1.0f - resultado.x - resultado.y;
  return resultado;
}

Vetor3D normalizar_vetor(Vetor3D v) {
    float tam_qd = v.x * v.x + v.y * v.y + v.z * v.z;
    float inv_tam = reciproco_raiz(tam_qd);
    if (tam_qd == 0.0f) {
        return (Vetor3D){0.0f, 0.0f, 0.0f};
    }
    return (Vetor3D){
        .x = v.x * inv_tam,
        .y = v.y * inv_tam,
        .z = v.z * inv_tam
    };
}

Vetor3D transformar_vetor(Vetor3D v, float matriz[4][4]) {
    float x = v.x * matriz[0][0] + v.y * matriz[0][1] + v.z * matriz[0][2] + matriz[0][3];
    float y = v.x * matriz[1][0] + v.y * matriz[1][1] + v.z * matriz[1][2] + matriz[1][3];
    float z = v.x * matriz[2][0] + v.y * matriz[2][1] + v.z * matriz[2][2] + matriz[2][3];
    float w = v.x * matriz[3][0] + v.y * matriz[3][1] + v.z * matriz[3][2] + matriz[3][3];
    if (w != 0.0f && w != 1.0f) {
        float reciproco_w = 1.0f / w;
        x *= reciproco_w;
        y *= reciproco_w;
        z *= reciproco_w;
    }
    return (Vetor3D){ .x = x, .y = y, .z = z };
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

Vetor3D interpolar_vetores(Vetor3D a, Vetor3D b, float t) {
  Vetor3D resultado;
  resultado.x = a.x + t * (b.x - a.x);
  resultado.y = a.y + t * (b.y - a.y);
  resultado.z = a.z + t * (b.z - a.z);
  return resultado;
}

Vetor3D calcular_normal(Vetor3D v1, Vetor3D v2, Vetor3D v3) {
    Vetor3D aresta1 = subtrair_vetores(v2, v1);
    Vetor3D aresta2 = subtrair_vetores(v3, v1);
    Vetor3D normal_nao_normalizada = produto_vetorial(aresta1, aresta2);
    return normalizar_vetor(normal_nao_normalizada);
}



Quaterniao quaterniao_identidade(void) {
  Quaterniao resultado;
  resultado.w = 1.0f;
  resultado.x = 0.0f;
  resultado.y = 0.0f;
  resultado.z = 0.0f;
  return resultado;
}



Raio gerar_raio_camara(Vetor3D posicao_camara, float matriz_inv_proj[4][4], int x, int y, int largura, int altura) {
  Raio raio;
  raio.origem = posicao_camara;
  if (largura <= 0 || altura <= 0) {
    raio.direcao.x = 0.0f;
    raio.direcao.y = 0.0f;
    raio.direcao.z = -1.0f;
    return raio;
  }
  float ndc_x = (2.0f * ((float)x + 0.5f) / (float)largura) - 1.0f;
  float ndc_y = 1.0f - (2.0f * ((float)y + 0.5f) / (float)altura);
  Vetor3D ponto_clip;
  ponto_clip.x = ndc_x;
  ponto_clip.y = ndc_y;
  ponto_clip.z = 1.0f; 
  Vetor3D direcao_camara = transformar_vetor(ponto_clip, matriz_inv_proj);
  raio.direcao = normalizar_vetor(direcao_camara);
  return raio;
}



Intersecao intersetar_esfera(Raio raio, Vetor3D centro, float raio_esfera, Cor cor) {
  Intersecao resultado;
  resultado.intersetou = false;
  resultado.t = -1.0f;
  resultado.ponto.x = 0.0f; resultado.ponto.y = 0.0f; resultado.ponto.z = 0.0f;
  resultado.normal.x = 0.0f; resultado.normal.y = 0.0f; resultado.normal.z = 1.0f;
  resultado.cor_material = cor;
  Vetor3D l = subtrair_vetores(centro, raio.origem);
  float tca = produto_escalar(l, raio.direcao);
  float d2 = produto_escalar(l, l) - tca * tca;
  float raio2 = raio_esfera * raio_esfera;
  if (d2 > raio2) {
    return resultado;
  }
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
  Vetor3D vetor_deslocamento;
  vetor_deslocamento.x = raio.direcao.x * t_escolhido;
  vetor_deslocamento.y = raio.direcao.y * t_escolhido;
  vetor_deslocamento.z = raio.direcao.z * t_escolhido;
  resultado.ponto = somar_vetores(raio.origem, vetor_deslocamento);
  Vetor3D vetor_raio = subtrair_vetores(resultado.ponto, centro);
  resultado.normal = normalizar_vetor(vetor_raio);
  return resultado;
}

Intersecao intersetar_plano(Raio raio, Vetor3D ponto_plano, Vetor3D normal_plano, Cor cor) {
  Intersecao resultado;
  resultado.intersetou = false;
  resultado.t = -1.0f;
  resultado.ponto.x = 0.0f; resultado.ponto.y = 0.0f; resultado.ponto.z = 0.0f;
  resultado.normal = normal_plano;
  resultado.cor_material = cor;
  float denominador = produto_escalar(raio.direcao, normal_plano);
  if (denominador > -0.0001f && denominador < 0.0001f) {
    return resultado;
  }
  Vetor3D origem_a_ponto = subtrair_vetores(ponto_plano, raio.origem);
  float t_calculado = produto_escalar(origem_a_ponto, normal_plano) / denominador;
  if (t_calculado < 0.0001f) {
    return resultado;
  }
  resultado.intersetou = true;
  resultado.t = t_calculado;
  if (denominador > 0.0f) {
    resultado.normal.x = -normal_plano.x;
    resultado.normal.y = -normal_plano.y;
    resultado.normal.z = -normal_plano.z;
  }
  Vetor3D vetor_deslocamento;
  vetor_deslocamento.x = raio.direcao.x * t_calculado;
  vetor_deslocamento.y = raio.direcao.y * t_calculado;
  vetor_deslocamento.z = raio.direcao.z * t_calculado;
  resultado.ponto = somar_vetores(raio.origem, vetor_deslocamento);
  return resultado;
}



Vertice interpolar_vertices(Vertice a, Vertice b, float t) {
  Vertice resultado;
  resultado.posicao.x = a.posicao.x + t * (b.posicao.x - a.posicao.x);
  resultado.posicao.y = a.posicao.y + t * (b.posicao.y - a.posicao.y);
  resultado.posicao.z = a.posicao.z + t * (b.posicao.z - a.posicao.z);
  resultado.uv.x = a.uv.x + t * (b.uv.x - a.uv.x);
  resultado.uv.y = a.uv.y + t * (b.uv.y - a.uv.y);
  resultado.cor = interpolar_cores(a.cor, b.cor, t);
  return resultado;
}



int main() {
  return 0;
}
