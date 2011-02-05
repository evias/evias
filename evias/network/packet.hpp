#ifndef EVIAS_NETWORK_NS_PACKET
#define EVIAS_NETWORK_NS_PACKET

#include <string>
#include <cstdlib>
#include <cstdio>

#include "../core/defines.h"

typedef uint8_t __netRequest_t;

#define HEADER_SIZE (4 * sizeof(int8_t)+sizeof(__netRequest_t)+sizeof(uint32_t))

#pragma pack(push, 1)
#pragma pack(1)
typedef struct {
    int8_t          ident[5]; // EVIAS
    __netRequest_t  requestId;
    uint32_t        dataSize; // size in bytes
} __packetHeader_t;
#pragma pack(pop)

#  define EVIAS_MSG_NULL               ((__netRequest_t) 0x00)
#  define EVIAS_MSG_LOGIN              ((__netRequest_t) 0x01)
#  define EVIAS_MSG_LOGOUT             ((__netRequest_t) 0x02)
#  define EVIAS_MSG_SIGNUP             ((__netRequest_t) 0x03)
#  define EVIAS_MSG_FEEDBACK           ((__netRequest_t) 0x04)
#  define EVIAS_MSG_MAX                ((__netRequest_t) 0x05)
#  define EVIAS_MSG_NONE               ((__netRequest_t) 0xFF)

namespace evias {

namespace network {

    using namespace std;

    inline bool headerValid(const __packetHeader_t *hdr)
    {
        if (! hdr)
            return false;

        // check the packet ident.
        return hdr->ident[0] == 'E' &&
               hdr->ident[1] == 'V' &&
               hdr->ident[2] == 'I' &&
               hdr->ident[3] == 'A' &&
               hdr->ident[4] == 'S' &&
               hdr->requestId < EVIAS_MSG_MAX;
    }

    inline bool fillHeader(__packetHeader_t *hdr, __netRequest_t req)
    {
        if (! hdr)
            return false;

        hdr->ident[0] = 'E';
        hdr->ident[1] = 'V';
        hdr->ident[2] = 'I';
        hdr->ident[3] = 'A';
        hdr->ident[4] = 'S';
        hdr->requestId= req;
        return true;
    }

    class netPacket {
        friend class netConnection;
        friend class tcpipConnection;

        private:
            int8_t*     m_dataPointer; // point directly to the packet data
            uint32_t    _curPos; // cursor position in the packet data

            netPacket(const netPacket& cpy) {}; // no copy.
            netPacket(netPacket &cpy) {}; // no copy.
            netPacket() {}; // no default constructor (empty request)

        public:
            netPacket(__netRequest_t);
            virtual ~netPacket();

            bool isValid() const;

            netPacket*      setType(__netRequest_t);
            __netRequest_t  getType() const;

            uint32_t getTotalLen() const;
            uint32_t getPayloadLen()const;
            netPacket *rewind();

            // allow several data input types
            // string
            // memory pointer
            // bits
            // 16bits representation
            // bytes
            // 64bits representations
            netPacket*          append(string&);
            netPacket*          append(const void *, uint32_t);
            inline netPacket*   append(int8_t in) {return this->_append(in);};
            inline netPacket*   append(uint8_t in) {return this->_append(in);};
            inline netPacket*   append(int16_t in) {return this->_append(in);};
            inline netPacket*   append(uint16_t in) {return this->_append(in);};
            inline netPacket*   append(int32_t in) {return this->_append(in);};
            inline netPacket*   append(uint32_t in) {return this->_append(in);};
            inline netPacket*   append(int64_t in) {return this->_append(in);};
            inline netPacket*   append(uint64_t in) {return this->_append(in);};

            // read data from packet
            // memory pointer
            // bits, 16bits rep., bytes, 64bits rep.
            // string
            int             read(void *out_pData, uint32_t in_iSize);

            inline int8_t   read()              {int8_t out = 0; this->_read(out); return out;};
            inline int8_t   read(int8_t &out)   {this->_read(out); return out;};
            inline uint8_t  read(uint8_t &out)  {this->_read(out); return out;};
            inline int16_t  read(int16_t &out)  {this->_read(out); return out;};
            inline uint16_t read(uint16_t &out) {this->_read(out); return out;};
            inline int32_t  read(int32_t &out)  {this->_read(out); return out;};
            inline uint32_t read(uint32_t &out) {this->_read(out); return out;};
            inline int64_t  read(int64_t &out)  {this->_read(out); return out;};
            inline uint64_t read(uint64_t &out) {this->_read(out); return out;};
            inline string   read(string& out)   {out = this->readString(); return out;};

            string readString();
            string extractString();

            int write(netPacket *);
            int read (netPacket *);

            int print(FILE * = NULL) const;

        private:

            /**
             * _append
             *
             * \access private
             * \internal
             *
             * methods appends any type of data to the current cursor
             * position in the data buffer.
             **/
            template<typename T>
            netPacket* _append(T in)
            {
                int8_t* buffer ;
                buffer = (int8_t*) realloc((void*)m_dataPointer, sizeof(T) + _curPos);
                if ( buffer != NULL ) {
                    m_dataPointer = buffer;

                    // write data to memory (equivalent to memset)
                    *((T *)& m_dataPointer[_curPos]) = in;
                    _curPos += sizeof(T);

                    ((__packetHeader_t*) m_dataPointer)->dataSize = _curPos - HEADER_SIZE ;
                }

                return this;
            }

            /**
             * _read
             *
             * \internal
             * \obsolete dangerous
             *
             * read any data from buffer
             **/
            template<typename T>
            netPacket* _read(T& in)
            {
                size_t len = this->getTotalLen();
                if (_curPos >= len || _curPos + sizeof(T) > len) {
                    in = (T) 0;
                    return this;
                }

                // retrieve data
                in = *((T *) (& m_dataPointer[_curPos]));
                _curPos += sizeof(T);

                return this;
            }

            // get data pointer, cursor set after header
            inline int8_t*          getData() { return &m_dataPointer[HEADER_SIZE];};
            inline const int8_t*    getReadOnlyData() const { return &m_dataPointer[HEADER_SIZE]; };

    }; // netPacket

}; // network

}; // evias

#endif

