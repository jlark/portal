#include <Adafruit_NeoPixel.h>

#define NUM_IMAGE_COLUMNS 10  // Number of columns in a single image (width).

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(67, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(67, 7, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

unsigned int rotationUsec = 1200;  // Number of microseconds for a single rotation.
volatile unsigned int columnIntervalUsec = rotationUsec / NUM_IMAGE_COLUMNS;

unsigned int columnIndex1 = 0;
unsigned int columnIndex2 = NUM_IMAGE_COLUMNS / 2;

bool image[NUM_IMAGE_COLUMNS][67] = {
 { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,  true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,  true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true },
 { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,  true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,  true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true },
 { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,  true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,  true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true },
 { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,  true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,  true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true },
 { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,  true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,  true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true },
 { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
 { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
 { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
 { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
 { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
};

void setup() {
  // Initialize all pixels to 'off'
  strip1.begin();
  strip1.show();
  strip2.begin();
  strip2.show();
}

void interrupt() {
  // Recompute the column interval time.
  // TODO: You have to calculate rotationUSec based on prev interrupt and this one.
  columnIntervalUsec = rotationUsec / NUM_IMAGE_COLUMNS;
}

unsigned int nextColumn(unsigned int x) {
  return (x + 1) % NUM_IMAGE_COLUMNS;
}

void drawColumn(Adafruit_NeoPixel* strip, const unsigned int columnIndex) {
  for (int ii = 0; ii < strip1.numPixels(); ++ii) {
    strip->setPixelColor(ii, (image[columnIndex][ii] ? strip->Color(255, 255, 255) : strip->Color(0, 0, 0)));
  }
}

unsigned int prevTime = micros();
unsigned int lastDrawTime = 0;

void loop()
{
   unsigned int currentTime = micros();
 
  // This is simulating interrupts.
  // TODO(cinar): Replace with actual interrupt function.
  if (currentTime % rotationUsec == 0) {
    interrupt();
  }
  
  // Actual loop() code begins here.
  if (lastDrawTime == 0 || currentTime - lastDrawTime >= columnIntervalUsec) {
    drawColumn(&strip1, columnIndex1);
    drawColumn(&strip2, columnIndex2);
    columnIndex1 = nextColumn(columnIndex1);
    columnIndex2 = nextColumn(columnIndex2);
    lastDrawTime = currentTime;
  }
}
