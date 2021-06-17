/*
 * References:
 * https://github.com/tpm2-software/tpm2-tools
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <tss2/tss2_tpm2_types.h>
#include <tss2/tss2_rc.h>
#include <tss2/tss2_mu.h>
#include <tss2/tss2_esys.h>
#include <tss2/tss2_tctildr.h>

#ifndef TCTI_NAME_CONF
#define TCTI_NAME_CONF NULL // auto detect
#endif

/** Initialize the Esys context
 *
 * Initialize an Esys context.
 * @param esys_ctx The context to initialize.
 * @retval TSS2_RC_SUCCESS on success
 * @retval TSS2_BASE_RC_BAD_REFERENCE if no pointer was provided
 * @retval Errors from Tcti initialization or Esys_Initialize()
 */
TSS2_RC
esys_ctx_init(ESYS_CONTEXT **esys_ctx)
{

    TSS2_RC rc;
    if (!esys_ctx) {
        rc = TSS2_BASE_RC_BAD_REFERENCE;
    } else {
        TSS2_TCTI_CONTEXT *tcti_ctx = NULL;
        rc = Tss2_TctiLdr_Initialize(TCTI_NAME_CONF, &tcti_ctx);
        if (TSS2_RC_SUCCESS != rc) {
        } else {
            rc = Esys_Initialize(esys_ctx, tcti_ctx, NULL);
            if (TSS2_RC_SUCCESS != rc) {
                Tss2_TctiLdr_Finalize(&tcti_ctx);
            }
        }
    }
    return rc;
}

/** Finalize the Esys context
 *
 * Get the TCTI context and finalize this alongside the Esys context.
 * @param esys_ctx The Esys context
 * @retval TSS2_RC_SUCCESS on success
 * @retval TSS2_BASE_RC_BAD_REFERENCE if no pointer was provided
 * @retval Errors from Esys_GetTcti()
 */
TSS2_RC
esys_ctx_free(ESYS_CONTEXT **esys_ctx)
{
    TSS2_RC rc;
    if ((!esys_ctx) || (!*esys_ctx)) {
        rc = TSS2_BASE_RC_BAD_REFERENCE;
    } else {
        TSS2_TCTI_CONTEXT *tcti_ctx;
        rc = Esys_GetTcti(*esys_ctx, &tcti_ctx);
        Esys_Finalize(esys_ctx);
        if (TSS2_RC_SUCCESS != rc) {
        } else {
            Tss2_TctiLdr_Finalize(&tcti_ctx);
        }
    }
    return rc;
}

/**
 * Get random
 */
int
tpm2_getrandom(void)
{
    TSS2_RC rc;
    ESYS_CONTEXT *esys_ctx = NULL;

    rc = esys_ctx_init(&esys_ctx);
    if (rc != TSS2_RC_SUCCESS) {
        printf("esys_ctx_init failed with error code: 0x%x (%s)\n", rc, Tss2_RC_Decode(rc));
        return 1;
    }

    {
        TPM2B_DIGEST *b;
        rc = Esys_GetRandom(esys_ctx,
                            ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE,
                            1, &b);
        if (rc != TSS2_RC_SUCCESS) {
            printf("Esys_GetRandom failed with error code: 0x%x (%s).\n", rc, Tss2_RC_Decode(rc));
            return 1;
        } else {
            printf("random received: 0x%02x\n", *(b->buffer));
            free(b);
        }
    }

    rc = esys_ctx_free(&esys_ctx);
    if (rc != TSS2_RC_SUCCESS) {
        printf("esys_ctx_free failed with error code: 0x%x (%s).\n", rc, Tss2_RC_Decode(rc));
        return 1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    int r = 1;

    if ((r = tpm2_getrandom()))
        return r;

    return r;
}

