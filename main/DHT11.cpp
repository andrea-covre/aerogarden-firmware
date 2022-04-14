#ifndef DHT11_cpp
#define DHT11_cpp

#include <dht.h>

class Dht11 {
  private:
    dht DHT;

    // PIN
    const int signal_pin = 31;

    // Data
    int refresh_delay = 1000;   // ms
    float temperature;
    float humidity;
    uint32_t last_update_ts = millis();

  private:
    int read_data() {
      if (millis() - last_update_ts > refresh_delay) {
        int chk = DHT.read11(signal_pin);
        String base_string = "DHT11: ";
        switch (chk) {
          case DHTLIB_OK:  
            temperature = DHT.temperature;
            humidity = DHT.humidity;
          break;
          
          case DHTLIB_ERROR_CHECKSUM: 
            Serial.println(base_string + "Checksum error"); 
            break;
            
          case DHTLIB_ERROR_TIMEOUT: 
            Serial.println(base_string + "Time out error"); 
            break;
            
          case DHTLIB_ERROR_CONNECT:
                Serial.println(base_string + "Connect error");
                break;
                
          case DHTLIB_ERROR_ACK_L:
                Serial.println(base_string + "Ack Low error");
                break;
                
          case DHTLIB_ERROR_ACK_H:
              Serial.println(base_string + "Ack High error");
              break;
              
          default: 
            Serial.println(base_string + "Unknown error"); 
            break;
          }  
       last_update_ts = millis();
      }
    }

   public:
     float get_temperature() {
      read_data();
      return temperature;
     }

     float get_humidity() {
      read_data();
      return humidity;
     }


};

#endif
