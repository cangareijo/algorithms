/*

ALGORITMO DE PREENCHIMENTO DE POLÍGONOS POR LINHAS DE VARRIMENTO (SCANLINE)

================================================================================
1. INTRODUÇÃO E FILOSOFIA DO ALGORITMO
================================================================================

O algoritmo de preenchimento por linhas de varrimento (Scanline Polygon Fill)
é um dos métodos fundamentais da computação gráfica raster. O seu principal 
objetivo é determinar quais os píxeis do ecrã que se encontram no interior 
de um polígono bidimensional arbitrário, permitindo renderizar formas sólidas 
numa matriz digital de píxeis.

Em vez de testar individualmente se cada píxel do ecrã pertence ou não ao 
polígono (uma abordagem de força bruta computacionalmente inviável), este 
algoritmo atua de forma cirúrgica e estruturada. Ele projeta linhas horizontais 
imaginárias ao longo do ecrã — chamadas "scanlines" — que coincidem com as 
linhas de píxeis do buffer de vídeo.

Para cada linha horizontal, o algoritmo calcula os pontos exatos de interseção 
com as arestas que delimitam o polígono. Sabendo onde a linha entra e sai da 
forma geométrica, o sistema consegue preencher os intervalos de píxeis 
corretos a altíssima velocidade.

================================================================================
2. ESTRUTURAS DE DADOS ESSENCIAIS
================================================================================

A grande eficiência deste algoritmo reside na organização geométrica das arestas. 
Para evitar cálculos redundantes, utilizam-se duas tabelas dinâmicas:

2.1. Tabela Global de Arestas (Edge Table - ET)
-----------------------------------------------
A ET armazena todas as linhas que compõem o polígono, exceto as arestas puramente 
horizontais. As arestas horizontais são intencionalmente ignoradas porque correm 
em paralelo com as linhas de varrimento e não delimitam nenhuma transição de 
entrada ou saída.

Cada aresta é classificada e inserida na ET com base no seu valor de coordenada 
vertical mínima (y_min). Isto cria uma lista perfeitamente indexada por linhas, 
permitindo ao algoritmo saber exatamente em que altura do ecrã uma determinada 
aresta "começa" a existir.

Cada entrada da Tabela Global de Arestas armazena os seguintes dados:
- y_max: A coordenada vertical máxima onde a aresta termina. Quando a linha de 
  varrimento ultrapassa este valor, a aresta deixa de ser processada.
- x_curr: A coordenada horizontal inicial associada ao ponto mais baixo da 
  aresta (y_min). Este valor é atualizado dinamicamente.
- Declive Inverso (1/m ou dx/dy): A taxa de variação horizontal em relação à 
  vertical, calculada como ((x2 - x1) / (y2 - y1)). É o fator matemático que 
  dita quanto a coordenada x se desloca para o lado a cada linha que descemos.

2.2. Tabela de Arestas Ativas (Active Edge Table - AET)
-------------------------------------------------------
A AET representa a "frente de onda" do ciclo de preenchimento. Ela contém 
apenas as arestas do polígono que estão a ser intercetadas pela linha de 
varrimento atual (y). 

À medida que o algoritmo avança verticalmente, a AET é constantemente 
atualizada: novas arestas entram em cena vindas da ET e arestas antigas são 
descartadas assim que a linha de varrimento ultrapassa o seu y_max.

================================================================================
3. O PROCESSO PASSO A PASSO
================================================================================

O ciclo de execução opera de forma estritamente sequencial ao longo do eixo 
vertical, progredindo desde o y mínimo absoluto do polígono até ao seu y máximo.

Passo 1: Inicialização e Extração
---------------------------------
O algoritmo varre os vértices do polígono fornecidos pelo utilizador, determina 
a extensão vertical total da forma (y_min global e y_max global) e constrói a 
Tabela Global de Arestas (ET), distribuindo as arestas válidas pelos respetivos 
índices de y_min.

Passo 2: Atualização da AET (Remoção e Inserção)
-----------------------------------------------
Para a linha de varrimento atual (y):
- Remoção: Qualquer aresta presente na AET cujo y_max seja igual ao y atual é 
  removida da tabela. Isto indica que a linha de varrimento saiu da zona de 
  influência dessa aresta.
- Inserção: Qualquer aresta guardada na ET cujo y_min corresponda ao y atual 
  é transferida para a AET, iniciando a sua participação nos cálculos.

Passo 3: Ordenação Horizontal
-----------------------------
As arestas que permanecem ativas dentro da AET são ordenadas de forma crescente 
com base no seu valor corrente de x_curr. Esta ordenação da esquerda para a 
direita é obrigatória para o correto funcionamento da regra de preenchimento.

Passo 4: O Princípio da Paridade (Regra Par-Ímpar)
--------------------------------------------------
Com as interseções ordenadas por coordenadas X na AET, o algoritmo aplica a 
regra par-ímpar (even-odd rule). Ao percorrer a lista de interseções da esquerda 
para a direita:
- A primeira interseção detetada ativa o estado de "dentro" do polígono.
- A segunda interseção desativa o estado, passando para "fora" do polígono.
- A terceira interseção ativa novamente o estado de "dentro" (em polígonos 
  complexos, côncavos ou com furos).
- A quarta interseção volta a desativar o estado.

O algoritmo preenche com a cor selecionada todos os píxeis que se encontram 
no intervalo compreendido entre os pares de interseções ativos (entre a 1ª e a 
2ª, entre a 3ª e a 4ª, etc.).

Passo 5: Progressão Geométrica Incremental
------------------------------------------
Antes de incrementar a linha de varrimento para a próxima posição (y + 1), 
o algoritmo atualiza a posição horizontal de todas as arestas que se mantêm 
vivas na AET. Para cada uma delas, aplica-se a seguinte operação:

    x_curr = x_curr + declive_inverso

Isto garante que, na iteração seguinte, o valor de x já corresponda à inclinação 
real da linha lateral do polígono, eliminando por completo a necessidade de 
recalcular equações de reta complexas ou de invocar divisões matemáticas caras.

================================================================================
4. VANTAGENS COMPUTACIONAIS DO ALGORITMO
================================================================================

O sucesso e a longevidade do algoritmo de varrimento assentam em três pilares:

4.1. Aritmética Incremental
---------------------------
A atualização das posições de X através da adição repetida do declive inverso 
substitui multiplicações e divisões por operações simples de soma. O hardware 
processa somas de forma quase instantânea, o que confere grande velocidade ao 
sistema.

4.2. Coerência Espacial de Memória
----------------------------------
Como o preenchimento é efetuado de forma horizontal e linear linha por linha, 
o algoritmo respeita rigorosamente a disposição sequencial como os píxeis estão 
armazenados na memória RAM física (Row-Major Order). Isto maximiza a eficácia 
da memória cache do processador e evita acessos desalinhados.

4.3. Robustez Geométrica
------------------------
O algoritmo lida nativamente e sem qualquer tipo de modificação estrutural com 
polígonos convexos, côncavos, autointercetantes ou formas que contenham vazios 
interiores, mantendo sempre uma precisão visual absoluta píxel a píxel.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct {
    int x, y;
} Point;

typedef struct {
    int y_min;
    int y_max;
    float x_curr;
    float inv_slope;
} Edge;

void fill_polygon(unsigned int *pixels, int width, int height, const Point *pts, int count, unsigned int color) {
    int y_min = height;
    int y_max = 0;

    for (int i = 0; i < count; i++) {
        if (pts[i].y < y_min) y_min = pts[i].y;
        if (pts[i].y > y_max) y_max = pts[i].y;
    }

    if (y_min < 0) y_min = 0;
    if (y_max >= height) y_max = height - 1;

    Edge *et = malloc(sizeof(Edge) * count);
    int et_size = 0;

    for (int i = 0; i < count; i++) {
        auto p1 = pts[i];
        auto p2 = pts[(i + 1) % count];

        if (p1.y == p2.y) continue;

        auto v1 = (p1.y < p2.y) ? p1 : p2;
        auto v2 = (p1.y < p2.y) ? p2 : p1;

        et[et_size++] = (Edge){
            .y_min = v1.y,
            .y_max = v2.y,
            .x_curr = (float)v1.x,
            .inv_slope = (float)(v2.x - v1.x) / (float)(v2.y - v1.y)
        };
    }

    Edge **aet = malloc(sizeof(Edge*) * et_size);
    int aet_size = 0;

    for (int y = y_min; y <= y_max; y++) {
        int write_idx = 0;
        for (int i = 0; i < aet_size; i++) {
            if (aet[i]->y_max > y) {
                aet[write_idx++] = aet[i];
            }
        }
        aet_size = write_idx;

        for (int i = 0; i < et_size; i++) {
            if (et[i].y_min == y) {
                aet[aet_size++] = &et[i];
            }
        }

        for (int i = 0; i < aet_size - 1; i++) {
            for (int j = 0; j < aet_size - i - 1; j++) {
                if (aet[j]->x_curr > aet[j + 1]->x_curr) {
                    auto temp = aet[j];
                    aet[j] = aet[j + 1];
                    aet[j + 1] = temp;
                }
            }
        }

        for (int i = 0; i < aet_size; i += 2) {
            if (i + 1 >= aet_size) break;
            int x_start = (int)(aet[i]->x_curr + 0.5f);
            int x_end = (int)(aet[i + 1]->x_curr + 0.5f);

            if (x_start < 0) x_start = 0;
            if (x_end >= width) x_end = width - 1;

            for (int x = x_start; x < x_end; x++) {
                pixels[y * width + x] = color;
            }
        }

        for (int i = 0; i < aet_size; i++) {
            aet[i]->x_curr += aet[i]->inv_slope;
        }
    }

    free(aet);
    free(et);
}

int main(void) {
    constexpr int width = 800;
    constexpr int height = 600;

    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) return 1;

    int screen = DefaultScreen(dpy);
    Window win = XCreateSimpleWindow(dpy, RootWindow(dpy, screen), 10, 10, width, height, 1, 
                                     BlackPixel(dpy, screen), BlackPixel(dpy, screen));
    XSelectInput(dpy, win, ExposureMask | KeyPressMask);
    XMapWindow(dpy, win);

    unsigned int *buffer = calloc(width * height, sizeof(unsigned int));
    
    Point polygon[] = {
        {400, 100},
        {650, 250},
        {550, 500},
        {250, 500},
        {150, 250}
    };
    int num_points = sizeof(polygon) / sizeof(polygon[0]);

    fill_polygon(buffer, width, height, polygon, num_points, 0x00FF00);

    XImage *img = XCreateImage(dpy, DefaultVisual(dpy, screen), DefaultDepth(dpy, screen), 
                               ZPixmap, 0, (char*)buffer, width, height, 32, 0);

    GC gc = XCreateGC(dpy, win, 0, NULL);
    XEvent ev;

    while (1) {
        XNextEvent(dpy, &ev);
        if (ev.type == Expose) {
            XPutImage(dpy, win, gc, img, 0, 0, 0, 0, width, height);
        } else if (ev.type == KeyPress) {
            break;
        }
    }

    XDestroyImage(img);
    XFreeGC(dpy, gc);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
    return 0;
}
