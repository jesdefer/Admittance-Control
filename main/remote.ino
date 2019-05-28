void load_menu(void){
  Serial.println("|****************************|");
  Serial.println("|**|Jesus' TFM Configuration|**|");
  Serial.println("|**|        Menu          |**|");
  Serial.println("|****************************|");
  Serial.println("");
  Serial.println("Please, select one of the following options:");
  Serial.println("e) Enable");
  Serial.println("d) Disable");
  Serial.println("h) Homing");
  Serial.println("a) Enter in Admittance Mode");
  Serial.println("l) Calibrate Load Cell");
  Serial.println("i) Calibate IMU");
  Serial.println("x) Set 45º");
  Serial.println("c) Set 90º");
}

void check_keyboard(void){
  char incomingChar_b = 0;
  if(Serial.available() > 0) incomingChar_b = Serial.read();
  if(incomingChar_b == 'h') {
    control_loop = false;
    homing();
  }
  else if(incomingChar_b == 'e') {
    control_loop = false;
    enable_motor();
    Serial.println("Enabled motor!");
  }
  else if(incomingChar_b == 'd') {
    control_loop = false;
    disable_motor();
    Serial.println("Disabled motor!");
  }
  else if(incomingChar_b == 'a') {
    Serial.println("Admittance mode!");
    Serial.println("Set virtual inertia! [Nms^2/rad]");
    while (Serial.available() == 0);
    virtual_inertia = Serial.parseFloat();
    Serial.println("Virtual inertia set to " + String(virtual_inertia) + " [Nms^2/rad]");
    Serial.println(" ");
    Serial.println("Set virtual damping! [Nms/rad]");
    while (Serial.available() == 0);
    virtual_damping = Serial.parseFloat();
    Serial.println("Virtual damping set to " + String(virtual_damping) + " [Nms/rad]");
    Serial.println(" ");
    Serial.println("Set virtual stiffness! [Nm/rad]");
    while (Serial.available() == 0);
    virtual_stiffness = Serial.parseFloat();
    switching_damping = virtual_damping;
    Serial.println("Virtual stiffness set to " + String(virtual_stiffness) + " [Nm/rad]");
    Serial.println(" ");
    Serial.println("Add extra delays? [Y/N]");
    while (Serial.available() == 0);
    if (Serial.read() == 'Y') {
      add_delays= true;
      Serial.println("Selected: YES");
    }
    else {
      add_delays = false;
      Serial.println("Selected: NO");
    }
    Serial.println("Apply Scattering transformation? [Y/N]");
    while (Serial.available() == 0);
    if (Serial.read() == 'Y') {
      scattering_transformation = true;
      Serial.println("Selected: YES");
    }
    else {
      scattering_transformation = false;
      Serial.println("Selected: NO");
    }
    Serial.println("Variable Damping? [Y/N]");
    while (Serial.available() == 0);
    if (Serial.read() == 'Y') {
      variable_damping_mode  = true;
      Serial.println("Selected: YES");
      Serial.println("Set safety damping! [Nms/rad]");
      while (Serial.available() == 0);
      max_damping = Serial.parseFloat();
      Serial.println("Saferty damping set to " + String(max_damping) + " [Nms/rad]");
      Serial.println(" ");
    }
    else {
      variable_damping_mode  = false;
      Serial.println("Selected: NO");
    }
    enter_in_position_mode();
    control_loop = true;
    load_menu();
    t_ini = millis();
  }

  else if(incomingChar_b == 'l') calibration_load_cell();
  else if(incomingChar_b == 'i') calibration_imu();
  else if(incomingChar_b == 'x') {
    control_loop = false;
    enter_in_position_mode();
    set_angle_to_motor(45);
    sync_pdo();
  }
  else if(incomingChar_b == 'c') {
    control_loop = false;
    enter_in_position_mode();
    set_angle_to_motor(90);
    sync_pdo();
  }
}
