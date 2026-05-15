#include <Adafruit_NeoPixel.h>

#define LED_PIN     6        // Pin connected to LED strip
#define BTN_START   2        // Start/reset button
#define BTN_PLAYER1 3        // Player 1 button
#define BTN_PLAYER2 4        // Player 2 button
#define BUZZER_PIN  5        // Buzzer pin

#define TOTAL_LEDS  60       // Total number of LEDs
#define PLAYER_LEDS 3        // 3 LEDs per player
#define LAPS        2        // Total laps to win
#define SPEED       50       // Speed of LED updates (higher = slower)

// Player Colors
#define COLOR_P1    strip.Color(255, 0, 0)  // Player 1 Red
#define COLOR_P2    strip.Color(0, 255, 0)  // Player 2 Green

Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

int player1Pos = 0, player2Pos = 0;
int player1Laps = 0, player2Laps = 0;
bool gameActive = false;

// Track previous button states
bool lastPlayer1State = HIGH;
bool lastPlayer2State = HIGH;

void setup() {
  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(BTN_PLAYER1, INPUT_PULLUP);
  pinMode(BTN_PLAYER2, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  strip.begin();
  strip.setBrightness(50);
  strip.show();
}

void loop() {
  // Sparkle effect until game starts
  while (!gameActive) {
    sparkleEffect();
    if (digitalRead(BTN_START) == LOW) {
      resetGame();
    }
  }

  // Main game loop
  if (digitalRead(BTN_START) == LOW) {  // game restart when start button is pressed
    resetGame();
  }

  if (gameActive) {
    // Read current button states
    bool currentPlayer1State = digitalRead(BTN_PLAYER1);
    bool currentPlayer2State = digitalRead(BTN_PLAYER2);

    // Detect state change for Player 1
    if (lastPlayer1State == HIGH && currentPlayer1State == LOW) {
      movePlayer(1);  // Only move when button is pressed
    }
    lastPlayer1State = currentPlayer1State;  // Update last state

    // Detect state change for Player 2
    if (lastPlayer2State == HIGH && currentPlayer2State == LOW) {
      movePlayer(2);  // Only move when button is pressed
    }
    lastPlayer2State = currentPlayer2State;  // Update last state

    updateLeds();
    checkWinner();
    delay(SPEED);
  }
}

// --- Core Functionalities ---
void movePlayer(int player) {
  if (player == 1) {
    player1Pos = (player1Pos + 1) % TOTAL_LEDS;
    if (player1Pos == 0) player1Laps++;
  } else {
    player2Pos = (player2Pos + 1) % TOTAL_LEDS;
    if (player2Pos == 0) player2Laps++;
  }
}
void updateLeds() {
  strip.clear();

  // Track which LEDs are set for overlap handling
  bool ledOccupied[TOTAL_LEDS] = {false};

  // Draw Player 2 LEDs first
  for (int i = 0; i < PLAYER_LEDS; i++) {
    int pos = (player2Pos + i) % TOTAL_LEDS;
    strip.setPixelColor(pos, COLOR_P2);  // Player 2 LEDs
    ledOccupied[pos] = true;             // Mark LED as occupied by Player 2
  }

  // Draw Player 1 LEDs
  for (int i = 0; i < PLAYER_LEDS; i++) {
    int pos = (player1Pos + i) % TOTAL_LEDS;
    if (ledOccupied[pos]) {
      // If Player 1 overlaps Player 2, set blue
      strip.setPixelColor(pos, strip.Color(0, 0, 255));  // Blue for overlap
    } else {
      // Otherwise, draw Player 1 LED
      strip.setPixelColor(pos, COLOR_P1);
    }
  }

  strip.show();
}

void checkWinner() {
  if (player1Laps >= LAPS || player2Laps >= LAPS) {
    gameActive = false;
    tone(BUZZER_PIN, 1000, 200);  // End beep
    displayWinner();
  }
}

void displayWinner() {
  strip.clear();
  uint32_t winnerColor = player1Laps >= LAPS ? COLOR_P1 : COLOR_P2;

  for (int i = 0; i < TOTAL_LEDS; i += 3) {
    strip.setPixelColor(i, winnerColor);  // Fill strip with winner color
    strip.show();
    delay(25);  // Moving sparkle effect
  }
  delay(2000);
}

// Reset and Effects
void resetGame() {
  gameActive = true;
  player1Pos = player2Pos = 0;
  player1Laps = player2Laps = 0;
  tone(BUZZER_PIN, 500, 200);  // Start beep
  strip.clear();
  strip.show();
}

void sparkleEffect() {
  strip.clear();
  for (int i = 0; i < 10; i++) {  // Random sparkles
    int ledIndex = random(0, TOTAL_LEDS);
    strip.setPixelColor(ledIndex, random(0, 256), random(0, 256), random(0, 256));  // Random colors
  }
  strip.show();
  delay(25);  // Sparkle effect speed
}
