#include <DHT.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

//sett esp
  const char* ssid = "Xrtamppp";// 
  const char* password = "asdfghjkl";
//WiFiClient client;
  char server[] = "172.20.10.7";
//endset esp

#define DHTPIN 2

DHT dht(DHTPIN, DHT11);



int sensor_pin = A0; // Soil Sensor input at Analog PIN A0
int output_value ;
int relayPin = 14;

float sensorSuhu;
float sensorKelembaban; 

//rule
float suhu [5];
float kelembaban [3];

float rule [5][3];
float rule00, rule01, rule02, rule10, rule11, rule12, rule20, rule21, rule22, rule30, rule31, rule32, rule40, rule41, rule42;
float defuz, pwm;
int defuzzy;
float temp;

void setup(){
  Serial.begin(9600);
  lcd.begin();
  dht.begin();
  lcd.backlight();
  
  pinMode(sensorSuhu, INPUT);
  pinMode(sensor_pin, INPUT);
  pinMode(relayPin, OUTPUT);

  digitalWrite(relayPin, HIGH);

//  setting koneksi wifi 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
// Start the server
  Serial.println("Server started");
  Serial.print(WiFi.localIP());
  delay(1000);
  Serial.println("connecting to");
  Serial.println(server);
//  stop koneksi wifi
}

void loop(){  
float  sensorSuhu = dht.readTemperature();
float  sensorKelembaban = output_value;


  
//  fuzifikasi
  FuzzySuhu(sensorSuhu);
  FuzzyKelembaban(sensorKelembaban);

//  defuzifikasi
  Defuzzy();

  lcd.setCursor(0,0); 
  lcd.print("Temp: ");
  lcd.print(sensorSuhu);
  lcd.print((char)223);
  lcd.print("C");
  
  lcd.setCursor(0,1); 
  output_value= analogRead(sensor_pin);
  output_value = map(output_value,1023,0,0,100);
  lcd.print("Mositure: ");
  lcd.print(output_value);
  lcd.print("%");
  
  
  digitalWrite(relayPin, HIGH);
  
  

  Serial.println(sensorSuhu);
  Serial.println(sensorKelembaban);
 
  Serial.println(rule00);
  Serial.println(rule01);
  Serial.println(rule02);

  Serial.println(rule10);
  Serial.println(rule11);
  Serial.println(rule12);

  Serial.println(rule20);
  Serial.println(rule21);
  Serial.println(rule22);

  Serial.println(rule30);
  Serial.println(rule31);
  Serial.println(rule32);

  Serial.println(rule40);
  Serial.println(rule41);
  Serial.println(rule42);
  Serial.println(defuzzy);

  Sending_To_phpmyadmindatabase();

  delay(10000);
//  delay(43200000); //untuk delay 12 jam pagi dan sore
  lcd.noAutoscroll();
  lcd.clear();
}


void FuzzySuhu(float sensorSuhu){
  // untuk suhu dingin
  if (sensorSuhu >= 25)
  { 
    suhu [0] = 0;
  }
  else if (sensorSuhu >= 10 && sensorSuhu <= 25)
  {  
    suhu [0] = (25 - sensorSuhu)/(25 - 10); 
  }
  else
  { 
    suhu [0] = 1;
  }
  
  // untuk suhu agak dingin
  if (sensorSuhu <= 20 || sensorSuhu >= 30)
  { 
    suhu [1] = 0;
  }
  else if (sensorSuhu >= 20 && sensorSuhu <= 25)
  { 
    suhu [1] = (sensorSuhu-20)/(25-20);
  }
  else if (sensorSuhu >= 25 && sensorSuhu <= 30) 
  { 
    suhu [1] = (30-sensorSuhu)/(30-25);
  }

  // suhu normal
  if (sensorSuhu <= 25 || sensorSuhu >= 35)
  { 
    suhu [2] = 0;
  }
  else if (sensorSuhu >= 25 && sensorSuhu <= 30)
  { 
    suhu [2] = (sensorSuhu-25)/(30-25);
  }
  else if (sensorSuhu >= 30 && sensorSuhu <= 35) 
  { 
    suhu [2] = (35-sensorSuhu)/(35-30);
  }

  // suhu hangat
  if (sensorSuhu <= 30 || sensorSuhu >= 40)
  { 
    suhu [3] = 0;
  }
  else if (sensorSuhu >= 30 && sensorSuhu <= 35)
  { 
    suhu [3] = (sensorSuhu-30)/(35-30);
  }
  else if (sensorSuhu >= 35 && sensorSuhu <= 40) 
  { 
    suhu [3] = (40-sensorSuhu)/(40-35);
  }

  // suhu panas  
  if (sensorSuhu <= 35)
  { 
    suhu [4] = 0;
  }
  else if (sensorSuhu >= 35 && sensorSuhu <= 50)
  {  
    suhu [4] = (sensorSuhu-35)/(50 - 35); 
  }
  else
  { 
    suhu [4] = 1;
  }
}


void FuzzyKelembaban(float sensorKelembaban){
  // untuk kering
  if (sensorKelembaban >= 40)
  { 
    kelembaban [0] = 0;
  }
  else if (sensorKelembaban >= 25 && sensorKelembaban <= 40)
  {  
    kelembaban [0] = (40 - sensorKelembaban)/(40 - 25); 
  }
  else
  { 
    kelembaban [0] = 1;
  }

  // kelembaban normal
  if (sensorKelembaban <= 25 || sensorKelembaban >= 75)
  { 
    kelembaban [1] = 0;
  }
  else if (sensorKelembaban >= 25 && sensorKelembaban <= 50)
  { 
    kelembaban [1] = (sensorKelembaban-25)/(50-25);
  }
  else if (sensorKelembaban >= 50 && sensorKelembaban <= 75) 
  { 
    kelembaban [1] = (75-sensorKelembaban)/(75-50);
  }
  
  //  kelembaban basah
  if (sensorKelembaban <= 60)
  { 
    kelembaban [2] = 0;
  }
  else if (sensorKelembaban >= 60 && sensorKelembaban <= 100)
  {  
    kelembaban [2] = (sensorKelembaban-60)/(100 - 60); 
  }
  else
  { 
    kelembaban [2] = 1;
  }
}


void Rule(){
 int i, j;
 for ( i=0; i<=4; i=i+1)
 {
   for ( j=0; j<=2; j=j+1)
   {
     temp = min(suhu[i], kelembaban[j]);
     rule [i][j] = temp;
   } 
 } 
 rule00 = rule[0][0]; // ( dingin, kering = 2 )
 rule01 = rule[0][1]; // ( dingin, normal = 2 )
 rule02 = rule[0][2]; // ( dingin, basah = 1 )

 rule10 = rule[1][0]; // ( agak dingin, kering = 2 )
 rule11 = rule[1][1]; // ( agak dingin, normal = 2 )
 rule12 = rule[1][2]; // ( agak dingin, basah = 1 )

 rule20 = rule[2][0]; // ( normal, kering = 3 )
 rule21 = rule[2][1]; // ( normal, normal = 3 )
 rule22 = rule[2][2]; // ( normal, basah = 2 )

 rule30 = rule[3][0]; // ( hangat, kering = 4 )
 rule31 = rule[3][1]; // ( hangat, normal = 3 )
 rule32 = rule[3][2]; // ( hangat, basah = 2 )

 rule40 = rule[4][0]; // ( panas, kering = 5 )
 rule41 = rule[4][1]; // ( panas, normal = 4 )
 rule42 = rule[4][2]; // ( panas, basah = 3 )
}


void Defuzzy(){
  // metode sugeno (weighted average)
  Rule();
  pwm = (rule00*2) + (rule01*2) + (rule02*1) + 
        (rule10*2) + (rule11*2) + (rule12*1) + 
        (rule20*3) + (rule21*3) + (rule22*2) + 
        (rule30*4) + (rule31*3) + (rule32*2) + 
        (rule40*5) + (rule41*4) + (rule42*3);
 
  defuz = 0;
  int i, j;
  for ( i=0; i<=4; i=i+1)
  {
    for ( j=0; j<=2; j=j+1)
    {
      defuz = defuz + rule [i][j];
    } 
  } 
  defuzzy = pwm / defuz;

  if (defuzzy == 1){
//pompa mati
    digitalWrite(relayPin, HIGH);
    lcd.setCursor(18,0);
    lcd.autoscroll();
    lcd.print("Motor OFF");
    lcd.setCursor(18,1);
    lcd.print("Pump Mati");
    delay(2000);
  }
  else if (defuzzy == 2){
//pompa hidup selama 4 detik
     digitalWrite(relayPin, LOW);
     lcd.setCursor(23,0);
     lcd.autoscroll();
     lcd.print("Motor ON");
     lcd.setCursor(23,1);
     lcd.print("Penyiraman 4s");
     delay(4000);
  }
  else if (defuzzy == 3){
//pompa hidup selama 7 detik
     digitalWrite(relayPin, LOW);
     lcd.setCursor(23,0);
     lcd.autoscroll();
     lcd.print("Motor ON");
     lcd.setCursor(23,1);
     lcd.print("Penyiraman 7s");
     delay(7000);
  }
  else if (defuzzy == 4){
//pompa hidup selama 9 detik
     digitalWrite(relayPin, LOW);
     lcd.setCursor(23,0);
     lcd.autoscroll();
     lcd.print("Motor ON");
     lcd.setCursor(23,1);
     lcd.print("Penyiraman 9s");
     delay(9000);
  }
  else if (defuzzy == 5){
//pompa hidup selama 12 detik
    digitalWrite(relayPin, LOW);
    lcd.setCursor(23,0);
    lcd.autoscroll();
    lcd.print("Motor ON");
    lcd.setCursor(23,1);
    lcd.print("Penyiraman 12s");
    delay(12000);
  }
  lcd.noAutoscroll();
  lcd.clear();
}

void Sending_To_phpmyadmindatabase(){   //CONNECTING WITH MYSQL
   float  sensorSuhu = dht.readTemperature();
   float  sensorKelembaban = output_value;
   WiFiClient client;
   const int port = 80;
   
   if (client.connect(server, port)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("GET /etilang/kirimdata.php?");     //YOUR URL
    client.print("temperature=");
    client.print(sensorSuhu);
    client.print("&kelembaban=");
    client.print(sensorKelembaban);
    client.print("&defuzzy=");
    client.print(defuzzy);
    
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: 172.20.10.7");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}
