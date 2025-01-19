//
// Created by np on 11/27/24.
//

#ifndef INETWORK_H
#define INETWORK_H

namespace MGE
{
    /**
     * Interface for networking modules.
     */
    class INetwork
    {
        public:
            virtual ~INetwork() = default;

            virtual void SendPacket() = 0;
    };
} // MGE

#endif //INETWORK_H
