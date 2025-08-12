/* sd_spi.c
Copyright 2021 Carl John Kugler III

Licensed under the Apache License, Version 2.0 (the License); you may not use 
this file except in compliance with the License. You may obtain a copy of the 
License at

   http://www.apache.org/licenses/LICENSE-2.0 
Unless required by applicable law or agreed to in writing, software distributed 
under the License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR 
CONDITIONS OF ANY KIND, either express or implied. See the License for the 
specific language governing permissions and limitations under the License.
*/

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
//
#include "hardware/gpio.h"
//
#include "my_debug.h"
#include "sd_card.h"
#include "sd_spi.h"
#include "spi.h"

//#define TRACE_PRINTF(fmt, args...)
#define TRACE_PRINTF printf  // task_printf

void sd_spi_go_high_frequency(sd_card_t *pSD) {
    // Use more conservative high frequency for better compatibility
    uint target_freq = pSD->spi->baud_rate;
    if (target_freq > 8000000) {
        target_freq = 8000000; // Cap at 8MHz instead of 12MHz
    }
    uint actual = spi_set_baudrate(pSD->spi->hw_inst, target_freq);
    TRACE_PRINTF("%s: Actual frequency: %lu (target was %lu)\n", __FUNCTION__, (long)actual, (long)target_freq);
}
void sd_spi_go_low_frequency(sd_card_t *pSD) {
    // Use even slower initialization frequency for problematic cards
    uint actual = spi_set_baudrate(pSD->spi->hw_inst, 100 * 1000); // 100kHz instead of 400kHz
    TRACE_PRINTF("%s: Actual frequency: %lu\n", __FUNCTION__, (long)actual);
}
void sd_spi_go_medium_frequency(sd_card_t *pSD) {
    // Medium frequency for initial data operations - helps problematic cards
    uint actual = spi_set_baudrate(pSD->spi->hw_inst, 2000000); // 2MHz intermediate step
    TRACE_PRINTF("%s: Actual frequency: %lu\n", __FUNCTION__, (long)actual);
}

static void sd_spi_lock(sd_card_t *pSD) {
    spi_lock(pSD->spi);
}
static void sd_spi_unlock(sd_card_t *pSD) {
   spi_unlock(pSD->spi);
}

// Would do nothing if pSD->ss_gpio were set to GPIO_FUNC_SPI.
static void sd_spi_select(sd_card_t *pSD) {
    gpio_put(pSD->ss_gpio, 0);
    // A fill byte seems to be necessary, sometimes:
    uint8_t fill = SPI_FILL_CHAR;
    spi_write_blocking(pSD->spi->hw_inst, &fill, 1);
    LED_ON();
}

static void sd_spi_deselect(sd_card_t *pSD) {
    gpio_put(pSD->ss_gpio, 1);
    LED_OFF();
    /*
    MMC/SDC enables/disables the DO output in synchronising to the SCLK. This
    means there is a posibility of bus conflict with MMC/SDC and another SPI
    slave that shares an SPI bus. Therefore to make MMC/SDC release the MISO
    line, the master device needs to send a byte after the CS signal is
    deasserted.
    */
    uint8_t fill = SPI_FILL_CHAR;
    spi_write_blocking(pSD->spi->hw_inst, &fill, 1);
}

void sd_spi_acquire(sd_card_t *pSD) {
    sd_spi_lock(pSD);
    sd_spi_select(pSD);
}

void sd_spi_release(sd_card_t *pSD) {
    sd_spi_deselect(pSD);
    sd_spi_unlock(pSD);
}

bool sd_spi_transfer(sd_card_t *pSD, const uint8_t *tx, uint8_t *rx,
                     size_t length) {
    return spi_transfer(pSD->spi, tx, rx, length);
}

uint8_t sd_spi_write(sd_card_t *pSD, const uint8_t value) {
    // TRACE_PRINTF("%s\n", __FUNCTION__);
    u_int8_t received = SPI_FILL_CHAR;
#if 0
    int num = spi_write_read_blocking(pSD->spi->hw_inst, &value, &received, 1);    
    myASSERT(1 == num);
#else
    bool success = spi_transfer(pSD->spi, &value, &received, 1);
    myASSERT(success);
#endif
    return received;
}

void sd_spi_send_initializing_sequence(sd_card_t * pSD) {
    bool old_ss = gpio_get(pSD->ss_gpio);
    // Enhanced initialization sequence for problematic cards
    gpio_put(pSD->ss_gpio, 1);
    
    // Send many more clock cycles (200+ instead of 80)
    uint8_t ones[25]; // 25 * 8 = 200 clock cycles
    memset(ones, 0xFF, sizeof ones);
    
    // Send initializing sequence multiple times with delays
    for (int attempt = 0; attempt < 3; attempt++) {
        absolute_time_t timeout_time = make_timeout_time_ms(5); // Longer timeout
        do {
            sd_spi_transfer(pSD, ones, NULL, sizeof ones);
        } while (0 < absolute_time_diff_us(get_absolute_time(), timeout_time));
        
        if (attempt < 2) {
            sleep_ms(10); // Delay between attempts
        }
    }
    
    gpio_put(pSD->ss_gpio, old_ss);
}

void sd_spi_init_pl022(sd_card_t *pSD) {
    // Let the PL022 SPI handle it.
    // the CS line is brought high between each byte during transmission.
    gpio_set_function(pSD->ss_gpio, GPIO_FUNC_SPI);
}

/* [] END OF FILE */
