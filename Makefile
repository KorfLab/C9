###################
# Makefile for C9 #
###################

APP1 = c9
SRC1 = c9.c
IK    = -Iik -Lik -lik
LIBS  = ik/libik.a

###########
# Targets #
###########

default:
	make $(APP1)

$(APP1): $(SRC1) $(LIBS)
	gcc -Wall -Werror -O2 -o  $(APP1) $(SRC1) -lm $(IK)

clean:
	rm -f *.o $(APP1)

