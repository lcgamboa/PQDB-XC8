FILE=Makefile

SUBDIRS= lib.X testaPQDB.X teste_lab.X TimerLoop.X Display7Segmentos.X EndurinoPort.X LCD.X LDR.X LED_RGB.X LM35.X Potenciometro.X Teclado.X Console.X

.PHONY: $(SUBDIRS)  

all:$(SUBDIRS) 

$(SUBDIRS):
	$(MAKE) -f $(FILE) -C $@ $(MAKECMDGOALS)
	
clean:
	rm -rf */dist/
	rm -rf */debug/
	rm -rf */build/

