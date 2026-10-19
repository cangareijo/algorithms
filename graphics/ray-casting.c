/*

Escreve uma demonstração de ray casting em C23.
Escreve menos de 5000 caracteres.
Inclui controlos.
Escreve o código em português europeu.

*/

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define LARGURA_MAPA 8
#define ALTURA_MAPA 8
#define LARGURA_ECRA 60
#define ALTURA_ECRA 20

const int mapa[ALTURA_MAPA][LARGURA_MAPA] = {
    {1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,1,0,0,1,0,1},
    {1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1}
};

struct Jogador {
    float x;
    float y;
    float angulo;
    float fov;
};

struct Jogador jogador = { .x = 3.5f, .y = 3.5f, .angulo = 0.0f, .fov = 1.0f };

// Configura o terminal para ler teclas instantaneamente
void configurar_terminal(bool ativar) {
    static struct termios antigo;
    if (ativar) {
        tcgetattr(STDIN_FILENO, &antigo);
        struct termios novo = antigo;
        novo.c_lflag &= ~(ICANON | ECHO); // Desativa buffer de linha e eco
        tcsetattr(STDIN_FILENO, TCSANOW, &novo);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &antigo);
    }
}

void atualizar_controlos(char comando) {
    float vel = 0.2f;
    float vel_rot = 0.1f;

    switch (comando) {
        case 'w': [[fallthrough]];
        case 'W': {
            float nx = jogador.x + cosf(jogador.angulo) * vel;
            float ny = jogador.y + sinf(jogador.angulo) * vel;
            if (mapa[(int)ny][(int)nx] == 0) { jogador.x = nx; jogador.y = ny; }
            break;
        }
        case 's': [[fallthrough]];
        case 'S': {
            float nx = jogador.x - cosf(jogador.angulo) * vel;
            float ny = jogador.y - sinf(jogador.angulo) * vel;
            if (mapa[(int)ny][(int)nx] == 0) { jogador.x = nx; jogador.y = ny; }
            break;
        }
        case 'a': [[fallthrough]];
        case 'A': jogador.angulo -= vel_rot; break;
        case 'd': [[fallthrough]];
        case 'D': jogador.angulo += vel_rot; break;
    }
}

void renderizar_ecra() {
    char ecra[ALTURA_ECRA][LARGURA_ECRA];

    for (int x = 0; x < LARGURA_ECRA; x++) {
        float angulo_raio = (jogador.angulo - jogador.fov / 2.0f) + ((float)x / LARGURA_ECRA) * jogador.fov;
        float dist = 0.0f;
        bool parede = false;

        float rx = cosf(angulo_raio);
        float ry = sinf(angulo_raio);

        while (!parede && dist < 16.0f) {
            dist += 0.05f;
            int tx = (int)(jogador.x + rx * dist);
            int ty = (int)(jogador.y + ry * dist);

            if (tx < 0 || tx >= LARGURA_MAPA || ty < 0 || ty >= ALTURA_MAPA) {
                parede = true; dist = 16.0f;
            } else if (mapa[ty][tx] > 0) {
                parede = true;
            }
        }

        float dist_corr = dist * cosf(angulo_raio - jogador.angulo);
        int alt_parede = (int)(ALTURA_ECRA / dist_corr);
        if (alt_parede > ALTURA_ECRA) alt_parede = ALTURA_ECRA;

        int teto = (ALTURA_ECRA - alt_parede) / 2;
        int chao = ALTURA_ECRA - teto;

        char sombreado = '#';
        if (dist_corr > 4.0f) sombreado = 'X';
        if (dist_corr > 7.0f) sombreado = 'x';
        if (dist_corr > 10.0f) sombreado = '.';

        for (int y = 0; y < ALTURA_ECRA; y++) {
            if (y < teto) ecra[y][x] = ' ';
            else if (y >= teto && y < chao) ecra[y][x] = sombreado;
            else ecra[y][x] = '-';
        }
    }

    printf("\x1b[H"); // Move o cursor para o topo
    for (int y = 0; y < ALTURA_ECRA; y++) {
        for (int x = 0; x < LARGURA_ECRA; x++) putchar(ecra[y][x]);
        putchar('\n');
    }
    printf("Posição: (%.2f, %.2f) | Ângulo: %.2f rad\n", jogador.x, jogador.y, jogador.angulo);
    printf("Controlos: [W/S/A/D] | [Q] Sair (Movimento Instantâneo)\n");
}

int main() {
    printf("\x1b[2J\x1b[?25l"); // Limpa e esconde o cursor do terminal
    configurar_terminal(true);

    char entrada = '\0';
    while (entrada != 'q' && entrada != 'Q') {
        renderizar_ecra();
        entrada = (char)getchar(); // Lê a tecla imediatamente
        atualizar_controlos(entrada);
    }

    configurar_terminal(false);
    printf("\x1b[?25hDemonstração terminada.\n"); // Mostra o cursor novamente
    return 0;
}

/*

gcc -lm -g -std=c23 -Wall -Wextra -Wpedantic graphics/ray-casting.c -o ray-casting

*/

/*

Escreve um artigo longo sobre ray casting.
Escreve em português europeu.
Escreve em formato TXT.
Escreve em Unicode.

*/

/*

ARTIGO TÉCNICO: A MAGIA DO RAY CASTING – DA ORIGEM EM TEXTO À REVOLUÇÃO DO 3D REALISTA

Data: Outubro de 2026

--------------------------------------------------------------------------------
1. INTRODUÇÃO
--------------------------------------------------------------------------------

A computação gráfica moderna permite-nos contemplar mundos virtuais com um nível 
de detalhe quase indistinguível da realidade. Contudo, muito antes de as placas 
gráficas modernas (GPUs) processarem milhares de milhões de polígonos por segundo 
através de "rasterização" ou "ray tracing" em tempo real, os programadores da 
década de 1990 enfrentavam um desafio monumental: como criar a ilusão de um 
espaço tridimensional fluido utilizando processadores que mal conseguiam efetuar 
operações aritméticas básicas em tempo útil?

A resposta a este enigma foi o Ray Casting (lançamento de raios), uma técnica 
elegante, matemática e computacionalmente económica que serve de ponte entre o 
design bidimensional e a perspetiva tridimensional. Este artigo explora em detalhe 
o funcionamento desta técnica pioneira, as suas fundações matemáticas, a evolução 
para o algoritmo DDA (Digital Differential Analysis) e o seu legado eterno no 
desenvolvimento de videojogos e simulações digitais.

--------------------------------------------------------------------------------
2. O QUE É O RAY CASTING?
--------------------------------------------------------------------------------

O Ray Casting é uma técnica de renderização geométrica onde, para cada coluna 
vertical do ecrã, é projetado um "raio" imaginário a partir do ponto de vista do 
observador (jogador) para dentro de um mapa bidimensional. O algoritmo calcula a 
distância exata entre o observador e o primeiro obstáculo sólido (geralmente uma 
parede) com o qual o raio colide. 

Com base nessa distância, o motor gráfico calcula a altura proporcional com que 
essa parede deve ser desenhada no ecrã:
- Se a parede estiver muito próxima, a distância é curta e a projeção vertical 
  ocupa quase todo o ecrã.
- Se a parede estiver distante, a distância é longa e a projeção vertical será 
  reduzida a uma linha ténue no horizonte.

É fundamental distinguir o Ray Casting do Ray Tracing:
- Ray Casting: É bidimensional no seu cálculo principal. Os raios movem-se apenas 
  num plano horizontal (X, Y). O eixo Z (altura) é uma ilusão matemática calculada 
  à posteriori com base na distância. Não suporta, nativamente, superfícies curvas 
  complexas, inclinações verticais na câmara (olhar para cima ou para baixo) ou 
  múltiplos níveis de altura (pontes).
- Ray Tracing: Lança raios num espaço puramente tridimensional (X, Y, Z). Simula 
  o comportamento físico real da luz, incluindo reflexões secundárias, refrações 
  em vidro e sombras suaves difusas, exigindo um poder computacional exponencialmente 
  superior.

--------------------------------------------------------------------------------
3. A ILUSÃO DO MAPA EM GRELHA (GRID-BASED WORLD)
--------------------------------------------------------------------------------

Para manter a máxima eficiência, os motores clássicos de Ray Casting, como o de 
Wolfenstein 3D, limitam o mundo virtual a uma matriz ou grelha bidimensional regular 
(geralmente composta por inteiros). Cada célula da grelha representa ou um espaço 
vazio (valor 0) ou um bloco sólido de parede (valor 1 ou superior, mapeando 
diferentes texturas).

O jogador possui uma coordenada flutuante no mapa (por exemplo, X = 3.5, Y = 4.2), 
um ângulo de visão em radianos e um Campo de Visão (FOV - Field of View). Se o 
ecrã tiver uma largura de 320 píxeis, o motor dividirá o FOV em 320 raios 
individuais, calculando a trajetória de cada um de forma independente.

--------------------------------------------------------------------------------
4. A MATEMÁTICA E O ALGORITMO DDA (DIGITAL DIFFERENTIAL ANALYSIS)
--------------------------------------------------------------------------------

A abordagem mais ingénua para detetar colisões consiste em avançar o raio em pequenos 
passos fixos (ex: incrementos de 0.05 unidades) e verificar constantemente se a 
coordenada resultante atingiu uma parede. Contudo, esta técnica apresenta dois 
graves defeitos:
1. Se o passo for muito grande, o raio pode "saltar" a esquina de uma parede, 
   causando falhas visuais (glitches).
2. Se o passo for muito pequeno, o processador desperdiça ciclos preciosos a testar 
   a mesma célula repetidamente.

A solução ideal é o algoritmo DDA. O DDA é um método matemático ultrarrápido de 
interseção de linhas que permite ao raio "saltar" diretamente de uma linha da grelha 
para a linha seguinte (seja horizontal ou vertical), eliminando testes redundantes.

O algoritmo funciona determinando duas variáveis principais:
- deltaDistX / deltaDistY: A distância que o raio precisa de percorrer para avançar 
  exatamente uma unidade inteira na direção X ou Y.
- sideDistX / sideDistY: A distância inicial desde a posição atual do jogador até 
  à primeira linha divisória da grelha em X e Y.

A cada iteração do ciclo DDA, o algoritmo compara sideDistX e sideDistY. Se 
sideDistX for menor, o raio avança para a próxima célula horizontal da grelha. Se 
sideDistY for menor, avança para a próxima célula vertical. Isto continua até que 
a célula inspecionada no mapa seja maior que zero (uma parede).

--------------------------------------------------------------------------------
5. A CORREÇÃO DO EFEITO "OLHO DE PEIXE" (FISH-EYE DISTORTION)
--------------------------------------------------------------------------------

Um dos problemas geométricos mais célebres no desenvolvimento de motores de Ray 
Casting é o efeito "olho de peixe". Se calcularmos a altura da parede utilizando 
a distância euclidiana direta entre o jogador e o ponto de impacto, as paredes 
retas parecerão curvas ou abauladas nas extremidades do ecrã.

Isto acontece porque os raios periféricos são naturalmente mais longos do que os 
raios centrais, mesmo quando apontam para uma parede perfeitamente plana à nossa 
frente. Para corrigir esta distorção, a distância deve ser projetada no vetor 
de direção central do jogador. A fórmula matemática aplicada é:

Distancia_Corrigida = Distancia_Real * cos(Angulo_do_Raio - Angulo_do_Jogador)

Esta operação simples achata a perceção da distância, garantindo que as linhas 
retas permaneçam retas na projeção geométrica do ecrã.

--------------------------------------------------------------------------------
6. TEXTURIZAÇÃO E SOMBREAMENTO
--------------------------------------------------------------------------------

Assim que a distância corrigida é estabelecida, o motor sabe exatamente qual a 
coluna de píxeis da parede que deve desenhar e qual a sua altura. Para adicionar 
fidelidade visual:
- Mapeamento de Texturas (Texture Mapping): O algoritmo determina em que ponto 
  exato da célula o raio colidiu (por exemplo, a 40% do início da face norte do bloco). 
  Esse valor percentual é utilizado para selecionar a coluna correspondente de píxeis 
  da imagem da textura original, aplicando-a verticalmente no ecrã.
- Sombreamento por Distância: Para simular atmosfera e profundidade, a intensidade 
  da cor ou o caractere ASCII utilizado é alterado com base na distância. Paredes 
  mais distantes recebem tons mais escuros (ou caracteres menos densos, como '.') 
  enquanto paredes próximas mantêm o brilho total (como '#').

--------------------------------------------------------------------------------
7. CONCLUSÃO E LEGADO
--------------------------------------------------------------------------------

O Ray Casting é o exemplo perfeito de como a engenharia de software e a matemática 
aplicada conseguem superar barreiras físicas de hardware. Ao transformar um problema 
tridimensional complexo numa série de projeções unidimensionais paralelas assentes 
num mapa 2D, programadores lendários como John Carmack revolucionaram a indústria 
dos computadores de consumo.

Embora as técnicas atuais de renderização em três dimensões sigam arquiteturas de 
polígonos e shaders sofisticados, o estudo do Ray Casting continua a ser um rito 
de passagem indispensável para estudantes de engenharia informática e entusiastas 
de computação gráfica. Compreender o Ray Casting é compreender a essência da 
otimização: extrair o máximo de impacto visual com o mínimo de custo computacional.

*/
