#!/bin/sh -x

export TPM2TOOLS_TCTI="mssim:host=localhost,port=2321"

# perform tpm startup
tpm2_startup -c
