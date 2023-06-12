#ifndef RGB
#define RGB

#include "io.h"

//todos desligados
#define OFF    0

//cores primárias
#define RED    PIN_D1
#define GREEN  PIN_D2
#define BLUE   PIN_D4

//cores secundárias
#define YELLOW (RED+GREEN)
#define CYAN   (GREEN+BLUE)
#define PURPLE (RED+BLUE)

//todos acesos
#define WHITE  (RED+GREEN+BLUE)

	void rgbColor(int led);
	void turnOn(int led);
	void turnOff(int led);
	void rgbInit(void);
#endif
