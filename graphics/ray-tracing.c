/*

Escreve uma demonstração de ray tracing em C23.
Escreve o código em português europeu.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Definições de vetores geométricas e cores
typedef struct {
    double x;
    double y;
    double z;
} Vetor3D;

typedef Vetor3D Cor;

// Estrutura para um Raio de luz: Origem + Direção
typedef struct {
    Vetor3D origem;
    Vetor3D direcao;
} Raio;

// Estrutura para uma Esfera na cena
typedef struct {
    Vetor3D centro;
    double raio;
    Cor cor;
} Esfera;

// Funções utilitárias para álgebra vetorial com atributos C23
[[nodiscard]] static inline Vetor3D somar(Vetor3D v1, Vetor3D v2) {
    return (Vetor3D){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

[[nodiscard]] static inline Vetor3D subtrair(Vetor3D v1, Vetor3D v2) {
    return (Vetor3D){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

[[nodiscard]] static inline Vetor3D multiplicar_escalar(Vetor3D v, double e) {
    return (Vetor3D){v.x * e, v.y * e, v.z * e};
}

[[nodiscard]] static inline Vetor3D multiplicar_cores(Cor c1, Cor c2) {
    return (Cor){c1.x * c2.x, c1.y * c2.y, c1.z * c2.z};
}

[[nodiscard]] static inline double produto_escalar(Vetor3D v1, Vetor3D v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

[[nodiscard]] static inline Vetor3D normalizar(Vetor3D v) {
    auto comprimento = sqrt(produto_escalar(v, v)); // C23: auto para inferência de tipo
    if (comprimento == 0.0) return (Vetor3D){};     // C23: inicialização universal vazia
    return multiplicar_escalar(v, 1.0 / comprimento);
}

// Verifica a interseção entre um raio e uma esfera
[[nodiscard]] static bool intersecao_esfera(Raio raio, Esfera esfera, double *t_saida) {
    auto oc = subtrair(raio.origem, esfera.centro);
    auto a = produto_escalar(raio.direcao, raio.direcao);
    auto b = 2.0 * produto_escalar(oc, raio.direcao);
    auto c = produto_escalar(oc, oc) - (esfera.raio * esfera.raio);
    auto discriminante = (b * b) - (4.0 * a * c);

    if (discriminante < 0.0) {
        return false;
    }

    auto t = (-b - sqrt(discriminante)) / (2.0 * a);
    if (t < 0.0) {
        t = (-b + sqrt(discriminante)) / (2.0 * a);
    }

    if (t > 0.0) {
        *t_saida = t;
        return true;
    }

    return false;
}

// Calcula a cor calculada para um determinado raio
[[nodiscard]] static Cor calcular_cor(Raio raio, Esfera esfera, Vetor3D posicao_luz, Cor cor_luz) {
    double t_esfera = 0.0;
    
    // Configuração do chão (um plano infinito simulado na coordenada Y fixa)
    auto altura_chao = -1.0;
    auto t_chao = (altura_chao - raio.origem.y) / raio.direcao.y;

    auto atingiu_esfera = intersecao_esfera(raio, esfera, &t_esfera);
    auto atingiu_chao = (t_chao > 0.001) && (raio.direcao.y < -0.001);

    // Determina qual o objeto visível mais próximo
    if (atingiu_esfera && (!atingiu_chao || t_esfera < t_chao)) {
        auto ponto_impacto = somar(raio.origem, multiplicar_escalar(raio.direcao, t_esfera));
        auto normal = normalizar(subtrair(ponto_impacto, esfera.centro));
        auto direcao_luz = normalizar(subtrair(posicao_luz, ponto_impacto));

        // Luz difusa (Lambertiana)
        auto intensidade_difusa = fmax(0.0, produto_escalar(normal, direcao_luz));
        
        // Sombra simples da esfera no seu próprio corpo
        auto cor_ambiente = multiplicar_escalar(esfera.cor, 0.1); 
        auto cor_difusa = multiplicar_escalar(multiplicar_cores(esfera.cor, cor_luz), intensidade_difusa);

        return somar(cor_ambiente, cor_difusa);
    } 
    
    if (atingiu_chao) {
        auto ponto_impacto = somar(raio.origem, multiplicar_escalar(raio.direcao, t_chao));
        
        // Padrão de xadrez para o chão
        auto x_ajustado = (int)(floor(ponto_impacto.x) + 1000.0);
        auto z_ajustado = (int)(floor(ponto_impacto.z) + 1000.0);
        auto cor_base = ((x_ajustado + z_ajustado) % 2 == 0) ? (Cor){0.8, 0.8, 0.8} : (Cor){0.4, 0.4, 0.4};

        auto normal = (Vetor3D){0.0, 1.0, 0.0};
        auto direcao_luz = normalizar(subtrair(posicao_luz, ponto_impacto));

        // Cálculo de sombra (lança um raio secundário do chão em direção à luz)
        Raio raio_sombra = { .origem = somar(ponto_impacto, multiplicar_escalar(normal, 0.001)), .direcao = direcao_luz };
        double t_sombra = 0.0;
        auto em_sombra = intersecao_esfera(raio_sombra, esfera, &t_sombra);

        if (em_sombra) {
            return multiplicar_escalar(cor_base, 0.1); // Apenas luz ambiente na sombra
        }

        auto intensidade_difusa = fmax(0.0, produto_escalar(normal, direcao_luz));
        return somar(multiplicar_escalar(cor_base, 0.1), multiplicar_escalar(multiplicar_cores(cor_base, cor_luz), intensidade_difusa));
    }

    // Cor de fundo (Gradiente do céu)
    auto t_ceu = 0.5 * (raio.direcao.y + 1.0);
    auto azul_ceu = (Cor){0.5, 0.7, 1.0};
    auto branco = (Cor){1.0, 1.0, 1.0};
    return somar(multiplicar_escalar(branco, 1.0 - t_ceu), multiplicar_escalar(azul_ceu, t_ceu));
}

int main(void) {
    // Resolução da imagem de saída
    const int largura = 800;
    const int altura = 600;

    // Definição dos elementos do cenário utilizando inicialização C23
    Esfera bola = { .centro = {0.0, 0.0, -5.0}, .raio = 1.0, .cor = {1.0, 0.2, 0.2} }; // Esfera vermelha
    Vetor3D ponto_luz = {5.0, 10.0, -2.0};
    Cor cor_luz = {1.0, 1.0, 1.0};
    Vetor3D camera = {}; // Origem na coordenada (0, 0, 0)

    // Criação do ficheiro PPM
    auto ficheiro = fopen("raytracing_c23.ppm", "wb");
    if (!ficheiro) {
        perror("Erro ao criar o ficheiro de imagem");
        return EXIT_FAILURE;
    }

    // Cabeçalho do formato PPM
    fprintf(ficheiro, "P3\n%d %d\n255\n", largura, altura);

    // Proporção do ecrã (Aspect Ratio)
    auto proporcao = (double)largura / (double)altura;

    // Loop de renderização (linha a linha, pixel a pixel)
    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            // Converte as coordenadas do pixel para o espaço tridimensional da câmara
            auto pixel_x = (2.0 * ((x + 0.5) / largura) - 1.0) * proporcao;
            auto pixel_y = 1.0 - 2.0 * ((y + 0.5) / altura);

            Vetor3D direcao_raio = normalizar((Vetor3D){pixel_x, pixel_y, -1.0});
            Raio raio_primario = { .origem = camera, .direcao = direcao_raio };

            Cor pixel_cor = calcular_cor(raio_primario, bola, ponto_luz, cor_luz);

            // Conversão de floats [0.0, 1.0] para bytes RGB [0, 255] com Tone Mapping simples
            int r = (int)(fmin(1.0, pixel_cor.x) * 255.0);
            int g = (int)(fmin(1.0, pixel_cor.y) * 255.0);
            int b = (int)(fmin(1.0, pixel_cor.z) * 255.0);

            fprintf(ficheiro, "%d %d %d\n", r, g, b);
        }
    }

    fclose(ficheiro);
    printf("Renderização concluída com sucesso! Imagem guardada em 'raytracing_c23.ppm'.\n");

    return EXIT_SUCCESS;
}

/*

gcc -lm -O3 -std=c23 raytracer.c -o raytracer

*/

/*

Escreve um artigo longo a explicar o algoritmo.
Escreve em português europeu.
Escreve em formato TXT.
Escreve em Unicode.

*/

/*

===============================================================================
O ESPELHO DA REALIDADE: DESVENDANDO O ALGORITMO DE RAY TRACING EM C23
===============================================================================

Introdução
----------
A procura pela fidedignidade visual na computação gráfica levou ao desenvolvimento
de diversas técnicas de renderização. Entre elas, o Ray Tracing (Lançamento de 
Raios) destaca-se como um dos algoritmos mais elegantes e intuitivos para simular
o comportamento físico da luz. 

Neste artigo, analisamos em detalhe a implementação de um motor de Ray Tracing 
básico, escrito em linguagem C moderna segundo o padrão C23. O algoritmo em causa
projeta uma cena tridimensional composta por uma esfera vermelha iluminada, um
chão com padrão em xadrez que projeta sombras e um fundo em gradiente que mimetiza
o céu azul. Tudo isto é processado de forma analítica e guardado diretamente num
ficheiro de imagem no formato raster PPM (Portable Pixmap).


1. A Filosofia do Ray Tracing: Luz Inversa
-------------------------------------------
No mundo real, as fontes de luz (como o Sol ou uma lâmpada) emitem triliões de
fotões que viajam pelo espaço, colidem com superfícies e, eventualmente, uma
fração minúscula atinge os nossos olhos ou o sensor de uma câmara. Simular este
processo diretamente num computador seria computacionalmente inviável, pois a
esmagadora maioria dos raios gerados nunca chegaria à câmara, desperdiçando
recursos preciosos.

O Ray Tracing resolve este problema invertendo o trajeto da luz. Em vez de lançar
raios a partir da fonte de luz, o algoritmo lança raios a partir do olho do
observador (a câmara) através de cada píxel do ecrã virtual para o interior da
cena. Se o raio colidir com um objeto, calcula-se a iluminação nesse ponto de 
impacto, determinando a cor final do píxel correspondente.


2. Estruturas de Dados e a Modernidade do C23
---------------------------------------------
O código adota representações geométricas limpas e conceitos da especificação
C23, que trazem maior legibilidade e segurança ao desenvolvimento em C.

* Vetor3D e Cor: No espaço tridimensional, pontos, direções e cores partilham a
  mesma natureza matemática de três componentes. O tipo 'Vetor3D' armazena as
  coordenadas (x, y, z), servindo também para definir a intensidade dos canais
  RGB (Vermelho, Verde, Azul) de uma 'Cor'.
* Raio: Definido por uma estrutura matemática rígida que contém uma 'origem' 
  (o ponto de partida no espaço) e uma 'direcao' (um vetor unitário que aponta 
  para onde o raio se propaga).
* Esfera: Caracterizada analiticamente por um ponto central ('centro'), um 
  comprimento numérico ('raio') e uma propriedade visual ('cor').

A modernização com o padrão C23 reflete-se na utilização do atributo
[[nodiscard]], que força o compilador a emitir um aviso se o valor de retorno 
de funções matemáticas cruciais for ignorado. Adicionalmente, o uso de 'auto' 
simplifica a inferência de tipos em variáveis locais (como no cálculo do
comprimento de vetores), e a inicialização universal 'return (Vetor3D){};' 
garante uma limpeza segura das estruturas para zero.


3. Álgebra Vetorial: Os Alicerces do Motor Gráfico
--------------------------------------------------
Toda a magia do Ray Tracing assenta na geometria analítica e na álgebra linear. 
O ficheiro implementa operações básicas de vetores através de funções 'inline'
para maximizar o desempenho:

* Soma e Subtração: Permitem deslocar pontos e encontrar vetores de direção.
* Multiplicação Escalar: Altera a magnitude de um vetor sem mudar a sua direção.
* Produto Escalar (Dot Product): É o coração do motor de iluminação. Retorna um
  escalar que representa a relação angular entre dois vetores. Se o produto
  escalar for máximo (igual a 1 para vetores normalizados), os vetores são 
  paralelos; se for 0, são perpendiculares.
* Normalização: Transforma qualquer vetor num vetor unitário (comprimento igual
  a 1.0). Isto é imperativo para que os cálculos de projeção e iluminação não
  sejam distorcidos pela escala do espaço.


4. Interseção Raio-Esfera: A Matemática do Impacto
--------------------------------------------------
Para descobrir se um raio atinge uma esfera, recorre-se à equação analítica da
geometria. Um ponto P num raio é definido por:
    P(t) = Origem + t * Direção, onde t > 0.

Uma esfera é definida pelo conjunto de pontos P que satisfazem:
    (P - Centro) . (P - Centro) = Raio^2

Substituindo a equação do raio na equação da esfera, obtemos uma equação do
segundo grau na forma tradicional: ax^2 + bx + c = 0, onde:
* a = Direção . Direção
* b = 2 * (Origem - Centro) . Direção
* c = (Origem - Centro) . (Origem - Centro) - Raio^2

A função 'intersecao_esfera' calcula o discriminante (Delta) desta equação:
1. Se Delta < 0: O raio falha completamente a esfera.
2. Se Delta >= 0: O raio atravessa ou toca a esfera.

Calcula-se a menor raiz positiva 't'. Se 't' for válido, encontramos a distância
exata da câmara até à superfície da esfera, permitindo mapear o ponto preciso de 
impacto no espaço tridimensional.


5. O Modelo de Iluminação e Sombras
------------------------------------
A função 'calcular_cor' é o centro nevrálgico do algoritmo. Ela decide a cor
de cada raio avaliando a proximidade dos objetos intersetados.

* O Chão Infinito: O chão é modelado como um plano horizontal perfeito numa 
  coordenada Y fixa. Para criar interesse visual, calcula-se a posição x e z
  do impacto. Arredondando estes valores e aplicando uma operação de módulo
  induz-se o padrão alternado de xadrez.
* Iluminação Difusa (Lambertiana): Quando um objeto é atingido, calcula-se o
  vetor 'Normal' (perpendicular à superfície) e o vetor que aponta para a luz.
  A intensidade da cor depende do cosseno do ângulo entre estes dois vetores,
  obtido através do produto escalar. Superfícies diretamente viradas para a luz
  ficam claras; superfícies tangenciais escurecem gradualmente, criando a ilusão
  perfeita de volume 3D.
* Luz Ambiente: Para evitar que as zonas não iluminadas fiquem totalmente
  negras e irreais, adiciona-se uma pequena percentagem de luz ambiente fixa,
  simulando a dispersão global da luz na atmosfera.
* Sombras Projetadas (Shadow Rays): O maior trunfo deste código é o realismo das
  sombras. Ao detetar que o raio primário atingiu o chão, o algoritmo dispara um
  raio secundário a partir do ponto de impacto em direção à fonte de luz. Se este
  raio de sombra intersetar a esfera antes de chegar à luz, significa que a esfera
  está a bloquear o Sol. O ponto do chão é, portanto, declarado em sombra, e a 
  sua iluminação difusa é anulada, restando apenas a luz ambiente ténue.


6. A Janela para o Mundo: A Câmara e a Renderização
---------------------------------------------------
A função 'main' orquestra a geração da imagem de 800x600 píxeis. A câmara está
posicionada estaticamente na origem (0, 0, 0) a olhar para o eixo Z negativo.

Para cada píxel da grelha bidimensional, o algoritmo normaliza as coordenadas
de ecrã para um intervalo entre -1.0 e 1.0, compensando a proporção de largura e 
altura (Aspect Ratio) para prevenir que a esfera fique oval ou deformada.

Após determinar a cor linear (valores entre 0.0 e 1.0) através do cálculo de
iluminação, o programa realiza um "Tone Mapping" elementar, limitando o teto
máximo com a função 'fmin' para evitar saturação, convertendo depois os valores
para a escala inteira padrão de 8 bits (0 a 255).


7. Saída de Dados: O Formato PPM
--------------------------------
Os dados resultantes são gravados sequencialmente num ficheiro chamado
'raytracing_c23.ppm'. O formato PPM (P3) é uma das formas mais simples de 
armazenamento de imagem: um cabeçalho de texto define o tipo de formato, as
dimensões da imagem e o valor máximo de cor, seguido por uma listagem direta
de tripletos inteiros representando o R, G e B de cada píxel. Isto elimina a
necessidade de incluir bibliotecas externas complexas de compressão (como as
de PNG ou JPEG), tornando o código totalmente autónomo e autocontido.


Conclusão
---------
Com menos de 200 linhas de código, este algoritmo ilustra com precisão o poder do
Ray Tracing. Ao unir conceitos geométricos fundamentais à eficiência pura da
linguagem C moderna (C23), o código demonstra como a simulação matemática das leis
da ótica consegue transformar números e equações abstratas numa imagem cénica de
alta fidelidade com luz, sombras e profundidade tangíveis.

*/
