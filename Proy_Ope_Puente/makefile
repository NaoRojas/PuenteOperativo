ejecutar: clean compilar
	@./Proyecto

compilar: Puente.o PuntosCardinales.o Auto.o Cola.o Main.o
	@gcc -o Proyecto Main.o Cola.o Puente.o PuntosCardinales.o Auto.o -lpthread -lm

Main.o: Main.c Cola.c Puente.c PuntosCardinales.c Auto.c
	@gcc -c Main.c -lpthread -lm

Auto.o: Auto.c
	@gcc -c Auto.c

PuntosCardinales.o: PuntosCardinales.c
	@gcc -c PuntosCardinales.c

Puente.o: Puente.c PuntosCardinales.c
	@gcc -c Puente.c

Cola.o: Cola.c Auto.c
	@gcc -c Cola.c

clean:
	@rm -f Proyecto *.o
