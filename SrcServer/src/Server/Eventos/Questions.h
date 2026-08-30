#pragma once
#include <Windows.h>
#include "smPacket.h"
#include "Utils/strings.h"

enum Stage {
    Stage_Finished,
    Stage_ToBegin,
    Stage_InProgress
};

namespace Events {

    class Questions
    {
        Questions();
        virtual ~Questions();

    public:
        BOOL MessageBegin();
        BOOL GetQuestion();
        BOOL CheckAnswers(rsPLAYINFO* Player, std::string Message);
        BOOL CheckRandomQuestion(UINT ID);
        BOOL CheckTime(UINT Time1, UINT Time2);


        int getStage();
        int setStage(int stage);
        int setMinBegin(int min);
        int getMinBegin();
    private:
    };


}

