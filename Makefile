all:
	cd cmd; make all;
	cd commands; make all;
	cd core; make all;
	cd os; make all;
	cd repo; make all;
	cd test; make all;
	cd thirdparty; make all;
	
clean:
	cd cmd; make clean;
	cd commands; make clean;
	cd core; make clean;
	cd os; make clean;
	cd repo; make clean;
	cd test; make clean;
	cd thirdparty; make clean;
	
