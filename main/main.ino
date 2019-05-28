

void setup() {
       Serial.begin(115200);
       spi_config();
       init_IMU();
       set_object_dictionary();
       initADS();
       homing();
       load_menu();         
}

void loop(){
       check_keyboard();
       admittance_control();
}
