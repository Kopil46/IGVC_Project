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
                             }
                    }
                if (strcmp(token, "Stop") == 0)
                                    {
                    putnum(64);
                    waitMicrosecond(10000);
                    putnum(192);
                    setRgbColor(0,0,0);
                                    }
        }
    }
}

char message[33];

void NRFPROCESS()
{
            strInput[0] = message[0];
            strInput[1] = message[1];
            strInput[2] = message[2];
            strInput[3] = '\0';
            token = strtok(strInput, "\n");
            if (strcmp(token, "FH") == 0)
            {

                    putnum(30);
                    waitMicrosecond(10000);
                    putnum(128+30);
                    setRgbColor(0, 0, 200);

            }

            if (strcmp(token, "BH") == 0)
            {


                    putnum(95);
                    waitMicrosecond(10000);
                    putnum(215);
                    setRgbColor(200,0, 0);


                }
                if (strcmp(token, "RH") == 0)
                    {

                        putnum(30);
                        waitMicrosecond(10000);
                        putnum(192);
                        setRgbColor(0, 200, 0);

                    }
                if (strcmp(token, "LH") == 0)
                    {

                        putnum(128+30);
                        waitMicrosecond(10000);
                        putnum(64);
                        setRgbColor(200, 200, 200);

                    }
                if (strcmp(token, "ST") == 0)
                                    {
                    putnum(64);
                    waitMicrosecond(10000);
                    putnum(192);
                    setRgbColor(0,0,0);

        }
    }



//uint8_t RxData[13];




int main(void)
{
  initHw();
  initUart0();
  setUart0BaudRate(19200, 40e6);

  initUart1();
  initSpi();
   setUart1BaudRate(9600, 40000000);   // 9600 baud, 40 MHz sysclk
   prepareForRx(RxAddress, 10);


      while (true)

      {

//          processSerial();

          if (isDataAvailable(1))
               {
                       NRF24_Receive(RxData, 32);        // Fills RxData buffer
                       memcpy(message, RxData, 32);      // Copy to message
                       message[32] = '\0';               // Null-terminate
                       putsUart0(message);               // Print to UART
                       NRFPROCESS();
                }
          prepareForRx(RxAddress, 10);

      }
}

*/
// TM4C123GH6PM  -- NRF24 Receiver (RF command -> UART1 + LEDs)

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


//Helpers for Packet MODE ON Sabertooth

// -------- Sabertooth Packetized Serial Helpers --------
#define SAB_ADDR   130   // your requested address (>128)
#define ST_M1_FWD  0     // command bytes (from manual)
#define ST_M1_REV  1
#define ST_M2_FWD  4
#define ST_M2_REV  5

static inline void st_uart1_put(uint8_t b)
{
    while (UART1_FR_R & UART_FR_TXFF);
    UART1_DR_R = b;
//    waitMicrosecond(1000);

}

// Send one packetized command
static inline void st_send(uint8_t cmd, uint8_t data)
{
    uint8_t cksum = (uint8_t)((SAB_ADDR + cmd + data) & 0x7F);
    st_uart1_put(SAB_ADDR);
    st_uart1_put(cmd);
    st_uart1_put(data);
    st_uart1_put(cksum);
}

// Convenience wrappers
static inline void st_m1_forward(uint8_t spd)  { st_send(ST_M1_FWD, spd); }
static inline void st_m1_reverse(uint8_t spd)  { st_send(ST_M1_REV, spd); }
static inline void st_m2_forward(uint8_t spd)  { st_send(ST_M2_FWD, spd); }
static inline void st_m2_reverse(uint8_t spd)  { st_send(ST_M2_REV, spd); }

// "Stop" = send speed 0 to each motor (packetized semantics)
static inline void st_stop_all(void)
{
    st_m1_forward(0);
    st_m2_forward(0);
}


static void handle_cmd_packet(const char *msg32)
{
    // Commands you already use: "FH","BH","RH","LH","ST"
    // Tune these speeds as you prefer
    const uint8_t SPEED_HALF = 20;   // ~50%
    const uint8_t SPEED_FULL = 20;  // 100%

    char a = msg32[0];
    char b = msg32[1];

    if (a=='F' && b=='H')            // Forward Half (both wheels forward)
    {
        st_m1_forward(SPEED_HALF);
        st_m2_forward(SPEED_HALF);
        setRgbColor(0,0,200);
    }
    else if (a=='B' && b=='H')       // Backward Half
    {
        st_m1_reverse(SPEED_HALF);
        st_m2_reverse(SPEED_HALF);
        setRgbColor(200,0,0);
    }
    else if (a=='R' && b=='H')       // Pivot Right Half (left fwd, right rev)
    {
        st_m1_forward(SPEED_HALF);   // M1 forward
        st_m2_reverse(SPEED_HALF);   // M2 reverse
        setRgbColor(0,200,0);
    }
    else if (a=='L' && b=='H')       // Pivot Left Half (left rev, right fwd)
    {
        st_m1_reverse(SPEED_HALF);   // M1 reverse
        st_m2_forward(SPEED_HALF);   // M2 forward
        setRgbColor(200,200,200);
    }
    else if (a=='S' && b=='T')       // Stop
    {
        st_stop_all();
        setRgbColor(0,0,0);
    }
    else if (a=='F' && b=='F')       // (optional) Forward Full
    {
        st_m1_forward(SPEED_FULL);
        st_m2_forward(SPEED_FULL);
    }
    else if (a=='B' && b=='F')       // (optional) Backward Full
    {
        st_m1_reverse(SPEED_FULL);
        st_m2_reverse(SPEED_FULL);
    }
    else
    {
        // unknown: soft stop
        st_stop_all();
        setRgbColor(50,0,50);
        waitMicrosecond(20000);
        setRgbColor(0,0,0);
    }
}



//HELPERS FOR PACKET MODE ON SERIAL PACKATIZED MODE************************************************

// ---------- Forward prototypes for your NRF driver ----------
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

static uint8_t RxAddress[5] = {0xA0,0xB0,0xA0,0xB0,0xA0};
#define RF_CHANNEL 10

static inline void putnum(uint8_t number)
{
    while (UART1_FR_R & UART_FR_TXFF);
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
                             }
                    }
                if (strcmp(token, "Stop") == 0)
                                    {
                    putnum(64);
                    waitMicrosecond(10000);
                    putnum(192);
                    setRgbColor(0,0,0);
                                    }
        }
    }
}

void processSerial_Packet(void)
{
    const uint8_t SPEED_FULL = 20;   // 100%
    const uint8_t SPEED_FULL_Right = 20;
    const uint8_t SPEED_HALF = 20;    // ~50%

    bool end;
    char c;

    if (!kbhitUart0())
        return;

    c = getcUart0();
    end = (c == 13) || (count == MAX_CHARS);

    if (!end)
    {
        if ((c == 8 || c == 127) && count > 0)
            count--;
        else if (c >= ' ' && c < 127)
            strInput[count++] = c;
        return;
    }

    // end-of-line: terminate and parse
    strInput[count] = '\0';
    count = 0;

    token = strtok(strInput, " ");
    if (!token) return;

    // ---------- Forward ----------
    if (strcmp(token, "Forward") == 0)
    {
        token = strtok(NULL, " ");
        if (token && strcmp(token, "Full") == 0)
        {
            st_m1_forward(SPEED_FULL_Right);
            st_m2_forward(SPEED_FULL);
            setRgbColor(0, 0, 1023);
        }
        else if (token && strcmp(token, "Half") == 0)
        {
            st_m1_forward(SPEED_FULL_Right);
            st_m2_forward(SPEED_HALF);
            setRgbColor(0, 0, 200);
        }
        return;
    }

    // ---------- Backward ----------
    if (strcmp(token, "Backward") == 0)
    {
        token = strtok(NULL, " ");
        if (token && strcmp(token, "Full") == 0)
        {
            st_m1_reverse(SPEED_FULL_Right);
            st_m2_reverse(SPEED_FULL);
            setRgbColor(1023, 0, 0);
        }
        else if (token && strcmp(token, "Half") == 0)
        {
            st_m1_reverse(SPEED_FULL_Right);
            st_m2_reverse(SPEED_HALF);
            setRgbColor(200, 0, 0);
        }
        return;
    }

    // ---------- Right (pivot: left fwd, right rev) ----------
    if (strcmp(token, "Right") == 0)
    {
        token = strtok(NULL, " ");
        if (token && strcmp(token, "Full") == 0)
        {
            st_m1_forward(SPEED_FULL_Right);
            st_m2_reverse(SPEED_FULL);
            setRgbColor(0, 1023, 0);
        }
        else if (token && strcmp(token, "Half") == 0)
        {
            st_m1_forward(SPEED_FULL_Right);
            st_m2_reverse(SPEED_HALF);
            setRgbColor(0, 200, 0);
        }
        return;
    }

    // ---------- Left (pivot: left rev, right fwd) ----------
    if (strcmp(token, "Left") == 0)
    {
        token = strtok(NULL, " ");
        if (token && strcmp(token, "Full") == 0)
        {
            st_m1_reverse(SPEED_FULL_Right);
            st_m2_forward(SPEED_FULL);
            setRgbColor(1023, 1023, 1023);
        }
        else if (token && strcmp(token, "Half") == 0)
        {
            st_m1_reverse(SPEED_FULL_Right);
            st_m2_forward(SPEED_HALF);
            setRgbColor(200, 200, 200);
        }
        return;
    }

    // ---------- Stop ----------
    if (strcmp(token, "Stop") == 0)
    {
        st_stop_all();
        setRgbColor(0, 0, 0);
        return;
    }

    // Unknown command → soft stop as a safe default
    st_stop_all();
    setRgbColor(50, 0, 50);
}




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

// One-time PRX setup
static void rf_prepare_rx(void)
{
    NRF24_Init();
    waitMicrosecond(5000);
    NRF24_InitRX();
    waitMicrosecond(5000);
    NRF24_Rx(RxAddress, RF_CHANNEL);
    rf_clear_irqs();
    NRF_COMMAND(FLUSH_TX);
    NRF_COMMAND(FLUSH_RX);
}

static void handle_cmd(const char *msg32)
{
    char a, b;

    a = msg32[0];
    b = msg32[1];

    if (a=='F' && b=='H') // Forward Half
    {
        putnum(30); waitMicrosecond(10000); putnum(128+30);
        setRgbColor(0,0,200);
    }
    else if (a=='B' && b=='H') // Backward Half
    {
        putnum(95); waitMicrosecond(10000); putnum(215);
        setRgbColor(200,0,0);
    }
    else if (a=='R' && b=='H') // Right Half
    {
        putnum(30); waitMicrosecond(10000); putnum(192);
        setRgbColor(0,200,0);
    }
    else if (a=='L' && b=='H') // Left Half
    {
        putnum(128+30); waitMicrosecond(10000); putnum(64);
        setRgbColor(200,200,200);
    }
    else if (a=='S' && b=='T') // Stop
    {
        putnum(64); waitMicrosecond(10000); putnum(192);
        setRgbColor(0,0,0);
    }
    else
    {
        // unknown
        setRgbColor(50,0,50);
        waitMicrosecond(20000);
        setRgbColor(0,0,0);
    }
}

int main(void)
{
    uint8_t rx[13];
    int i;
    int have0;
    int have1;

    initHw();
    rf_prepare_rx();             // one-time PRX

    for (i = 0; i < 13; i++)
        rx[i] = 0;

    while (true)
    {


//        processSerial();
        processSerial_Packet();   //PACKET MODE***********************
        // try both pipes (your NRF24_Rx config enables P1; check both to be safe)
        have1 = isDataAvailable(1);
        have0 = isDataAvailable(0);

        st_send(16, 5);


        if (have1 || have0)
        {
            for (i = 0; i < 13; i++)
                rx[i] = 0;

            NRF24_Receive(rx, 13);     // your function reads 13 bytes and flushes

            rf_clear_irqs();

            // Optional: print for debug on UART0
            putsUart0((char*)rx);

//            handle_cmd((char*)rx);

            handle_cmd_packet((char*)rx);      //PACKET MODE*************
        }
    }
}
