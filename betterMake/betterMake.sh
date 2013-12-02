#!/bin/bash

#DEBUG
rm -rf att
#adasdasdas
#DEBUG


#Crea lo stretto necessario in att
mkdir att
cp sol/grader.c att/grader.c
cd att
ln -s grader.c grader.cpp
touch template.c
ln -s template.c template.cpp
cd ..

#Copia dentro att input e output di prova
count=4;
for i in $(seq 0 $[$count-1])
do
	for pre in "in" "out"
	do
		cp "$pre"put/"$pre"put"$i".txt att/"$pre"put"$i".txt
	done
done
	
cat gen/GEN | while read line
do
	echo $line
done