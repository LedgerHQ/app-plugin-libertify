#pragma once

#include "eth_plugin_interface.h"
#include <string.h>

// Number of selectors defined in this plugin. Should match the enum `selector_t`.
#define NUM_SELECTORS 2

// Name of the plugin.
#define PLUGIN_NAME "Libertify"

// 1inch uses `0xeeeee` as a dummy address to represent ETH in Swap.
extern const uint8_t ONE_INCH_ETH_ADDRESS[ADDRESS_LENGTH];

// Returns 1 if corresponding address is the 1inch address for the chain token (ETH, BNB, MATIC,
// etc.. are 0xeeeee...).
#define ADDRESS_IS_NETWORK_TOKEN(_addr) (!memcmp(_addr, ONE_INCH_ETH_ADDRESS, ADDRESS_LENGTH))

// Enumeration of the different selectors possible.
// Should follow the exact same order as the array declared in main.c
typedef enum {
    DEPOSIT = 0,
    WITHDRAW,
} selector_t;

// Enumeration used to parse the smart contract data.
typedef enum {
    TOKEN_SENT = 0,
    AMOUNT_SENT,
    VAULT_ADDRESS,
    VAULT_SYMBOL_OFFSET,
    VAULT_SYMBOL_LENGTH,
    VAULT_SYMBOL_VALUE,
    MIN_AMOUNT_RECEIVED,
    TOKEN_RECEIVED,
    UNEXPECTED_PARAMETER,
} parameter;

extern const uint32_t LIBERTIFY_SELECTORS[NUM_SELECTORS];

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
typedef struct context_t {
    uint8_t token_sent[ADDRESS_LENGTH];
    uint8_t amount_sent[INT256_LENGTH];
    uint8_t vault_address[ADDRESS_LENGTH];
    char vault_symbol[MAX_TICKER_LEN];
    uint16_t vault_symbol_length;

    // For display.
    uint8_t amount_received[INT256_LENGTH];
    uint8_t token_received[ADDRESS_LENGTH];  // FIXME: Merge with token_sent?
    char ticker[MAX_TICKER_LEN];
    uint8_t decimals;
    uint8_t token_found;

    // For parsing data.
    uint8_t next_param;  // Set to be the next param we expect to parse.
    uint16_t offset;     // Offset at which the array or struct starts.
    bool go_to_offset;   // If set, will force the parsing to iterate through parameters until
                         // `offset` is reached.

    // For both parsing and display.
    selector_t selectorIndex;
} context_t;

// Piece of code that will check that the above structure is not bigger than 5 * 32. Do not remove
// this check.
_Static_assert(sizeof(context_t) <= 5 * 32, "Structure of parameters too big.");
