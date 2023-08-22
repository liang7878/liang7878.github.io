#!/bin/bash

git checkout -b users/chs/$1

mkdir $1

cd $1

touch solution.cpp

hexo new post -p luogu/$1
