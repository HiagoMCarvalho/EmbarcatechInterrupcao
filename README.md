# PIO Matrix - Controlador de Matriz de LEDs para Raspberry Pi Pico

## Descrição
Este projeto implementa um controlador para uma matriz de LEDs utilizando um Raspberry Pi Pico e o sistema PIO (Programmable I/O). Ele permite a exibição dos números de 0 a 9 em uma matriz de LEDs Neopixel (WS2812), garantindo alto desempenho e baixo uso da CPU.

## Funcionalidades
- Controle eficiente de LEDs RGB utilizando o PIO do Raspberry Pi Pico.
- Suporte para animações e exibição de números na matriz de LEDs.
- Compatibilidade com o simulador Wokwi para testes sem hardware físico.
- Configuração simples através do CMake.

## Estrutura do Projeto
```
pio_matrix-master/
├── .gitignore
├── .vscode/                # Configurações do VS Code
├── CMakeLists.txt          # Configuração do build com Pico SDK
├── diagram.json            # Esquema de conexões para simulação no Wokwi
├── pico_sdk_import.cmake   # Importação do SDK do Raspberry Pi Pico
├── pio_matrix.c            # Lógica principal do controle da matriz
├── pio_matrix.pio          # Código PIO para comunicação com os LEDs
├── wokwi.toml              # Configuração da simulação no Wokwi
```

## Requisitos
- **Hardware**: Raspberry Pi Pico, LEDs Neopixel (WS2812)
- **Software**:
  - [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk)
  - CMake
  - Simulador Wokwi (opcional)

## Como Compilar e Executar
1. Clone o repositório e acesse a pasta do projeto:
   ```sh
   git clone https://github.com/HiagoMCarvalho/EmbarcatechInterrupcao.git
   cd pio_matrix-master
   ```
2. Configure e compile o projeto:
   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```
3. Carregue o firmware no Raspberry Pi Pico copiando o arquivo `.uf2` para a unidade correspondente.

## Simulação no Wokwi
1. Acesse [Wokwi](https://wokwi.com/) e importe os arquivos `diagram.json` e `wokwi.toml`.
2. Inicie a simulação para visualizar o funcionamento do projeto sem precisar de hardware físico.

## Licença
Este projeto é distribuído sob a licença MIT. Consulte o arquivo `LICENSE` para mais detalhes.

## Link para o vídeo
<https://drive.google.com/file/d/19OIQUytOVMvoQ2PZjhGP4l7CMhHPrWBp/view?usp=sharing>

## Autor
Desenvolvido por <https://github.com/HiagoMCarvalho>.

