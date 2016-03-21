#pragma once

#include <stdio.h>

#include "st7735_ioAccessor.h"
#include "st7735_const.h"

typedef struct _color16bit_t
{
	uint16_t b: 5;
	uint16_t g: 6;
	uint16_t r: 5;
} st7735_Color16bit_t;

typedef struct _color18bit_t
{
	uint8_t b;
	uint8_t g;
	uint8_t r;
} st7735_Color18bit_t;

void st7735_Initialize(void);
void st7735_Finalize(void);
void st7735_Clear(st7735_Color16bit_t* color);
void st7735_InitializeDeviceB(void);
void st7735_InitializeDeviceR(void);

static inline void st7735_WriteCommand(uint8_t command)
{
	st7735_EnableCommand();
	//st7735_EnableChipSelect();
	st7735_SpiTransmitByte(command);
	//st7735_DisableChipSelect();
}

static inline void st7735_WriteDataBytes(uint8_t *data, size_t length)
{
	st7735_DisableCommand();
	//st7735_EnableChipSelect();
	for(uint32_t i=0; i<length;i++)
	{
		st7735_SpiTransmitByte(data[i]);
	}
	//st7735_DisableChipSelect();
}

static inline void st7735_WriteDataByte(uint8_t data)
{
	st7735_DisableCommand();
	//st7735_EnableChipSelect();
	st7735_SpiTransmitByte(data);
	//st7735_DisableChipSelect();
}

static inline void st7735_WriteDataWords(uint16_t *data, size_t length)
{
	uint8_t *bytes = (uint8_t*)data;

	st7735_DisableCommand();
//	st7735_EnableChipSelect();
	for(uint32_t i=0; i<length*2;i+=2)
	{
		st7735_SpiTransmitByte(bytes[i]);
		st7735_SpiTransmitByte(bytes[i+1]);
	}
//	st7735_DisableChipSelect();
}

static inline void st7735_WriteDataWord(uint16_t* data)
{
	uint8_t *bytes = (uint8_t*)data;

	st7735_DisableCommand();
//	st7735_EnableChipSelect();
	st7735_SpiTransmitByte(bytes[0]);
	st7735_SpiTransmitByte(bytes[1]);
//	st7735_DisableChipSelect();
}

static inline void st7735_SoftwareReset(void)
{
	st7735_WriteCommand(0x01);
	st7735_WaitMicroSeconds(120*1000);
}

static inline void st7735_SleepIn(void)
{
	st7735_WriteCommand(0x10);
}

static inline void st7735_SleepOut(void)
{
	st7735_WriteCommand(0x11);
	st7735_WaitMicroSeconds(120*1000);
}

static inline void st7735_PartialModeOn(void)
{
	st7735_WriteCommand(0x12);
}

static inline void st7735_PartialModeOff(void)
{
	st7735_WriteCommand(0x13);
}

static inline void st7735_SetDisplayInversion(uint8_t isEnable)
{
	st7735_WriteCommand(0x20 | (isEnable? 1:0));
}

static inline void st7735_SetGammaCurve(uint8_t gamma)
{
	st7735_WriteCommand(0x26);
	st7735_WriteDataByte(gamma);
}

static inline void st7735_DisplayOff(void)
{
	st7735_WriteCommand(0x28);
}

static inline void st7735_DisplayOn(void)
{
	st7735_WriteCommand(0x29);
}

static inline void st7735_SetColumnAddress(uint16_t start, uint16_t end)
{
	st7735_WriteCommand(0x2A);
	st7735_WriteDataByte((uint8_t)(start >> 8));
	st7735_WriteDataByte((uint8_t)(start));
	st7735_WriteDataByte((uint8_t)(end >> 8));
	st7735_WriteDataByte((uint8_t)(end));
}

static inline void st7735_SetRowAddress(uint16_t start, uint16_t end)
{
	st7735_WriteCommand(0x2B);
	st7735_WriteDataByte((uint8_t)(start >> 8));
	st7735_WriteDataByte((uint8_t)(start));
	st7735_WriteDataByte((uint8_t)(end >> 8));
	st7735_WriteDataByte((uint8_t)(end));
}

static inline void st7735_WriteMemory(uint8_t *data, size_t length)
{
	st7735_WriteCommand(0x2C);
	st7735_WriteDataBytes(data, length);
}

static inline void st7735_SetPartialAddress(uint16_t start, uint16_t end)
{
	st7735_WriteCommand(0x30);
	st7735_WriteDataBytes((uint8_t*)(&start),sizeof(uint16_t));
	st7735_WriteDataBytes((uint8_t*)(&end),sizeof(uint16_t));
}

static inline void st7735_TearOn(void)
{
	st7735_WriteCommand(0x34);
}

static inline void st7735_TearOff(void)
{
	st7735_WriteCommand(0x35);
}

static inline void st7735_SetMemoryAccessControl(uint8_t rowAddressOrder, uint8_t columnAddressOrder, uint8_t rowColumnExchange)
{
	st7735_WriteCommand(0x36);
}

static inline void st7735_IdleModeOff(void)
{
	st7735_WriteCommand(0x39);
}

static inline void st7735_IdleModeOn(void)
{
	st7735_WriteCommand(0x38);
}

static inline void st7735_SetPixelFormat(st7735_PixelFormat format)
{
	st7735_WriteCommand(0x3A);
	st7735_WriteDataByte((uint8_t)(format));
}

static inline void st7735_SetNormalFrameRate(uint8_t RTNA, uint8_t FPA, uint8_t BPA)
{
	st7735_WriteCommand(0xB1);
	st7735_WriteDataByte(RTNA);
	st7735_WriteDataByte(FPA);
	st7735_WriteDataByte(BPA);
}

static inline void st7735_SetIdleFrameRate(uint8_t RTNB, uint8_t FPB, uint8_t BPB)
{
	st7735_WriteCommand(0xB2);
	st7735_WriteDataByte(RTNB);
	st7735_WriteDataByte(FPB);
	st7735_WriteDataByte(BPB);
}

static inline void st7735_SetPartialFrameRate(uint8_t RTNC, uint8_t FPC, uint8_t BPC,uint8_t RTND, uint8_t FPD, uint8_t BPD)
{
	st7735_WriteCommand(0xB3);
	st7735_WriteDataByte(RTNC);
	st7735_WriteDataByte(FPC);
	st7735_WriteDataByte(BPC);
	st7735_WriteDataByte(RTND);
	st7735_WriteDataByte(FPD);
	st7735_WriteDataByte(BPD);
}

static inline void st7735_SetDisplayInversion_(uint8_t data)
{
	st7735_WriteCommand(0xB4);
	st7735_WriteDataByte(data);
}

static inline void st7735_SetDisplayFunction(uint8_t param1, uint8_t param2)
{
	st7735_WriteCommand(0xB6);
	st7735_WriteDataByte(param1);
	st7735_WriteDataByte(param2);
}

static inline void st7735_SetPowerControl1(uint8_t param1, uint8_t param2)
{
	st7735_WriteCommand(0xC0);
	st7735_WriteDataByte(param1);
	st7735_WriteDataByte(param2 | 0b01000000);
}

static inline void st7735_SetPowerControl2(uint8_t param)
{
	st7735_WriteCommand(0xC1);
	st7735_WriteDataByte(param);
}

static inline void st7735_SetPowerControl3(uint8_t CurrentInOpAmp, uint8_t CycleInBooster)
{
	st7735_WriteCommand(0xC2);
	st7735_WriteDataByte(CurrentInOpAmp);
	st7735_WriteDataByte(CycleInBooster);
}

static inline void st7735_SetPowerControl4(uint8_t CurrentInOpAmp, uint8_t CycleInBooster)
{
	st7735_WriteCommand(0xC3);
	st7735_WriteDataByte(CurrentInOpAmp);
	st7735_WriteDataByte(CycleInBooster);
}

static inline void st7735_SetPowerControl5(uint8_t CurrentInOpAmp, uint8_t CycleInBooster)
{
	st7735_WriteCommand(0xC4);
	st7735_WriteDataByte(CurrentInOpAmp);
	st7735_WriteDataByte(CycleInBooster);
}

static inline void st7735_SetVcomControl(uint8_t VcomH, uint8_t VcomL)
{
	st7735_WriteCommand(0xC5);
	st7735_WriteDataByte(VcomH);
	st7735_WriteDataByte(VcomL);
}

static inline void st7735_SetVcomOffset(uint8_t VMF)
{
	st7735_WriteCommand(0xC7);
	st7735_WriteDataByte(VMF);
}

static inline void st7735_SetPowerControl6(uint8_t param1, uint8_t param2)
{
	st7735_WriteCommand(0xFC);
	st7735_WriteDataByte(param1);
	st7735_WriteDataByte(param2);
}

static inline void st7735_SetExtentionCommand(void)
{
	st7735_WriteCommand(0xF0);
	st7735_WriteDataByte(0x01);
}

static inline void st7735_HardwareReset(void)
{
	st7735_EnableReset();
	st7735_WaitMicroSeconds(20);
	st7735_DisableReset();
	st7735_WaitMicroSeconds(120*1000);
}
