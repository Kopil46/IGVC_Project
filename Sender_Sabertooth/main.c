/*
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "clock.h"
#include "gpio.h"
#include "wait.h"
#include "uart0.h"
#include "setupuart.h"
#include "math.h"
#include "uart1.h"
#include "rgb_led.h"
#include "nrf0.h"
#include "spi0.h"

#define RED_LED PORTF,1
#define BLUE_LED PORTF,2
#define GREEN_LED PORTF,3

uint8_t TxAddress[] = {0xA0,0xB0,0xA0,0xB0,0xA0};
   uint8_t TxData[32] = "Hello Worldo\n";
   uint8_t RxAddress[] = {0xA0,0xB0,0xA0,0xB0,0xA0};
   uint8_t RxData[32];

   uint8_t txPacket[32];


void prepareForTx(uint8_t *txAddress, uint8_t channel)
{
    NRF24_Init();
       waitMicrosecond(5000);
    NRF24_Tx(txAddress, channel);
//    waitMicrosecond(150);  // Let it settle in TX mode
}

void prepareForRx(uint8_t *rxAddress, uint8_t channel)
{
    NRF24_res(0);
    NRF24_InitRX();
    waitMicrosecond(5000);
    NRF24_Rx(rxAddress, channel);
//    waitMicrosecond(150);  // Let it settle in RX mode
}

void sendnrf(uint8_t *data)
{
    prepareForTx(TxAddress, 10);
    waitMicrosecond(500000);
    NRF24_Transmit(data, 32);
    waitMicrosecond(1000000);
}

// Initialize Hardware
void initHw()
{
    // Initialize system clock to 40 MHz
    initSystemClockTo40Mhz();

    // Enable clocks
    enablePort(PORTF);
    _delay_cycles(3);

    // Configure LED and pushbutton pins
    selectPinPushPullOutput(RED_LED);
    selectPinPushPullOutput(GREEN_LED);
    selectPinPushPullOutput(BLUE_LED);
    initRgb();
}

putnum(uint8_t number)
{
    while (UART1_FR_R & UART_FR_TXFF);  // Wait if TX FIFO full
        UART1_DR_R = number;
}

#define MAX_CHARS 80
char strInput[MAX_CHARS+1];
char* token;
uint8_t count = 0;
uint8_t SYN_SEND = 0;



void processSerial()
{
    bool end;
    char c;

    if (kbhitUart0())
    {
        c = getcUart0();

        end = (c == 13) || (count == MAX_CHARS);
        if (!end)
        {
            if ((c == 8 || c == 127) && count > 0)
                count--;
            if (c >= ' ' && c < 127)
                strInput[count++] = c;
        }
        else
        {
            strInput[count] = '\0';
            count = 0;
            token = strtok(strInput, " ");
            if (strcmp(token, "Forward") == 0)
            {
                token = strtok(NULL, " ");
                if (strcmp(token, "Full") == 0)
                {
                    putnum(30);
                    waitMicrosecond(10000);
                    putnum(128+30);
                    setRgbColor(0, 0, 1023);

                }

                else  if (strcmp(token, "Half") == 0)
                {
                    putnum(30);
                    waitMicrosecond(10000);
                    putnum(128+30);
                    setRgbColor(0, 0, 200);

                    strcpy((char*)txPacket, "FH\n");
                    sendnrf(txPacket);
                }
            }

            if (strcmp(token, "Backward") == 0)
            {
                token = strtok(NULL, " ");
                if (strcmp(token, "Full") == 0)
                    {
                    putnum(95);
                    waitMicrosecond(10000);
                    putnum(215);
                    setRgbColor(1023,0, 0);
                    }
                else if (strcmp(token, "Half") == 0)
                    {

                    putnum(95);
                    waitMicrosecond(10000);
                    putnum(215);
                    setRgbColor(200,0, 0);
                    strcpy((char*)txPacket, "BH\n");
                    sendnrf(txPacket);
                    }

                }
                if (strcmp(token, "Right") == 0)
                    {
                    token = strtok(NULL, " ");
                    if (strcmp(token, "Full") == 0)
                            {
                                putnum(30);
                                waitMicrosecond(10000);
                                putnum(192);
                                setRgbColor(0, 1023, 0);
                            }
                    if (strcmp(token, "Half") == 0)
                             {
                        putnum(30);
                        waitMicrosecond(10000);
                        putnum(192);
                        setRgbColor(0, 200, 0);
                        strcpy((char*)txPacket, "RH\n");
                        sendnrf(txPacket);
                             }
                    }
                if (strcmp(token, "Left") == 0)
                    {
                    token = strtok(NULL, " ");
                    if (strcmp(token, "Full") == 0)
                            {
                            putnum(128+30);
                            waitMicrosecond(10000);
                            putnum(64);
                            setRgbColor(1023, 1023, 1023);


                            }
                    if (strcmp(token, "Half") == 0)
                             {
                        putnum(128+30);
                        waitMicrosecond(10000);
                        putnum(64);
                        setRgbColor(200, 200, 200);
                        strcpy((char*)txPacket, "LH\n");
                        sendnrf(txPacket);
                             }
                    }
                if (strcmp(token, "Stop") == 0)
                                    {
                    putnum(64);
                    waitMicrosecond(10000);
                    putnum(192);
                    setRgbColor(0,0,0);
                    strcpy((char*)txPacket, "ST\n");
                    sendnrf(txPacket);
                                    }
        }
    }
}




int main(void)
{
  initHw();
  initUart0();
  setUart0BaudRate(19200, 40e6);

  initUart1();
   setUart1BaudRate(9600, 40000000);   // 9600 baud, 40 MHz sysclk

   initSpi();



      while (true)

      {
          prepareForTx(TxAddress, 10);
          processSerial();


      }
}
*/

// TM4C123GH6PM  -- NRF24 Sender (UART0 text -> RF command)
// Build with your existing libs: clock/gpio/wait/uart0/uart1/spi0/nrf0/rgb_led

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "tm4c123gh6pm.h"
#include "clock.h"
#include "gpio.h"
#include "wait.h"
#include "uart0.h"
#include "uart1.h"
#include "rgb_led.h"
#include "spi0.h"
#include "nrf0.h"

// ---------- Forward prototypes for your NRF driver (avoid implicit-declare) ----------
void     NRF_WR_REG(uint8_t reg, uint8_t data);
uint8_t  NRF_READ_REG(uint8_t reg);
void     NRF_COMMAND(uint8_t cmd);
void     NRF_WR_MULTI(uint8_t reg, uint8_t *data, uint8_t len);

void     NRF24_Init(void);
void     NRF24_InitRX(void);
void     NRF24_Tx(uint8_t *Address, uint8_t channel);
void     NRF24_Rx(uint8_t *Address, uint8_t channel);
uint8_t  NRF24_Transmit(uint8_t *data, uint8_t len);
void     NRF24_Receive(uint8_t *data, uint8_t len);
uint8_t  isDataAvailable (int num);

// ---------- LED pins ----------
#define RED_LED   PORTF,1
#define BLUE_LED  PORTF,2
#define GREEN_LED PORTF,3

// ---------- RF addressing/channel ----------
static uint8_t TxAddress[5] = {0xA0,0xB0,0xA0,0xB0,0xA0};
#define RF_CHANNEL 10

// ---------- UART1 helper (Sabertooth) ----------
static inline void putnum(uint8_t number)
{
    while (UART1_FR_R & UART_FR_TXFF);
    UART1_DR_R = number;
}

// ---------- HW init ----------
static void initHw(void)
{
    initSystemClockTo40Mhz();

    enablePort(PORTF);
    _delay_cycles(3);
    selectPinPushPullOutput(RED_LED);
    selectPinPushPullOutput(GREEN_LED);
    selectPinPushPullOutput(BLUE_LED);
    initRgb();

    initUart0();
    setUart0BaudRate(19200, 40000000);

    initUart1();
    setUart1BaudRate(9600, 40000000);

    initSpi();
}

// Clear IRQs: RX_DR (bit6), TX_DS (bit5), MAX_RT (bit4)
static inline void rf_clear_irqs(void)
{
    NRF_WR_REG(STATUS, (1<<6)|(1<<5)|(1<<4));
}

// One-time: PTX setup
static void rf_prepare_tx(void)
{
    NRF24_Init();
    waitMicrosecond(5000);
    NRF24_Tx(TxAddress, RF_CHANNEL);
    rf_clear_irqs();
    NRF_COMMAND(FLUSH_TX);
    NRF_COMMAND(FLUSH_RX);
}

// Send ascii command like "FH\n"
static void rf_send_cmd(const char *cmd)
{
    uint8_t buf[13];
    int n;
    uint8_t st;
    uint32_t i;

    for (i = 0; i < sizeof(buf); i++)
        buf[i] = 0;

    n = (int)strlen(cmd);
    if (n > (int)sizeof(buf))
        n = (int)sizeof(buf);

    for (i = 0; i < (uint32_t)n; i++)
        buf[i] = (uint8_t)cmd[i];

    (void)NRF24_Transmit(buf, (uint8_t)n);

    // Poll STATUS for TX_DS or MAX_RT, timeout ~30 ms
    for (i = 0; i < 6000U; i++)
    {
        st = NRF_READ_REG(STATUS);
        if (st & (1<<5))          // TX_DS
        {
            rf_clear_irqs();
            break;
        }
        if (st & (1<<4))          // MAX_RT
        {
            rf_clear_irqs();
            NRF_COMMAND(FLUSH_TX);
            break;
        }
        waitMicrosecond(5);
    }
}

// ---------- UART0 command parser ----------
#define MAX_CHARS 80
static char strInput[MAX_CHARS+1];
static uint8_t count = 0;

static void do_stop_rf(void)
{
    setRgbColor(0,0,0);
    rf_send_cmd("ST\n");
    // Also stop locally if wanted
    putnum(64); waitMicrosecond(10000); putnum(192);
}

static void processSerial(void)
{
    char c;
    bool end;
    char *token;

    while (kbhitUart0())
    {
        c = getcUart0();
        end = (c=='\r') || (c=='\n') || (count==MAX_CHARS);

        if (!end)
        {
            if ((c==8 || c==127) && count>0)
                count--;
            else if (c >= ' ' && c < 127)
                strInput[count++] = c;
        }
        else
        {
            strInput[count] = '\0';
            count = 0;
            if (strInput[0] == '\0')
                continue;

            token = strtok(strInput, " ");
            if (!token)
                continue;

            if (strcmp(token, "Forward") == 0)
            {
                token = strtok(NULL, " ");
                if (!token) continue;

                if (strcmp(token, "Half") == 0)
                {
                    setRgbColor(0,0,200);
                    rf_send_cmd("FH\n");
                }
                else if (strcmp(token, "Full") == 0)
                {
                    putnum(30); waitMicrosecond(10000); putnum(128+30);
                    setRgbColor(0,0,1023);
                }
            }
            else if (strcmp(token, "Backward") == 0)
            {
                token = strtok(NULL, " ");
                if (!token) continue;

                if (strcmp(token, "Half") == 0)
                {
                    setRgbColor(200,0,0);
                    rf_send_cmd("BH\n");
                }
                else if (strcmp(token, "Full") == 0)
                {
                    putnum(95); waitMicrosecond(10000); putnum(215);
                    setRgbColor(1023,0,0);
                }
            }
            else if (strcmp(token, "Right") == 0)
            {
                token = strtok(NULL, " ");
                if (!token) continue;

                if (strcmp(token, "Half") == 0)
                {
                    setRgbColor(0,200,0);
                    rf_send_cmd("RH\n");
                }
                else if (strcmp(token, "Full") == 0)
                {
                    putnum(30); waitMicrosecond(10000); putnum(192);
                    setRgbColor(0,1023,0);
                }
            }
            else if (strcmp(token, "Left") == 0)
            {
                token = strtok(NULL, " ");
                if (!token) continue;

                if (strcmp(token, "Half") == 0)
                {
                    setRgbColor(200,200,200);
                    rf_send_cmd("LH\n");
                }
                else if (strcmp(token, "Full") == 0)
                {
                    putnum(128+30); waitMicrosecond(10000); putnum(64);
                    setRgbColor(1023,1023,1023);
                }
            }
            else if (strcmp(token, "Stop") == 0)
            {
                do_stop_rf();
            }
        }
    }
}

// ---------- main ----------
int main(void)
{
    initHw();
    rf_prepare_tx();             // one-time PTX

    while (true)
    {
        processSerial();
    }
}

