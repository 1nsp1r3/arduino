#include <Arduino_GFX_Library.h>

#define PIN_NUM_LCD_SCLK 39
#define PIN_NUM_LCD_MOSI 38
#define PIN_NUM_LCD_MISO 40
#define PIN_NUM_LCD_DC 42
#define PIN_NUM_LCD_RST -1
#define PIN_NUM_LCD_CS 45
#define PIN_NUM_LCD_BL 1

#define LCD_ROTATION 1
#define LCD_WIDTH  240
#define LCD_HEIGHT 320

//https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class
Arduino_DataBus *bus = new Arduino_ESP32SPI(
  PIN_NUM_LCD_DC,   // DC
  PIN_NUM_LCD_CS,   // CS
  PIN_NUM_LCD_SCLK, // SCK
  PIN_NUM_LCD_MOSI, // MOSI
  PIN_NUM_LCD_MISO  // MISO
);

//https://github.com/moononournation/Arduino_GFX/wiki/Display-Class
Arduino_GFX *gfx = new Arduino_ST7789(
  bus,
  PIN_NUM_LCD_RST, // RST
  LCD_ROTATION,    // rotation
  true,            // IPS
  LCD_WIDTH,       // width
  LCD_HEIGHT       // height
);

//Tentative de double buffer
/*
Arduino_Canvas *canvas = new Arduino_Canvas(LCD_WIDTH, LCD_HEIGHT, gfx, LCD_ROTATION);

void i3_lcdCanvasFlush(){
  canvas->flush();
}
*/

void i3_lcdInit(){
  if (!gfx->begin()) Serial.println("gfx->begin() failed!");

  pinMode(PIN_NUM_LCD_BL, OUTPUT);
  digitalWrite(PIN_NUM_LCD_BL, HIGH);
}

void i3_lcdClear(){
  gfx->fillScreen(BLACK);
}

void i3_lcdStartWrite(){
  gfx->startWrite();
}

void i3_lcdEndWrite(){
  gfx->endWrite();
}


void i3_lcdText(int16_t X, int16_t Y, uint16_t Color, uint8_t Size, const char* Text){
  gfx->setCursor(X, Y);
  gfx->setTextColor(Color); //random(0xffff)
  gfx->setTextSize(
    Size, //x scale
    Size, //y scale
    2     //pixel_margin
  );
  gfx->println(Text);
}

/*
 * Fonctionne qu'avec i3_lcdStartWrite() & i3_lcdEndWrite()
 */
void i3_lcdLine(int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint16_t Color){
   gfx->writeLine(X1, Y1, X2, Y2, Color);
}

