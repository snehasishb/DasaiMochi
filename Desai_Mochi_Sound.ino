#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <FluxGarage_RoboEyes.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define BUZZER_PIN    21  // Passive buzzer connected to GPIO 21

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
roboEyes roboEyes;

String fullText = "HELLO SNEHASISH";
unsigned long eventTimer = 0;
int currentStep = 0;

// --- Play a short tone using tone() ---
void playTone(uint8_t pin, unsigned int freq, unsigned long duration) {
  tone(pin, freq);
  delay(duration);
  noTone(pin);
}

// --- Display HELLO SNEHASISH text animation ---
void showTextAnimation() {
  display.setRotation(2); // 180°
  display.setTextColor(SSD1306_WHITE);
  display.setFont(&FreeMonoBold18pt7b);

  // 1. One letter at a time
  for (int i = 0; i < fullText.length(); i++) {
    display.clearDisplay();
    String ch = String(fullText.charAt(i));
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(ch, 0, 0, &x1, &y1, &w, &h);
    int x = (SCREEN_WIDTH - w) / 2 - x1;
    int y = (SCREEN_HEIGHT + h) / 2;
    display.setCursor(x, y);
    display.print(ch);
    display.display();
    playTone(BUZZER_PIN, 1000, 80);  // Beep on each letter
    delay(400);
  }

  delay(800);

  // 2. Static blinking text
  display.setFont(NULL);
  display.setTextSize(2);
  String line1 = "HELLO";
  String line2 = "SNEHASISH";

  int16_t x1, y1, x2, y2;
  uint16_t w1, h1, w2, h2;
  display.getTextBounds(line1, 0, 0, &x1, &y1, &w1, &h1);
  display.getTextBounds(line2, 0, 0, &x2, &y2, &w2, &h2);

  int lx1 = (SCREEN_WIDTH - w1) / 2 - x1;
  int ly1 = 20;
  int lx2 = (SCREEN_WIDTH - w2) / 2 - x2;
  int ly2 = 48;

  for (int i = 0; i < 4; i++) {
    display.clearDisplay();
    display.display();
    delay(300);
    display.setCursor(lx1, ly1);
    display.print(line1);
    display.setCursor(lx2, ly2);
    display.print(line2);
    display.display();
    delay(500);
  }

  delay(1000);
  display.clearDisplay();
  display.display();
}

// --- Scroll the "Let's ROCK!!!" text ---
void scrollMessage() {
  display.setRotation(2);
  display.setTextColor(SSD1306_WHITE);
  display.setFont(NULL);
  display.setTextSize(2);
  String msg = "Let's ROCK!!!";
  int textWidth = msg.length() * 12; // Approx width for size 2 font

  for (int x = SCREEN_WIDTH; x > -textWidth; x--) {
    display.clearDisplay();
    display.setCursor(x, 32);  // vertical center
    display.print(msg);
    display.display();
    delay(25);
  }
  delay(500);
  display.clearDisplay();
  display.display();
}

// --- Setup ---
void setup() {
  Wire.begin(22, 23);  // SDA = GPIO22, SCL = GPIO23
  Serial.begin(9600);
  delay(100);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 init failed"));
    while (1);
  }

  display.clearDisplay();
  display.display();

  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 60);
  roboEyes.setPosition(DEFAULT);
  roboEyes.close();

  showTextAnimation();
  eventTimer = millis();
}

// --- Main loop ---
void loop() {
  roboEyes.update();
  unsigned long now = millis();

  switch (currentStep) {
    case 0:
      if (now - eventTimer >= 10000) {
        roboEyes.open();
        playTone(BUZZER_PIN, 1500, 100);  // Sound on eye open
        currentStep++;
        showTextAnimation();
        eventTimer = millis();
      }
      break;

    case 1:
      if (now - eventTimer >= 10000) {
        roboEyes.setMood(HAPPY);
        playTone(BUZZER_PIN, 1800, 150);  // Happy tone
        roboEyes.anim_laugh();
        currentStep++;
        showTextAnimation();
        eventTimer = millis();
      }
      break;

    case 2:
      if (now - eventTimer >= 10000) {
        roboEyes.setMood(ANGRY);
        playTone(BUZZER_PIN, 1000, 300);  // Angry tone
        currentStep++;
        showTextAnimation();
        eventTimer = millis();
      }
      break;

    case 3:
      if (now - eventTimer >= 10000) {
        roboEyes.setMood(TIRED);
        playTone(BUZZER_PIN, 800, 300);  // Tired tone
        currentStep++;
        showTextAnimation();
        eventTimer = millis();
      }
      break;

    case 4:
      if (now - eventTimer >= 10000) {
        roboEyes.setMood(DEFAULT);
        roboEyes.close();
        playTone(BUZZER_PIN, 1200, 100);  // Eye close tone
        delay(300);
        roboEyes.open();
        playTone(BUZZER_PIN, 1400, 100);  // Eye re-open tone
        scrollMessage();
        currentStep = 0;
        showTextAnimation();
        eventTimer = millis();
      }
      break;
  }
}
