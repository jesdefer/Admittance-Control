#include <Wire.h>
 
#define    MPU9250_ADDRESS            0x68
 
#define    GYRO_FULL_SCALE_250_DPS    0x00  
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18
 
#define    ACC_FULL_SCALE_2_G        0x00  
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

#define M_PI 3.14159265359
#define ANGULAR_VELOCITY_CONV 131 //2^16/500
#define ACCEL_CONV 16384 //2^16/4
#define gravity_term 9.8
#define radius 0.3

int Pitch = 0.0;

float aPitch = 0.0;
float angular_velocity = 0.0;
float alpha_filter = 0.7;

float past_instant = millis();
float current_instant = millis();

int n_samples_calibration = 500;
float offset_imu = 0.0;

struct imu_data {
   double ang_pos;
   double ang_vel;
   double ang_accel;
} imu;

 
 
void init_IMU()
{
   Wire.begin();
   I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_2_G);
   I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_250_DPS);
}


void update_imu()
{  
   float delta_t;
   uint8_t Buf[14];
   I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);
 
   int16_t ax = (Buf[0] << 8 | Buf[1]);
   int16_t ay = (Buf[2] << 8 | Buf[3]);
   int16_t gy = (Buf[10] << 8 | Buf[11]);
   int16_t gz = (Buf[12] << 8 | Buf[13]);

   current_instant = millis();
   delta_t = (current_instant-past_instant)*0.001;
   aPitch = atan2f((float)ax, (float)ay) * (180/M_PI);
   angular_velocity = (float)gz/ANGULAR_VELOCITY_CONV;
   Pitch = alpha_filter*(Pitch + angular_velocity *delta_t) + (1-alpha_filter)*aPitch - offset_imu;

   imu.ang_pos= -Pitch*(M_PI/180) ;
   imu.ang_vel = -angular_velocity*(M_PI/180);
   imu.ang_accel = (1/radius)*(gravity_term*(-(float)ay/ACCEL_CONV) + gravity_term*cos(imu.ang_pos));
   
   past_instant = current_instant;
}

void calibration_imu(void){
  offset_imu = 0.0;
   for (int j = 0; j<n_samples_calibration; j++){
   update_imu();
   offset_imu = Pitch + offset_imu;
  }
  offset_imu = offset_imu/n_samples_calibration;
  Serial.println("IMU Calibrated! " + String(offset_imu));
}



//Funcion auxiliar lectura
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
   Wire.beginTransmission(Address);
   Wire.write(Register);
   Wire.endTransmission();
 
   Wire.requestFrom(Address, Nbytes);
   uint8_t index = 0;
   while (Wire.available())
      Data[index++] = Wire.read();
}
 
 
// Funcion auxiliar de escritura
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
   Wire.beginTransmission(Address);
   Wire.write(Register);
   Wire.write(Data);
   Wire.endTransmission();
}
