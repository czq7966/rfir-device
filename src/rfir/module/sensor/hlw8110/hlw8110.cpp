#include "hlw8110.h"

bool 			rfir::module::sensor::HLW8110::Inited = false;
unsigned char 	rfir::module::sensor::HLW8110::u8_TxBuf[] = {};
unsigned char 	rfir::module::sensor::HLW8110::u8_RxBuf[] = {};
unsigned char	rfir::module::sensor::HLW8110::u8_TX_Length = 0;
unsigned char	rfir::module::sensor::HLW8110::u8_RX_Length = 0;
unsigned char	rfir::module::sensor::HLW8110::u8_RX_Index = 0;
//unsigned char	B_ReadReg_Time_EN;			// 串口读取寄存器数据，时间计数器标志位，1--开启计数，0--关闭计数
//unsigned char	B_Tx_Finish;
unsigned char	rfir::module::sensor::HLW8110::B_Rx_Finish = 0;
unsigned char	rfir::module::sensor::HLW8110::B_Rx_Data_ING = 0;					// 接收数据标志位	,		< 1:接收数据中,0:未接收到数据 >
unsigned char	rfir::module::sensor::HLW8110::B_Read_Error = 0;							// UART读取出据校验和出错,< 1:数据读错，0:数据读取正确 >
//unsigned char	u8_ReadReg_Index;
//unsigned char	u8_ReadReg_Time;				// 串口读取寄存器数据的时间
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

unsigned int    rfir::module::sensor::HLW8110::U16_TempData = 0;	


unsigned int    rfir::module::sensor::HLW8110::U16_IFData = 0;
unsigned int    rfir::module::sensor::HLW8110::U16_RIFData = 0;
unsigned int    rfir::module::sensor::HLW8110::U16_LineFData = 0;
unsigned int    rfir::module::sensor::HLW8110::U16_AngleData = 0;
unsigned int    rfir::module::sensor::HLW8110::U16_PFData = 0;
unsigned int 	rfir::module::sensor::HLW8110::U16_HFConst_RegData = 0;
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
unsigned int	rfir::module::sensor::HLW8110::U16_RMSIAC_RegData = 0; 			// A通道电流转换系数
unsigned int	rfir::module::sensor::HLW8110::U16_RMSIBC_RegData = 0; 			// B通道电流转换系数
unsigned int	rfir::module::sensor::HLW8110::U16_RMSUC_RegData = 0; 				// 电压通道转换系数
unsigned int	rfir::module::sensor::HLW8110::U16_PowerPAC_RegData = 0; 		// A通道功率转换系数
unsigned int	rfir::module::sensor::HLW8110::U16_PowerPBC_RegData = 0; 		// B通道功率转换系数
unsigned int	rfir::module::sensor::HLW8110::U16_PowerSC_RegData = 0; 			// 视在功率转换系数,如果选择A通道，则是A通道视在功率转换系数。A和B通道只能二者选其一
unsigned int	rfir::module::sensor::HLW8110::U16_EnergyAC_RegData = 0; 		// A通道有功电能(量)转换系数 
unsigned int	rfir::module::sensor::HLW8110::U16_EnergyBC_RegData = 0; 		// A通道有功电能(量)转换系数
unsigned int	rfir::module::sensor::HLW8110::U16_CheckSUM_RegData = 0; 		// 转换系数的CheckSum
unsigned int	rfir::module::sensor::HLW8110::U16_CheckSUM_Data = 0; 				// 转换系数计算出来的CheckSum

unsigned int	rfir::module::sensor::HLW8110::U16_Check_SysconReg_Data = 0; 						
unsigned int	rfir::module::sensor::HLW8110::U16_Check_EmuconReg_Data = 0; 						
unsigned int	rfir::module::sensor::HLW8110::U16_Check_Emucon2Reg_Data = 0; 				

/*---------------------------------------------------------------------------------------------------------*/			
/*---------------------------------------------------------------------------------------------------------*/
unsigned long 	rfir::module::sensor::HLW8110::U32_RMSIA_RegData = 0;			// A通道电流有效值寄存器
unsigned long 	rfir::module::sensor::HLW8110::U32_RMSU_RegData = 0;				// 电压有效值寄存器
unsigned long 	rfir::module::sensor::HLW8110::U32_POWERPA_RegData = 0;		// A通道功率有效值寄存器
unsigned long 	rfir::module::sensor::HLW8110::U32_ENERGY_PA_RegData = 0;	// A通道有功电能（量）有效值寄存器


unsigned long 	rfir::module::sensor::HLW8110::U32_RMSIB_RegData = 0;			// B通道电流有效值寄存器
unsigned long	rfir::module::sensor::HLW8110::U32_POWERPB_RegData = 0;		// B通道功率有效值寄存器
unsigned long 	rfir::module::sensor::HLW8110::U32_ENERGY_PB_RegData = 0;	// B通道有功电能（量）有效值寄存器
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
float   rfir::module::sensor::HLW8110::F_AC_V = 0;													// 电压有效值
float   rfir::module::sensor::HLW8110::F_AC_I = 0;													// A通道电流
float   rfir::module::sensor::HLW8110::F_AC_P = 0;													// A通道有功功率
float   rfir::module::sensor::HLW8110::F_AC_E = 0;													// A通道有功电能(量)
float   rfir::module::sensor::HLW8110::F_AC_BACKUP_E = 0;									// A通道电量备份	
float   rfir::module::sensor::HLW8110::F_AC_PF = 0;												// 功率因素，A通道和B通道只能选其一 
float   rfir::module::sensor::HLW8110::F_Angle = 0;												// 相角，A通道和B通道只能选其一 

float   rfir::module::sensor::HLW8110::F_AC_I_B = 0;												// B通道电流有效值
float   rfir::module::sensor::HLW8110::F_AC_P_B = 0;												// B通道有功功率
float   rfir::module::sensor::HLW8110::F_AC_E_B = 0;												// B通道有功电能(量)
float   rfir::module::sensor::HLW8110::F_AC_BACKUP_E_B = 0;								// B通道电量备份	
float   rfir::module::sensor::HLW8110::F_AC_LINE_Freq = 0;     						// 市电线性频率
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/     

rfir::module::sensor::HLW8110::OnIASwitch rfir::module::sensor::HLW8110::On_IA_Switch = nullptr;
int      rfir::module::sensor::HLW8110::IA_Switch_CB_Arg = 0;
int      rfir::module::sensor::HLW8110::IA_Switch_Check_Interval = 1 * 1000;    //每秒检查一次A电流 ms
float    rfir::module::sensor::HLW8110::IA_Switch_Point = 50;             //A电流代表切换的阀值 mA
int      rfir::module::sensor::HLW8110::IA_Switch_elapse = 200;           //阀值后至少持续的时长 ms
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/     


void rfir::module::sensor::HLW8110::delay_ms(unsigned long ms) {
    delayMicroseconds(ms * 1000);
}

/*=========================================================================================================
 * Function : void Start_Send_UartData(unsigned char len)
 * Describe : UART2串口发送数据
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/16
=========================================================================================================*/

void rfir::module::sensor::HLW8110::Start_Send_UartData(unsigned char len)
{
	unsigned char i;
	for(i=0;i<len;i++)
	{
        USART.write(u8_TxBuf[i]);		
	}

}

void rfir::module::sensor::HLW8110::Start_Recv_UartData(unsigned char len, unsigned long timeout_ms) {
    static unsigned long HLW8110_Start_Recv_UartData_time = 0;
    u8_RX_Index = 0;
    int c = -1;
    HLW8110_Start_Recv_UartData_time = millis();
    while ((u8_RX_Index < len) && (millis() - HLW8110_Start_Recv_UartData_time <= timeout_ms)) {
        c = USART.read();
        if (c >=0) {
            u8_RxBuf[u8_RX_Index] = (char)c;
            u8_RX_Index++;
            B_Rx_Data_ING = 1;                                  // 置数据接收标志位
            HLW8110_Start_Recv_UartData_time = millis();
        }
    }

    if (u8_RX_Index == len) {
        // printf("接收成功:%x == %x\r\n" ,u8_RX_Index, len);
        // for (size_t i = 0; i < len; i++)
        // {
        //     GDebuger.write(u8_RxBuf[i]);
        // }
        // GDebuger.println("");

        B_Rx_Finish = true;										// 数据接收完毕
        u8_RX_Index = 0;              

    } else {
        // printf("接收失败:%x != %x\r\n" ,u8_RX_Index, len);
    }



}

/*=========================================================================================================
 * Function : void Clear_RxBuf(void)
 * Describe : 在准备接收串口数据前，清空接收缓存器的数据
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/03
=========================================================================================================*/
void rfir::module::sensor::HLW8110::Clear_RxBuf(void)
{
	unsigned char i;
	for(i = 0; i < U8_Buf_Max_Length; i++)
	{
		u8_RxBuf[i] = 0x00;
	}
	
	B_Rx_Data_ING = 0;
	B_Rx_Finish = false;
	u8_RX_Index = 0;
}

/*=========================================================================================================
 * Function : unsigned char HLW8110_checkSum_Write(unsigned char u8_Reg_length)
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/03
=========================================================================================================*/
unsigned char rfir::module::sensor::HLW8110::HLW8110_checkSum_Write(unsigned char u8_Reg_length)
{
	unsigned char i;
	unsigned char Temp_u8_checksum;
	unsigned int	a;

	a = 0x0000;
	Temp_u8_checksum = 0;
	for (i = 0; i< (u8_Reg_length-1); i++)
		{
			a += u8_TxBuf[i];
		}
	
	a = ~a;
	Temp_u8_checksum = a & 0xff;

	return Temp_u8_checksum;
	
}

/*=========================================================================================================
 * Function : unsigned char HLW8110_checkSum_Read(void)
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/03
==========================================================================================================*/
unsigned char rfir::module::sensor::HLW8110::HLW8110_checkSum_Read(unsigned char u8_Reg_length)
{
	unsigned char i;
	unsigned char Temp_u8_checksum;
	unsigned int a;

	a = 0x0000;
	Temp_u8_checksum = 0;
	for (i = 0; i< (u8_Reg_length-1); i++)
		{
			a += u8_RxBuf[i];
		}
		
	a = a + u8_TxBuf[0] + u8_TxBuf[1];
	a = ~a;
		
	Temp_u8_checksum = a & 0xff;

	return Temp_u8_checksum;
	
}

/*=========================================================================================================
 * Function : void Uart_HLW8110_WriteREG_EN(void)
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/03
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Uart_HLW8110_WriteREG_EN(void)
{

	u8_TX_Length = 4;
	u8_RX_Length = 0;
	
	u8_TxBuf[0] = 0xa5;
	u8_TxBuf[1] = 0xea;
	u8_TxBuf[2] = 0xe5;
//	u8_TxBuf[3] = 0x8b;  //checksum
	u8_TxBuf[3] = HLW8110_checkSum_Write(u8_TX_Length);

	Start_Send_UartData(u8_TX_Length);
		
}

/*=========================================================================================================
 * Function : void Uart_HLW8110_WriteREG_DIS(void)
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/03
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Uart_HLW8110_WriteREG_DIS(void)
{
	
	u8_TX_Length = 4;
	u8_RX_Length = 0;
	
	u8_TxBuf[0] = 0xa5;
	u8_TxBuf[1] = 0xea;
	u8_TxBuf[2] = 0xdc;
//	u8_TxBuf[3] = 0x94;  //checksum
	u8_TxBuf[3] = HLW8110_checkSum_Write(u8_TX_Length);
        
	Start_Send_UartData(u8_TX_Length);
}

/*=========================================================================================================
 * Function : void Uart_HLW8110_Set_Channel_A(void)
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/03
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Uart_HLW8110_Set_Channel_A(void)
{
	u8_TX_Length = 4;
	u8_RX_Length = 0;
	
	u8_TxBuf[0] = 0xa5;
	u8_TxBuf[1] = 0xea;
	u8_TxBuf[2] = 0x5a;
//	u8_TxBuf[3] = 0x16;  //checksum
	u8_TxBuf[3] = HLW8110_checkSum_Write(u8_TX_Length);

	Start_Send_UartData(u8_TX_Length);
}


/*=========================================================================================================
 * Function : void Uart_Read_HLW8110_Reg(unsigned char ADDR_Reg,unsigned char u8_reg_length)
 * Describe :
 * Input    : 
 * Output   : 
 * Return   : 
 * Record   : 2019/04/04
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Uart_Read_HLW8110_Reg(unsigned char ADDR_Reg,unsigned char u8_reg_length)
{
	u8_TxBuf[0] = 0xa5;
	u8_TxBuf[1] = ADDR_Reg;
	u8_TX_Length =  2;
	u8_RX_Length = u8_reg_length + 1;	// +1，是因为接收的数据长度，除了REG值，还有一个校验和数据
	
	
	Clear_RxBuf();										//清空接收缓冲区
	Start_Send_UartData(u8_TX_Length);
    Start_Recv_UartData(u8_RX_Length);

}

/*=========================================================================================================
 * Function : void Uart_Write_HLW8110_Reg(unsigned char ADDR_Reg,unsigned char u8_reg_length,unsigned long u32_data)
 * Describe : 写寄存器命令，u8_reg_length：写入的寄存器数据字节长度
 * Input    : 
 * Output   : 
 * Return   : 
 * Record   : 2019/04/03
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Uart_Write_HLW8110_Reg(unsigned char ADDR_Reg,unsigned char u8_reg_length,unsigned long u32_data)
{
	unsigned char i;
	union LongData Temp_u32_a;

	
	u8_TxBuf[0] = 0xa5;
	u8_TxBuf[1] = ADDR_Reg|0x80;

	Temp_u32_a.word = u32_data;
	for (i = 0; i< u8_reg_length; i++)
		{
			u8_TxBuf[i+2] = Temp_u32_a.byte[u8_reg_length-1-i];						//STM32，32位MCU，union定义，是低位在前
			//u8_TxBuf[i+2] = Temp_u32_a.byte[4-u8_reg_length + i];				//STM8,STC MCU,   union定义，是高位在前
		}


	u8_TX_Length = 3 + u8_reg_length ;
	u8_RX_Length = 0;
	
	u8_TxBuf[u8_TX_Length-1] = HLW8110_checkSum_Write(u8_TX_Length);


	Start_Send_UartData(u8_TX_Length);
}


/*=========================================================================================================
 * Function : void Uart_HLW8110_Reset(void)
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/03
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Uart_HLW8110_Reset(void)
{
	
	u8_TX_Length = 4;
	u8_RX_Length = 0;
	
	u8_TxBuf[0] = 0xa5;
	u8_TxBuf[1] = 0xea;
	u8_TxBuf[2] = 0x96;
//	u8_TxBuf[3] = 0xda;  //checksum
	
	u8_TxBuf[3] = HLW8110_checkSum_Write(u8_TX_Length);

	Start_Send_UartData(u8_TX_Length);
}



/*=========================================================================================================
 * Function : void Judge_CheckSum_HLW8110_Calfactor(void)
 * Describe : 验证地址0x70-0x77地址的系数和
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/03/18
==========================================================================================================*/
unsigned char rfir::module::sensor::HLW8110::Judge_CheckSum_HLW8110_Calfactor(void)
{
	unsigned long a;
	unsigned int b;
 
  	//读取RmsIAC、RmsIBC、RmsUC、PowerPAC、PowerPBC、PowerSC、EnergAc、EnergBc的值
	
	Uart_Read_HLW8110_Reg(REG_RMS_IAC_ADDR,2);
	delay_ms(50);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U16_RMSIAC_RegData = (u8_RxBuf[0]<<8) + u8_RxBuf[1] ;
		printf("A通道电流转换系数:%x\r\n" ,U16_RMSIAC_RegData);
	}	
	
	Uart_Read_HLW8110_Reg(REG_RMS_IBC_ADDR,2);
	delay_ms(50);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U16_RMSIBC_RegData = (u8_RxBuf[0]<<8) + u8_RxBuf[1] ;
		printf("B通道电流转换系数:%x\r\n" ,U16_RMSIBC_RegData);
	}	
	
	Uart_Read_HLW8110_Reg(REG_RMS_UC_ADDR,2);
	delay_ms(50);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U16_RMSUC_RegData = (u8_RxBuf[0]<<8) + u8_RxBuf[1] ;
		printf("电压通道转换系数:%x\r\n" ,U16_RMSUC_RegData);
	}
		
	Uart_Read_HLW8110_Reg(REG_POWER_PAC_ADDR,2);
	delay_ms(50);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U16_PowerPAC_RegData = (u8_RxBuf[0]<<8) + u8_RxBuf[1] ;
		printf("A通道功率转换系数:%x\r\n" ,U16_PowerPAC_RegData);
	}
		
	Uart_Read_HLW8110_Reg(REG_POWER_PBC_ADDR,2);
	delay_ms(50);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U16_PowerPBC_RegData = (u8_RxBuf[0]<<8) + u8_RxBuf[1] ;
		printf("B通道功率转换系数:%x\r\n" ,U16_PowerPAC_RegData);
	}
	
	Uart_Read_HLW8110_Reg(REG_POWER_SC_ADDR,2);
	delay_ms(50);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U16_PowerSC_RegData = (u8_RxBuf[0]<<8) + u8_RxBuf[1] ;
		printf("视在功率转换系数:%x\r\n" ,U16_PowerSC_RegData);
	}
	
	Uart_Read_HLW8110_Reg(REG_ENERGY_AC_ADDR,2);
	delay_ms(50);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U16_EnergyAC_RegData = (u8_RxBuf[0]<<8) + u8_RxBuf[1] ;
		printf("A通道电量转换系数:%x\r\n" ,U16_EnergyAC_RegData);
	
	}
	
	Uart_Read_HLW8110_Reg(REG_ENERGY_BC_ADDR,2);
	delay_ms(50);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U16_EnergyBC_RegData = (u8_RxBuf[0]<<8) + u8_RxBuf[1] ;
		printf("B通道电量转换系数:%x\r\n" ,U16_EnergyBC_RegData);
	}
	 
	Uart_Read_HLW8110_Reg(REG_CHECKSUM_ADDR,2);
	delay_ms(50);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U16_CheckSUM_RegData= (u8_RxBuf[0]<<8) + u8_RxBuf[1] ;
		printf("系数校验和:%x\r\n" ,U16_CheckSUM_RegData);
	}
	
	a = 0;
  	a = ~(0xffff+U16_RMSIAC_RegData + U16_RMSIBC_RegData + U16_RMSUC_RegData + 
        U16_PowerPAC_RegData + U16_PowerPBC_RegData + U16_PowerSC_RegData + 
        U16_EnergyAC_RegData + U16_EnergyBC_RegData  );
  
	U16_CheckSUM_Data = a & 0xffff;
	
	printf("计算系数校验和:%x\r\n" ,U16_CheckSUM_Data);
  
	if ( U16_CheckSUM_Data == U16_CheckSUM_RegData)
	{
		b = 1;
		printf("校验和正确\r\n");
    
	}
	else
	{
		b = 0;
		printf("校验和出错\r\n");
	}
  
	return b;
  
}


/*=========================================================================================================
 * Function : void Init_HLW8110(void)
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/03
==========================================================================================================*/
bool rfir::module::sensor::HLW8110::Init_HLW8110(bool force)
{
	if (force) Inited = false;	
	if (Inited) return Inited;
	
#if 0
	//STM32 IO init
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟	   	 
//	RCC->APB2ENR|=5<<6;    //使能PORTD时钟	
	   	 
	GPIOA->CRL&=0X000FFFFF;  //IO状态设置,设置PA7、PA6、PA5、PA3、PA2
	GPIOA->CRL|=0X33300000;  //IO状态设置，PA7、PA6、PA5、PA2-输出、PA3-输入	
	
	
  //9600 bps,1S传输9600/11bit = 872byte
  IO_HLW8112_EN = LOW;
  IO_HLW8112_SCLK = HIGH;
  IO_HLW8112_CS = LOW;
#endif	
	
	Uart_HLW8110_WriteREG_EN();
	delay_ms(10);
	
//电流通道A设置命令，指定当前用于计算视在功率、功率因数、相角、瞬时有功功率、瞬时视在功率和有功功率过载的信号指示 的通道为通道A	
	Uart_HLW8110_Set_Channel_A();			
	delay_ms(10);

	//Uart_Write_HLW8110_Reg(REG_SYSCON_ADDR,2,0x0a04);	//开启A通道，关闭B通道，电压通道PGA = 1，电流通道PGA = 16
	Uart_Write_HLW8110_Reg(REG_SYSCON_ADDR,2,0x0a01);	//开启A通道，关闭B通道，电压通道PGA = 1，电流通道PGA = 2
	delay_ms(10);
#if 0	

	Uart_Write_HLW8110_Reg(REG_EMUCON_ADDR,2,0x0001);	//1，使能PFA 脉冲输出和有功电能寄存器累加；
//	Uart_Write_HLW8110_Reg(REG_EMUCON_ADDR,2,0x0018);	//正向和负向过零点均发生变化，ZXD0 = 1，ZXD1 = 1
	Uart_Write_HLW8110_Reg(REG_EMUCON2_ADDR,2,0x0465);	//0x0001是EMUCON2的默认值，waveEn = 1,zxEn = 1，A通道电量寄存器，读后不清0，EPA_CB = 1；打开功率因素检测
	delay_ms(10);
#endif
	
	Uart_HLW8110_WriteREG_DIS();
//	delay_ms(10);	
  //读取地址是0x6F至0x77的寄存器，验证系数是否正确
  Inited = Judge_CheckSum_HLW8110_Calfactor();	
  return Inited;
}



/*=========================================================================================================
 * Function : void Check_WriteReg_Success(void)
 * Describe : 检验写入的REG是否正确写入
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2020/04/02
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Check_WriteReg_Success(void)
{
	Uart_Read_HLW8110_Reg(REG_SYSCON_ADDR,2);
	delay_ms(10);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U16_Check_SysconReg_Data =  (u8_RxBuf[0]<<8) + (u8_RxBuf[1]); 
		printf("写入的SysconReg寄存器:%lx\n " ,U16_Check_SysconReg_Data);
	}
	else
	{
		printf("SysconReg寄存器读取出错\r\n");
		B_Read_Error = 1;
	}
	
	Uart_Read_HLW8110_Reg(REG_EMUCON_ADDR,2);
	delay_ms(10);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U16_Check_EmuconReg_Data =  (u8_RxBuf[0]<<8) + (u8_RxBuf[1]); 
		printf("写入的EmuconReg寄存器:%lx\n " ,U16_Check_EmuconReg_Data);
	}
	else
	{
		printf("EmuconReg寄存器读取出错\r\n");
		B_Read_Error = 1;
	}
	
	
	Uart_Read_HLW8110_Reg(REG_EMUCON2_ADDR,2);
		delay_ms(10);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U16_Check_Emucon2Reg_Data =  (u8_RxBuf[0]<<8) + (u8_RxBuf[1]); 
		printf("写入的Emucon2Reg寄存器寄存器:%lx\n " ,U16_Check_Emucon2Reg_Data);
	}
	else
	{
		printf("Emucon2Reg寄存器读取出错\r\n");
		B_Read_Error = 1;
	}
	
}



/*=========================================================================================================
 * Function : void Read_HLW8112_IA(void)
 * Describe : 读取A通道电流
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/03
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Read_HLW8110_IA(void)
{	
	float a;
	
	Uart_Read_HLW8110_Reg(REG_RMSIA_ADDR,3);
		delay_ms(10);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U32_RMSIA_RegData = (unsigned long)(u8_RxBuf[0]<<16) + (unsigned long)(u8_RxBuf[1]<<8) + (unsigned long)(u8_RxBuf[2]); 
		//printf("A通道电流寄存器:%lx\n " ,U32_RMSIA_RegData);
	}
	else
	{
		printf("A通道电流寄存器读取出错\r\n");
		B_Read_Error = 1;
	}
	
	
	//A通道电流PGA = 16,电压通道PGA = 1;电流采样电阻1mR,电压采样电阻1M
  //计算公式,U16_AC_I = (U32_RMSIA_RegData * U16_RMSIAC_RegData)/(电流系数* 2^23）
	if ((U32_RMSIA_RegData & 0x800000) == 0x800000)
	 {
			F_AC_I = 0;
	 }
	 else
	 {
		//内存保存的值: U16_RMSIAC_RegData = 0xcb2c = 52012
		//0xcb2c * 8 = 416096
	 	unsigned int RMSIAC = U16_RMSIAC_RegData * 8;
		
		a = (float)U32_RMSIA_RegData;
		a = a * RMSIAC;
		a  = a/0x800000;                     //电流计算出来的浮点数单位是mA,比如5003.12 
		//a = a/1;  								// 1 = 电流系数
		a = a/10;  								// 10 = 电流系数
		a = a/1000;              //a= 5003ma,a/1000 = 5.003A,单位转换成A
		a = a * D_CAL_A_I;				//D_CAL_A_I是校正系数，默认是1
		F_AC_I = a;
	 }
}
/*=========================================================================================================
 * Function : void Read_HLW8110_U(void)
 * Describe : 读取电压
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/03
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Read_HLW8110_U(void)
{
	float a;
	
	Uart_Read_HLW8110_Reg(REG_RMSU_ADDR,3);
		delay_ms(10);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U32_RMSU_RegData = (unsigned long)(u8_RxBuf[0]<<16) + (unsigned long)(u8_RxBuf[1]<<8) + (unsigned long)(u8_RxBuf[2]);
		printf("电压通道寄存器:%lx\n " ,U32_RMSU_RegData);
	}
	else
	{
		printf("电压通道寄存器读取出错\r\n");
		B_Read_Error = 1;
	}
	
		//电压
	//计算:U16_AC_V = (U32_RMSU_RegData * U16_RMSUC_RegData)/2^23
	
	 if ((U32_RMSU_RegData &0x800000) == 0x800000)
	 {
			F_AC_V = 0;
	 }
  else
	{
  a =  (float)U32_RMSU_RegData;
  a = a*U16_RMSUC_RegData;  
  a = a/0x400000;       
  a = a/1;  						// 1 = 电压系数
  a = a/100;    				//计算出a = 22083.12mV,a/100表示220.8312V，电压转换成V
  a = a*D_CAL_U;				//D_CAL_U是校正系数，默认是1,		
  F_AC_V = a;
	}
}
/*=========================================================================================================
 * Function : void Read_HLW8110_PA(void)
 * Describe : 读取A通道功率
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/03
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Read_HLW8110_PA(void)
{
	float a;
	float b;
	
	Uart_Read_HLW8110_Reg(REG_POWER_PA_ADDR,4);
		delay_ms(10);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U32_POWERPA_RegData = (unsigned long)(u8_RxBuf[0]<<24) + (unsigned long)(u8_RxBuf[1]<<16) + (unsigned long)(u8_RxBuf[2]<<8) + (unsigned long)(u8_RxBuf[3]);
		printf("A通道功率寄存器:%lx\n " ,U32_POWERPA_RegData);
	}
	else
	{
		printf("A通道功率寄存器读取出错\r\n");
		B_Read_Error = 1;
	}
	
	
	 if (U32_POWERPA_RegData > 0x80000000)
   {
     b = ~U32_POWERPA_RegData;
     a = (float)b;
   }
   else
     a =  (float)U32_POWERPA_RegData;
     
   
	//功率需要分正功和负功
  //计算,U16_AC_P = (U32_POWERPA_RegData * U16_PowerPAC_RegData)/(2^31*电压系数*电流系数)
	//单位为W,比如算出来5000.123，表示5000.123W
	
    a = a*U16_PowerPAC_RegData;
    a = a/0x80000000;             
    a = a/1;  										// 1 = 电流系数
    a = a/1;  										// 1 = 电压系数
    a = a * D_CAL_A_P;						//D_CAL_A_P是校正系数，默认是1
    F_AC_P = a;									//单位为W,比如算出来5000.123，表示5000.123W

}
/*=========================================================================================================
 * Function : void void Read_HLW8112_EA(void)
 * Describe : 读取A通道有功电量
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/03
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Read_HLW8110_EA(void)
{
	float a;
	Uart_Read_HLW8110_Reg(REG_ENERGY_PA_ADDR,3); 
	delay_ms(10);
	
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U32_ENERGY_PA_RegData = (unsigned long)(u8_RxBuf[0]<<16) + (unsigned long)(u8_RxBuf[1]<<8) + (unsigned long)(u8_RxBuf[2]);
		printf("A通道有功电量寄存器:%lx\n " ,U32_ENERGY_PA_RegData);
	}
	else
	{
		printf("A通道有功电量寄存器读取出错\r\n");
		B_Read_Error = 1;
	}
	
	Uart_Read_HLW8110_Reg(REG_HFCONST_ADDR,2); 
	delay_ms(10);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		U16_HFConst_RegData = (unsigned int)(u8_RxBuf[0]<<8) + (unsigned int)(u8_RxBuf[1]);
		printf("HFCONST常数 = :%d\n " ,U16_HFConst_RegData);
	}
	else
	{
		printf("HFCONST常数寄存器读取出错\r\n");
		B_Read_Error = 1;
	}

	//电量计算,电量 = (U32_ENERGY_PA_RegData * U16_EnergyAC_RegData * HFCONST) /(K1*K2 * 2^29 * 4096)
	//HFCONST:默认值是0x1000, HFCONST/(2^29 * 4096) = 0x20000000
	a =  (float)U32_ENERGY_PA_RegData;	
  a = a*U16_EnergyAC_RegData;
  a = a/0x20000000;             //电量单位是0.001KWH,比如算出来是2.002,表示2.002KWH    
   a = a/1;  										// 1 = 电流系数
   a = a/1;  										// 1 = 电压系数
   a = a * D_CAL_A_E;     			//D_CAL_A_E是校正系数，默认是1
  F_AC_E = a;
	F_AC_BACKUP_E = F_AC_E;
		
}
/*=========================================================================================================
 * Function : void Read_HLW8110_LineFreq(void)
 * Describe : 读取A通道的线性频率
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/12
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Read_HLW8110_LineFreq(void)
{
	float a;
	unsigned long b;
	Uart_Read_HLW8110_Reg(REG_UFREQ_ADDR,2);
	delay_ms(10);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		b = (unsigned long)(u8_RxBuf[0]<<8) + (unsigned long)(u8_RxBuf[1]);
		printf("A通道线性频率寄存器:%ld\n " ,b);
	}
	else
	{
		printf("A通道线性频率寄存器读取出错\r\n");
		B_Read_Error = 1;
	}
	a = (float)b;
	a = 3579545/(8*a);    
	F_AC_LINE_Freq = a;
}

/*=========================================================================================================
 * Function : void Read_HLW8110_PF(void)
 * Describe : 读取功率因素
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/03/18
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Read_HLW8110_PF(void)
{
  float a;
  unsigned long b;
	
//测量A通道的功率因素，需要发送EA+5A命令
//测量B通道的功率因素，需要发送EA+A5命令	
	
	Uart_Read_HLW8110_Reg(REG_PF_ADDR,3);
	delay_ms(10);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		b = (unsigned long)(u8_RxBuf[0]<<16) + (unsigned long)(u8_RxBuf[1]<<8) + (unsigned long)(u8_RxBuf[2]);
		printf("A通道功率因素寄存器:%ld\n " ,b);
	}
	else
	{
		printf("读取A通道功率因素寄存器出错\r\n");
		B_Read_Error = 1;
	}

  if (b>0x800000)       //为负，容性负载
  {
      a = (float)(0xffffff-b + 1)/0x7fffff;
  }
  else
  {
      a = (float)b/0x7fffff;
  }
  
  if (F_AC_P < 0.3) // 小于0.3W，空载或小功率，PF不准
	  a = 0; 
  

//功率因素*100，最大为100，最小负100
  F_AC_PF = a;
  
}


/*=========================================================================================================
 * Function : void Read_HLW8110_Angle(void)
 * Describe : 读取相位角
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2019/04/12
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Read_HLW8110_Angle(void)
{
	float a;	
	unsigned long b;
	Uart_Read_HLW8110_Reg(REG_ANGLE_ADDR,2);
	delay_ms(10);
	if ( u8_RxBuf[u8_RX_Length-1] == HLW8110_checkSum_Read(u8_RX_Length) )
	{
		b =(unsigned long)(u8_RxBuf[0]<<8) + (unsigned long)(u8_RxBuf[1]);
		printf("A通道线相角寄存器:%ld\n " ,b);
	}
	else
	{
		printf("A通道线相角寄存器出错\r\n");
		B_Read_Error = 1;
	}
	
	if ( F_AC_PF < 55)	//线性频率50HZ
	{
		a = b;
		a = a * 0.0805;
		F_Angle = a;
	}
	else
	{
		//线性频率60HZ
		a = b;
		a = a * 0.0965;
		F_Angle = a;
	}
	
	
		if (F_AC_P < 0.5)		//功率小于0.5时，说明没有负载，相角为0
	{
		F_Angle = 0;
	}
	
	if (F_Angle < 90)
	{
		a = F_Angle;
		printf("电流超前电压:%f\n " ,a);
	}
	else if (F_Angle < 180)
	{
		a = 180-F_Angle;
		printf("电流滞后电压:%f\n " ,a);	
	}
	else if (F_Angle < 360)
	{
		a = 360 - F_Angle;
		printf("电流滞后电压:%f\n " ,a);	
	}
	else
	{
			a = F_Angle -360;
			printf("电流超前电压:%f\n " ,a);	
	}
}

/*=========================================================================================================
 * Function : void Calculate_HLW8110_MeterData(void);
 * Describe : 
 * Input    : none
 * Output   : none
 * Return   : none
 * Record   : 2018/05/10
==========================================================================================================*/
void rfir::module::sensor::HLW8110::Calculate_HLW8110_MeterData(void)
{
	Check_WriteReg_Success();
	
	Read_HLW8110_IA();
	Read_HLW8110_U();
	Read_HLW8110_PA();
	Read_HLW8110_EA();
	
	Read_HLW8110_LineFreq();
	Read_HLW8110_Angle();
	Read_HLW8110_PF();
	
	printf("\r\n");	
	printf("\r\n");	
	//printf("交流测量,uart通讯方式\r\n");
	printf("A通道电流转换系数:%x\n " ,U16_RMSIAC_RegData);
	printf("B通道电流转换系数:%x\n " ,U16_RMSIBC_RegData);
	printf("电压通道转换系数:%x\n " ,U16_RMSUC_RegData);
	printf("A通道功率转换系数:%x\n " ,U16_PowerPAC_RegData);
	printf("B通道功率转换系数:%x\n " ,U16_PowerPBC_RegData);
	printf("视在功率转换系数:%x\n " ,U16_PowerSC_RegData);
	printf("A通道电量转换系数:%x\n " ,U16_EnergyAC_RegData);
	printf("B通道电量转换系数:%x\n " ,U16_EnergyBC_RegData);
	printf("转换系数校验和:%x\n " ,U16_CheckSUM_RegData);
	printf("转换系数计算出的校验和:%x\n " ,U16_CheckSUM_Data);
	
	printf("\r\n");//插入换行
	printf("A通道电流寄存器:%x\n " ,U32_RMSIA_RegData);
	printf("电压寄存器:%x\n " ,U32_RMSU_RegData);
	printf("A通道功率寄存器:%x\n " ,U32_POWERPA_RegData);
	printf("A通道电量寄存器:%x\n " ,U32_ENERGY_PA_RegData);

	printf("\r\n");//插入换行
	printf("F_AC_I = %f A\r\n " ,F_AC_I);						//电流
	printf("F_AC_V = %f V	\n " ,F_AC_V);							//电压
	printf("F_AC_P = %f W	\n " ,F_AC_P);						//功率
	printf("F_AC_BACKUP_E = %f KWH \n " ,F_AC_BACKUP_E);			//电量
	printf("F_AC_LINE_Freq = %f Hz \n " ,F_AC_LINE_Freq);		//市电线性频率	
	printf("F_Angle = %f\n " ,F_Angle);					//L和N的相角
	printf("F_AC_PF = %f\n " ,F_AC_PF);		//功率因素
	
	printf("\r\n");//插入换行
	printf("\r\n");//插入换行
	
	printf("----------------------------------------------\r\n");	
	printf("----------------------------------------------\r\n");			
}




//-------------------------------------


float rfir::module::sensor::HLW8110::Hlw8110_Get_Current(bool realtime)
{
	if (realtime) Read_HLW8110_IA();
    return F_AC_I * 1000;
}

bool rfir::module::sensor::HLW8110::Start() {
    return Init_HLW8110();
}

void rfir::module::sensor::HLW8110::Loop() {
    Check_IA_Switch();
}


void rfir::module::sensor::HLW8110::Check_IA_Switch(bool reset) {
    static unsigned long HLW8110_Check_IA_Switch_last_time = 0;
    static unsigned long HLW8110_Check_IA_Switch_pre_state = false;
    if (reset) HLW8110_Check_IA_Switch_last_time = 0;

    if (millis() - HLW8110_Check_IA_Switch_last_time > IA_Switch_Check_Interval) {
        if (On_IA_Switch) {
            bool isOn = IsIAOn();
            if (isOn != HLW8110_Check_IA_Switch_pre_state) {
                delay_ms(IA_Switch_elapse);
                isOn = IsIAOn();
                if (isOn != HLW8110_Check_IA_Switch_pre_state) {                                    
                    On_IA_Switch(IA_Switch_CB_Arg, isOn);
                    HLW8110_Check_IA_Switch_pre_state = isOn;
                }
            }
        }

        HLW8110_Check_IA_Switch_last_time = millis();
    }
}


bool rfir::module::sensor::HLW8110::IsIAOn() {
    return Hlw8110_Get_Current() >= IA_Switch_Point;   
}