#include "hFramework.h"
//#include "hCloudClient.h"
#include <stddef.h>
#include <stdio.h>

using namespace hFramework;

void hMain()
{
	sys.setLogDev(&Serial);
	//platform.begin(&RPi);
	for (;;)
	{
		hLED1.toggle();
		printf("test 111 %d\r\n", (int) sys.getRefTime());
		sys.delay(500);
	}
}

        sys.taskCreate([]() {
		while (true)
		{
        hMot1.rotRel(360, 200, true, INFINITE); //obrócić o 360 tików w stosunku do aktualnej pozycji, z mocą 20%
        hMot2.rotRel(360, 200, true, INFINITE); //obrócić o 360 tików w stosunku do aktualnej pozycji, z mocą 20%
        }
	    });
        sys.delay(50);
