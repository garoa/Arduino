O objetivo deste projeto é interligar em uma rede I2C vários Arduinos com o Garoa Dojo Shield para permitir a geração de animações coordenadas. 

https://garoa.net.br/wiki/DojoI2C

O objetivo da prova de conceito (feita em 11/12/14) foi demonstrar a capacidade de comunicação via I2C entre Arduinos.

O hardware consistiu em três Arduinos (um UNO, um DUEMILINOVE e um MEGA), cada um deles com um Garoa Dojo Shield. Além da interligação de SDA, SCL e GND, foram interligados o +5V, para que a alimentação de todos viesse da USB do mestre (conectado a um PC). Este esquema de alimentação deve ser inapropriado no caso de uma quantidade maiores de Arduinos.

O software carregado nos três foi o mesmo. A definição do modo (Master ou Slave) e do endereço (se slave) são armazenadas na EEProm. A configuração é feita através da conexão serial a um PC, por simplificação o endereço é um dígito de 1 a 9.

O formato das mensagens foi restringido a um byte. Na escrita do mestre, o byte controle os oito LEDs do display do escravo (sete segmentos mais o ponto decimal). Na leitura do mestre, o escravo informa a leitura do potenciômetro (dividida por 4 para ficar de 0 a 255). A expansão destas mensagens para controlar mais sinais de saída e ler mais entrada (como o LDR no shield) é trivial e fica como exercício para os implementadores futuros.

Como teste, o mestre comanda o display dos escravos (e o seu próprio) com o valor lido do seu potenciômetro (também dividido por quatro) e envia pela serial a leitura dos potenciômetros dos escravos. 

