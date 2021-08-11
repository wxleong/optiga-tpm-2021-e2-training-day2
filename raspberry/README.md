# Introduction

This guide is intended for Raspberry Pi only. Download the repository:
```
$ git clone https://github.com/wxleong/optiga-tpm-2021-e2-training-day2 ~/optiga-tpm-2021-e2-training-day2
```

If you have an [Iridium 9670 TPM 2.0 board](https://www.infineon.com/cms/en/product/evaluation-boards/iridium9670-tpm2.0-linux/), remember to attach it to your Raspberry Pi.

<p align="center">
    <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/raspberry-with-tpm.jpg" width="50%">
</p>

# Table of Contents

**[Exercise 1](#exercise-1)**<br>
**[Exercise 2](#exercise-2)**<br>
**[Exercise 3](#exercise-3)**<br>
**[Exercise 4](#exercise-4)**<br>
**[Final Assessment](#final-assessment)**<br>

# Exercise 1

This is an exercise using the [tpm2-tss](https://github.com/tpm2-software/tpm2-tss) library to read random value from a TPM.

| Directory  | Description |
| ------------- | ------------- |
| [exercise-1-mssim-tss-c-app](https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/tree/master/raspberry/exercise-1-mssim-tss-c-app) | Using Microsoft TPM 2.0 simulator.<br>`$ cd ~/optiga-tpm-2021-e2-training-day2/raspberry/exercise-1-mssim-tss-c-app` |
| [exercise-1-optiga-tpm-tss-c-app](https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/tree/master/raspberry/exercise-1-optiga-tpm-tss-c-app) | Using OPTIGA™ TPM.<br>`$ cd ~/optiga-tpm-2021-e2-training-day2/raspberry/exercise-1-optiga-tpm-tss-c-app` |

# Exercise 2

This is an exercise using the [tpm2-tss](https://github.com/tpm2-software/tpm2-tss) library to disable the platform hierarchy and configure the dictionary attack protection of a TPM.

| Directory  | Description |
| ------------- | ------------- |
| [exercise-2-mssim-tss-c-app](https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/tree/master/raspberry/exercise-2-mssim-tss-c-app) | Using Microsoft TPM 2.0 simulator.<br>`$ cd ~/optiga-tpm-2021-e2-training-day2/raspberry/exercise-2-mssim-tss-c-app` |
| [exercise-2-optiga-tpm-tss-c-app](https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/tree/master/raspberry/exercise-2-optiga-tpm-tss-c-app) | Using OPTIGA™ TPM.<br>`$ cd ~/optiga-tpm-2021-e2-training-day2/raspberry/exercise-2-optiga-tpm-tss-c-app` |

Use the following command to display variable TPM properties.
```
$ tpm2_getcap properties-variable
```

# Exercise 3

This is an exercise using the OpenSSL library to perform client-server TLS communication.

<p align="center">
    <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/client-server-tls.jpg" width="80%">
</p>

| Directory  | Description |
| ------------- | ------------- |
| [exercise-3-mssim-openssl-c-app](https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/tree/master/raspberry/exercise-3-mssim-openssl-c-app) | Using Microsoft TPM 2.0 simulator.<br>`$ cd ~/optiga-tpm-2021-e2-training-day2/raspberry/exercise-3-mssim-openssl-c-app` |
| [exercise-3-optiga-tpm-openssl-c-app](https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/tree/master/raspberry/exercise-3-optiga-tpm-openssl-c-app) | Using OPTIGA™ TPM.<br>`$ cd ~/optiga-tpm-2021-e2-training-day2/raspberry/exercise-3-optiga-tpm-openssl-c-app` |

# Exercise 4

This is an exercise using the [AWS IoT Device SDK](https://github.com/aws/aws-iot-device-sdk-embedded-c) to connect to [AWS IoT Core](https://docs.aws.amazon.com/iot/index.html).

<p align="center">
    <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/iot-core-exercise-4.jpg" width="80%">
</p>

| Directory  | Description |
| ------------- | ------------- |
| [exercise-4-software-key-aws-iot](https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/tree/master/raspberry/exercise-4-software-key-aws-iot) | Using Microsoft TPM 2.0 simulator.<br>`$ cd ~/optiga-tpm-2021-e2-training-day2/raspberry/exercise-4-software-key-aws-iot` |

1. Install dependencies (e.g., [AWS Command Line Interface](https://aws.amazon.com/cli/), [command-line JSON processor](https://stedolan.github.io/jq/)).
    ```
    $ sudo apt update
    $ sudo apt install awscli jq git cmake curl
    ```

2. Set AWS account credential and region.
    ```
    $ aws configure
    AWS Access Key ID [None]: <YOUR-PERSONAL-ACCESS-KEY-ID>
    AWS Secret Access Key [None]: <YOUR-PERSONAL-SECRET-ACCESS-KEY>
    Default region name [None]: ap-southeast-1
    Default output format [None]: json
    ```
    Check if the configuration is done correctly.
    ```
    $ cat ~/.aws/credentials
    $ cat ~/.aws/config
    ```

3. Get the AWS IoT endpoint and save it for later.
    ```
    $ aws iot describe-endpoint --endpoint-type iot:Data-ATS
    ```

<hr>
<ins><b>Provisioning & Onboarding</b></ins>

4. Device onboarding using an OpenSSL generated software-based key. First, navigate to:
    ```
    $ cd ~/optiga-tpm-2021-e2-training-day2/raspberry/exercise-4-software-key-aws-iot
    ```
    Go to the file `config.jsn` and edit the parameters (`ThingName` and `PolicyName`) to include your name so that you can be identified for later assessment (**no spaces allowed**):
    ```
    {
      "ThingName": "tpm-e2-training-day2-thing-<YOUR-NAME>",
      "PolicyName": "tpm-e2-training-day2-policy-<YOUR-NAME>"
    }
    ```
    The John Doe sample:
    ```
    {
      "ThingName": "tpm-e2-training-day2-thing-john-doe",
      "PolicyName": "tpm-e2-training-day2-policy-john-doe"
    }
    ```
    Finally, execute the scripts:
    ```
    $ ./0_clean-up.sh
    $ ./1_init-key.sh
    $ ./2_gen_csr.sh
    $ ./3_create_awsiot_thing.sh
    $ ls out/
    ```
    You will receive AmazonRootCA1.pem.crt (ca certificate) and software.crt (device certificate) in the directory `~/optiga-tpm-2021-e2-training-day2/raspberry/exercise-4-software-key-aws-iot/out`.

5. Sign in to your [AWS IoT account](https://ap-southeast-1.console.aws.amazon.com/). Select the option of `IAM user`.
    <p align="center">
        <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/iot-core-iam-page.jpg" width="70%">
    </p>
    
    Sign in using your IAM user account.
    <p align="center">
        <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/iot-core-signin-page.jpg" width="70%">
    </p>

    Confirm that you are in the right region.
    <p align="center">
        <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/iot-core-region.jpg" width="70%">
    </p>

    Navigate to IoT Core (search for the keyword "IoT Core")  
    <p align="center">
        <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/iot-core-navigate.jpg" width="70%">
    </p>
    
6. To verify if device onboarding step is sucessful (showing here is the John Doe sample). First, check if a thing has been created.
    <p align="center">
        <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/iot-core-things-page.jpg" width="70%">
    </p>
    
    Click on the thing and check if there is a certificate attached to it.
    <p align="center">
        <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/iot-core-cert-page.jpg" width="70%">
    </p>
    
    Click on the certificate to view the details.
    <p align="center">
        <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/iot-core-cert-details-page.jpg" width="70%">
    </p>
    The certificate id is the SHA-256 hash of a device certificate. You can generate this value by:

    ```
    $ cd ~/optiga-tpm-2021-e2-training-day2/raspberry/exercise-4-software-key-aws-iot
    $ openssl x509 -noout -fingerprint -sha256 -in out/software.crt
    ```

    Check if there is a policy attached to the certificate.
    <p align="center">
        <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/iot-core-policy-page.jpg" width="70%">
    </p>
    
    Click on the policy to view the details.
    <p align="center">
        <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/iot-core-policy-details-page.jpg" width="70%">
    </p>

<hr>
<ins><b>Connect Device to AWS IoT Core</b></ins>

7. Download the AWS IoT Device SDK and work on tag 202103.00.
    ```
    $ git clone https://github.com/aws/aws-iot-device-sdk-embedded-c ~/aws-iot-device-sdk-embedded-c
    $ cd ~/aws-iot-device-sdk-embedded-c
    $ git checkout 202103.00
    $ git submodule update --init --recursive
    ```

8. Generate Makefile. <br>
    Edit the command line to include the endpoint, client identifier, and name of the thing (**no spaces allowed**):
    ```
    $ cd ~/aws-iot-device-sdk-embedded-c
    $ cmake -S. -Bbuild \
      -DAWS_IOT_ENDPOINT="<ENDPOINT-FROM-STEP-3>" \
      -DTHING_NAME="<THINGNAME-FROM-STEP-4>" \
      -DCLIENT_IDENTIFIER="<YOUR-NAME>" \
      -DROOT_CA_CERT_PATH="${HOME}/optiga-tpm-2021-e2-training-day2/raspberry/exercise-4-software-key-aws-iot/out/AmazonRootCA1.pem.crt" \
      -DCLIENT_PRIVATE_KEY_PATH="${HOME}/optiga-tpm-2021-e2-training-day2/raspberry/exercise-4-software-key-aws-iot/out/software.key" \
      -DCLIENT_CERT_PATH="${HOME}/optiga-tpm-2021-e2-training-day2/raspberry/exercise-4-software-key-aws-iot/out/software.crt"
    ```
    The John Doe sample:
    ```
    $ cmake -S. -Bbuild \
      -DAWS_IOT_ENDPOINT="4389ntvsaefag8-ats.iot.ap-southeast-1.amazonaws.com" \
      -DTHING_NAME="tpm-e2-training-day2-thing-john-doe" \
      -DCLIENT_IDENTIFIER="john-doe" \
      -DROOT_CA_CERT_PATH="${HOME}/optiga-tpm-2021-e2-training-day2/raspberry/exercise-4-software-key-aws-iot/out/AmazonRootCA1.pem.crt" \
      -DCLIENT_PRIVATE_KEY_PATH="${HOME}/optiga-tpm-2021-e2-training-day2/raspberry/exercise-4-software-key-aws-iot/out/software.key" \
      -DCLIENT_CERT_PATH="${HOME}/optiga-tpm-2021-e2-training-day2/raspberry/exercise-4-software-key-aws-iot/out/software.crt"
    ```
    The output should look like this:
    <p align="center">
        <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/aws-iot-sdk-cmake.jpg" width="100%">
    </p>

9. Build demo application mqtt_demo_mutual_auth.
    ```
    $ cd ~/aws-iot-device-sdk-embedded-c/build
    $ make -j$(nproc) mqtt_demo_mutual_auth
    ```

10. Start the demo application mqtt_demo_mutual_auth and leave it running. 
    ```
    $ cd ~/aws-iot-device-sdk-embedded-c/build/bin
    $ ./mqtt_demo_mutual_auth
    ```
    The output should look like this:
    <p align="center">
        <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/aws-iot-sdk-sample-app.jpg" width="100%">
    </p>

11. Back to the AWS IoT Core website, click on the "Test" option to enter MQTT test client. Subscribe to the topic `<CLIENT_IDENTIFIER>/example/topic` (The John Doe sample: `john-doe/example/topic`). You will see the message "Hello World!"
    <p align="center">
        <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/iot-core-test-page.jpg" width="70%">
    </p>

12. Stop the running application before deleting the onboarded device from AWS IoT Core.
    ```
    $ cd ~/optiga-tpm-2021-e2-training-day2/raspberry/exercise-4-software-key-aws-iot
    $ ./4_clean_awsiot_thing.sh
    ```

# Final Assessment

Improve the [AWS IoT Device SDK](https://github.com/aws/aws-iot-device-sdk-embedded-c) by switching from software-based key to TPM-based key. <br>

Use only the [Microsoft TPM 2.0 Simulator](https://github.com/wxleong/optiga-tpm-2021-e2-training-day1/tree/master/raspberry#setup-microsoft-tpm-20-simulator) for this assessment.

The AWS IoT Device SDK supports OpenSSL and your main objective is to configure the OpenSSL to load and utilize the [TPM engine](https://github.com/tpm2-software/tpm2-tss-engine).<!-- In addition, disable the TPM platform hierarchy. -->

Use the provided scripts to perform TPM provisioning and onboarding to AWS IoT Core. Remember to edit the parameters (`ThingName` and `PolicyName`) in the file `config.jsn` to include your name so that you can be identified.

| Directory  | Description |
| ------------- | ------------- |
| [assessment-mssim-aws-iot](https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/tree/master/raspberry/assessment-mssim-aws-iot) | Scripts to perform TPM provisioning (Microsoft TPM 2.0 simulator) and onboarding to AWS IoT Core.<br>`$ cd ~/optiga-tpm-2021-e2-training-day2/raspberry/assessment-mssim-aws-iot` |

Workflow:
1. Launch Microsoft TPM 2.0 Simulator.
2. Complete the TPM provisioning and onboarding to AWS IoT Core using the provided scripts (remember to edit the config file to add your name). You will receive a CA certificate (AmazonRootCA1.pem.crt) and a device certificate (tpm.crt). You will find them in the directory `~/optiga-tpm-2021-e2-training-day2/raspberry/assessment-mssim-aws-iot/out/`.
3. Regenerate the Makefile (step 8) using following commands, remember to update the parameters:
    ```
    $ cd ~/aws-iot-device-sdk-embedded-c
    $ cmake -S. -Bbuild \
     -DAWS_IOT_ENDPOINT="<ENDPOINT-FROM-STEP-3>" \
     -DTHING_NAME="<THINGNAME-FROM-STEP-4>" \
     -DCLIENT_IDENTIFIER="<YOUR-NAME>" \
     -DROOT_CA_CERT_PATH="${HOME}/optiga-tpm-2021-e2-training-day2/raspberry/assessment-mssim-aws-iot/out/AmazonRootCA1.pem.crt" \
     -DCLIENT_PRIVATE_KEY_PATH="/ignored" \
     -DCLIENT_CERT_PATH="${HOME}/optiga-tpm-2021-e2-training-day2/raspberry/assessment-mssim-aws-iot/out/tpm.crt"
    ```
4. Integrate TPM engine into AWS IoT Device SDK by editing file `~/aws-iot-device-sdk-embedded-c/platform/posix/transport/src/openssl_posix.c` then rebuild the source (step 9). <br>
    Hint: search for `ENABLE_TPM_TSS_ENGINE` in exercise-3 `client.c`.
5. Run the demo application mqtt_demo_mutual_auth (step 10) and confirm the message "Hello World!" appeared on your MQTT test client web page (step 11).
6. Submission checklist:

    **Warning: after submission do not delete your onboarded device from AWS IoT Core (not to execute script 4_clean_awsiot_thing.sh).**

    1. Screenshot the terminal running the application mqtt_demo_mutual_auth showing the "Hello World!" message. The output should look like this:
        <p align="center">
            <img src="https://github.com/wxleong/optiga-tpm-2021-e2-training-day2/blob/master/media/aws-iot-sdk-sample-app.jpg" width="100%">
        </p>
    2. The file `~/aws-iot-device-sdk-embedded-c/platform/posix/transport/src/openssl_posix.c` 
