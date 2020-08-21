#include "msp430g2553.h"
#include <stdio.h>

unsigned char cont = 0;
unsigned char contPWM = 0; 
unsigned long int var_armazenagem; 
unsigned long int aux; 
unsigned int valor;
unsigned int valor_on;
unsigned int valor_off;
unsigned int tensao;
unsigned int tam;
unsigned char i;
unsigned int conv1;
unsigned int conv2;
unsigned long int med = 0;
unsigned int LeAD0();
unsigned int x;
unsigned char TEMPO_ON=0;
unsigned char TEMPO_OFF=0;
unsigned char tempo_off;
unsigned char tempo_on;
void ConfigAD();
void UART_TX();
void configUART();


unsigned int TAMANHO = 2;
unsigned int flag=0;
unsigned int flagSemAgua = 0;

unsigned int LeAD0();

char mensagem_tx[20];
char mensagem_rx[32];


void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  DCOCTL = 0;
  BCSCTL1 = CALBC1_1MHZ;              
  DCOCTL = CALDCO_1MHZ;
  
  P1DIR |= BIT6;           //Configura P1.0 e P1.6 como saídas
  
  P1REN |= BIT3 + BIT4;                //Habilita resistor interno
  P1OUT |= BIT3 + BIT4;                //Configura como pull-up
  
  
  P1IE |= BIT3;                 //Habilita interrupcao no pino 3
  P1IES |= BIT3;                //Declara como transicao de SUBIDA
  P1IFG &= ~BIT3;               //Limpa o flag de interrupcao
  
  
  //Configura timer1 A1                      //para 500ms
  TA1CTL = TA1CTL|(MC_1 + TASSEL_2 + ID_3);  //Habilita modo UP, Clock SMCLK e Divisor 8
  TA1CCR0 = 62500;                           //intervalo para uma contagem de 500ms
  TA1CCTL0 = CCIE;                           //habilita o funcionamento do timer
  
  //Configurando PWM 
  P1SEL |= BIT6;        //CONFIGURANDO PWM - mudar para servo-motor 
  TA0CCTL1 = OUTMOD_7;  // CONFIGURANDO EM RESET/SET 
  TA0CCR0 = 20000-1;    // LIMITE SUPERIOR 
  TA0CCR1 = 0;          // LIMITE INFERIOR          
  TA0CTL = TASSEL_2 + ID_0 + MC_1; // CONFIGURANDO TIMER
  
  //Config ADC
  ConfigAD(BIT0 + BIT5);
  //Config UART
  configUART(); 
  _BIS_SR(GIE);                         //habilita interrupção global       
  
  UART_TX("Entre com TON: \r\n");
  x=0;
  do
  {
    if ( mensagem_rx[0] !=0 && mensagem_rx[1] !=0)
    {
      valor_on = (mensagem_rx[0]-48)*10 + (mensagem_rx[1]-48)*1 ;
      tempo_on = valor_on*2;
      if( valor_on<=10)
        x=1;
      
    }
    
  }while(x==0);
  mensagem_rx[1] = 0;
  mensagem_rx[0] = 0;
  UART_TX("Entre com TOF: \r\n");
  x=0;
  do
  {
    if ( mensagem_rx[0] !=0 && mensagem_rx[1] !=0)
    {
      valor_off = (mensagem_rx[0]-48)*10 + (mensagem_rx[1]-48)*1;
      tempo_off = valor_off*2;
      if(valor_off<=10)
        x=1;
    }
  }while(x==0);
  UART_TX("OK");
  
  for(;;)
  {  
    //POTENCIOMETROS 
    conv1 = LeAD0(INCH_0);
    conv2 = LeAD0(INCH_5);
    med = (conv1 + conv2)/2;
    valor = (unsigned int) med;
    med = 0;
    
    //Conversao para tensao 
    aux = (unsigned long int) valor; 
    aux = (aux * 3300)/1023;
    tensao = (unsigned int)  aux; 
    
    //lançar a condição de tempo aqui 
    if ( tensao <  660)
    {
      while(TEMPO_ON<=tempo_on && flag==0)
      {
        TA0CCR1 = 700; // INCREMENTA  minimo do servo motor
      }
      flag=1;
      TEMPO_OFF=0;
      while(TEMPO_OFF<=tempo_off && flag==1)
      {
        TA0CCR1 = 700;
      }
      flag=0;
      TEMPO_ON=0;
      
    }
    else if (tensao >= 660 && tensao < 1320)
    {
      while(TEMPO_ON<=tempo_on && flag==0)
      {
        TA0CCR1 = 1275;
      }
      flag=1;
      TEMPO_OFF=0;
      while(TEMPO_OFF<=tempo_off && flag==1)
      {
        TA0CCR1 = 700;
      }
      flag=0;
      TEMPO_ON=0;
      
    }
    else if (tensao >= 1320 && tensao < 1980)
    {
      while(TEMPO_ON<=tempo_on && flag==0)
      {
        TA0CCR1 = 1850;
      }
      flag=1;
      TEMPO_OFF=0;
      while(TEMPO_OFF<=tempo_off && flag==1)
      {
        TA0CCR1 = 700;
      }
      flag=0;
      TEMPO_ON=0;
      
    }
    else if (tensao >= 1980 && tensao < 2640)
    {
      while(TEMPO_ON<=tempo_on && flag==0)
      {
        TA0CCR1 = 2825;
      }
      flag=1;
      TEMPO_OFF=0;
      while(TEMPO_OFF<=tempo_off && flag==1)
      {
        TA0CCR1 = 700;
      }
      flag=0;
      TEMPO_ON=0;
      
    }
    else if (tensao >= 2640 && tensao < 3300){
      while(TEMPO_ON<=tempo_on && flag==0)
      {
        TA0CCR1 = 3000-1;
      }
      flag=1;
      TEMPO_OFF=0;
      while(TEMPO_OFF<=tempo_off && flag==1)
      {
        TA0CCR1 = 700;
      }
      flag=0;
      TEMPO_ON=0;
      
    }
    else
    { //SE LIMITE INFERIOR IGUAL LIMITE  SUPERIOR 
      TA0CCR1 = 700;   // ZERA LIMITE INFERIOR 
      
    }
  }
}
void ConfigAD( unsigned int entradas){
  
  // Configurando ADC
  ADC10AE0 |= entradas; // Configurando entrada ADC
  ADC10CTL1 = ADC10SSEL_3; //configurando clock 
  
}
unsigned int LeAD0(unsigned int canal)
{
  ADC10CTL0 &=~(ADC10ON); //desliga o conversor
  ADC10CTL0 &=~(ENC + ADC10SC); //para e desabilita as conversões 
  ADC10CTL1 &= ~INCH_7;  //Configurando pino q queremos converter
  ADC10CTL1 = canal;  //Configurando pino q queremos converter     
  ADC10CTL0 |=(ADC10ON); //liga o conversor
  
  for (i = 0; i < 100; i++) {
    ADC10CTL0|= (ENC + ADC10SC);
    
    while ((ADC10CTL0&ADC10IFG)==0){};
    var_armazenagem = var_armazenagem + ADC10MEM;               
  }
  
  //Valor de 1 amostragem 
  var_armazenagem = var_armazenagem/100;
  return var_armazenagem;
}

//FUNÇÃO TIMER 
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR( void )  // Mensagem UART
{
  // TA0CCR0
  TEMPO_ON++;
  TEMPO_OFF++;
  cont++;                               // Contador tempo
  if(cont == 20)                        // 20 x 500ms = 10s
  {
    //envia mensagem UART avisando que o dispositivo está ativo
    UART_TX("Dispositivo Ativo  \r\n");
    cont=0;
  }
  
  TA1CCTL0 &=~ CCIFG;                   //limpa flag de interrupção
}

//FUNÇÃO INTERRUPCAO EXTERNA - SEM AGUA
#pragma vector = PORT1_VECTOR
__interrupt void interrupcao_bit3( void )   
{
  flagSemAgua = 0;
  while((P1IN&BIT4)==BIT4 && (P1IN&BIT3)==0)//ENQUANTO OUTRO BOTÃO(BIT4) NÃO PRESS - FICA NA INTERRUPÇÃO
  {
    //envia mensagem UART q não tem agua
    if ( flagSemAgua == 0)
    {
      UART_TX("Verificar! Problemas Aparentes  \r\n");
      flagSemAgua = 1;
    }
    while((P1IN&BIT4)==BIT4)
    {
      
    } 
  }
  P1IFG &= ~BIT3;  //Limpa o flag de interrupcao
  //Zerando a contagem
  TEMPO_ON=0;
  TEMPO_OFF=0;
  cont=0;
}


// Função configura uart
void configUART(void)
{ 
  
  P1SEL |= BIT1+BIT2;                // Seleciona os pinos P1.1 e P1.2 para serem UART   
  P1SEL2 |= BIT1+BIT2;               // Seleciona os pinos P1.1 e P1.2 para serem UART   
  UCA0CTL1 |= UCSSEL_2;             // Utilizar clck principal  
  UCA0BR0 = 104;                    // baudrate 9600 
  UCA0BR1 = 0;                      // baudrate 9600   
  UCA0MCTL = UCBRS0;                // Modulation UCBRSx = 1;
  UCA0CTL1 &= ~UCSWRST;             // Inicia UART   
  IE2 |= UCA0RXIE;
  
} 

void UART_TX (char * tx_data) //Função de envio de mensagens “STRINGS” 
{ 
  unsigned int i=0; 
  
  while(tx_data[i]) // Espera enviar todos caracteres da STRING    
  {        
    while ((UCA0STAT & UCBUSY)); //espera terminar o envio da ultima informação        
    UCA0TXBUF = tx_data[i]; // envia o elemento na posição i      
    i++; // incrementa posição do vetor     
  } 
} 

#pragma vector = USCIAB0RX_VECTOR 
__interrupt void USCI0RX_ISR(void)
{   
  mensagem_rx[tam] = UCA0RXBUF;             // TX -> RXed character 
  tam++;   
  if(tam == TAMANHO)
  {     
    tam=0;   
  }  
  
  IFG2=IFG2&~UCA0RXIFG; 
}
