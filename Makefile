run:
	 export LD_LIBRARY_PATH=$(pwd)/raylib/lib:$LD_LIBRARY_PATH 
	 gcc -o bezier_curve  main.c -L./raylib/lib/ -I./raylib/include -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	 ./bezier_curve
