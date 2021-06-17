#!/bin/sh -x

mkdir ./out 2> /dev/null

openssl genrsa -out ./out/software.key 2048
