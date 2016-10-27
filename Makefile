all:
	cd repo; make all;
	cd cmd; make all;
	cd commands; make all;
	cd test; make all;
	
clean:
	cd repo; make clean;
	cd cmd; make clean;
	cd commands; make clean;
	cd test; make clean;