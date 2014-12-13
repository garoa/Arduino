#include <EEPROM.h>
#include <Wire.h>

// Configurações na EEProm
#define EP_MODO  0
#define EP_ENDER 1

// Modos de operação
enum { MASTER, SLAVE, CFG } modo;

// Endereço (se escravo)
uint8_t ender;

// Iniciação
void setup (void)
{
  uint8_t aux;
  
  Serial.begin (9600);
  Serial.println ("DojoI2C v1.00");
  Serial.print ("Modo: ");
  aux = EEPROM.read (EP_MODO);
  ender = EEPROM.read (EP_ENDER);
  if (aux == 0)
  {
    modo = MASTER;
    Serial.println ("Master");
  }
  else if (aux == 1)
  {
    modo = SLAVE;
    Serial.print ("slave ");
    Serial.println (ender);
  }
  else
  {
    modo = CFG;
    Serial.println ("config");
  }
  init_display ();
  if (modo != CFG)
    init_i2c ();
}

// inicia os pinos conectados ao display
// (pisca para indicar que funciona)
void init_display (void)
{
  for (int pino = 6; pino <= 13; pino++)
  {
    pinMode (pino, OUTPUT);
    digitalWrite (pino, HIGH);
    delay (100);
    digitalWrite (pino, LOW);
  }
}

// inicia a comunicação I2C conforme o modo atual
void init_i2c()
{
  if (modo == MASTER)
  {
    Wire.begin();
  }
  else
  {
    Wire.begin (ender);
    Wire.onReceive (aciona_disp);
    Wire.onRequest (le_pot);
  }
}

// Tratamento de recepção no slave
void aciona_disp (int n)
{
  write_disp (Wire.read());
}

// Escreve no display
void write_disp (uint8_t cmd)
{
  for (int pino = 6; pino <= 13; pino++)
  {
    digitalWrite (pino, (cmd & 1) ? HIGH : LOW);
    cmd >>= 1;
  }
}

// Tratamento de pedido de envio
void le_pot (void)
{
  int leitura = analogRead(A0) >> 2;
  Wire.write ((uint8_t) leitura);
}

// Loop principal
void loop (void)
{
  if (Serial.available() && (Serial.read() == 'C'))
    modo = CFG;
  if (modo == CFG)
    config();
  else if (modo == MASTER)
  {
    // para teste
    uint8_t leitura = analogRead(A0) >> 2;
    write_disp (leitura);
    
    for (int addr = 1; addr <= 2; addr++)
    {
      Wire.beginTransmission(addr);
      Wire.write (leitura);
      Wire.endTransmission();
    }
    
    for (int addr = 1; addr <= 2; addr++)
    {
      Wire.requestFrom(addr, 1);
      delay (10);
      leitura = Wire.read();
      Serial.print ("Pot");
      Serial.print (addr);
      Serial.print (": ");
      Serial.println (leitura);
    }
    delay (100);
  }
  else
  {
    // slave é acionado pelo I2C
  }
}

void config(void)
{
  char resp = ' ';
  Serial.println ("Modo (M/S)?");
  while (true)
  {
    resp = Serial.read();
    if ((resp == 'm') || (resp == 'M') || (resp == 's') || (resp == 'S'))
      break;
  }
  if ((resp == 'm') || (resp == 'M'))
    modo = MASTER;
  else
  {
    modo = SLAVE;
    Serial.println ("Endereco (1 a 9)?");
    while (true)
    {
      resp = Serial.read();
      if ((resp >= '1') && (resp <= '9'))
        break;
    }
    ender = resp - '0';
  }
  EEPROM.write (EP_MODO, modo);
  EEPROM.write (EP_ENDER, ender);
  init_i2c();
}


