/*
 * Copyright (c) 2017  Martin Lund
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MSG_HEADER_SIZE 16
#define MSG_HEADER_PROLOGUE 0x4853 // "HS"

typedef struct __attribute__((__packed__))
{
    uint16_t prologue;
    uint8_t type;
    uint8_t control_code;
    union{
        struct{
            uint16_t upper;
            uint16_t lower;
        }s;
        uint32_t value;
    }parameter;
    uint64_t payload_length;
} msg_header_t;

typedef enum
{
    Initialize,
    InitializeResponse,
    FatalError,
    Error,
    AsyncLock,
    AsyncLockResponse,
    Data,
    DataEnd,
    DeviceClearComplete,
    DeviceClearAcknowledge,
    AsyncRemoteLocalControl,
    AsyncRemoteLocalResponse,
    Trigger,
    Interrupted,
    AsyncInterrupted,
    AsyncMaximumMessageSize,
    AsyncMaximumMessageSizeResponse,
    AsyncInitialize,
    AsyncInitializeResponse,
    AsyncDeviceClear,
    AsyncServiceRequest,
    AsyncStatusQuery,
    AsyncStatusResponse,
    AsyncDeviceClearAcknowledge,
    AsyncLockInfo,
    AsyncLockInfoResponse
} msg_type_t;


typedef enum
{
    F_UnidentifiedError,
    PoorlyFormedMessageHeader,
    AttemptToUseConnectionWithoutBothChannelsEstablished,
    InvalidInitializationSequence,
    ServerRefusedConnectionDueToMaximumNumberOfClinetsExceeded
}fatal_error_code_t;

typedef enum
{
    UnidentifiedError,
    UnrecognizedMessageType,
    UnrecognizedControlCode,
    UnreconnizedVendorDefinedMessage,
    MessageTooLarge
}error_code_t;

int msg_header_verify(msg_header_t *header);
int msg_initialize_response(msg_header_t *header, int sessionId, bool overlap, uint16_t version);
int msg_async_initialize_response(msg_header_t *header, uint32_t vendorID);
int msg_async_lock_response(msg_header_t *header, uint8_t result);
int msg_async_maximum_message_size_response(msg_header_t *header, uint64_t size);

#endif
