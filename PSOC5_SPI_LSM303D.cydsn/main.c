#include <device.h>
#include <stdlib.h>

//operation codes for Control registers
#define RCR 0x80    //Read Control Register
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


void main()
{
    uint8 i = 0u;
    uint8 value = 0;
    uint8 low, high;
    uint16 two_c = 0;
    
    uint8 low2, high2;
    uint16 two_c2 = 0;
    
    int inc = 0;
    int dir = 1;
    char OutputString[7];
    char OutputString2[7];
    uint8 ch;           /* Data sent on the serial port */
    
    int sign; 
    int sign2;
    
    int hundreds; 
    int tens; 
    int units; 
    int accX;
    
    uint16 dec_value;
    
    
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
    SPIM_Start();
    SPIM_ClearRxBuffer();
	SPIM_ClearTxBuffer();
	SPIM_ClearFIFO();
    

    CyDelay(15);//Wait for it to finish P-O-S-T


    // ---------- SS #1 ------------
    SS_Write(0);
    CyDelay(15);
    value=ReadControlRegister(RCR,0x0F);
	CyDelay(15);
    
	UART_1_PutString("\r\n");
    UART_1_PutString("#1 is 0x");
    sprintf(OutputString, "%02x", value);
    UART_1_PutString(OutputString);
    UART_1_PutString("\r\n");
    CyDelay(100);
    
    // ---------- SS #2 ------------
    SS_Write(1);
    CyDelay(15);
    value=ReadControlRegister(RCR,0x0F);
    CyDelay(15);
    
    UART_1_PutString("#2 is 0x");
    sprintf(OutputString, "%02x", value);
    UART_1_PutString(OutputString);
    UART_1_PutString("\r\n");
    CyDelay(100);
    
    // ---------- SS #3 ------------
    SS_Write(2);
    CyDelay(15);
    value=ReadControlRegister(RCR,0x0F);
    CyDelay(15);
    
    UART_1_PutString("#3 is 0x");
    sprintf(OutputString, "%02x", value);
    UART_1_PutString(OutputString);
    UART_1_PutString("\r\n");
    CyDelay(100);
    
    
    // ---------- SS #4 ------------
    SS_Write(3);
    CyDelay(15);
    value=ReadControlRegister(RCR,0x0F);
    CyDelay(15);
    
    UART_1_PutString("#4 is 0x");
    sprintf(OutputString, "%02x", value);
    UART_1_PutString(OutputString);
    UART_1_PutString("\r\n");
    CyDelay(100);
    UART_1_PutString("\r\n");
    // ------------------------------
    
    
    CyDelay(1000);
    
    SPIM_ClearRxBuffer();
	SPIM_ClearTxBuffer();
	SPIM_ClearFIFO();
    
    LCD_ClearDisplay();
    
    //Start accel
    SS_Write(0);   // select SS #1
    CyDelay(10);
    WriteControlRegister(WCR, 0x20, 0x67);
    CyDelay(10);
    
    SS_Write(1);   // select SS #2
    CyDelay(10);
    WriteControlRegister(WCR, 0x20, 0x67);
    CyDelay(10);
    
    SS_Write(2);   // select SS #3
    CyDelay(10);
    WriteControlRegister(WCR, 0x20, 0x67);
    CyDelay(10);
    
    SS_Write(3);   // select SS #4
    CyDelay(10);
    WriteControlRegister(WCR, 0x20, 0x67);
    CyDelay(10);
    
    
    while(1u){

        low = 0;
        high = 0;
        two_c = 0;
        strcpy(OutputString, "");

		
	   
        // ACC #1 -----------------------------
        SS_Write(0x00);   // select SS #2
        CyDelay(5);
        LCD_Position(0u,0u);
        LCD_PrintString(" #");
        
		// READ X
		
        low = ReadControlRegister(RCR,LSM303D_OUT_X_L_A);    // Low
        CyDelay(5);
        high = ReadControlRegister(RCR,LSM303D_OUT_X_H_A);    // High
        
        
        
        two_c = (low << 8 | high);
         sign = low >> 7; 

           if(sign != 0)   // negative result, form two's complement 
           { 
              two_c ^= 0xFFFF;  // low = low XOR 0xFF
              two_c ++; 
              sign = 1; 
           } 
            
           //two_c >>= 1;      // make into whole degrees 

        sprintf(OutputString, "%d", two_c);
        UART_1_PutString("1,");
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
        
        two_c = (low << 8 | high);
         sign = low >> 7; 

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
        
        two_c = (low << 8 | high);
         sign = low >> 7; 

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
 
 
		
		
        // ACC #2 -----------------------------
        SS_Write(0x02);   // select SS #3
        CyDelay(5);
        
        LCD_Position(0u,0u);
        
        LCD_PrintString("  #");
                
        low = ReadControlRegister(RCR,LSM303D_OUT_X_L_A);    // Low
        CyDelay(10);
        high = ReadControlRegister(RCR,LSM303D_OUT_X_H_A);    // High
        
        two_c = (low << 8 | high);
         sign = low >> 7; 

           if(sign != 0)   // negative result, form two's complement 
           { 
              two_c ^= 0xFFFF;  // low = low XOR 0xFF
              two_c++; 
              sign = 1; 
           } 
            
           //two_c >>= 1;      // make into whole degrees 

        sprintf(OutputString, "%d", two_c);
        UART_1_PutString("2,");
        if (sign == 1) { UART_1_PutString("-"); }
        if (sign == 0) { UART_1_PutString("+"); }
        UART_1_PutString(OutputString);
        
        low = high = two_c = 0;
        strcpy(OutputString, "");
        
        // READ Y

		low = ReadControlRegister(RCR,LSM303D_OUT_Y_L_A);    // Low
        CyDelay(5);
        high = ReadControlRegister(RCR,LSM303D_OUT_Y_H_A);    // High
        
        two_c = (low << 8 | high);
         sign = low >> 7; 

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
        
        two_c = (low << 8 | high);
         sign = low >> 7; 

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
		
		
        // ACC #3 -----------------------------
        SS_Write(0x03);   // select SS #4
        CyDelay(5);
        LCD_Position(0u,0u);
        
        LCD_PrintString("   #");
                
        low = ReadControlRegister(RCR,LSM303D_OUT_X_L_A);    // Low
        CyDelay(10);
        high = ReadControlRegister(RCR,LSM303D_OUT_X_H_A);    // High
        
        two_c = (low << 8 | high);
         sign = low >> 7; 

           if(sign != 0)   // negative result, form two's complement 
           { 
              two_c ^= 0xFFFF;  // low = low XOR 0xFF
              two_c++; 
              sign = 1; 
           } 
            
           //two_c >>= 1;      // make into whole degrees 

        sprintf(OutputString, "%d", two_c);
        UART_1_PutString("3,");
        if (sign == 1) { UART_1_PutString("-"); }
        if (sign == 0) { UART_1_PutString("+"); }
        UART_1_PutString(OutputString);
        
        low = high = two_c = 0;
		
		// READ Y

		low = ReadControlRegister(RCR,LSM303D_OUT_Y_L_A);    // Low
        CyDelay(5);
        high = ReadControlRegister(RCR,LSM303D_OUT_Y_H_A);    // High
        
        two_c = (low << 8 | high);
         sign = low >> 7; 

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
        
        two_c = (low << 8 | high);
         sign = low >> 7; 

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
	
		
        
        // ACC #4 -----------------------------
        SS_Write(0x00);   // select SS #1
        CyDelay(5);
        
        LCD_Position(0u,0u);
        LCD_PrintString("#");
        
        low = ReadControlRegister(RCR,LSM303D_OUT_X_L_A);    // Low
        CyDelay(5);
        high = ReadControlRegister(RCR,LSM303D_OUT_X_H_A);    // High
        
        two_c = (low << 8 | high);
        //two_c = (high << 8 | low) >> 4;

        LCD_Position(1u,0u);
        LCD_PrintInt8(low);
        LCD_Position(1u,2u);
        LCD_PrintInt8(high);

        sign = low >> 7; 

           if(sign != 0)   // negative result, form two's complement 
           { 
              two_c ^= 0xFFFF;  // low = low XOR 0xFF
              two_c++; 
              sign = 1; 
           } 
            
           //two_c >>= 1;      // make into whole degrees 
            
        sprintf(OutputString, "%d", two_c);
        UART_1_PutString("4,");
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
        
        two_c = (low << 8 | high);
         sign = low >> 7; 

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
        
        two_c = (low << 8 | high);
         sign = low >> 7; 

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
      
}    


// This function reads data from Control Register
// Depending on register set bank before
uint8 ReadControlRegister(uint8 opcode, uint8 address)
   {   
      uint8 controlreg;    
    
      SPIM_WriteByte(opcode | address );   
         while(!(SPIM_ReadStatus() & SPIM_STS_TX_FIFO_EMPTY));
 
         
      SPIM_WriteByte(0x00);      //dummy write to clock out data
        while(!(SPIM_ReadStatus() & SPIM_STS_SPI_DONE));

                           
      controlreg=SPIM_ReadByte();   //dummy read
      controlreg=SPIM_ReadByte(); // real data

      return controlreg;
   }//End of RCR
   
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


/* [] END OF FILE */
