#define Kg2N  10
#define N2Nm 0.1

volatile byte control_loop = false;
// Sensor calibration
float torque = 0.0;
int n_samples_calib = 500;
float offset_load_cell = 0.0;
//Admittance model
float desired_position = 0.0;
float desired_velocity = 0.0;
float desired_acceleration = 0.0;
float virtual_inertia = 0.3;
float virtual_damping = 0.1;
float virtual_stiffness = 0.1;
//Time measurments
float t_ini = 0.0;
float dt = 0.0;
float current_time = millis();
float past_time = millis();
//Scattering
bool add_delays = false;
bool scattering_transformation = false;
int b_factor = 1;
float ul = 0.0; float vl = 0.0;
float ur = 0.0; float vr = 0.0;
int delay_driver = 5;
int delay_sensor = 5;
//Variable acceleration reference
bool variable_damping_mode = false;
float switching_damping = virtual_damping;
float exchanged_energy_k = 0.0;
float exchanged_energy_k_1 = 0.0;
float delta_exchanged_energy_k = 0.0;
float angular_position_k = 0.0;
float angular_position_k_1 = 0.0;
float torque_k_1 = 0.0;
float virtual_storage_function = 0.0;
float max_damping = 20;

void admittance_control(void){
  current_time = millis();
  dt = (current_time - past_time)*0.001;
  past_time = current_time;
  if (control_loop) desired_admittance_model();
}

void desired_admittance_model(void){
  float time_sim = (millis()-t_ini)*0.001;

  torque = -(read_load_cell() - offset_load_cell)*0.000044421*Kg2N*N2Nm;
  
  if (scattering_transformation){
    vl = vr;
    desired_velocity = b_factor*torque - sqrt(2*b_factor)*vl;
    ul = sqrt(2*b_factor)*torque - vl;}
    
  if (add_delays) delay(delay_sensor);
  
  if(scattering_transformation){
    ur = ul;
    vr = ur - (sqrt(2*b_factor)/b_factor) * desired_velocity;
    torque = (1/b_factor)*(sqrt(2*b_factor)*ur - desired_velocity);}

  desired_acceleration = (1/virtual_inertia)*(torque - switching_damping *desired_velocity - virtual_stiffness*desired_position);
  desired_velocity = desired_velocity + desired_acceleration*dt;
  desired_position = desired_position + desired_velocity*dt;
  
  if (add_delays) delay(delay_driver);
  
  if (desired_position<0){
    desired_position = 0;
    desired_velocity = 0;
  }
  if (desired_position>100*PI/180){
    desired_position = 100*PI/180;
    desired_velocity = 0;
  }
  
  set_angle_to_motor(desired_position*180/PI);
  read_from_epos();

  angular_position_k = fromqc_todeg*(float)actualposition_data*(PI/180);
  delta_exchanged_energy_k = torque_k_1*(angular_position_k - angular_position_k_1);
  exchanged_energy_k = exchanged_energy_k_1 + delta_exchanged_energy_k;
  exchanged_energy_k_1 = exchanged_energy_k;
  angular_position_k_1 = angular_position_k;
  torque_k_1 = torque;
  virtual_storage_function = 0.5*virtual_inertia*desired_velocity*desired_velocity + 0.5*virtual_stiffness*desired_position*desired_position;
  if(variable_damping_mode){
    if (exchanged_energy_k < virtual_storage_function) switching_damping  = max_damping;
    else switching_damping  = virtual_damping;
  }
 }


void calibration_load_cell(void){
  offset_load_cell = 0.0;
  for (int i = 1;i<n_samples_calib;i++){
   offset_load_cell = read_load_cell() + offset_load_cell;
  }
  offset_load_cell = offset_load_cell/n_samples_calib;
  Serial.println("Load Cell Calibrated!");
}

