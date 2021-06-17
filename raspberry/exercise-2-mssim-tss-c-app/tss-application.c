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

int
tpm2_get32ByteRandom(ESYS_CONTEXT *ectx, unsigned char *buffer)
{
    TSS2_RC rc;

    TPM2B_DIGEST *b;
    rc = Esys_GetRandom(ectx,
                        ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE,
                        32, &b);
    if (rc != TSS2_RC_SUCCESS) {
        printf("Esys_GetRandom failed with error code: 0x%x (%s).\n", rc, Tss2_RC_Decode(rc));
        free(b);
        return 1;
    } else {
        memcpy(buffer, b->buffer, 32);
        free(b);
    }

    return 0;
}

int
tpm2_getProperties(ESYS_CONTEXT *ectx, int *isOwnerAuthSet, int *isEndorseAuthSet,
                   int *isLockoutAuthSet, int *isDisableClearSet)
{
    TPMI_YES_NO more_data;
    TPMS_CAPABILITY_DATA *capability_data = NULL;
    
    *isOwnerAuthSet = *isEndorseAuthSet = *isLockoutAuthSet = *isDisableClearSet = -1;

    TSS2_RC rc = Esys_GetCapability(ectx, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE,
            TPM2_CAP_TPM_PROPERTIES, TPM2_PT_VAR, TPM2_MAX_TPM_PROPERTIES, &more_data, &capability_data);
    if (rc != TSS2_RC_SUCCESS) {
        printf("Esys_GetCapability failed with error code: 0x%x (%s).\n", rc, Tss2_RC_Decode(rc));
        return 1;
    }

    {
        TPMU_CAPABILITIES capabilities = capability_data->data;
        TPMS_TAGGED_PROPERTY *properties = capabilities.tpmProperties.tpmProperty;
        int count = capabilities.tpmProperties.count;
        int i = 0;
    
        for (; i < count; ++i) {
            TPM2_PT property = properties[i].property;
            UINT32 value = properties[i].value;
            switch (property) {
            case TPM2_PT_PERMANENT:
                {
                    TPMA_PERMANENT attrs = value;

                    *isOwnerAuthSet = *isEndorseAuthSet = *isLockoutAuthSet = *isDisableClearSet = 0;
                    if (attrs & TPMA_PERMANENT_OWNERAUTHSET)
                        *isOwnerAuthSet = 1;
                    if (attrs & TPMA_PERMANENT_ENDORSEMENTAUTHSET)
                        *isEndorseAuthSet = 1;
                    if (attrs & TPMA_PERMANENT_LOCKOUTAUTHSET)
                        *isLockoutAuthSet = 1;
                    if (attrs & TPMA_PERMANENT_DISABLECLEAR)
                        *isDisableClearSet = 1;
                }
                break;
            }
        }
    }
    
    if (*isOwnerAuthSet == -1 || *isEndorseAuthSet == -1 ||
        *isLockoutAuthSet == -1 || *isDisableClearSet == -1)
        return 1;

    return 0;
}

int
tpm2_takeOwnership(ESYS_CONTEXT *ectx, const char *ownerAuth,
                   const char *endorseAuth, const char *lockoutAuth) 
{
    TSS2_RC rc;
    TPM2B_DIGEST auth;

    if (ownerAuth) {
        auth.size = (UINT16)snprintf((char *)auth.buffer, sizeof(auth.buffer), "%s", ownerAuth);

        rc = Esys_HierarchyChangeAuth(ectx, ESYS_TR_RH_OWNER, ESYS_TR_PASSWORD,
                ESYS_TR_NONE, ESYS_TR_NONE, &auth);

        if (rc != TPM2_RC_SUCCESS) {
            printf("Platform Esys_HierarchyChangeAuth failed with error code: 0x%x (%s).\n", rc, Tss2_RC_Decode(rc));
            return 1;
        }
    }
    
    if (endorseAuth) {
        auth.size = (UINT16)snprintf((char *)auth.buffer, sizeof(auth.buffer), "%s", endorseAuth);

        rc = Esys_HierarchyChangeAuth(ectx, ESYS_TR_RH_ENDORSEMENT, ESYS_TR_PASSWORD,
                ESYS_TR_NONE, ESYS_TR_NONE, &auth);

        if (rc != TPM2_RC_SUCCESS) {
            printf("Platform Esys_HierarchyChangeAuth failed with error code: 0x%x (%s).\n", rc, Tss2_RC_Decode(rc));
            return 1;
        }
    }
    
    if (lockoutAuth) {
        auth.size = (UINT16)snprintf((char *)auth.buffer, sizeof(auth.buffer), "%s", lockoutAuth);

        rc = Esys_HierarchyChangeAuth(ectx, ESYS_TR_RH_LOCKOUT, ESYS_TR_PASSWORD,
                ESYS_TR_NONE, ESYS_TR_NONE, &auth);

        if (rc != TPM2_RC_SUCCESS) {
            printf("Platform Esys_HierarchyChangeAuth failed with error code: 0x%x (%s).\n", rc, Tss2_RC_Decode(rc));
            return 1;
        }
    }
    return 0;
}

int
tpm2_setDictionaryLockout(ESYS_CONTEXT *ectx, const char *lockoutAuth)
{
    UINT32 max_tries = 32; // 32 retries before entering lockout
    UINT32 recovery_time = 5; // every 5 sec recover 1 retry
    UINT32 lockout_recovery_time = 300; // 300 sec lockout
    TPM2B_DIGEST auth;
    auth.size = (UINT16)snprintf((char *)auth.buffer, sizeof(auth.buffer), "%s", lockoutAuth);

    TSS2_RC rc = Esys_TR_SetAuth(ectx, ESYS_TR_RH_LOCKOUT, &auth);
    if (rc != TPM2_RC_SUCCESS) {
        printf("Esys_TR_SetAuth failed with error code: 0x%x (%s).\n", rc, Tss2_RC_Decode(rc));
        return 1;
    }

    rc = Esys_DictionaryAttackParameters(ectx, ESYS_TR_RH_LOCKOUT,
                ESYS_TR_PASSWORD, ESYS_TR_NONE, ESYS_TR_NONE,
                max_tries, recovery_time,
                lockout_recovery_time);
    if (rc != TPM2_RC_SUCCESS) {
        printf("Esys_DictionaryAttackParameters failed with error code: 0x%x (%s).\n", rc, Tss2_RC_Decode(rc));
        return 1;
    }

    return 0;
}

int
tpm2_disableClear(ESYS_CONTEXT *ectx)
{
    TPMI_YES_NO setDisable = TPM2_YES; // set the disableClear attribute
    
    TSS2_RC rc = Esys_ClearControl(ectx, ESYS_TR_RH_PLATFORM,
                ESYS_TR_PASSWORD, ESYS_TR_NONE, ESYS_TR_NONE, setDisable);
    if (rc != TPM2_RC_SUCCESS && rc != TPM2_RC_INITIALIZE) {
        printf("Esys_ClearControl failed with error code: 0x%x (%s).\n", rc, Tss2_RC_Decode(rc));
        return 1;
    }

    return 0;
}

int
tpm2_randomizePlatformAuth(ESYS_CONTEXT *ectx) 
{
    TSS2_RC rc;
    TPM2B_DIGEST auth;
    unsigned char buffer[32] = {0};
    
    if (tpm2_get32ByteRandom(ectx, buffer))
        return 1;

    auth.size = 32;
    memcpy(auth.buffer, buffer, 32);

    rc = Esys_HierarchyChangeAuth(ectx, ESYS_TR_RH_PLATFORM, ESYS_TR_PASSWORD,
            ESYS_TR_NONE, ESYS_TR_NONE, &auth);

    if (rc != TPM2_RC_SUCCESS) {
        printf("Platform Esys_HierarchyChangeAuth failed with error code: 0x%x (%s).\n", rc, Tss2_RC_Decode(rc));
        return 1;
    }

    return 0;
}

int
tpm2_disablePlatform(ESYS_CONTEXT *ectx) 
{
    TSS2_RC rc;
    ESYS_TR authHandle_handle = ESYS_TR_RH_PLATFORM;
    TPMI_RH_ENABLES target = ESYS_TR_RH_PLATFORM;
    TPMI_YES_NO toEnable = TPM2_NO;

    rc = Esys_HierarchyControl(
        ectx,
        authHandle_handle,
        ESYS_TR_PASSWORD,
        ESYS_TR_NONE,
        ESYS_TR_NONE,
        target,
        toEnable);

    if (rc != TPM2_RC_SUCCESS) {
        printf("Esys_HierarchyControl failed with error code: 0x%x (%s).\n", rc, Tss2_RC_Decode(rc));
        return 1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    ESYS_CONTEXT *ectx = NULL;
    TSS2_RC rc;
    int isOwnerAuthSet = 0;
    int isEndorseAuthSet = 0;
    int isLockoutAuthSet = 0;
    int isDisableClearSet = 0;
    int ret = 1;

    rc = esys_ctx_init(&ectx);
    if (rc != TSS2_RC_SUCCESS) {
        printf("esys_ctx_init failed with error code: 0x%x (%s)\n", rc, Tss2_RC_Decode(rc));
        return 1;
    }

    if ((ret = tpm2_getProperties(ectx, &isOwnerAuthSet, &isEndorseAuthSet, &isLockoutAuthSet, &isDisableClearSet)))
        goto exit;

    
    if (!isOwnerAuthSet) {
        if ((ret = tpm2_takeOwnership(ectx, "owner123", NULL, NULL)))
            goto exit;
    }

    if (!isEndorseAuthSet) {
        if ((ret = tpm2_takeOwnership(ectx, NULL, "endorsement123", NULL)))
            goto exit;
    }

    if (!isLockoutAuthSet) {
        if ((ret = tpm2_takeOwnership(ectx, NULL, NULL, "lockout123")))
            goto exit;
            
        if ((ret = tpm2_setDictionaryLockout(ectx, "lockout123")))
            goto exit;
    }

    if (!isDisableClearSet) {
        if ((ret = tpm2_disableClear(ectx)))
            goto exit;
    }

    if ((ret = tpm2_randomizePlatformAuth(ectx)))
        goto exit;

    if ((ret = tpm2_disablePlatform(ectx)))
        goto exit;

exit:
    rc = esys_ctx_free(&ectx);
    if (rc != TSS2_RC_SUCCESS) {
        printf("esys_ctx_free failed with error code: 0x%x (%s).\n", rc, Tss2_RC_Decode(rc));
        return 1;
    }

    return ret;
}

