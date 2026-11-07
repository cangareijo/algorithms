#include <stddef.h>
#include <stdint.h>
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

[[nodiscard]] Tela criar_tela(const uint32_t largura, const uint32_t altura);
void destruir_tela(Tela *const tela);
void limpar_tela(Tela *const tela, const Cor fundo);
void desenhar_píxel(Tela *const tela, const Ponto2D ponto, const Cor cor);
void desenhar_linha(Tela *const tela, const Ponto2D início, const Ponto2D fim, const Cor cor);
void desenhar_retângulo_contorno(Tela *const tela, const Ponto2D superior_esquerdo, const uint32_t largura, const uint32_t altura, const Cor cor);
void desenhar_retângulo_preenchido(Tela *const tela, const Ponto2D superior_esquerdo, const uint32_t largura, const uint32_t altura, const Cor cor);
void desenhar_círculo_contorno(Tela *const tela, const Ponto2D centro, const uint32_t raio, const Cor cor);
void desenhar_círculo_preenchido(Tela *const tela, const Ponto2D centro, const uint32_t raio, const Cor cor);
[[nodiscard]] bool exportar_para_ficheiro(const Tela *const tela, const char *const caminho);

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
  if (ponto.x < 0 || ponto.y < 0 || ponto.x >= (int32_t)tela->largura || ponto.y >= (int32_t)tela->altura) return;
  tela->píxeis[ponto.x + ponto.y * tela->largura] = cor;
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

void desenhar_retângulo_contorno(Tela *const tela, const Ponto2D superior_esquerdo, const uint32_t largura, const uint32_t altura, const Cor cor) {
  if (tela == nullptr || tela->píxeis == nullptr || largura == 0 || altura == 0) return;
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

int main() {
  return 0;
}
