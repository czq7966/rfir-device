#ifndef __RFIR_MODULE_SENSOR_HLW8110_H__
#define __RFIR_MODULE_SENSOR_HLW8110_H__


#include <Arduino.h>


#define USART Serial

//8112/8110 reg define
#define REG_SYSCON_ADDR         0x00
#define REG_EMUCON_ADDR         0x01
#define REG_HFCONST_ADDR        0x02
#define REG_EMUCON2_ADDR        0x13
#define REG_ANGLE_ADDR        	0x22			//相角寄存器
#define REG_UFREQ_ADDR          0x23     	//市电线性频率地址
#define REG_RMSIA_ADDR          0x24
#define REG_RMSIB_ADDR          0x25
#define REG_RMSU_ADDR           0x26
#define REG_PF_ADDR             0x27
#define REG_ENERGY_PA_ADDR			0x28
#define REG_ENERGY_PB_ADDR			0x29
#define REG_POWER_PA_ADDR       0x2C
#define REG_POWER_PB_ADDR       0x2D



#define REG_OVLVL_ADDR          0x19

#define REG_INT_ADDR          	0x1D
#define REG_IE_ADDR          		0x40
#define REG_IF_ADDR          		0x41
#define REG_RIF_ADDR          	0x42

#define REG_RDATA_ADDR          0x44


#define REG_CHECKSUM_ADDR				0x6f
#define REG_RMS_IAC_ADDR				0x70
#define REG_RMS_IBC_ADDR				0x71
#define REG_RMS_UC_ADDR					0x72
#define REG_POWER_PAC_ADDR			0x73
#define REG_POWER_PBC_ADDR			0x74
#define REG_POWER_SC_ADDR				0x75
#define REG_ENERGY_AC_ADDR			0x76
#define REG_ENERGY_BC_ADDR			0x77


//宏定义
#define D_TIME1_50MS	50

//直流校正系数

//8112A通道或8110通道校正系数
#define D_CAL_U		1000/1000		//电压校正系数
#define D_CAL_A_I	1000/1000		//A通道电流校正系数
#define D_CAL_A_P	1000/1000		//A通道功率校正系数
#define D_CAL_A_E	1000/1000		//A通道电能校正系数


//8112 B通道校正系数
#define D_CAL_B_P	1000/1000		//B通道功率校正系数
#define D_CAL_B_I	1000/1000		//B通道电流校正系数
#define D_CAL_B_E	1000/1000		//B通道电能校正系数



namespace rfir {
    namespace module {
        namespace sensor {
            class HLW8110 {
            public:
                // static uint16_t ImA; //电流互感器电流值, 单位mA
                // static uint16_t ImA_Pre;            
            public:

                /*---------------------------------------------------------------------------------------------------------*/
                /*---------------------------------------------------------------------------------------------------------*/
                union IntData
                {
                    u16  inte;			
                    u8 byte[2];		
                };
                union LongData
                {
                    u32  word;		
                    u16  inte[2];		
                    u8   byte[4];		
                };

                #define         U8_Buf_Max_Length 10

                static unsigned char	u8_TxBuf[U8_Buf_Max_Length]; 
                static unsigned char	u8_RxBuf[U8_Buf_Max_Length];
                static unsigned char	u8_TX_Length;
                static unsigned char	u8_RX_Length;
                static unsigned char	u8_RX_Index;
                //unsigned char	B_ReadReg_Time_EN;			// 串口读取寄存器数据，时间计数器标志位，1--开启计数，0--关闭计数
                //unsigned char	B_Tx_Finish;
                static unsigned char	B_Rx_Finish;
                static unsigned char	B_Rx_Data_ING;					// 接收数据标志位	,		< 1:接收数据中,0:未接收到数据 >
                static unsigned char	B_Read_Error;							// UART读取出据校验和出错,< 1:数据读错，0:数据读取正确 >
                //unsigned char	u8_ReadReg_Index;
                //unsigned char	u8_ReadReg_Time;				// 串口读取寄存器数据的时间
                /*---------------------------------------------------------------------------------------------------------*/
                /*---------------------------------------------------------------------------------------------------------*/

                static unsigned int    U16_TempData;	


                static unsigned int    U16_IFData;
                static unsigned int    U16_RIFData;
                static unsigned int    U16_LineFData;
                static unsigned int    U16_AngleData;
                static unsigned int    U16_PFData;
                static unsigned int 		U16_HFConst_RegData;
                /*---------------------------------------------------------------------------------------------------------*/
                /*---------------------------------------------------------------------------------------------------------*/
                static unsigned int	U16_RMSIAC_RegData; 			// A通道电流转换系数
                static unsigned int	U16_RMSIBC_RegData; 			// B通道电流转换系数
                static unsigned int	U16_RMSUC_RegData; 				// 电压通道转换系数
                static unsigned int	U16_PowerPAC_RegData; 		// A通道功率转换系数
                static unsigned int	U16_PowerPBC_RegData; 		// B通道功率转换系数
                static unsigned int	U16_PowerSC_RegData; 			// 视在功率转换系数,如果选择A通道，则是A通道视在功率转换系数。A和B通道只能二者选其一
                static unsigned int	U16_EnergyAC_RegData; 		// A通道有功电能(量)转换系数 
                static unsigned int	U16_EnergyBC_RegData; 		// A通道有功电能(量)转换系数
                static unsigned int	U16_CheckSUM_RegData; 		// 转换系数的CheckSum
                static unsigned int	U16_CheckSUM_Data; 				// 转换系数计算出来的CheckSum

                static unsigned int	U16_Check_SysconReg_Data; 						
                static unsigned int	U16_Check_EmuconReg_Data; 						
                static unsigned int	U16_Check_Emucon2Reg_Data; 				

                /*---------------------------------------------------------------------------------------------------------*/			
                /*---------------------------------------------------------------------------------------------------------*/
                static unsigned long 	U32_RMSIA_RegData;			// A通道电流有效值寄存器
                static unsigned long 	U32_RMSU_RegData;				// 电压有效值寄存器
                static unsigned long 	U32_POWERPA_RegData;		// A通道功率有效值寄存器
                static unsigned long 	U32_ENERGY_PA_RegData;	// A通道有功电能（量）有效值寄存器


                static unsigned long 	U32_RMSIB_RegData;			// B通道电流有效值寄存器
                static unsigned long	U32_POWERPB_RegData;		// B通道功率有效值寄存器
                static unsigned long 	U32_ENERGY_PB_RegData;	// B通道有功电能（量）有效值寄存器
                /*---------------------------------------------------------------------------------------------------------*/
                /*---------------------------------------------------------------------------------------------------------*/
                static float   F_AC_V;													// 电压有效值
                static float   F_AC_I;													// A通道电流
                static float   F_AC_P;													// A通道有功功率
                static float   F_AC_E;													// A通道有功电能(量)
                static float   F_AC_BACKUP_E;									// A通道电量备份	
                static float   F_AC_PF;												// 功率因素，A通道和B通道只能选其一 
                static float   F_Angle;												// 相角，A通道和B通道只能选其一 

                static float   F_AC_I_B;												// B通道电流有效值
                static float   F_AC_P_B;												// B通道有功功率
                static float   F_AC_E_B;												// B通道有功电能(量)
                static float   F_AC_BACKUP_E_B;								// B通道电量备份	
                static float   F_AC_LINE_Freq;     						// 市电线性频率
                /*---------------------------------------------------------------------------------------------------------*/
                /*---------------------------------------------------------------------------------------------------------*/            
            public:
                static void delay_ms(unsigned long ms);
                static void Start_Send_UartData(unsigned char len);
                static void Start_Recv_UartData(unsigned char len, unsigned long timeout_ms = 100);
                static void Clear_RxBuf(void);
                static unsigned char HLW8110_checkSum_Write(unsigned char u8_Reg_length);
                static unsigned char HLW8110_checkSum_Read(unsigned char u8_Reg_length);
                static void Uart_HLW8110_WriteREG_EN(void);
                static void Uart_HLW8110_WriteREG_DIS(void);
                static void Uart_HLW8110_Set_Channel_A(void);
                static void Uart_Read_HLW8110_Reg(unsigned char ADDR_Reg,unsigned char u8_reg_length);
                static void Uart_Write_HLW8110_Reg(unsigned char ADDR_Reg,unsigned char u8_reg_length,unsigned long u32_data);
                static void Uart_HLW8110_Reset(void);
                static unsigned char Judge_CheckSum_HLW8110_Calfactor(void);
                static void Init_HLW8110(void);
                static void Check_WriteReg_Success(void);
                static void Read_HLW8110_IA(void);
                static void Read_HLW8110_U(void);
                static void Read_HLW8110_PA(void);
                static void Read_HLW8110_EA(void);
                static void Read_HLW8110_LineFreq(void);
                static void Read_HLW8110_PF(void);
                static void Read_HLW8110_Angle(void);
                static void Calculate_HLW8110_MeterData(void);
                static float Hlw8110_Get_Current(void);
            public:
                typedef std::function<void(int arg, bool ison)> OnIASwitch;  
                static int      IA_Switch_CB_Arg;
                static OnIASwitch On_IA_Switch;
                
                static int      IA_Switch_Check_Interval;
                static float    IA_Switch_Point;
                static int      IA_Switch_elapse;


            public:
                static void Start();
                static void Loop();
                static void Check_IA_Switch(bool reset = false);
                static bool IsIAOn();
            };
        }
    }
}



#endif //
