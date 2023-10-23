/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#include <WiFi.h>
#include <aREST.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const int trigPin = 26;
const int echoPin = 25;
long duration;
float distanceCm;

// Create aREST instance
aREST rest = aREST();

// WiFi parameters
const char* ssid = "Network Name";
const char* password = "password";

//define sound speed in cm/uS
#define SOUND_SPEED 0.034

#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// P connections 

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Wire.beginOnPins(32, SDApin)

void setup() {
  Wire.begin(32, 33);
  Serial.begin(115200);

  // Init variables and expose them to REST API
  rest.variable("distance",&distanceCm);

    // Give name and ID to device
  rest.set_id("121");
  rest.set_name("HC-SR04");
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  IPAddress ip(192, 168, 253, 121); //set static ip
  IPAddress gateway(192, 168, 253, 1); //set getteway
  Serial.print(F("Setting static ip to : "));
  Serial.println(ip);
  IPAddress subnet(255, 255, 255, 0);//set subnet
  WiFi.config(ip, gateway, subnet);


  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

    // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
// Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  display.clearDisplay();

  Serial.print("distance: ");
  Serial.println(distanceCm);

  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println(distanceCm);
  display.display(); 
  delay(2000);
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
}
