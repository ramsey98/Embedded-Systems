/*
 * pixy.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

/*
sem_t masterSem;
static SPI_Handle masterSpi;

void slaveReadyFxn(uint_least8_t index)
{
    sem_post(&masterSem);
}

void pixy_init()
{
    SPI_Params spiParams;
    int32_t status;

    GPIO_setConfig(CONFIG_SPI_MASTER_READY, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_SPI_SLAVE_READY, GPIO_CFG_INPUT);

    GPIO_write(CONFIG_SPI_MASTER_READY, 1);
    while (GPIO_read(CONFIG_SPI_SLAVE_READY) == 0) {}

    GPIO_setConfig(CONFIG_SPI_SLAVE_READY, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setCallback(CONFIG_SPI_SLAVE_READY, slaveReadyFxn);
    GPIO_enableInt(CONFIG_SPI_SLAVE_READY);

    status = sem_init(&masterSem, 0, 0);
    if (status != 0)
    {
        halt();
    }

    SPI_Params_init(&spiParams);
    spiParams.frameFormat = SPI_POL0_PHA1;
    spiParams.bitRate = 10000000;
    masterSpi = SPI_open(CONFIG_SPI_MASTER, &spiParams);
    if (masterSpi == NULL)
    {
        halt();
    }

    GPIO_write(CONFIG_SPI_MASTER_READY, 0);
    strncpy((char *) masterTxBuffer, MASTER_MSG, SPI_MSG_LENGTH);
}

void read_pixy()
{
    bool transferOK;
    SPI_Transaction transaction;
    sem_wait(&masterSem);

    masterTxBuffer[sizeof(MASTER_MSG) - 1] = (i % 10) + '0';
    memset((void *) masterRxBuffer, 0, SPI_MSG_LENGTH);
    transaction.count = SPI_MSG_LENGTH;
    transaction.txBuf = (void *) masterTxBuffer;
    transaction.rxBuf = (void *) masterRxBuffer;

    transferOK = SPI_transfer(masterSpi, &transaction);
    if (!transferOK)
    {
        halt();
    }
}
*/
