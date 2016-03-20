#pragma once

#include <stdio.h>
#include <stdint.h>

#include <lpc11xx_syscon.h>
#include <lpc11xx_ssp.h>
#include <lpc11xx_gpio.h>

#include "st7735_const.hpp"

typedef struct _color16bit_t
{
	uint16_t r: 5;
	uint16_t g: 6;
	uint16_t b: 5;
} color16bit_t;

typedef struct _color18bit_t
{
	uint8_t b;
	uint8_t g;
	uint8_t r;
} color18bit_t;

class st7735
{

public:
	st7735();
	~st7735();

	void Initialize();
	void Finalize();

	void Clear(color18bit_t* color);

	// Function of ST7735
	void SoftwareReset()
	{
		WriteCommand(0x01);
		usleep(120*1000);
	}
	void SleepIn()
	{
		WriteCommand(0x10);
	}
	void SleepOut()
	{
		WriteCommand(0x11);
		usleep(120*1000);
	}
	void PartialModeOn()
	{
		WriteCommand(0x12);
	}
	void PartialModeOff()
	{
		WriteCommand(0x13);
	}
	void SetDisplayInversion(bool isEnable)
	{
		WriteCommand(0x20 | (isEnable? 1:0));
	}
	void SetGammaCurve(uint8_t gamma)
	{
		WriteCommand(0x26);
		WriteData(gamma);
	}
	void DisplayOff()
	{
		WriteCommand(0x28);
	}
	void DisplayOn()
	{
		WriteCommand(0x29);
	}
	void SetColumnAddress(uint16_t start, uint16_t end)
	{
		WriteCommand(0x2A);
		WriteData(static_cast<uint8_t>(start >> 8));
		WriteData(static_cast<uint8_t>(start));
		WriteData(static_cast<uint8_t>(end >> 8));
		WriteData(static_cast<uint8_t>(end));

//		WriteData(reinterpret_cast<uint8_t*>(&start),2);
//		WriteData(reinterpret_cast<uint8_t*>(&end),2);
	}
	void SetRowAddress(uint16_t start, uint16_t end)
	{
		WriteCommand(0x2B);
		WriteData(static_cast<uint8_t>(start >> 8));
		WriteData(static_cast<uint8_t>(start));
		WriteData(static_cast<uint8_t>(end >> 8));
		WriteData(static_cast<uint8_t>(end));

//		WriteData(reinterpret_cast<uint8_t*>(&start),2);
//		WriteData(reinterpret_cast<uint8_t*>(&end),2);
	}
	void WriteMemory(uint8_t *data, size_t length)
	{
		WriteCommand(0x2C);
		WriteData(data, length);
	}
	void SetPartialAddress(uint16_t start, uint16_t end)
	{
		WriteCommand(0x30);
		WriteData(reinterpret_cast<uint8_t*>(&start),sizeof(uint16_t));
		WriteData(reinterpret_cast<uint8_t*>(&end),sizeof(uint16_t));
	}
	void TearOn()
	{
		WriteCommand(0x34);
	}
	void TearOff()
	{
		WriteCommand(0x35);
	}
	void SetMemoryAccessControl(uint8_t rowAddressOrder, uint8_t columnAddressOrder, uint8_t rowColumnExchange)
	{
		WriteCommand(0x36);
	}
	void IdleModeOff()
	{
		WriteCommand(0x39);
	}
	void IdleModeOn()
	{
		WriteCommand(0x38);
	}
	void SetPixelFormat(PixelFormat format)
	{
		WriteCommand(0x3A);
		WriteData(static_cast<uint8_t>(format));
	}
	void SetNormalFrameRate(uint8_t RTNA, uint8_t FPA, uint8_t BPA)
	{
		WriteCommand(0xB1);
		WriteData(RTNA);
		WriteData(FPA);
		WriteData(BPA);
	}
	void SetIdleFrameRate(uint8_t RTNB, uint8_t FPB, uint8_t BPB)
	{
		WriteCommand(0xB2);
		WriteData(RTNB);
		WriteData(FPB);
		WriteData(BPB);
	}
	void SetPartialFrameRate(uint8_t RTNC, uint8_t FPC, uint8_t BPC,uint8_t RTND, uint8_t FPD, uint8_t BPD)
	{
		WriteCommand(0xB3);
		WriteData(RTNC);
		WriteData(FPC);
		WriteData(BPC);
		WriteData(RTND);
		WriteData(FPD);
		WriteData(BPD);
	}
	void SetDisplayInversion(uint8_t data)
	{
		WriteCommand(0xB4);
		WriteData(data);
	}
	void SetDisplayFunction(uint8_t param1, uint8_t param2)
	{
		WriteCommand(0xB6);
		WriteData(param1);
		WriteData(param2);
	}
	void SetPowerControl1(uint8_t param1, uint8_t param2)
	{
		WriteCommand(0xC0);
		WriteData(param1);
		WriteData(param2 | 0b01000000);
	}
	void SetPowerControl2(uint8_t param)
	{
		WriteCommand(0xC1);
		WriteData(param);
	}
	void SetPowerControl3(uint8_t CurrentInOpAmp, uint8_t CycleInBooster)
	{
		WriteCommand(0xC2);
		WriteData(CurrentInOpAmp);
		WriteData(CycleInBooster);
	}
	void SetPowerControl4(uint8_t CurrentInOpAmp, uint8_t CycleInBooster)
	{
		WriteCommand(0xC3);
		WriteData(CurrentInOpAmp);
		WriteData(CycleInBooster);
	}
	void SetPowerControl5(uint8_t CurrentInOpAmp, uint8_t CycleInBooster)
	{
		WriteCommand(0xC4);
		WriteData(CurrentInOpAmp);
		WriteData(CycleInBooster);
	}
	void SetVcomControl(uint8_t VcomH, uint8_t VcomL)
	{
		WriteCommand(0xC5);
		WriteData(VcomH);
		WriteData(VcomL);
	}
	void SetVcomOffset(uint8_t VMF)
	{
		WriteCommand(0xC7);
		WriteData(VMF);
	}
	void SetPowerControl6(uint8_t param1, uint8_t param2)
	{
		WriteCommand(0xFC);
		WriteData(param1);
		WriteData(param2);
	}
	void SetExtentionCommand()
	{
		WriteCommand(0xF0);
		WriteData(0x01);
	}

private:
	bool m_IsInitialized;

	void InitializeDeviceB();
	void InitializeDeviceR();

	void HardwareReset()
	{
		GPIO_ResetBits(PORT2, GPIO_Pin_9);
		usleep(20);
		GPIO_SetBits(PORT2, GPIO_Pin_9);
		usleep(120*1000);

	}
	void WriteCommand(uint8_t command)
	{
		GPIO_ResetBits(PORT2, GPIO_Pin_10);

		GPIO_SetBits(PORT2, GPIO_Pin_2);
		SSP_SendData(LPC_SSP0,(uint16_t)command);
		GPIO_ResetBits(PORT2, GPIO_Pin_2);
	}
	void WriteData(uint8_t *data, size_t length)
	{
		GPIO_SetBits(PORT2, GPIO_Pin_10);

		GPIO_SetBits(PORT2, GPIO_Pin_2);
		for(uint32_t i=0; i<length;i++)
		{
			SSP_SendData(LPC_SSP0,(uint16_t)data[i]);
		}
		GPIO_ResetBits(PORT2, GPIO_Pin_2);
	}
	void WriteData(uint8_t data)
	{
		GPIO_SetBits(PORT2, GPIO_Pin_10);

		GPIO_SetBits(PORT2, GPIO_Pin_2);
		SSP_SendData(LPC_SSP0,(uint16_t)data);
		GPIO_ResetBits(PORT2, GPIO_Pin_2);
	}
	void WriteData(uint16_t *data, size_t length)
	{
		GPIO_SetBits(PORT2, GPIO_Pin_10);

		GPIO_SetBits(PORT2, GPIO_Pin_2);
		for(uint32_t i=0; i<length;i++)
		{
			SSP_SendData(LPC_SSP0,(uint16_t)data[i]);
		}
		GPIO_ResetBits(PORT2, GPIO_Pin_2);
	}

};
