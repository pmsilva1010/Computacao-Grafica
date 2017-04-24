FLAGS	= -lglut -lGLU -lGL -lm
CC 		= g++
PROG	= CG_Archery
OBJS	= projeto.o RgbImage.o glm.o

all: ${PROG}

clean: 
	rm ${OBJS} ${PROG}

${PROG}:	${OBJS}
	${CC} ${OBJS} -o $@ ${FLAGS}

.c.o:
	${CC} ${FLAGS} $< -c

##################################

glm.o: glm.h glm.cpp

RgbImage.o: RgbImage.h RgbImage.cpp

projeto.o: projeto.cpp

teste: glm.o RgbImage.o projeto.o
