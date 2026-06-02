sl_status_t aht20_read(sl_i2cspm_t *i2cspm_handle, float *temp, float *hum)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;

  uint8_t cmd_trigger[] = {0xAC, 0x33, 0x00};
  uint8_t read_data[7]; // 1 status + 2 hum + 1 bridge + 2 temp + 1 CRC

  // 1. Send measurement trigger command.
  seq.addr = AHT20_I2C_ADDR << 1;
  seq.flags = I2C_FLAG_WRITE;
  seq.buf[0].data = cmd_trigger;
  seq.buf[0].len = sizeof(cmd_trigger);
  ret = I2CSPM_Transfer(i2cspm_handle, &seq);
  if (ret != i2cTransferDone)
    return SL_STATUS_FAIL;

  // 2. Wait 80 ms for the sensor measurement.
  sl_sleeptimer_delay_millisecond(80);

  // 3. Read 7 bytes of sensor data.
  seq.flags = I2C_FLAG_READ;
  seq.buf[0].data = read_data;
  seq.buf[0].len = 7;
  ret = I2CSPM_Transfer(i2cspm_handle, &seq);
  if (ret != i2cTransferDone)
    return SL_STATUS_FAIL;

  // 4. Check the busy bit, bit 7 of byte 0.
  if ((read_data[0] & 0x80) != 0)
  {
    return SL_STATUS_BUSY; // Sensor is still busy.
  }

  // 5. Convert raw data according to the AHT20 datasheet.
  uint32_t raw_hum = ((uint32_t)read_data[1] << 12) |
                     ((uint32_t)read_data[2] << 4) |
                     ((uint32_t)read_data[3] >> 4);

  uint32_t raw_temp = (((uint32_t)read_data[3] & 0x0F) << 16) |
                      ((uint32_t)read_data[4] << 8) |
                      ((uint32_t)read_data[5]);

  *hum = ((float)raw_hum / 0x100000) * 100.0f;
  *temp = ((float)raw_temp / 0x100000) * 200.0f - 50.0f;

  return SL_STATUS_OK;
}
