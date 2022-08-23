#include <catch2/catch_test_macros.hpp>

#include "lfbb.h"

TEST_CASE("Write to the beginning", "[write_beginning]") {
  uint8_t buf[512];
  const uint8_t test_data[320] = {0xE5U};

  LFBB_Inst_Type lfbb;
  LFBB_Init(&lfbb, buf, sizeof(buf));

  uint8_t *write_location = LFBB_WriteAcquire(&lfbb, sizeof(test_data));
  REQUIRE(write_location != nullptr);

  memcpy(write_location, test_data, sizeof(test_data));

  LFBB_WriteRelease(&lfbb, sizeof(test_data));

  size_t read_available;
  const uint8_t *read_location = LFBB_ReadAcquire(&lfbb, &read_available);

  REQUIRE(read_location != nullptr);
  REQUIRE(read_available == sizeof(test_data));
  REQUIRE(memcmp(test_data, read_location, sizeof(test_data)) == 0);
}

TEST_CASE("Try to acquire too much data", "[acquire_too_much]") {
  uint8_t buf[512];

  LFBB_Inst_Type lfbb;
  LFBB_Init(&lfbb, buf, sizeof(buf));

  uint8_t *write_location = LFBB_WriteAcquire(&lfbb, sizeof(buf) + rand());
  REQUIRE(write_location == nullptr);
}

TEST_CASE("Try to acquire read with an empty buffer", "[read_empty]") {
  uint8_t buf[512];

  LFBB_Inst_Type lfbb;
  LFBB_Init(&lfbb, buf, sizeof(buf));

  size_t read_available;
  uint8_t *read_location = LFBB_ReadAcquire(&lfbb, &read_available);
  REQUIRE(read_location == nullptr);
  REQUIRE(read_available == 0);
}

TEST_CASE("Write with overflow condition", "[write_overflow]") {
  uint8_t buf[512];
  const uint8_t test_data[320] = {0xE5U};

  LFBB_Inst_Type lfbb;
  LFBB_Init(&lfbb, buf, sizeof(buf));

  /* Write to the start and read the data back */
  uint8_t *write_location = LFBB_WriteAcquire(&lfbb, sizeof(test_data));
  memcpy(write_location, test_data, sizeof(test_data));
  LFBB_WriteRelease(&lfbb, sizeof(test_data));

  size_t read_available;
  uint8_t *read_location = LFBB_ReadAcquire(&lfbb, &read_available);
  LFBB_ReadRelease(&lfbb, sizeof(test_data));

  /* Write again, this time the oveflow will trigger and should give us the
   * beginning again */
  const uint8_t test_data2[240] = {0xA3U};
  write_location = LFBB_WriteAcquire(&lfbb, sizeof(test_data2));
  REQUIRE(write_location != nullptr);
  memcpy(write_location, test_data2, sizeof(test_data2));
  LFBB_WriteRelease(&lfbb, sizeof(test_data2));

  read_location = LFBB_ReadAcquire(&lfbb, &read_available);
  REQUIRE(read_location != nullptr);
  REQUIRE(read_available == sizeof(test_data2));
  REQUIRE(memcmp(test_data2, read_location, sizeof(test_data2)) == 0);
}

TEST_CASE("Read data written after overflow condition write",
          "[read_after_overflow_write]") {
  uint8_t buf[512];
  const uint8_t test_data[320] = {0xE5U};

  LFBB_Inst_Type lfbb;
  LFBB_Init(&lfbb, buf, sizeof(buf));

  /* Write to the start and read the data back */
  uint8_t *write_location = LFBB_WriteAcquire(&lfbb, sizeof(test_data));
  memcpy(write_location, test_data, sizeof(test_data));
  LFBB_WriteRelease(&lfbb, sizeof(test_data));

  size_t read_available;
  uint8_t *read_location = LFBB_ReadAcquire(&lfbb, &read_available);
  LFBB_ReadRelease(&lfbb, sizeof(test_data));

  /* Write again, this time the oveflow will trigger and should give us the
   * beginning again */
  const uint8_t test_data2[240] = {0xA3U};
  write_location = LFBB_WriteAcquire(&lfbb, sizeof(test_data2));
  memcpy(write_location, test_data2, sizeof(test_data2));
  LFBB_WriteRelease(&lfbb, sizeof(test_data2));

  read_location = LFBB_ReadAcquire(&lfbb, &read_available);
  LFBB_ReadRelease(&lfbb, sizeof(test_data2));

  /* Write again, without overflow and read back */
  const uint8_t test_data3[120] = {0xBCU};
  write_location = LFBB_WriteAcquire(&lfbb, sizeof(test_data3));
  memcpy(write_location, test_data3, sizeof(test_data3));
  LFBB_WriteRelease(&lfbb, sizeof(test_data3));

  read_location = LFBB_ReadAcquire(&lfbb, &read_available);
  REQUIRE(read_location != nullptr);
  REQUIRE(read_available == sizeof(test_data3));
  REQUIRE(memcmp(test_data3, read_location, sizeof(test_data3)) == 0);
}

TEST_CASE("Interleaved write and read with enough space",
          "[interleaved_success]") {
  uint8_t buf[512];
  const uint8_t test_data[320] = {0xE5U};

  LFBB_Inst_Type lfbb;
  LFBB_Init(&lfbb, buf, sizeof(buf));

  /* 1. Complete write */
  uint8_t *write_location = LFBB_WriteAcquire(&lfbb, sizeof(test_data));
  memcpy(write_location, test_data, sizeof(test_data));
  LFBB_WriteRelease(&lfbb, sizeof(test_data));

  /* 2. Read acquire, the linear space previously written is reserved for
   * reading now */
  size_t read_available;
  uint8_t *read_location = LFBB_ReadAcquire(&lfbb, &read_available);

  /* 3. Write acquire, a linear space after the read linear space is reserved
   * for writing and is copied to*/
  const uint8_t test_data2[120] = {0xA3U};
  write_location = LFBB_WriteAcquire(&lfbb, sizeof(test_data2));
  REQUIRE(write_location != nullptr);
  memcpy(write_location, test_data2, sizeof(test_data2));

  /* 4. Compare the data */
  REQUIRE(memcmp(test_data, read_location, sizeof(test_data)) == 0U);
}

TEST_CASE("Interleaved write and read without enough space",
          "[interleaved_fail]") {
  uint8_t buf[512];
  const uint8_t test_data[320] = {0xE5U};

  LFBB_Inst_Type lfbb;
  LFBB_Init(&lfbb, buf, sizeof(buf));

  /* 1. Complete write */
  uint8_t *write_location = LFBB_WriteAcquire(&lfbb, sizeof(test_data));
  memcpy(write_location, test_data, sizeof(test_data));
  LFBB_WriteRelease(&lfbb, sizeof(test_data));

  /* 2. Read acquire, the linear space previously written is reserved for
   * reading now */
  size_t read_available;
  uint8_t *read_location = LFBB_ReadAcquire(&lfbb, &read_available);

  /* 3. Write acquire, a linear space after the read linear space is reserved
   * for writing and is copied to*/
  const uint8_t test_data2[240] = {0xA3U};
  write_location = LFBB_WriteAcquire(&lfbb, sizeof(test_data2));
  REQUIRE(write_location == nullptr);
}
