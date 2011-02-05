#include "connection.hpp"

using namespace evias::network;

/**
 * searchQueue
 *
 * pop the queue's first element or the first with identification reqId.
 *
 * \note returned object has to be free'd
 * \return netPacket*
 */
netPacket *evias::network::netConnection::searchQueue(__netRequest_t reqId)
{
    // execute this algorithm safely in creating a specific thread and locking
    // the mutex
    Lock l(_mutex);

    if (_queue.empty()) {
        return NULL;
    }

    netPacket *p = NULL;

    // either get the first message available
    // or get a specific packet by its message ID

    if (reqId == EVIAS_MSG_NONE) {

        p = _queue.front();
        _queue.pop_front();
    }
    else {

        for (std::list<netPacket*>::iterator i = _queue.begin() ; i != _queue.end() ; i++) {

            // packet is current iterator's value
            p = *i;

            if ( ((__packetHeader_t *) p->m_dataPointer)->requestId == reqId ) {

                _queue.erase(i);
                break;
            }

            p = NULL;
        }

        if (p == NULL) {
            return NULL;
        }
    }

    return p;
}

/**
 * addPacketToQueue
 *
 * adds the specified packet to the current queue. If the queue's length
 * is greater than allowed, pop the first element.
 *
 * \return void
 **/
void evias::network::netConnection::pushQueue(netPacket* packetAdded)
{
    if (! packetAdded)
        return ;

    Lock l(_mutex);

    // add the specified packet
    _queue.push_back(packetAdded);

    while (_queue.size() > QUEUE_MAX) {

        // get the instance for free-ing memory
        netPacket* frontPacket = _queue.front();

        _queue.pop_front();
        releaseMemory(frontPacket);
    }
}

