// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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

#include <stdlib.h>

#include "gfx/driver/gfx_driver.h"
#include "gfx/canvas/gfx_canvas_api.h"
#include "gfx/canvas/gfx_canvas.h"

#include "definitions.h"


/*******************************************************************************
  GFX Canvas

  Company:
    Microchip Technology Inc.

  File Name:
    gfx_canvas.c

  Summary:
    GFX Canvas Virtual Display Component private header file

  Description:
    The GFX Canvas provides a virtual display driver interface. 

  Remarks:
    None
*******************************************************************************/

typedef enum 
{
    GFXC_INIT,
    GFXC_RUNNING,
} GFXC_STATE;

//LE Driver interface function prototypes
static void GFXC_Update(void);
static gfxResult GFXC_BlitBuffer(int32_t x, int32_t y, gfxPixelBuffer* buf);
static gfxDriverIOCTLResponse GFX_CANVAS_IOCTL(gfxDriverIOCTLRequest request, void* arg);

GFXC_CANVAS canvas[CONFIG_NUM_CANVAS_OBJ];
unsigned int numLayers = CONFIG_NUM_LAYERS;

static unsigned int activeCanvasID = 0;
static GFXC_STATE gfxcState;

static unsigned int effectsIntervalMS = CONFIG_FX_INTERVAL_MS;
static SYS_TIME_HANDLE effectsTimer;
static volatile unsigned int effectsTick = 0;
static unsigned int oldEffectsTick = 0;

static unsigned int displayWidth = 0;
static unsigned int displayHeight = 0;


const gfxGraphicsProcessor * gfxcGPU = NULL;

extern const gfxDisplayDriver glcdDisplayDriver;
const gfxDisplayDriver * gfxDispCtrlr = &glcdDisplayDriver;

const gfxDisplayDriver gfxDriverInterface =
{
    .update = GFXC_Update,
    .blitBuffer = GFXC_BlitBuffer,
    .ioctl = GFX_CANVAS_IOCTL
};


static void gfxcObjectsInitialize(void)
{
    unsigned int id;

    id = gfxcCreate();
    gfxcSetPixelBuffer(id, 800, 480, GFX_COLOR_MODE_RGBA_8888,
                       (void *) 0xa8000000);
    id = gfxcCreate();
    gfxcSetPixelBuffer(id, 800, 200, GFX_COLOR_MODE_RGBA_8888,
                       (void *) 0xa8465000);
    id = gfxcCreate();
    gfxcSetPixelBuffer(id, 480, 272, GFX_COLOR_MODE_RGB_565,
                       (void *) 0xa8177000);
    id = gfxcCreate();
    gfxcSetPixelBuffer(id, 800, 480, GFX_COLOR_MODE_RGB_565,
                       (void *) 0xa85dc000);
}

static void effectsTimerCallback ( uintptr_t context )
{
    effectsTick++;
}


void GFX_CANVAS_Initialize(void)
{
    unsigned int i;
    gfxcState = GFXC_INIT;
    
    //Initialize canvas objects
    for (i = 0; i < CONFIG_NUM_CANVAS_OBJ; i++)
    {
        canvas[i].pixelBuffer.mode = CANVAS_DEFAULT_FORMAT;
        canvas[i].id = CANVAS_ID_INVALID;
        canvas[i].layer.id = LAYER_ID_INVALID;
        canvas[i].effects.cb = NULL;
        canvas[i].effects.fade.status = GFXC_FX_IDLE;
        canvas[i].effects.move.status = GFXC_FX_IDLE;
    }

    if (gfxDispCtrlr != NULL)
    {
        gfxIOCTLArg_DisplaySize disp;
        
        gfxDispCtrlr->ioctl(GFX_IOCTL_GET_DISPLAY_SIZE, (gfxIOCTLArg_DisplaySize *) &disp);
        displayWidth = disp.width;
        displayHeight = disp.height;
    }

    gfxcObjectsInitialize();
}

gfxResult GFXC_FrameStart(uint32_t reserved)
{
    return GFX_SUCCESS;
}

gfxResult GFXC_FrameEnd(void)
{
    return GFX_SUCCESS;
}

uint32_t GFXC_GetVSYNCCount(void)
{
    return 0;
}

void GFXC_Update()
{
}

gfxPixelBuffer * GFXC_GetPixelBuffer(int32_t idx)
{
    return &canvas[activeCanvasID].pixelBuffer;
}

gfxResult GFXC_BlitBuffer(int32_t x,
                          int32_t y,
                          gfxPixelBuffer* buf)
{
    void* srcPtr;
    void* destPtr;
    uint32_t row, rowSize;    

    if (gfxcState != GFXC_RUNNING)
        return GFX_FAILURE;

    rowSize = buf->size.width * gfxColorInfoTable[buf->mode].size;

    for(row = 0; row < buf->size.height; row++)
    {
        srcPtr = gfxPixelBufferOffsetGet(buf, 0, row);
        destPtr = gfxPixelBufferOffsetGet(&canvas[activeCanvasID].pixelBuffer,
                                          x, y + row);

        memcpy(destPtr, srcPtr, rowSize);
    }

    return GFX_SUCCESS;
}

gfxResult GFXC_GetLayerState(uint32_t idx, gfxLayerState* st)
{
    st->rect.x = canvas[idx].layer.pos.xpos;
    st->rect.y = canvas[idx].layer.pos.ypos;
    st->rect.width = canvas[idx].layer.size.width;
    st->rect.height = canvas[idx].layer.size.height;
    st->enabled = canvas[idx].active;

    return GFX_SUCCESS;
}

GFXC_RESULT _gfxcCanvasUpdate(unsigned int canvasID)
{
    gfxIOCTLArg_Value argValue;
    gfxIOCTLArg_LayerValue setBaseAddressParm;
    gfxIOCTLArg_LayerValue setLayerEnabledParm;
    gfxIOCTLArg_LayerValue setColorModeParm;
    gfxIOCTLArg_LayerSize setResParm;
    gfxIOCTLArg_LayerPosition setPositionParm;
    gfxIOCTLArg_LayerSize setSizeParm;
    gfxIOCTLArg_LayerValue setAlphaParm;
    
    if (canvasID < CONFIG_NUM_CANVAS_OBJ &&
        canvas[canvasID].layer.id != LAYER_ID_INVALID &&
        gfxDispCtrlr != NULL &&
        gfxDispCtrlr->ioctl != NULL)
    {
        setBaseAddressParm.base.id = canvas[canvasID].layer.id;
        if (canvas[canvasID].pixelBuffer.pixels != NULL)
            setBaseAddressParm.value.v_uint = (unsigned int) canvas[canvasID].pixelBuffer.pixels;
        else
        {
            argValue.value.v_int = canvas[canvasID].layer.id;
            gfxDispCtrlr->ioctl(GFX_IOCTL_SET_ACTIVE_LAYER, (gfxIOCTLArg_Value*) &argValue);
            
            gfxDispCtrlr->ioctl(GFX_IOCTL_GET_FRAMEBUFFER, (gfxIOCTLArg_Value*) &argValue);
            setBaseAddressParm.value.v_uint = argValue.value.v_uint;
        }
                    
        setColorModeParm.base.id = canvas[canvasID].layer.id;
        setColorModeParm.value.v_uint = (unsigned int) canvas[canvasID].pixelBuffer.mode;
        
        setResParm.base.id = canvas[canvasID].layer.id;
        setResParm.width = canvas[canvasID].pixelBuffer.size.width;
        setResParm.height = canvas[canvasID].pixelBuffer.size.height;
        
        setSizeParm.base.id = canvas[canvasID].layer.id;
        setSizeParm.width = canvas[canvasID].layer.size.width;
        setSizeParm.height = canvas[canvasID].layer.size.height; 
        
        setPositionParm.base.id = canvas[canvasID].layer.id;
        setPositionParm.x = canvas[canvasID].layer.pos.xpos;
        setPositionParm.y = canvas[canvasID].layer.pos.ypos;   
        
        setAlphaParm.base.id = canvas[canvasID].layer.id;
        setAlphaParm.value.v_uint = canvas[canvasID].layer.alpha;   

        if (setPositionParm.x < 0)
        {
            //align offsets for 8bpp frames
            if (gfxColorInfoTable[canvas[canvasID].pixelBuffer.mode].size == 1)
                setPositionParm.x = -(abs(setPositionParm.x) & ~0x3);

            setBaseAddressParm.value.v_uint += abs(setPositionParm.x) * 
                    gfxColorInfoTable[canvas[canvasID].pixelBuffer.mode].size; 

            setSizeParm.width += setPositionParm.x;
            setPositionParm.x = 0;
        }

        if (setPositionParm.y < 0)
        {
            setBaseAddressParm.value.v_uint += abs(setPositionParm.y) * 
                    setResParm.width * 
                    gfxColorInfoTable[canvas[canvasID].pixelBuffer.mode].size;

            setSizeParm.height += setPositionParm.y;
            setPositionParm.y = 0;
        }

        if (setPositionParm.x + setSizeParm.width > displayWidth)
            setSizeParm.width = displayWidth - setPositionParm.x;

        if (setPositionParm.y + setSizeParm.height > displayHeight)
            setSizeParm.height = displayHeight - setPositionParm.y;

        //Lock layer and apply layer properties
        gfxDispCtrlr->ioctl(GFX_IOCTL_SET_LAYER_LOCK, (gfxIOCTLArg_LayerValue *) &setBaseAddressParm);

        if (setBaseAddressParm.value.v_uint != NULL)
            gfxDispCtrlr->ioctl(GFX_IOCTL_SET_LAYER_BASE_ADDRESS, (gfxIOCTLArg_LayerValue *) &setBaseAddressParm);
        
        gfxDispCtrlr->ioctl(GFX_IOCTL_SET_LAYER_COLOR_MODE, (gfxIOCTLArg_LayerValue *) &setColorModeParm);
        gfxDispCtrlr->ioctl(GFX_IOCTL_SET_LAYER_SIZE, (gfxIOCTLArg_LayerSize *) &setResParm);
        gfxDispCtrlr->ioctl(GFX_IOCTL_SET_LAYER_POSITION, (void *) &setPositionParm);
        gfxDispCtrlr->ioctl(GFX_IOCTL_SET_LAYER_WINDOW_SIZE, (void *) &setSizeParm);
        
        gfxDispCtrlr->ioctl(GFX_IOCTL_SET_LAYER_ALPHA, (void *) &setAlphaParm);

        setLayerEnabledParm.base.id = canvas[canvasID].layer.id;
        setLayerEnabledParm.value.v_bool = canvas[canvasID].active;
        gfxDispCtrlr->ioctl(GFX_IOCTL_SET_LAYER_ENABLED, (gfxIOCTLArg_LayerValue *) &setLayerEnabledParm);


        //Unlock layer to affect changes
        gfxDispCtrlr->ioctl(GFX_IOCTL_SET_LAYER_UNLOCK, (gfxIOCTLArg_LayerValue *) &setBaseAddressParm);

        return GFX_SUCCESS;
    }
            
    return GFX_FAILURE;
}

GFXC_RESULT _gfxcSetEffectsIntervalMS(unsigned int ms)
{
    effectsIntervalMS = ms;

    SYS_TIME_TimerDestroy(effectsTimer);
    
    effectsTimer = SYS_TIME_CallbackRegisterMS(effectsTimerCallback, 
                           (uintptr_t) NULL,
                           effectsIntervalMS,
                           SYS_TIME_PERIODIC);
    SYS_TIME_TimerStop(effectsTimer);
    
    return GFX_SUCCESS;
}

GFXC_RESULT _gfxcStartEffects(void)
{
    SYS_TIME_TimerStart(effectsTimer);
    
    return GFX_SUCCESS;
}

GFXC_RESULT _gfxcStopEffects(void)
{
    SYS_TIME_TimerStop(effectsTimer);
    
    return GFX_SUCCESS;
}

static GFXC_RESULT gfxcProcessFadeEffect(GFXC_CANVAS * canvas)
{
    GFXC_RESULT retval = GFX_FAILURE;
    
    //Fade out
    if (canvas->effects.fade.startAlpha > canvas->effects.fade.endAlpha)
    {
        if (canvas->layer.alpha > canvas->effects.fade.endAlpha + 
                                  canvas->effects.fade.delta)
        {
            canvas->layer.alpha -= canvas->effects.fade.delta;
            
            retval = GFX_SUCCESS;
        }
        else
        {
            canvas->layer.alpha = canvas->effects.fade.endAlpha;
            canvas->effects.fade.status = GFXC_FX_DONE;
        }
    }
    //Fade in
    else if (canvas->effects.fade.startAlpha < canvas->effects.fade.endAlpha)
    {
        if (canvas->layer.alpha < canvas->effects.fade.endAlpha - 
                                  canvas->effects.fade.delta)
        {
            canvas->layer.alpha += canvas->effects.fade.delta;
            
            retval = GFX_SUCCESS;
        }
        else
        {
            canvas->layer.alpha = canvas->effects.fade.endAlpha;
            canvas->effects.fade.status = GFXC_FX_DONE;
        }
    }
    
    gfxcCanvasUpdate(canvas->id);
    
    if (retval == GFX_FAILURE && canvas->effects.cb != NULL)
        canvas->effects.cb(canvas->id,
                           GFXC_FX_FADE,
                           GFXC_FX_DONE,
                           canvas->effects.parm);            
        

        
    return retval;
}

static GFXC_RESULT gfxcProcessMoveEffect(GFXC_CANVAS * canvas)
{
    GFXC_RESULT retval = GFX_FAILURE;
    int deltaX, deltaY;
    
    switch(canvas->effects.move.type)
    {
        case GFXC_FX_MOVE_ACC:
        {
            deltaX = (canvas->effects.move.startX != canvas->effects.move.endX) ? 
                      abs(canvas->layer.pos.xpos - canvas->effects.move.startX) /
                      canvas->effects.move.delta + 1 : 0;
            deltaY = (canvas->effects.move.startY != canvas->effects.move.endY) ? 
                      abs(canvas->layer.pos.ypos - canvas->effects.move.startY) /
                        canvas->effects.move.delta + 1 : 0;
            break;
        }
        case GFXC_FX_MOVE_DEC:
        {
            deltaX = (canvas->effects.move.startX != canvas->effects.move.endX) ? 
                     abs(canvas->effects.move.endX - canvas->layer.pos.xpos) /
                        canvas->effects.move.delta + 1 : 0;
            deltaY = (canvas->effects.move.startY != canvas->effects.move.endY) ? 
                     abs(canvas->effects.move.endY - canvas->layer.pos.ypos) /
                        canvas->effects.move.delta + 1 : 0;
            break;
        }
        case GFXC_FX_MOVE_FIXED:
        default:
        {
            deltaX = deltaY = canvas->effects.move.delta;
            break;
        }
    }
    
    //Move right
    if (canvas->effects.move.startX < canvas->effects.move.endX)
    {
        if (canvas->layer.pos.xpos < canvas->effects.move.endX -
                                  deltaX)
        {
            canvas->layer.pos.xpos += deltaX;
            retval = GFX_SUCCESS;
        }
        else
        {
            canvas->layer.pos.xpos = canvas->effects.move.endX;
        }
    }
    //Move left
    else if (canvas->effects.move.startX > canvas->effects.move.endX)
    {
        if (canvas->layer.pos.xpos > canvas->effects.move.endX +
                                  deltaX)
        {
            canvas->layer.pos.xpos -= deltaX;
            retval = GFX_SUCCESS;
        }
        else
        {
            canvas->layer.pos.xpos = canvas->effects.move.endX;
        }
    }
    
        //Move down
    if (canvas->effects.move.startY < canvas->effects.move.endY)
    {
        if (canvas->layer.pos.ypos < canvas->effects.move.endY -
                                  deltaY)
        {
            canvas->layer.pos.ypos += deltaY;
            retval = GFX_SUCCESS;
        }
        else
        {
            canvas->layer.pos.ypos = canvas->effects.move.endY;
        }
    }
    //Move up
    else if (canvas->effects.move.startY > canvas->effects.move.endY)
    {
        if (canvas->layer.pos.ypos > canvas->effects.move.endY +
                                  deltaY)
        {
            canvas->layer.pos.ypos -= deltaY;
            retval = GFX_SUCCESS;
        }
        else
        {
            canvas->layer.pos.ypos = canvas->effects.move.endY;
        }
    }
    
    gfxcCanvasUpdate(canvas->id);
    
    if (retval == GFX_FAILURE)
    {
        canvas->effects.move.status = GFXC_FX_DONE;
        
        if (canvas->effects.cb != NULL)
           canvas->effects.cb(canvas->id,
                               GFXC_FX_MOVE,
                               GFXC_FX_DONE,
                               canvas->effects.parm);
    }
    
    return retval;
}

void GFX_CANVAS_Task(void)
{
    switch(gfxcState)
    {
        case GFXC_INIT:
        {
            effectsTimer = SYS_TIME_CallbackRegisterMS(effectsTimerCallback, 
                           (uintptr_t) NULL,
                           effectsIntervalMS,
                           SYS_TIME_PERIODIC);
            SYS_TIME_TimerStop(effectsTimer);
            gfxcState = GFXC_RUNNING;
            break;
        }
        case GFXC_RUNNING:
        {
            unsigned int i;

            //Process effects
            if (oldEffectsTick != effectsTick)
            {
                GFXC_RESULT gres = GFX_FAILURE; 
                
                for (i = 0; i < CONFIG_NUM_CANVAS_OBJ; i++)
                {
                    if (canvas[i].id != CANVAS_ID_INVALID)
                    {
                        GFXC_RESULT res = GFX_FAILURE; 

                        //Process alpha effects
                        switch(canvas[i].effects.fade.status)
                        {
                            case GFXC_FX_START:
                            {
                                canvas[i].effects.fade.status = GFXC_FX_RUN;
                                //no break, start running effect;
                            }
                            case GFXC_FX_RUN:
                            {
                                res = gfxcProcessFadeEffect(&canvas[i]);
                                if (gres == GFX_FAILURE)
                                    gres = res;
                                break;
                            }
                            default:
                                break;
                        }
                        //Process move effects
                        switch(canvas[i].effects.move.status)
                        {
                            case GFXC_FX_START:
                            {
                                canvas[i].effects.move.status = GFXC_FX_RUN;
                                //no break, start running effect;
                            }
                            case GFXC_FX_RUN:
                            {
                                res = gfxcProcessMoveEffect(&canvas[i]);
                                if (gres == GFX_FAILURE)
                                    gres = res;
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
                
                if (gres == GFX_FAILURE)
                    SYS_TIME_TimerStop(effectsTimer);
                
                oldEffectsTick = effectsTick;
            }
            break;
        }
        default:
            break;
    }
}

gfxDriverIOCTLResponse GFX_CANVAS_IOCTL(gfxDriverIOCTLRequest request,
                                      void* arg)
{
    gfxIOCTLArg_Value* val;
    gfxIOCTLArg_DisplaySize* disp;
    gfxIOCTLArg_LayerRect* rect;
    
    switch(request)
    {
        case GFX_IOCTL_GET_BUFFER_COUNT:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_uint = CONFIG_BUFFER_PER_LAYER;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_DISPLAY_SIZE:
        {
            disp = (gfxIOCTLArg_DisplaySize*)arg;
            
            disp->width = canvas[activeCanvasID].pixelBuffer.size.width;
            disp->height = canvas[activeCanvasID].pixelBuffer.size.height;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_LAYER_COUNT:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_uint = CONFIG_NUM_CANVAS_OBJ;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_ACTIVE_LAYER:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_uint = activeCanvasID;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_ACTIVE_LAYER:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            if(val->value.v_uint >= CONFIG_NUM_CANVAS_OBJ)
            {
                return GFX_IOCTL_ERROR_UNKNOWN;
            }
            else
            {
                activeCanvasID = val->value.v_uint;

                return GFX_IOCTL_OK;
            }
        }
        case GFX_IOCTL_GET_LAYER_RECT:
        {
            rect = (gfxIOCTLArg_LayerRect*)arg;
            
            if(rect->base.id >= CONFIG_NUM_CANVAS_OBJ)        
                return GFX_IOCTL_ERROR_UNKNOWN;
            
            rect->x = canvas[rect->base.id].layer.pos.xpos;
            rect->y = canvas[rect->base.id].layer.pos.ypos;
            rect->width = canvas[rect->base.id].layer.size.width;
            rect->height = canvas[rect->base.id].layer.size.height;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_VSYNC_COUNT:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_uint = 0;
            
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_GET_FRAMEBUFFER:
        {
            val = (gfxIOCTLArg_Value*)arg;
            
            val->value.v_pbuffer = &canvas[activeCanvasID].pixelBuffer;
                        
            return GFX_IOCTL_OK;
        }
        case GFX_IOCTL_SET_PALETTE:
        {
            //Global Palette is a controller function, pass it down to the controller
            if (gfxDispCtrlr != NULL)
                return gfxDispCtrlr->ioctl(GFX_IOCTL_SET_PALETTE, (gfxIOCTLArg_Palette*) arg);
            else
                return GFX_FAILURE;
        }
        default:
        {
            if (request >= GFX_IOCTL_LAYER_REQ_START && 
                request < GFX_IOCTL_LAYER_REQ_END)
            {
                return gfxDispCtrlr->ioctl(request, (gfxIOCTLArg_LayerArg*)arg);
            }
        }
    }
    
    return GFX_IOCTL_UNSUPPORTED;
}

/* *****************************************************************************
 End of File
 */
