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
package io.aeron.driver;

import io.aeron.protocol.StatusMessageFlyweight;

import java.net.InetSocketAddress;

import static io.aeron.logbuffer.LogBufferDescriptor.computePosition;

/**
 * Default unicast sender flow control strategy.
 *
 * Max of right edges.
 * No tracking of receivers.
 */
public class UnicastFlowControl implements FlowControl
{
    /**
     * {@inheritDoc}
     */
    public long onStatusMessage(
        final StatusMessageFlyweight flyweight,
        final InetSocketAddress receiverAddress,
        final long senderLimit,
        final int initialTermId,
        final int positionBitsToShift,
        final long now)
    {
        final long position =
            computePosition(
                flyweight.consumptionTermId(),
                flyweight.consumptionTermOffset(),
                positionBitsToShift,
                initialTermId);

        return Math.max(senderLimit, position + flyweight.receiverWindowLength());
    }

    /**
     * {@inheritDoc}
     */
    public void initialize(final int initialTermId, final int termBufferCapacity)
    {
    }

    /**
     * {@inheritDoc}
     */
    public long onIdle(final long now, final long senderLimit)
    {
        return senderLimit;
    }
}
