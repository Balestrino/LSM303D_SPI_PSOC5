/*******************************************************************************
* File Name: m_ss_pin.c  
* Version 1.70
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "m_ss_pin.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 m_ss_pin__PORT == 15 && (m_ss_pin__MASK & 0xC0))

/*******************************************************************************
* Function Name: m_ss_pin_Write
********************************************************************************
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  void 
*  
*******************************************************************************/
void m_ss_pin_Write(uint8 value) 
{
    uint8 staticBits = m_ss_pin_DR & ~m_ss_pin_MASK;
    m_ss_pin_DR = staticBits | ((value << m_ss_pin_SHIFT) & m_ss_pin_MASK);
}


/*******************************************************************************
* Function Name: m_ss_pin_SetDriveMode
********************************************************************************
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to this drive mode.
*
* Return: 
*  void
*
*******************************************************************************/
void m_ss_pin_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(m_ss_pin_0, mode);
}


/*******************************************************************************
* Function Name: m_ss_pin_Read
********************************************************************************
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro m_ss_pin_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 m_ss_pin_Read(void) 
{
    return (m_ss_pin_PS & m_ss_pin_MASK) >> m_ss_pin_SHIFT;
}


/*******************************************************************************
* Function Name: m_ss_pin_ReadDataReg
********************************************************************************
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 m_ss_pin_ReadDataReg(void) 
{
    return (m_ss_pin_DR & m_ss_pin_MASK) >> m_ss_pin_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(m_ss_pin_INTSTAT) 

    /*******************************************************************************
    * Function Name: m_ss_pin_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  void 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 m_ss_pin_ClearInterrupt(void) 
    {
        return (m_ss_pin_INTSTAT & m_ss_pin_MASK) >> m_ss_pin_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif
/* [] END OF FILE */ 
