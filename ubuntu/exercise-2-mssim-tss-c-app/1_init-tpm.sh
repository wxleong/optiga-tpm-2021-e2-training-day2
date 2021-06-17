#!/bin/sh -x

export TPM2TOOLS_TCTI="mssim:host=localhost,port=2321"

# startup
tpm2_startup -c

# take ownership
tpm2_changeauth -c o owner123
tpm2_changeauth -c e endorsement123
tpm2_changeauth -c l lockout123

# configure the lockout
tpm2_dictionarylockout -s -n 100000 -t 0 -l 0 -p lockout123

# set the disableClear attribute, this will prohibit tpm2_clear
tpm2_clearcontrol -C p s
