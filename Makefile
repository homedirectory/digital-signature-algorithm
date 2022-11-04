keygen:
	gcc -g -o keygen keygen.{c,h} numbers.{c,h} hash/libhash.a -lm
mksign:
	gcc -g -o mksign mksign.c sign.{c,h} sign_common.{c,h} numbers.{c,h} keygen.h hash/libhash.a -lm
verif:
	gcc -g -o verif verif.{c,h} sign_common.{c,h} numbers.{c,h} keygen.h hash/libhash.a -lm
gui:
	gcc -g -o gui gui.c sign.{c,h} sign_common.{c,h} numbers.{c,h} hash/libhash.a -lm `pkg-config --cflags --libs gtk4`
