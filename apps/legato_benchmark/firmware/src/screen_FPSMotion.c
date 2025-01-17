/*******************************************************************************
  MPLAB Harmony Application Splash Screen Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    screen_FPSCMotion.c

  Summary:
    This file contains the source code for the demo FPS motion screen.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************
#include "definitions.h"

#include <stdio.h>
#include <string.h>

#include "system/time/sys_time.h"

#define AVE_FPS_COUNT 10
#define FPS_UPDATE_TIMER_PERIOD_MS 1000
#define DEF_RECT_SIZE 40
#define RECT_PIXSIZE_INC 10
#define MAX_RECT_SIZE 260
#define MIN_RECT_SIZE 10
#define FULLSCREEN_RECT_SIZE 0xffffffff
#define MAX_FILL_SCHEMES_CYCLE 5
#define MOTION_BLACKHOLE_WIDTH  120

typedef enum
{
    RECT_WIDGET_0,
    RECT_WIDGET_1,
    RECT_WIDGET_2,
    RECT_WIDGET_3,
    RECT_WIDGET_4,
    RECT_WIDGET_5,
    RECT_WIDGET_6,
    RECT_WIDGET_7,
    RECT_WIDGET_8,
    RECT_WIDGET_9,
    MOTION_WIDGETS_NUM,
} MOTION_WIDGET_IDX;

typedef struct 
{
    MOTION_WIDGET_IDX idx;
    leWidget* widget;
    int dx;
    int dy;            
} MOTION_WIDGET_T;

MOTION_WIDGET_T motionWidgets[MOTION_WIDGETS_NUM] = 
{
    {RECT_WIDGET_0, NULL, 1, -1},
    {RECT_WIDGET_1, NULL, -1, 1},
    {RECT_WIDGET_2, NULL, 1, -1},
    {RECT_WIDGET_3, NULL, 1, 1},
    {RECT_WIDGET_4, NULL, -1, 1},    
    {RECT_WIDGET_5, NULL, 1, 1},    
    {RECT_WIDGET_6, NULL, 1, -1},    
    {RECT_WIDGET_7, NULL, 1, -1},    
    {RECT_WIDGET_8, NULL, 1, 1},    
    {RECT_WIDGET_9, NULL, -1, -1},    
};

static const leScheme* FillSchemes[MAX_FILL_SCHEMES_CYCLE] = 
{
    &PinkFillScheme,
    &PurpleFillScheme,
    &RedFillScheme,
    &BlueFillScheme,
    &LightRedBackgroundScheme,
};

static volatile leBool aveFPSValid = LE_FALSE;
static int aveCounter = 0;
static uint32_t prevDrawCountAve[AVE_FPS_COUNT] = {0};
static uint32_t prevDrawCount;
static uint32_t prevVsyncCount;

static SYS_TIME_HANDLE fpsUpdateTimer;

static leDynamicString fpsBtnText;
static leDynamicString refreshRateText;
static leDynamicString motionRectsCountText;
static leDynamicString squareSizeSmallStr;
static leTableString squareSizeFullScreenStr;

static uint32_t motionRectsCount;
static uint32_t rectSize;

static char charBuff[16] = {0};

static enum
{
    SCREEN_DO_NOTHING = 0,
    SCREEN_RECT_SIZE_UP,
    SCREEN_RECT_SIZE_DOWN,
    SCREEN_RECT_COUNT_UP,
    SCREEN_RECT_COUNT_DOWN,
    SCREEN_NEXT,
    SCREEN_WAIT_FOR_NEXT,
    SCREEN_MOVE_TO_NEXT,
    SCREEN_DONE
} screenState;

static void resetFPS(void)
{
    aveFPSValid = LE_FALSE;
    aveCounter = 0;
    
    sprintf(charBuff, "---");
    
    fpsBtnText.fn->setFromCStr(&fpsBtnText, charBuff);
    Screen2_MotionUpdateValue->fn->setString(Screen2_MotionUpdateValue, (leString*)&fpsBtnText);
    
    prevDrawCount = 0;
    prevVsyncCount = 0;
}

static void fpsUpdateTimer_Callback()
{
    uint32_t rate;
    uint32_t i;
    gfxIOCTLArg_Value val;
    
    if(Screen2_MotionUpdateValue == NULL)
        return;
    
    //Update the 10-pt rolling average
    prevDrawCountAve[aveCounter] = (leRenderer_GetDrawCount() - prevDrawCount);
    
    //If not pressed, show current FPS
    if(Screen2_MotionUpdateValue->fn->getPressed(Screen2_MotionUpdateValue) == LE_FALSE)
    {
            //Update FPS
        rate = (leRenderer_GetDrawCount() - prevDrawCount)/
               (FPS_UPDATE_TIMER_PERIOD_MS/1000);

        sprintf(charBuff, "%u curr", (unsigned int)rate);
    }
    //If pressed, show average FPS
    else
    {
        for (i = 0, rate = 0; i < AVE_FPS_COUNT; i++)
        {
            rate += prevDrawCountAve[i];
        }

        if (aveFPSValid)
        {
            sprintf(charBuff, "%u avg", 
                   (unsigned int)(rate / ((AVE_FPS_COUNT)*(FPS_UPDATE_TIMER_PERIOD_MS / 1000))));
        }
        else
        {
            sprintf(charBuff, "---");
        }
    }
    
    if (aveCounter < (AVE_FPS_COUNT - 1))
    {
        aveCounter++;
    }
    else
    {
        aveCounter = 0;
        aveFPSValid = LE_TRUE;
    }
    
    fpsBtnText.fn->setFromCStr(&fpsBtnText, charBuff);
    Screen2_MotionUpdateValue->fn->setString(Screen2_MotionUpdateValue, (leString*)&fpsBtnText);
    
    //Update Refresh Rate
    leRenderer_DisplayInterface()->ioctl(GFX_IOCTL_GET_VSYNC_COUNT, (gfxIOCTLArg_Value*) &val); 
    rate =  (val.value.v_uint - prevVsyncCount) / (FPS_UPDATE_TIMER_PERIOD_MS/1000);

    //If vsyncCount does not increase, assume fixed refresh rate using vsyncCount
    //value
    if (rate == 0)
        rate = val.value.v_uint;

    sprintf(charBuff, "%u", (unsigned int)rate);
    
    refreshRateText.fn->setFromCStr(&refreshRateText, charBuff);
    Screen2_MotionRefreshValue->fn->setString(Screen2_MotionRefreshValue,
                                      (leString*)&refreshRateText);
    
    
    prevDrawCount = leRenderer_GetDrawCount();
    prevVsyncCount = val.value.v_uint;
    
//    APP_DecrementCount(Counter1LabelWidget);
}

static void moveWidget(MOTION_WIDGET_IDX id)
{
    leWidget* widget = motionWidgets[id].widget;
    gfxIOCTLArg_DisplaySize size; 
    
    gfxDriverInterface.ioctl(GFX_IOCTL_GET_DISPLAY_SIZE, (gfxIOCTLArg_DisplaySize *) &size); 

    if (((widget->rect.x + rectSize) >= (size.width - MOTION_BLACKHOLE_WIDTH)) ||
        ((widget->rect.x + motionWidgets[id].dx + rectSize) > (size.width - MOTION_BLACKHOLE_WIDTH)))
    {
        motionWidgets[id].dx = -1;
    }
    
    if((widget->rect.x <= 0) || ((widget->rect.x - motionWidgets[id].dx) < 0))
       
    {
        motionWidgets[id].dx = 1;
    }
    
    if (((widget->rect.y + rectSize) >= size.height) ||
        (((widget->rect.y + rectSize) + motionWidgets[id].dy) > size.height))
    {
        motionWidgets[id].dy = -1;
    }
    
    if ((widget->rect.y <= 0) || ((widget->rect.y + motionWidgets[id].dy) < 0))
    {
        motionWidgets[id].dy = 1;
    }
   
    widget->fn->setPosition(widget, 
                            widget->rect.x + motionWidgets[id].dx,
                            widget->rect.y + motionWidgets[id].dy);
}

static void moveRectangles()
{
    uint32_t i;
    
    //If doing full screen fill, just update the widget color
    if (rectSize == FULLSCREEN_RECT_SIZE)
    {
        static uint32_t fillSchemeIndex = 0;

        motionWidgets[0].widget->fn->setPosition(motionWidgets[0].widget, 0, 0);                
        motionWidgets[0].widget->fn->setScheme(motionWidgets[0].widget, FillSchemes[fillSchemeIndex]);

        fillSchemeIndex = (fillSchemeIndex < MAX_FILL_SCHEMES_CYCLE - 1) ?
            fillSchemeIndex + 1 : 0;
    }
    else
    {
        for (i = 0; i < motionRectsCount; i++)
        {
            moveWidget(RECT_WIDGET_0 + i);
        }
    }
}

static void increaseRectSize()
{
    uint32_t i;
    
    if(rectSize == FULLSCREEN_RECT_SIZE)
    {
        rectSize = MIN_RECT_SIZE;
    }
    else
    {
        //If max size is already reached, switch to full screen fill
        if(rectSize < MAX_RECT_SIZE)
        {
            rectSize += RECT_PIXSIZE_INC;
        }
        else
        {
            rectSize = FULLSCREEN_RECT_SIZE;
        }
    }

    if (rectSize == FULLSCREEN_RECT_SIZE)
    {
        gfxIOCTLArg_DisplaySize size; 
    
        gfxDriverInterface.ioctl(GFX_IOCTL_GET_DISPLAY_SIZE, (gfxIOCTLArg_DisplaySize *) &size); 
    
        motionWidgets[RECT_WIDGET_0].widget->fn->setWidth(motionWidgets[RECT_WIDGET_0].widget, size.width);
        motionWidgets[RECT_WIDGET_0].widget->fn->setHeight(motionWidgets[RECT_WIDGET_0].widget, size.height);

        Screen2_MotionRectSizeValue->fn->invalidateContents(Screen2_MotionRectSizeValue);
        Screen2_MotionRectSizeValue->fn->setString(Screen2_MotionRectSizeValue, (leString*)&squareSizeFullScreenStr);
    }
    else
    {
        for (i = 0; i < MOTION_WIDGETS_NUM; i++)
        {
            motionWidgets[RECT_WIDGET_0 + i].widget->fn->setWidth(motionWidgets[RECT_WIDGET_0 + i].widget, rectSize);
            motionWidgets[RECT_WIDGET_0 + i].widget->fn->setHeight(motionWidgets[RECT_WIDGET_0 + i].widget, rectSize);                        
        }

        //Update image size label
        sprintf(charBuff, "%u", (unsigned int)rectSize);

        squareSizeSmallStr.fn->setFromCStr(&squareSizeSmallStr, charBuff);
        Screen2_MotionRectSizeValue->fn->setString(Screen2_MotionRectSizeValue, (leString*)&squareSizeSmallStr);
    }

    resetFPS();
}

static void decreaseRectSize()
{
    uint32_t i;
    
    if(rectSize == FULLSCREEN_RECT_SIZE)
    {
        rectSize = MAX_RECT_SIZE;
    }
    else
    {
        //If min size is already reached, switch to full screen fill
        if(rectSize > MIN_RECT_SIZE)
        {
            rectSize -= RECT_PIXSIZE_INC;
        }   
        else
        {
            rectSize = FULLSCREEN_RECT_SIZE;
        }
    }

    if (rectSize == FULLSCREEN_RECT_SIZE)
    {   
        gfxIOCTLArg_DisplaySize size; 
    
        gfxDriverInterface.ioctl(GFX_IOCTL_GET_DISPLAY_SIZE, (gfxIOCTLArg_DisplaySize *) &size); 
        
        motionWidgets[RECT_WIDGET_0].widget->fn->setWidth(motionWidgets[RECT_WIDGET_0].widget, size.width);
        motionWidgets[RECT_WIDGET_0].widget->fn->setHeight(motionWidgets[RECT_WIDGET_0].widget, size.height);

        Screen2_MotionRectSizeValue->fn->invalidateContents(Screen2_MotionRectSizeValue);
        Screen2_MotionRectSizeValue->fn->setString(Screen2_MotionRectSizeValue, (leString*)&squareSizeFullScreenStr);
    }
    else
    {
        for (i = 0; i < MOTION_WIDGETS_NUM; i++)
        {
            motionWidgets[RECT_WIDGET_0 + i].widget->fn->setWidth(motionWidgets[RECT_WIDGET_0 + i].widget, rectSize);
            motionWidgets[RECT_WIDGET_0 + i].widget->fn->setHeight(motionWidgets[RECT_WIDGET_0 + i].widget, rectSize);                        
        }

        //Update image size label
        sprintf(charBuff, "%u", (unsigned int)rectSize);

        squareSizeSmallStr.fn->setFromCStr(&squareSizeSmallStr, charBuff);
        Screen2_MotionRectSizeValue->fn->setString(Screen2_MotionRectSizeValue, (leString*)&squareSizeSmallStr);
    }

    resetFPS();
}

static void addRect()
{
    uint32_t i;
    
    if(motionRectsCount < MOTION_WIDGETS_NUM)
    {
        motionRectsCount += 1;
    }

    for (i = 0; i < MOTION_WIDGETS_NUM; i++)
    {
        if (i < motionRectsCount)
        {
            motionWidgets[RECT_WIDGET_0 + i].widget->fn->setVisible(motionWidgets[RECT_WIDGET_0 + i].widget, LE_TRUE);
        }
        else
        {
            motionWidgets[RECT_WIDGET_0 + i].widget->fn->setVisible(motionWidgets[RECT_WIDGET_0 + i].widget, LE_FALSE);
        }
    }

    // Update rectangle count label
    sprintf(charBuff, "%u", (unsigned int)motionRectsCount);

    motionRectsCountText.fn->setFromCStr(&motionRectsCountText, charBuff);
    Screen2_MotionRectCount->fn->setString(Screen2_MotionRectCount, (leString*)&motionRectsCountText);

    resetFPS();
}

static void removeRect()
{
    uint32_t i;
    
    if(motionRectsCount > 1)
    {
        motionRectsCount -= 1;
    }
    
    for (i = 0; i < MOTION_WIDGETS_NUM; i++)
    {
        if (i < motionRectsCount)
        {
            motionWidgets[RECT_WIDGET_0 + i].widget->fn->setVisible(motionWidgets[RECT_WIDGET_0 + i].widget, LE_TRUE);
        }
        else
        {
            motionWidgets[RECT_WIDGET_0 + i].widget->fn->setVisible(motionWidgets[RECT_WIDGET_0 + i].widget, LE_FALSE);
        }
    }

    // Update rectangle count label
    sprintf(charBuff, "%u", (unsigned int)motionRectsCount);

    motionRectsCountText.fn->setFromCStr(&motionRectsCountText, charBuff);
    Screen2_MotionRectCount->fn->setString(Screen2_MotionRectCount, (leString*)&motionRectsCountText);

    resetFPS();
}

void Screen2_OnShow()
{
    leFont* font = NULL;
    uint32_t i;
    
    font = leStringTable_GetStringFont(&stringTable, stringID_NumsLittle, 0);
    
    leDynamicString_Constructor(&fpsBtnText);
    fpsBtnText.fn->setFont(&fpsBtnText, font);
    
    leDynamicString_Constructor(&refreshRateText);
    refreshRateText.fn->setFont(&refreshRateText, font);
    
    font = leStringTable_GetStringFont(leGetState()->stringTable, stringID_NumsTiny, 0);
    
    leDynamicString_Constructor(&motionRectsCountText);
    motionRectsCountText.fn->setFont(&motionRectsCountText, font);
    
    leDynamicString_Constructor(&squareSizeSmallStr);
    squareSizeSmallStr.fn->setFont(&squareSizeSmallStr, font);
    
    leTableString_Constructor(&squareSizeFullScreenStr, stringID_Fullscreen);
    
    motionRectsCount = 1;
    rectSize = DEF_RECT_SIZE;

    //Initialize the motion widgets
    motionWidgets[RECT_WIDGET_0].widget = &Screen2_RectMotionWidget1->widget;
    motionWidgets[RECT_WIDGET_1].widget = &Screen2_RectMotionWidget2->widget;
    motionWidgets[RECT_WIDGET_2].widget = &Screen2_RectMotionWidget3->widget;
    motionWidgets[RECT_WIDGET_3].widget = &Screen2_RectMotionWidget4->widget;
    motionWidgets[RECT_WIDGET_4].widget = &Screen2_RectMotionWidget5->widget;
    motionWidgets[RECT_WIDGET_5].widget = &Screen2_RectMotionWidget6->widget;
    motionWidgets[RECT_WIDGET_6].widget = &Screen2_RectMotionWidget7->widget;
    motionWidgets[RECT_WIDGET_7].widget = &Screen2_RectMotionWidget8->widget;
    motionWidgets[RECT_WIDGET_8].widget = &Screen2_RectMotionWidget9->widget;
    motionWidgets[RECT_WIDGET_9].widget = &Screen2_RectMotionWidget10->widget;

    for (i = 0; i < MOTION_WIDGETS_NUM; i++)
    {
        motionWidgets[RECT_WIDGET_0 + i].widget->fn->setWidth(motionWidgets[RECT_WIDGET_0 + i].widget, DEF_RECT_SIZE);
        motionWidgets[RECT_WIDGET_0 + i].widget->fn->setHeight(motionWidgets[RECT_WIDGET_0 + i].widget, DEF_RECT_SIZE);
        
        if(i >= motionRectsCount)
        {
            motionWidgets[RECT_WIDGET_0 + i].widget->fn->setVisible(motionWidgets[RECT_WIDGET_0 + i].widget, LE_FALSE);
        }
    }

    motionWidgets[RECT_WIDGET_0].widget->fn->setVisible(motionWidgets[RECT_WIDGET_0].widget, LE_TRUE);

    sprintf(charBuff, "%u", (unsigned int)motionRectsCount);
    motionRectsCountText.fn->setFromCStr(&motionRectsCountText, charBuff);
    Screen2_MotionRectCount->fn->setString(Screen2_MotionRectCount, (leString*)&motionRectsCountText);

    sprintf(charBuff, "%u", (unsigned int)rectSize);
    squareSizeSmallStr.fn->setFromCStr(&squareSizeSmallStr, charBuff);
    Screen2_MotionRectSizeValue->fn->setString(Screen2_MotionRectSizeValue, (leString*)&squareSizeSmallStr);

    //fpsTmrCtxt.refreshRate = RefreshRateMotionLabelWidget;
    //fpsTmrCtxt.fpsButton = FPSButtonMotion;
    
    resetFPS();
                    
    memset(prevDrawCountAve, 0, sizeof(prevDrawCountAve));
                            
    fpsUpdateTimer = SYS_TIME_CallbackRegisterMS(fpsUpdateTimer_Callback, 
                                                 1,
                                                 FPS_UPDATE_TIMER_PERIOD_MS,
                                                 SYS_TIME_PERIODIC);
    
    screenState = SCREEN_DO_NOTHING;
}

void Screen2_OnHide()
{
    SYS_TIME_TimerDestroy(fpsUpdateTimer);
    
    fpsBtnText.fn->destructor(&fpsBtnText);
    refreshRateText.fn->destructor(&refreshRateText);
    motionRectsCountText.fn->destructor(&motionRectsCountText);
    squareSizeSmallStr.fn->destructor(&squareSizeSmallStr);
    squareSizeFullScreenStr.fn->destructor(&squareSizeFullScreenStr);
}

void Screen2_OnUpdate()
{
    switch (screenState)
    {
        case SCREEN_DO_NOTHING:
        {
            if(leRenderer_IsIdle() &&
               leEvent_GetCount() == 0)
            {            
                moveRectangles();
            }
            
            break;
        }
        case SCREEN_RECT_SIZE_UP:
        {
            if(leRenderer_IsIdle() &&
               leEvent_GetCount() == 0)
            {            
                increaseRectSize();

                screenState = SCREEN_DO_NOTHING;
            }

            break;
        }
        case SCREEN_RECT_SIZE_DOWN:
        {
            if(leRenderer_IsIdle() &&
               leEvent_GetCount() == 0)
            {            
                decreaseRectSize();

                screenState = SCREEN_DO_NOTHING;
            }

            break;                    
        }
        case SCREEN_RECT_COUNT_UP:
        {
            if(leRenderer_IsIdle() &&
               leEvent_GetCount() == 0)
            {            
                addRect();

                screenState = SCREEN_DO_NOTHING;
            }

            break;
        }
        case SCREEN_RECT_COUNT_DOWN:
        {
            if(leRenderer_IsIdle() &&
               leEvent_GetCount() == 0)
            {            
                removeRect();

                screenState = SCREEN_DO_NOTHING;
            }

            break;                    
        }
        case SCREEN_NEXT:
        {
            screenState = SCREEN_WAIT_FOR_NEXT;

            SYS_TIME_TimerStop(fpsUpdateTimer);

            aveFPSValid = LE_FALSE;

            break;                    
        }
        case SCREEN_WAIT_FOR_NEXT:
        {
            if(leRenderer_IsIdle() &&
               leEvent_GetCount() == 0)
            {
                screenState = SCREEN_MOVE_TO_NEXT;
            }

            break;
        }
        case SCREEN_MOVE_TO_NEXT:
        {
            legato_showScreen(screenID_Screen3);

            screenState = SCREEN_DONE;

            break;
        }
        case SCREEN_DONE:
        default:
            break;
    }

    
}

// event handlers
void event_Screen2_MotionPlusButton_OnPressed(leButtonWidget* btn)
{
    if(screenState == SCREEN_DO_NOTHING)
    {
        screenState = SCREEN_RECT_COUNT_UP;
    }
}

void event_Screen2_MotionMinusButton_OnPressed(leButtonWidget* btn)
{
    if(screenState == SCREEN_DO_NOTHING)
    {
        screenState = SCREEN_RECT_COUNT_DOWN;
    }
}

void event_Screen2_MotionNextButton_OnPressed(leButtonWidget* btn)
{
    if(screenState == SCREEN_DO_NOTHING)
    {
        screenState = SCREEN_WAIT_FOR_NEXT;
    }
}

void event_Screen2_MotionSizeDownButton_OnPressed(leButtonWidget* btn)
{
    if(screenState == SCREEN_DO_NOTHING)
    {
        screenState = SCREEN_RECT_SIZE_DOWN;
    }
}

void event_Screen2_MotionSizeUpButton_OnPressed(leButtonWidget* btn)
{
    if(screenState == SCREEN_DO_NOTHING)
    {
        screenState = SCREEN_RECT_SIZE_UP;
    }
}

/*******************************************************************************
 End of File
 */
