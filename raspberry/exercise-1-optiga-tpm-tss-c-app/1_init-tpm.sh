#!/bin/sh -x

# make tpm device node accessible by non-privileged user
sudo chmod a+rw /dev/tpmrm0
