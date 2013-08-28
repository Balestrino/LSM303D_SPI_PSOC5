#include <device.h>
#include <stdlib.h>

//operation codes for Control registers
#define RCR 0x80    //Read Control Register 0x80
#define WCR 0x00    //Write Control Register

#define ADDR_MASK        0x80

#define LSM303D_OUT_X_L_A         0x28
#define LSM303D_OUT_X_H_A         0x29
#define LSM303D_OUT_Y_L_A         0x2A
#define LSM303D_OUT_Y_H_A         0x2B
#define LSM303D_OUT_Z_L_A         0x2C
#define LSM303D_OUT_Z_H_A         0x2D

uint8 ReadControlRegister(uint8 opcode, uint8 address);
void WriteControlRegister(uint8 opcode,uint8 address,uint8 dta);
void start_acc(uint8 n);
void read_acc(uint8 n);
void check_lsm(uint8 n);
uint8 Read1Byte();
uint8 Write1Byte(uint8 ucData);

void main()
{
    /* Software buffers use internal interrupt functionality to interact with
    * hardware buffers. Thus global interrupt enable command should be called 
    */
    //isr_1_Start();      /* Initializing the ISR */
    UART_1_Start();     /* Enabling the UART */
    
    UART_1_PutString("START\n");

    LCD_Position(0u,0u);
    LCD_PrintString("START..");

	CyGlobalIntEnable;

    /* We need to start Character LCD, SPI Master and Slave components */
    LCD_Start();
    LCD_ClearDisplay();
    
    SPIM_Start();
    SPIM_ClearRxBuffer();
	SPIM_ClearTxBuffer();
	SPIM_ClearFIFO();
    

    CyDelay(35);//Wait for it to finish P-O-S-T

    // check LSM code response
    // check_lsm(0);
 
   
    //Start accel
    start_acc(0);
    start_acc(1);
    start_acc(2);
    start_acc(3);
    
    start_acc(4);
    start_acc(5);
    start_acc(6);
    

    while(1u){
           
        
        
		read_acc(0);
        //CyDelay(300);
        read_acc(1);
	    //CyDelay(300);
        read_acc(2);
	    //CyDelay(300);
        read_acc(3);
	    //CyDelay(300);
        read_acc(4);
	    //CyDelay(300);
        read_acc(5);
        //CyDelay(300);
        read_acc(6);
        
        //Timer_1_WriteCounter(0);
        //Timer_1_Start();
        //CyDelay(5);
        //uint16 counter = Timer_1_ReadCounter();
        //Timer_1_WriteCounter(0);
        //Timer_1_Stop();
        LCD_Position(0u,0u);
        LCD_PrintInt16(counter);

        CyDelay(1000);
    }   

}
// ########################################################################################
// ########################################################################################
// ########################################################################################
// ########################################################################################



void check_lsm(uint8 n)
{
    int value;
    char OutputString[7];
    
    SS_Write(n);
    CyDelay(15);
    
    
    Write1Byte(0x0F);
	SPIM_ClearRxBuffer();
	value = Read1Byte();
    
    //value = ReadControlRegister(RCR,0x0F);
	CyDelay(15);
    
	UART_1_PutString("\r\n");
    UART_1_PutString("#");
    sprintf(OutputString, "%i", n);
    UART_1_PutString(OutputString);
    UART_1_PutString(" ...");
    
    sprintf(OutputString, "%02x", value);
    UART_1_PutString(OutputString);
    UART_1_PutString("\r\n");
    
    LCD_Position(0u,0u);
    LCD_PrintString(OutputString);
    
    CyDelay(1000);
}

void start_acc(uint8 n)
{
    SS_Write(n);
    CyDelay(10);
    WriteControlRegister(WCR, 0x20, 0x67);
    CyDelay(10);
}



void read_acc (uint8 n)
{
        //uint8 i = 0u;
        uint8 value = 0;
        uint8 low, high;
        uint16 two_c = 0;
        char OutputString[7];
        int sign;
        
        //uint8 low2, high2;
        //uint16 two_c2 = 0;
        
        strcpy(OutputString, "");
        
        SS_Write(n);   // select SS
        CyDelay(5);
        
        LCD_Position(0u,0u);
        LCD_PrintString("#");
        
        low = ReadControlRegister(RCR,LSM303D_OUT_X_L_A);    // Low
        CyDelay(5);
        high = ReadControlRegister(RCR,LSM303D_OUT_X_H_A);    // High
        
        two_c = (high << 8 | low);

        LCD_Position(1u,0u);
        LCD_PrintInt8(high);
        LCD_Position(1u,2u);
        LCD_PrintInt8(low);

        sign = high >> 7; 

           if(sign != 0)   // negative result, form two's complement 
           { 
              two_c ^= 0xFFFF;  // low = low XOR 0xFF
              two_c++; 
              sign = 1; 
           } 
            
           //two_c >>= 1;      // make into whole degrees 
        
        sprintf(OutputString, "%i", n);
        UART_1_PutString("#");
        UART_1_PutString(OutputString);
        UART_1_PutString(",");
        
        sprintf(OutputString, "%d", two_c);
        
        if (sign == 1) { UART_1_PutString("-"); }
        if (sign == 0) { UART_1_PutString("+"); }
        UART_1_PutString(OutputString);
       
        low = 0;
        high = 0;
        two_c = 0;
        strcpy(OutputString, "");

        
        
        // READ Y

		low = ReadControlRegister(RCR,LSM303D_OUT_Y_L_A);    // Low
        CyDelay(5);
        high = ReadControlRegister(RCR,LSM303D_OUT_Y_H_A);    // High
        
        LCD_Position(1u,5u);
        LCD_PrintInt8(high);
        LCD_Position(1u,7u);
        LCD_PrintInt8(low);
        
        two_c = (high << 8 | low);
        sign = high >> 7; 

           if(sign != 0)   // negative result, form two's complement 
           { 
              two_c ^= 0xFFFF;  // low = low XOR 0xFF
              two_c ++; 
              sign = 1; 
           } 
            
           //two_c >>= 1;      // make into whole degrees 

        sprintf(OutputString, "%d", two_c);
        UART_1_PutString(",");
        if (sign == 1) { UART_1_PutString("-"); }
        if (sign == 0) { UART_1_PutString("+"); }
        UART_1_PutString(OutputString);
		
		low = 0;
        high = 0;
        two_c = 0;
        strcpy(OutputString, "");
		
		// --------
		
		
		
		// READ Z
		low = ReadControlRegister(RCR,LSM303D_OUT_Z_L_A);    // Low
        CyDelay(5);
        high = ReadControlRegister(RCR,LSM303D_OUT_Z_H_A);    // High
        
        LCD_Position(1u,10u);
        LCD_PrintInt8(high);
        LCD_Position(1u,12u);
        LCD_PrintInt8(low);
        
        two_c = (high << 8 | low);
        sign = high >> 7; 

           if(sign != 0)   // negative result, form two's complement 
           { 
              two_c ^= 0xFFFF;  // low = low XOR 0xFF
              two_c ++; 
              sign = 1; 
           } 
            
           //two_c >>= 1;      // make into whole degrees 

        sprintf(OutputString, "%d", two_c);
        UART_1_PutString(",");
        if (sign == 1) { UART_1_PutString("-"); }
        if (sign == 0) { UART_1_PutString("+"); }
        UART_1_PutString(OutputString);
		
        UART_1_PutString("\r\n");
        
        low = high = two_c = 0;
        strcpy(OutputString, "");
		
       
        
		UART_1_PutString("\r\n");
}


// This function reads data from Control Register
// Depending on register set bank before
uint8 ReadControlRegister(uint8 opcode, uint8 address)
   {   
      uint8 controlreg;    
    
      SPIM_WriteByte(opcode | address );   
      //SPIM_WriteByte(address ); 
      
         while(!(SPIM_ReadStatus() & SPIM_STS_TX_FIFO_EMPTY));
 
         
      SPIM_WriteByte(0x00);      //dummy write to clock out data
        while(!(SPIM_ReadStatus() & SPIM_STS_SPI_DONE));

                           
      //controlreg=SPIM_ReadByte();   //dummy read
      controlreg=SPIM_ReadByte(); // real data

      return controlreg;
   }
   
void WriteControlRegister(uint8 opcode,uint8 address,uint8 dta){

	SPIM_ClearRxBuffer();
	SPIM_ClearTxBuffer();
	SPIM_ClearFIFO();
	
	SPIM_WriteByte(opcode | address);
	//Send the data now.
	 while(!(SPIM_ReadStatus() & SPIM_STS_TX_FIFO_EMPTY));
	SPIM_WriteByte(dta);
	while(!(SPIM_ReadStatus() & SPIM_STS_TX_FIFO_EMPTY));
}//END of WCR

int fromHexStr(const char *s) {
	char *p;
	long n = 0;
	
	p = (char *)s;
	while(*p!=0) {
		int ch = (int)*p++; 
		if (ch>='0' && ch<='9') { ch -= '0';
		} else if (ch>='A' && ch<='F') { ch -= 'A'-10;
		} else if (ch>='a' && ch<='f') { ch -= 'a'-10;
		} else { ch = 0;
		}
		n = n << 4 | ch;
	}
	return n;
}

uint8 Read1Byte()
{
	SPIM_ClearRxBuffer();
	Write1Byte(0x00);
	return SPIM_ReadByte();
}
 
uint8 Write1Byte(uint8 ucData)
{
	uint8 ucStatus = 0;
	while (!(SPIM_ReadStatus() & SPIM_STS_TX_FIFO_EMPTY))
	{
	};
	SPIM_WriteByte(ucData);
	while (!(SPIM_ReadStatus() & SPIM_STS_SPI_DONE))
	{
	};
}
 

/* [] END OF FILE */
