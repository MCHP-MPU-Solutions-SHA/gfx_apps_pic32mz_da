/*******************************************************************************
 System Tasks File

  File Name:
    tasks.c

  Summary:
    This file contains source code necessary to maintain system's polled tasks.

  Description:
    This file contains source code necessary to maintain system's polled tasks.
    It implements the "SYS_Tasks" function that calls the individual "Tasks"
    functions for all polled MPLAB Harmony modules in the system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    polled in the system.  These handles are passed into the individual module
    "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"


// *****************************************************************************
// *****************************************************************************
// Section: RTOS "Tasks" Routine
// *****************************************************************************
// *****************************************************************************
void _GLCD_Tasks(  void *pvParameters  )
{
    while(1)
    {
        DRV_GLCD_Update();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void _SYS_INPUT_Tasks(  void *pvParameters  )
{
    while(1)
    {
        SYS_INP_Tasks();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void _SYS_FS_Tasks(  void *pvParameters  )
{
    while(1)
    {
        SYS_FS_Tasks();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void _DRV_SDMMC0_Tasks(  void *pvParameters  )
{
    while(1)
    {
        DRV_SDMMC_Tasks(sysObj.drvSDMMC0);
        vTaskDelay(DRV_SDMMC_RTOS_DELAY_IDX0 / portTICK_PERIOD_MS);
    }
}

void _DRV_MAXTOUCH_Tasks(  void *pvParameters  )
{
    while(1)
    {
        DRV_MAXTOUCH_Tasks(sysObj.drvMAXTOUCH);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

/* Handle for the APP_GLCD_WVGA_Tasks. */
TaskHandle_t xAPP_GLCD_WVGA_Tasks;

void _APP_GLCD_WVGA_Tasks(  void *pvParameters  )
{   
    while(1)
    {
        APP_GLCD_WVGA_Tasks();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}




// *****************************************************************************
// *****************************************************************************
// Section: System "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/
void SYS_Tasks ( void )
{
    /* Maintain system services */
    
    xTaskCreate( _SYS_FS_Tasks,
        "SYS_FS_TASKS",
        SYS_FS_STACK_SIZE,
        (void*)NULL,
        SYS_FS_PRIORITY,
        (TaskHandle_t*)NULL
    );

    xTaskCreate( _DRV_SDMMC0_Tasks,
        "DRV_SDMMC0_Tasks",
        DRV_SDMMC_STACK_SIZE_IDX0,
        (void*)NULL,
        DRV_SDMMC_PRIORITY_IDX0,
        (TaskHandle_t*)NULL
    );





    /* Maintain Device Drivers */
        xTaskCreate( _GLCD_Tasks,
        "GLCD_Tasks",
        1024,
        (void*)NULL,
        1,
        (TaskHandle_t*)NULL
    );


    xTaskCreate( _DRV_MAXTOUCH_Tasks,
        "DRV_MAXTOUCH_Tasks",
        1024,
        (void*)NULL,
        1,
        (TaskHandle_t*)NULL
    );



    /* Maintain Middleware & Other Libraries */
    
    xTaskCreate( _SYS_INPUT_Tasks,
        "SYS_INPUT_Tasks",
        1024,
        (void*)NULL,
        1,
        (TaskHandle_t*)NULL
    );



    /* Maintain the application's state machine. */
        /* Create OS Thread for APP_GLCD_WVGA_Tasks. */
    xTaskCreate((TaskFunction_t) _APP_GLCD_WVGA_Tasks,
                "APP_GLCD_WVGA_Tasks",
                1024,
                NULL,
                1,
                &xAPP_GLCD_WVGA_Tasks);




    /* Start RTOS Scheduler. */
    
     /**********************************************************************
     * Create all Threads for APP Tasks before starting FreeRTOS Scheduler *
     ***********************************************************************/
    vTaskStartScheduler(); /* This function never returns. */

}

/*******************************************************************************
 End of File
 */

