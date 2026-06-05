#VARIABLES
	#file sorgente sono in src\
	
	SRCS:=$(wildcard src/*.c)
	
	#file oggetto sono in obj\ ma non esistono ancora

	OBJS:=$(patsubst src/%.c,obj/%.o,$(SRCS))


#TARGETS
platformer.exe: $(OBJS)
	gcc $(wildcard obj/*.o) -o bin/$@
	./bin/platformer.exe
	

obj/%.o: src/%.c
	gcc -MMD -MP -c $< -o $@

-include $(OBJS:.o=.d)

.PHONY: clean
clean:
	del /f obj\*.o
	 
.PHONY: cleanexe
cleanexe:
	del bin\platformer.exe

.PHONY: cleanall
cleanall:
	del /f obj\*.o
	del bin\platformer.exe

	