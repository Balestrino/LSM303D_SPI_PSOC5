#include <device.h>
#include <stdlib.h>

//operation codes for Control registers
#define RCR 0x80    //Read Control Register
#define WCR 0x00   //Write Control Register

#define ADDR_MASK        0x80

uint8 ReadControlRegister(uint8 opcode, uint8 address);
void WriteControlRegister(uint8 opcode,uint8 address,uint8 dta);


void main()
{
    uint8 i = 0u;
    uint8 value = 0;
    int inc = 0;
    int dir = 1;
    char OutputString[7];
    
    /* Software buffers use internal interrupt functionality to interact with
    * hardware buffers. Thus global interrupt enable command should be called 
    */
    CyGlobalIntEnable;
    
    /* We need to start Character LCD, SPI Master and Slave components */
    LCD_Start();
    SPIM_Start();
    SPIM_ClearRxBuffer();
	SPIM_ClearTxBuffer();
	SPIM_ClearFIFO();
    
    CyDelay(15);//Wait for it to finish P-O-S-T

    value=ReadControlRegister(RCR,0x0F);
   
	CyDelay(10);
    
	LCD_Position(0u,0u);
    LCD_PrintString("Device is: 0x");
    LCD_PrintHexUint8(value);
    
    LCD_Position(1u,0u);
    LCD_PrintString("test passed..");
    
    //LCD_PrintString(OutputString);
    
    CyDelay(3000);
    
    SPIM_ClearRxBuffer();
	SPIM_ClearTxBuffer();
	SPIM_ClearFIFO();
    
    LCD_ClearDisplay();
    
    //Start accel
    WriteControlRegister(WCR, 0x20, 0x67);
    
    while(1u){
          
        if (dir) {inc++;} else {inc--;}
        if (inc == 16) { dir = 0; }
        if (inc == 0) { dir = 1; }
        
        LCD_Position(0u,inc);
        if (dir) LCD_PrintString("x"); else LCD_PrintString(" ");
        
        LCD_Position(1u,0u);
        
        LCD_PrintString("RX:");
        
        value=ReadControlRegister(RCR,0x29);
        LCD_PrintHexUint8(value);

        CyDelay(50u);
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

/* [] END OF FILE */
