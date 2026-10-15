#include "Nvm.h"
#include "EEPROM.h"

#include <stddef.h>
#include <string.h>

typedef struct
{
    uint8_t blockId;
    uint8_t data[NVM_BLOCK_SIZE];
    NvM_WriteMode mode;
    uint32_t requestId;
} NvM_QueueEntry;

static NvM_QueueEntry queue[NVM_QUEUE_SIZE];

static uint8_t front;
static uint8_t rear;
static uint8_t count;

static NvM_State state;
static NvM_WriteMode blockMode[NVM_MAX_BLOCKS];
static NvM_BlockStatus blockStatus[NVM_MAX_BLOCKS];

static uint8_t currentBlock;
static uint8_t currentData[NVM_BLOCK_SIZE];
static NvM_WriteMode currentMode;

static uint32_t nextRequestId;
static uint32_t completedRequestId;
static uint32_t currentRequestId;

static const uint8_t blockAddress[NVM_MAX_BLOCKS] ={0x10U,0x20U,0x30U};

static uint8_t NvM_IsBlockQueued(uint8_t blockId)
{
    uint8_t index = front;
    uint8_t i;
    for (i = 0U; i < count; i++)
    {
        if (queue[index].blockId == blockId)
            return 1U;
        index = (uint8_t)((index + 1U) % NVM_QUEUE_SIZE);
    }
    return 0U;
}

void NvM_Init(void)
{
    uint8_t i;
    front = 0U;
    rear = 0U;
    count = 0U;
    state = IDLE;
    currentBlock = 0U;
    currentMode = DEFERRED;
    nextRequestId = 1U;
    completedRequestId = 0U;
    currentRequestId = 0U;
    for (i = 0U; i < NVM_MAX_BLOCKS; i++)
    {
        blockMode[i] = DEFERRED;
        blockStatus[i] = NVM_STATUS_IDLE;
    }
}
uint8_t NvM_SetBlockMode(uint8_t blockId, NvM_WriteMode mode)
{
    if (blockId >= NVM_MAX_BLOCKS)
        return 0U;
    if ((mode != IMMEDIATE) && (mode != DEFERRED))
        return 0U;
    blockMode[blockId] = mode;
    return 1U;
}

NvM_BlockStatus NvM_GetBlockStatus(uint8_t blockId)
{
    if (blockId >= NVM_MAX_BLOCKS)
        return NVM_STATUS_ERROR;
    return blockStatus[blockId];
}

uint8_t NvM_WriteBlock(uint8_t blockId, const uint8_t *data)
{
    uint32_t requestId;
    NvM_WriteMode requestMode;
    if ((blockId >= NVM_MAX_BLOCKS) || (data == NULL))
        return 0U;
    if (count >= NVM_QUEUE_SIZE)
        return 0U;
    requestMode = blockMode[blockId];
    requestId = nextRequestId++;
    queue[rear].blockId = blockId;
    queue[rear].mode = requestMode;
    queue[rear].requestId = requestId;
    memcpy(queue[rear].data, data, NVM_BLOCK_SIZE);
    rear = (uint8_t)((rear + 1U) % NVM_QUEUE_SIZE);
    count++;
    blockStatus[blockId] = NVM_STATUS_PENDING;
    if (requestMode == IMMEDIATE)
    {
        while (completedRequestId < requestId)
            NvM_MainFunction();
    }
    return 1U;
}
void NvM_MainFunction(void)
{
    switch (state)
    {
        case IDLE:
        {
            if (count > 0U)
            {
                state = WRITE_PENDING;
                OSIF_TimeDelay(1000U);
            }
            break;
        }

        case WRITE_PENDING:
        {
            currentBlock = queue[front].blockId;
            currentMode = queue[front].mode;
            currentRequestId = queue[front].requestId;

            memcpy(currentData, queue[front].data, NVM_BLOCK_SIZE);

            blockStatus[currentBlock] = NVM_STATUS_BUSY;
            state = WRITE_BUSY;
            OSIF_TimeDelay(1000U);
            break;
        }
        case WRITE_BUSY:
        {
            EEPROM_Write(blockAddress[currentBlock],currentData,NVM_BLOCK_SIZE);
            state = DONE;
            OSIF_TimeDelay(1000U);
            break;
        }
        case DONE:
        {
            front = (uint8_t)((front + 1U) % NVM_QUEUE_SIZE);
            count--;
            completedRequestId = currentRequestId;
            if (NvM_IsBlockQueued(currentBlock) != 0U)
                blockStatus[currentBlock] = NVM_STATUS_PENDING;
            else
                blockStatus[currentBlock] = NVM_STATUS_DONE;
            state = IDLE;
            OSIF_TimeDelay(1000U);
            break;
        }
        default:
        {
            state = IDLE;
            OSIF_TimeDelay(1000U);
            break;
        }
    }
}

NvM_State NvM_GetState(void)
{
    return state;
}

uint8_t NvM_GetCurrentBlock(void)
{
    return currentBlock;
}

NvM_WriteMode NvM_GetCurrentMode(void)
{
    return currentMode;
}
