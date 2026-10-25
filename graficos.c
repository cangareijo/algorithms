#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t CorRGBA;

typedef struct {
  CorRGBA* pixeis;
  int largura;
  int altura;
} Framebuffer;

typedef struct {
  CorRGBA* dados;
  int largura;
  int altura;
} Textura;

typedef struct {
  float x, y;
} Vetor2D;

typedef struct {
  float x, y, z;
} Vetor3D;

typedef struct {
  Vetor3D posicao;
  CorRGBA cor;
} Vertice;

void limpar_zbuffer(float* zbuffer, int largura, int altura);
[[nodiscard]] float* criar_zbuffer(int largura, int altura);

void destruir_framebuffer(Framebuffer* fb);
void limpar_framebuffer(Framebuffer* fb, CorRGBA cor);
void desenhar_pixel(Framebuffer* fb, int x, int y, CorRGBA cor);
[[nodiscard]] Framebuffer criar_framebuffer(int largura, int altura);

[[nodiscard]] CorRGBA amostrar_textura(const Textura* textura, float u, float v);

void desenhar_linha(Framebuffer* fb, Vetor2D p1, Vetor2D p2, CorRGBA cor);
void rasterizar_triangulo(Framebuffer* fb, Vetor2D p1, Vetor2D p2, Vetor2D p3, CorRGBA cor);

[[nodiscard]] Vetor2D projetar_ponto(Vetor3D ponto_3d, int largura, int altura);
[[nodiscard]] Vetor3D transformar_vetor(Vetor3D v, float matriz[static 16]);
[[nodiscard]] Vetor3D calcular_normal(Vetor3D v1, Vetor3D v2, Vetor3D v3);

void rasterizar_triangulo_interpolado(Framebuffer* fb, Vertice v1, Vertice v2, Vertice v3);

void limpar_zbuffer(float* zbuffer, int largura, int altura) {
    // Proteção contra ponteiros nulos ou dimensões inválidas
    if (zbuffer == nullptr || largura <= 0 || altura <= 0) {
        return;
    }

    int total_pixeis = largura * altura;

    // Preenche o buffer com 1.0f (profundidade máxima no espaço de projeção)
    for (int i = 0; i < total_pixeis; ++i) {
        zbuffer[i] = 1.0f;
    }
}

[[nodiscard]] float* criar_zbuffer(int largura, int altura) {
    // Validação estrita dos argumentos para evitar overflows ou alocações inválidas
    if (largura <= 0 || altura <= 0) {
        fprintf(stderr, "Erro: Dimensões do Z-buffer inválidas (%dx%d).\n", largura, altura);
        return nullptr;
    }

    // Conversão explícita para size_t para evitar overflow de inteiros (UBSan)
    int total_pixeis = largura * altura;
    
    // Alocação dinâmica de memória para os valores de profundidade (float)
    float* zbuffer = calloc(total_pixeis, sizeof(float));

    if (zbuffer == nullptr) {
        fprintf(stderr, "Erro: Falha na alocação de memória para o Z-buffer.\n");
        return nullptr;
    }

    // Inicializa o buffer imediatamente para evitar leituras de memória não inicializada
    limpar_zbuffer(zbuffer, largura, altura);

    return zbuffer;
}

void destruir_framebuffer(Framebuffer* fb) {
  // Se o ponteiro for nulo, não há nada a libertar
  if (fb == nullptr) {
    return;
  }

  // Liberta a memória contígua dos pixeis
  free(fb->pixeis);

  // Invalida os membros da estrutura para maior segurança
  fb->pixeis = nullptr;
  fb->largura = 0;
  fb->altura = 0;
}

void limpar_framebuffer(Framebuffer* fb, CorRGBA cor) {
  // Validação de segurança dos ponteiros para evitar falhas de segmentação
  if (fb == nullptr || fb->pixeis == nullptr) {
    return;
  }

  // Cálculo da área total do ecrã em píxeis
  int area = fb->largura * fb->altura;

  // Preenchimento linear da memória contígua
  for (int i = 0; i < area; ++i) {
    fb->pixeis[i] = cor;
  }
}

void desenhar_pixel(Framebuffer* fb, int x, int y, CorRGBA cor) {
  // Validação do ponteiro do framebuffer e da sua memória de píxeis
  if (fb == nullptr || fb->pixeis == nullptr) {
    return;
  }

  // Controlo de limites (Clipping) para evitar acessos fora do array
  if (x < 0 || x >= fb->largura || y < 0 || y >= fb->altura) {
    return;
  }

  // Cálculo do índice linear na memória contígua (fórmula: x + y * largura)
  fb->pixeis[x + y * fb->largura] = cor;
}

[[nodiscard]] Framebuffer criar_framebuffer(int largura, int altura) {
  Framebuffer fb = {};
  if (largura <= 0 || altura <= 0) {
    return fb;
  }
  CorRGBA* memoria_pixeis = malloc(largura * altura * sizeof(CorRGBA));
  if (memoria_pixeis == nullptr) {
    return fb;
  }
  fb.pixeis = memoria_pixeis;
  fb.largura = largura;
  fb.altura = altura;
  return fb;
}

[[nodiscard]] CorRGBA amostrar_textura(const Textura* textura, float u, float v) {
  // Se a textura for inválida ou não tiver dados, devolve um píxel transparente (0)
  if (textura == nullptr || textura->dados == nullptr || textura->largura <= 0 || textura->altura <= 0) {
    return 0;
  }

  // Implementação do modo "Repeat" (Wrap) para as coordenadas u e v.
  // Garante que os valores ficam sempre mapeados no intervalo [0.0f, 1.0f[.
  float u_repetido = u - floorf(u);
  float v_repetido = v - floorf(v);

  // Conversão das coordenadas normalizadas para índices discretos de píxeis (téxeis)
  int x = (int)(u_repetido * (float)textura->largura);
  int y = (int)(v_repetido * (float)textura->altura);

  // Salvaguarda por precisão numérica flutuante para evitar extrapolar os limites do array
  if (x >= textura->largura) x = textura->largura - 1;
  if (y >= textura->altura)  y =  textura->altura - 1;
  if (x < 0) x = 0;
  if (y < 0) y = 0;

  // Retorna a cor do téxel correspondente
  return textura->dados[x + y * textura->largura];
}

void desenhar_linha(Framebuffer* fb, Vetor2D p1, Vetor2D p2, CorRGBA cor) {
    // Validação do ponteiro do framebuffer e do array de píxeis
    if (fb == nullptr || fb->pixeis == nullptr || fb->largura <= 0 || fb->altura <= 0) {
        return;
    }

    // Conversão das coordenadas contínuas (float) para discretas (int)
    // O truncamento simples é suficiente para a rasterização básica
    int x0 = (int)p1.x;
    int y0 = (int)p1.y;
    int x1 = (int)p2.x;
    int y1 = (int)p2.y;

    // Algoritmo de Bresenham
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    
    int erro = dx - dy;

    while (true) {
        // Guardião de Limites (Pixel Clipping): Garante total segurança de memória.
        // Impede que coordenadas fora do ecrã corrompam a memória do programa.
        if (x0 >= 0 && x0 < fb->largura && y0 >= 0 && y0 < fb->altura) {
            fb->pixeis[y0 * fb->largura + x0] = cor;
        }

        // Se chegámos ao ponto final, o traçado terminou
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

void rasterizar_triangulo(Framebuffer* fb, Vetor2D p1, Vetor2D p2, Vetor2D p3, CorRGBA cor) {
  if (fb == nullptr || fb->pixeis == nullptr) {
    return;
  }
  int min_x = fminf(fminf(p1.x, p2.x), p3.x);
  int max_x = fmaxf(fmaxf(p1.x, p2.x), p3.x);
  int min_y = fminf(fminf(p1.y, p2.y), p3.y);
  int max_y = fmaxf(fmaxf(p1.y, p2.y), p3.y);
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

[[nodiscard]] Vetor2D projetar_ponto(Vetor3D ponto_3d, int largura, int altura) {
  // Evita a divisão por zero ou projeções inválidas se o ponto estiver atrás da câmara
  // ou mesmo na origem do plano de corte anterior (near plane).
  if (ponto_3d.z <= 0.1f) {
    return (Vetor2D){ .x = 0.0f, .y = 0.0f };
  }

  // Define um campo de visão (FOV) simulado através de uma distância focal.
  // Utiliza-se a largura do ecrã como base para a escala de perspetiva.
  float fov_escala = (float)largura * 0.5f;

  // Projeção de perspetiva para o plano 2D normalizado
  float x_projetado = (ponto_3d.x * fov_escala) / ponto_3d.z;
  float y_projetado = (ponto_3d.y * fov_escala) / ponto_3d.z;

  // Translação para o espaço do ecrã (píxeis)
  // Converte o centro do ecrã para a origem (0,0) do sistema 3D
  // e inverte o eixo Y porque no ecrã o Y cresce para baixo.
  float x_ecra = x_projetado + ((float)largura * 0.5f);
  float y_ecra = -y_projetado + ((float)altura * 0.5f);

  return (Vetor2D){ .x = x_ecra, .y = y_ecra };
}

[[nodiscard]] Vetor3D transformar_vetor(Vetor3D v, float matriz[static 16]) {
  // Multiplicação da matriz 4x4 pelo vetor homogéneo [v.x, v.y, v.z, 1.0f]
  float x = matriz[0] * v.x + matriz[1] * v.y + matriz[2] * v.z + matriz[3];
  float y = matriz[4] * v.x + matriz[5] * v.y + matriz[6] * v.z + matriz[7];
  float z = matriz[8] * v.x + matriz[9] * v.y + matriz[10] * v.z + matriz[11];
  float w = matriz[12] * v.x + matriz[13] * v.y + matriz[14] * v.z + matriz[15];

  // Divisão de perspetiva para regressar ao espaço 3D cartesiano
  // Evita a divisão por zero caso w seja nulo
  if (w != 0.0f) {
    float inv_w = 1.0f / w;
    x *= inv_w;
    y *= inv_w;
    z *= inv_w;
  }

  return (Vetor3D){ .x = x, .y = y, .z = z };
}

[[nodiscard]] Vetor3D calcular_normal(Vetor3D v1, Vetor3D v2, Vetor3D v3) {
    // 1. Calcular as arestas do triângulo (dois vetores diretores)
    Vetor3D aresta1 = { .x = v2.x - v1.x, .y = v2.y - v1.y, .z = v2.z - v1.z };
    Vetor3D aresta2 = { .x = v3.x - v1.x, .y = v3.y - v1.y, .z = v3.z - v1.z };

    // 2. Calcular o produto cruzado (Aresta1 x Aresta2) para obter o vetor perpendicular
    Vetor3D normal = {
        .x = (aresta1.y * aresta2.z) - (aresta1.z * aresta2.y),
        .y = (aresta1.z * aresta2.x) - (aresta1.x * aresta2.z),
        .z = (aresta1.x * aresta2.y) - (aresta1.y * aresta2.x)
    };

    // 3. Calcular a magnitude (comprimento) do vetor resultante
    // Usamos a função de precisão simples sqrtf() ideal para floats
    float magnitude = sqrtf((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

    // 4. Normalizar o vetor (dividir cada componente pela magnitude)
    // Prevenção contra a divisão por zero para evitar comportamento indefinido (UB)
    if (magnitude > 0.000001f) {
        float inverso_magnitude = 1.0f / magnitude;
        normal.x *= inverso_magnitude;
        normal.y *= inverso_magnitude;
        normal.z *= inverso_magnitude;
    } else {
        // Retorna um vetor nulo por defeito caso o triângulo seja degenerado (área zero)
        normal = (Vetor3D){ .x = 0.0f, .y = 0.0f, .z = 0.0f };
    }

    return normal;
}

void rasterizar_triangulo_interpolado(Framebuffer* fb, Vertice v1, Vertice v2, Vertice v3) {
  if (fb == nullptr || fb->pixeis == nullptr) {
    return;
  }

  // Extração das posições 2D para simplificar os cálculos de área
  float x1 = v1.posicao.x; float y1 = v1.posicao.y;
  float x2 = v2.posicao.x; float y2 = v2.posicao.y;
  float x3 = v3.posicao.x; float y3 = v3.posicao.y;

  // Encontrar os limites da caixa delimitadora (bounding box)
  int min_x = (int)fminf(fminf(x1, x2), x3);
  int max_x = (int)fmaxf(fmaxf(x1, x2), x3);
  int min_y = (int)fminf(fminf(y1, y2), y3);
  int max_y = (int)fmaxf(fmaxf(y1, y2), y3);

  // Recortar a caixa delimitadora contra os limites do framebuffer
  if (min_x < 0) min_x = 0;
  if (min_y < 0) min_y = 0;
  if (max_x >= fb->largura) max_x = fb->largura - 1;
  if (max_y >= fb->altura) max_y = fb->altura - 1;

  // Cálculo do determinante (área dupla do triângulo)
  // Determina também a orientação (CW vs CCW)
  float det = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
  if (det == 0.0f) {
    return; // Triângulo degenerado
  }
  float inv_det = 1.0f / det;

  // Extração e separação dos canais de cor de cada vértice
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

  // Percorrer a caixa delimitadora píxel a píxel
  for (int y = min_y; y <= max_y; ++y) {
    for (int x = min_x; x <= max_x; ++x) {
      float px = (float)x + 0.5f; // Centro do píxel
      float py = (float)y + 0.5f;

      // Sub-áreas para o cálculo dos pesos baricêntricos
      float area1 = (x3 - x2) * (py - y2) - (y3 - y2) * (px - x2);
      float area2 = (x1 - x3) * (py - y3) - (y1 - y3) * (px - x3);
      
      // Pesos baricêntricos (w1, w2, w3) relativos a cada vértice
      float w1 = area1 * inv_det;
      float w2 = area2 * inv_det;
      float w3 = 1.0f - w1 - w2;

      // Se todos os pesos tiverem o mesmo sinal, o píxel está dentro do triângulo
      // (Suporta ambas as orientações horária e anti-horária)
      if ((w1 >= 0.0f && w2 >= 0.0f && w3 >= 0.0f) || (w1 <= 0.0f && w2 <= 0.0f && w3 <= 0.0f)) {
        
        // Interpolação linear dos canais de cor
        float r_interp = w1 * r1 + w2 * r2 + w3 * r3;
        float g_interp = w1 * g1 + w2 * g2 + w3 * g3;
        float b_interp = w1 * b1 + w2 * b2 + w3 * b3;
        float a_interp = w1 * a1 + w2 * a2 + w3 * a3;

        // Garantir que os valores finais estão no intervalo válido [0, 255]
        uint32_t r = (uint32_t)fminf(fmaxf(r_interp, 0.0f), 255.0f);
        uint32_t g = (uint32_t)fminf(fmaxf(g_interp, 0.0f), 255.0f);
        uint32_t b = (uint32_t)fminf(fmaxf(b_interp, 0.0f), 255.0f);
        uint32_t a = (uint32_t)fminf(fmaxf(a_interp, 0.0f), 255.0f);

        // Reconstrução da cor final compactada em RGBA (32-bit)
        CorRGBA cor_final = (r << 24) | (g << 16) | (b << 8) | a;

        desenhar_pixel(fb, x, y, cor_final);
      }
    }
  }
}

int main() {
  return 0;
}
