
#include "LCD_characterBitmaps.h"

void CBM_ascii5x8(uint8_t charID, uint8_t *buffer) {

  switch (charID) {

    /* Character 'a'. */

  case ((uint8_t)'a'): {

    // 0x40, 0xa8, 0xa8, 0xf0, 0x00

    buffer[0] = 0x40;
    buffer[1] = 0xA8;
    buffer[2] = 0xA8;
    buffer[3] = 0xF0;
    buffer[4] = 0x00;
  } break;

  case ((uint8_t)'b'): {

    // 0xfe, 0x90, 0x88, 0x88, 0x70

    buffer[0] = 0xFE;
    buffer[1] = 0x90;
    buffer[2] = 0x88;
    buffer[3] = 0x88;
    buffer[4] = 0x70;
  } break;

  case ((uint8_t)'c'): {

    // 0x70, 0x88, 0x88, 0x88, 0x40

    buffer[0] = 0x70;
    buffer[1] = 0x88;
    buffer[2] = 0x88;
    buffer[3] = 0x88;
    buffer[4] = 0x40;
  } break;

  case ((uint8_t)'d'): {

    // 0x70, 0x88, 0x88, 0x90, 0x7e

    buffer[0] = 0x70;
    buffer[1] = 0x88;
    buffer[2] = 0x88;
    buffer[3] = 0x90;
    buffer[4] = 0x7e;
  } break;

  case ((uint8_t)'e'): {

    // 0x70, 0xa8, 0xa8, 0xa8, 0x30

    buffer[0] = 0x70;
    buffer[1] = 0xa8;
    buffer[2] = 0xa8;
    buffer[3] = 0xa0;
    buffer[4] = 0x30;
  } break;

  case ((uint8_t)'f'): {

    // 0x10, 0xfc, 0x12, 0x02, 0x04

    buffer[0] = 0x10;
    buffer[1] = 0xfc;
    buffer[2] = 0x12;
    buffer[3] = 0x02;
    buffer[4] = 0x04;
  } break;

  case ((uint8_t)'g'): {

    // 0x10, 0xa8, 0xa8, 0xa8, 0x78

    buffer[0] = 0x10;
    buffer[1] = 0xa8;
    buffer[2] = 0xa8;
    buffer[3] = 0xa8;
    buffer[4] = 0x78;
  } break;

  case ((uint8_t)'h'): {

    // 0xfe, 0x10, 0x08, 0x08, 0xf0

    buffer[0] = 0xfe;
    buffer[1] = 0x10;
    buffer[2] = 0x08;
    buffer[3] = 0x08;
    buffer[4] = 0xf0;

  } break;

  case ((uint8_t)'i'): {

    // 0x00, 0x90, 0xfa, 0x80, 0x00

    buffer[0] = 0x00;
    buffer[1] = 0x90;
    buffer[2] = 0xfa;
    buffer[3] = 0x80;
    buffer[4] = 0x00;

  } break;

  case ((uint8_t)'j'): {

    // 0x00, 0x40, 0x80, 0x88, 0x7a

    buffer[0] = 0x00;
    buffer[1] = 0x40;
    buffer[2] = 0x80;
    buffer[3] = 0x88;
    buffer[4] = 0x7a;

  } break;

  case ((uint8_t)'k'): {

    // 0xfe, 0x20, 0x50, 0x88, 0x00

    buffer[0] = 0xfe;
    buffer[1] = 0x20;
    buffer[2] = 0x50;
    buffer[3] = 0x88;
    buffer[4] = 0x00;

  } break;

  case ((uint8_t)'l'): {

    // 0x00, 0x82, 0xfe, 0x80, 0x00

    buffer[0] = 0x00;
    buffer[1] = 0x82;
    buffer[2] = 0xfe;
    buffer[3] = 0x80;
    buffer[4] = 0x00;

  } break;

  case ((uint8_t)'m'): {

    // 0xf8, 0x08, 0xf0, 0x08, 0xf0

    buffer[0] = 0xf8;
    buffer[1] = 0x08;
    buffer[2] = 0xf0;
    buffer[3] = 0x08;
    buffer[4] = 0xf0;

  } break;

  case ((uint8_t)'n'): {

    // 0xf8, 0x10, 0x08, 0x08, 0xf0

    buffer[0] = 0xf8;
    buffer[1] = 0x10;
    buffer[2] = 0x08;
    buffer[3] = 0x08;
    buffer[4] = 0xf0;

  } break;

  case ((uint8_t)'o'): {

    // 0x70, 0x88, 0x88, 0x88, 0x70

    buffer[0] = 0x70;
    buffer[1] = 0x88;
    buffer[2] = 0x88;
    buffer[3] = 0x88;
    buffer[4] = 0x70;

  } break;

  case ((uint8_t)'p'): {

    // 0xf8, 0x28, 0x28, 0x28, 0x10

    buffer[0] = 0xf8;
    buffer[1] = 0x28;
    buffer[2] = 0x28;
    buffer[3] = 0x28;
    buffer[4] = 0x10;

  } break;

  case ((uint8_t)'q'): {

    // 0x10, 0x28, 0x28, 0x30, 0xf8

    buffer[0] = 0x10;
    buffer[1] = 0x28;
    buffer[2] = 0x28;
    buffer[3] = 0x30;
    buffer[4] = 0xf8;

  } break;

  case ((uint8_t)'r'): {

    // 0xf8, 0x10, 0x08, 0x08, 0x10

    buffer[0] = 0xf8;
    buffer[1] = 0x10;
    buffer[2] = 0x08;
    buffer[3] = 0x08;
    buffer[4] = 0x10;

  } break;

  case ((uint8_t)'s'): {

    // 0x90, 0xa8, 0xa8, 0xa8, 0x40

    buffer[0] = 0x90;
    buffer[1] = 0xa8;
    buffer[2] = 0xa8;
    buffer[3] = 0xa8;
    buffer[4] = 0x40;

  } break;

  case ((uint8_t)'t'): {

    // 0x08, 0x7e, 0x88, 0x80, 0x40

    buffer[0] = 0x08;
    buffer[1] = 0x7e;
    buffer[2] = 0x88;
    buffer[3] = 0x80;
    buffer[4] = 0x40;

  } break;

  case ((uint8_t)'u'): {

    // 0x78, 0x80, 0x80, 0x40, 0xf8

    buffer[0] = 0x78;
    buffer[1] = 0x80;
    buffer[2] = 0x80;
    buffer[3] = 0x40;
    buffer[4] = 0xf8;

  } break;

  case ((uint8_t)'v'): {

    // 0x38, 0x40, 0x80, 0x40, 0x38

    buffer[0] = 0x38;
    buffer[1] = 0x40;
    buffer[2] = 0x80;
    buffer[3] = 0x40;
    buffer[4] = 0x38;

  } break;

  case ((uint8_t)'w'): {

    // 0x78, 0x80, 0x60, 0x80, 0x78

    buffer[0] = 0x78;
    buffer[1] = 0x80;
    buffer[2] = 0x60;
    buffer[3] = 0x80;
    buffer[4] = 0x78;

  } break;

  case ((uint8_t)'x'): {

    // 0x88, 0x50, 0x20, 0x50, 0x88

    buffer[0] = 0x88;
    buffer[1] = 0x50;
    buffer[2] = 0x20;
    buffer[3] = 0x50;
    buffer[4] = 0x88;

  } break;

  case ((uint8_t)'y'): {

    // 0x18, 0xa0, 0xa0, 0xa0, 0x78

    buffer[0] = 0x18;
    buffer[1] = 0xa0;
    buffer[2] = 0xa0;
    buffer[3] = 0xa0;
    buffer[4] = 0x78;

  } break;

  case ((uint8_t)'z'): {

    // 0x88, 0xc8, 0xa8, 0x98, 0x88

    buffer[0] = 0x88;
    buffer[1] = 0xc8;
    buffer[2] = 0xa8;
    buffer[3] = 0x98;
    buffer[4] = 0x88;

  } break;

  case((uint8_t)'A'):{
    buffer[0]=0xfc;
    buffer[1]=0x22;
    buffer[2]=0x22;
    buffer[3]=0x22;
    buffer[4]=0xfc;
  }break;
  case((uint8_t)'B'):{
    buffer[0]=0xfe;
    buffer[1]=0x92;
    buffer[2]=0x92;
    buffer[3]=0x92;
    buffer[4]=0x6c;
  }break;
  case((uint8_t)'C'):{
    buffer[0]=0x7c;
    buffer[1]=0x82;
    buffer[2]=0x82;
    buffer[3]=0x82;
    buffer[4]=0x44;
  }break;
  case((uint8_t)'D'):{
    buffer[0]=0xfe;
    buffer[1]=0x82;
    buffer[2]=0x82;
    buffer[3]=0x44;
    buffer[4]=0x38;
  }break;
  case((uint8_t)'E'):{
    buffer[0]=0xfe;
    buffer[1]=0x92;
    buffer[2]=0x92;
    buffer[3]=0x92;
    buffer[4]=0x92;
  }break;
  case((uint8_t)'F'):{
    buffer[0]=0xfe;
    buffer[1]=0x12;
    buffer[2]=0x12;
    buffer[3]=0x12;
    buffer[4]=0x02;
  }break;
  case((uint8_t)'G'):{
    buffer[0]=0x7c;
    buffer[1]=0x82;
    buffer[2]=0x92;
    buffer[3]=0x92;
    buffer[4]=0xf4;
  }break;
  case((uint8_t)'H'):{
    buffer[0]=0xfe;
    buffer[1]=0x10;
    buffer[2]=0x10;
    buffer[3]=0x10;
    buffer[4]=0xfe;
  }break;
  case((uint8_t)'I'):{
    buffer[0]=0x00;
    buffer[1]=0x82;
    buffer[2]=0xfe;
    buffer[3]=0x82;
    buffer[4]=0x00;
  }break;
  case((uint8_t)'J'):{
    buffer[0]=0x40;
    buffer[1]=0x80;
    buffer[2]=0x82;
    buffer[3]=0x7e;
    buffer[4]=0x02;
  }break;
  case((uint8_t)'K'):{
    buffer[0]=0xfe;
    buffer[1]=0x10;
    buffer[2]=0x28;
    buffer[3]=0x44;
    buffer[4]=0x82;
  }break;
  case((uint8_t)'L'):{
    buffer[0]=0xfe;
    buffer[1]=0x80;
    buffer[2]=0x80;
    buffer[3]=0x80;
    buffer[4]=0x80;
  }break;
  case((uint8_t)'M'):{
    buffer[0]=0xfe;
    buffer[1]=0x04;
    buffer[2]=0x18;
    buffer[3]=0x04;
    buffer[4]=0xfe;
  }break;
  case((uint8_t)'N'):{
    buffer[0]=0xfe;
    buffer[1]=0x08;
    buffer[2]=0x10;
    buffer[3]=0x20;
    buffer[4]=0xfe;
  }break;
  case((uint8_t)'O'):{
    buffer[0]=0x7c;
    buffer[1]=0x82;
    buffer[2]=0x82;
    buffer[3]=0x82;
    buffer[4]=0x7c;
  }break;
  case((uint8_t)'P'):{
    buffer[0]=0xfe;
    buffer[1]=0x12;
    buffer[2]=0x12;
    buffer[3]=0x12;
    buffer[4]=0x0c;
  }break;
  case((uint8_t)'Q'):{
    buffer[0]=0x7c;
    buffer[1]=0x82;
    buffer[2]=0xa2;
    buffer[3]=0x42;
    buffer[4]=0xbc;
  }break;
  case((uint8_t)'R'):{
    buffer[0]=0xfe;
    buffer[1]=0x12;
    buffer[2]=0x32;
    buffer[3]=0x52;
    buffer[4]=0x8c;
  }break;
  case((uint8_t)'S'):{
    buffer[0]=0x8c;
    buffer[1]=0x92;
    buffer[2]=0x92;
    buffer[3]=0x92;
    buffer[4]=0x62;
  }break;
  case((uint8_t)'T'):{
    buffer[0]=0x02;
    buffer[1]=0x02;
    buffer[2]=0xfe;
    buffer[3]=0x02;
    buffer[4]=0x02;
  }break;
  case((uint8_t)'U'):{
    buffer[0]=0x7e;
    buffer[1]=0x80;
    buffer[2]=0x80;
    buffer[3]=0x80;
    buffer[4]=0x7e;
  }break;
  case((uint8_t)'V'):{
    buffer[0]=0x3e;
    buffer[1]=0x40;
    buffer[2]=0x80;
    buffer[3]=0x40;
    buffer[4]=0x3e;
  }break;
  case((uint8_t)'W'):{
    buffer[0]=0x7e;
    buffer[1]=0x80;
    buffer[2]=0x70;
    buffer[3]=0x80;
    buffer[4]=0x7e;
  }break;
  case((uint8_t)'X'):{
    buffer[0]=0xc6;
    buffer[1]=0x28;
    buffer[2]=0x10;
    buffer[3]=0x28;
    buffer[4]=0xc6;
  }break;
  case((uint8_t)'Y'):{
    buffer[0]=0x0e;
    buffer[1]=0x10;
    buffer[2]=0xe0;
    buffer[3]=0x10;
    buffer[4]=0x0e;
  }break;
  case((uint8_t)'Z'):{
    buffer[0]=0xc2;
    buffer[1]=0xa2;
    buffer[2]=0x92;
    buffer[3]=0x8a;
    buffer[4]=0x86;
  }break;
  case((uint8_t)'0'):{
    buffer[0]=0x7c;
    buffer[1]=0xa2;
    buffer[2]=0x92;
    buffer[3]=0x8a;
    buffer[4]=0x7c;
  }break;
  case((uint8_t)'1'):{
    buffer[0]=0x00;
    buffer[1]=0x84;
    buffer[2]=0xfe;
    buffer[3]=0x80;
    buffer[4]=0x00;
  }break;
  case((uint8_t)'2'):{
    buffer[0]=0x84;
    buffer[1]=0xc2;
    buffer[2]=0xa2;
    buffer[3]=0x92;
    buffer[4]=0x8c;
  }break;
  case((uint8_t)'3'):{
    buffer[0]=0x42;
    buffer[1]=0x82;
    buffer[2]=0x8a;
    buffer[3]=0x96;
    buffer[4]=0x62;
  }break;
  case((uint8_t)'4'):{
    buffer[0]=0x30;
    buffer[1]=0x28;
    buffer[2]=0x24;
    buffer[3]=0xfe;
    buffer[4]=0x20;
  }break;
  case((uint8_t)'5'):{
    buffer[0]=0x4e;
    buffer[1]=0x8a;
    buffer[2]=0x8a;
    buffer[3]=0x8a;
    buffer[4]=0x72;
  }break;
  case((uint8_t)'6'):{
    buffer[0]=0x78;
    buffer[1]=0x94;
    buffer[2]=0x92;
    buffer[3]=0x92;
    buffer[4]=0x60;
  }break;
  case((uint8_t)'7'):{
    buffer[0]=0x06;
    buffer[1]=0x02;
    buffer[2]=0xe2;
    buffer[3]=0x12;
    buffer[4]=0x0e;
  }break;
  case((uint8_t)'8'):{
    buffer[0]=0x6c;
    buffer[1]=0x92;
    buffer[2]=0x92;
    buffer[3]=0x92;
    buffer[4]=0x6c;
  }break;
  case((uint8_t)'9'):{
    buffer[0]=0x0c;
    buffer[1]=0x12;
    buffer[2]=0x92;
    buffer[3]=0x52;
    buffer[4]=0x3c;
  }break;
  case((uint8_t)' '):{
    buffer[0]=0x00;
    buffer[1]=0x00;
    buffer[2]=0x00;
    buffer[3]=0x00;
    buffer[4]=0x00;
  }break;


  case((uint8_t)'!'):{
    buffer[0]=0x00;
    buffer[1]=0x00;
    buffer[2]=0x5e;
    buffer[3]=0x00;
    buffer[4]=0x00;
  }break;
  case((uint8_t)'"'):{
    buffer[0]=0x00;
    buffer[1]=0x0e;
    buffer[2]=0x00;
    buffer[3]=0x0e;
    buffer[4]=0x00;
  }break;
  case((uint8_t)'#'):{
    buffer[0]=0x28;
    buffer[1]=0x7e;
    buffer[2]=0x28;
    buffer[3]=0x7e;
    buffer[4]=0x28;
  }break;
  case((uint8_t)'$'):{
    buffer[0]=0x48;
    buffer[1]=0x54;
    buffer[2]=0xfe;
    buffer[3]=0x54;
    buffer[4]=0x24;
  }break;
  case((uint8_t)'%'):{
    buffer[0]=0x46;
    buffer[1]=0x26;
    buffer[2]=0x10;
    buffer[3]=0xc8;
    buffer[4]=0xc4;
  }break;
  case((uint8_t)'&'):{
    buffer[0]=0x6c;
    buffer[1]=0x92;
    buffer[2]=0xaa;
    buffer[3]=0x44;
    buffer[4]=0xa0;
  }break;
  case((uint8_t)'\''):{
    buffer[0]=0x00;
    buffer[1]=0x00;
    buffer[2]=0x0a;
    buffer[3]=0x06;
    buffer[4]=0x00;
  }break;
  case((uint8_t)'('):{
    buffer[0]=0x00;
    buffer[1]=0x38;
    buffer[2]=0x44;
    buffer[3]=0x82;
    buffer[4]=0x00;
  }break;
  case((uint8_t)')'):{
    buffer[0]=0x00;
    buffer[1]=0x82;
    buffer[2]=0x44;
    buffer[3]=0x38;
    buffer[4]=0x00;
  }break;
  case((uint8_t)'*'):{
    buffer[0]=0x28;
    buffer[1]=0x10;
    buffer[2]=0x7c;
    buffer[3]=0x10;
    buffer[4]=0x28;
  }break;
  case((uint8_t)'+'):{
    buffer[0]=0x10;
    buffer[1]=0x10;
    buffer[2]=0x7c;
    buffer[3]=0x10;
    buffer[4]=0x10;
  }break;
  case((uint8_t)','):{
    buffer[0]=0x00;
    buffer[1]=0x00;
    buffer[2]=0xa0;
    buffer[3]=0x60;
    buffer[4]=0x00;
  }break;
  case((uint8_t)'-'):{
    buffer[0]=0x10;
    buffer[1]=0x10;
    buffer[2]=0x10;
    buffer[3]=0x10;
    buffer[4]=0x10;
  }break;
  case((uint8_t)'.'):{
    buffer[0]=0x00;
    buffer[1]=0x00;
    buffer[2]=0x60;
    buffer[3]=0x60;
    buffer[4]=0x00;
  }break;
  case((uint8_t)'/'):{
    buffer[0]=0x40;
    buffer[1]=0x20;
    buffer[2]=0x10;
    buffer[3]=0x08;
    buffer[4]=0x04;
  }break;
  case((uint8_t)':'):{
    buffer[0]=0x00;
    buffer[1]=0x6c;
    buffer[2]=0x6c;
    buffer[3]=0x00;
    buffer[4]=0x00;
  }break;
  case((uint8_t)';'):{
    buffer[0]=0x00;
    buffer[1]=0xac;
    buffer[2]=0x6c;
    buffer[3]=0x00;
    buffer[4]=0x00;
  }break;
  case((uint8_t)'<'):{
    buffer[0]=0x10;
    buffer[1]=0x28;
    buffer[2]=0x44;
    buffer[3]=0x82;
    buffer[4]=0x00;
  }break;
  case((uint8_t)'='):{
    buffer[0]=0x28;
    buffer[1]=0x28;
    buffer[2]=0x28;
    buffer[3]=0x28;
    buffer[4]=0x28;
  }break;
  case((uint8_t)'>'):{
    buffer[0]=0x00;
    buffer[1]=0x82;
    buffer[2]=0x44;
    buffer[3]=0x28;
    buffer[4]=0x10;
  }break;
  case((uint8_t)'?'):{
    buffer[0]=0x04;
    buffer[1]=0x02;
    buffer[2]=0xa2;
    buffer[3]=0x12;
    buffer[4]=0x0c;
  }break;
  case((uint8_t)'['):{
    buffer[0]=0x00;
    buffer[1]=0x7e;
    buffer[2]=0x42;
    buffer[3]=0x42;
    buffer[4]=0x00;
  }break;
  case((uint8_t)']'):{
    buffer[0]=0x00;
    buffer[1]=0x42;
    buffer[2]=0x42;
    buffer[3]=0x7e;
    buffer[4]=0x00;
  }break;
  case((uint8_t)'^'):{
    buffer[0]=0x00;
    buffer[1]=0x04;
    buffer[2]=0x02;
    buffer[3]=0x04;
    buffer[4]=0x00;
  }break;
  case((uint8_t)'_'):{
    buffer[0]=0x80;
    buffer[1]=0x80;
    buffer[2]=0x80;
    buffer[3]=0x80;
    buffer[4]=0x80;
  }break;
  case((uint8_t)'`'):{
    buffer[0]=0x00;
    buffer[1]=0x02;
    buffer[2]=0x04;
    buffer[3]=0x00;
    buffer[4]=0x00;
  }break;
  case((uint8_t)'{'):{
    buffer[0]=0x00;
    buffer[1]=0x10;
    buffer[2]=0x6c;
    buffer[3]=0x82;
    buffer[4]=0x00;
  }break;
  case((uint8_t)'}'):{
    buffer[0]=0x00;
    buffer[1]=0x82;
    buffer[2]=0x6c;
    buffer[3]=0x10;
    buffer[4]=0x00;
  }break;
  }
}
