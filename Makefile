###################
# Makefile for C9 #
###################

APP1 = c9
SRC1 = c9.c

APP2 = unimeter
SRC2 = unimeter.c

IK    = -Iik -Lik -lik
LIBS  = ik/libik.a

###########
# Targets #
###########

default:
	make $(APP1)
	make $(APP2)

$(APP1): $(SRC1) $(LIBS)
	gcc -Wall -Werror -O2 -o  $(APP1) $(SRC1) -lm $(IK)

$(APP2): $(SRC2) $(LIBS)
	gcc -Wall -Werror -O2 -o  $(APP2) $(SRC2) -lm $(IK)

clean:
	rm -f *.o $(APP1) $(APP2)

