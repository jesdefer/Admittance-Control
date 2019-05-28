/*
            Mega UNO SPI
            50  12  MISO
            51  11  MOSI
            52  13  SCK
 */
#include <SPI.h>
#include "mcp_can.h"
#include <mcp_can_dfs.h>

const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);

int16_t actualcurrent_data = 0;
int32_t actualposition_data = 0;
int32_t actualvelocity_data = 0;

float fromdeg_toqc = -(740/90);
float fromqc_todeg = -0.1216; //90/740
unsigned char set_position_and_current[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void spi_config(void){
  while (CAN_OK != CAN.begin(CAN_1000KBPS))
  {
    Serial.println("CAN BUS Shield init fail: Init CAN BUS Shield again");
    delay(100);
  }
  Serial.println("CAN BUS Shield init ok!");
}

void enable_motor(void){
  CAN.sendMsgBuf(0x602, 0, 8, enable_epos);
  delay(10);
}

void disable_motor(void){
  CAN.sendMsgBuf(0x602, 0, 8, disable_epos);
  delay(10);
}

void homing(void){
  CAN.sendMsgBuf(0x602, 0, 8, set_homing_mode);
  delay(10);
  enable_motor();
  CAN.sendMsgBuf(0x602, 0, 8, start_homing);
  delay(10);
}

void enter_in_position_mode(){
  CAN.sendMsgBuf(0x602, 0, 8, set_position_mode);
  delay(10);
}

void enter_in_current_mode(){
  CAN.sendMsgBuf(0x602, 0, 8, set_current_mode);
  delay(10);
}


void set_object_dictionary(void){
  disable_motor();
  enable_motor();
  CAN.sendMsgBuf(0x00, 0, 2, set_preoperational);
  delay(10);
  pdo_config();
  set_motor_dictionary();
  set_controller_dictionary();
  set_position_mode_dictionary();
  set_homing_mode_dictionary();
  set_current_mode_dictionary();
  CAN.sendMsgBuf(0x00, 0, 2, set_operational);
  delay(10);
}

void sync_pdo(void) {
  CAN.sendMsgBuf(0x80, 0, 1, pdo_sync_message);
}


void set_angle_to_motor(uint32_t angle){
  angle = angle*fromdeg_toqc;
  set_position_and_current[0] = angle & 0xFF; 
  set_position_and_current[1] = (angle >> 8) & 0xFF;
  set_position_and_current[2] = (angle >> 16) & 0xFF;
  set_position_and_current[3] = (angle >> 24) & 0xFF;
  CAN.sendMsgBuf(0x202, 0, 8, set_position_and_current);
  //delay(1);
  //sync_pdo();
}

void set_current_to_motor(int32_t current){
  set_position_and_current[4] = current & 0xFF; 
  set_position_and_current[5] = (current >> 8) & 0xFF;
  set_position_and_current[6] = (current >> 16) & 0xFF;
  set_position_and_current[7] = (current >> 24) & 0xFF;
  CAN.sendMsgBuf(0x202, 0, 8, set_position_and_current);
  delay(10);
  sync_pdo();
}

void set_motor_dictionary(void) {
  CAN.sendMsgBuf(0x602, 0, 8, continuous_current_limit);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, output_current_limit);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, pole_pair_number);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, max_motor_speed);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, thermal_time_constant);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, max_motor_accel);
  delay(10);
  Serial.println("Motor specifications loaded!"); 
}

void set_homing_mode_dictionary(void) {
  CAN.sendMsgBuf(0x602, 0, 5, homing_method);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, homing_speed_switch);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, homing_speed_zero_search);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, homing_acceleration);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, home_offset);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, homing_current_threshold);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, home_position);
  delay(10);
  Serial.println("Homing mode specifications loaded!"); 
}

void set_position_mode_dictionary(void) {
  CAN.sendMsgBuf(0x602, 0, 8, set_min_position_limit);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, set_max_position_limit);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, set_max_following_error_position);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, set_max_profile_velocity);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, set_max_acceleration);
  delay(10);
  Serial.println("Position mode specifications loaded!"); 
}
void set_current_mode_dictionary(void){
  CAN.sendMsgBuf(0x602, 0, 8, maximal_speed_in_current_mode);
  delay(10);
  Serial.println("Current mode specifications loaded!");
}

void set_controller_dictionary(void) {
  CAN.sendMsgBuf(0x602, 0, 8, current_regulator_P_gain);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, current_regulator_I_gain);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, speed_regulator_P_gain);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, speed_regulator_I_gain);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, speed_regulator_vel_ff);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, speed_regulator_accel_ff);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, position_regulator_P_gain);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, position_regulator_I_gain);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, position_regulator_D_gain);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, position_regulator_vel_ff);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, position_regulator_accel_ff);
  delay(10);
  Serial.println("Control parameters loaded!"); 
}

void pdo_config(void) {
  CAN.sendMsgBuf(0x602, 0, 8, rpdo1_parameter_COBID);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 5, rpdo1_parameter_transmission_type);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, rpdo1_mapping_disable);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, rpdo1_mapping_position);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, rpdo1_mapping_current);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, rpdo1_mapping_nobjects);
  delay(10);
  
  CAN.sendMsgBuf(0x602, 0, 8, tpdo1_parameter_COBID);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 5, tpdo1_parameter_transmission_type);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, tpdo1_mapping_disable);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, tpdo1_mapping_position);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, tpdo1_mapping_velocity);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, tpdo1_mapping_nobjects);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, tpdo2_parameter_COBID);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 5, tpdo2_parameter_transmission_type);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, tpdo2_mapping_disable);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, tpdo2_mapping_current);
  delay(10);
  CAN.sendMsgBuf(0x602, 0, 8, tpdo2_mapping_nobjects);
  delay(10);
  
  Serial.println("PDOs configured!");
  
}

void read_from_epos(void)
{
  unsigned char len = 0;
  unsigned char buf[8];
  
  sync_pdo();
  
  if(CAN_MSGAVAIL == CAN.checkReceive()){
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    unsigned int canId = CAN.getCanId();
    decide_CAN_msg(canId, buf);
    CAN.readMsgBuf(&len, buf);
    canId = CAN.getCanId();
    decide_CAN_msg(canId, buf);
  }
}


void decide_CAN_msg(unsigned int id, unsigned char buf[8]){
switch (id) { 
  case 0x182:
        actualvelocity_data = actualvelocity_data | buf[7];
        actualcurrent_data <<= 8;
        actualvelocity_data = actualvelocity_data | buf[6];
        actualvelocity_data <<= 8;
        actualvelocity_data = actualvelocity_data | buf[5];
        actualvelocity_data <<= 8;
        actualvelocity_data = actualvelocity_data | buf[4];
        
        actualposition_data = actualposition_data | buf[3];
        actualposition_data <<= 8;
        actualposition_data = actualposition_data | buf[2];
        actualposition_data <<= 8;
        actualposition_data = actualposition_data | buf[1];
        actualposition_data <<= 8;
        actualposition_data = actualposition_data | buf[0];
        //Serial.println(canId);
        //Serial.println(fromqc_todeg*(float)actualposition_data);
        
        break;

   case 0x282:
        actualcurrent_data = actualcurrent_data | buf[3];
        actualcurrent_data <<= 8;
        actualcurrent_data = actualcurrent_data | buf[2];
        actualcurrent_data <<= 8;
        actualcurrent_data = actualcurrent_data | buf[1];
        actualcurrent_data <<= 8;
        actualcurrent_data = actualcurrent_data | buf[0];
        //Serial.println((float)actualcurrent_data);
        break;
  }
}
