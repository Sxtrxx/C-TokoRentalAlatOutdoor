name = TOKO_RENTAL_ALAT_OURDOOR
sources = main.c LIBRARY_TOKO_RENTAL.c

$(name): $(sources)
	gcc -o $(name) $(sources)