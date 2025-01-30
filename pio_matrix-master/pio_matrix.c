#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

#include "pio_matrix.pio.h"

// Configurações dos pinos
#define LED 13 // GPIO13 LED VERMELHO
#define BOTAOA 5 // Botão A = 5
#define BOTAOB 6 // Botão A = 6
#define OUT_PIN 7 // Matriz de leds

//Definicao de macros
#define tempo 100   
#define FRAMES 10
#define NUM_PIXELS 25

// Variáveis globais
static volatile int a = -1;
static volatile uint32_t lastEventButton = 0; // Armazena o tempo do último evento (em microssegundos)
static volatile uint32_t lastEventLED = 0; // Armazena o tempo do último evento (em microssegundos)
PIO pio = pio0;                            // definicao do Pio     
bool ok;
uint16_t i;
uint32_t valor_led;
uint sm;
uint offset;
double r = 0.0, b = 0.0, g = 0.0;

//matriz com numeros de 0 a 9 respectivamente
double numeros[FRAMES][NUM_PIXELS] =
{ 
    {
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0
    },

    {
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0
    },

    {
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0
    },


    {
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0
    },



    {
    1.0, 0.0, 0.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0
    },


    {
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0
    },


    {
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0
    },


    {
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0
    },


    {
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0
    },

    {
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0
    }

 };

// rotina para definição da intensidade de cores do led e conversao para um binario de 32 bits
uint32_t matrix_rgb(double b, double r, double g)
{
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// rotina para "desenhar" o numero na matriz de leds
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    //funcao para espelhar os leds (Por padrão as imagens aparecem invertidas)
    int ordem_fisica[NUM_PIXELS] = 
    {
        24, 23, 22, 21, 20, 
        15, 16, 17, 18, 19, 
        14, 13, 12, 11, 10,
        5, 6, 7, 8, 9,     
        4, 3, 2, 1, 0       
    };

    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        int indice_fisico = ordem_fisica[i];
        valor_led = matrix_rgb(desenho[indice_fisico], r = 0.0, g = 0.0); // Define a cor do LED
        pio_sm_put_blocking(pio, sm, valor_led); //Manda a configuração do led para o pio
    }
}


// Função de interrupção com debouncing para incremento e decremento dos numeros
void gpio_irq_handler_BOTAO(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica se passou tempo suficiente desde o último evento
    if (current_time - lastEventButton > 200000) // 200 ms de debouncing
    {
            lastEventButton = current_time;
            if(gpio == BOTAOA)
            {
                //incremento do numero apresentado na matriz de leds
                a++;
                if(a > 9) a = 9;
                desenho_pio(numeros[a], valor_led, pio, sm, r, g, b); 
                
            }

            if (gpio == BOTAOB)
            {
                //decremento do numero apresentado na matriz de leds
                a--;
                if(a < 0) a = 0;
                desenho_pio(numeros[a], valor_led, pio, sm, r, g, b);     
            }
                                            
    }
}

//funcao para evitar o uso de sleep_ms e não correr o risco do botão ser apertando enquanto o sistema esta travado
void ledFunc()
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica se passou tempo suficiente desde o último evento
    if (current_time - lastEventLED > 100000)
    {
        lastEventLED = current_time;
        gpio_put(LED, !(gpio_get(LED)));
                       
    }
}

int main()
{    

    // coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    // Inicialização da saida padrao
    stdio_init_all();

    printf("iniciando a transmissão PIO");
    if (ok) printf("clock set to %ld\n", clock_get_hz(clk_sys));

    //configuracao do pio (offset e state machine)
    offset = pio_add_program(pio, &pio_matrix_program);
    sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    // Inicializa o LED
    gpio_init(LED);              // Inicializa o pino do LED
    gpio_set_dir(LED, GPIO_OUT); // Configura o pino como saída
    gpio_put(LED, false); // inicializacao do LED desligado

    gpio_init(BOTAOA);             // Inicializa o pino do Botao A
    gpio_set_dir(BOTAOA, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(BOTAOA);          // Habilita o pull-up interno

    gpio_init(BOTAOB);              // Inicializa o pino do Botao Bs
    gpio_set_dir(BOTAOB, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(BOTAOB);          // Habilita o pull-up interno

    // Configuração da interrupção com callback para botao A
    gpio_set_irq_enabled_with_callback(BOTAOA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler_BOTAO);

    // Configuração da interrupção com callback para botao B
    gpio_set_irq_enabled_with_callback(BOTAOB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler_BOTAO);

    // Loop principal
    while (true)
    {
        ledFunc();
    }
}

