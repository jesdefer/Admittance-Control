#include <SPI.h>

#define ADS_RDY_PIN    3 //ADS1256 data ready
#define ADS_CS_PIN    6 //ADS1256 chip select

volatile int DRDY_state = HIGH;

void initADS() {
  SPI.begin();
  
  attachInterrupt(digitalPinToInterrupt(ADS_RDY_PIN), DRDY_Interupt, FALLING);
  pinMode(ADS_CS_PIN, OUTPUT);
  pinMode(ADS_RDY_PIN, INPUT);

  delay(1000);

  //now reset the ADS
  Reset();

  //let the system power up and stabilize (datasheet pg 24)
  delay(2000);

  SetRegisterValue(MUX, P_AIN0|N_AIN1); //set the mux register
  SetRegisterValue(ADCON, PGA_64); //set the adcon register
  SetRegisterValue(DRATE, DR_1000); //set the drate register

  //let it settle
  delay(2000);

  //then do calibration
  SendCMD(SELFCAL); //send the calibration command

  delay(5);
}


int32_t read_load_cell() {
  int32_t adc_val;
  waitforDRDY();
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS_CS_PIN, LOW);
  delayMicroseconds(5);
  SPI.transfer(RDATA);
  adc_val |= SPI.transfer(NOP);
  adc_val <<= 8;
  adc_val |= SPI.transfer(NOP);
  adc_val <<= 8;
  adc_val |= SPI.transfer(NOP);
  digitalWrite(ADS_CS_PIN, HIGH);
  SPI.endTransaction();

  if (adc_val > 0x7fffff) { //if MSB == 1
    adc_val = adc_val - 16777216; //do 2's complement, keep the sign this time!
  }

  return adc_val;
}


void waitforDRDY() {
  while (DRDY_state) {}
  
  noInterrupts();
  DRDY_state = HIGH;
  interrupts();
}

void DRDY_Interupt() {
  DRDY_state = LOW;
}

long GetRegisterValue(uint8_t regAdress) {
  uint8_t bufr;
  digitalWrite(ADS_CS_PIN, LOW);
  delayMicroseconds(10);
  SPI.transfer(RREG | regAdress); // send 1st command byte, address of the register
  SPI.transfer(0x00);     // send 2nd command byte, read only one register
  delay(10);
  bufr = SPI.transfer(NOP); // read data of the register
  delay(10);
  digitalWrite(ADS_CS_PIN, HIGH);
  SPI.endTransaction();
  return bufr;
}

void SendCMD(uint8_t cmd) {
  waitforDRDY();
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS_CS_PIN, LOW);
  delayMicroseconds(10);
  SPI.transfer(cmd);
  delayMicroseconds(10);
  digitalWrite(ADS_CS_PIN, HIGH);
  SPI.endTransaction();
}

void Reset() {
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS_CS_PIN, LOW);
  delayMicroseconds(10);
  SPI.transfer(RESET);
  delay(2);
  SPI.transfer(SDATAC);
  delayMicroseconds(100);
  digitalWrite(ADS_CS_PIN, HIGH);
  SPI.endTransaction();
}

void SetRegisterValue(uint8_t regAdress, uint8_t regValue) {
  uint8_t regValuePre = GetRegisterValue(regAdress);
  
  if (regValue != regValuePre) {
    delayMicroseconds(10);
    waitforDRDY();
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with SPI_SPEED, MSB first, SPI Mode1
    digitalWrite(ADS_CS_PIN, LOW);
    delayMicroseconds(10);
    SPI.transfer(WREG | regAdress); // send 1st command byte, address of the register
    SPI.transfer(0x00);   // send 2nd command byte, write only one register
    SPI.transfer(regValue);         // write data (1 Byte) for the register
    delay(10);
    digitalWrite(ADS_CS_PIN, HIGH);
    /*
    Serial.print("Write to Register 0x"); Serial.print(regAdress, HEX);
    Serial.print(" the value: "); Serial.println(regValue);*/
    
    SPI.endTransaction();
  }
}
