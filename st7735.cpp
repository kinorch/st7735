#include <unistd.h>
#include <lpc11xx_syscon.h>
#include <lpc11xx_ssp.h>
#include <lpc11xx_gpio.h>

#include "st7735.hpp"

st7735::st7735()
{
	m_IsInitialized = false;

}

st7735::~st7735()
{
	Finalize();
}

void st7735::Initialize(void)
{
	if(m_IsInitialized) return;

	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_GPIO, ENABLE);
	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_SSP1, ENABLE);

	SSP_CFG_Type sspConfig;

	SSP_ConfigStructInit(&sspConfig);
	SSP_Init(LPC_SSP0, &sspConfig);

	GPIO_SetDir(PORT2, GPIO_Pin_9, 1);
	GPIO_SetDir(PORT2, GPIO_Pin_10, 1);
	GPIO_SetDir(PORT2, GPIO_Pin_2, 1);

	m_IsInitialized = true;

	InitializeDeviceR();
}

void st7735::Finalize()
{
	if(!m_IsInitialized) return;

	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_GPIO, ENABLE);
	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_SSP1, ENABLE);

	m_IsInitialized = false;
}

void st7735::InitializeDeviceB()
{
	HardwareReset();
	SoftwareReset();

	usleep(50*1000);
	WriteCommand(ST7735_SLPOUT);  // out of sleep mode
	usleep(120*1000);

	WriteCommand(ST7735_COLMOD);  // set color mode
	WriteData(0x06);        // 16-bit color
	usleep(10*1000);

	WriteCommand(ST7735_FRMCTR1);  // frame rate control
	WriteData(0x00);  // fastest refresh
	WriteData(0x06);  // 6 lines front porch
	WriteData(0x03);  // 3 lines backporch
	usleep(10*1000);

	WriteCommand(ST7735_MADCTL);  // memory access control (directions)
	WriteData(0x08);  // row address/col address, bottom to top refresh

	WriteCommand(ST7735_DISSET5);  // display settings #5
	WriteData(0x15);  // 1 clock cycle nonoverlap, 2 cycle gate rise, 3 cycle oscil. equalize
	WriteData(0x02);  // fix on VTL

	WriteCommand(ST7735_INVCTR);  // display inversion control
	WriteData(0x0);  // line inversion

	WriteCommand(ST7735_PWCTR1);  // power control
	WriteData(0x02);      // GVDD = 4.7V
	WriteData(0x70);      // 1.0uA
	usleep(10*1000);
	WriteCommand(ST7735_PWCTR2);  // power control
	WriteData(0x05);      // VGH = 14.7V, VGL = -7.35V
	WriteCommand(ST7735_PWCTR3);  // power control
	WriteData(0x01);      // Opamp current small
	WriteData(0x02);      // Boost frequency


	WriteCommand(ST7735_VMCTR1);  // power control
	WriteData(0x3C);      // VCOMH = 4V
	WriteData(0x38);      // VCOML = -1.1V
	usleep(10*1000);

	WriteCommand(ST7735_PWCTR6);  // power control
	WriteData(0x11);
	WriteData(0x15);

	WriteCommand(ST7735_GMCTRP1);
	WriteData(0x0f);	//WriteData(0x09);
	WriteData(0x1a);  //WriteData(0x16);
	WriteData(0x0f);  //WriteData(0x09);
	WriteData(0x18);  //WriteData(0x20);
	WriteData(0x2f);  //WriteData(0x21);
	WriteData(0x28);  //WriteData(0x1B);
	WriteData(0x20);  //WriteData(0x13);
	WriteData(0x22);  //WriteData(0x19);
	WriteData(0x1f);  //WriteData(0x17);
	WriteData(0x1b);  //WriteData(0x15);
	WriteData(0x23);  //WriteData(0x1E);
	WriteData(0x37);  //WriteData(0x2B);
	WriteData(0x00);  //WriteData(0x04);
	WriteData(0x07);  //WriteData(0x05);
	WriteData(0x02);  //WriteData(0x02);
	WriteData(0x10);  //WriteData(0x0E);
	WriteCommand(ST7735_GMCTRN1);
	WriteData(0x0f);   //WriteData(0x0B);
	WriteData(0x1b);   //WriteData(0x14);
	WriteData(0x0f);   //WriteData(0x08);
	WriteData(0x17);   //WriteData(0x1E);
	WriteData(0x33);   //WriteData(0x22);
	WriteData(0x2c);   //WriteData(0x1D);
	WriteData(0x29);   //WriteData(0x18);
	WriteData(0x2e);   //WriteData(0x1E);
	WriteData(0x30);   //WriteData(0x1B);
	WriteData(0x30);   //WriteData(0x1A);
	WriteData(0x39);   //WriteData(0x24);
	WriteData(0x3f);   //WriteData(0x2B);
	WriteData(0x00);   //WriteData(0x06);
	WriteData(0x07);   //WriteData(0x06);
	WriteData(0x03);   //WriteData(0x02);
	WriteData(0x10);   //WriteData(0x0F);
	usleep(10*1000);

	WriteCommand(ST7735_CASET);  // column addr set
	WriteData(0x00);
	WriteData(0x02);   // XSTART = 2
	WriteData(0x00);
	WriteData(0x81);   // XEND = 129

	WriteCommand(ST7735_RASET);  // row addr set
	WriteData(0x00);
	WriteData(0x01);    // XSTART = 1
	WriteData(0x00);
	WriteData(0xA0);    // XEND = 160

	WriteCommand(ST7735_NORON);  // normal display on
	usleep(10*1000);

	WriteCommand(ST7735_RAMWR);
	usleep(120*1000);

	DisplayOn();
	usleep(120*1000);
}

void st7735::InitializeDeviceR()
{
	HardwareReset();
	SoftwareReset();

	WriteCommand(0x11);//Sleep out

	usleep(120*1000);

	WriteCommand(ST7735_FRMCTR1);  // frame rate control - normal mode
	WriteData(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
	WriteData(0x2C);
	WriteData(0x2D);

	WriteCommand(ST7735_FRMCTR2);  // frame rate control - idle mode
	WriteData(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
	WriteData(0x2C);
	WriteData(0x2D);

	WriteCommand(ST7735_FRMCTR3);  // frame rate control - partial mode
	WriteData(0x01); // dot inversion mode
	WriteData(0x2C);
	WriteData(0x2D);
	WriteData(0x01); // line inversion mode
	WriteData(0x2C);
	WriteData(0x2D);

	WriteCommand(ST7735_INVCTR);  // display inversion control
	WriteData(0x07);  // no inversion

	WriteCommand(ST7735_PWCTR1);  // power control
	WriteData(0xA2);
	WriteData(0x02);      // -4.6V
	WriteData(0x84);      // AUTO mode

	WriteCommand(ST7735_PWCTR2);  // power control
	WriteData(0xC5);      // VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD

	WriteCommand(ST7735_PWCTR3);  // power control
	WriteData(0x0A);      // Opamp current small
	WriteData(0x00);      // Boost frequency

	WriteCommand(ST7735_PWCTR4);  // power control
	WriteData(0x8A);      // BCLK/2, Opamp current small & Medium low
	WriteData(0x2A);

	WriteCommand(ST7735_PWCTR5);  // power control
	WriteData(0x8A);
	WriteData(0xEE);

	WriteCommand(ST7735_VMCTR1);  // power control
	WriteData(0x0E);

	WriteCommand(ST7735_INVOFF);    // don't invert display

	WriteCommand(ST7735_MADCTL);  // memory access control (directions)
	WriteData(0xC8);  // row address/col address, bottom to top refresh

	WriteCommand(ST7735_COLMOD);  // set color mode
	WriteData(0x06);        // 16-bit color

	WriteCommand(ST7735_CASET);  // column addr set
	WriteData(0x00);
	WriteData(0x00);   // XSTART = 0
	WriteData(0x00);
	WriteData(0x7F);   // XEND = 127

	WriteCommand(ST7735_RASET);  // row addr set
	WriteData(0x00);
	WriteData(0x00);    // XSTART = 0
	WriteData(0x00);
	WriteData(0x9F);    // XEND = 159


	WriteCommand(ST7735_GMCTRP1);
	WriteData(0x0f);
	WriteData(0x1a);
	WriteData(0x0f);
	WriteData(0x18);
	WriteData(0x2f);
	WriteData(0x28);
	WriteData(0x20);
	WriteData(0x22);
	WriteData(0x1f);
	WriteData(0x1b);
	WriteData(0x23);
	WriteData(0x37);
	WriteData(0x00);
	WriteData(0x07);
	WriteData(0x02);
	WriteData(0x10);
	WriteCommand(ST7735_GMCTRN1);
	WriteData(0x0f);
	WriteData(0x1b);
	WriteData(0x0f);
	WriteData(0x17);
	WriteData(0x33);
	WriteData(0x2c);
	WriteData(0x29);
	WriteData(0x2e);
	WriteData(0x30);
	WriteData(0x30);
	WriteData(0x39);
	WriteData(0x3f);
	WriteData(0x00);
	WriteData(0x07);
	WriteData(0x03);
	WriteData(0x10);

/*
	WriteCommand(ST7735_GMCTRP1);
	WriteData(0x02);
	WriteData(0x1c);
	WriteData(0x07);
	WriteData(0x12);
	WriteData(0x37);
	WriteData(0x32);
	WriteData(0x29);
	WriteData(0x2d);
	WriteData(0x29);
	WriteData(0x25);
	WriteData(0x2B);
	WriteData(0x39);
	WriteData(0x00);
	WriteData(0x01);
	WriteData(0x03);
	WriteData(0x10);
	WriteCommand(ST7735_GMCTRN1);
	WriteData(0x03);
	WriteData(0x1d);
	WriteData(0x07);
	WriteData(0x06);
	WriteData(0x2e);
	WriteData(0x2c);
	WriteData(0x29);
	WriteData(0x2d);
	WriteData(0x2e);
	WriteData(0x2e);
	WriteData(0x37);
	WriteData(0x3f);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x02);
	WriteData(0x10);
*/

	SetDisplayInversion(true);
	usleep(120*1000);

	WriteCommand(ST7735_DISPON);

	WriteCommand(ST7735_NORON);  // normal display on

	usleep(120*1000);
}


void st7735::Clear(color18bit_t* color)
{
	SetColumnAddress(0x0002,0x0081);
	SetRowAddress(0x0001,0x00A0);
	WriteCommand(0x2C);
	for(volatile int i=0; i < LCD_HEIGHT*LCD_WIDTH; i++)
	{
		WriteData( reinterpret_cast<uint8_t*>(color), 3);
	}
}
