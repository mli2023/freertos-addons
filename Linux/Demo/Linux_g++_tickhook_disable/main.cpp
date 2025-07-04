/****************************************************************************
 *
 *  Copyright (c) 2023, Michael Becker (michael.f.becker@gmail.com)
 *
 *  This file is part of the FreeRTOS Add-ons project.
 *
 *  Source Code:
 *  https://github.com/michaelbecker/freertos-addons
 *
 *  Project Page:
 *  http://michaelbecker.github.io/freertos-addons/
 *
 *  On-line Documentation:
 *  http://michaelbecker.github.io/freertos-addons/docs/html/index.html
 *
 *  MIT License
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files
 *  (the "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so,subject to the
 *  following conditions:
 *
 *  + The above copyright notice and this permission notice shall be included
 *    in all copies or substantial portions of the Software.
 *  + Credit is appreciated, but not required, if you find this project
 *    useful enough to include in your application, product, device, etc.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/



#include <stdio.h>
#include <iostream>
#include "FreeRTOS.h"
#include "task.h"
#include "cThread.h"
#include "cTicks.h"
#include "cTickHook.h"


using namespace cpp_freertos;
using namespace std;




class MyTickHook : public TickHook {

    public:
        MyTickHook(int id) : TickHook(), Id(id), Cnt(0)
        {
            Register();
        }

    protected:
        void Run() {

            if (++Cnt > 1000) {
                cout << "Running TickHook # " << Id << endl;
                Cnt = 0;
            }
        }

    private:
        int Id;
        int Cnt;
};


class MyThread : public Thread {

    public:

        MyThread(MyTickHook &h1, MyTickHook &h2, MyTickHook &h3)
           : Thread("MyThread", 100, 1),
             Hook1(h1), Hook2(h2), Hook3(h3)
        {
            Start();
        };

    protected:

        virtual void Run() {

            int DelayInSeconds = 3;
            int Count = 0;

            cout << "Starting thread" << endl;
            
            while (true) {
            
                TickType_t ticks = Ticks::SecondsToTicks(DelayInSeconds);
                Delay(ticks);

                switch (Count % 6) {
                    case 0:
                        cout << "Disabling Hook 1" << endl;
                        Hook1.Disable();
                        break;
                    case 1:
                        cout << "Disabling Hook 2" << endl;
                        Hook2.Disable();
                        break;
                    case 2:
                        cout << "Disabling Hook 3" << endl;
                        Hook3.Disable();
                        break;
                    case 3:
                        cout << "Enabling Hook 2" << endl;
                        Hook2.Enable();
                        break;
                    case 4:
                        cout << "Enabling Hook 1" << endl;
                        Hook1.Enable();
                        break;
                    case 5:
                        cout << "Enabling Hook 3" << endl;
                        Hook3.Enable();
                        break;
                }

                Count++;
            }
        };

    private:
        MyTickHook &Hook1;
        MyTickHook &Hook2;
        MyTickHook &Hook3;
};


int main (void)
{
    cout << "Testing FreeRTOS C++ wrappers" << endl;
    cout << "Simple Tasks" << endl;

    MyTickHook hook1(1);
    MyTickHook hook2(2);
    MyTickHook hook3(3);

    MyThread thr(hook1, hook2, hook3);

    Thread::StartScheduler();

    //
    //  We shouldn't ever get here unless someone calls 
    //  Thread::EndScheduler()
    //

    cout << "Scheduler ended!" << endl;

    return 0;
}


void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
    printf("ASSERT: %s : %d\n", pcFileName, (int)ulLine);
    while(1);
}


unsigned long ulGetRunTimeCounterValue(void)
{
    return 0;
}

void vConfigureTimerForRunTimeStats(void)
{
    return;
}


extern "C" void vApplicationMallocFailedHook(void);
void vApplicationMallocFailedHook(void)
{
	while(1);
}


