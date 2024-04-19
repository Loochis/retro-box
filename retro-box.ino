#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define INPUT_U D3
#define INPUT_D D6
#define INPUT_L D7
#define INPUT_R D5

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

static const int screen_half_x = SCREEN_WIDTH / 2;
static const int screen_half_y = SCREEN_HEIGHT / 2;

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DEG90RAD 1.5708


// ---- BITMAPS ---- //

// FONT
static const unsigned char digits_small[10][3] = {
  { 0b11110110, 0b11011110 }, // 0
  { 0b11001001, 0b00101110 }, // 1
  { 0b11100111, 0b11001110 }, // 2
  { 0b11100101, 0b10011110 }, // 3
  { 0b10110111, 0b10010010 }, // 4
  { 0b11110011, 0b10011110 }, // 5
  { 0b11110011, 0b11011110 }, // 6
  { 0b11100100, 0b10010010 }, // 7
  { 0b11110111, 0b11011110 }, // 8
  { 0b11110111, 0b10011110 }  // 9
};

// THUMBNAILS
static const unsigned char snake_thumb[] = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00100000, 0b00000100, 0b11111111, 0b10011100,
  0b01110011, 0b11111110, 0b00111111, 0b00100011, 0b10000100, 0b11111111, 0b10011100, 0b01110011, 0b11111110, 0b00111111, 0b11100011, 0b11000100,
  0b11111111, 0b10011100, 0b01110011, 0b11111110, 0b00111000, 0b00000111, 0b11001100, 0b11100011, 0b10011100, 0b11100011, 0b10000000, 0b00111000,
  0b00000111, 0b11001100, 0b11100011, 0b10011101, 0b11000011, 0b10000000, 0b00111000, 0b00000111, 0b11101100, 0b11100000, 0b00000000, 0b00000000,
  0b00000000, 0b00111111, 0b11000110, 0b11101100, 0b11000011, 0b11111111, 0b11111111, 0b10010000, 0b00111111, 0b11100110, 0b11101100, 0b00011111,
  0b11111111, 0b11111111, 0b10010000, 0b00011111, 0b11100110, 0b11101100, 0b11111111, 0b11111111, 0b11111111, 0b11110000, 0b00000000, 0b11100110,
  0b11111100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11100111, 0b01111100, 0b11100011, 0b10011101, 0b11000011, 0b10000000,
  0b00000000, 0b11100111, 0b01111100, 0b11100011, 0b10011100, 0b11100011, 0b10000000, 0b00000000, 0b11100001, 0b01111000, 0b11100011, 0b10011100,
  0b01110011, 0b11111110, 0b00000111, 0b11100001, 0b00111000, 0b11100011, 0b10011100, 0b01110011, 0b11111110, 0b00111111, 0b11000111, 0b00000000,
  0b11100011, 0b10011100, 0b01110011, 0b11111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001000, 0b00000000, 0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00100000, 0b00000000,
  0b00000100, 0b00000000, 0b00011111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01111001, 0b11111111, 0b11110000, 0b00000000,
  0b00000000, 0b00000010, 0b00000000, 0b11000111, 0b11111111, 0b11111000, 0b00000000, 0b00000000, 0b01000000, 0b00010100, 0b11000111, 0b11100000,
  0b00001100, 0b00000000, 0b00000100, 0b00010000, 0b10000001, 0b11000111, 0b00000000, 0b00000100, 0b00000000, 0b00100000, 0b00100010, 0b00010011,
  0b11111110, 0b01110000, 0b00010000, 0b00000000, 0b00000001, 0b01010111, 0b11000111, 0b11111100, 0b00110000, 0b00001100, 0b00000000, 0b00010000,
  0b01110000, 0b00011111, 0b11110000, 0b00110000, 0b00001100, 0b00000000, 0b00101011, 0b00000111, 0b11111011, 0b11100000, 0b00100000, 0b00001000,
  0b00000000, 0b00011000, 0b01111111, 0b01110111, 0b10001000, 0b01000000, 0b00010000, 0b00000000, 0b00100011, 0b11111010, 0b10101111, 0b00010000,
  0b00000000, 0b00000000, 0b00000000, 0b00001110, 0b11111101, 0b11011100, 0b00010000, 0b00000000, 0b00000000, 0b10000000, 0b00111011, 0b01101011,
  0b10101110, 0b00100000, 0b00000000, 0b00000001, 0b00000000, 0b01010111, 0b10110111, 0b01110111, 0b10010000, 0b00000001, 0b00000111, 0b11000000,
  0b00101111, 0b11011011, 0b10101011, 0b11001000, 0b00000000, 0b00001001, 0b01100000, 0b01011110, 0b11101101, 0b11011101, 0b11101000, 0b00000010,
  0b00001011, 0b11100000, 0b00111011, 0b01011111, 0b10101111, 0b11100000, 0b00000010, 0b00001111, 0b11100000, 0b01010111, 0b10110111, 0b01110111,
  0b01110000, 0b00000000, 0b00001111, 0b11100000, 0b00101111, 0b01001011, 0b11111111, 0b10110000, 0b00000000, 0b00001111, 0b11100000, 0b01010110,
  0b11010101, 0b11000000, 0b11111000, 0b00000001, 0b00000111, 0b11000000, 0b00111001, 0b10111110, 0b00001110, 0b00011100, 0b00000000, 0b00000000,
  0b00000000, 0b01010101, 0b11110000, 0b11101010, 0b00001111, 0b00000000, 0b10000000, 0b00000000, 0b00101111, 0b11000011, 0b01010100, 0b00000111,
  0b11000000, 0b01000000, 0b00000000, 0b01010111, 0b00001100, 0b10100000, 0b00000000, 0b11111111, 0b10000000, 0b00000000, 0b00111000, 0b01100000,
  0b01000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01000011, 0b01000100, 0b00000010, 0b00010000, 0b00000000, 0b00000000, 0b00000000,
  0b00001100, 0b10100001, 0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00110100, 0b01000000, 0b00000000, 0b00000000, 0b00000000,
  0b10000000, 0b00000000, 0b00001010, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000
};

static const unsigned char asteroids_thumb[] = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000100, 0b01101111, 0b11001111, 0b00111000, 0b00111001,
  0b11110111, 0b00001110, 0b00001101, 0b11000001, 0b00010000, 0b01000100, 0b01000100, 0b01000100, 0b10111000, 0b00001101, 0b00000001, 0b00010000,
  0b01000100, 0b01000100, 0b01000100, 0b10100001, 0b00010101, 0b11100010, 0b00111100, 0b11111000, 0b10001000, 0b10001000, 0b10111100, 0b00111100,
  0b00100100, 0b01000001, 0b00010001, 0b00010001, 0b00010001, 0b00000101, 0b00100100, 0b11100100, 0b01000001, 0b00001001, 0b00010001, 0b00010010,
  0b01111100, 0b01000101, 0b10001000, 0b11111010, 0b00010000, 0b11100111, 0b11011100, 0b11000001, 0b00000000, 0b00000000, 0b00000000, 0b00000010,
  0b00000000, 0b00000000, 0b00001010, 0b00000000, 0b00000101, 0b01010000, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01100000, 0b00000000,
  0b10101010, 0b00101010, 0b10101011, 0b10101110, 0b11101111, 0b00111100, 0b00111110, 0b01111111, 0b11001111, 0b11111110, 0b11111111, 0b11111000,
  0b00000000, 0b00000111, 0b00101110, 0b11110011, 0b01011111, 0b11101111, 0b00000111, 0b00000000, 0b00000001, 0b10011111, 0b11111100, 0b11111111,
  0b11111100, 0b01111111, 0b00000000, 0b00000000, 0b01000111, 0b11111110, 0b00000000, 0b11000011, 0b11111111, 0b00000000, 0b00000000, 0b00010011,
  0b11111110, 0b10001110, 0b00001111, 0b11111111, 0b00111111, 0b00000000, 0b00000001, 0b11111110, 0b10000000, 0b01101111, 0b11111111, 0b00011111,
  0b11111110, 0b00000001, 0b11111110, 0b11000011, 0b01101111, 0b11111111, 0b00011111, 0b11111111, 0b11111101, 0b11111110, 0b00001001, 0b01101111,
  0b11111111, 0b00000011, 0b11111111, 0b11110001, 0b11111000, 0b00011001, 0b01101111, 0b11111111, 0b00000000, 0b01111111, 0b10000111, 0b11100011,
  0b00110001, 0b01101111, 0b11111111, 0b00000000, 0b00111100, 0b00011111, 0b10001110, 0b00100000, 0b01101111, 0b11111111, 0b00000000, 0b00000000,
  0b00111111, 0b11111110, 0b01100000, 0b11001111, 0b11111111, 0b00000000, 0b00000000, 0b00011111, 0b11111100, 0b11000001, 0b11011111, 0b11111111,
  0b00000000, 0b00000001, 0b00011111, 0b11111001, 0b10000000, 0b10011111, 0b11111111, 0b00000000, 0b00000110, 0b00001111, 0b11110011, 0b00010010,
  0b10001111, 0b11111111, 0b00000000, 0b00011100, 0b00001110, 0b11100011, 0b00010010, 0b00100111, 0b11111111, 0b00000000, 0b00000000, 0b00000110,
  0b10000110, 0b00110010, 0b01110111, 0b11111111, 0b00000000, 0b00000000, 0b00000110, 0b10001100, 0b00111001, 0b01110111, 0b11111111, 0b00000000,
  0b00000000, 0b00010010, 0b00011000, 0b01111001, 0b01100111, 0b11111111, 0b00000000, 0b00000000, 0b00011001, 0b00111000, 0b11111101, 0b00101111,
  0b11111111, 0b00000000, 0b00000000, 0b00001101, 0b00110000, 0b11111100, 0b10001111, 0b11111111, 0b00000000, 0b00000000, 0b00000111, 0b01100001,
  0b11111100, 0b10011111, 0b11111111, 0b00000000, 0b00000000, 0b00000001, 0b11100001, 0b11111110, 0b00011111, 0b11111111, 0b00000000, 0b00000000,
  0b00000111, 0b11000011, 0b11111111, 0b01011111, 0b11111111, 0b00000000, 0b00000011, 0b11111111, 0b11111100, 0b01111111, 0b00001111, 0b11111111,
  0b00000011, 0b11111111, 0b11111110, 0b10110011, 0b11111111, 0b00000111, 0b11111111, 0b11111111, 0b11111111, 0b11111101, 0b10010011, 0b11111111,
  0b10000111, 0b11111111, 0b11111111, 0b11111111, 0b11111011, 0b00001011, 0b11111111, 0b10000011, 0b11111111, 0b11111111, 0b11111111, 0b11110101,
  0b00000001, 0b11111111, 0b11000011, 0b11111111, 0b11111111, 0b11111111, 0b11101011, 0b00000001, 0b11111111, 0b11000001, 0b11111111, 0b11111111,
  0b11111111, 0b11010101, 0b00000000, 0b11111111, 0b11100001, 0b11111111, 0b11111111, 0b11111111, 0b10101011, 0b00000000, 0b01111111, 0b11100001,
  0b11111111, 0b11111111, 0b11111111, 0b01010101, 0b00000000, 0b00111111, 0b11110000, 0b11111111, 0b11111111, 0b11111110, 0b10101011, 0b00000000,
  0b00011111, 0b11110000, 0b11111111, 0b11111111, 0b11111101, 0b01010110, 0b00000000, 0b00000011, 0b11111000, 0b11111111, 0b11111111, 0b11111010,
  0b10101010, 0b00000000, 0b00000000, 0b01111000, 0b00011111, 0b11111111, 0b11110101, 0b01010110, 0b00000000, 0b00000000, 0b00001000, 0b00000000,
  0b11111111, 0b11101010, 0b10101010, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11010101, 0b01010110, 0b00000000, 0b00000000,
  0b00000000, 0b00100000, 0b11111111, 0b10101010, 0b10101010, 0b00000000, 0b00000001, 0b00000000, 0b00110000, 0b00000111, 0b01010101, 0b01010110,
  0b00000000, 0b00000011, 0b00000000, 0b00011000, 0b11110001, 0b10101010, 0b10101010, 0b00000000, 0b00000011, 0b00000000, 0b00000000, 0b00000001,
  0b01010101, 0b01010100, 0b00000000, 0b00000011, 0b00000100, 0b00000000, 0b11111100, 0b10101010, 0b10101100, 0b00000000, 0b00000011, 0b00000110,
  0b00000000, 0b00000000, 0b11010101, 0b01010100, 0b00000000, 0b00000000, 0b00000010, 0b00000000
};

// GAME ASSETS

static const unsigned char faster_bmp[] = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00011011, 0b00001111, 0b11011111, 0b10111111, 0b01111110, 0b11111101, 0b11111000, 0b01101100, 
  0b00011011, 0b00001111, 0b11011111, 0b10111111, 0b01111110, 0b11111101, 0b11111000, 0b01101100, 
  0b00011011, 0b00001100, 0b00011001, 0b10110000, 0b00011000, 0b11000001, 0b10011000, 0b01101100,
  0b00011011, 0b00001100, 0b00011001, 0b10110000, 0b00011000, 0b11000001, 0b10011000, 0b01101100, 
  0b00011011, 0b00001111, 0b00011111, 0b10111111, 0b00011000, 0b11110001, 0b11100000, 0b01101100, 
  0b00011011, 0b00001111, 0b00011111, 0b10111111, 0b00011000, 0b11110001, 0b11100000, 0b01101100,
  0b00011011, 0b00001100, 0b00011001, 0b10000011, 0b00011000, 0b11000001, 0b10011000, 0b01101100, 
  0b00011011, 0b00001100, 0b00011001, 0b10000011, 0b00011000, 0b11000001, 0b10011000, 0b01101100, 
  0b00011011, 0b00001100, 0b00011001, 0b10000011, 0b00011000, 0b11000001, 0b10011000, 0b01101100,
  0b00011011, 0b00001100, 0b00011001, 0b10000011, 0b00011000, 0b11000001, 0b10011000, 0b01101100, 
  0b00000000, 0b00001100, 0b00011001, 0b10000011, 0b00011000, 0b11000001, 0b10011000, 0b00000000, 
  0b00011011, 0b00001100, 0b00011001, 0b10111111, 0b00011000, 0b11111101, 0b10011000, 0b01101100,
  0b00011011, 0b00001100, 0b00011001, 0b10111111, 0b00011000, 0b11111101, 0b10011000, 0b01101100, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111100,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000
};

static const unsigned char snake_scoreboard[] = {
  0b11101110, 0b11101110, 0b11100000, 0b00000000, 0b00000011, 0b10111011, 0b10111000, 0b10001000, 0b10101010, 0b10001000, 0b00000000, 0b00000010,
  0b10100010, 0b00010010, 0b11101000, 0b10101100, 0b11000000, 0b00000000, 0b00000011, 0b00110011, 0b10010000, 0b00101000, 0b10101010, 0b10001000,
  0b00000000, 0b00000010, 0b10100000, 0b10010010, 0b11101110, 0b11101010, 0b11100000, 0b00000000, 0b00000011, 0b10111011, 0b10010000
};

// --- GAME LIST --- //

#define GAME_COUNT 2
int current_game = 0;

static const unsigned char* game_thumbs[] = {
  snake_thumb,
  asteroids_thumb
};

// ---- START CODE ---- //

void setup() {
  // Start serial
  Serial.begin(9600);


  // Setup input pins
  pinMode(INPUT_U, INPUT_PULLUP);
  pinMode(INPUT_D, INPUT_PULLUP);
  pinMode(INPUT_L, INPUT_PULLUP);
  pinMode(INPUT_R, INPUT_PULLUP);

  // Set the I2C bus to fast mode
  Wire.setClock(400000);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Set the I2C bus to fast mode
  Wire.setClock(400000);

  // Clear the buffer
  display.clearDisplay();

  // Override menu
  //asteroids_Game();
}

void loop() {
  menuSM();
}

// ---------------- MATH / UTILITY FUNCTIONS ----------------

struct screen_coord {
  uint16_t x = 0;
  uint16_t y = 0;
};

struct world_coord {
  float x = 0;
  float y = 0;
  float z = 0;
};

float lerp(float a, float b, float t, bool clamp = true) {
  if (t < 0 && clamp) t = 0;
  if (t > 1 && clamp) t = 1;

  return a*(1.0f-t) + b*t;
}

// 3D transformation variables
float scale_fac = 200;
float persp_fac = 1; 
world_coord model_translate;

struct screen_coord mdlToScreen(struct world_coord coords) {
  coords.x += model_translate.x;
  coords.y += model_translate.y;
  coords.z += model_translate.z;
  
  coords.x *= scale_fac / coords.z;
  coords.y *= scale_fac / coords.z;

  coords.x += screen_half_x;
  coords.y += screen_half_y;

  screen_coord retCoord;
  retCoord.x = coords.x; // implicit truncation
  retCoord.y = coords.y;
  return retCoord;
}

struct world_coord collateVertData(float *vert_arr, char *ind_arr, int index) {
  world_coord retCoord;
  retCoord.x = vert_arr[ind_arr[index]*3 + 0];
  retCoord.y = vert_arr[ind_arr[index]*3 + 1];
  retCoord.z = vert_arr[ind_arr[index]*3 + 2];
  return retCoord;
}

struct world_coord collateVertData(float *vert_arr, int index) {
  world_coord retCoord;
  retCoord.x = vert_arr[index*3 + 0];
  retCoord.y = vert_arr[index*3 + 1];
  retCoord.z = vert_arr[index*3 + 2];
  return retCoord;
}

void draw_digit(int digit, int x_pos, int y_pos, int scale) {
  int ctr = 0;
  for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 3; x++) {
      if ((digits_small[digit][ctr/8] << ctr%8) & 0b10000000) {
        display.fillRect(x_pos+x*scale, y_pos+y*scale, scale, scale, 1);
      }
      ctr++;
    }
  }
}

// --------------- MENU RELATED FUNCTIONALITY ---------------

static const unsigned char* GetMenuThumb(int idx) {
   if (idx >= GAME_COUNT) return game_thumbs[0];
   if (idx < 0) return game_thumbs[GAME_COUNT - 1];
   return game_thumbs[idx];
}

// Main menu cube definition
static float verts[] = {
  -1, -1, -1, // FTL // MM
  -1,  1, -1, // FBL // MM
   1, -1, -1, // FTR // MM
   1,  1, -1, // FBR // MM
  -1, -1,  1, // BTL // RT
  -1,  1,  1, // BBL // RT
   1, -1,  1, // BTR // LT
   1,  1,  1  // BBR // LT
};

static screen_coord ss_buffer[8];

static char f_indices[] = {
  0, 1, // F
  0, 2, // F
  3, 1, // F
  3, 2  // F
};

static char l_indices[] = {
  0, 1,
  0, 4,
  5, 1,
  5, 4
};

static char r_indices[] = {
  2, 3,
  2, 6,
  7, 3,
  7, 6
};

static char t_indices[] = {
  0, 2,
  0, 4,
  6, 2,
  6, 4
};

float menu_accspeed = 1;
bool firstframe = false;
int num_ffs = 0;
void menuSM() {
  
  display.clearDisplay();

  // Set transform vars
  scale_fac = 200;
  persp_fac = 1; 
  model_translate.z = 8;
  model_translate.x = -1;
  
  // Draw front face of cube
  for (int i = 0; i < 4; i++) {
    world_coord wCoord0 = collateVertData(verts, f_indices, i*2 + 0);
    world_coord wCoord1 = collateVertData(verts, f_indices, i*2 + 1);
        
    screen_coord sCoord0 = mdlToScreen(wCoord0);
    screen_coord sCoord1 = mdlToScreen(wCoord1);
    
    display.drawLine(sCoord0.x, sCoord0.y, sCoord1.x, sCoord1.y, SSD1306_WHITE);
  }

  // Draw game thumbnail
  display.drawBitmap(8, 4, GetMenuThumb(current_game), 56, 56, 1);
  
  display.display();
  
  //delay(200);

  if (firstframe) {
    num_ffs++;
    if (num_ffs > 1 && menu_accspeed < 2)
      menu_accspeed += 0.2; 
  } else {
    num_ffs = 0;
    menu_accspeed = 1;
  }
  
  if (!digitalRead(INPUT_R)) {
    menuSideFlip(true, menu_accspeed);
    current_game++;
    if (current_game >= GAME_COUNT) current_game = 0;
    firstframe = true;
  }
  else if (!digitalRead(INPUT_L)) {
    menuSideFlip(false, menu_accspeed);
    current_game--;
    if (current_game < 0) current_game = GAME_COUNT - 1;
    firstframe = true;
  }
  else if (!digitalRead(INPUT_U)) {
    menuUFlip();
    // start the game
    snake_Game();
  } 
  else {
    firstframe = false;
  }
}

void menuSideFlip(bool dir, float speed) {
  float theta = 0;
  float t_val = 0;
  
  for (int d = 0; d < int(12/speed); d++) {
    t_val = lerp(t_val, 1, 0.25);
    //t_val = (float)(d) / floor(10.0f/speed);

    if (dir)
      theta = lerp(0, -DEG90RAD, t_val);
    else
      theta = lerp(0, DEG90RAD, t_val);
    
    float sinTheta = sin(theta);
    float cosTheta = cos(theta);

    // compute screenspace coordinates
    for (int i = 0; i < 8; i++) {
      world_coord wCoord = collateVertData(verts, i);
      world_coord wCoordT;

      wCoordT.x = wCoord.x*cosTheta - wCoord.z*sinTheta;
      wCoordT.y = wCoord.y;
      wCoordT.z = wCoord.x*sinTheta + wCoord.z*cosTheta;

      ss_buffer[i] = mdlToScreen(wCoordT);
    }

    display.clearDisplay();

    if (dir) {
      // draw thumbnails
      unsigned char bmpXOff = ss_buffer[2].x;
      display.drawBitmap(bmpXOff - 57, 4, GetMenuThumb(current_game), 56, 56, 1);
      display.fillRect(ss_buffer[0].x - 56, 4, 56, 56, SSD1306_BLACK);
      display.drawBitmap(bmpXOff + 1, 4, GetMenuThumb(current_game + 1), 56, 56, 1);
      display.fillRect(ss_buffer[6].x, 4, 56, 56, SSD1306_BLACK);
      
    }
    else
    {
      // draw thumbnails
      unsigned char bmpXOff = ss_buffer[0].x;
      display.drawBitmap(bmpXOff - 57, 4, GetMenuThumb(current_game - 1), 56, 56, 1);
      display.fillRect(min(ss_buffer[2].x, ss_buffer[4].x) - 56, 4, 56, 56, SSD1306_BLACK);
      display.drawBitmap(bmpXOff + 1, 4, GetMenuThumb(current_game), 56, 56, 1);
      display.fillRect(ss_buffer[2].x, 4, 56, 56, SSD1306_BLACK);
      
    }

    // draw front face
    // cull front face when behind right face
    if (ss_buffer[0].x <= ss_buffer[2].x)
      for (int i = 0; i < 4; i++)
        display.drawLine(ss_buffer[f_indices[i*2 + 0]].x, ss_buffer[f_indices[i*2 + 0]].y, ss_buffer[f_indices[i*2 + 1]].x, ss_buffer[f_indices[i*2 + 1]].y, SSD1306_WHITE);

    if (dir) // Draw right face
      for (int i = 0; i < 4; i++)
        display.drawLine(ss_buffer[r_indices[i*2 + 0]].x, ss_buffer[r_indices[i*2 + 0]].y, ss_buffer[r_indices[i*2 + 1]].x, ss_buffer[r_indices[i*2 + 1]].y, SSD1306_WHITE);
    else     // Draw left face
      for (int i = 0; i < 4; i++)
        display.drawLine(ss_buffer[l_indices[i*2 + 0]].x, ss_buffer[l_indices[i*2 + 0]].y, ss_buffer[l_indices[i*2 + 1]].x, ss_buffer[l_indices[i*2 + 1]].y, SSD1306_WHITE);

    display.display();
    delay(5);
  }
}


void menuUFlip() {
  float theta = 0;
  float t_val = 0;
  
  for (int d = 0; d < 12; d++) {
    
    t_val = lerp(t_val, 1, 0.25);
    theta = lerp(0, DEG90RAD, t_val);

    // animate box towards center of screen
    model_translate.x = lerp(-1, 0, t_val);
    
    float sinTheta = sin(theta);
    float cosTheta = cos(theta);

    // compute screenspace coordinates
    for (int i = 0; i < 8; i++) {
      world_coord wCoord = collateVertData(verts, i);
      world_coord wCoordT;

      wCoordT.x = wCoord.x;
      wCoordT.y = wCoord.y*cosTheta - wCoord.z*sinTheta;
      wCoordT.z = wCoord.y*sinTheta + wCoord.z*cosTheta;

      ss_buffer[i] = mdlToScreen(wCoordT);
    }

    display.clearDisplay();
    
    // draw thumbnail
    unsigned char bmpXOff = ss_buffer[0].x;
    unsigned char bmpYOff = ss_buffer[0].y;
    display.drawBitmap(bmpXOff + 1, bmpYOff + 1, GetMenuThumb(current_game), 56, 56, 1);
    display.fillRect(bmpXOff + 1, ss_buffer[1].y, 56, 56, SSD1306_BLACK);

    // draw front face
    // cull front face when behind top face
    if (ss_buffer[0].y <= ss_buffer[1].y)
      for (int i = 0; i < 4; i++)
        display.drawLine(ss_buffer[f_indices[i*2 + 0]].x, ss_buffer[f_indices[i*2 + 0]].y, ss_buffer[f_indices[i*2 + 1]].x, ss_buffer[f_indices[i*2 + 1]].y, SSD1306_WHITE);

    // Draw top face
    for (int i = 0; i < 4; i++)
      display.drawLine(ss_buffer[t_indices[i*2 + 0]].x, ss_buffer[t_indices[i*2 + 0]].y, ss_buffer[t_indices[i*2 + 1]].x, ss_buffer[t_indices[i*2 + 1]].y, SSD1306_WHITE);
      
    display.display();
    delay(5);
  }
  

  // Cube zoom
  for (int i = 0; i < 10; i++) {
    model_translate.z = lerp(8, 0, (float)i/10.0);

    display.clearDisplay();
    // Draw front face of cube
    for (int i = 0; i < 4; i++) {
      world_coord wCoord0 = collateVertData(verts, f_indices, i*2 + 0);
      world_coord wCoord1 = collateVertData(verts, f_indices, i*2 + 1);
          
      screen_coord sCoord0 = mdlToScreen(wCoord0);
      screen_coord sCoord1 = mdlToScreen(wCoord1);
      
      display.drawLine(sCoord0.x, sCoord0.y, sCoord1.x, sCoord1.y, SSD1306_WHITE);
    }

    display.display();
    delay(5);
  }
}


// --------------- // GAMES // --------------- //

struct game_coord {
  unsigned char x = 0;
  unsigned char y = 0;
};

// Input handling

#define MAX_INPUT_BUFFER 2
unsigned char input_buffer[MAX_INPUT_BUFFER];
int input_buffer_idx = 0;

int latest_input = 0;
unsigned char firstframe_dict = 0; // 0000 RLDU

void getInput() {
    // process inputs
    if (digitalRead(INPUT_U)) firstframe_dict &= 0b11111110; // button is not pressed
    else if (!(firstframe_dict & 0b00000001)) 
    { 
      latest_input = 1; 
      firstframe_dict |= 0b00000001; 
      // write input to buffer
      if (input_buffer_idx < MAX_INPUT_BUFFER) {
        input_buffer[input_buffer_idx] = 1;
        input_buffer_idx++;
      }
    }
    if (digitalRead(INPUT_D)) firstframe_dict &= 0b11111101; // button is not pressed
    else if (!(firstframe_dict & 0b00000010)) 
    { 
      latest_input = 2; 
      firstframe_dict |= 0b00000010; 
      // write input to buffer
      if (input_buffer_idx < MAX_INPUT_BUFFER) {
        input_buffer[input_buffer_idx] = 2;
        input_buffer_idx++;
      }
    }
    if (digitalRead(INPUT_L)) firstframe_dict &= 0b11111011; // button is not pressed
    else if (!(firstframe_dict & 0b00000100)) 
    { 
      latest_input = 3; 
      firstframe_dict |= 0b00000100; 
      // write input to buffer
      if (input_buffer_idx < MAX_INPUT_BUFFER) {
        input_buffer[input_buffer_idx] = 3;
        input_buffer_idx++;
      }
    }
    if (digitalRead(INPUT_R)) firstframe_dict &= 0b11110111; // button is not pressed
    else if (!(firstframe_dict & 0b00001000)) 
    { 
      latest_input = 4; 
      firstframe_dict |= 0b00001000; 
      // write input to buffer
      if (input_buffer_idx < MAX_INPUT_BUFFER) {
        input_buffer[input_buffer_idx] = 4;
        input_buffer_idx++;
      }
    }
} 

int popInput() {
  if (!input_buffer_idx) return 0; // no input in buffer

  // pop top off buffer
  unsigned char popped_input = input_buffer[0];
  // shift buffer
  for (int i = 0; i < MAX_INPUT_BUFFER - 1; i++) {
    input_buffer[i] = input_buffer[i+1];
  }

  input_buffer_idx--;
  return popped_input;
}

void resetInput() {
  latest_input = 0;
}

// Particle management
#define PARTICLE_COUNT 50

float particle_gravity = 1;
float particle_lifetime = 30; // seconds

float particle_lifectr[PARTICLE_COUNT];
float particle_coords_x[PARTICLE_COUNT];
float particle_coords_y[PARTICLE_COUNT];
float particle_vel_x[PARTICLE_COUNT];
float particle_vel_y[PARTICLE_COUNT];
bool  particle_alive[PARTICLE_COUNT];

int particle_idx = 0;

void resetParticles() {
  particle_gravity = 1;
  particle_lifetime = 30;
  for (int i = 0; i < PARTICLE_COUNT; i++) {
    particle_lifectr[i] = 0;
    particle_coords_x[i] = 0;
    particle_coords_y[i] = 0;
    particle_vel_x[i] = 0;
    particle_vel_y[i] = 0;
    particle_alive[i] = false;
  }
}

void spawnParticle(float x, float y, float vX, float vY) {
  particle_lifectr[particle_idx] = 0;
  particle_coords_x[particle_idx] = x;
  particle_coords_y[particle_idx] = y;
  particle_vel_x[particle_idx] = vX;
  particle_vel_y[particle_idx] = vY;
  particle_alive[particle_idx] = true;

  particle_idx++;
  if (particle_idx >= PARTICLE_COUNT) particle_idx = 0;
}

void simulateParticles(float d_t) {
  for (int i = 0; i < PARTICLE_COUNT; i++) {
    if (!particle_alive[i]) continue;

    particle_lifectr[i] += d_t;
    if (particle_lifectr[i] > particle_lifetime) {
      particle_alive[i] = false;
      continue;
    }

    particle_coords_x[i] += particle_vel_x[i] * d_t;
    particle_coords_y[i] += particle_vel_y[i] * d_t;

    particle_vel_y[i] += particle_gravity * d_t;
  }
}

void displayParticles() {
  for (int i = 0; i < PARTICLE_COUNT; i++) {
    if (!particle_alive[i]) continue;

    display.drawPixel(int(particle_coords_x[i]), int(particle_coords_y[i]), SSD1306_WHITE);
  }
}

// ------------------ SNAKE ------------------ //
int snake_length = 10;
unsigned char snake_segments_x[512]; // (32x16)
unsigned char snake_segments_y[512]; // (32x16)

unsigned char snake_projected_dir = 4;
unsigned char snake_facing_dir = 4;      // U D L R

int snake_game_delay = 4;
int snake_game_delayctr = 0;

int snake_game_diffgate = 5;
int snake_game_diffctr = 0;

unsigned char snake_apple_x = 20;
unsigned char snake_apple_y = 4;

int snake_score = 0;

void snake_Game() {
  // setup the game

  // reset all vars
  snake_length = 10;
  snake_projected_dir = 4;
  snake_facing_dir = 4;
  snake_game_delay = 3;
  snake_game_delayctr = 0;
  snake_game_diffgate = 5;
  snake_game_diffctr = 0;
  snake_apple_x = 20;
  snake_apple_y = 4;
  snake_score = 0;

  // setup particle vars
  resetParticles();
  
  // make the segments spawn in the abyss of space
  for (int i = 1; i < 512; i++) {
    snake_segments_x[i] = 200;
    snake_segments_y[i] = 200;
  }
  snake_segments_x[0] = 16;
  snake_segments_y[0] = 8;
  
  while (true) {
    snake_InputProcess();
    
    // Create artificial delay in game steps
    snake_game_delayctr++;
    if (snake_game_delayctr >= snake_game_delay) {
      snake_game_delayctr = 0;
      if (snake_GameStep()) return;
    }

    simulateParticles(0.5);
    snake_DisplayGame();
    display.display();
  }
}

// simulate a single step of the games logic
int snake_GameStep() {
    // reverse through body to update chain
    for (int i = snake_length - 1; i >= 1; i--) {
      snake_segments_x[i] = snake_segments_x[i - 1];
      snake_segments_y[i] = snake_segments_y[i - 1];
    }

    // pop input
    int inval = popInput();
    if (inval) {
      bool changeDir = false;
      // prevent backing into yourself
      changeDir |= (inval == 1 && snake_facing_dir == 2);
      changeDir |= (inval == 2 && snake_facing_dir == 1);
      changeDir |= (inval == 3 && snake_facing_dir == 4);
      changeDir |= (inval == 4 && snake_facing_dir == 3);
      if (!changeDir)
        snake_projected_dir = inval;
      resetInput();
    }
    
    // apply cached input
    snake_facing_dir = snake_projected_dir;

    // update head position
    switch (snake_facing_dir) {
      case 1:
        snake_segments_y[0]--;
        break;
      case 2:
        snake_segments_y[0]++;
        break;
      case 3:
        snake_segments_x[0]--;
        break;
      case 4:
        snake_segments_x[0]++;
        break;
    }

    // Check for collisions
    // Apple collision
    if (snake_segments_x[0] == snake_apple_x && snake_segments_y[0] == snake_apple_y) {
      display.invertDisplay(true);
      delay(50);
      display.invertDisplay(false);
      snake_ParticleBurst();
      snake_game_diffctr++;
      if (snake_game_diffctr >= snake_game_diffgate) {
        snake_game_diffctr = 0;
        //snake_SpeedUp();
      }
      snake_length++;
      snake_score++;
      // respawn apple
      bool foundspot = false;
      while (!foundspot) {
        snake_apple_x = random(1, 31);
        snake_apple_y = random(2, 15);
        
        // check apple isn't inside the snake
        foundspot = true;
        for (int i = 0; i < snake_length; i++) {
          if (snake_segments_x[i] == snake_apple_x && snake_segments_y[i] == snake_apple_y) { foundspot = false; break; }
        }
      }
    }

    // Snake on border action
    if (snake_segments_x[0] <= 0 || snake_segments_x[0] >= 31) { snake_GameOver(); return 1; }
    if (snake_segments_y[0] <= 1 || snake_segments_y[0] >= 15) { snake_GameOver(); return 1; }

    // Snake on Snake action
    for (int i = 1; i < snake_length; i++)
      if (snake_segments_x[0] == snake_segments_x[i] && snake_segments_y[0] == snake_segments_y[i])  { snake_GameOver(); return 1; }

    return 0;
}

void snake_DisplayGame() {
    // display the game
    display.clearDisplay();
    // draw border
    display.drawRect(3, 7, 122, 54, SSD1306_WHITE);
    //draw scoreboard
    display.drawBitmap(3, 1, snake_scoreboard, 56, 5, 1);
    if (snake_score >= 100) {
      int place = int(floor(snake_score / 100.0)) % 10;
      //display.drawBitmap(26, 1, digits_small[place], 3, 5, 1);
      draw_digit(place, 26, 1, 1);
    }
    if (snake_score >= 10) {
      int place = int(floor(snake_score / 10.0)) % 10;
      //display.drawBitmap(30, 1, digits_small[place], 3, 5, 1);
      draw_digit(place, 30, 1, 1);
    }
    if (snake_score >= 1) {
      int place = snake_score % 10;
      //display.drawBitmap(34, 1, digits_small[place], 3, 5, 1);
      draw_digit(place, 34, 1, 1);
    }
    // draw apple
    display.drawRect(snake_apple_x*4 + 1, snake_apple_y*4 + 1, 2, 2, SSD1306_WHITE);
    // draw snake
    for (int i = 0; i < snake_length; i++) {
      display.drawRect(snake_segments_x[i]*4, snake_segments_y[i]*4, 4, 4, SSD1306_WHITE);
    }
    
    // draw particles
    displayParticles();
  
    delay(5);
}

void snake_ParticleBurst() {
    for (int i = 0; i < 20; i++) {
      spawnParticle(snake_segments_x[0]*4 + 2, snake_segments_y[0]*4 + 2,
                    (random(-99, 99) / 100.0f)*5, (random(-99, 99) / 100.0f)*5);
    }
}

void snake_SpeedUp() {
  if (snake_game_delay > 5) snake_game_delay--;
  else return;
  
  float textPos = -40;
  for (int i = 0; i <= 10; i++) {

    // STILL PROCESS INPUTS
    snake_InputProcess();
    
    display.fillRect(0, 22, i*13, 20, SSD1306_BLACK);

    textPos = lerp(textPos, 32, 0.3);
    int offX = random(-1,2);
    int offY = random(-1,2);
    display.drawBitmap(int(textPos) + offX, 20 + offY, faster_bmp, 64, 20, 1);
    display.display();
  }
  
  for (int i = 0; i <= 10; i++) {

    // STILL PROCESS INPUTS
    snake_InputProcess();
    
    snake_DisplayGame();
    display.fillRect(i*13, 22, 128, 20, SSD1306_BLACK);

    textPos = lerp(textPos, 136, 0.3);
    int offX = random(-1,2);
    int offY = random(-1,2);
    display.drawBitmap(int(textPos) + offX, 20 + offY, faster_bmp, 64, 20, 1);
    
    display.display();
  }
}

void snake_InputProcess() {
    getInput();
}

// Juicy effects incoming
void snake_GameOver() {
  display.invertDisplay(true);
  delay(500);
  display.invertDisplay(false);

  int ctr = 0;

  for (int z = 0; z < snake_length; z++) {
    for (int d = 0; d < 1; d++) {
      display.clearDisplay();
      for (int i = z; i < snake_length; i++) {
        int shakeX = random(-1, 2);
        int shakeY = random(-1, 2);
        display.drawRect(snake_segments_x[i]*4 + shakeX, snake_segments_y[i]*4 + shakeY, 4, 4, SSD1306_WHITE);
      }
      simulateParticles(0.5);
      displayParticles();
      display.display();
    }

    // spawn some particles
    for (int i = 0; i < 5; i++) {
      spawnParticle(snake_segments_x[z]*4 + 2, snake_segments_y[z]*4 + 2,
                    (random(-99, 99) / 100.0f)*5, (random(-99, 99) / 100.0f)*5);
    }
  }

  // snake is now dissasembled

  int placeH = int(floor(snake_score / 100.0)) % 10;
  int placeT = int(floor(snake_score / 10.0)) % 10;
  int placeO = snake_score % 10;
  
  for (int i = 0; i < 20; i++) {
    display.clearDisplay();
    simulateParticles(1);
    displayParticles();
    
    float shakeX = (random(-99, 99) / 500.0f) * (20-i);
    float shakeY = (random(-99, 99) / 500.0f) * (20-i);
    
    draw_digit(placeH, 42+int(shakeX), 22+int(shakeY), 4);
    
    display.display();
    delay(10);
  }

  for (int i = 0; i < 20; i++) {
    display.clearDisplay();
    simulateParticles(1);
    displayParticles();
    
    float shakeX = (random(-99, 99) / 500.0f) * (20-i);
    float shakeY = (random(-99, 99) / 500.0f) * (20-i);

    draw_digit(placeH, 42, 22, 4);
    draw_digit(placeT, 58+int(shakeX), 22+int(shakeY), 4);
    
    display.display();
    delay(10);
  }

  for (int i = 0; i < 20; i++) {
    display.clearDisplay();
    simulateParticles(1);
    displayParticles();

    float shakeX = (random(-99, 99) / 500.0f) * (20-i);
    float shakeY = (random(-99, 99) / 500.0f) * (20-i);

    draw_digit(placeH, 42, 22, 4);
    draw_digit(placeT, 58, 22, 4);
    draw_digit(placeO, 74+int(shakeX), 22+int(shakeY), 4);
    
    display.display();
    delay(10);
  }

  while (digitalRead(INPUT_U)) {delay(20);}
  
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
}

// ------------------ ASTEROIDS ------------------ //

float asteroids_ship_vecs[] = {
     0,  0.5,
   0.5, -0.5,
     0, -0.2,
  -0.5, -0.5
};

float asteroids_ship_transf[8];

float asteroids_ship_rotation = 0;
float asteroids_ship_scale = 6;

float asteroids_ship_pos_x = 64;
float asteroids_ship_pos_y = 32;
float asteroids_ship_vel_x = 0;
float asteroids_ship_vel_y = 0;

#define ASTEROIDS_MAX_SHOTS 10
int   asteroids_shots_idx = 0;
float asteroids_shots_pos_x[ASTEROIDS_MAX_SHOTS];
float asteroids_shots_pos_y[ASTEROIDS_MAX_SHOTS];
float asteroids_shots_vel_x[ASTEROIDS_MAX_SHOTS];
float asteroids_shots_vel_y[ASTEROIDS_MAX_SHOTS];
bool  asteroids_shots_alive[ASTEROIDS_MAX_SHOTS];

int asteroids_shots_delayGate = 20;
int asteroids_shots_delayCtr = 0;

void asteroids_Game() {

  resetParticles();
  particle_gravity = 0;
  particle_lifetime = 10;
  
  while (true) {
    // update the shot timer
    if (asteroids_shots_delayCtr < asteroids_shots_delayGate)
      asteroids_shots_delayCtr++;

    float sinTheta = sin(asteroids_ship_rotation);
    float cosTheta = cos(asteroids_ship_rotation);
    
    if (!digitalRead(INPUT_L)) asteroids_ship_rotation -= 0.1f;
    if (!digitalRead(INPUT_R)) asteroids_ship_rotation += 0.1f;
    if (!digitalRead(INPUT_U)) {
      asteroids_ship_vel_x -= sinTheta*0.05;
      asteroids_ship_vel_y += cosTheta*0.05;
      
      float particleVX = (random(-99, 99) / 100.0f)*0.5 + sinTheta*2 + asteroids_ship_vel_x/2;
      float particleVY = (random(-99, 99) / 100.0f)*0.5 - cosTheta*2 + asteroids_ship_vel_y/2;
      spawnParticle(asteroids_ship_transf[4], asteroids_ship_transf[5],
                   particleVX, particleVY);
    }
    if (!digitalRead(INPUT_D) && asteroids_shots_delayCtr == asteroids_shots_delayGate) {
      asteroids_shots_delayCtr = 0;
      asteroids_shots_idx++;
      if (asteroids_shots_idx == ASTEROIDS_MAX_SHOTS) asteroids_shots_idx = 0;
      
      asteroids_shots_alive[asteroids_shots_idx] = true;
      asteroids_shots_pos_x[asteroids_shots_idx] = asteroids_ship_transf[0];
      asteroids_shots_pos_y[asteroids_shots_idx] = asteroids_ship_transf[1];
      asteroids_shots_vel_x[asteroids_shots_idx] = -sinTheta*4;
      asteroids_shots_vel_y[asteroids_shots_idx] = cosTheta*4;
    }

    // apply euler fysiks
    asteroids_ship_pos_x += asteroids_ship_vel_x;
    asteroids_ship_pos_y += asteroids_ship_vel_y;

    for (int i = 0; i < ASTEROIDS_MAX_SHOTS; i++) {
      if (!asteroids_shots_alive[i]) continue;
      asteroids_shots_pos_x[i] += asteroids_shots_vel_x[i];
      asteroids_shots_pos_y[i] += asteroids_shots_vel_y[i];
      if (asteroids_shots_pos_x[i] > 127 || asteroids_shots_pos_x[i] < 0) asteroids_shots_alive[i] = false;
      if (asteroids_shots_pos_y[i] > 63  || asteroids_shots_pos_y[i] < 0) asteroids_shots_alive[i] = false;
    }

    // apply non-euler fysiks
    if (asteroids_ship_pos_x > 127) asteroids_ship_pos_x = 0;
    if (asteroids_ship_pos_x < 0) asteroids_ship_pos_x = 127;

    if (asteroids_ship_pos_y > 63) asteroids_ship_pos_y = 0;
    if (asteroids_ship_pos_y < 0) asteroids_ship_pos_y = 63;
    
    // compute ship points
    for (int i = 0; i < 4; i++) {
      asteroids_ship_transf[i*2]   = cosTheta*asteroids_ship_vecs[i*2] - sinTheta*asteroids_ship_vecs[i*2+1];
      asteroids_ship_transf[i*2+1] = sinTheta*asteroids_ship_vecs[i*2] + cosTheta*asteroids_ship_vecs[i*2+1];

      asteroids_ship_transf[i*2]   *= asteroids_ship_scale;
      asteroids_ship_transf[i*2+1] *= asteroids_ship_scale;

      asteroids_ship_transf[i*2]   += asteroids_ship_pos_x;
      asteroids_ship_transf[i*2+1] += asteroids_ship_pos_y;
    }

    // simulate particles
    simulateParticles(0.5);

    display.clearDisplay();

    // display ship
    for (int i = 0; i < 3; i++) {
      display.drawLine(asteroids_ship_transf[i*2], asteroids_ship_transf[i*2+1], asteroids_ship_transf[i*2+2], asteroids_ship_transf[i*2+3], 1);
    }
    display.drawLine(asteroids_ship_transf[6], asteroids_ship_transf[7], asteroids_ship_transf[0], asteroids_ship_transf[1], 1);

    // display shots
    for (int i = 0; i < ASTEROIDS_MAX_SHOTS; i++) {
      if (!asteroids_shots_alive[i]) continue;
      display.drawLine(asteroids_shots_pos_x[i] - asteroids_shots_vel_x[i]/2, 
                       asteroids_shots_pos_y[i] - asteroids_shots_vel_y[i]/2, 
                       asteroids_shots_pos_x[i] + asteroids_shots_vel_x[i]/2, 
                       asteroids_shots_pos_y[i] + asteroids_shots_vel_y[i]/2, 
                       1);
    }
    
    displayParticles();
    
    display.display();
  }
}
