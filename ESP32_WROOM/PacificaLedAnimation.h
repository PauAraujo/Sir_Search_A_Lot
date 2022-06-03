//  "Pacifica"
//  Gentle, blue-green ocean waves.
//  December 2019, Mark Kriegsman and Mary Corey March.
//  For Dan.
//  https://github.com/FastLED/FastLED/blob/master/examples/Pacifica/Pacifica.ino

// 0 = blue
// 1 = orange
// 2 = red
int activeColorPallate;
void setColorPallate(int color){
  if(color == activeColorPallate){
    return;
  }

  if(color == 0){
    for( uint16_t i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Blue;
    }
  }if(color == 1){
    for( uint16_t i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Orange;
    }
  }else if(color == 2){
    for( uint16_t i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Red;
    }
  }
  
  activeColorPallate = color;
}

CRGBPalette16 getColorWave(int wave){
  const CRGBPalette16 blue_wave[3] = {
    {0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50},
    {0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F},
    {0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF}
  };
  const CRGBPalette16 orange_wave[3] = {
    {0x070400, 0x090400, 0x0B0400, 0x0D0500, 0x100700, 0x120900, 0x140900, 0x170A00, 0x190A00, 0x1C0E00, 0x260F00, 0x311800, 0x3B2000, 0x462300, 0x553114, 0xAA6328},
    {0x070400, 0x090400, 0x0B0400, 0x0D0500, 0x100700, 0x120900, 0x140900, 0x170A00, 0x190A00, 0x1C0E00, 0x260F00, 0x311800, 0x3B2000, 0x462300, 0x5F520C, 0xBEB419},
    {0xBE7425, 0x0E0800, 0x140C00, 0x1A0F00, 0x201000, 0x271200, 0x2D1000, 0x331F00, 0x392100, 0x402500, 0x503500, 0x603700, 0x704000, 0x805100, 0xBF7F10, 0xFFA720}
  };
  const CRGBPalette16 red_wave[3] = {
    {0x070000, 0x090000, 0x0B0000, 0x0D0000, 0x100000, 0x120000, 0x140000, 0x170000, 0x190000, 0x1C0000, 0x260000, 0x310000, 0x3B0000, 0x460000, 0x552E14, 0xAA5228},
    {0x070000, 0x090000, 0x0B0000, 0x0D0000, 0x100000, 0x120000, 0x140000, 0x170000, 0x190000, 0x1C0000, 0x260000, 0x310000, 0x3B0000, 0x460000, 0x5F0C0C, 0xBE1919},
    {0xBE2525, 0x0E0000, 0x140000, 0x1A0000, 0x200000, 0x270000, 0x2D0000, 0x330000, 0x390000, 0x400000, 0x500000, 0x600000, 0x700000, 0x800000, 0xBF1010, 0xFF2020}
  };

  if(activeColorPallate == 1){
    return orange_wave[wave];
  }else if(activeColorPallate == 2){
    return red_wave[wave];
  }
   
  return blue_wave[wave];
}

// Add one layer of waves into the led array
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

void runPacifica()
{
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));

  // Clear out the LED array to a dim background
  if(activeColorPallate == 0){
    fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));
  }else if(activeColorPallate == 1){
    fill_solid( leds, NUM_LEDS, CRGB( 10, 7, 2));
  }else if(activeColorPallate == 2){
    fill_solid( leds, NUM_LEDS, CRGB( 10, 2, 2));
  }
  
  // Render each layer, with different scales and speeds, that vary over time
  CRGBPalette16 wave1 = getColorWave(0);
  pacifica_one_layer( wave1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301));
  CRGBPalette16 wave2 = getColorWave(1);
  pacifica_one_layer( wave2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401));
  CRGBPalette16 wave3 = getColorWave(2);
  pacifica_one_layer( wave3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer( wave3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));
  
  FastLED.show();
}
