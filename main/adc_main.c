/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"



#define EXAMPLE_ADC1_CHAN0		ADC_CHANNEL_4
#define EXAMPLE_ADC1_CHAN1		ADC_CHANNEL_5

#define SAMPLES		64

static int adc_raw;
static int adc_average;
static int i;

void app_main(void)
{
	adc_oneshot_unit_handle_t adc1_handle;
	adc_oneshot_unit_init_cfg_t init_config1 = {
		.unit_id = ADC_UNIT_1,
		.ulp_mode = ADC_ULP_MODE_DISABLE,
	};

	adc_oneshot_new_unit(&init_config1, &adc1_handle);

	adc_oneshot_chan_cfg_t config = {
		.bitwidth = ADC_BITWIDTH_10,//ADC_BITWIDTH_DEFAULT,
		.atten = ADC_ATTEN_DB_12
	};

	adc_oneshot_config_channel(adc1_handle, EXAMPLE_ADC1_CHAN0, &config);
	adc_oneshot_config_channel(adc1_handle, EXAMPLE_ADC1_CHAN1, &config);

	while (1)
	{
		adc_average = 0;
		for(i = 0; i < SAMPLES; i++)
		{
			adc_raw = 0;
			adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN0, &adc_raw);
			adc_average += adc_raw;
		}
		adc_average /= SAMPLES;

		printf ("Raw Value CH 0: %d \n", adc_average);
		
		vTaskDelay(pdMS_TO_TICKS(10));

		adc_average = 0;
		for(i = 0; i < SAMPLES; i++)
		{
			adc_raw = 0;
			adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN1, &adc_raw);
			adc_average += adc_raw;
		}
		adc_average /= SAMPLES;

		printf ("Raw Value CH 1: %d \n\n", adc_average);
		
		vTaskDelay(pdMS_TO_TICKS(490));
	}

}
