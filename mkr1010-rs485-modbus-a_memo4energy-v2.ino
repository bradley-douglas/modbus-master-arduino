/*
  Modbus energy with Memo4 energy meter on Modbus half duplex
  author : Bradley Douglas, south africa
  Date 2019/07/01
  


*/

#include <ArduinoModbus.h>


//declare readings 
float voltage;
float current;
float frequency;
float total_active_power;
float total_active_energy;
float power_factor;
float  fwd_active_energy;
float  rev_active_energy;

//declare modbus addresses for registers
int modbus_address_voltage = 8192;
int modbus_address_current = 8288;
int modbus_address_frequency = 8224;
int modbus_address_active_power = 8320;
int modbus_address_active_energy = 12288;
int modbus_address_PF = 8416;
int modbus_address_fwd_energy = 12320;
int modbus_address_rev_energy = 12352;

//define slave modbus address 
int slave_address = 36;
int modbus_request_delay = 1000;


void setup() {
  Serial.begin(115200);

  Serial.println("Modbus energy meter over Modbus 485");
  // start the Modbus RTU client
  if (!ModbusRTUClient.begin(9600)) {
    Serial.println("Failed to start Modbus RTU Client!");
    while (1);
  }
}
//*************************************************************************************************
void loop()
{

  // request registers modbus ( slave address , register address , datablock amounts)
  voltage = modbus_request(slave_address, modbus_address_voltage, 2);
  current = modbus_request(slave_address, modbus_address_current, 2);
  frequency = modbus_request(slave_address, modbus_address_frequency, 2);
  total_active_power = modbus_request(slave_address, modbus_address_active_power, 2);
  total_active_energy = modbus_request(slave_address, modbus_address_active_energy, 2);
  fwd_active_energy = modbus_request(slave_address, modbus_address_fwd_energy, 2);
  rev_active_energy = modbus_request(slave_address, modbus_address_rev_energy, 2);
  power_factor = modbus_request(slave_address, modbus_address_PF, 2);

  printloop();


}


//************************************************************************************************
float modbus_request( int slave_address, int modbus_add , int number_data_block )
{
  float read_value;
  if (!ModbusRTUClient.requestFrom(slave_address, HOLDING_REGISTERS, modbus_add, number_data_block))
  {
    Serial.println("");
    Serial.print("failed to read registers! ");
    Serial.print(ModbusRTUClient.lastError());
    Serial.print(" --register address :  ");
    Serial.print(modbus_add);
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to

    short block1 = ModbusRTUClient.read();
    delay(50); // to allow time for doing stuff
    short block2 = ModbusRTUClient.read();

    //union to get 4 bytes into float value(shit did i battle to get this working :)
    union u_tag
    {
      byte var_a[4];
      float fval;
    } u;

    u.var_a[0] = lowByte  (block2);
    u.var_a[1] = highByte (block2);
    u.var_a[2] = lowByte  (block1);
    u.var_a[3] = highByte (block1);

    read_value  = u.fval;
    return read_value ;
  }
  delay (modbus_request_delay);
}

//****************************************************************************************
void printloop() {
  Serial.println("");
  Serial.print("Voltage value :  ");
  Serial.print (voltage, 2);
  Serial.println(" Volts");


  Serial.print("Frequency value :  ");
  Serial.print (frequency, 2);
  Serial.println(" HZ");


  Serial.print("Current :  ");
  Serial.print (current, 3);
   Serial.println(" Amps");

  Serial.print("Active Power :  ");
  Serial.print (total_active_power, 3);
  Serial.println(" kw");

  Serial.print("Active Energy :  ");
  Serial.print (total_active_energy, 3);
   Serial.println(" kwh");

  Serial.print("FWD Active Energy :  ");
  Serial.print (fwd_active_energy, 3);
  Serial.println(" kwh");


  Serial.print("REV Active Energy :  ");
  Serial.print (rev_active_energy, 3);
  Serial.println(" kwh");


  Serial.print("Power Factor :  ");
  Serial.println (power_factor , 3);

  Serial.println("");


}
