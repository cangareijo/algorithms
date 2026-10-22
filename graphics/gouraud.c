#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    double x, y, z;
} Vetor3;

typedef struct {
    double r, g, b;
} Cor;

static inline Vetor3 vetor_sub(Vetor3 a, Vetor3 b) {
    return (Vetor3){a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline double vetor_ponto(Vetor3 a, Vetor3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline double vetor_comprimento(Vetor3 v) {
    return sqrt(vetor_ponto(v, v));
}

static inline Vetor3 vetor_normalizar(Vetor3 v) {
    double len = vetor_comprimento(v);
    if (len < 1e-9) return (Vetor3){0.0, 0.0, 0.0};
    return (Vetor3){v.x / len, v.y / len, v.z / len};
}

typedef struct {
    Vetor3 posicao;
    Vetor3 normal;
    Cor cor_iluminada;
} Vertice;

typedef struct {
    int v0, v1, v2;
} Face;

static inline Cor calcular_iluminacao_gouraud(Vetor3 normal, Vetor3 dir_luz, Cor cor_objeto, Cor cor_luz, Cor luz_ambiente) {
    double dot = vetor_ponto(normal, dir_luz);
    double intensidade_difusa = dot > 0.0 ? dot : 0.0;

    Cor c;
    c.r = luz_ambiente.r * cor_objeto.r + intensidade_difusa * cor_luz.r * cor_objeto.r;
    c.g = luz_ambiente.g * cor_objeto.g + intensidade_difusa * cor_luz.g * cor_objeto.g;
    c.b = luz_ambiente.b * cor_objeto.b + intensidade_difusa * cor_luz.b * cor_objeto.b;

    if (c.r > 1.0) c.r = 1.0;
    if (c.g > 1.0) c.g = 1.0;
    if (c.b > 1.0) c.b = 1.0;

    return c;
}

static inline bool calcular_baricentricas(double px, double py, Vetor3 v0, Vetor3 v1, Vetor3 v2, double *w0, double *w1, double *w2) {
    double det = (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y);
    if (fabs(det) < 1e-9) return false;

    *w0 = ((v1.y - v2.y) * (px - v2.x) + (v2.x - v1.x) * (py - v2.y)) / det;
    *w1 = ((v2.y - v0.y) * (px - v2.x) + (v0.x - v2.x) * (py - v2.y)) / det;
    *w2 = 1.0 - *w0 - *w1;

    return (*w0 >= 0.0) && (*w1 >= 0.0) && (*w2 >= 0.0);
}

static inline Vetor3 projetar(Vetor3 v, int larg, int alt) {
    double escala = 2.4;
    double screen_x = ((v.x / escala) + 0.5) * larg;
    double screen_y = ((0.5 - v.y / escala)) * alt;
    return (Vetor3){screen_x, screen_y, v.z};
}

int main(void) {

    const int largura = 800;
    const int altura = 800;

    unsigned char *imagem = malloc(largura * altura * 3);
    if (imagem == nullptr) {
        fprintf(stderr, "Erro ao alocar memória para o buffer de imagem.\\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < largura * altura * 3; i++) {
        imagem[i] = 25;
    }

    const int paralelos = 40;
    const int meridianos = 40;
    const int num_vertices = (paralelos + 1) * (meridianos + 1);
    const int num_faces = paralelos * meridianos * 2;

    Vertice *vertices = malloc(num_vertices * sizeof(Vertice));
    Face *faces = malloc(num_faces * sizeof(Face));

    if (vertices == nullptr || faces == nullptr) {
        fprintf(stderr, "Erro ao alocar memória para a malha geométrica.\\n");
        free(imagem);
        free(vertices);
        free(faces);
        return EXIT_FAILURE;
    }

    double raio = 1.0;
    Vetor3 centro = {0.0, 0.0, 0.0};

    int v_idx = 0;
    for (int p = 0; p <= paralelos; p++) {
        double phi = M_PI * (double)p / paralelos;
        for (int m = 0; m <= meridianos; m++) {
            double theta = 2.0 * M_PI * (double)m / meridianos;

            double x = raio * sin(phi) * cos(theta);
            double y = raio * cos(phi);
            double z = raio * sin(phi) * sin(theta);

            vertices[v_idx].posicao = (Vetor3){x + centro.x, y + centro.y, z + centro.z};

            vertices[v_idx].normal = vetor_normalizar((Vetor3){x, y, z});
            v_idx++;
        }
    }

    int f_idx = 0;
    for (int p = 0; p < paralelos; p++) {
        for (int m = 0; m < meridianos; m++) {
            int atual = p * (meridianos + 1) + m;
            int seguinte = atual + 1;
            int baixo = (p + 1) * (meridianos + 1) + m;
            int baixo_seguinte = baixo + 1;

            faces[f_idx++] = (Face){atual, baixo, seguinte};
            faces[f_idx++] = (Face){seguinte, baixo, baixo_seguinte};
        }
    }

    Vetor3 dir_luz = vetor_normalizar((Vetor3){1.0, 1.0, 1.2});
    Cor cor_luz = {1.0, 0.95, 0.85};
    Cor luz_ambiente = {0.15, 0.15, 0.25};
    Cor cor_esfera = {0.1, 0.55, 0.85};

    for (int i = 0; i < num_vertices; i++) {
        vertices[i].cor_iluminada = calcular_iluminacao_gouraud(
            vertices[i].normal, dir_luz, cor_esfera, cor_luz, luz_ambiente
        );
    }

    for (int i = 0; i < num_faces; i++) {
        Face f = faces[i];
        Vetor3 p0 = projetar(vertices[f.v0].posicao, largura, altura);
        Vetor3 p1 = projetar(vertices[f.v1].posicao, largura, altura);
        Vetor3 p2 = projetar(vertices[f.v2].posicao, largura, altura);

        int min_x = (int)fmax(0.0, fmin(p0.x, fmin(p1.x, p2.x)));
        int max_x = (int)fmin((double)(largura - 1), fmax(p0.x, fmax(p1.x, p2.x)));
        int min_y = (int)fmax(0.0, fmin(p0.y, fmin(p1.y, p2.y)));
        int max_y = (int)fmin((double)(altura - 1), fmax(p0.y, fmax(p1.y, p2.y)));

        for (int y = min_y; y <= max_y; y++) {
            for (int x = min_x; x <= max_x; x++) {
                double w0, w1, w2;
                if (calcular_baricentricas((double)x, (double)y, p0, p1, p2, &w0, &w1, &w2)) {

                    Cor cor_pixel;
                    cor_pixel.r = w0 * vertices[f.v0].cor_iluminada.r + w1 * vertices[f.v1].cor_iluminada.r + w2 * vertices[f.v2].cor_iluminada.r;
                    cor_pixel.g = w0 * vertices[f.v0].cor_iluminada.g + w1 * vertices[f.v1].cor_iluminada.g + w2 * vertices[f.v2].cor_iluminada.g;
                    cor_pixel.b = w0 * vertices[f.v0].cor_iluminada.b + w1 * vertices[f.v1].cor_iluminada.b + w2 * vertices[f.v2].cor_iluminada.b;

                    int pixel_idx = (y * largura + x) * 3;

                    imagem[pixel_idx]     = (unsigned char)(cor_pixel.r * 255.0);
                    imagem[pixel_idx + 1] = (unsigned char)(cor_pixel.g * 255.0);
                    imagem[pixel_idx + 2] = (unsigned char)(cor_pixel.b * 255.0);
                }
            }
        }
    }

    FILE *ficheiro = fopen("esfera_gouraud.ppm", "wb");
    if (ficheiro != nullptr) {
        fprintf(ficheiro, "P6\n%d %d\n255\n", largura, altura);
        fwrite(imagem, 1, largura * altura * 3, ficheiro);
        fclose(ficheiro);
        printf("Sucesso! Imagem da esfera renderizada em 'esfera_gouraud.ppm'\n");
    } else {
        fprintf(stderr, "Erro ao gravar o ficheiro no disco.\n");
    }

    free(imagem);
    free(vertices);
    free(faces);

    return EXIT_SUCCESS;
}

/*

O Algoritmo de Gouraud Shading: Iluminação Suave na Computação Gráfica Clássica

A evolução da computação gráfica tridimensional é marcada por uma procura contínua pelo equilíbrio entre o realismo
visual e a eficiência computacional. Nos primórdios do desenvolvimento de software 3D, a representação de superfícies
curvas de forma convincente constituía um desafio técnico monumental. As geometrias eram, e continuam a ser,
representadas maioritariamente por polígonos planos — tipicamente triângulos. Sem um algoritmo de sombreamento
adequado, estas superfícies curvas pareciam blocos multifacetados e artificiais.

Em 1971, o cientista de computação francês Henri Gouraud publicou um método revolucionário que alterou profundamente
este cenário. O algoritmo, batizado como Gouraud Shading (ou Sombreamento de Gouraud), introduziu uma técnica simples,
elegante e incrivelmente rápida para simular superfícies contínuas e curvas a partir de malhas poligonais discretas.
Este artigo explora em detalhe o funcionamento, a matemática, as vantagens, as limitações e o legado histórico deste
pilar da computação gráfica.

O Problema da Face Plana: Flat Shading

Para compreender a inovação de Gouraud, é necessário olhar primeiro para a técnica utilizada na altura: o Flat
Shading (Sombreamento Plano). Numa abordagem de Flat Shading, cada polígono ou triângulo da malha 3D possui uma única
normal (um vetor perpendicular à sua superfície). O cálculo de iluminação é realizado apenas uma vez por face,
utilizando essa mesma normal.

Como resultado, todo o interior do triângulo recebe exatamente a mesma cor e intensidade luminosa. Quando múltiplos
triângulos se unem para formar um objeto curvo, como uma esfera ou um cilindro, a transição de cor entre um triângulo e
o seu vizinho é abrupta. O olho humano, altamente sensível a variações repentinas de contraste devido a um fenómeno
fisiológico conhecido como Bandas de Mach, exagera visualmente estas divisões, fazendo com que o objeto pareça facetado
e geométrico.

A solução intuitiva seria aumentar drasticamente o número de triângulos para tornar as transições impercetíveis. No
entanto, nos anos 70 (e mesmo nas décadas seguintes), a memória e o poder de processamento dos computadores eram
extremamente escassos. Era imperativo encontrar uma solução matemática que funcionasse ao nível do preenchimento dos
píxeis, sem sobrecarregar a contagem de polígonos.

A Filosofia do Gouraud Shading

Henri Gouraud percebeu que o segredo para enganar a perceção visual humana não residia na alteração da geometria do
objeto, mas sim na transição gradual das cores ao longo da sua superfície. A ideia central do Gouraud Shading assenta
num princípio fundamental: calcular a iluminação estritamente nos vértices dos polígonos e, em seguida, interpolar
linearmente essas cores ao longo do corpo de cada polígono durante a rasterização.

Em vez de tratar cada triângulo de forma isolada, o método de Gouraud une idealmente os polígonos adjacentes ao
partilhar a informação dos vértices comuns. O processo divide-se em três etapas lógicas principais:

1. Cálculo das normais dos vértices.

2. Cálculo da intensidade da cor em cada vértice (Modelo de Iluminação).

3. Interpolação linear das cores no interior do polígono (Rasterização).

Passo a Passo do Algoritmo

1. Cálculo das Normais dos Vértices

Numa malha de polígonos convencional, os triângulos partilham vértices entre si. Um único vértice pode ser a quina de
quatro, cinco ou mais triângulos adjacentes. Para calcular uma iluminação suave, precisamos de um vetor normal para
esse vértice que represente a curvatura teórica da superfície naquele ponto exato.

Para obter esta "normal média", o algoritmo recolhe as normais de todas as faces planas que partilham esse vértice e
realiza a sua média vetorial. Matematicamente, somam-se os vetores normais das faces adjacentes e, em seguida,
normaliza-se o vetor resultante (ajustando o seu comprimento para 1). Desta forma, a normal do vértice passa a apontar
na direção aproximada da superfície curva ideal que a malha está a tentar emular.

2. Cálculo da Iluminação nos Vértices

Com as normais dos vértices devidamente calculadas, aplica-se um modelo de reflexão luminosa — tradicionalmente o Modelo
de Reflexão de Lambert ou o Modelo de Phong (aplicado apenas ao vértice) — em cada um deles.

Tomando como exemplo a iluminação difusa de Lambert, a intensidade da luz num vértice depende do produto escalar entre a
normal do vértice e o vetor que aponta na direção da fonte luminosa. Se o vértice estiver virado diretamente para a
luz, receberá brilho máximo; se estiver de perfil ou de costas, ficará escuro. O resultado desta operação é uma cor
final específica (composta por componentes Vermelho, Verde e Azul) atribuída individualmente a cada vértice da malha.

3. Interpolação Linear de Cores

Esta é a fase onde a magia da rasterização acontece. Quando o motor gráfico projeta o triângulo 3D para o ecrã 2D, ele
conhece a posição dos três vértices no ecrã e as respetivas cores calculadas no passo anterior.

Ao preencher o triângulo linha por linha (um processo conhecido como scanline rendering), o algoritmo calcula a cor dos
píxeis nas bordas do triângulo através de uma interpolação linear entre os vértices superiores e inferiores. Depois,
para cada linha horizontal que compõe o interior do triângulo, o algoritmo faz uma nova interpolação linear horizontal
entre a borda esquerda e a borda direita.

Nas arquiteturas modernas de GPUs, esta operação é efetuada de forma extremamente eficiente através de Coordenadas
Baricêntricas. Cada píxel interno recebe uma ponderação baseada na sua proximidade em relação aos três vértices,
gerando um gradiente contínuo e suave que elimina completamente as divisões abruptas do Flat Shading.

Vantagens do Método

O Gouraud Shading tornou-se o padrão da indústria de jogos e simulações durante décadas devido a benefícios claros:

• Eficiência Computacional Extrema: O cálculo pesado de iluminação (envolvendo produtos escalares, potências e
  normalizações de vetores) é executado apenas três vezes por triângulo (uma vez por vértice). O preenchimento do resto
  da superfície é feito com interpolações matemáticas simples (adições e multiplicações básicas), que eram facilmente
  implementadas em hardware antigo.

• Melhoria Estética Imediata: Sem alterar uma única coordenada geométrica, objetos que antes pareciam construídos com
  blocos de cartão passam a ostentar uma aparência orgânica, polida e esférica.

• Simplicidade de Implementação: O algoritmo integra-se perfeitamente no pipeline clássico de rasterização, sem
  necessitar de estruturas de dados excessivamente complexas.

Limitações e Desvantagens

Apesar do seu sucesso histórico, o Gouraud Shading introduz aproximações matemáticas que geram artefactos visuais
percetíveis sob determinadas condições:

O Problema do Brilho Especular (Highlights)

A maior fraqueza do Gouraud Shading reside na renderização de materiais brilhantes ou metálicos que refletem pontos de
luz focados (brilhos especulares). Como a iluminação é avaliada exclusivamente nos vértices, se um triângulo for
relativamente grande e o ponto real de brilho máximo incidir exatamente no centro da sua face, nenhum dos vértices
captará esse reflexo intenso.

O resultado prático é que o brilho especular pode simplesmente desaparecer ou ficar severamente distorcido,
espalhando-se de forma bizarra em direção aos vértices. Se o objeto se mover, o brilho parecerá "saltar" erraticamente
de vértice em vértice, quebrando a ilusão de realismo.

Anisotropia da Perspetiva e Dependência da Malha

Dado que a interpolação é realizada no espaço de ecrã bidimensional durante a rasterização, a forma como uma malha é
dividida em triângulos pode afetar o padrão de sombreamento. Se rodarmos o mesmo objeto polidirecionalmente, os
gradientes de cor podem sofrer pequenas distorções internas porque as linhas de rasterização mudam de orientação em
relação aos ecrãs.

Gouraud Shading vs. Phong Shading

Para mitigar estas falhas, Bui Tuong Phong apresentou em 1973 o Phong Shading (Sombreamento de Phong). Embora partilhem
nomes parecidos devido à época e ao contexto académico, a abordagem é fundamentalmente diferente:

• No Gouraud Shading, interpolam-se as cores. A iluminação é calculada por vértice.

• No Phong Shading, interpolam-se as normais. As normais dos vértices são misturadas ao longo do triângulo para calcular
  uma normal única para cada píxel individual. O modelo de iluminação é então executado píxel a píxel.

O Phong Shading resolve perfeitamente o problema dos brilhos especulares, permitindo destaques perfeitamente definidos
no centro das faces poligonais. No entanto, durante os anos 80 e 90, o Phong Shading era considerado proibitivamente
lento para aplicações em tempo real, deixando o Gouraud Shading como o rei indiscutível dos videojogos 3D da quinta
geração (como os títulos da PlayStation 1 e Nintendo 64).

Legado Histórico e Atualidade

Com o advento das placas gráficas modernas e das linguagens de shading modernas (como HLSL e GLSL), o pipeline fixo que
ditava a escolha estrita entre Gouraud e Phong desapareceu. Atualmente, os Pixel Shaders (ou Fragment Shaders) correm
cálculos complexos por píxel de forma nativa e paralela a taxas de frames altíssimas, tornando o sombreamento por
píxel (semelhante ao conceito de Phong) a norma para quase todas as aplicações 3D em tempo real.

Contudo, o princípio básico do Gouraud Shading continua vivo. Sempre que declaramos atributos nos vértices (como cores
de vértice ou coordenadas de textura) e deixamos que a GPU os propague automaticamente para o interior dos fragmentos
através dos "interpoladores", estamos a usar o mecanismo fundamental que Henri Gouraud desenhou há mais de cinquenta
anos. O Sombreamento de Gouraud não foi apenas um truque matemático inteligente para o hardware do seu tempo; foi a
fundação que provou que a percepção e a iluminação inteligente podiam superar as barreiras da força bruta geométrica.

*/
