#include "project_config.h"
#include "webpage/controller_webpage.h"

// ----------- INTERRUPTS/EVENTS ------------
void IRAM_ATTR onTempHumiditySensorsTimer();
void onPageRequest();
void onSendXML();
void onLightStateChange();
void onLouverStateChange();
void onDoorStateChange();

// --------------- INIT FUNCS ---------------
void createLCDChars()
{
  lcd.createChar(TERMOMETER_CHAR_INDEX, termometer_char);
  lcd.createChar(DEGREE_CHAR_INDEX, degree_char);
  lcd.createChar(GOTA_CHAR_INDEX, gota_char);
  lcd.createChar(PERCENT_CHAR_INDEX, percent_char);
}

void initI2CLCD()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  createLCDChars();
}

void initShowTempAndHumidity()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(TERMOMETER_CHAR_INDEX);
  lcd.setCursor(4, 0);
  lcd.write(DEGREE_CHAR_INDEX);
  lcd.setCursor(5, 0);
  lcd.print("C");
  lcd.setCursor(2, 0);
  lcd.print(hallRead());
  lcd.setCursor(0, 1);
  lcd.write(GOTA_CHAR_INDEX);
  lcd.setCursor(2, 1);
  lcd.printf("%d%", random(101));
  lcd.setCursor(5, 1);
  lcd.write(PERCENT_CHAR_INDEX);
  lcd.setCursor(11, 1);
  lcd.setCursor(12, 1);
}

void initTempAndHumiditySensor()
{
  shtc3.begin();
  TempSensorTimer = timerBegin(TEMP_SENS_TIMERNUM, TEMP_SENS_PREESCALER, true);
  timerAttachInterrupt(TempSensorTimer, &onTempHumiditySensorsTimer, true);
  timerAlarmWrite(TempSensorTimer, TEMP_SENS_MICROSECS, true);
  timerAlarmEnable(TempSensorTimer);
}

void initServo()
{
  int size = sizeof(servoPins) / sizeof(uint8_t);
  for (int i = 0; i < size; i++)
  {
    ledcSetup(servoChannels[i], SERVO_FREQ, SERVO_DUTY_CYCLE);
    ledcAttachPin(servoPins[i], servoChannels[i]);
  }
}

void initDoorServo()
{
  int size = sizeof(doorPins) / sizeof(uint8_t);
  for (int i = 0; i < size; i++)
  {
    ledcSetup(doorChannels[i], DOOR_FREQ, DOOR_DUTY_CYCLE);
    ledcAttachPin(doorPins[i], doorChannels[i]);
  }
}

void initWiFiAP()
{
  WiFi.softAP(WIFI_SSID_AP, WIFI_PASS_AP);
  WiFi.softAPConfig(ipPagina, gateway, subred);
  thisIP = WiFi.softAPIP();
}

void initServer()
{
  server.on("/", onPageRequest);
  server.on("/xml", onSendXML);
  server.on("/updateLightState", onLightStateChange);
  server.on("/updateLouverState", onLouverStateChange);
  server.on("/updateDoorState", onDoorStateChange);
  server.begin();
}

void initLights()
{
  int size = sizeof(lightPins) / sizeof(uint8_t);
  for (int i = 0; i < size; i++)
  {
    pinMode(lightPins[i], OUTPUT);
  }
}

void setup()
{
  initLights();
  initI2CLCD();
  initTempAndHumiditySensor();
  initServo();
  initDoorServo();
  initWiFiAP();
  initServer();
  initShowTempAndHumidity();
}

// ------------- NORMAL FUNCTIONS ---------------
void updateTemperature()
{
  if (changeTemp)
  {
    shtc3.getEvent(&humidity, &temperature);
    int newTemp = (int) temperature.temperature;
    if (newTemp != tempInteger)
    {
      tempHasChanged = true;
      tempInteger = newTemp;
    }
    if (tempInteger < 0)
    {
      tempBelowZero = true;
    }
    if (tempInteger >= 100)
    {
      tempHigherThan100 = true;
    }
    tempHasChanged = true;
    changeTemp = false;
  }
}

inline void updateTemperatureLCD()
{
  if (tempHasChanged)
  {
    if (tempHigherThan100 || tempBelowZero)
    {
      lcd.setCursor(1, 0);
      tempHigherThan100 = false;
      tempBelowZero = false;
    }
    else
    {
      lcd.setCursor(1, 0);
      lcd.print(" ");
      lcd.setCursor(2, 0);
    }
    lcd.print(tempInteger);
    tempHasChanged = false;
  }
}

void updateHumidity()
{
  if (changeHum)
  {
    shtc3.getEvent(&humidity, &temperature);
    int newHum = (int) humidity.relative_humidity;
    if (newHum != humidityInteger)
    {
      humidityInteger = newHum;
      if (humidityInteger == 100)
      {
        humidityEqualsHundred = true;
      }
      humidityHasChanged = true;
      changeHum = false;
    }
  }
}

inline void updateHumidityLCD() {
  if (humidityHasChanged)
  {
    if (humidityEqualsHundred)
    {
      lcd.setCursor(4, 1);
      lcd.print(" ");
    }
    lcd.setCursor(2, 1);
    lcd.printf("%d%", humidityInteger);
    humidityHasChanged = false;
  }
}

inline void changeLightState(int lightId, boolean lightState)
{
  lightStates[lightId] = lightState;
  digitalWrite(lightPins[lightId], lightStates[lightId]);
}

inline void changeLouverAngle(int louverId, int angle)
{
  while (angle != servoAngles[louverId])
  {
    if (servoAngles[louverId] <= angle)
    {
      servoAngles[louverId]++;
    }
    else if (servoAngles[louverId] >= angle)
    {
      servoAngles[louverId]--;
    }
    int value = map(servoAngles[louverId], 0, 180, SERVO_MIN, SERVO_MAX);
    ledcWrite(servoChannels[louverId], value);
  }
}

inline void changeDoorState(int doorId, boolean open)
{
  if (open)
  {
    while (doorAngles[doorId] < DOOR_ANGLE_MAX)
    {
      doorAngles[doorId]++;
      int value = map(doorAngles[doorId], 0, 180, DOOR_MIN, DOOR_MAX);
      ledcWrite(doorChannels[doorId], value);
    }
  }
  else
  {
    while (doorAngles[doorId] > 0)
    {
      doorAngles[doorId]--;
      int value = map(doorAngles[doorId], 0, 180, DOOR_MIN, DOOR_MAX);
      ledcWrite(doorChannels[doorId], value);
    }
  }
  doorStates[doorId] = open;
}

void loop()
{
  updateTemperature();
  updateHumidity();
  updateTemperatureLCD();
  updateHumidityLCD();
  server.handleClient();
}

// ----------- INTERRUPT/EVENTS IMPLEMENTATIONS ------------
void IRAM_ATTR onTempHumiditySensorsTimer()
{
  if (!changeTemp)
  {
    changeTemp = true;
  }
  if (!changeHum)
  {
    changeHum = true;
  }
}

void onPageRequest()
{
  server.send(200, "text/html", MAIN_PAGE);
}

// si, estoy usando xml y no json, algun problema? >:(((((
void onSendXML()
{
  strcpy(xmlBuffer, "<?xml version = '1.0'?>\n<Data>\n");
  sprintf(buff, "<temp0>%d</temp0>\n", tempInteger);
  strcat(xmlBuffer, buff);
  sprintf(buff, "<hum0>%d</hum0>\n", humidityInteger);
  strcat(xmlBuffer, buff);

  int lightsQuant = sizeof(lightPins) / sizeof(uint8_t);
  for (int i = 0; i < lightsQuant; i++)
  {
    sprintf(buff, "<light%d>%s</light%d>\n", i, lightStates[i]? "ON": "OFF", i);
    strcat(xmlBuffer, buff);
  }

  int servosQuant = sizeof(servoPins) / sizeof(uint8_t);
  for (int i = 0; i < servosQuant; i++)
  {
    sprintf(buff, "<louver%d>%d</louver%d>\n", i, servoAngles[i], i);
    strcat(xmlBuffer, buff);
  }

  int doorsQuant = sizeof(doorPins) / sizeof(uint8_t);
  for (int i = 0; i < doorsQuant; i++)
  {
    sprintf(buff, "<door%d>%s</door%d>\n", i, doorStates[i]? "OPENED" : "CLOSED", i);
    strcat(xmlBuffer, buff);
  }
  strcat(xmlBuffer, "</Data>\n");
  server.send(200, "text/xml", xmlBuffer);
}

void onLightStateChange()
{
  String lightId = server.arg("lightId");
  String lightState = server.arg("lightState");
  int lightIdNum = lightId[lightId.length() - 1] - '0';
  boolean lightStateBool = lightState.equals("ON")? true : false;
  changeLightState(lightIdNum, lightStateBool);
  server.send(200, "text/plain", "Done");
}

void onLouverStateChange()
{
  String louverId = server.arg("louverId");
  String louverAngle = server.arg("louverAngle");
  int louverIdNum = louverId[louverId.length() - 1] - '0';
  int louverAngleNum = atoi(louverAngle.c_str());
  changeLouverAngle(louverIdNum, louverAngleNum);
  server.send(200, "text/plain", "Done");
}

void onDoorStateChange()
{
  String doorId = server.arg("doorId");
  String doorState = server.arg("doorState");
  int doorIdNum = doorId[doorId.length() - 1] - '0';
  boolean doorStateBool = doorState.equals("OPENED")? true : false;
  changeDoorState(doorIdNum, doorStateBool);
  server.send(200, "text/plain", "Done");
}
