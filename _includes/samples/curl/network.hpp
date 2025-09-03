#pragma once

#ifdef __PSP__

#include <pspsdk.h>
#include <pspnet_inet.h>
#include <psputility.h>
#include <pspnet.h>
#include <pspnet_apctl.h>
#include <pspnet_resolver.h>
#include <arpa/inet.h>

#include <string.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#define RESOLVE_NAME "google.com"

extern "C"
{
    char *basename(const char *filename)
    {
        char *p = strrchr(filename, '/');
        return p ? p + 1 : (char *)filename;
    }
}

int netDialog(SDL_Renderer *renderer)
{
    int done = 0;

    pspUtilityNetconfData data;

    memset(&data, 0, sizeof(data));
    data.base.size = sizeof(data);
    data.base.language = PSP_SYSTEMPARAM_LANGUAGE_ENGLISH;
    data.base.buttonSwap = PSP_UTILITY_ACCEPT_CROSS;
    data.base.graphicsThread = 17;
    data.base.accessThread = 19;
    data.base.fontThread = 18;
    data.base.soundThread = 16;
    data.action = PSP_NETCONF_ACTION_CONNECTAP;

    struct pspUtilityNetconfAdhoc adhocparam;
    memset(&adhocparam, 0, sizeof(adhocparam));
    data.adhocparam = &adhocparam;

    sceUtilityNetconfInitStart(&data);

    while (!done)
    {
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        switch (sceUtilityNetconfGetStatus())
        {
        case PSP_UTILITY_DIALOG_NONE:
            break;

        case PSP_UTILITY_DIALOG_VISIBLE:
            sceUtilityNetconfUpdate(1);
            break;

        case PSP_UTILITY_DIALOG_QUIT:
            printf("PSP_UTILITY_DIALOG_QUIT");
            sceUtilityNetconfShutdownStart();
            break;

        case PSP_UTILITY_DIALOG_FINISHED:
            done = 1;
            break;

        default:
            break;
        }

        SDL_Delay(10);
    }

    return 1;
}

void netInit(void)
{
    sceNetInit(128 * 1024, 42, 4 * 1024, 42, 4 * 1024);
    sceNetInetInit();
    sceNetApctlInit(0x8000, 48);
}

void startNetworking(SDL_Renderer *renderer)
{
    int rc = sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
    if (rc < 0)
        printf("net common didn't load.\n");
    rc = sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
    if (rc < 0)
        printf("inet didn't load.\n");

    netInit();
    netDialog(renderer);
}

void netTerm(void)
{
    sceNetApctlTerm();
    sceNetInetTerm();
    sceNetTerm();
}

void stopNetworking()
{
    sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
    sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
    netTerm();
}

#else
#define stopNetworking()
#define startNetworking(x)
#endif
