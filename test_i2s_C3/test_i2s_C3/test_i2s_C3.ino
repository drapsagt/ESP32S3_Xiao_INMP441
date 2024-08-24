#include <driver/i2s.h>

i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    //.channel_format       = I2S_CHANNEL_FMT_ONLY_RIGHT,     // Also works
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT, // make sure your L/R pin is set right !
    //.communication_format = I2S_COMM_FORMAT_PCM,            // Also works
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// gpio pins for esp32S3 Xiao board
i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = 7,
    .ws_io_num = 9,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = 8};

void setup()
{
  Serial.begin(115200);

  // start up the I2S peripheral
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &i2s_mic_pins);
}

#define BUFFER_SIZE 512
int32_t raw_samples[BUFFER_SIZE];

void loop()
{
  // read from the I2S device
  size_t bytes_read = 0;
  i2s_read(I2S_NUM_0, raw_samples, sizeof(int32_t) * BUFFER_SIZE, &bytes_read, portMAX_DELAY);
  int samples_read = bytes_read / sizeof(int32_t);

  for (int i = 0; i < samples_read; i++)
  {
    Serial.printf("%ld\n", raw_samples[i]);
  }
}