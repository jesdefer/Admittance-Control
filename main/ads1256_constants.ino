#define SPI_SPEED 5000

#define   STATUS    0x00 //Status Control
#define   MUX       0x01 //Multiplexer Control
#define   ADCON     0x02 //A/D Control
#define   DRATE     0x03 //A/D Data Rate

/* MUX - Multiplexer Control Register 0 (see p31 - bring together with bitwise OR | */
/* BIT7  - BIT6  -  BIT5   -  BIT4   -  BIT3   -  BIT2   -  BIT1   -  BIT0 */
/* PSEL3 - PSEL2 -  PSEL1  -  PSEL0  -  NSEL3  -  NSEL2   - NSEL1   - NSEL0 */
#define MUX_RESET 0x01      // Reset MUX0 Register
/* PSEL3:0 Positive input channel selection bits */
#define P_AIN0   0b00000000 //(default)
#define P_AIN1   0b00010000
#define P_AIN2   0b00100000
#define P_AIN3   0b00110000
#define P_AIN4   0b01000000
#define P_AIN5   0b01010000
#define P_AIN6   0b01100000
#define P_AIN7   0b01110000
#define P_AINCOM 0b10000000
/* NSEL3:0 Negative input channel selection bits */
#define N_AIN0   0b00000000
#define N_AIN1   0b00000001 //(default)
#define N_AIN2   0b00000010
#define N_AIN3   0b00000011
#define N_AIN4   0b00000100
#define N_AIN5   0b00000101
#define N_AIN6   0b00000110
#define N_AIN7   0b00000111
#define N_AINCOM 0b00001000

/*ADCON - A/D Control Register 0 ( see p31)*/
/* BIT7 - BIT6   -  BIT5   -  BIT4   -  BIT3   -  BIT2   -  BIT1   -  BIT0 */
/* 0    - CLK1   -  CLK0   -  SDCS1  -  SDCS0  -  PGA2   -  PGA1   -  PAG0 */
#define ADCON_RESET 0x20 // Reset ADCON Register
/*The Sensor Detect Current Sources can be activated to verify the integrity of an external sensor supplying a signal to the
ADS1255/6. A shorted sensor produces a very small signal while an open-circuit sensor produces a very large signal.*/
/*PGA3:0 Programmable Gain Amplifier Setting*/
#define PGA_1  0b00100000
#define PGA_2  0b00100001
#define PGA_4  0b00100010
#define PGA_8  0b00100011
#define PGA_16 0b00100100
#define PGA_32 0b00100101
#define PGA_64 0b00100111

/*DRATE - A/D Data Rate Register 0 ( see p32)*/
/* BIT7 - BIT6   -  BIT5   -  BIT4   -  BIT3   -  BIT2   -  BIT1   -  BIT0 */
/* DR7  - DR6    -  DR5    -  DR4    -  DR3    -  DR2    -  DR1    -  DR0 */
#define DRATE_RESET 0xF0 // Reset DRATE Register
/*DR7:0 Data Rate Setting*/
#define DR_30000 0b11110000 //30.000 SPS (default)
#define DR_15000 0b11100000 //15.000 SPS
#define DR_7500  0b11010000 //7.500 SPS
#define DR_3750  0b11000000 //3.750 SPS
#define DR_2000  0b10110000 //2.000 SPS
#define DR_1000  0b10100001 //1.000 SPS
#define DR_500   0b10010010 //500 SPS
#define DR_100   0b10000010 //100 SPS
#define DR_60    0b01110010 //60 SPS
#define DR_50    0b01100011 //50 SPS
#define DR_30    0b01010011 //30 SPS
#define DR_25    0b01000011 //25 SPS
#define DR_15    0b00110011 //15 SPS
#define DR_10    0b00100011 //10 SPS
#define DR_5     0b00010011 //5 SPS
#define DR2_5    0b00000011 //2,5 SPS

/* SPI COMMAND DEFINITIONS (p34) */
/*SYSTEM CONTROL */
#define   WAKEUP    0x00  //Exit Sleep Mode
#define   STANDBY   0xFD  //Enter Sleep Mode
#define   SYNC      0xFC    //Synchornize the A/D Conversion
#define   RESET     0xFE  //Reset To Power UP values
#define   NOP       0xFF  //No operation
/*DATA READ*/
#define   RDATA     0x01  //Read data once
#define   RDATAC    0x03  //Read data continously
#define   SDATAC    0x0F  //Stop reading data continously
/*READ REGISTER */
#define   RREG      0x10  //Read From Register
#define   WREG      0x50  //Write To Register
/*Calibration */
#define   SYSOCAL   0xF3  //System Offset Calibration
#define   SYSGCAL   0xF2  //System Gain Calibration
#define   SELFCAL   0xF0  //Self Offset Calibration

