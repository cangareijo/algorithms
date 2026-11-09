#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  uint8_t vermelho;
  uint8_t verde;
  uint8_t azul;
  uint8_t alfa;
} Cor;

typedef struct {
  int32_t x;
  int32_t y;
} Ponto2D;

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

[[nodiscard]] Ponto2D rodar_ponto(const Ponto2D ponto, const Ponto2D origem, const float radianos);
[[nodiscard]] Tela criar_tela(const uint32_t largura, const uint32_t altura);
void destruir_tela(Tela *const tela);
void limpar_tela(Tela *const tela, const Cor fundo);
void desenhar_píxel(Tela *const tela, const Ponto2D ponto, const Cor cor);
void desenhar_píxel_alfa(Tela *const tela, const Ponto2D ponto, const Cor cor_fonte);
void desenhar_linha(Tela *const tela, const Ponto2D início, const Ponto2D fim, const Cor cor);
void desenhar_triângulo(Tela *const tela, const Ponto2D p1, const Ponto2D p2, const Ponto2D p3, const Cor cor);
void desenhar_triângulo_preenchido(Tela *const tela, const Ponto2D p1, const Ponto2D p2, const Ponto2D p3, const Cor cor);
void desenhar_retângulo(Tela *const tela, const Ponto2D superior_esquerdo, const uint32_t largura, const uint32_t altura, const Cor cor);
void desenhar_retângulo_preenchido(Tela *const tela, const Ponto2D superior_esquerdo, const uint32_t largura, const uint32_t altura, const Cor cor);
void desenhar_polígono(Tela *const tela, const Ponto2D vértices[], const size_t núm_vértices, const Cor cor);
void desenhar_círculo(Tela *const tela, const Ponto2D centro, const uint32_t raio, const Cor cor);
void desenhar_círculo_preenchido(Tela *const tela, const Ponto2D centro, const uint32_t raio, const Cor cor);
void desenhar_elipse(Tela *const tela, const Ponto2D centro, const int32_t rx, const int32_t ry, const Cor cor);
void desenhar_imagem(Tela *const destino, const Tela *const origem, const Ponto2D posição);
void desenhar_imagem_escala(Tela *const destino, const Tela *const origem, const Ponto2D posição, const int32_t largura_nova, const int32_t altura_nova);
bool exportar_para_ficheiro(const Tela *const tela, const char *const caminho);

[[nodiscard]] Ponto2D rodar_ponto(const Ponto2D ponto, const Ponto2D origem, const float radianos) {
  if (radianos == 0.0f) return ponto;
  const float cosseno = cosf(radianos);
  const float seno = sinf(radianos);
  const float delta_x = (float)(ponto.x - origem.x);
  const float delta_y = (float)(ponto.y - origem.y);
  const float novo_x = origem.x + (delta_x * cosseno) - (delta_y * seno);
  const float novo_y = origem.y + (delta_x * seno) + (delta_y * cosseno);
  return (Ponto2D){
    .x = (int32_t)roundf(novo_x),
    .y = (int32_t)roundf(novo_y)
  };
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

void desenhar_píxel(Tela *const tela, const Ponto2D ponto, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  if (ponto.x < 0 || ponto.x >= (int32_t)tela->largura) return;
  if (ponto.y < 0 || ponto.y >= (int32_t)tela->altura) return;
  tela->píxeis[ponto.x + ponto.y * tela->largura] = cor;
}

void desenhar_píxel_alfa(Tela *const tela, const Ponto2D ponto, const Cor cor_fonte) {
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

void desenhar_linha(Tela *const tela, const Ponto2D início, const Ponto2D fim, const Cor cor) {
  const int32_t delta_x = abs(fim.x - início.x);
  const int32_t delta_y = abs(fim.y - início.y);
  const int32_t passo_x = (início.x < fim.x) ? 1 : -1;
  const int32_t passo_y = (início.y < fim.y) ? 1 : -1;
  int32_t erro = delta_x - delta_y;
  Ponto2D atual = início;
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

void desenhar_triângulo(Tela *const tela, const Ponto2D p1, const Ponto2D p2, const Ponto2D p3, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  desenhar_linha(tela, p1, p2, cor);
  desenhar_linha(tela, p2, p3, cor);
  desenhar_linha(tela, p3, p1, cor);
}

static inline void ordenar_pontos_y(Ponto2D *const p1, Ponto2D *const p2) {
  if (p1->y > p2->y) {
    const Ponto2D temporário = *p1;
    *p1 = *p2;
    *p2 = temporário;
  }
}

static void preencher_triângulo_base_plana(Tela *const tela, const Ponto2D p1, const Ponto2D p2, const Ponto2D p3, const Cor cor) {
  const int32_t altura = p2.y - p1.y;
  if (altura == 0) return;
  const int32_t dx1 = p2.x - p1.x;
  const int32_t dx2 = p3.x - p1.x;
  for (int32_t y = p1.y; y <= p2.y; ++y) {
    const int32_t dy = y - p1.y;
    int32_t x1 = p1.x + (dy * dx1) / altura;
    int32_t x2 = p1.x + (dy * dx2) / altura;
    if (x1 > x2) { const int32_t t = x1; x1 = x2; x2 = t; }
    desenhar_linha(tela, (Ponto2D){ x1, y }, (Ponto2D){ x2, y }, cor);
  }
}

static void preencher_triângulo_topo_plano(Tela *const tela, const Ponto2D p1, const Ponto2D p2, const Ponto2D p3, const Cor cor) {
  const int32_t altura = p3.y - p1.y;
  if (altura == 0) return;
  const int32_t dx1 = p3.x - p1.x;
  const int32_t dx2 = p3.x - p2.x;
  for (int32_t y = p1.y; y <= p3.y; ++y) {
    const int32_t dy = p3.y - y;
    int32_t x1 = p3.x - (dy * dx1) / altura;
    int32_t x2 = p3.x - (dy * dx2) / altura;
    if (x1 > x2) { const int32_t t = x1; x1 = x2; x2 = t; }
    desenhar_linha(tela, (Ponto2D){ x1, y }, (Ponto2D){ x2, y }, cor);
  }
}

void desenhar_triângulo_preenchido(Tela *const tela, const Ponto2D p1, const Ponto2D p2, const Ponto2D p3, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  Ponto2D v1 = p1;
  Ponto2D v2 = p2;
  Ponto2D v3 = p3;
  ordenar_pontos_y(&v1, &v2);
  ordenar_pontos_y(&v1, &v3);
  ordenar_pontos_y(&v2, &v3);
  if (v2.y == v3.y) preencher_triângulo_base_plana(tela, v1, v2, v3, cor);
  else if (v1.y == v2.y) preencher_triângulo_topo_plano(tela, v1, v2, v3, cor);
  else {
    const int32_t altura_total = v3.y - v1.y;
    const int32_t dx = v3.x - v1.x;
    const int32_t dy = v2.y - v1.y;
    const Ponto2D v4 = {
      .x = v1.x + (dy * dx) / altura_total,
      .y = v2.y
    };
    preencher_triângulo_base_plana(tela, v1, v2, v4, cor);
    preencher_triângulo_topo_plano(tela, v2, v4, v3, cor);
  }
}

void desenhar_retângulo(Tela *const tela, const Ponto2D superior_esquerdo, const uint32_t largura, const uint32_t altura, const Cor cor) {
  if (largura == 0 || altura == 0) return;
  const int32_t inferior_direito_x = superior_esquerdo.x + (int32_t)largura - 1;
  const int32_t inferior_direito_y = superior_esquerdo.y + (int32_t)altura  - 1;
  const Ponto2D superior_direito  = { .x =  inferior_direito_x, .y = superior_esquerdo.y };
  const Ponto2D inferior_esquerdo = { .x = superior_esquerdo.x, .y = inferior_direito_y  };
  const Ponto2D inferior_direito  = { .x =  inferior_direito_x, .y = inferior_direito_y  };
  desenhar_linha(tela, superior_esquerdo,  superior_direito, cor);
  desenhar_linha(tela, inferior_esquerdo,  inferior_direito, cor);
  desenhar_linha(tela, superior_esquerdo, inferior_esquerdo, cor);
  desenhar_linha(tela,  superior_direito,  inferior_direito, cor);
}

void desenhar_retângulo_preenchido(Tela *const tela, const Ponto2D superior_esquerdo, const uint32_t largura, const uint32_t altura, const Cor cor) {
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

void desenhar_polígono(Tela *const tela, const Ponto2D vértices[], const size_t núm_vértices, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || vértices == nullptr || núm_vértices < 3) return;
  for (size_t i = 1; i < núm_vértices; ++i) desenhar_linha(tela, vértices[i - 1], vértices[i], cor);
  desenhar_linha(tela, vértices[núm_vértices - 1], vértices[0], cor);
}

void desenhar_círculo(Tela *const tela, const Ponto2D centro, const uint32_t raio, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  if (raio == 0) {
    desenhar_píxel(tela, centro, cor);
    return;
  }
  int32_t x = 0;
  int32_t y = (int32_t)raio;
  int32_t decisão = 3 - 2 * (int32_t)raio;
  while (x <= y) {
    desenhar_píxel(tela, (Ponto2D){ centro.x + x, centro.y + y }, cor);
    desenhar_píxel(tela, (Ponto2D){ centro.x - x, centro.y + y }, cor);
    desenhar_píxel(tela, (Ponto2D){ centro.x + x, centro.y - y }, cor);
    desenhar_píxel(tela, (Ponto2D){ centro.x - x, centro.y - y }, cor);
    desenhar_píxel(tela, (Ponto2D){ centro.x + y, centro.y + x }, cor);
    desenhar_píxel(tela, (Ponto2D){ centro.x - y, centro.y + x }, cor);
    desenhar_píxel(tela, (Ponto2D){ centro.x + y, centro.y - x }, cor);
    desenhar_píxel(tela, (Ponto2D){ centro.x - y, centro.y - x }, cor);
    if (decisão < 0) {
      decisão += 4 * x + 6;
    } else {
      decisão += 4 * (x - y) + 10;
      y--;
    }
    x++;
  }
}

void desenhar_círculo_preenchido(Tela *const tela, const Ponto2D centro, const uint32_t raio, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr) return;
  if (raio == 0) {
    desenhar_píxel(tela, centro, cor);
    return;
  }
  int32_t x = 0;
  int32_t y = (int32_t)raio;
  int32_t decisão = 3 - 2 * (int32_t)raio;
  while (x <= y) {
    desenhar_linha(tela, (Ponto2D){ centro.x - x, centro.y + y }, (Ponto2D){ centro.x + x, centro.y + y }, cor);
    desenhar_linha(tela, (Ponto2D){ centro.x - x, centro.y - y }, (Ponto2D){ centro.x + x, centro.y - y }, cor);
    desenhar_linha(tela, (Ponto2D){ centro.x - y, centro.y + x }, (Ponto2D){ centro.x + y, centro.y + x }, cor);
    desenhar_linha(tela, (Ponto2D){ centro.x - y, centro.y - x }, (Ponto2D){ centro.x + y, centro.y - x }, cor);
    if (decisão < 0) {
      decisão += 4 * x + 6;
    } else {
      decisão += 4 * (x - y) + 10;
      y--;
    }
    x++;
  }
}

void desenhar_elipse(Tela *const tela, const Ponto2D centro, const int32_t rx, const int32_t ry, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || rx <= 0 || ry <= 0) return;
  const int32_t rx_quadrado = rx * rx;
  const int32_t ry_quadrado = ry * ry;
  int32_t x = 0;
  int32_t y = ry;
  int32_t dx = 2 * ry_quadrado * x;
  int32_t dy = 2 * rx_quadrado * y;
  int32_t decisão = ry_quadrado - (rx_quadrado * ry) + (rx_quadrado / 4);
  while (dx < dy) {
    desenhar_píxel(tela, (Ponto2D){ centro.x + x, centro.y + y }, cor);
    desenhar_píxel(tela, (Ponto2D){ centro.x - x, centro.y + y }, cor);
    desenhar_píxel(tela, (Ponto2D){ centro.x + x, centro.y - y }, cor);
    desenhar_píxel(tela, (Ponto2D){ centro.x - x, centro.y - y }, cor);
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
    desenhar_píxel(tela, (Ponto2D){ centro.x + x, centro.y + y }, cor);
    desenhar_píxel(tela, (Ponto2D){ centro.x - x, centro.y + y }, cor);
    desenhar_píxel(tela, (Ponto2D){ centro.x + x, centro.y - y }, cor);
    desenhar_píxel(tela, (Ponto2D){ centro.x - x, centro.y - y }, cor);
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

void desenhar_imagem(Tela *const destino, const Tela *const origem, const Ponto2D posição) {
  if (destino == nullptr || destino->píxeis == nullptr || origem == nullptr || origem->píxeis == nullptr) return;
  for (int32_t y_origem = 0; y_origem < (int32_t)origem->altura; ++y_origem) {
    const int32_t y_destino = posição.y + y_origem;
    if (y_destino < 0 || y_destino >= (int32_t)destino->altura) continue;
    for (int32_t x_origem = 0; x_origem < (int32_t)origem->largura; ++x_origem) {
      const int32_t x_destino = posição.x + x_origem;
      if (x_destino < 0 || x_destino >= (int32_t)destino->largura) continue;
      const size_t índice_origem = (size_t)x_origem + (size_t)y_origem * origem->largura;
      const Cor cor_píxel = origem->píxeis[índice_origem];
      desenhar_píxel_alfa(destino, (Ponto2D){ x_destino, y_destino }, cor_píxel);
    }
  }
}

void desenhar_imagem_escala(Tela *const destino, const Tela *const origem, const Ponto2D posição, const int32_t largura_nova, const int32_t altura_nova) {
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
      desenhar_píxel_alfa(destino, (Ponto2D){ x_destino_real, y_destino_real }, cor_píxel);
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

int main() {
  Tela tela = criar_tela(500, 500);
  if (tela.píxeis == nullptr) return 1;
  limpar_tela(&tela, COR_BRANCA);
  desenhar_píxel(&tela, (Ponto2D){ 250, 250 }, COR_PRETA);
  desenhar_linha(&tela, (Ponto2D){ 10, 10 }, (Ponto2D){ 490, 490 }, COR_VERMELHA);
  desenhar_retângulo(&tela, (Ponto2D){ 30, 50 }, 120, 80, COR_AZUL);
  desenhar_retângulo_preenchido(&tela, (Ponto2D){ 30, 350 }, 100, 100, COR_VERDE);
  desenhar_círculo(&tela, (Ponto2D){ 400, 100 }, 50, COR_AMARELA);
  desenhar_círculo_preenchido(&tela, (Ponto2D){ 400, 380 }, 60, COR_MAGENTA);
  if (exportar_para_ficheiro(&tela, "exemplo.ppm")) {
    printf("Imagem guardada com sucesso em 'exemplo.ppm'!\n");
  } else {
    printf("Erro ao exportar o ficheiro.\n");
  }
  destruir_tela(&tela);
  return 0;
}
