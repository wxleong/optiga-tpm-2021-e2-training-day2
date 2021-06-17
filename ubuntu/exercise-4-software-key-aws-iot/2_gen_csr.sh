#!/bin/bash

touch ~/.rnd
rm -f out/*.csr 2> /dev/null

if ! command -v jq &> /dev/null
then
  echo "jq could not be found. Install it by $ sudo apt install jq."
  exit
fi

# read from config file
thingname=`jq -r '.ThingName' config.jsn`

# Generate CSR
openssl req -new -key out/software.key -subj "/CN=${thingname}/O=Infineon/C=SG" -out out/software.csr

# Read cert
#openssl x509 -in out/software.csr -text -noout