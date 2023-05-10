#ifndef PROJECT_CONF_HEADER
#define PROFECT_CONF_HEADER
  #include <LiquidCrystal_I2C.h>
  #include <Arduino.h>
  #include <WiFi.h>
  #include <WebServer.h>
  #include <EEPROM.h>
  #include <DHT.h>

  #ifndef WIFI_CONFIG
  #define WIFI_CONFIG
    #define WIFI_SSID_AP "NIOT_Domotica"
    #define WIFI_PASS_AP "abc12345"
  #endif

  #ifndef WEB_CONFIG
  #define WEB_CONFIG
    #define USE_INTRANET
    char buff[32];
    char xmlBuffer[2048];
    IPAddress thisIP;
    IPAddress ipPagina(192, 168, 100, 1);
    IPAddress gateway(192, 168, 100, 1);
    IPAddress subred(255, 255, 255, 0);
    WebServer server(80);
  #endif

  #ifndef I2C_LCD_CONFIG
  #define I2C_LCD_CONFIG
    #define I2C_LCD_ADDRESS 0x27
    #define I2C_LCD_COLS 16
    #define I2C_LCD_ROWS 2
    LiquidCrystal_I2C lcd(
      I2C_LCD_ADDRESS,
      I2C_LCD_COLS,
      I2C_LCD_ROWS);
  #endif

  #ifndef TEMP_SENS_CONFIG
  #define TEMP_SENS_CONFIG
    #define TEMP_SENS_PIN 4
    #define TEMP_SENS_TYPE DHT11
    #define TEMP_SENS_TIMERNUM 0
    #define TEMP_SENS_PREESCALER 80
    #define TEMP_SENS_MICROSECS 10000000
    DHT dht(TEMP_SENS_PIN, TEMP_SENS_TYPE);
    hw_timer_t *TempSensorTimer = NULL;
    volatile boolean tempHasChanged = false;
    volatile boolean tempHumTimerPaused = false;
    volatile boolean tempHigherThan100 = false;
    volatile boolean tempBelowZero = false;
    volatile boolean changeTemp = false;
    int temperature = 0;
    byte termometer_char[] = {
      0b00100,
      0b01010,
      0b01010,
      0b01010,
      0b01110,
      0b11111,
      0b11111,
      0b01110
    };
    byte degree_char[] = {
      0b01100,
      0b10010,
      0b01100,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
    };
    #define TERMOMETER_CHAR_INDEX 0
    #define DEGREE_CHAR_INDEX 1
  #endif

  #ifndef HUM_SENS_CONFIG
  #define HUM_SENS_CONFIG
    volatile boolean humidityHasChanged = false;
    volatile boolean humidityEqualsHundred = false;
    volatile boolean changeHum = false;
    int humidity = 0;
    byte gota_char[] = {
      0b00100,
      0b00100,
      0b01010,
      0b01010,
      0b10001,
      0b10001,
      0b10001,
      0b01110,
    };
    byte percent_char[] = {
      0b11001,
      0b11010,
      0b00010,
      0b00100,
      0b01000,
      0b01011,
      0b10011,
      0b00000,
    };
    #define GOTA_CHAR_INDEX 2
    #define PERCENT_CHAR_INDEX 3
  #endif

  #ifndef SERVO_CONFIG
  #define SERVO_CONFIG
    #define SERVO0_PIN 18
    #define SERVO0_CHANNEL 2
    #define SERVO1_PIN 5
    #define SERVO1_CHANNEL 3
    #define SERVO_FREQ 50
    #define SERVO_DUTY_CYCLE 12
    #define SERVO_MIN 100
    #define SERVO_MAX 511
    uint8_t servoPins[] = {SERVO0_PIN, SERVO1_PIN};
    uint8_t servoChannels[] = {SERVO0_CHANNEL, SERVO1_CHANNEL};
    uint8_t servoAngles[] = {0, 0};
  #endif

  #ifndef DOOR_CONFIG
  #define DOOR_CONFIG
    #define DOOR_FREQ 50
    #define DOOR_DUTY_CYCLE 12
    #define DOOR_MIN 100
    #define DOOR_MAX 511
    #define DOOR0_PIN 0
    #define DOOR0_CHANNEL 4
    uint8_t doorPins[] = {DOOR0_PIN};
    uint8_t doorChannels[] = {DOOR0_CHANNEL};
    uint8_t doorAngles[] = {0};
    boolean doorStates[] = {false};
  #endif

  #ifndef EEPROM_CONFIG
  #define EEPROM_CONFIG
    #define EEPROM_SIZE 512
    #define EEPROM_LIGHT_STATE_SIZE 1
    #define EEPROM_SSID_AP_SIZE 32
    #define EEPROM_PASS_AP_SIZE 63
  #endif

  #ifndef LIGHT_CONFIG
  #define LIGHT_CONFIG
    #define LIGHT_0_PIN 32
    #define LIGHT_1_PIN 33
    uint8_t lightPins[] = {LIGHT_0_PIN, LIGHT_1_PIN};
    boolean lightStates[] = {false, false};
  #endif
#endif
