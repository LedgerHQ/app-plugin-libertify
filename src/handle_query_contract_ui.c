#include <stdbool.h>
#include "libertify_plugin.h"

static bool set_deposit_send_ui(ethQueryContractUI_t *msg, const context_t *context) {
    bool ret = false;

    strlcpy(msg->title, "Send", msg->titleLength);
    if (ADDRESS_IS_NETWORK_TOKEN(context->token_sent)) {
        const uint8_t *eth_amount = msg->pluginSharedRO->txContent->value.value;
        uint8_t eth_amount_size = msg->pluginSharedRO->txContent->value.length;

        // Converts the uint256 number located in `eth_amount` to its string representation and
        // copies this to `msg->msg`.
        ret = amountToString(eth_amount,
                             eth_amount_size,
                             WEI_TO_ETHER,
                             msg->network_ticker,
                             msg->msg,
                             msg->msgLength);
    } else {
        uint8_t decimals = context->decimals;
        const char *ticker = context->ticker;

        // If the token look up failed, use the default network ticker along with the default
        // decimals.
        if (!context->token_found) {
            decimals = WEI_TO_ETHER;
            ticker = msg->network_ticker;
        }

        ret = amountToString(context->amount_sent,
                             sizeof(context->amount_sent),
                             decimals,
                             ticker,
                             msg->msg,
                             msg->msgLength);
    }
    return ret;
}

static bool set_deposit_vault_ui(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Vault", msg->titleLength);
    strlcpy(msg->msg, context->vault_symbol, context->vault_symbol_length + 1);
    return true;
}

static bool set_withdraw_send_ui(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Send", msg->titleLength);

    uint8_t decimals = context->decimals;

    // If the token look up failed, use the default network ticker along with the default decimals.
    if (!context->token_found) {
        decimals = WEI_TO_ETHER;
    }

    return amountToString(context->amount_sent,
                          sizeof(context->amount_sent),
                          decimals,
                          context->vault_symbol,
                          msg->msg,
                          msg->msgLength);
}

static bool set_withdraw_receive_ui(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Receive Min", msg->titleLength);

    uint8_t decimals = context->decimals;
    const char *ticker = context->ticker;

    // If the token look up failed, use the default network ticker along with the default decimals.
    if (!context->token_found) {
        decimals = WEI_TO_ETHER;
        ticker = msg->network_ticker;
    }

    return amountToString(context->amount_received,
                          sizeof(context->amount_received),
                          decimals,
                          ticker,
                          msg->msg,
                          msg->msgLength);
}

void handle_query_contract_ui(ethQueryContractUI_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    bool ret = false;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    if (context->selectorIndex == DEPOSIT) {
        switch (msg->screenIndex) {
            case 0:
                ret = set_deposit_send_ui(msg, context);
                break;
            case 1:
                ret = set_deposit_vault_ui(msg, context);
                break;
            default:
                PRINTF("Received an invalid screenIndex\n");
        }
    } else if (context->selectorIndex == WITHDRAW) {
        switch (msg->screenIndex) {
            case 0:
                ret = set_withdraw_send_ui(msg, context);
                break;
            case 1:
                ret = set_withdraw_receive_ui(msg, context);
                break;
            default:
                PRINTF("Received an invalid screenIndex\n");
        }
    }
    msg->result = ret ? ETH_PLUGIN_RESULT_OK : ETH_PLUGIN_RESULT_ERROR;
}
