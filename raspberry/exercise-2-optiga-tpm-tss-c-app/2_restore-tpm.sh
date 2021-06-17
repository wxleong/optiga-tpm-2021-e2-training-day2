#!/bin/sh -x

unset TPM2TOOLS_TCTI

echo "Reminder: Perform a reset by pressing the button on your TPM board."
echo "Press any key to continue..."
read null

# startup
tpm2_startup -c

# clear the disableClear attribute, this will permit tpm2_clear
tpm2_clearcontrol -C p c

# clear tpm
tpm2_clear -c p

