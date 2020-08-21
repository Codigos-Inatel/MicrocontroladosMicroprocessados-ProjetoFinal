# Projeto-E209
Projeto prático final da disciplina de Microcontrolados e Microprocessados do curso de Engenharia da Computação do Inatel.

Tema proposto: Controle de umidade do solo para jardim aromático
Deve-se medir o valor da umidade de 2 pontos do jardim. Em função da umidade medida, a válvula de irrigação (simulada por um servo-motor) deve ser acionada de forma proporcional (utilizar 5 escalas de ajustes). O tempo ON e OFF são ajustados através de um menu inicial via UART. O projeto conta com um sensor (Simulado com botão) que indica a presença de água no cano que vai até a válvula, em qualquer momento caso não exista agua o processo deve ser parado e uma mensagem enviada indicando a falta de água, assim que a água voltar o processo se reinicializa. Além disso, deve ser enviada uma mensagem de segurança a cada 10 segundos para o PC, avisando que o Dispositivo se encontra ativo.

Foi utilizado o MSP430G2553.

Projeto realizado no primeiro semestre de 2019 por Vanessa Swerts, Eduardo Holzbach e Kaique Santos.
