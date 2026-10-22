#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
    double x, y, z;
} Vetor3;

static inline Vetor3 vet_sub(Vetor3 a, Vetor3 b) {
    return (Vetor3){a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline double vet_pdot(Vetor3 a, Vetor3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline double vet_norma(Vetor3 a) {
    return sqrt(vet_pdot(a, a));
}

static inline Vetor3 vet_normalizar(Vetor3 a) {
    double n = vet_norma(a);
    if (n < 1e-9) return (Vetor3){0, 0, 0};
    return (Vetor3){a.x / n, a.y / n, a.z / n};
}

typedef struct {
    Vetor3 centro;
    double raio;
    Vetor3 cor_albedo;
} Esfera;

typedef struct {
    Vetor3 posicao;
    Vetor3 cor;
} Luz;

constexpr int LARGURA = 800;
constexpr int ALTURA = 600;

int main(void) {

    constexpr Esfera esfera = {
        .centro = {0.0, 0.0, -5.0},
        .raio = 2.0,
        .cor_albedo = {1.0, 0.2, 0.2}
    };

    constexpr Luz luz = {
        .posicao = {5.0, 5.0, 0.0},
        .cor = {1.0, 1.0, 1.0}
    };

    constexpr Vetor3 luz_ambiente = {0.1, 0.1, 0.1};
    constexpr Vetor3 camara = {0.0, 0.0, 0.0};

    constexpr double k_a = 0.2;
    constexpr double k_d = 0.6;
    constexpr double k_s = 0.4;
    constexpr double brilho = 50.0;

    unsigned char *imagem = malloc(LARGURA * ALTURA * 3);
    if (imagem == nullptr) {
        perror("Erro ao alocar memória para o buffer da imagem");
        return EXIT_FAILURE;
    }

    const double proporcao = (double)LARGURA / ALTURA;
    constexpr double tamanho_fov = 1.0;

    for (int y = 0; y < ALTURA; ++y) {
        for (int x = 0; x < LARGURA; ++x) {

            double px = (2.0 * ((x + 0.5) / LARGURA) - 1.0) * proporcao * tamanho_fov;
            double py = (1.0 - 2.0 * ((y + 0.5) / ALTURA)) * tamanho_fov;

            Vetor3 dir_raio = vet_normalizar((Vetor3){px, py, -1.0});

            Vetor3 oc = vet_sub(camara, esfera.centro);
            double b = 2.0 * vet_pdot(oc, dir_raio);
            double c = vet_pdot(oc, oc) - esfera.raio * esfera.raio;
            double discriminante = b * b - 4.0 * c;

            Vetor3 cor_final = {0.0, 0.0, 0.0};

            if (discriminante >= 0.0) {
                double t = (-b - sqrt(discriminante)) / 2.0;
                if (t > 0.0) {

                    Vetor3 p_interseccao = {
                        camara.x + dir_raio.x * t,
                        camara.y + dir_raio.y * t,
                        camara.z + dir_raio.z * t
                    };
                    Vetor3 normal = vet_normalizar(vet_sub(p_interseccao, esfera.centro));

                    Vetor3 dir_luz = vet_normalizar(vet_sub(luz.posicao, p_interseccao));
                    Vetor3 dir_visao = vet_normalizar(vet_sub(camara, p_interseccao));

                    double n_dot_l = vet_pdot(normal, dir_luz);
                    Vetor3 dir_reflexao = {
                        2.0 * n_dot_l * normal.x - dir_luz.x,
                        2.0 * n_dot_l * normal.y - dir_luz.y,
                        2.0 * n_dot_l * normal.z - dir_luz.z
                    };
                    dir_reflexao = vet_normalizar(dir_reflexao);

                    Vetor3 ambiente = {
                        k_a * luz_ambiente.x * esfera.cor_albedo.x,
                        k_a * luz_ambiente.y * esfera.cor_albedo.y,
                        k_a * luz_ambiente.z * esfera.cor_albedo.z
                    };

                    double fator_difuso = fmax(n_dot_l, 0.0);
                    Vetor3 difusa = {
                        k_d * fator_difuso * luz.cor.x * esfera.cor_albedo.x,
                        k_d * fator_difuso * luz.cor.y * esfera.cor_albedo.y,
                        k_d * fator_difuso * luz.cor.z * esfera.cor_albedo.z
                    };

                    double r_dot_v = fmax(vet_pdot(dir_reflexao, dir_visao), 0.0);
                    double fator_especular = pow(r_dot_v, brilho);
                    Vetor3 especular = {
                        k_s * fator_especular * luz.cor.x,
                        k_s * fator_especular * luz.cor.y,
                        k_s * fator_especular * luz.cor.z
                    };

                    cor_final.x = ambiente.x + difusa.x + especular.x;
                    cor_final.y = ambiente.y + difusa.y + especular.y;
                    cor_final.z = ambiente.z + difusa.z + especular.z;
                }
            }

            int idx = (y * LARGURA + x) * 3;
            imagem[idx]     = (unsigned char)(fmin(fmax(cor_final.x, 0.0), 1.0) * 255.0);
            imagem[idx + 1] = (unsigned char)(fmin(fmax(cor_final.y, 0.0), 1.0) * 255.0);
            imagem[idx + 2] = (unsigned char)(fmin(fmax(cor_final.z, 0.0), 1.0) * 255.0);
        }
    }

    FILE *ficheiro = fopen("phong.ppm", "wb");
    if (ficheiro == nullptr) {
        perror("Erro ao criar o ficheiro de imagem");
        free(imagem);
        return EXIT_FAILURE;
    }

    fprintf(ficheiro, "P6\n%d %d\n255\n", LARGURA, ALTURA);
    fwrite(imagem, sizeof(unsigned char), LARGURA * ALTURA * 3, ficheiro);
    fclose(ficheiro);

    free(imagem);

    printf("Renderização concluída com sucesso! Imagem salva como 'phong.ppm'.\n");
    return EXIT_SUCCESS;
}

/*

O Modelo de Iluminação de Phong: Da Teoria Matemática aos Píxeis no Ecrã

No desenvolvimento de motores gráficos, simuladores e videojogos tridimensionais, um dos maiores desafios consiste em
converter dados puramente matemáticos — como coordenadas num espaço vetorial — em imagens digitais que o olho humano
consiga interpretar como objetos sólidos e realistas.

A forma como a luz interage com uma superfície determina a nossa perceção de profundidade, curvatura, textura e
material. Na computação gráfica tridimensional, um dos algoritmos fundamentais e mais elegantes para resolver este
problema é o Phong shading (e o respetivo Modelo de Reflexão de Phong), desenvolvido pelo investigador vietnamita Bui
Tuong Phong em 1973.

Este artigo explora o funcionamento detalhado deste algoritmo, decompondo a sua matemática subjacente, a lógica de
interpolação e a sua aplicação prática na renderização de superfícies suaves.

1. A Diferença Fundamental: Shading vs. Iluminação

Antes de avançar para a matemática, importa clarificar uma distinção técnica crucial que frequentemente gera confusão: a
diferença entre o Modelo de Reflexão (equação de iluminação) e o Método de Shading (técnica de interpolação). Bui Tuong
Phong introduziu contribuições para ambos, e os dois conceitos operam em conjunto:

• O Modelo de Reflexão de Phong: É a fórmula matemática que calcula a cor e a intensidade da luz num ponto específico da
  superfície, considerando a luz ambiente, a dispersão difusa e os destaques especulares (os brilhos reluzentes).

• O Phong Shading (Interpolação): É a técnica que dita onde e como essas equações de iluminação são calculadas ao longo
  de um polígono.

Para compreender o impacto do Phong shading, vale a pena compará-lo brevemente com os seus predecessores históricos:

Flat Shading (Sombreado Plano)

A equação de iluminação é calculada apenas uma vez por cada polígono (triângulo). Toda a face do triângulo recebe
exatamente a mesma cor. O resultado é um aspeto "facetado" e angular, onde as arestas geométricas entre os polígonos
são violentamente visíveis. É computacionalmente muito barato, mas inadequado para superfícies organicamente curvas.

Gouraud Shading (Sombreado por Vértices)

Criado por Henri Gouraud, este método calcula a equação de iluminação apenas nos vértices do triângulo. As cores
resultantes nos vértices são depois interpoladas linearmente ao longo dos píxeis da face do polígono. Embora suavize a
transição de cor, falha gravemente ao renderizar destaques especulares (brilhos de luz). Se o ponto de brilho máximo da
luz incidir no centro de um triângulo e não perto dos seus vértices, o Gouraud shading ignora por completo esse brilho,
fazendo com que a superfície pareça artificialmente baça ou que o brilho desapareça consoante o ângulo da câmara.

Phong Shading (Sombreado por Píxel)

O método de Phong inverte a lógica de Gouraud. Em vez de interpolar as cores já calculadas, o Phong shading interpola os
vetores normais dos vértices ao longo de toda a face do polígono. Isto significa que cada píxel individual dentro do
triângulo obtém o seu próprio vetor normal exclusivo, calculado com base na orientação dos vértices circundantes. A
equação de iluminação de Phong é então executada píxel a píxel. O resultado é a renderização perfeita de curvas suaves
e destaques especulares altamente realistas e dinâmicos.

2. A Anatomia do Modelo de Reflexão de Phong

A magia visual do modelo de Phong reside na divisão da luz em três componentes visuais distintas e independentes. Ao
somar estas três componentes, o algoritmo simula com enorme eficácia o comportamento de superfícies plásticas,
metálicas polidas ou baças.

A fórmula geral para a intensidade da luz (I) num determinado ponto é expressa pela soma das três componentes:

Intensidade Final = Luz Ambiente + Luz Difusa + Luz Especular

Para calcular estas componentes, precisamos de definir quatro vetores tridimensionais essenciais, todos normalizados (ou
seja, com comprimento/módulo igual a 1.0) e originados no ponto da superfície que estamos a processar:

1. Vetor Normal (N): O vetor perpendicular à superfície no ponto que está a ser renderizado. Indica para onde a
superfície está "virada".

2. Vetor da Luz (L): O vetor que aponta do ponto da superfície diretamente em direção à fonte de luz.

3. Vetor de Visão / Câmara (V): O vetor que aponta do ponto da superfície em direção ao olho do observador ou à câmara
virtual.

4. Vetor de Reflexão (R): A direção que um raio de luz perfeito tomaria ao fazer ricochete na superfície, seguindo a lei
da reflexão ótica (o ângulo de incidência é igual ao ângulo de reflexão).

Vejamos agora o papel de cada componente individualmente:

A Componente Ambiente (Ambient)

Na realidade, a luz faz ricochete em paredes, tetos e objetos, preenchendo indiretamente as áreas que não estão viradas
para uma fonte de luz direta. Uma sombra no mundo real raramente é um negrume absoluto.

Como calcular este fenómeno global de forma fisicamente exata seria computacionalmente impensável na década de 1970,
Phong simplificou o processo introduzindo uma constante de iluminação global. A componente ambiente assume que existe
uma quantidade fixa de luz dispersa uniformemente por toda a cena.

Ela não depende de vetores, de ângulos ou da posição da câmara. É simplesmente o produto entre a intensidade da luz
ambiente da cena e um coeficiente de reflexão ambiente (ka) do próprio material do objeto. Garante que mesmo as partes
totalmente escondidas da luz direta mantêm alguma cor e definição de silhueta.

A Componente Difusa (Diffuse)

A componente difusa modela a iluminação lambertiana, que é característica de superfícies rugosas ou mate (como gesso,
papel ou madeira). Quando a luz atinge este tipo de materiais, ela é refletida e espalhada igualmente em todas as
direções, independentemente da posição onde o observador se encontra.

No entanto, a quantidade de luz recebida pela superfície depende crucialmente do ângulo de incidência do raio de luz. Se
a luz atingir a superfície perpendicularmente (a direito), o fluxo de energia por unidade de área é máximo. À medida
que o ângulo entre a fonte de luz e a normal da superfície aumenta, a energia espalha-se por uma área maior, diminuindo
a intensidade observada.

Matematicamente, isto é modelado através do produto escalar (dot product) entre o vetor normal (N) e o vetor da luz (L).
O produto escalar de dois vetores normalizados devolve o cosseno do ângulo entre eles:

• Se os vetores forem paralelos (ângulo de 0 graus), o cosseno é 1.0 (iluminação máxima).

• Se os vetores forem perpendiculares (ângulo de 90 graus), o cosseno é 0.0 (nenhuma iluminação).

Caso a luz venha de trás da superfície, o produto escalar devolve um valor negativo. Graficamente, isto não faz
sentido (não existe luz negativa), pelo que o algoritmo utiliza uma função de limiar (como a função max em programação)
para garantir que qualquer valor inferior a zero é truncado para zero.

A Componente Especular (Specular)

Esta é a verdadeira assinatura do modelo de Phong. A componente especular modela os reflexos brilhantes e nítidos que
vemos em materiais polidos, vidros, plásticos brilhantes ou metais. Ao contrário da componente difusa, o brilho
especular é altamente dependente da posição do observador.

O fenómeno físico dita que uma superfície polida reflete a luz predominantemente numa direção ideal (o vetor R). Se o
olho do observador estiver diretamente alinhado com o vetor de reflexão R, ele verá um brilho ofuscante. À medida que o
observador se afasta desse eixo de reflexão, a intensidade do brilho diminui rapidamente.

Phong modelou esta dinâmica calculando o produto escalar entre o vetor de reflexão (R) e o vetor de visão (V).
Novamente, o resultado fornece o cosseno do ângulo entre a direção para onde a luz ressalta e a direção para onde o
observador olha.

Para controlar a dispersão deste brilho, Phong introduziu um expoente matemático conhecido como o expoente de
brilho (ou "shininess"). Elevando o produto escalar a esta potência (por exemplo, elevar a 10, 50 ou 200), conseguimos
simular diferentes materiais:

• Expoentes Baixos (ex: 5 a 20): Produzem um vislumbre amplo, suave e espalhado, simulando plásticos ligeiramente baços
  ou superfícies semipolidas.

• Expoentes Altos (ex: 100 a 1000): Produzem um ponto de brilho minúsculo, agudo e intensamente focado, característico
  de metais polidos, espelhos ou superfícies molhadas.

3. O Algoritmo de Interpolação no Pipeline Gráfico

Para compreender como o Phong shading opera em termos computacionais modernos (como nos Vertex Shaders e Fragment
Shaders de APIs gráficas como OpenGL, WebGL ou Direct3D), importa ver o fluxo de processamento de um triângulo:

1. Fase de Vértice: O modelo tridimensional envia os seus vértices para a placa gráfica. Cada vértice possui a sua
posição tridimensional e um vetor normal associado (geralmente calculado pela média das normais das faces que partilham
esse vértice).

2. Fase de Rasterização: A placa gráfica determina quais os píxeis do ecrã que estão cobertos pelo triângulo. Durante
esta divisão, o hardware interpola matematicamente os atributos dos vértices para cada píxel intermédio através de
coordenadas baricêntricas. No caso do Phong shading, o hardware faz uma interpolação linear dos vetores normais de cada
vértice.

3. Fase de Fragmento (Píxel): O fragment shader recebe o vetor normal interpolado para aquele píxel específico. Como a
interpolação linear direta de vetores unitários destrói ligeiramente o seu comprimento (o vetor resultante no centro
tende a ficar com um módulo ligeiramente inferior a 1.0), a primeira instrução obrigatória no processamento do píxel é
renormalizar o vetor normal. Com a normal restaurada para o comprimento de 1.0, todas as equações do Modelo de Reflexão
de Phong (Ambiente, Difusa e Especular) são calculadas individualmente para aquele píxel.

4. Vantagens e Limitações

Apesar de ter sido idealizado há várias décadas, o Phong shading continua a ser um marco de engenharia devido ao seu
excelente equilíbrio entre custo computacional e fidelidade visual.

Vantagens

• Ilusionismo Geométrico: Consegue transformar com sucesso malhas poligonais de baixa densidade (poucos triângulos) em
  objetos que aparentam ser perfeitamente curvos e orgânicos à vista.

• Eficiência: Baseia-se em operações de álgebra linear extremamente baratas para os processadores modernos, tais como
  produtos escalares, somas vetoriais e potências simples.

Limitações e Evolução

Sendo um modelo puramente empírico e fenomenológico — isto é, focado em "parecer correto" em vez de simular
rigorosamente as leis da física quântica e ótica —, o modelo clássico de Phong apresenta algumas falhas:

• Violação da Conservação de Energia: O modelo clássico permite que um objeto reflita mais energia do que aquela que
  recebe da fonte de luz, originando por vezes artefactos excessivamente claros em ângulos rasos.

• O Problema do Ângulo Obtuso: Se o ângulo entre o vetor da luz e a normal for superior a 90 graus, a componente difusa
  zera imediatamente, mas sob certas configurações matemáticas, a componente especular poderia continuar a tentar gerar
  brilho nas "costas" do objeto.

Para mitigar algumas destas falhas, Jim Blinn propôs mais tarde uma variação conhecida como o modelo Blinn-Phong. Em vez
de calcular o vetor de reflexão (R), que é computacionalmente dispendioso, Blinn introduziu o conceito de Vetor
Médio (Halfway Vector, H), que aponta exatamente a meio caminho entre o vetor da luz (L) e o vetor de visão (V). O
brilho especular passa a ser calculado pelo produto escalar entre a Normal (N) e este vetor Médio (H). O Blinn-Phong é
ainda mais veloz e oferece maior consistência física em ângulos de visão agudos.

Atualmente, a indústria de topo migrou maioritariamente para modelos de PBR (Physically Based Rendering), como o modelo
microfacetado de Cook-Torrance, que respeitam rigorosamente a conservação de energia e a rugosidade real dos materiais.
Contudo, o Phong shading permanece como a porta de entrada fundamental para o estudo da computação gráfica
tridimensional e uma escolha robusta para estilização e renderização eficiente.

*/
