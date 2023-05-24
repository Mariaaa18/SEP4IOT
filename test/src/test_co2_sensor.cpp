/*

FAKE_VOID_FUNC(mh_z19_initialise, serial_comPort_t);
FAKE_VALUE_FUNC(mh_z19_returnCode_t, mh_z19_takeMeassuring);
FAKE_VALUE_FUNC(mh_z19_returnCode_t, mh_z19_getCo2Ppm, uint16_t *);
// FAKE_VOID_FUNC(run_coTwo);

extern uint32_t _myEventGroupSender = 0;

class CoTwoTest : public ::testing::Test
{
protected:
    void SetUp() override
    {

        RESET_FAKE(mh_z19_initialise);
        RESET_FAKE(mh_z19_takeMeassuring);
        RESET_FAKE(xEventGroupCreate);
        RESET_FAKE(xEventGroupSetBits);

        RESET_FAKE(mh_z19_getCo2Ppm);
        FFF_RESET_HISTORY();
    }
};

TEST_F(CoTwoTest, create_co2_task)
{
    mh_z19_takeMeassuring_fake.return_val = MHZ19_OK;

    createCoTwo();

    ASSERT_EQ(reinterpret_cast<void (*)(void *)>(xTaskCreate_fake.arg0_val), reinterpret_cast<void (*)(void *)>(&coTwo_task));
    ASSERT_EQ(strncmp(xTaskCreate_fake.arg1_val, "CoTwoTask", 2), 0);
}

TEST_F(CoTwoTest, RunCoTwo_success_full_measurement)
{
    mh_z19_initialise_fake.arg0_val = ser_USART3;
    mh_z19_takeMeassuring_fake.return_val = MHZ19_OK;
    mh_z19_getCo2Ppm_fake.return_val = MHZ19_OK;
    mh_z19_getCo2Ppm_fake.custom_fake = [](uint16_t *ppm)
    {
        *ppm = 1000; // Set the CO2 value to 1000 ppm
        return MHZ19_OK;
    };

    createCoTwo();

    ASSERT_EQ(mh_z19_initialise_fake.call_count, 1);

    run_coTow();

    ASSERT_EQ(mh_z19_takeMeassuring_fake.call_count, 1);
    ASSERT_EQ(mh_z19_getCo2Ppm_fake.call_count, 1);
    ASSERT_EQ(xEventGroupSetBits_fake.call_count, 1);

    ASSERT_EQ(getCoTwo(), 1000);
}
*/