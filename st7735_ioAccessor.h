#pragma once

#include <lpc11xx_syscon.h>
#include <lpc11xx_ssp.h>
#include <lpc11xx_gpio.h>

#define RESET_PORT	PORT2
#define RESET_PIN	GPIO_Pin_9
#define CS_PORT		PORT2
#define CS_PIN		GPIO_Pin_10
#define RS_PORT		PORT2
#define RS_PIN		GPIO_Pin_2

extern volatile uint8_t st7735_IsCommandMode;

static inline void st7735_IoInitilaze(void)
{
	// Initialize SPI
	SYSCON_PeriphResetCmd(SYSCON_RSTPeriph_SSP0, DISABLE);
	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_SSP1, ENABLE);
	SYSCON_SetSPI0ClockDiv(1);

	SSP_SSP0PinsInit(SCK0_PIO2_11,DISABLE);

	SSP_CFG_Type sspConfig;
	SSP_ConfigStructInit(&sspConfig);
	SSP_Init(LPC_SSP0, &sspConfig);

	SSP_SetClock(LPC_SSP0, 30*1000*1000);

	SSP_Cmd(LPC_SSP0,ENABLE);

	// Initialize SSEL
	IOCON_SetPinFunc(IOCON_PIO0_2, PIO0_2_FUN_SSEL0);

	// Initialize GPIO
	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_GPIO, ENABLE);

	GPIO_SetDir(RESET_PORT, RESET_PIN, 1);
	GPIO_SetDir(CS_PORT, 	CS_PIN, 1);
	GPIO_SetDir(RS_PORT, 	RS_PIN, 1);
}

static inline void st7735_IoFinalize(void)
{
	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_GPIO, DISABLE);
	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_SSP1, DISABLE);
	SSP_Cmd(LPC_SSP0,ENABLE);
	SYSCON_PeriphResetCmd(SYSCON_RSTPeriph_SSP0, ENABLE);
}

static inline void st7735_EnableChipSelect(void)
{
	//GPIO_ResetBits(CS_PORT, CS_PIN);
}

static inline void st7735_DisableChipSelect(void)
{
	//GPIO_SetBits(CS_PORT, CS_PIN);
}

static inline void st7735_EnableReset(void)
{
	GPIO_ResetBits(RESET_PORT, RESET_PIN);
}

static inline void st7735_DisableReset(void)
{
	GPIO_SetBits(RESET_PORT, RESET_PIN);
}

static inline void st7735_EnableCommand(void)
{
	if(st7735_IsCommandMode == 1)
	{
		return;
	}
	else
	{
		while(SSP_GetStatus(LPC_SSP0, SSP_STAT_BUSY));
		GPIO_ResetBits(RS_PORT, RS_PIN);
		st7735_IsCommandMode = 1;
	}
}

static inline void st7735_DisableCommand(void)
{
	if(st7735_IsCommandMode == 0)
	{
		return;
	}
	else
	{
		while(SSP_GetStatus(LPC_SSP0, SSP_STAT_BUSY));
		GPIO_SetBits(RS_PORT, RS_PIN);
		st7735_IsCommandMode = 0;
	}
}

static inline void st7735_SpiTransmitByte(uint8_t data)
{
	while(!SSP_GetStatus(LPC_SSP0, SSP_STAT_TXFIFO_NOTFULL));
	SSP_SendData(LPC_SSP0,(uint16_t)data);
}

static inline void st7735_WaitMicroSeconds(uint32_t usec)
{
	// TODO : Implement
	//(void)usec;
	return;
}

