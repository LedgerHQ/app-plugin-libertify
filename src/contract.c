/*******************************************************************************
 *   Ethereum 2 Deposit Application
 *   (c) 2020 Ledger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ********************************************************************************/

#include <stdint.h>
#include "libertify_plugin.h"

// 1inch uses `0xeeeee` as a dummy address to represent ETH.
const uint8_t ONE_INCH_ETH_ADDRESS[ADDRESS_LENGTH] = {0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
                                                      0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
                                                      0xee, 0xee, 0xee, 0xee, 0xee, 0xee};

// clang-format off

// Function: depositWithSymbolCheck(uint256 amountIn,address srcToken,address vaultAddr,string vaultSymbol,bytes[] data)
// MethodID: 0xaa2daba6
static const uint32_t LIBERTIFY_PROXY_DEPOSIT_SELECTOR = 0xaa2daba6;

// Function: withdrawWithSymbolCheck(uint256 amountIn,address dstToken,address vaultAddr,uint256 minAmountOut,string vaultSymbol,bytes[] data)
// MethodID: 0xa2922622
static const uint32_t LIBERTIFY_PROXY_WITHDRAW_SELECTOR = 0xa2922622;

// clang-format on

// Array of all the different boilerplate selectors. Make sure this follows the same order as the
// enum defined in `boilerplate_plugin.h`
const uint32_t LIBERTIFY_SELECTORS[NUM_SELECTORS] = {
    LIBERTIFY_PROXY_DEPOSIT_SELECTOR,
    LIBERTIFY_PROXY_WITHDRAW_SELECTOR,
};
