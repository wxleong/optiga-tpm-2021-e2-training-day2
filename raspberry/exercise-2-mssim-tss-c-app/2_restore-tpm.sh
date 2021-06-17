#!/bin/sh -x

export TPM2TOOLS_TCTI="mssim:host=localhost,port=2321"

echo "Reminder: Simulate a power cycle by restarting the tpm2-simulator."
echo "Press any key to continue..."
read null

# startup
tpm2_startup -c

# clear the disableClear attribute, this will permit tpm2_clear
tpm2_clearcontrol -C p c

# clear tpm
tpm2_clear -c p

