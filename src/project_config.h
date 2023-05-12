#ifndef PROJECT_CONF_HEADER
#define PROFECT_CONF_HEADER
  #include <LiquidCrystal_I2C.h>
  #include <Arduino.h>
  #include <WiFi.h>
  #include <WebServer.h>
  #include <EEPROM.h>
  #include <SPI.h>
  #include <Adafruit_SHTC3.h>

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
    #define TEMP_SENS_ADDRESS 0x70
    #define TEMP_SENS_TIMERNUM 0
    #define TEMP_SENS_PREESCALER 80
    #define TEMP_SENS_MICROSECS 10000000
    Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();
    sensors_event_t temperature;
    int tempInteger = 0;
    hw_timer_t *TempSensorTimer = NULL;
    volatile boolean tempHasChanged = false;
    volatile boolean tempHumTimerPaused = false;
    volatile boolean tempHigherThan100 = false;
    volatile boolean tempBelowZero = false;
    volatile boolean changeTemp = false;
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
    sensors_event_t humidity;
    int humidityInteger = 0;
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
    #define SERVO0_CHANNEL 4
    #define SERVO_FREQ 50
    #define SERVO_DUTY_CYCLE 12
    #define SERVO_MIN 100
    #define SERVO_MAX 511
    uint8_t servoPins[] = {SERVO0_PIN};
    uint8_t servoChannels[] = {SERVO0_CHANNEL};
    uint8_t servoAngles[] = {0};
  #endif

  #ifndef DOOR_CONFIG
  #define DOOR_CONFIG
    #define DOOR_FREQ 50
    #define DOOR_DUTY_CYCLE 12
    #define DOOR_MIN 100
    #define DOOR_MAX 511
    #define DOOR_ANGLE_MAX 100
    #define DOOR0_PIN 5
    #define DOOR0_CHANNEL 5
    #define DOOR1_PIN 17
    #define DOOR1_CHANNEL 6
    uint8_t doorPins[] = {DOOR0_PIN, DOOR1_PIN};
    uint8_t doorChannels[] = {DOOR0_CHANNEL, DOOR1_CHANNEL};
    uint8_t doorAngles[] = {0, 0};
    boolean doorStates[] = {false, false};
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
    #define LIGHT_2_PIN 25
    #define LIGHT_3_PIN 26
    uint8_t lightPins[] = {LIGHT_0_PIN, LIGHT_1_PIN, LIGHT_2_PIN, LIGHT_3_PIN};
    boolean lightStates[] = {false, false, false, false};
  #endif
#endif
