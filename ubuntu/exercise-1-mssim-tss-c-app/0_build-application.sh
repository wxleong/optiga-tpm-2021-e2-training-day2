#!/bin/sh -x

gcc -Wall -o tss-application tss-application.c -ltss2-tctildr -ltss2-esys -ltss2-rc -DTCTI_NAME_CONF=\"mssim:host=localhost,port=2321\"
