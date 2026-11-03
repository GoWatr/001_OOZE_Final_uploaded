#include "General.h"

#define USE_WIFI false
#define TEST_FM true



#define MODEM_RST 5
#define MODEM_PWKEY 4
#define MODEM_POWER_ON 23
#define MODEM_TX 27
#define MODEM_RX 26
#define RESET_PIN 5

const int DEREPin = -1;

auto& usbSerial = Serial;
//SoftwareSerial modbusSerial;

modbusMaster modbus;

const char* ssid = "GOWATRPRIVATELIMITED";
const char* password = "9150013963";
const char* apiKeyIn = "...................";
const unsigned int writeInterval = 30000;
static volatile int num = 0;

WiFiMulti WiFiMulti;
HTTPClient gowatrnew;

#define SerialAT Serial1

GSMSimHTTP http(SerialAT, RESET_PIN);
GSMSimTime dateTime(SerialAT, RESET_PIN);

int WiFi_Detect = 0;
//String url = "http://www.gowatr.com/wms/pushdata/xanadu.php?FlowmeterId=46138666&Totalizer="
//String url = "https://www.gowatr.com/wms/pushdata/ooze.php?project_id=ooze&data=device_id:GWFMCT02,flowrate:";
//String ID1 = ",totalizer:";
//int counter = 0;

String datatosend = "";

uint32_t FlowmeterId;
bool FM_Error = 0, communication_channel = 0;

//const unsigned int writeInterval = 45000;



void ModBus_Init(void) {
  //if (DEREPin > 0) pinMode(DEREPin, OUTPUT);
  usbSerial.begin(9600, SERIAL_8N1, RX, TX);
  usbSerial.println(PSTR("\n\rInitizing the ModBus... "));
  //modbusSerial.begin(9600, SWSERIAL_8N1, MODBUS_RX, MODBUS_TX, false, 95, 11);
  //modbus.begin(modbusAddress, modbusSerial, DEREPin);
  SerialAT.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(1000);
}

void GSM_Init(void) {
  usbSerial.println(PSTR("Initializing GSM... "));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing GSM");
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);
  delay(2000);
  http.init();
  dateTime.init();
  delay(1000);
  usbSerial.print(PSTR("Set Phone Function: "));
  usbSerial.println(http.setPhoneFunc(1));
  delay(1000);
  usbSerial.print(PSTR("GSM_Module Registered to Network: "));
  usbSerial.println(http.isRegistered());
  delay(1000);
  usbSerial.print(PSTR("Signal Quality:  "));
  usbSerial.println(http.signalQuality());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Signal Quality:");
  lcd.setCursor(0, 1);
  lcd.print(http.signalQuality());
  delay(1000);
  usbSerial.print(PSTR("Operator Name: "));
  usbSerial.println(http.operatorNameFromSim());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Operator Name:");
  lcd.setCursor(0, 1);
  lcd.print(http.operatorNameFromSim());
  delay(1000);
  usbSerial.println(PSTR("GPRS Init... "));
  http.gprsInit("airtelgprs.com", "", "");  // Its optional. You can set apn, user and password with this method. Default APN: "internet" Default USER: "" Default PWD: ""
  delay(500);
  usbSerial.print(PSTR("Connecting to GPRS : "));
  usbSerial.println(http.connect());
  delay(500);
  usbSerial.print(PSTR("Get IP Address: "));
  usbSerial.println(http.getIP());
  delay(500);

  Serial.print("Set timezone and time server... ");
  Serial.println(dateTime.setServer(5, "0.in.pool.ntp.org"));  // timezone +3//time1.google.com , //in.pool.ntp.org
  delay(500);

  usbSerial.print("Sync date time from server... ");
  usbSerial.println(dateTime.syncFromServer());
  delay(500);
}

void WiFi_Init(void) {
  usbSerial.println(PSTR("Checking the WiFi... "));
  WiFiMulti.addAP(ssid, password);
  usbSerial.println(PSTR("WiFi Done... "));

  int count = 10, Yes = 0, j = 60;
  while ((Yes == 0) & (WiFiMulti.run() != WL_CONNECTED)) {
    usbSerial.print(PSTR((".")));
    ++count;
    count + j;
    if (count == 20) Yes = 1;
    delay(500);
  }
  if (Yes == 0) WiFi_Detect = 1;
  else WiFi_Detect = 0;

  if (WiFi_Detect) {
    usbSerial.println(PSTR("WiFi connected"));
    usbSerial.println(PSTR("IP address: "));
    usbSerial.println(WiFi.localIP());
    delay(1000);
  } else {
    usbSerial.println(PSTR("WiFi not detected"));
    delay(1000);
  }
}

void Send_Data_WIFI(void) {
  delay(writeInterval);
  communication_channel = 0;
  usbSerial.println("Sending data to server by WIFI");
  if (WiFiMulti.run() == WL_CONNECTED) {
    HTTPClient http;
    usbSerial.println(PSTR("wifi connected"));
    //String temp_url = Format_Data();
    //http.begin(temp_url);
    //int httpResponseCode = http.GET();
    //if(httpResponseCode>0){
    //String response = http.getString();
    //if(httpResponseCode == 200) usbSerial.println(PSTR("Data send successfully\n\r"));
    // else usbSerial.println("Problem in sending Data, Error Code: " + String(httpResponseCode));

    //else
    //{
    //usbSerial.println(PSTR("Error occurred while sending HTTP POST"));
    //delay(1000);

    http.end();
  }
}

void Send_Data_GSM(void) {
  String url = "http://www.gowatr.com/wms/pushdata/ooze.php?project_id=ooze&data=device_id:GWFMCT01,flowrate:";
  String ID1 = ",totalizer:";
  String ID2 = ",hsid:GWFMHS01,hsvalue:";
  delay(writeInterval);
  communication_channel = 1;
  bool GPRSconnection = http.isConnected();
  if (GPRSconnection == true) {
    usbSerial.println("gprs connected");
    MODBUS();
    MODBUS2();
    
    count++;
    if (count >= 8) {
      Serial.print("The SOLVED RESULT in Dflux is ");
      Serial.println(hydro_result);
      datatosend = (url + String(Flow1) + ID1 + String(Tot1) + ID2 + String(hydro_result));
      //url  += String(Flow);
      //url  += ",totalizer:";
      //url  += String(Tot);
      //url += "&current_time=23/03/10,14:28:45+00&forward_total_flow=&flow_percentage=&flow_meter_not_connected=1&flow_meter_data_not_sync=1&gsm_signal_strength=17&wifi_signal_strength=40&communication_channel=0";
      Serial.println(datatosend);
      //String temp_url = Format_Data();
      usbSerial.println(http.get(datatosend, true));
      lcd_data_sent();
      count = 0;
    }
  } else {
    lcd_data_fail();
    bool error;
    usbSerial.print(PSTR("Close GPRS: "));
    usbSerial.println(http.closeConn());
    delay(1000);
    usbSerial.print(PSTR("Connecting to GPRS : "));
    usbSerial.println(http.connect());
    delay(1000);
    usbSerial.print(PSTR("Get IP Address: "));
    usbSerial.println(http.getIP());
    delay(1000);
    MODBUS();
    MODBUS2();
    //String temp_url = Format_Data();
    usbSerial.println(http.get(datatosend, true));
    //String temp_url = Format_Data();
    //Serial.println(http.get(temp_url, true));
    if (error) {
      Serial.println("ERROR data send");
    }
  }
}

String Get_GSMTime() {
  return (dateTime.getRaw());
}

int Get_GSM_SS(void) {
  return (http.signalQuality());
}

int Get_WiFi_SS(void) {
  if (WiFi_Detect) return 40;
  else return 0;
}
