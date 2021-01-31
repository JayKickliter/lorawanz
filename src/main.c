/*
 * Class A LoRaWAN sample application
 *
 * Copyright (c) 2020 Manivannan Sadhasivam <mani@kernel.org>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "personality.h"
#include <device.h>
#include <lorawan/lorawan.h>
#include <stdint.h>
#include <zephyr.h>

#define DEFAULT_RADIO_NODE DT_ALIAS(lora0)
BUILD_ASSERT(DT_NODE_HAS_STATUS(DEFAULT_RADIO_NODE, okay),
             "No default LoRa radio specified in DT");
#define DEFAULT_RADIO DT_LABEL(DEFAULT_RADIO_NODE)
#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(lorawanz);
#define DELAY K_MSEC(30000)

char data[] = {'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd'};

void
main(void) {
    const struct device *      lora_dev;
    struct lorawan_join_config join_cfg;
    int                        ret;

    lora_dev = device_get_binding(DEFAULT_RADIO);
    if (!lora_dev) {
        LOG_ERR("%s Device not found", DEFAULT_RADIO);
        return;
    }

    ret = lorawan_start();
    if (ret < 0) {
        LOG_ERR("lorawan_start failed: %d", ret);
        return;
    }

    join_cfg.mode          = LORAWAN_ACT_OTAA;
    join_cfg.dev_eui       = LORAWAN_DEV_EUI;
    join_cfg.otaa.join_eui = LORAWAN_JOIN_EUI;
    join_cfg.otaa.app_key  = LORAWAN_APP_KEY;
    join_cfg.otaa.nwk_key  = LORAWAN_APP_KEY;

    int join_attempts = 0;
    while (1) {
        LOG_INF("Joining network over OTAA, attempt %d", join_attempts);
        ret = lorawan_join(&join_cfg);
        if (ret < 0) {
            LOG_ERR("lorawan_join_network failed: %d", ret);
            join_attempts += 1;
            continue;
        }
        join_attempts = 0;

        int uplink_ctr = 0;
        while (1) {
            k_sleep(DELAY);
            /* Request ACK every 10 uplinks */
            int uplink_flags = uplink_ctr % 10 == 0 ? LORAWAN_MSG_CONFIRMED : 0;
            uplink_ctr += 1;
            LOG_INF("Sending uplink %d with flags 0x%02X...",
                    uplink_ctr,
                    uplink_flags);
            ret = lorawan_send(2, data, sizeof(data), uplink_flags);

            /*
             * Note: The stack may return -EAGAIN if the provided data
             * length exceeds the maximum possible one for the region and
             * datarate. But since we are just sending the same data here,
             * we'll just continue.
             */
            if (ret == -EAGAIN) {
                LOG_WRN("lorawan_send failed: %d. Continuing...", ret);
                continue;
            }

            if (ret < 0) {
                LOG_ERR("lorawan_send failed: %d", ret);
                continue;
            }

            LOG_INF("Data sent!");
        }
        k_sleep(DELAY);
    }

    LOG_ERR("This application should not exit");
}


/* Override these defaults by defining them in personality.c */
__attribute__((weak)) uint8_t LORAWAN_DEV_EUI[]  = {0};
__attribute__((weak)) uint8_t LORAWAN_JOIN_EUI[] = {0};
__attribute__((weak)) uint8_t LORAWAN_APP_KEY[]  = {0};
