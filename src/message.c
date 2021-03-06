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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "message.h"
#include "error.h"


int msg_header_verify(msg_header_t *header)
{
    // Verify message prefix
    if (header->prologue != MSG_HEADER_PROLOGUE)
    {
        error_printf("Received invalid message header (invalid prologue)0x%x\n", header->prologue);
        return 1;
    }

    return 0;
}

static void msg_header_init(msg_header_t* header, uint8_t type)
{
    memset(header, 0, sizeof(msg_header_t));
    header->prologue = htons(MSG_HEADER_PROLOGUE);
    header->type = type;
}

int msg_initialize_response(msg_header_t* header, int sessionId, bool overlap, uint16_t version)
{
    msg_header_init(header, InitializeResponse);

    header->control_code = overlap & 0x01;          // Bit0: Perfer Overlap(0) or Perfer Synchronized
    header->parameter.s.upper = htons(version);     // Server Protocol version
    header->parameter.s.lower = htons(sessionId);   // SessionId

    return sizeof(msg_header_t);
}

int msg_fatal_error(msg_header_t* header, uint8_t error, char* message)
{
    int len = 0;
    msg_header_init(header, FatalError);

    header->control_code = error;

    if(message)
    {
        len = strlen(message);
        memcpy(header, message, len);
        header->payload_length = htobe64(len);
    }
    return sizeof(msg_header_t) + len;
}

int msg_async_initialize_response(msg_header_t *header, uint32_t vendorID)
{
    msg_header_init(header, AsyncInitializeResponse);

    header->parameter.value = htonl(vendorID);

    return sizeof(msg_header_t);
}

int msg_async_maximum_message_size_response(msg_header_t *header, uint64_t size)
{
    uint64_t *payload = (uint64_t*)((uint8_t*)header + sizeof(msg_header_t));

    msg_header_init(header, AsyncMaximumMessageSizeResponse);

    header->payload_length = htobe64(sizeof(uint64_t));
    *payload = htobe64(size);

    return sizeof(msg_header_t) + size;
}

int msg_async_lock_response(msg_header_t *header, uint8_t result)
{
    msg_header_init(header, AsyncLockResponse);

    header->control_code = result;

    return sizeof(msg_header_t);
}
