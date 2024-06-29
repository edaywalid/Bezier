export LD_LIBRARY_PATH=$(pwd)/raylib/lib:$LD_LIBRARY_PATH
gcc -o bezier_curve main.c -I./raylib/include -L./raylib/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./bezier_curve

