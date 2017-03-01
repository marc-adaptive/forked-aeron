/*
 * Copyright 2014-2017 Real Logic Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef INCLUDED_AERON_DRIVER_UDPCHANNELTRANSPORT__
#define INCLUDED_AERON_DRIVER_UDPCHANNELTRANSPORT__

#include <protocol/HeaderFlyweight.h>
#include <concurrent/AtomicBuffer.h>
#include <concurrent/logbuffer/FrameDescriptor.h>
#include <unistd.h>

#include "UdpChannel.h"

namespace aeron { namespace driver { namespace media {

using namespace aeron::concurrent;
using namespace aeron::concurrent::logbuffer;
using namespace aeron::protocol;

class UdpChannelTransport
{
public:
    UdpChannelTransport(
        std::unique_ptr<UdpChannel>& channel,
        InetAddress* endPointAddress,
        InetAddress* bindAddress,
        InetAddress* connectAddress)
        : m_channel(std::move(channel)),
          m_endPointAddress(endPointAddress),
          m_bindAddress(bindAddress),
          m_connectAddress(connectAddress),
          m_sendSocketFd(0),
          m_receiveBuffer(m_receiveBufferBytes, m_receiveBufferLength)
    {
        m_receiveBuffer.setMemory(0, m_receiveBuffer.capacity(), 0);
    }

    virtual ~UdpChannelTransport()
    {
        if (0 != m_sendSocketFd)
        {
            close(m_sendSocketFd);
        }
    }

    inline void output(std::ostream& out) const
    {
        out << "Channel: " << *m_channel <<
            ", endPoint: " << *m_endPointAddress <<
            ", bind: " << *m_bindAddress <<
            ", connect: ";
        if (nullptr != m_connectAddress)
        {
            out << *m_connectAddress;
        }
        else
        {
            out << "N/A";
        }
    }

    void openDatagramChannel();
    void send(const void* data, const int32_t len);
    std::int32_t recv(char* data, const int32_t len);
    void setTimeout(timeval timeout);
    InetAddress* receive(int32_t* pInt);
    bool isMulticast();
    UdpChannel& udpChannel();

protected:
    inline AtomicBuffer& receiveBuffer()
    {
        return m_receiveBuffer;
    }

    inline bool isValidFrame(AtomicBuffer& buffer, std::int32_t length)
    {
        bool isValid = true;

        if (FrameDescriptor::frameVersion(buffer, 0) != HeaderFlyweight::CURRENT_VERSION)
        {
            isValid = false;
        }
        else if (length < HeaderFlyweight::headerLength())
        {
            isValid = false;
        }

        return isValid;
    }

private:
    static const int m_receiveBufferLength = 4096;

    std::unique_ptr <UdpChannel> m_channel;
    InetAddress* m_endPointAddress;
    InetAddress* m_bindAddress;
    InetAddress* m_connectAddress;
    int m_sendSocketFd;
    int m_recvSocketFd;
    std::uint8_t m_receiveBufferBytes[m_receiveBufferLength];
    AtomicBuffer m_receiveBuffer;
};

inline std::ostream& operator<<(std::ostream& os, const UdpChannelTransport& dt)
{
    dt.output(os);
    return os;
}


}}}

#endif //AERON_UDPCHANNELTRANSPORT_H
