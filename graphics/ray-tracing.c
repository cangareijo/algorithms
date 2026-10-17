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

Escreve um artigo longo a explicar o programa.
Escreve em português europeu.
Escreve em formato TXT.
Escreve em Unicode.

*/

/*

================================================================================
1. INTRODUÇÃO
================================================================================

O desenvolvimento de gráficos por computador tem sido historicamente impulsionado 
pela busca pelo realismo visual. Entre as diversas técnicas disponíveis, o Ray Tracing 
(Traçado de Raios) destaca-se como o algoritmo mais intuitivo e fisicamente fiel para 
simular o comportamento da luz. Embora tenha sido considerado proibitivamente dispendioso 
em termos computacionais no passado, a evolução do hardware e das linguagens de programação 
tornou esta técnica acessível e omnipresente, desde os videojogos modernos até ao cinema.

Paralelamente, a linguagem C passou recentemente por uma das suas atualizações mais 
profundas e significativas: o padrão C23. Longe de ser apenas uma revisão incremental, 
o C23 traz para a linguagem uma modernização há muito esperada, combinando a máxima 
performance e controlo ao nível do hardware com uma sintaxe mais limpa, segura e expressiva.

Este artigo técnico analisa detalhadamente uma implementação minimalista de um motor 
de Ray Tracing desenvolvido em C23. Ao longo das próximas secções, exploraremos a 
matemática subjacente ao traçado de raios, a arquitetura do código proposto e a forma 
como as novas funcionalidades do C23 foram aplicadas para criar um código elegante e eficiente.

================================================================================
2. FUNDAMENTOS DO RAY TRACING
================================================================================

O princípio fundamental do Ray Tracing inverte a física real para economizar recursos 
computacionais. Na natureza, as fontes de luz emitem triliões de fotões que ressaltam 
nos objetos até atingirem os nossos olhos. Simular todos estes fotões num computador seria 
impossível, pois a esmagadora maioria nunca chegaria à câmara.

No Ray Tracing, o processo é invertido:
1. Lançamos um raio primário a partir da posição da câmara através de cada pixel do ecrã virtual.
2. Determinamos se esse raio interseta algum objeto na cena tridimensional.
3. Se houver uma interseção, calculamos o ponto de impacto e a geometria local (a normal).
4. Lançamos raios secundários a partir desse ponto em direção às fontes de luz para determinar 
   se o ponto está na sombra ou iluminado.
5. Calculamos a cor final com base nas propriedades do material e da luz.

Este modelo simplificado permite simular sombras perfeitas, reflexos e refrações com uma 
fidelidade geométrica matemática absoluta.

================================================================================
3. AS INOVAÇÕES DO C23 NO CÓDIGO
================================================================================

O programa fornecido beneficia grandemente do padrão C23. Destacam-se três grandes inovações:

A. Inferência de Tipos com 'auto'
Inspirado por linguagens modernas (e pelo C++), o C23 introduz a palavra-chave 'auto'. 
No nosso código, isto é visível em atribuições como:
    auto oc = subtrair(raio.origem, esfera.centro);
    auto discriminante = (b * b) - (4.0 * a * c);
A inferência de tipos reduz o ruído visual do código, especialmente quando lidamos com 
ponteiros complexos ou tipos derivados, mantendo a tipagem forte e a verificação em tempo 
de compilação.

B. Inicialização Universal e Segura '{}'
Anteriormente, inicializar uma estrutura com zeros exigia sintaxes inconsistentes ou o uso 
de 'memset'. Com o C23, a inicialização universal por chavetas vazias '{}' garante que 
todos os campos de uma estrutura ou array são limpos de forma segura:
    if (comprimento == 0.0) return (Vetor3D){};
Isto previne bugs catastróficos causados por memória residual "lixo".

C. Atributos Standard: '[[nodiscard]]'
O C23 introduz uma sintaxe oficial para atributos. O uso de '[[nodiscard]]' nas funções de 
álgebra vetorial instrui o compilador a emitir um aviso (warning) caso o código chamador 
ignore o valor de retorno. Sendo funções puramente matemáticas, ignorar o seu retorno seria 
sempre um erro de lógica, tornando o desenvolvimento substancialmente mais seguro.

================================================================================
4. GEOMETRIA E MATEMÁTICA DA INTERSEÇÃO
================================================================================

O motor gráfico utiliza o conceito de vetores tridimensionais (Vetor3D) para todas as suas 
operações. A estrutura básica define uma coordenada (x, y, z), que serve tanto para posições 
no espaço como para direções ou cores (RGB).

A. Equação do Raio
Um raio é definido matematicamente como:
    P(t) = O + t * D
Onde:
- O é a origem do raio (posição da câmara).
- D é o vetor de direção normalizado (comprimento unitário).
- t é um escalar real positivo que representa a distância percorrida ao longo do raio.

B. Interseção com a Esfera
Uma esfera com centro C e raio R cumpre a equação:
    (P - C) · (P - C) = R^2
Substituindo a equação do raio na equação da esfera, obtemos uma equação quadrática na forma:
    a*t^2 + b*t + c = 0
Onde:
- a = D · D
- b = 2 * (O - C) · D
- c = (O - C) · (O - C) - R^2

O discriminante (b^2 - 4ac) determina o número de interseções. Se for negativo, o raio falha 
a esfera. Se for positivo, resolvemos a fórmula quadrática para encontrar o menor valor de 't' 
positivo, que representa o ponto de impacto visível mais próximo da câmara.

================================================================================
5. ILUMINAÇÃO, SOMBRAS E O CHÃO DE XADREZ
================================================================================

Assim que o ponto de impacto é determinado, entra em ação o modelo de iluminação.

A. Iluminação Difusa (Lambertiana)
A intensidade da luz numa superfície depende do ângulo entre o vetor normal da superfície 
(a direção perpendicular à superfície no ponto de impacto) e a direção da luz. Isto é calculado 
através do produto escalar (dot product). Quanto mais direto for o ângulo, mais iluminada 
estará a superfície. Adiciona-se uma pequena componente "ambiente" para que as zonas na 
sombra não fiquem totalmente pretas, simulando a dispersão global da luz.

B. O Desafio das Sombras
Para determinar se o chão está na sombra da esfera, o algoritmo lança um "raio de sombra". 
A sua origem é o ponto de impacto no chão e a sua direção aponta para a luz. Se este raio 
intersetar a esfera antes de chegar à luz, sabemos que o ponto está obstruído. O chão recebe 
então apenas a luz ambiente ténue, criando uma sombra projetada geometricamente perfeita.

C. O Chão de Xadrez Infinito
Para dar profundidade e perspetiva à cena, o chão simula um plano matemático infinito. Utilizando 
a função 'floor' nas coordenadas X e Z do ponto de impacto, criamos um padrão alternado baseado na 
paridade da soma das coordenadas inteiras:
    ((x_ajustado + z_ajustado) % 2 == 0)
Isto permite demonstrar o efeito de fuga tridimensional em direção ao horizonte sem gastar 
memória com texturas pesadas.

================================================================================
6. OUTPUT SEM DEPENDÊNCIAS: O FORMATO PPM
================================================================================

Para manter o projeto puramente educativo e evitar a complexidade de gerir bibliotecas gráficas 
específicas de cada sistema operativo (como OpenGL, DirectX ou bibliotecas de compressão PNG), 
o programa faz o output diretamente num ficheiro PPM (Portable Pixmap).

O formato PPM é um formato de imagem de texto plano extremamente simples. O cabeçalho consiste em:
- "P3" : Identificador do formato (ASCII RGB).
- "800 600" : Largura e altura em pixels.
- "255" : O valor máximo para cada componente de cor.

Seguem-se os valores inteiros de RGB (0 a 255) para cada pixel, ordenados da esquerda para a 
direita, de cima para baixo. Qualquer visualizador moderno ou sistema baseado em Unix consegue 
converter e abrir este ficheiro nativamente, permitindo focar toda a atenção no algoritmo 
core do Ray Tracing.

================================================================================
7. CONCLUSÃO
================================================================================

O Ray Tracing e o padrão C23 formam uma combinação poderosa. O C23 prova que a linguagem C 
continua viva, relevante e em constante modernização, oferecendo ferramentas que tornam o 
código mais limpo e legível sem abdicar do desempenho bruto. 

Este motor minimalista demonstra que, com menos de 200 linhas de código limpo e estruturado, 
é possível simular os princípios fundamentais da ótica física e da computação gráfica 3D. 
A partir deste alicerce robusto em C23, o leitor poderá expandir o programa para suportar 
múltiplas esferas, reflexos recursivos (esferas espelhadas), refração (vidro) e técnicas de 
anti-aliasing para suavizar as arestas dos objetos.

*/
