#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Constantes C23 usando constexpr
constexpr int LARGURA_ECRA = 800;
constexpr int ALTURA_ECRA = 600;
constexpr int TAMANHO_TEXTURA = 256;
constexpr double PI = 3.14159265358979323846;

// Estruturas de dados utilitárias
typedef struct {
    double x, y;
} Vetor2D;

typedef struct {
    double x, y, z;
} Vetor3D;

// Cor em formato RGB
typedef struct {
    unsigned char r, g, b;
} Cor;

// Função para gerar uma textura de xadrez procedural com gradiente
Cor* gerar_textura_procedural() {
    auto textura = (Cor*)malloc(sizeof(Cor) * TAMANHO_TEXTURA * TAMANHO_TEXTURA);
    if (textura == nullptr) {
        return nullptr;
    }

    for (int y = 0; y < TAMANHO_TEXTURA; ++y) {
        for (int x = 0; x < TAMANHO_TEXTURA; ++x) {
            // Padrão de xadrez baseado em bits
            bool xadrez = ((x / 32) + (y / 32)) % 2 == 0;
            
            if (xadrez) {
                textura[y * TAMANHO_TEXTURA + x] = (Cor){ .r = 255, .g = 100, .b = 100 };
            } else {
                // Gradiente para tornar o visual mais dinâmico
                textura[y * TAMANHO_TEXTURA + x] = (Cor){ 
                    .r = (unsigned char)x, 
                    .g = (unsigned char)y, 
                    .b = 205 
                };
            }
        }
    }
    return textura;
}

// Guarda o ecrã virtual num ficheiro PPM (P6)
bool guardar_imagem_ppm(const char* nome_ficheiro, const Cor* buffer) {
    auto ficheiro = fopen(nome_ficheiro, "wb");
    if (ficheiro == nullptr) {
        return false;
    }

    // Cabeçalho do formato PPM
    fprintf(ficheiro, "P6\n%d %d\n255\n", LARGURA_ECRA, ALTURA_ECRA);
    
    // Escrita dos píxeis
    fwrite(buffer, sizeof(Cor), LARGURA_ECRA * ALTURA_ECRA, ficheiro);
    fclose(ficheiro);
    return true;
}

int main(void) {
    // Alocação dinâmica do buffer do ecrã para evitar overflow da stack
    auto ecr_buffer = (Cor*)calloc(LARGURA_ECRA * ALTURA_ECRA, sizeof(Cor));
    if (ecr_buffer == nullptr) {
        return EXIT_FAILURE;
    }

    auto textura = gerar_textura_procedural();
    if (textura == nullptr) {
        free(ecr_buffer);
        return EXIT_FAILURE;
    }

    // Definição de um quadrilátero 3D com coordenadas de textura (U, V)
    // Ângulo de rotação para dar efeito tridimensional
    constexpr double angulo = 45.0 * PI / 180.0;
    
    // Quatro vértices projetados manualmente com uma perspetiva simples
    Vetor3D vertices_3d[4] = {
        { -2.0, -2.0, 5.0 + sin(angulo) * 2.0 },
        {  2.0, -2.0, 5.0 - sin(angulo) * 2.0 },
        {  2.0,  2.0, 5.0 - sin(angulo) * 2.0 },
        { -2.0,  2.0, 5.0 + sin(angulo) * 2.0 }
    };

    Vetor2D uv[4] = {
        { 0.0, 0.0 },
        { 1.0, 0.0 },
        { 1.0, 1.0 },
        { 0.0, 1.0 }
    };

    Vetor2D ecra_pts[4];
    constexpr double fov = 600.0; // Distância focal simulada

    for (int i = 0; i < 4; ++i) {
        // Projeção perspetiva básica: X_ecra = (X_3d * fov) / Z_3d + Centro
        ecra_pts[i].x = (vertices_3d[i].x * fov) / vertices_3d[i].z + (LARGURA_ECRA / 2.0);
        ecra_pts[i].y = (vertices_3d[i].y * fov) / vertices_3d[i].z + (ALTURA_ECRA / 2.0);
    }

    // Rasterização simplificada por mapeamento inverso (Inverse Texture Mapping)
    // Para cada píxel do ecrã, calculamos a coordenada correspondente na textura
    for (int y = 0; y < ALTURA_ECRA; ++y) {
        for (int x = 0; x < LARGURA_ECRA; ++x) {
            
            // Fundo gradiente escuro por omissão
            ecr_buffer[y * LARGURA_ECRA + x] = (Cor){ .r = 15, .g = 15, .b = 25 };

            // Interpolação bilinear através de coordenadas bilineares locais (Mapeamento Inverso)
            // Abordagem simplificada: Ray-casting contra o plano delimitado pelos 4 pontos projetados
            // Para manter o código limpo e sem dependências, usamos uma aproximação de bounding box para o rendering
            
            // Vamos testar se o ponto está dentro do quadrilátero usando interpolação bilinear invertida
            // Para esta demonstração matemática limpa, resolvemos o mapeamento de perspetiva correto:
            double px = x;
            double py = y;

            // Algoritmo de varrimento e interpolação por sub-triângulos (Triângulo 0-1-2 e 0-2-3)
            for (int t = 0; t < 2; ++t) {
                int i0 = 0, i1 = (t == 0) ? 1 : 2, i2 = (t == 0) ? 2 : 3;
                
                double det = (ecra_pts[i1].y - ecra_pts[i2].y) * (ecra_pts[i0].x - ecra_pts[i2].x) + 
                             (ecra_pts[i2].x - ecra_pts[i1].x) * (ecra_pts[i0].y - ecra_pts[i2].y);
                
                if (fabs(det) < 1e-5) continue;

                double l1 = ((ecra_pts[i1].y - ecra_pts[i2].y) * (px - ecra_pts[i2].x) + 
                             (ecra_pts[i2].x - ecra_pts[i1].x) * (py - ecra_pts[i2].y)) / det;
                double l2 = ((ecra_pts[i2].y - ecra_pts[i0].y) * (px - ecra_pts[i2].x) + 
                             (ecra_pts[i0].x - ecra_pts[i2].x) * (py - ecra_pts[i2].y)) / det;
                double l3 = 1.0 - l1 - l2;

                if (l1 >= 0.0 && l2 >= 0.0 && l3 >= 0.0) {
                    // Interpolação com correção de perspetiva (W-buffering / Z-correcto)
                    double w0 = 1.0 / vertices_3d[i0].z;
                    double w1 = 1.0 / vertices_3d[i1].z;
                    double w2 = 1.0 / vertices_3d[i2].z;

                    double interp_w = l1 * w0 + l2 * w1 + l3 * w2;
                    
                    double u = (l1 * uv[i0].x * w0 + l2 * uv[i1].x * w1 + l3 * uv[i2].x * w2) / interp_w;
                    double v = (l1 * uv[i0].y * w0 + l2 * uv[i1].y * w1 + l3 * uv[i2].y * w2) / interp_w;

                    // Mapeamento inverso para coordenadas de píxel da textura (Nearest Neighbor sampling)
                    int tx = (int)(u * (TAMANHO_TEXTURA - 1));
                    int ty = (int)(v * (TAMANHO_TEXTURA - 1));

                    // Clamping de segurança
                    if (tx < 0) tx = 0;
                    if (tx >= TAMANHO_TEXTURA) tx = TAMANHO_TEXTURA - 1;
                    if (ty < 0) ty = 0;
                    if (ty >= TAMANHO_TEXTURA) ty = TAMANHO_TEXTURA - 1;

                    ecr_buffer[y * LARGURA_ECRA + x] = textura[ty * TAMANHO_TEXTURA + tx];
                    break;
                }
            }
        }
    }

    // Guardar a renderização num ficheiro de imagem
    if (guardar_imagem_ppm("textura_mapeada.ppm", ecr_buffer)) {
        printf("Demonstração executada com sucesso! Imagem gravada em 'textura_mapeada.ppm'.\n");
    } else {
        fprintf(stderr, "Erro ao gravar o ficheiro de saída.\n");
    }

    // Libertação de memória (evita memory leaks testados pelo AddressSanitizer)
    free(textura);
    free(ecr_buffer);

    return EXIT_SUCCESS;
}

/*

O Guia Definitivo Sobre Texture Mapping: Da Geometria Plana à Ilusão Tridimensional

No desenvolvimento de gráficos por computador, a geometria pura e os modelos matemáticos tridimensionais são
fundamentais, mas têm uma limitação óbvia: são visualmente estéreis. Um cubo perfeito, por mais polido que esteja e por
mais complexos que sejam os algoritmos de iluminação aplicados sobre as suas faces, continuará a parecer um bloco
artificial de plástico ou metal monocromático.

Para que os mundos virtuais transmitam a complexidade do mundo real — as ranhuras da madeira, a rugosidade do betão, as
imperfeições da pele humana ou os detalhes de um tecido — os pioneiros da computação gráfica perceberam que precisavam
de uma abordagem diferente. Em vez de aumentar infinitamente o número de polígonos para modelar cada minúsculo detalhe,
nasceu o conceito de Texture Mapping (Mapeamento de Texturas).

Este artigo didático explora a fundo a teoria, a matemática e os desafios técnicos por trás desta técnica
revolucionária, abordando desde os seus fundamentos geométricos até à amostragem digital e à correção de perspetiva.

1. O Conceito Fundamental e a Analogia do Papel de Embrulho

Desenvolvido originalmente por Edwin Catmull em 1974, o mapeamento de texturas pode ser idealizado através de uma
analogia simples do quotidiano: embrulhar um presente. Imagine uma caixa de sapatos tridimensional que representa o
nosso objeto geométrico e uma folha de papel de embrulho decorada que representa a nossa imagem bidimensional (a
textura). O processo de mapeamento consiste em esticar, rodar e colar essa folha de papel sobre as superfícies da
caixa.

Na computação gráfica, a imagem bidimensional que serve de padrão é chamada de textura, e os seus elementos individuais
de píxel não se chamam píxeis, mas sim téxeis (texture elements), para evitar confusões com os píxeis do ecrã final.

O grande trunfo desta técnica é a eficiência computacional. Adicionar detalhe visual através da geometria exige criar
milhões de novos polígonos, o que sobrecarrega severamente o processador (CPU ou GPU) no cálculo de transformações e
colisões. Adicionar detalhe através de texturas transfere essa carga para a memória de vídeo e para algoritmos de
amostragem digital, permitindo criar um realismo impressionante com um custo computacional drasticamente inferior.

2. O Espaço de Coordenadas UV

Para colar uma imagem bidimensional num objeto tridimensional, precisamos de um sistema de coordenadas que estabeleça
uma ponte entre estes dois mundos distintos. Esse sistema é conhecido como o Espaço de Coordenadas UV.

Enquanto os objetos no espaço tridimensional utilizam os eixos X, Y e Z, e o ecrã bidimensional utiliza os eixos X e Y,
a textura utiliza os eixos U (horizontal) e V (vertical).

Por convenção e para garantir a independência da resolução da imagem, as coordenadas U e V são normalizadas. Isto
significa que os seus valores variam estritamente entre 0.0 e 1.0, independentemente de a textura ter uma resolução de
256 por 256 téxeis ou de 4096 por 4096 téxeis:

• O ponto (0.0, 0.0) representa, habitualmente, o canto inferior esquerdo (ou superior esquerdo, dependendo da API
  gráfica como OpenGL ou DirectX) da imagem da textura.

• O ponto (1.0, 1.0) representa o canto oposto na diagonal da imagem.

Durante a fase de modelação 3D, o artista atribui a cada vértice de um polígono uma coordenada UV correspondente. Por
exemplo, num triângulo tridimensional, o Vértice 1 pode receber a coordenada U = 0.0, V = 0.0; o Vértice 2 recebe U =
1.0, V = 0.0; e o Vértice 3 recebe U = 0.5, V = 1.0. Quando o triângulo for desenhado no ecrã, o computador saberá
exatamente como recortar e distorcer a imagem digital para que ela se ajuste perfeitamente àquela geometria.

3. Rasterização e Mapeamento Inverso

Quando o motor gráfico se prepara para desenhar um polígono no ecrã, ele passa pelo processo de rasterização, que
converte a geometria matemática contínua em píxeis discretos na matriz do ecrã. Existem duas abordagens teóricas para
mapear os téxeis durante este processo:

Mapeamento Direto (Forward Mapping)

Nesta abordagem, o algoritmo percorre cada téxel da textura na memória e calcula em que posição do ecrã ele deve ser
desenhado. Embora pareça intuitivo, este método é altamente ineficiente e problemático. Como a geometria pode estar
muito distante ou distorcida pela perspetiva, múltiplos téxeis podem convergir para o mesmo píxel do ecrã (causando
desperdício de processamento) ou, pior, podem surgir "buracos" vazios no ecrã onde nenhum téxel foi mapeado.

Mapeamento Inverso (Inverse Mapping)

Esta é a abordagem padrão utilizada por placas gráficas e motores de renderização por software. Em vez de
perguntar "onde é que este téxel vai parar no ecrã?", o algoritmo percorre cada píxel que compõe o polígono projetado
no ecrã e pergunta: "qual é o téxel correspondente na textura para este píxel exato?".

Através de interpolação matemática, o sistema calcula o valor de U e V para a posição atual do píxel e acede à memória
da textura para ler a cor do téxel correspondente. Isto garante que nenhum píxel do ecrã fique por pintar e que o
processamento seja feito apenas para os píxeis visíveis.

4. A Armadilha da Perspetiva e a Interpolação Correta

O aspeto mais crítico e matematicamente refinado do texture mapping reside na forma como os valores de U e V são
interpolados ao longo do polígono.

Mapeamento Afim (Affine Texture Mapping)

Nas primeiras gerações de consolas de videojogos 3D (como a PlayStation 1), o mapeamento de texturas usava a
interpolação linear simples no espaço do ecrã. Dadas as coordenadas UV nos vértices de uma linha, o valor no meio da
linha era simplesmente a média aritmética das coordenadas dos extremos.

Esta técnica funciona perfeitamente para gráficos bidimensionais ou superfícies paralelas ao ecrã. Contudo, quando uma
superfície se inclina em direção ao horizonte tridimensional, o mapeamento afim falha redondamente. Como a perspetiva
faz com que os objetos distantes pareçam mais pequenos, os téxeis mais distantes deveriam aparecer mais densos e
comprimidos do que os téxeis mais próximos. A interpolação linear ignora este facto, fazendo com que a textura
pareça "ondular", "entortar" ou mover-se de forma gelatinosa conforme a câmara se desloca.

Mapeamento com Correção de Perspetiva (Perspective-Correct Mapping)

Para resolver este problema, a matemática deve ter em conta a profundidade, tradicionalmente representada pela
coordenada Z (ou o seu inverso, W = 1 / Z). A perspetiva dita que a projeção no ecrã é inversamente proporcional à
distância do objeto à câmara.

A solução correta passa por não interpolar as coordenadas U e V diretamente. Em vez disso, dividem-se as coordenadas UV
pela profundidade Z em cada vértice, resultando nos termos (U / Z) e (V / Z). Adicionalmente, calcula-se o inverso da
profundidade autónomo, ou seja, (1 / Z).

Durante a rasterização, o hardware faz a interpolação linear destes três novos valores transformados: (U / Z), (V / Z)
e (1 / Z). No momento exato de pintar um píxel específico, o computador calcula o valor corrigido de U e V dividindo as
frações interpoladas pelo inverso da profundidade interpolada. A equação matemática traduz-se da seguinte forma:

U corrigido = (U / Z) interpolado / (1 / Z) interpolado

V corrigido = (V / Z) interpolado / (1 / Z) interpolado

Graças a esta operação de divisão por píxel, a textura mantém-se perfeitamente linear e geometricamente coerente no
espaço tridimensional, eliminando por completo as distorções visuais do mapeamento afim.

5. Amostragem e Filtragem de Texturas

Raras são as vezes em que a resolução de um polígono no ecrã coincide exatamente com o tamanho da textura na memória.
Quando há uma disparidade de escala, ocorrem artefactos visuais que exigem técnicas conhecidas como filtragem de
texturas.

Magnificação: Nearest Neighbor vs. Filtragem Bilinear

A magnificação ocorre quando a câmara está extremamente próxima de um objeto, fazendo com que um único téxel ocupe
múltiplos píxeis no ecrã.

• Nearest Neighbor (Vizinho Mais Próximo): O algoritmo simplesmente escolhe o téxel cuja coordenada UV está mais próxima
  do centro do píxel. O resultado visual é um aspeto pixelizado e em bloco (muito comum e intencional em jogos de
  estilo "retro" ou pixel-art).

• Filtragem Bilinear: O algoritmo lê os quatro téxeis mais próximos da coordenada UV calculada e realiza uma média
  ponderada entre eles com base na distância linear. Isto suaviza as transições de cor, eliminando os blocos
  geométricos e substituindo-os por um aspeto ligeiramente desfocado, mas mais suave e orgânico.

Minificação e o Conceito de Mipmapping

A minificação ocorre quando o objeto está muito distante da câmara. Neste cenário, um único píxel do ecrã pode abranger
uma área que corresponde a dezenas ou centenas de téxeis na textura. Se usarmos amostragem simples, o computador terá
de escolher arbitrariamente um téxel individual no meio de muitos, ignorando os restantes. À medida que a câmara se
move, esta escolha arbitrária muda rapidamente de frame para frame, gerando um ruído visual cintilante e altamente
perturbador conhecido como aliasing.

Para mitigar este problema, Lance Williams introduziu em 1983 o conceito de Mipmapping. Um "Mipmap" (derivado da
expressão latina multum in parvo, que significa "muito em pouco espaço") é uma cadeia de imagens pré-calculadas. A
imagem original (Nível 0) é progressivamente reduzida para metade da sua largura e altura nos níveis seguintes (Nível 1
é 1/4 da área original, Nível 2 é 1/16, e assim sucessivamente até chegar a um único píxel).

Quando o objeto está longe, o motor gráfico não amostra a textura original gigante. Em vez disso, calcula a taxa de
compressão no ecrã, determina o nível de Mipmap ideal correspondente àquela distância e lê a cor a partir dessa versão
mais pequena e pré-filtrada. Isto estabiliza a imagem, elimina o ruído cintilante e melhora o desempenho ao otimizar a
utilização da memória cache da placa gráfica.

Quando se combina a filtragem bilinear dentro de um nível de Mipmap com a interpolação linear entre dois níveis de
Mipmap adjacentes, obtém-se a Filtragem Trilinear, que elimina as linhas de transição abruptas visíveis no cenário à
medida que o observador avança.

6. Conclusão e Evolução Moderna

O mapeamento de texturas básico foi a rampa de lançamento para quase todas as técnicas modernas de renderização
fotorrealista. A beleza do conceito reside na sua flexibilidade. Uma vez dominada a infraestrutura matemática para
mapear coordenadas UV com correção de perspetiva, os engenheiros perceberam que os dados guardados nas texturas não
tinham de ser obrigatoriamente cores RGB.

Atualmente, o mesmo mapeamento UV é utilizado para ler dados de relevo e vetores normais de superfície através de Bump
Mapping e Normal Mapping, permitindo que a luz reaja a micro-detalhes tridimensionais falsificados na textura. É também
a base do PBR (Physically Based Rendering), onde texturas específicas definem a rugosidade (Roughness), a
metalicidade (Metallness) e a oclusão ambiental de um modelo.

Compreender o texture mapping clássico é, portanto, compreender o pilar central que transformou os computadores de meras
calculadoras de vetores em janelas capazes de projetar realidades virtuais indistinguíveis do mundo que nos rodeia.

*/
