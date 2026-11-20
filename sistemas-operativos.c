#include <stddef.h>
#include <stdint.h>

typedef uintptr_t endereço_físico_t;
typedef uintptr_t endereço_virtual_t;

struct bloco_memória {
    endereço_físico_t base;
    size_t tamanho;
    bool livre;
};

[[nodiscard]] endereço_físico_t reservar_página_física(void);
void libertar_página_física(endereço_físico_t endereço);

struct tabela_páginas;

[[nodiscard]] bool mapear_página_virtual(
    struct tabela_páginas *tabela_raiz,
    endereço_virtual_t virtual,
    endereço_físico_t físico,
    uint32_t bandeiras
);

void desmapear_página_virtual(struct tabela_páginas *tabela_raiz, endereço_virtual_t virtual);

void limpar_memória_segura(void *destino, size_t tamanho);
[[nodiscard]] bool validar_intervalo_utilizador(const void *ponteiro, size_t tamanho);

#include <stdint.h>

typedef uint32_t id_processo_t;

enum estado_processo : uint8_t {
    ESTADO_LIVRE = 0,
    ESTADO_PRONTO,
    ESTADO_EM_EXECUÇÃO,
    ESTADO_BLOQUEADO,
    ESTADO_TERMINADO
};

struct contexto_cpu {
    uint64_t registos_gerais;
    uint64_t ponteiro_pilha;
    uint64_t ponteiro_instrução;
    uint64_t registo_estado;
};

struct bloco_controlo_processo {
    id_processo_t id;
    enum estado_processo estado;
    uint8_t prioridade;
    endereço_físico_t base_pilha;
    struct contexto_cpu contexto;
    struct bloco_controlo_processo *próximo;
};

[[nodiscard]] id_processo_t criar_processo(void (*ponto_entrada)(void), uint8_t prioridade);
[[noreturn]] void terminar_processo_atual(int32_t código_saída);

void escalonar_próximo_processo(void);
void trocar_contexto(struct contexto_cpu *anterior, const struct contexto_cpu *seguinte);
[[nodiscard]] uint8_t obter_prioridade_mais_alta(uint64_t mapa_bits_filas);

#include <stdatomic.h>

struct cadeado_rotativo {
    atomic_flag trancado;
};

struct semáforo {
    atomic_int contador;
    struct bloco_controlo_processo *fila_espera;
};

void inicializar_cadeado(struct cadeado_rotativo *cadeado);
void adquirir_cadeado(struct cadeado_rotativo *cadeado);
void libertar_cadeado(struct cadeado_rotativo *cadeado);

void esperar_semáforo(struct semáforo *semáforo);
void assinalar_semáforo(struct semáforo *semáforo);

#include <stdint.h>

typedef uint16_t porta_es_t;
typedef uint8_t vetor_interrupção_t;

struct quadro_interrupção {
    uint64_t número_interrupção;
    uint64_t código_erro;
    uint64_t ponteiro_instrução;
};

void registar_tratador_interrupção(vetor_interrupção_t vetor, void (*tratador)(struct quadro_interrupção *));
void ativar_interrupções(void);
void desativar_interrupções(void);

uint8_t ler_porta_8(porta_es_t porta);
void escrever_porta_8(porta_es_t porta, uint8_t valor);

uint32_t ler_registo_mmio(volatile uint32_t *endereço);
void escrever_registo_mmio(volatile uint32_t *endereço, uint32_t valor);

[[noreturn]] void pânico_núcleo(const char *mensagem, const char *ficheiro, uint32_t linha);
void escrever_consola_depuração(const char *texto);



#include <stdbit.h>
#include <stddef.h>
#include <stdint.h>

typedef uintptr_t endereço_físico_t;

constexpr size_t TAMANHO_PÁGINA = 4096;
constexpr size_t MÁXIMO_PÁGINAS = 32768; // Exemplo: 128 MiB de RAM
constexpr size_t ENTRADAS_MAPA = MÁXIMO_PÁGINAS / 64;

// Cada bit a '1' indica página livre; '0' indica página ocupada
static uint64_t mapa_páginas[ENTRADAS_MAPA] = {0};
static endereço_físico_t endereço_base_memória = 0;

void inicializar_gestor_memória(endereço_físico_t base, size_t total_páginas) {
    endereço_base_memória = base;
    size_t limite = total_páginas < MÁXIMO_PÁGINAS ? total_páginas : MÁXIMO_PÁGINAS;

    for (size_t i = 0; i < limite; ++i) {
        mapa_páginas[i / 64] |= (1ULL << (i % 64));
    }
}

[[nodiscard]] endereço_físico_t reservar_página_física(void) {
    for (size_t índice_bloco = 0; índice_bloco < ENTRADAS_MAPA; ++índice_bloco) {
        uint64_t bloco = mapa_páginas[índice_bloco];

        // Se houver pelo menos um bit a 1 (página livre) neste bloco de 64 páginas
        if (bloco != 0) {
            // C23 <stdbit.h>: encontra a posição do primeiro bit a '1' a partir da direita (LSB)
            unsigned int deslocamento_bit = stdc_trailing_zeros(bloco);

            // Marca a página como ocupada (coloca o bit a 0)
            mapa_páginas[índice_bloco] &= ~(1ULL << deslocamento_bit);

            size_t índice_página = (índice_bloco * 64) + deslocamento_bit;
            return endereço_base_memória + (índice_página * TAMANHO_PÁGINA);
        }
    }

    // Devolve 0 (endereço inválido/nulo) se a memória física estiver esgotada
    return 0;
}

void libertar_página_física(endereço_físico_t endereço) {
    if (endereço < endereço_base_memória) {
        return;
    }

    size_t índice_página = (endereço - endereço_base_memória) / TAMANHO_PÁGINA;
    if (índice_página >= MÁXIMO_PÁGINAS) {
        return;
    }

    // Marca a página novamente como livre (coloca o bit a 1)
    mapa_páginas[índice_página / 64] |= (1ULL << (índice_página % 64));
}

#include <stddef.h>
#include <stdint.h>

typedef uintptr_t endereço_físico_t;
typedef uintptr_t endereço_virtual_t;

// Bandeiras padrão das entradas das tabelas de páginas
constexpr uint64_t PÁGINA_PRESENTE      = 1ULL << 0;
constexpr uint64_t PÁGINA_ESCRITA       = 1ULL << 1;
constexpr uint64_t PÁGINA_UTILIZADOR    = 1ULL << 2;
constexpr uint64_t PÁGINA_SEM_EXECUÇÃO  = 1ULL << 63;

constexpr uint64_t MÁSCARA_ENDEREÇO     = 0x000F'FFFF'FFFF'F000ULL;
constexpr size_t ENTRADAS_POR_TABELA    = 512;

struct tabela_páginas {
    uint64_t entradas[ENTRADAS_POR_TABELA];
};

// Protótipos externos necessários do gestor físico e de conversão de endereços
[[nodiscard]] extern endereço_físico_t reservar_página_física(void);
extern void *físico_para_virtual_direto(endereço_físico_t endereço);
extern void invalidar_entrada_tlb(endereço_virtual_t virtual);

static void zerar_tabela(struct tabela_páginas *tabela) {
    for (size_t i = 0; i < ENTRADAS_POR_TABELA; ++i) {
        tabela->entradas[i] = 0;
    }
}

[[nodiscard]] bool mapear_página_virtual(
    struct tabela_páginas *tabela_raiz,
    endereço_virtual_t virtual,
    endereço_físico_t físico,
    uint32_t bandeiras
) {
    // Verifica se ambos os endereços estão alinhados a 4096 bytes (4 KiB)
    if (tabela_raiz == nullptr || (virtual & 0xFFF) != 0 || (físico & 0xFFF) != 0) {
        return false;
    }

    // Extrai os 4 índices de 9 bits do endereço virtual de 48 bits
    size_t índices[4] = {
        (virtual >> 39) & 0x1FF, // Nível 4 (PML4)
        (virtual >> 30) & 0x1FF, // Nível 3 (PDPT)
        (virtual >> 21) & 0x1FF, // Nível 2 (PD)
        (virtual >> 12) & 0x1FF  // Nível 1 (PT)
    };

    struct tabela_páginas *tabela_atual = tabela_raiz;

    // Percorre os 3 níveis superiores, alocando tabelas intermédias se não existirem
    for (size_t nível = 0; nível < 3; ++nível) {
        size_t índice = índices[nível];
        uint64_t entrada = tabela_atual->entradas[índice];

        if ((entrada & PÁGINA_PRESENTE) == 0) {
            endereço_físico_t nova_tabela_física = reservar_página_física();
            if (nova_tabela_física == 0) {
                return false; // Sem memória física para criar a tabela intermédia
            }

            struct tabela_páginas *nova_tabela_virtual = físico_para_virtual_direto(nova_tabela_física);
            zerar_tabela(nova_tabela_virtual);

            // As tabelas intermédias recebem permissões permissivas; o controlo real fica na folha (Nível 1)
            entrada = (nova_tabela_física & MÁSCARA_ENDEREÇO) | PÁGINA_PRESENTE | PÁGINA_ESCRITA | PÁGINA_UTILIZADOR;
            tabela_atual->entradas[índice] = entrada;
        }

        endereço_físico_t próximo_físico = entrada & MÁSCARA_ENDEREÇO;
        tabela_atual = físico_para_virtual_direto(próximo_físico);
    }

    // Configura a entrada final (Nível 1) com o endereço físico alvo e as bandeiras pedidas
    size_t índice_folha = índices[3];
    tabela_atual->entradas[índice_folha] = (físico & MÁSCARA_ENDEREÇO) | (uint64_t)bandeiras | PÁGINA_PRESENTE;

    // Atualiza a cache de tradução do processador (TLB) para este endereço
    invalidar_entrada_tlb(virtual);

    return true;
}

static bool tabela_está_vazia(const struct tabela_páginas *tabela) {
    for (size_t i = 0; i < ENTRADAS_POR_TABELA; ++i) {
        if ((tabela->entradas[i] & PÁGINA_PRESENTE) != 0) {
            return false;
        }
    }
    return true;
}

void desmapear_página_virtual(struct tabela_páginas *tabela_raiz, endereço_virtual_t virtual) {
    // Verifica se a raiz é válida e se o endereço está alinhado a 4 KiB
    if (tabela_raiz == nullptr || (virtual & 0xFFF) != 0) {
        return;
    }

    size_t índices[4] = {
        (virtual >> 39) & 0x1FF, // Nível 4 (PML4)
        (virtual >> 30) & 0x1FF, // Nível 3 (PDPT)
        (virtual >> 21) & 0x1FF, // Nível 2 (PD)
        (virtual >> 12) & 0x1FF  // Nível 1 (PT)
    };

    // Guarda o caminho percorrido para podermos limpar tabelas vazias no regresso
    struct tabela_páginas *caminho_tabelas[4] = {nullptr};
    endereço_físico_t caminho_físico[4] = {0};

    struct tabela_páginas *tabela_atual = tabela_raiz;

    for (size_t nível = 0; nível < 3; ++nível) {
        caminho_tabelas[nível] = tabela_atual;
        uint64_t entrada = tabela_atual->entradas[índices[nível]];

        // Se alguma tabela intermédia não existir, a página já não está mapeada
        if ((entrada & PÁGINA_PRESENTE) == 0) {
            return;
        }

        endereço_físico_t próximo_físico = entrada & MÁSCARA_ENDEREÇO;
        caminho_físico[nível + 1] = próximo_físico;
        tabela_atual = físico_para_virtual_direto(próximo_físico);
    }

    caminho_tabelas[3] = tabela_atual;

    // Remove o mapeamento na tabela folha (Nível 1)
    size_t índice_folha = índices[3];
    if ((tabela_atual->entradas[índice_folha] & PÁGINA_PRESENTE) == 0) {
        return; // A página já não estava presente
    }

    tabela_atual->entradas[índice_folha] = 0;

    // Invalida a cache de tradução do processador (TLB) imediatamente
    invalidar_entrada_tlb(virtual);

    // Sobe a árvore (do Nível 1 até ao Nível 3) a libertar tabelas que ficaram sem entradas
    for (size_t nível = 3; nível >= 1; --nível) {
        if (tabela_está_vazia(caminho_tabelas[nível])) {
            // Desliga a tabela vazia da tabela pai
            struct tabela_páginas *tabela_pai = caminho_tabelas[nível - 1];
            tabela_pai->entradas[índices[nível - 1]] = 0;

            // Devolve a página física da tabela vazia ao gestor de memória
            libertar_página_física(caminho_físico[nível]);
        } else {
            // Se esta tabela ainda tem outras entradas ativas, as superiores também têm
            break;
        }
    }
}

#include <stddef.h>
#include <stdint.h>

void limpar_memória_segura(void *destino, size_t tamanho) {
    if (destino == nullptr || tamanho == 0) {
        return;
    }

    // O qualificador 'volatile' impede o compilador de otimizar e remover
    // estas escritas mesmo que a memória não volte a ser lida (Dead Store Elimination)
    volatile uint8_t *ponteiro = (volatile uint8_t *)destino;

    while (tamanho > 0) {
        *ponteiro = 0;
        ++ponteiro;
        --tamanho;
    }

    // Barreira de memória para o compilador (GCC/Clang):
    // garante que a limpeza ocorre exatamente neste ponto antes de qualquer instrução seguinte
    __asm__ volatile ("" : : "r"(destino) : "memory");
}

#include <stdckdint.h>
#include <stddef.h>
#include <stdint.h>

typedef uintptr_t endereço_virtual_t;

// Limite típico do espaço de utilizador em arquiteturas de 64 bits (canonical lower half: 128 TiB)
// Tudo acima disto pertence ao espaço reservado ao núcleo (kernel-space)
constexpr endereço_virtual_t LIMITE_ESPAÇO_UTILIZADOR = 0x0000'7FFF'FFFF'FFFFULL;

[[nodiscard]] bool validar_intervalo_utilizador(const void *ponteiro, size_t tamanho) {
    // Rejeita ponteiros nulos ou intervalos vazios
    if (ponteiro == nullptr || tamanho == 0) {
        return false;
    }

    endereço_virtual_t início = (endereço_virtual_t)ponteiro;
    endereço_virtual_t fim = 0;

    // C23 <stdckdint.h>: calcula (início + tamanho) e devolve 'true' se ocorrer overflow aritmético.
    // Sem isto, um utilizador poderia passar (início = 0xFFFF...F000, tamanho = 0x2000),
    // fazendo a soma dar a volta para 0x1000 e contornando uma verificação simples de limite.
    if (ckd_add(&fim, início, tamanho)) {
        return false;
    }

    // Garante que o intervalo começa e termina estritamente dentro do espaço do utilizador
    // (o intervalo é [início, fim[, logo o último byte acedido é fim - 1)
    if (início >= LIMITE_ESPAÇO_UTILIZADOR || (fim - 1) > LIMITE_ESPAÇO_UTILIZADOR) {
        return false;
    }

    return true;
}

#include <stdbit.h>
#include <stddef.h>
#include <stdint.h>

typedef uint32_t id_processo_t;
typedef uintptr_t endereço_físico_t;

constexpr size_t MÁXIMO_PROCESSOS = 64;
constexpr size_t TAMANHO_PILHA_PROCESSO = 4096; // 1 página (4 KiB)
constexpr uint8_t PRIORIDADE_MÁXIMA = 63;

struct contexto_cpu {
    uint64_t registos_gerais; // Exemplo: r15, r14, r13, r12, rbx, rbp
    uint64_t ponteiro_pilha;
    uint64_t ponteiro_instrução;
    uint64_t registo_estado;
};

// Tabela estática de processos do núcleo e mapa de bits para slots livres (1 = livre, 0 = ocupado)
static struct bloco_controlo_processo tabela_processos[MÁXIMO_PROCESSOS] = {0};
static uint64_t mapa_slots_livres = 0xFFFF'FFFF'FFFF'FFFFULL;
static id_processo_t próximo_id_disponível = 1;

// Protótipos externos necessários (já implementados nos módulos anteriores)
[[nodiscard]] extern endereço_físico_t reservar_página_física(void);
extern void *físico_para_virtual_direto(endereço_físico_t endereço);
extern void limpar_memória_segura(void *destino, size_t tamanho);
extern void inserir_na_fila_prontos(struct bloco_controlo_processo *processo);

[[nodiscard]] id_processo_t criar_processo(void (*ponto_entrada)(void), uint8_t prioridade) {
    // Valida o ponteiro da função de entrada, a prioridade e se há vagas na tabela
    if (ponto_entrada == nullptr || prioridade > PRIORIDADE_MÁXIMA || mapa_slots_livres == 0) {
        return 0; // Devolve 0 para indicar falha na criação do processo
    }

    // Reserva uma página física para servir de pilha (stack) exclusiva deste processo
    endereço_físico_t pilha_física = reservar_página_física();
    if (pilha_física == 0) {
        return 0; // Sem memória física disponível
    }

    // C23 <stdbit.h>: encontra o primeiro slot livre na tabela de 64 processos em O(1)
    unsigned int índice_slot = stdc_trailing_zeros(mapa_slots_livres);
    mapa_slots_livres &= ~(1ULL << índice_slot);

    struct bloco_controlo_processo *pcb = &tabela_processos[índice_slot];
    limpar_memória_segura(pcb, sizeof(struct bloco_controlo_processo));

    // Preenche os metadados do processo
    pcb->id = próximo_id_disponível++;
    pcb->estado = ESTADO_PRONTO;
    pcb->prioridade = prioridade;
    pcb->base_pilha = pilha_física;
    pcb->próximo = nullptr;

    // Configura o contexto inicial da CPU:
    // A pilha cresce para baixo (do topo para a base) e é alinhada a 16 bytes (requisito ABI)
    uintptr_t topo_pilha_virtual = (uintptr_t)físico_para_virtual_direto(pilha_física) + TAMANHO_PILHA_PROCESSO;
    pcb->contexto.ponteiro_pilha = topo_pilha_virtual & ~0xFULL;
    pcb->contexto.ponteiro_instrução = (uintptr_t)ponto_entrada;
    pcb->contexto.registo_estado = 0x202; // Exemplo x86_64 (RFLAGS com interrupções ativadas - IF=1)

    // Coloca o processo na fila do escalonador para ficar elegível para execução
    inserir_na_fila_prontos(pcb);

    return pcb->id;
}
