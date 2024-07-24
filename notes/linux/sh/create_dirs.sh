#!/bin/bash

names=(a b c d e f g h i j k l m n o p q r s t u v w x y z 0 1 2 3 4 5 6 7 8 9)

for l1 in "${names[@]}"; do
  mkdir -m a=rwx $l1
  cd $l1
  for l2 in "${names[@]}"; do
    mkdir -m a=rwx $l2
    cd $l2
    for l3 in "${names[@]}"; do
      mkdir -m a=rwx $l2
      cd $l3
      for l4 in "${names[@]}"; do
        mkdir -m a=rwx $l4
      done
      cd ..
    done
    cd ..
  done
  cd ..
done
cd ..
