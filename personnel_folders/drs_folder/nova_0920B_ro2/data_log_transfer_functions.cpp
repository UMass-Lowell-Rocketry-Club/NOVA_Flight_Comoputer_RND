#include "macros.hh"
#include "Vector3D.hh"

void write_data_to_log (ICM_20948 *sensor, Vector3D vector){
    //vector is the velocity vector
    card = SD.open(data_log_file_name, FILE_WRITE);
    myICM.getAGMT();
    float temp(NAN), hum(NAN), pres(NAN);
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);
    bme.read(pres, temp, hum, tempUnit, presUnit);   
    card.print(MET); card.print(", "); 
    card.print(sensor->accX()); card.print(", ");card.print(sensor->accY()); card.print(", ");card.print(sensor->accZ()); card.print(", ");
    card.print(sensor->gyrX()); card.print(", ");card.print(sensor->gyrY()); card.print(", ");card.print(sensor->gyrZ()); card.print(", ");
    //Added code for printing velocity vector, I am assuming this is correct.
    card.print( vector.get_x_cord() ); card.print(", ");card.print(vector.get_x_cord(); card.print(", ");card.print(vector.get_x_cord(); card.print(", ");
    //End code for printing velocity vector. 
    card.print(pres); card.print(", "); card.print(temp); card.print(", "); card.println(hum);
    card.close();
}

void update_met(){
    MET = millis()-launch_time_in_milliseconds;
}

void transmit_data_to_ground_full_packet(){

}

void transmit_data_to_ground_partial_packet(){
    
}

double get_adjusted_acceleration(double acceleration_value){
    return acceleration_value;
}