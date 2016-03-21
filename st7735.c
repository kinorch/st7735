#include "st7735.h"
#include "st7735_ioAccessor.h"
#include "st7735_const.h"

static uint8_t st7735_IsInitialized = 0;
volatile uint8_t st7735_IsCommandMode = 0;

void st7735_Initialize(void)
{
	if(st7735_IsInitialized) return;

	st7735_IoInitilaze();
	st7735_InitializeDeviceR();

	st7735_IsInitialized = 1;
}

void st7735_Finalize(void)
{
	if(!st7735_IsInitialized) return;

	st7735_IoFinalize();

	st7735_IsInitialized = 0;
}

void st7735_InitializeDeviceB(void)
{
	st7735_HardwareReset();
	st7735_SoftwareReset();

	st7735_WaitMicroSeconds(50*1000);
	st7735_WriteCommand(ST7735_SLPOUT);  // out of sleep mode
	st7735_WaitMicroSeconds(120*1000);

	st7735_WriteCommand(ST7735_COLMOD);  // set color mode
	st7735_WriteDataByte(0x06);        // 16-bit color
	st7735_WaitMicroSeconds(10*1000);

	st7735_WriteCommand(ST7735_FRMCTR1);  // frame rate control
	st7735_WriteDataByte(0x00);  // fastest refresh
	st7735_WriteDataByte(0x06);  // 6 lines front porch
	st7735_WriteDataByte(0x03);  // 3 lines backporch
	st7735_WaitMicroSeconds(10*1000);

	st7735_WriteCommand(ST7735_MADCTL);  // memory access control (directions)
	st7735_WriteDataByte(0x08);  // row address/col address, bottom to top refresh

	st7735_WriteCommand(ST7735_DISSET5);  // display settings #5
	st7735_WriteDataByte(0x15);  // 1 clock cycle nonoverlap, 2 cycle gate rise, 3 cycle oscil. equalize
	st7735_WriteDataByte(0x02);  // fix on VTL

	st7735_WriteCommand(ST7735_INVCTR);  // display inversion control
	st7735_WriteDataByte(0x0);  // line inversion

	st7735_WriteCommand(ST7735_PWCTR1);  // power control
	st7735_WriteDataByte(0x02);      // GVDD = 4.7V
	st7735_WriteDataByte(0x70);      // 1.0uA
	st7735_WaitMicroSeconds(10*1000);
	st7735_WriteCommand(ST7735_PWCTR2);  // power control
	st7735_WriteDataByte(0x05);      // VGH = 14.7V, VGL = -7.35V
	st7735_WriteCommand(ST7735_PWCTR3);  // power control
	st7735_WriteDataByte(0x01);      // Opamp current small
	st7735_WriteDataByte(0x02);      // Boost frequency


	st7735_WriteCommand(ST7735_VMCTR1);  // power control
	st7735_WriteDataByte(0x3C);      // VCOMH = 4V
	st7735_WriteDataByte(0x38);      // VCOML = -1.1V
	st7735_WaitMicroSeconds(10*1000);

	st7735_WriteCommand(ST7735_PWCTR6);  // power control
	st7735_WriteDataByte(0x11);
	st7735_WriteDataByte(0x15);

	st7735_WriteCommand(ST7735_GMCTRP1);
	st7735_WriteDataByte(0x0f);	//st7735_WriteDataByte(0x09);
	st7735_WriteDataByte(0x1a);  //st7735_WriteDataByte(0x16);
	st7735_WriteDataByte(0x0f);  //st7735_WriteDataByte(0x09);
	st7735_WriteDataByte(0x18);  //st7735_WriteDataByte(0x20);
	st7735_WriteDataByte(0x2f);  //st7735_WriteDataByte(0x21);
	st7735_WriteDataByte(0x28);  //st7735_WriteDataByte(0x1B);
	st7735_WriteDataByte(0x20);  //st7735_WriteDataByte(0x13);
	st7735_WriteDataByte(0x22);  //st7735_WriteDataByte(0x19);
	st7735_WriteDataByte(0x1f);  //st7735_WriteDataByte(0x17);
	st7735_WriteDataByte(0x1b);  //st7735_WriteDataByte(0x15);
	st7735_WriteDataByte(0x23);  //st7735_WriteDataByte(0x1E);
	st7735_WriteDataByte(0x37);  //st7735_WriteDataByte(0x2B);
	st7735_WriteDataByte(0x00);  //st7735_WriteDataByte(0x04);
	st7735_WriteDataByte(0x07);  //st7735_WriteDataByte(0x05);
	st7735_WriteDataByte(0x02);  //st7735_WriteDataByte(0x02);
	st7735_WriteDataByte(0x10);  //st7735_WriteDataByte(0x0E);
	st7735_WriteCommand(ST7735_GMCTRN1);
	st7735_WriteDataByte(0x0f);   //st7735_WriteDataByte(0x0B);
	st7735_WriteDataByte(0x1b);   //st7735_WriteDataByte(0x14);
	st7735_WriteDataByte(0x0f);   //st7735_WriteDataByte(0x08);
	st7735_WriteDataByte(0x17);   //st7735_WriteDataByte(0x1E);
	st7735_WriteDataByte(0x33);   //st7735_WriteDataByte(0x22);
	st7735_WriteDataByte(0x2c);   //st7735_WriteDataByte(0x1D);
	st7735_WriteDataByte(0x29);   //st7735_WriteDataByte(0x18);
	st7735_WriteDataByte(0x2e);   //st7735_WriteDataByte(0x1E);
	st7735_WriteDataByte(0x30);   //st7735_WriteDataByte(0x1B);
	st7735_WriteDataByte(0x30);   //st7735_WriteDataByte(0x1A);
	st7735_WriteDataByte(0x39);   //st7735_WriteDataByte(0x24);
	st7735_WriteDataByte(0x3f);   //st7735_WriteDataByte(0x2B);
	st7735_WriteDataByte(0x00);   //st7735_WriteDataByte(0x06);
	st7735_WriteDataByte(0x07);   //st7735_WriteDataByte(0x06);
	st7735_WriteDataByte(0x03);   //st7735_WriteDataByte(0x02);
	st7735_WriteDataByte(0x10);   //st7735_WriteDataByte(0x0F);
	st7735_WaitMicroSeconds(10*1000);

	st7735_WriteCommand(ST7735_CASET);  // column addr set
	st7735_WriteDataByte(0x00);
	st7735_WriteDataByte(0x02);   // XSTART = 2
	st7735_WriteDataByte(0x00);
	st7735_WriteDataByte(0x81);   // XEND = 129

	st7735_WriteCommand(ST7735_RASET);  // row addr set
	st7735_WriteDataByte(0x00);
	st7735_WriteDataByte(0x01);    // XSTART = 1
	st7735_WriteDataByte(0x00);
	st7735_WriteDataByte(0xA0);    // XEND = 160

	st7735_WriteCommand(ST7735_NORON);  // normal display on
	st7735_WaitMicroSeconds(10*1000);

	st7735_WriteCommand(ST7735_RAMWR);
	st7735_WaitMicroSeconds(120*1000);

	st7735_DisplayOn();
	st7735_WaitMicroSeconds(120*1000);
}

void st7735_InitializeDeviceR(void)
{
	st7735_HardwareReset();
	st7735_SoftwareReset();

	st7735_WriteCommand(0x11);//Sleep out

	st7735_WaitMicroSeconds(120*1000);

	st7735_WriteCommand(ST7735_FRMCTR1);  // frame rate control - normal mode
	st7735_WriteDataByte(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
	st7735_WriteDataByte(0x2C);
	st7735_WriteDataByte(0x2D);

	st7735_WriteCommand(ST7735_FRMCTR2);  // frame rate control - idle mode
	st7735_WriteDataByte(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
	st7735_WriteDataByte(0x2C);
	st7735_WriteDataByte(0x2D);

	st7735_WriteCommand(ST7735_FRMCTR3);  // frame rate control - partial mode
	st7735_WriteDataByte(0x01); // dot inversion mode
	st7735_WriteDataByte(0x2C);
	st7735_WriteDataByte(0x2D);
	st7735_WriteDataByte(0x01); // line inversion mode
	st7735_WriteDataByte(0x2C);
	st7735_WriteDataByte(0x2D);

	st7735_WriteCommand(ST7735_INVCTR);  // display inversion control
	st7735_WriteDataByte(0x07);  // no inversion

	st7735_WriteCommand(ST7735_PWCTR1);  // power control
	st7735_WriteDataByte(0xA2);
	st7735_WriteDataByte(0x02);      // -4.6V
	st7735_WriteDataByte(0x84);      // AUTO mode

	st7735_WriteCommand(ST7735_PWCTR2);  // power control
	st7735_WriteDataByte(0xC5);      // VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD

	st7735_WriteCommand(ST7735_PWCTR3);  // power control
	st7735_WriteDataByte(0x0A);      // Opamp current small
	st7735_WriteDataByte(0x00);      // Boost frequency

	st7735_WriteCommand(ST7735_PWCTR4);  // power control
	st7735_WriteDataByte(0x8A);      // BCLK/2, Opamp current small & Medium low
	st7735_WriteDataByte(0x2A);

	st7735_WriteCommand(ST7735_PWCTR5);  // power control
	st7735_WriteDataByte(0x8A);
	st7735_WriteDataByte(0xEE);

	st7735_WriteCommand(ST7735_VMCTR1);  // power control
	st7735_WriteDataByte(0x0E);

	st7735_WriteCommand(ST7735_INVOFF);    // don't invert display

	st7735_WriteCommand(ST7735_MADCTL);  // memory access control (directions)
	st7735_WriteDataByte(0xC8);  // row address/col address, bottom to top refresh

	st7735_WriteCommand(ST7735_COLMOD);  // set color mode
	st7735_WriteDataByte(0x05);        // 16-bit color

	st7735_WriteCommand(ST7735_CASET);  // column addr set
	st7735_WriteDataByte(0x00);
	st7735_WriteDataByte(0x00);   // XSTART = 0
	st7735_WriteDataByte(0x00);
	st7735_WriteDataByte(0x7F);   // XEND = 127

	st7735_WriteCommand(ST7735_RASET);  // row addr set
	st7735_WriteDataByte(0x00);
	st7735_WriteDataByte(0x00);    // XSTART = 0
	st7735_WriteDataByte(0x00);
	st7735_WriteDataByte(0x9F);    // XEND = 159


	st7735_WriteCommand(ST7735_GMCTRP1);
	st7735_WriteDataByte(0x0f);
	st7735_WriteDataByte(0x1a);
	st7735_WriteDataByte(0x0f);
	st7735_WriteDataByte(0x18);
	st7735_WriteDataByte(0x2f);
	st7735_WriteDataByte(0x28);
	st7735_WriteDataByte(0x20);
	st7735_WriteDataByte(0x22);
	st7735_WriteDataByte(0x1f);
	st7735_WriteDataByte(0x1b);
	st7735_WriteDataByte(0x23);
	st7735_WriteDataByte(0x37);
	st7735_WriteDataByte(0x00);
	st7735_WriteDataByte(0x07);
	st7735_WriteDataByte(0x02);
	st7735_WriteDataByte(0x10);
	st7735_WriteCommand(ST7735_GMCTRN1);
	st7735_WriteDataByte(0x0f);
	st7735_WriteDataByte(0x1b);
	st7735_WriteDataByte(0x0f);
	st7735_WriteDataByte(0x17);
	st7735_WriteDataByte(0x33);
	st7735_WriteDataByte(0x2c);
	st7735_WriteDataByte(0x29);
	st7735_WriteDataByte(0x2e);
	st7735_WriteDataByte(0x30);
	st7735_WriteDataByte(0x30);
	st7735_WriteDataByte(0x39);
	st7735_WriteDataByte(0x3f);
	st7735_WriteDataByte(0x00);
	st7735_WriteDataByte(0x07);
	st7735_WriteDataByte(0x03);
	st7735_WriteDataByte(0x10);

/*
	st7735_WriteCommand(ST7735_GMCTRP1);
	st7735_WriteDataByte(0x02);
	st7735_WriteDataByte(0x1c);
	st7735_WriteDataByte(0x07);
	st7735_WriteDataByte(0x12);
	st7735_WriteDataByte(0x37);
	st7735_WriteDataByte(0x32);
	st7735_WriteDataByte(0x29);
	st7735_WriteDataByte(0x2d);
	st7735_WriteDataByte(0x29);
	st7735_WriteDataByte(0x25);
	st7735_WriteDataByte(0x2B);
	st7735_WriteDataByte(0x39);
	st7735_WriteDataByte(0x00);
	st7735_WriteDataByte(0x01);
	st7735_WriteDataByte(0x03);
	st7735_WriteDataByte(0x10);
	st7735_WriteCommand(ST7735_GMCTRN1);
	st7735_WriteDataByte(0x03);
	st7735_WriteDataByte(0x1d);
	st7735_WriteDataByte(0x07);
	st7735_WriteDataByte(0x06);
	st7735_WriteDataByte(0x2e);
	st7735_WriteDataByte(0x2c);
	st7735_WriteDataByte(0x29);
	st7735_WriteDataByte(0x2d);
	st7735_WriteDataByte(0x2e);
	st7735_WriteDataByte(0x2e);
	st7735_WriteDataByte(0x37);
	st7735_WriteDataByte(0x3f);
	st7735_WriteDataByte(0x00);
	st7735_WriteDataByte(0x00);
	st7735_WriteDataByte(0x02);
	st7735_WriteDataByte(0x10);
*/

	st7735_SetDisplayInversion(1);
	st7735_WaitMicroSeconds(120*1000);

	st7735_WriteCommand(ST7735_DISPON);

	st7735_WriteCommand(ST7735_NORON);  // normal display on

	st7735_WaitMicroSeconds(120*1000);
}


void st7735_Clear(st7735_Color16bit_t* color)
{
	st7735_SetColumnAddress(0x0002,0x0081);
	st7735_SetRowAddress(0x0001,0x00A0);
	st7735_WriteCommand(0x2C);
	for(volatile uint32_t i=0; i < st7735_LcdHeigth*st7735_LcdWidth; i++)
	{
		st7735_WriteDataBytes( (uint8_t*)(color), 2);
	}
}
