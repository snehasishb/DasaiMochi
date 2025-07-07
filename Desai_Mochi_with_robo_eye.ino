#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FluxGarage_RoboEyes.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // No reset pin with I2C

// Initialize SSD1306 with I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Declare RoboEyes after display
#include <FluxGarage_RoboEyes.h>
roboEyes roboEyes;

// Event timers
unsigned long eventTimer;
bool event1wasPlayed = false;
bool event2wasPlayed = false;
bool event3wasPlayed = false;
bool event4wasPlayed = false;
bool event5wasPlayed = false;

void setup() {
  Wire.begin(22, 23);  // ESP32-C6: SDA = GPIO22, SCL = GPIO23
  Serial.begin(9600);
  delay(100);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 initialization failed"));
    while (true);
  }

  display.clearDisplay();
  display.display();

  // Initialize eyes
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 60);  // 60 FPS
  roboEyes.setPosition(DEFAULT);
  roboEyes.close();  // Start closed

  eventTimer = millis();  // Start timing
}

void loop() {
  roboEyes.update();  // continuously draw eyes
  unsigned long currentTime = millis();

  // After 10s: Open eyes
  if (currentTime - eventTimer >= 10000 && !event1wasPlayed) {
    roboEyes.open();
    event1wasPlayed = true;
  }

  // After 20s: Happy + Laugh
  if (currentTime - eventTimer >= 20000 && !event2wasPlayed) {
    roboEyes.setMood(HAPPY);
    roboEyes.anim_laugh();
    event2wasPlayed = true;
  }

  // After 30s: Angry mode
  if (currentTime - eventTimer >= 30000 && !event3wasPlayed) {
    roboEyes.setMood(ANGRY);
    event3wasPlayed = true;
  }

  // After 40s: Tired mode
  if (currentTime - eventTimer >= 40000 && !event4wasPlayed) {
    roboEyes.setMood(TIRED);
    event4wasPlayed = true;
  }

  // After 50s: Reset to DEFAULT and keep open
  if (currentTime - eventTimer >= 50000 && !event5wasPlayed) {
    roboEyes.close();
    delay(500);  // tiny pause
    roboEyes.setMood(DEFAULT);
    roboEyes.open();
    event5wasPlayed = true;
  }

  // After 60s: Restart the full sequence
  if (currentTime - eventTimer >= 60000) {
    event1wasPlayed = false;
    event2wasPlayed = false;
    event3wasPlayed = false;
    event4wasPlayed = false;
    event5wasPlayed = false;
    eventTimer = currentTime;
  }
}
