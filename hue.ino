/*
   author ryosuke ando
   mail ryo@ando.link
*/
#include <WiFi.h>
#include <HTTPClient.h>
//#include <Stepper.h>

String ID = "HNyxEyPRkCn0dQIDF1eBkBy-UDvs0qtZRww7z3Cv";
const char* ssid     = "pinoseWiFi";
const char* password = "komediadesign";

int host_ip[] = {192, 168, 2, 101};
int id = 5;
int hue = 100;
int bri = 100;
int sat = 100;

void setup()
{
  Serial.begin(115200);
  connect_to_wifi();
  motor_init();
  //  while (true) {
  //    motor_step(-1);
  //  }
  create_task();
}
void loop()
{

  //  //  analogRead
  //  motor_step(1);

  String ret = hue_change_color( id,  hue,  bri,  sat);

  hue += 16 * 256;
  bri += 16;
  sat += 16;
  //Serial.println(ret);
  delay(10);
}

void loop2(void *pvParameters) {
  int subcount = 1000; // サブカウンタ
  while (1) {
    int a = analogRead(34);
    motor_step(-1);
    Serial.println(a);
    delayMicroseconds(uint32_t(a*50));
  }
}


//create_task
int create_task() {
  xTaskCreate(loop2, "loop2", 1024, NULL, 1, NULL);
}



//
#define IN0 16
#define IN1 17
#define IN2 18
#define IN3 19
//_モーター用の関数と変数
int stepsMatrix[8][4] = {
  {LOW, LOW, LOW, HIGH},
  {LOW, LOW, HIGH, HIGH},
  {LOW, LOW, HIGH, LOW},
  {LOW, HIGH, HIGH, LOW},
  {LOW, HIGH, LOW, LOW},
  {HIGH, HIGH, LOW, LOW},
  {HIGH, LOW, LOW, LOW},
  {HIGH, LOW, LOW, HIGH},
};
void motor_init() {
  pinMode(IN0, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
}
void motor_step(int step) {
  static uint64_t counter = 0;
  for (int i = 0; i < abs(step); i++) {
    digitalWrite(IN0, stepsMatrix[counter % 8][0]);
    digitalWrite(IN1, stepsMatrix[counter % 8][1]);
    digitalWrite(IN2, stepsMatrix[counter % 8][2]);
    digitalWrite(IN3, stepsMatrix[counter % 8][3]);
    counter += sign(step);
    Serial.print(stepsMatrix[counter % 8][0]);
    Serial.print('\t');
    Serial.print(stepsMatrix[counter % 8][1]);
    Serial.print('\t');
    Serial.print(stepsMatrix[counter % 8][2]);
    Serial.print('\t');
    Serial.print(stepsMatrix[counter % 8][3]);
    Serial.print('\n');
  }
}
int sign(int A) {
  return (A > 0) - (A < 0);
}
//


String get_hue_status() {
  String host = String(host_ip[0]) + "." + String(host_ip[1]) + "." + String(host_ip[2]) + "." + String(host_ip[3]);
  String url = "http://" + host + "/api/" + ID + "/lights";
  const char* URL = url.c_str();
  HTTPClient http;
  http.begin(URL);
  int httpCode = http.GET();
  //  Serial.printf("Response: %d", httpCode);
  //  Serial.println();
  if (httpCode == HTTP_CODE_OK) {
    String body = http.getString();
    return body;
    //    Serial.print("Response Body: ");
    //    Serial.println(body);
  }
  return "";
}
String hue_change_color(int id, uint16_t hue, uint8_t bri, uint8_t sat) {
  if (bri == 255)
    return "";
  if (sat == 255)
    return "";

  String host = String(host_ip[0]) + "." + String(host_ip[1]) + "." + String(host_ip[2]) + "." + String(host_ip[3]);
  String url = "http://" + host + "/api/" + ID + "/lights/" + id + "/state";
  const char* URL = url.c_str();
  HTTPClient http;
  http.begin(URL);
  String param = "{\"on\":true, \"bri\":" + String(bri) + ", \"hue\":" + String(hue) + ", \"sat\":" + String(sat) + "}";

  //Serial.print(param);
  int httpCode = http.PUT(param);

  if (httpCode != HTTP_CODE_OK)
    return "";

  String body = http.getString();
  return body;
}

void connect_to_wifi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  if (!Serial)
    return;

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

