/*==============================================================================
Event Parameter Example
Copyright (c), Firelight Technologies Pty, Ltd 2012-2021.

This example demonstrates how to control event playback using game parameters.
==============================================================================*/
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "common.h"
#include <string>
#include <cstring>
#include <iostream>




int FMOD_Main()
{
    void *extraDriverData = NULL;
    Common_Init(&extraDriverData);

    FMOD::Studio::System* system = NULL;
    ERRCHECK( FMOD::Studio::System::create(&system) );
    ERRCHECK( system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData) );

    FMOD::Studio::Bank* masterBank = NULL;
    ERRCHECK( system->loadBankFile(Common_MediaPath("Master.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank) );

    FMOD::Studio::Bank* stringsBank = NULL;
    ERRCHECK( system->loadBankFile(Common_MediaPath("Master.strings.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank) );

    FMOD::Studio::Bank* sfxBank = NULL;
    ERRCHECK( system->loadBankFile(Common_MediaPath("SFX.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &sfxBank) );

    // -----------------    DESERT

    FMOD::Studio::EventDescription* eventDescDesert = NULL;
    ERRCHECK( system->getEvent("event:/Desert Leitmotif", &eventDescDesert) );

    // Encuentra el parametro del evento según su nombre y crea una instancia del evento
    FMOD_STUDIO_PARAMETER_DESCRIPTION paramDesertProgreso;
    ERRCHECK( eventDescDesert->getParameterDescriptionByName("progreso", &paramDesertProgreso) );
    FMOD_STUDIO_PARAMETER_ID desertID = paramDesertProgreso.id;

    FMOD::Studio::EventInstance* eventInstDesert = NULL;
    ERRCHECK( eventDescDesert->createInstance(&eventInstDesert) );

    // El parametro progreso empieza en 0
    float progresoParameterValue = 0.0f;
    ERRCHECK( eventInstDesert->setParameterByID(desertID, progresoParameterValue) );

    // -----------------    JUNGLE

    FMOD::Studio::EventDescription* eventDescJungle = NULL;
    ERRCHECK( system->getEvent("event:/Selva", &eventDescJungle) );

    // Encuentra el parametro del evento según su nombre y crea una instancia del evento
    FMOD_STUDIO_PARAMETER_DESCRIPTION paramJungleHora;
    ERRCHECK( eventDescJungle->getParameterDescriptionByName("hora", &paramJungleHora) );
    FMOD_STUDIO_PARAMETER_ID horaID = paramJungleHora.id;

    FMOD::Studio::EventInstance* eventInstJungle = NULL;
    ERRCHECK( eventDescJungle->createInstance(&eventInstJungle) );

    // El parametro hora empieza en 0
    float horaParameterValue = 0.0f;
    ERRCHECK( eventInstJungle->setParameterByID(horaID, horaParameterValue) );

    // -----------------    ELEVATOR

    FMOD::Studio::EventDescription* eventDescElevator = NULL;
    ERRCHECK( system->getEvent("event:/Ascensor", &eventDescElevator) );

    // Encuentra el parametro del evento según su nombre y crea una instancia del evento
    FMOD_STUDIO_PARAMETER_DESCRIPTION paramElevatorNivel;
    ERRCHECK( eventDescElevator->getParameterDescriptionByName("nivel", &paramElevatorNivel) );
    FMOD_STUDIO_PARAMETER_ID nivelID = paramElevatorNivel.id;

    FMOD::Studio::EventInstance* eventInstElevator = NULL;
    ERRCHECK( eventDescElevator->createInstance(&eventInstElevator) );

    // El parametro nivel empieza en 0
    float nivelParameterValue = 0.0f;
    ERRCHECK( eventInstElevator->setParameterByID(nivelID, nivelParameterValue) );


    // ----------------- VARIABLE
    FMOD::Studio::EventInstance* eventInst = eventInstDesert;
    FMOD_STUDIO_PARAMETER_DESCRIPTION paramEvent = paramDesertProgreso;
    FMOD_STUDIO_PARAMETER_ID paramID = desertID; 
    float paramValue = progresoParameterValue;
    std::string desertstr = "Desert";
    const char* eventSelected = desertstr.c_str();

    do
    {
        Common_Update();

        if (Common_BtnPress(BTN_MORE))
        {
            ERRCHECK( eventInst->start() );
        }

        if (Common_BtnDown(BTN_ACTION1))
        {
            paramValue = Common_Max(paramEvent.minimum, paramValue - 0.1f);
            ERRCHECK( eventInst->setParameterByID(paramID, paramValue) );
        }

        if (Common_BtnDown(BTN_ACTION2))
        {
            paramValue = Common_Min(paramValue + 0.1f, paramEvent.maximum);
            ERRCHECK( eventInst->setParameterByID(paramID, paramValue) );
        }

        if (Common_BtnPress(BTN_ACTION3))
        {
            ERRCHECK(eventInst->stop(FMOD_STUDIO_STOP_IMMEDIATE));
        }

        if (Common_BtnPress(BTN_LEFT)) // DESERT
        {
            ERRCHECK(eventInst->stop(FMOD_STUDIO_STOP_IMMEDIATE));
            std::string str = "Desert";
            eventSelected = str.c_str();
            eventInst = eventInstDesert;
            paramEvent = paramDesertProgreso;
            paramID = desertID;
            paramValue = progresoParameterValue;
        }

        if (Common_BtnPress(BTN_RIGHT)) // JUNGLE
        {
            ERRCHECK(eventInst->stop(FMOD_STUDIO_STOP_IMMEDIATE));
            std::string str = "Jungle";
            eventSelected = str.c_str();
            eventInst = eventInstJungle;
            paramEvent = paramJungleHora;
            paramID = horaID;
            paramValue = horaParameterValue;
        }

        if (Common_BtnPress(BTN_UP)) // ELEVATOR
        {
            ERRCHECK(eventInst->stop(FMOD_STUDIO_STOP_IMMEDIATE));
            std::string str = "Elevator";
            eventSelected = str.c_str();
            eventInst = eventInstElevator;
            paramEvent = paramElevatorNivel;
            paramID = nivelID;
            paramValue = nivelParameterValue;
        }

        ERRCHECK( system->update() );

        float userValue = 0.0f;
        float finalValue = 0.0f;
        ERRCHECK( eventInst->getParameterByID(paramID, &userValue, &finalValue) );

        Common_Draw("==================================================");
        Common_Draw("Event Parameter Example.");
        Common_Draw("Copyright (c) Firelight Technologies 2012-2021.");
        Common_Draw("==================================================");
        Common_Draw("");
        Common_Draw("SELECTED: %s", eventSelected);
        Common_Draw("Parameter = (user: %1.1f, final: %1.1f)", userValue, finalValue);
        Common_Draw("");
        Common_Draw("Parameter:");
        Common_Draw("Press %s to play event", Common_BtnStr(BTN_MORE));
        Common_Draw("Press %s to decrease value", Common_BtnStr(BTN_ACTION1));
        Common_Draw("Press %s to increase value", Common_BtnStr(BTN_ACTION2));
        Common_Draw("Press %s to pause", Common_BtnStr(BTN_ACTION3));
        Common_Draw("");
        Common_Draw("Event:");
        Common_Draw("Press %s to select Desert Event", Common_BtnStr(BTN_LEFT));
        Common_Draw("Press %s to select Jungle Event", Common_BtnStr(BTN_RIGHT));
        Common_Draw("Press %s to select Elevator Event", Common_BtnStr(BTN_UP));
        Common_Draw("");
        Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));

        Common_Sleep(50);
    } while (!Common_BtnPress(BTN_QUIT));

    ERRCHECK( system->release() );

    Common_Close();

    return 0;
}
