/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

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

// FONTS
// small digits

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
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b01000000, 0b00010011, 0b11110011, 0b00110011, 0b11110000,
  0b00001111, 0b11000110, 0b00010011, 0b11110011, 0b00110011, 0b11110000, 0b00001100, 0b00001111, 0b00110011, 0b00110011, 0b01100011, 0b00000000,
  0b00001100, 0b00001111, 0b00110010, 0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b11001101, 0b10110000, 0b11111111, 0b11111111, 0b01000000,
  0b00001111, 0b11001101, 0b10110011, 0b11111111, 0b11111111, 0b11000000, 0b00000000, 0b11001100, 0b11110000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b11001100, 0b11110011, 0b00110011, 0b01100011, 0b00000000, 0b00000011, 0b11000100, 0b01100011, 0b00110011, 0b00110011, 0b11110000,
  0b00001111, 0b11001100, 0b00000011, 0b00110011, 0b00110011, 0b11110000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b00000010, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b01000000, 0b00000001, 0b11110000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b10011111, 0b11111111, 0b00000000,
  0b00000000, 0b00100000, 0b00001100, 0b01111111, 0b11111111, 0b10000000, 0b00000100, 0b00000001, 0b01001100, 0b01111110, 0b00000000, 0b11000000,
  0b00000001, 0b00001000, 0b00011100, 0b01110000, 0b00000000, 0b01000000, 0b00000010, 0b00100001, 0b00111111, 0b11100111, 0b00000001, 0b00000000,
  0b00010101, 0b01111100, 0b01111111, 0b11000011, 0b00000000, 0b11000000, 0b00000111, 0b00000001, 0b11111111, 0b00000011, 0b00000000, 0b11000000,
  0b00010000, 0b01111111, 0b10111110, 0b00000010, 0b00000000, 0b10000000, 0b00000111, 0b11110111, 0b01111000, 0b10000100, 0b00000001, 0b00000000,
  0b00011111, 0b10101010, 0b11110001, 0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b11011101, 0b11000001, 0b00000000, 0b00000000, 0b00000000,
  0b00010110, 0b10111010, 0b11100010, 0b00000000, 0b00000000, 0b00000000, 0b00011011, 0b01110111, 0b01111001, 0b00000000, 0b00010000, 0b00000000,
  0b00011101, 0b10111010, 0b10111100, 0b10000000, 0b00000000, 0b00000000, 0b00001110, 0b11011101, 0b11011110, 0b10000000, 0b00100000, 0b00000000,
  0b00010101, 0b11111010, 0b11111110, 0b00000000, 0b00100000, 0b00000000, 0b00011011, 0b01110111, 0b01110111, 0b00000000, 0b00000000, 0b00000000,
  0b00010100, 0b10111111, 0b11111011, 0b00000000, 0b00000000, 0b00000000, 0b00001101, 0b01011100, 0b00001111, 0b10000000, 0b00010000, 0b00000000,
  0b00011011, 0b11100000, 0b11100001, 0b11000000, 0b00000000, 0b00010000, 0b00011111, 0b00001110, 0b10100000, 0b11110000, 0b00001000, 0b00000000,
  0b00011100, 0b00110101, 0b01000000, 0b01111100, 0b00000100, 0b00000000, 0b00010000, 0b11001010, 0b00000000, 0b00001111, 0b11111000, 0b00000000,
  0b00000110, 0b00000100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010100, 0b01000000, 0b00100001, 0b00000000, 0b00000000, 0b00000000,
  0b00001010, 0b00010010, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000100, 0b00000000, 0b00000000, 0b00000000, 0b00001000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000
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

// ---- START CODE ---- //

void setup() {
  // Start serial
  Serial.begin(9600);


  // Setup input pins
  pinMode(INPUT_U, INPUT_PULLUP);
  pinMode(INPUT_D, INPUT_PULLUP);
  pinMode(INPUT_L, INPUT_PULLUP);
  pinMode(INPUT_R, INPUT_PULLUP);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

  // Override menu
  
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

static char front_indices[] = {
  3, 2, // F
  0, 2, // F
  3, 1, // F
  0, 1  // F
};

static char right_indices[] = {
  0, 4,
  1, 5,
  4, 5
};

static char left_indices[] = {
  2, 6,
  3, 7,
  6, 7
};

static char top_indices[] = {
  0, 4,
  2, 6,
  4, 6
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
    world_coord wCoord0 = collateVertData(verts, front_indices, i*2 + 0);
    world_coord wCoord1 = collateVertData(verts, front_indices, i*2 + 1);
        
    screen_coord sCoord0 = mdlToScreen(wCoord0);
    screen_coord sCoord1 = mdlToScreen(wCoord1);
    
    display.drawLine(sCoord0.x, sCoord0.y, sCoord1.x, sCoord1.y, SSD1306_WHITE);
  }

  // Draw game thumbnail
  display.drawBitmap(12, 8, snake_thumb, 48, 48, 1);
  
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
    menuRFlip(menu_accspeed);
    firstframe = true;
  }
  else if (!digitalRead(INPUT_L)) {
    menuLFlip(menu_accspeed);
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

void menuRFlip(float speed) {
  float theta = 0;
  float t_val = 0;
  for (int d = 0; d < int(10/speed); d++) {
    t_val = lerp(t_val, 1, 0.3);
    theta = lerp(0, -DEG90RAD, t_val);
    
    float sinTheta = sin(theta);
    float cosTheta = cos(theta);

    display.clearDisplay();

    // Draw front face
    for (int i = 0; i < 4; i++) {
      world_coord wCoord0 = collateVertData(verts, front_indices, i*2 + 0);
      world_coord wCoord1 = collateVertData(verts, front_indices, i*2 + 1);
      world_coord wCT0;
      world_coord wCT1;
  
      wCT0.x = wCoord0.x*cosTheta - wCoord0.z*sinTheta;
      wCT0.y = wCoord0.y;
      wCT0.z = wCoord0.x*sinTheta + wCoord0.z*cosTheta;
  
      wCT1.x = wCoord1.x*cosTheta - wCoord1.z*sinTheta;
      wCT1.y = wCoord1.y;
      wCT1.z = wCoord1.x*sinTheta + wCoord1.z*cosTheta;
  
      screen_coord sCoord0 = mdlToScreen(wCT0);
      screen_coord sCoord1 = mdlToScreen(wCT1);

      // cursed technique: culling
      if (i == 1) {
        if (sCoord0.x > sCoord1.x)
          break;
      }
      
      display.drawLine(sCoord0.x, sCoord0.y, sCoord1.x, sCoord1.y, SSD1306_WHITE);
    }

    // Draw left face
    for (int i = 0; i < 3; i++) {
      world_coord wCoord0 = collateVertData(verts, left_indices, i*2 + 0);
      world_coord wCoord1 = collateVertData(verts, left_indices, i*2 + 1);
      world_coord wCT0;
      world_coord wCT1;
  
      wCT0.x = wCoord0.x*cosTheta - wCoord0.z*sinTheta;
      wCT0.y = wCoord0.y;
      wCT0.z = wCoord0.x*sinTheta + wCoord0.z*cosTheta;
  
      wCT1.x = wCoord1.x*cosTheta - wCoord1.z*sinTheta;
      wCT1.y = wCoord1.y;
      wCT1.z = wCoord1.x*sinTheta + wCoord1.z*cosTheta;
  
      screen_coord sCoord0 = mdlToScreen(wCT0);
      screen_coord sCoord1 = mdlToScreen(wCT1);
      
      display.drawLine(sCoord0.x, sCoord0.y, sCoord1.x, sCoord1.y, SSD1306_WHITE);
    }

    // Draw game thumbnails
    int x_offset0 = lerp(12, -48, t_val);
    display.drawBitmap(x_offset0, 8, snake_thumb, 48, 48, 1);
    int x_offset1 = lerp(128, 12, t_val);
    display.drawBitmap(x_offset1, 8, snake_thumb, 48, 48, 1);
    
    display.display();

    delay(5);
  }
}

void menuLFlip(float speed) {
  float theta = 0;
  float t_val = 0;
  for (int d = 0; d < int(10/speed); d++) {
    t_val = lerp(t_val, 1, 0.3);
    theta = lerp(0, DEG90RAD, t_val);
    
    float sinTheta = sin(theta);
    float cosTheta = cos(theta);

    display.clearDisplay();

    // Draw front face
    for (int i = 0; i < 4; i++) {
      world_coord wCoord0 = collateVertData(verts, front_indices, i*2 + 0);
      world_coord wCoord1 = collateVertData(verts, front_indices, i*2 + 1);
      world_coord wCT0;
      world_coord wCT1;
  
      wCT0.x = wCoord0.x*cosTheta - wCoord0.z*sinTheta;
      wCT0.y = wCoord0.y;
      wCT0.z = wCoord0.x*sinTheta + wCoord0.z*cosTheta;
  
      wCT1.x = wCoord1.x*cosTheta - wCoord1.z*sinTheta;
      wCT1.y = wCoord1.y;
      wCT1.z = wCoord1.x*sinTheta + wCoord1.z*cosTheta;
  
      screen_coord sCoord0 = mdlToScreen(wCT0);
      screen_coord sCoord1 = mdlToScreen(wCT1);
      
      display.drawLine(sCoord0.x, sCoord0.y, sCoord1.x, sCoord1.y, SSD1306_WHITE);
    }

    // Draw right face
    for (int i = 0; i < 3; i++) {
      world_coord wCoord0 = collateVertData(verts, right_indices, i*2 + 0);
      world_coord wCoord1 = collateVertData(verts, right_indices, i*2 + 1);
      world_coord wCT0;
      world_coord wCT1;
  
      wCT0.x = wCoord0.x*cosTheta - wCoord0.z*sinTheta;
      wCT0.y = wCoord0.y;
      wCT0.z = wCoord0.x*sinTheta + wCoord0.z*cosTheta;
  
      wCT1.x = wCoord1.x*cosTheta - wCoord1.z*sinTheta;
      wCT1.y = wCoord1.y;
      wCT1.z = wCoord1.x*sinTheta + wCoord1.z*cosTheta;
  
      screen_coord sCoord0 = mdlToScreen(wCT0);
      screen_coord sCoord1 = mdlToScreen(wCT1);
      
      display.drawLine(sCoord0.x, sCoord0.y, sCoord1.x, sCoord1.y, SSD1306_WHITE);
    }

    // Draw game thumbnail
    int x_offset0 = lerp(12, 128, t_val);
    display.drawBitmap(x_offset0, 8, snake_thumb, 48, 48, 1);
    int x_offset1 = lerp(-48, 12, t_val);
    display.drawBitmap(x_offset1, 8, snake_thumb, 48, 48, 1);

    display.display();

    delay(5);
  }
}

void menuUFlip() {
  float theta = 0;
  float t_val = 0;
  for (int d = 0; d < 10; d++) {
    t_val = lerp(t_val, 1, 0.3);
    theta = lerp(0, DEG90RAD, t_val);

    // animate box towards center of screen
    model_translate.x = lerp(-1, 0, t_val);
    
    float sinTheta = sin(theta);
    float cosTheta = cos(theta);

    display.clearDisplay();

    // Draw front face
    for (int i = 0; i < 4; i++) {
      world_coord wCoord0 = collateVertData(verts, front_indices, i*2 + 0);
      world_coord wCoord1 = collateVertData(verts, front_indices, i*2 + 1);
      world_coord wCT0;
      world_coord wCT1;
      
      wCT0.x = wCoord0.x;
      wCT0.y = wCoord0.y*cosTheta - wCoord0.z*sinTheta;
      wCT0.z = wCoord0.y*sinTheta + wCoord0.z*cosTheta;

      wCT1.x = wCoord1.x;
      wCT1.y = wCoord1.y*cosTheta - wCoord1.z*sinTheta;
      wCT1.z = wCoord1.y*sinTheta + wCoord1.z*cosTheta;
  
      screen_coord sCoord0 = mdlToScreen(wCT0);
      screen_coord sCoord1 = mdlToScreen(wCT1);

      // cursed technique: culling
      if (i == 1) {
        if (sCoord0.y < sCoord1.y)
          break;
      }
      
      display.drawLine(sCoord0.x, sCoord0.y, sCoord1.x, sCoord1.y, SSD1306_WHITE);
    }

    // Draw top face
    for (int i = 0; i < 3; i++) {
      world_coord wCoord0 = collateVertData(verts, top_indices, i*2 + 0);
      world_coord wCoord1 = collateVertData(verts, top_indices, i*2 + 1);
      world_coord wCT0;
      world_coord wCT1;
  
      wCT0.x = wCoord0.x;
      wCT0.y = wCoord0.y*cosTheta - wCoord0.z*sinTheta;
      wCT0.z = wCoord0.y*sinTheta + wCoord0.z*cosTheta;

      wCT1.x = wCoord1.x;
      wCT1.y = wCoord1.y*cosTheta - wCoord1.z*sinTheta;
      wCT1.z = wCoord1.y*sinTheta + wCoord1.z*cosTheta;
  
      screen_coord sCoord0 = mdlToScreen(wCT0);
      screen_coord sCoord1 = mdlToScreen(wCT1);
      
      display.drawLine(sCoord0.x, sCoord0.y, sCoord1.x, sCoord1.y, SSD1306_WHITE);
    }

    // Draw game thumbnail
    int y_offset0 = lerp(8, 64, t_val);
    display.drawBitmap(12, y_offset0, snake_thumb, 48, 48, 1);

    display.display();

    delay(5);
  }

  // Cube zoom
  for (int i = 0; i < 10; i++) {
    model_translate.z = lerp(8, 0, (float)i/10.0);

    display.clearDisplay();
    // Draw front face of cube
    for (int i = 0; i < 4; i++) {
      world_coord wCoord0 = collateVertData(verts, front_indices, i*2 + 0);
      world_coord wCoord1 = collateVertData(verts, front_indices, i*2 + 1);
          
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

int snake_game_delay = 10;
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
  snake_game_delay = 10;
  snake_game_delayctr = 0;
  snake_game_diffgate = 5;
  snake_game_diffctr = 0;
  snake_apple_x = 20;
  snake_apple_y = 4;
  snake_score = 0;

  // setup particle vars
  particle_gravity = 1;
  particle_lifetime = 30; // seconds
  
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
        snake_SpeedUp();
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
      if (i >= PARTICLE_COUNT) return;
      particle_alive[i] = true;
      particle_lifectr[i] = 0;
      particle_coords_x[i] = snake_segments_x[0]*4 + 2;
      particle_coords_y[i] = snake_segments_y[0]*4 + 2;
      particle_vel_x[i] = (random(-99, 99) / 100.0f)*5;
      particle_vel_y[i] = (random(-99, 99) / 100.0f)*5;
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
    for (int i = 0; i < 20; i++) {
      if (i >= PARTICLE_COUNT) return;
      particle_alive[i] = true;
      particle_lifectr[i] = 0;
      particle_coords_x[i] = snake_segments_x[z]*4 + 2;
      particle_coords_y[i] = snake_segments_y[z]*4 + 2;
      particle_vel_x[i] = (random(-99, 99) / 100.0f)*5;
      particle_vel_y[i] = (random(-99, 99) / 100.0f)*5;
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
