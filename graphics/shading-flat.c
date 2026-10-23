#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Definição de estruturas básicas utilizando funcionalidades modernas de C23
typedef struct {
    double x;
    double y;
    double z;
} Vetor3;

typedef struct {
    Vetor3 centro;
    double raio;
} Esfera;

typedef struct {
    Vetor3 posicao;
    Vetor3 cor;
} Luz;

// Funções utilitárias para operações vetoriais
static inline Vetor3 subtrair(Vetor3 a, Vetor3 b) {
    return (Vetor3){a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline double produto_escalar(Vetor3 a, Vetor3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline Vetor3 normalizar(Vetor3 v) {
    double comprimento = sqrt(produto_escalar(v, v));
    if (comprimento == 0.0) return (Vetor3){0, 0, 0};
    return (Vetor3){v.x / comprimento, v.y / comprimento, v.z / comprimento};
}

// Interseção entre um raio e uma esfera
// Retorna a distância 't' da interseção mais próxima, ou -1.0 se não intersetar
static double intersetar_esfera(Vetor3 origem_raio, Vetor3 direcao_raio, Esfera esfera) {
    Vetor3 oc = subtrair(origem_raio, esfera.centro);
    double a = produto_escalar(direcao_raio, direcao_raio);
    double b = 2.0 * produto_escalar(oc, direcao_raio);
    double c = produto_escalar(oc, oc) - esfera.raio * esfera.raio;
    double discriminante = b * b - 4 * a * c;

    if (discriminante < 0) {
        return -1.0;
    }

    double t1 = (-b - sqrt(discriminante)) / (2.0 * a);
    if (t1 > 0.0) return t1;

    double t2 = (-b + sqrt(discriminante)) / (2.0 * a);
    if (t2 > 0.0) return t2;

    return -1.0;
}

// Função para aplicar o efeito de Flat Shading aproximado numa malha geométrica da esfera.
// Para garantir o aspeto clássico "facetado" (Flat Shading) sem criar uma malha complexa de triângulos,
// truncamos as coordenadas da normal do ponto de interseção para criar subdivisões discretas (faces).
static Vetor3 calcular_normal_flat(Vetor3 ponto_intersecao, Esfera esfera) {
    Vetor3 normal_suave = normalizar(subtrair(ponto_intersecao, esfera.centro));

    constexpr double pi = 3.14159265358979323846;
    // Nível de subdivisão para criar as faces planas (flat) da esfera
    constexpr double subdivisoes = 12.0;

    // Converte as coordenadas cartesianas para esféricas para discretizar os ângulos (faces)
    double phi = atan2(normal_suave.z, normal_suave.x);
    double theta = acos(normal_suave.y);

    // Discretização (quantização) dos ângulos para simular as facetas planas
    phi = round(phi * (subdivisoes / pi)) * (pi / subdivisoes);
    theta = round(theta * (subdivisoes / pi)) * (pi / subdivisoes);

    // Reconverte de volta para uma normal única por face
    Vetor3 normal_flat = {
        .x = sin(theta) * cos(phi),
        .y = cos(theta),
        .z = sin(theta) * sin(phi)
    };

    return normalizar(normal_flat);
}

int main(void) {
    // Configurações da imagem de saída utilizando diretivas e constantes C23
    constexpr int largura = 800;
    constexpr int altura = 800;

    // Inicialização de objetos usando a sintaxe limpa de C23
    auto esfera = (Esfera){ .centro = {0.0, 0.0, -5.0}, .raio = 2.0 };
    auto luz = (Luz){ .posicao = {5.0, 5.0, 0.0}, .cor = {1.0, 1.0, 1.0} };

    Vetor3 cor_esfera = {0.2, 0.6, 1.0}; // Azul claro
    Vetor3 cor_fundo = {0.1, 0.1, 0.15};  // Escuro azulado

    // Alocação dinâmica para demonstrar que o -fsanitize=leak está ativo e validado
    unsigned char* imagem = malloc(largura * altura * 3);
    if (imagem == nullptr) { // Uso do novo 'nullptr' introduzido no C23
        fprintf(stderr, "Erro ao alocar memória para a imagem.\n");
        return EXIT_FAILURE;
    }

    Vetor3 origem_camara = {0.0, 0.0, 0.0};

    // Renderização da cena (Ray Casting)
    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            // Mapeia os píxeis do ecrã para o plano de projeção tridimensional
            double px = (2.0 * ((double)x + 0.5) / largura - 1.0) * ((double)largura / altura);
            double py = -(2.0 * ((double)y + 0.5) / altura - 1.0);

            Vetor3 direcao_raio = normalizar((Vetor3){px, py, -1.0});
            double t = intersetar_esfera(origem_camara, direcao_raio, esfera);

            int pixel_index = (y * largura + x) * 3;

            if (t > 0.0) {
                // Ponto exato onde o raio toca a superfície da esfera
                Vetor3 ponto_intersecao = {
                    origem_camara.x + direcao_raio.x * t,
                    origem_camara.y + direcao_raio.y * t,
                    origem_camara.z + direcao_raio.z * t
                };

                // Calcula a normal facetada (Flat) correspondente a essa região
                Vetor3 normal = calcular_normal_flat(ponto_intersecao, esfera);
                Vetor3 direcao_luz = normalizar(subtrair(luz.posicao, ponto_intersecao));

                // Equação de sombreamento Lambertiano (Difuso) para o Flat Shading
                double intensidade_difusa = produto_escalar(normal, direcao_luz);
                if (intensidade_difusa < 0.0) intensidade_difusa = 0.0;

                // Intensidade ambiente para evitar sombras totalmente pretas
                constexpr double intensidade_ambiente = 0.15;
                double iluminacao_final = intensidade_difusa + intensidade_ambiente;
                if (iluminacao_final > 1.0) iluminacao_final = 1.0;

                // Aplica a iluminação calculada à cor do objeto
                imagem[pixel_index]     = (unsigned char)(cor_esfera.x * iluminacao_final * 255.0);
                imagem[pixel_index + 1] = (unsigned char)(cor_esfera.y * iluminacao_final * 255.0);
                imagem[pixel_index + 2] = (unsigned char)(cor_esfera.z * iluminacao_final * 255.0);
            } else {
                // Desenha a cor de fundo caso não haja interseção
                imagem[pixel_index]     = (unsigned char)(cor_fundo.x * 255.0);
                imagem[pixel_index + 1] = (unsigned char)(cor_fundo.y * 255.0);
                imagem[pixel_index + 2] = (unsigned char)(cor_fundo.z * 255.0);
            }
        }
    }

    // Gravação do ficheiro PPM final
    FILE* ficheiro = fopen("esfera.ppm", "wb");
    if (ficheiro == nullptr) {
        fprintf(stderr, "Erro ao criar o ficheiro de saída.\n");
        free(imagem);
        return EXIT_FAILURE;
    }

    // Cabeçalho obrigatório do formato PPM P6
    fprintf(ficheiro, "P6\n%d %d\n255\n", largura, altura);
    fwrite(imagem, sizeof(unsigned char), largura * altura * 3, ficheiro);
    fclose(ficheiro);

    printf("Demos de flat shading gerada com sucesso em 'esfera.ppm'!\n");

    // Libertação explícita de memória para evitar alertas do LeakSanitizer
    free(imagem);
    return EXIT_SUCCESS;
}

/*

Compreender o Flat Shading: O Fundamento do Sombreamento Tridimensional

No universo da computação gráfica 3D, a representação de objetos tridimensionais num ecrã bidimensional exige
transformações matemáticas complexas e uma compreensão profunda de como a luz interage com a matéria. Entre as diversas
técnicas desenvolvidas desde os primórdios da disciplina, o Flat Shading (ou sombreamento plano) destaca-se como o
método mais simples, computacionalmente económico e historicamente relevante para dar volume a formas geométricas.

Este artigo explora detalhadamente o funcionamento do Flat Shading, a sua fundamentação matemática, as suas vantagens e
limitações, e o seu impacto cultural na estética dos videojogos e das produções digitais.

O Desafio da Projeção e da Iluminação

Para compreender a utilidade do Flat Shading, imagine um cubo ou uma esfera flutuando no espaço virtual. Sem iluminação,
se desenharmos este objeto pintando-o inteiramente com uma única cor sólida (como o azul), ele perderá toda a
tridimensionalidade aos olhos do observador. O cubo parecerá um hexágono plano e a esfera parecerá um círculo
bidimensional.

O cérebro humano depende das variações de luz e sombra (os gradientes e contrastes) para interpretar a profundidade, a
curvatura e a textura dos objetos. É aqui que entram os algoritmos de sombreamento. O seu papel é calcular a cor e a
intensidade luminosa de cada porção da superfície do objeto com base na posição das luzes na cena e na orientação da
própria superfície.

O que é o Flat Shading?

O Flat Shading é uma técnica de renderização onde uma única cor e intensidade de iluminação são calculadas para uma face
poligonal inteira. Independentemente do tamanho do polígono (que geralmente é um triângulo ou um quadrilátero), todos
os píxeis que compõem essa face recebem exatamente a mesma tonalidade.

Para que isto aconteça, o algoritmo avalia as propriedades físicas da face num único ponto geométrico — frequentemente o
centro do polígono (o baricentro) ou o seu primeiro vértice. O resultado desse cálculo é estendido de forma homogénea
por toda a superfície daquela faceta.

A Estética "Low Poly"

O resultado visual desta abordagem é um objeto com um aspeto vincadamente multifacetado, geométrico e angular. Se
aplicarmos o Flat Shading a uma superfície que deveria ser curva (como uma esfera ou um cilindro), a aproximação
poligonal torna-se imediatamente evidente. O objeto parecerá esculpido em blocos ou lapidado como um diamante.

Embora esta característica tenha sido inicialmente considerada uma limitação técnica devido ao hardware do século
passado, transformou-se ao longo dos anos numa escolha artística intencional. É a base da famosa estética Low
Poly (baixo número de polígonos), amplamente utilizada em videojogos modernos independentes, animações estilizadas e
design de interfaces.

A Matemática por Trás do Algoritmo

O cálculo do Flat Shading baseia-se num princípio fundamental da física ótica conhecido como a Lei de Cossenos de
Lambert (ou Reflexão Lambertiana). Este modelo assume que a superfície do objeto é perfeitamente difusa (opaca e rugosa
a nível microscópico), o que significa que ela reflete a luz de forma igual em todas as direções, independentemente da
posição da câmara do observador.

A intensidade da luz refletida por uma face depende exclusivamente do ângulo com que os raios luminosos atingem essa
mesma face.

1. O Vetor Normal da Face

O elemento mais crítico para o cálculo é o vetor normal. A normal é um vetor unitário (com comprimento igual a 1) que
aponta perpendicularmente para fora da superfície da face.

Num polígono plano determinado por três vértices (A, B e C), a normal da face pode ser facilmente encontrada calculando
o produto vetorial entre dois dos seus lados:

Normal = (B - A) × (C - A)

Após este cálculo, o vetor resultante é normalizado (dividido pelo seu próprio comprimento) para garantir que representa
apenas a direção geométrica da face.

2. O Vetor da Luz

O segundo elemento é o vetor de direção da luz, que aponta do ponto que está a ser sombreado (o centro da face) em
direção à fonte de luz. Se a luz for direcional (como o Sol, cujos raios são considerados paralelos), este vetor é
constante para toda a cena. Se for uma luz posicional (como uma lâmpada ou uma fogueira), o vetor deve ser recalculado
subtraindo a posição do centro da face da posição da luz.

3. O Produto Escalar (Dot Product)

A magia do sombreamento acontece através duma operação matemática linear simples: o produto escalar entre o vetor normal
da face (N) e o vetor da direção da luz (L).

Intensidade = N ⋅ L

O produto escalar de dois vetores unitários retorna o cosseno do ângulo entre eles:

• Se a luz incidir perpendicularmente sobre a face (ângulo de 0 graus), o produto escalar é 1.0. A face recebe a
  iluminação máxima.

• Se a luz incidir de forma oblíqua, o valor desce gradualmente em direção a 0.0.

• Se o valor for igual ou inferior a 0.0, significa que a luz está a atingir a face por trás ou de raspão. Neste caso, a
  iluminação difusa é definida como zero.

4. A Cor Final do Píxel

Para obter a cor que será escrita no ecrã, multiplicamos a cor base do objeto pela intensidade calculada, adicionando
frequentemente uma pequena componente de luz ambiente para garantir que as faces na sombra completa não fiquem
totalmente pretas e invisíveis:

Cor Final = (Cor do Objeto × Intensidade Difusa) + Luz Ambiente

Como esta fórmula é aplicada apenas uma vez por polígono, o processo é extremamente veloz.

Comparação Direta com Outras Técnicas de Sombreamento

Para contextualizar a simplicidade do Flat Shading, é útil compreender de que forma ele difere dos métodos que surgiram
posteriormente, concebidos especificamente para resolver o problema do aspeto facetado em superfícies que deveriam
parecer curvas.

No Flat Shading, como vimos, o cálculo de iluminação é feito uma única vez por polígono, utilizando a normal geométrica
da própria face. O resultado é uma cor uniforme em toda a extensão do polígono, gerando um custo computacional
extremamente baixo, mas com uma transição abrupta de tons nas arestas.

No Gouraud Shading (ou sombreamento por vértices), o algoritmo adota uma abordagem diferente. Em vez de calcular a
iluminação para a face, ele calcula os vetores normais em cada um dos vértices individuais (geralmente fazendo a média
das normais das faces que partilham esse vértice). A iluminação é calculada em cada vértice e as cores resultantes são
depois interpoladas linearmente ao longo da superfície do polígono. Isto suaviza o interior dos objetos e mascara as
divisões internas, embora a silhueta exterior do modelo continue a revelar a sua natureza poligonal. O seu custo
computacional é moderado.

No Phong Shading (ou sombreamento por píxel), a suavização é levada ao extremo. Em vez de interpolar as cores calculadas
nos vértices, o algoritmo interpola os próprios vetores normais através da superfície do polígono. Isto significa que a
fórmula de iluminação de Lambert (e outras componentes mais avançadas, como os reflexos brilhantes especulares) é
executada individualmente para cada píxel do ecrã. O aspeto visual final em superfícies curvas é perfeitamente suave e
realista, mas o custo computacional é substancialmente mais elevado quando comparado com os métodos anteriores.

Vantagens do Flat Shading

1. Desempenho Excecional

Por exigir apenas um cálculo de iluminação por polígono, o Flat Shading consome uma fração minúscula dos recursos do
processador (CPU) ou da placa gráfica (GPU). Nos primórdios da computação gráfica, nos anos 70 e 80, esta era a única
técnica viável para renderizar gráficos 3D em tempo real.

2. Clareza de Topologia

O Flat Shading expõe diretamente a estrutura geométrica do modelo (a sua malha de polígonos). Isto torna-o uma
ferramenta indispensável para artistas digitais e modeladores 3D durante a fase de criação de ativos, pois permite
identificar facilmente imperfeições na geometria, triângulos invertidos ou problemas de distribuição de polígonos.

3. Identidade Visual Distinta

Longe de ser um fóssil tecnológico, o Flat Shading goza de enorme popularidade no design contemporâneo. A sua estética
limpa, minimalista e geométrica evoca nostalgia, mas também transmite uma sensação de modernidade e sofisticação
artística quando combinada com paletas de cores bem selecionadas.

Limitações e Desvantagens

1. Falta de Realismo

O Flat Shading é incapaz de simular materiais do mundo real com precisão. Superfícies polidas, metálicas, molhadas ou
orgânicas perdem toda a sua credibilidade sob este algoritmo, uma vez que as transições abruptas de cor entre as faces
destroem a ilusão de continuidade física.

2. O Efeito das Bandas de Mach (Mach Bands)

O sistema visual humano possui um mecanismo de realce de contrastes conhecido como inibição lateral. Quando duas áreas
com tons ligeiramente diferentes de cinzento ou de cor partilham uma fronteira nítida, os nossos olhos tendem a
exagerar a diferença na vizinhança imediata dessa linha.

No Flat Shading, isto faz com que as arestas entre os polígonos pareçam ainda mais proeminentes e escuras/claras do que
realmente são matematicamente. Este fenómeno visual (as Bandas de Mach) torna impossível disfarçar as divisões das
facetas, mesmo que se aumente significativamente o número de polígonos do objeto.

O Impacto Histórico e a Cultura Pop

O Flat Shading foi o motor de arranque dos primeiros videojogos tridimensionais comerciais. Títulos pioneiros como "Star
Fox" (1993) na Super Nintendo, ou simuladores de voo clássicos da década de 80, dependiam totalmente desta técnica para
desenhar naves, montanhas e inimigos no ecrã a taxas de fotogramas aceitáveis.

Mesmo com a rápida transição para o Gouraud Shading e texturas mapeadas na era da PlayStation original e da Nintendo 64,
o Flat Shading nunca desapareceu. Hoje em dia, jogos aclamados como "Unturned", "Grow Home" ou "Superhot" apropriam-se
conscientemente desta limitação do passado, provando que o sombreamento plano transcendeu a sua natureza técnica
originária para se estabelecer como uma linguagem artística intemporal na computação gráfica.

*/
