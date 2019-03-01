#!/bin/bash

echo "Compiling..."
echo " "
cd src/

g++ -std=c++11 -c main.cpp Mass.cpp Spring.cpp Rope.cpp Box.cpp Tree.cpp Scene.cpp GenerateForce.cpp Background.cpp Owl.cpp Bug.cpp Grass.cpp -I/usr/include


echo "Linking..."
echo " "

if [ -e main.cpp~ ];
then
    rm main.cpp~
fi

mv *.o ../linkers
cd ../linkers

g++ main.o Mass.o Spring.o Rope.o Box.o Tree.o Scene.o GenerateForce.o Background.o Bug.o Grass.o Owl.o -o program -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -L/usr/lib -lnoise

mv program ../
cd ../

if [ -e compile.sh~ ] ;
then 
    rm compile.sh~ 
fi

echo "Executing..."
echo " "
./program

