#pragma once
#include <Windows.h>
#include "smPacket.h"
#include "Utils/strings.h"

namespace Events {

    class EragonLair
    {
        enum Stage {
            Stage_Finished,
            Stage_ToBegin,
            Stage_InProgress
        };

        EragonLair();
        virtual ~EragonLair();

    public:
        BOOL MessageBegin();
        BOOL StartEvent();
        BOOL CheckEragon(smCHAR* Monster);

        int getStage();
        int setStage(int stage);
        int setMinBegin(int min);
        int getMinBegin();
    private:
       
    };


}

