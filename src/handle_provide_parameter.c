#include "libertify_plugin.h"

static void handle_deposit(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case AMOUNT_SENT:  // amountIn
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            context->next_param = TOKEN_SENT;
            break;
        case TOKEN_SENT:  // srcToken
            copy_address(context->token_sent, msg->parameter, sizeof(context->token_sent));
            context->next_param = VAULT_ADDRESS;
            break;
        case VAULT_ADDRESS:  // vaultAddress
            context->next_param = VAULT_SYMBOL_OFFSET;
            break;
        case VAULT_SYMBOL_OFFSET:  // vaultSymbol
            context->offset = U2BE(msg->parameter, PARAMETER_LENGTH - 2);
            context->go_to_offset = true;
            context->next_param = VAULT_SYMBOL_LENGTH;
            break;
        case VAULT_SYMBOL_LENGTH:
            context->vault_symbol_length = U2BE(msg->parameter, PARAMETER_LENGTH - 2);
            context->next_param = VAULT_SYMBOL_VALUE;
            break;
        case VAULT_SYMBOL_VALUE:
            strlcpy(context->vault_symbol,
                    (const char *) msg->parameter,
                    context->vault_symbol_length + 1);
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        case UNEXPECTED_PARAMETER:
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_withdraw(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case AMOUNT_SENT:  // amountIn
            copy_parameter(context->amount_sent, msg->parameter, sizeof(context->amount_sent));
            context->next_param = TOKEN_RECEIVED;
            break;
        case TOKEN_RECEIVED:  // dstToken
            copy_address(context->token_received, msg->parameter, sizeof(context->token_received));
            context->next_param = VAULT_ADDRESS;
            break;
        case VAULT_ADDRESS:  // vaultAddress
            context->next_param = MIN_AMOUNT_RECEIVED;
            break;
        case MIN_AMOUNT_RECEIVED:  // minAmountOut
            copy_parameter(context->amount_received,
                           msg->parameter,
                           sizeof(context->amount_received));
            context->next_param = VAULT_SYMBOL_OFFSET;
            break;
        case VAULT_SYMBOL_OFFSET:  // vaultSymbol
            context->offset = U2BE(msg->parameter, PARAMETER_LENGTH - 2);
            context->go_to_offset = true;
            context->next_param = VAULT_SYMBOL_LENGTH;
            break;
        case VAULT_SYMBOL_LENGTH:
            context->vault_symbol_length = U2BE(msg->parameter, PARAMETER_LENGTH - 2);
            context->next_param = VAULT_SYMBOL_VALUE;
            break;
        case VAULT_SYMBOL_VALUE:
            strlcpy(context->vault_symbol,
                    (const char *) msg->parameter,
                    context->vault_symbol_length + 1);
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        case UNEXPECTED_PARAMETER:
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

void handle_provide_parameter(ethPluginProvideParameter_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    // We use `%.*H`: it's a utility function to print bytes. You first give
    // the number of bytes you wish to print (in this case, `PARAMETER_LENGTH`) and then
    // the address (here `msg->parameter`).
    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    switch (context->selectorIndex) {
        case DEPOSIT:
            handle_deposit(msg, context);
            break;
        case WITHDRAW:
            handle_withdraw(msg, context);
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
