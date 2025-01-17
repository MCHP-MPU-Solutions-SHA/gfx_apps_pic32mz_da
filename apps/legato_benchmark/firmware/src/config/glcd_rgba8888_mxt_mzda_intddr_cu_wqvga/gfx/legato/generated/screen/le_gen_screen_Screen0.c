#include "gfx/legato/generated/screen/le_gen_screen_Screen0.h"

// screen member widget declarations
static leWidget* root0;

leWidget* Screen0_SplashBackground;
leImageWidget* Screen0_SplashPIC32Logo;
leImageWidget* Screen0_SplashHarmonyLogo;
leImageWidget* Screen0_SplashBar;
leImageWidget* Screen0_SplashMicrochipLogo;

static leBool initialized = LE_FALSE;
static leBool showing = LE_FALSE;

leResult screenInit_Screen0(void)
{
    if(initialized == LE_TRUE)
        return LE_FAILURE;

    initialized = LE_TRUE;

    return LE_SUCCESS;
}

leResult screenShow_Screen0(void)
{
    if(showing == LE_TRUE)
        return LE_FAILURE;

    // layer 0
    root0 = leWidget_New();
    root0->fn->setSize(root0, 480, 272);
    root0->fn->setBackgroundType(root0, LE_WIDGET_BACKGROUND_NONE);
    root0->fn->setMargins(root0, 0, 0, 0, 0);
    root0->flags |= LE_WIDGET_IGNOREEVENTS;
    root0->flags |= LE_WIDGET_IGNOREPICK;

    Screen0_SplashBackground = leWidget_New();
    Screen0_SplashBackground->fn->setPosition(Screen0_SplashBackground, 0, 0);
    Screen0_SplashBackground->fn->setSize(Screen0_SplashBackground, 480, 272);
    Screen0_SplashBackground->fn->setScheme(Screen0_SplashBackground, &WhiteFillScheme);
    root0->fn->addChild(root0, (leWidget*)Screen0_SplashBackground);

    Screen0_SplashPIC32Logo = leImageWidget_New();
    Screen0_SplashPIC32Logo->fn->setPosition(Screen0_SplashPIC32Logo, 120, 40);
    Screen0_SplashPIC32Logo->fn->setSize(Screen0_SplashPIC32Logo, 240, 139);
    Screen0_SplashPIC32Logo->fn->setBackgroundType(Screen0_SplashPIC32Logo, LE_WIDGET_BACKGROUND_NONE);
    Screen0_SplashPIC32Logo->fn->setBorderType(Screen0_SplashPIC32Logo, LE_WIDGET_BORDER_NONE);
    Screen0_SplashPIC32Logo->fn->setImage(Screen0_SplashPIC32Logo, (leImage*)&PIC32Logo);
    root0->fn->addChild(root0, (leWidget*)Screen0_SplashPIC32Logo);

    Screen0_SplashHarmonyLogo = leImageWidget_New();
    Screen0_SplashHarmonyLogo->fn->setPosition(Screen0_SplashHarmonyLogo, 120, 40);
    Screen0_SplashHarmonyLogo->fn->setSize(Screen0_SplashHarmonyLogo, 240, 139);
    Screen0_SplashHarmonyLogo->fn->setVisible(Screen0_SplashHarmonyLogo, LE_FALSE);
    Screen0_SplashHarmonyLogo->fn->setScheme(Screen0_SplashHarmonyLogo, &WhiteBackgroundScheme);
    Screen0_SplashHarmonyLogo->fn->setBorderType(Screen0_SplashHarmonyLogo, LE_WIDGET_BORDER_NONE);
    Screen0_SplashHarmonyLogo->fn->setImage(Screen0_SplashHarmonyLogo, (leImage*)&PNG_HarmonyLogo);
    root0->fn->addChild(root0, (leWidget*)Screen0_SplashHarmonyLogo);

    Screen0_SplashBar = leImageWidget_New();
    Screen0_SplashBar->fn->setPosition(Screen0_SplashBar, 480, 207);
    Screen0_SplashBar->fn->setSize(Screen0_SplashBar, 480, 65);
    Screen0_SplashBar->fn->setBackgroundType(Screen0_SplashBar, LE_WIDGET_BACKGROUND_NONE);
    Screen0_SplashBar->fn->setBorderType(Screen0_SplashBar, LE_WIDGET_BORDER_NONE);
    Screen0_SplashBar->fn->setImage(Screen0_SplashBar, (leImage*)&Bar);
    root0->fn->addChild(root0, (leWidget*)Screen0_SplashBar);

    Screen0_SplashMicrochipLogo = leImageWidget_New();
    Screen0_SplashMicrochipLogo->fn->setPosition(Screen0_SplashMicrochipLogo, 17, 223);
    Screen0_SplashMicrochipLogo->fn->setSize(Screen0_SplashMicrochipLogo, 144, 39);
    Screen0_SplashMicrochipLogo->fn->setVisible(Screen0_SplashMicrochipLogo, LE_FALSE);
    Screen0_SplashMicrochipLogo->fn->setBackgroundType(Screen0_SplashMicrochipLogo, LE_WIDGET_BACKGROUND_NONE);
    Screen0_SplashMicrochipLogo->fn->setBorderType(Screen0_SplashMicrochipLogo, LE_WIDGET_BORDER_NONE);
    Screen0_SplashMicrochipLogo->fn->setImage(Screen0_SplashMicrochipLogo, (leImage*)&MicrochipLogo);
    root0->fn->addChild(root0, (leWidget*)Screen0_SplashMicrochipLogo);

    leAddRootWidget(root0, 0);
    leSetLayerColorMode(0, LE_COLOR_MODE_RGBA_8888);

    Screen0_OnShow(); // raise event

    showing = LE_TRUE;

    return LE_SUCCESS;
}

void screenUpdate_Screen0(void)
{
    root0->fn->setSize(root0, root0->rect.width, root0->rect.height);

    Screen0_OnUpdate(); // raise event
}

void screenHide_Screen0(void)
{
    Screen0_OnHide(); // raise event


    leRemoveRootWidget(root0, 0);
    leWidget_Delete(root0);
    root0 = NULL;

    Screen0_SplashBackground = NULL;
    Screen0_SplashPIC32Logo = NULL;
    Screen0_SplashHarmonyLogo = NULL;
    Screen0_SplashBar = NULL;
    Screen0_SplashMicrochipLogo = NULL;


    showing = LE_FALSE;
}

void screenDestroy_Screen0(void)
{
    if(initialized == LE_FALSE)
        return;

    initialized = LE_FALSE;
}

leWidget* screenGetRoot_Screen0(uint32_t lyrIdx)
{
    if(lyrIdx >= LE_LAYER_COUNT)
        return NULL;

    switch(lyrIdx)
    {
        case 0:
        {
            return root0;
        }
        default:
        {
            return NULL;
        }
    }
}

