/*******************************************************************************
* File Name: SlaveCtrlReg.h  
* Version 1.70
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_SlaveCtrlReg_H) /* CY_CONTROL_REG_SlaveCtrlReg_H */
#define CY_CONTROL_REG_SlaveCtrlReg_H

#include "cytypes.h"


/***************************************
*         Function Prototypes 
***************************************/

void    SlaveCtrlReg_Write(uint8 control) ;
uint8   SlaveCtrlReg_Read(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define SlaveCtrlReg_Control        (* (reg8 *) SlaveCtrlReg_Async_ctrl_reg__CONTROL_REG )
#define SlaveCtrlReg_Control_PTR    (  (reg8 *) SlaveCtrlReg_Async_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_SlaveCtrlReg_H */


/* [] END OF FILE */
