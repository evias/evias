#include "net_functions.hpp"

using namespace evias::network;

/**
 * doHttpGetRequest
 *
 * execute a GET request to the given server at the given path. The result
 * data container is stored in the first argument.
 *
 * \return int
 **/
int evias::network::doHttpGetRequest(baseContainer& outContainer, string serverHost, string serverPath, uint64_t timeoutMillisec)
{
    // open connection on port 80 to server serverHost
    tcpipConnection conn(serverHost, 80, timeoutMillisec);
    if (! conn.isUp())
        return ERR_NOT_CONNECTED;

    // create packet for GET request
    string request = "GET http://" + serverHost + serverPath + " HTTP/1.0\r\n\r\n";
    netPacket* toSend = new netPacket(EVIAS_MSG_NONE);

    toSend->append(request);

    conn.send( toSend );

    // handle the first response line (the http request status number)
    string line         = conn.readLine("\r\n", timeoutMillisec);
    string statusCode   = line.substr(9, 3);
    if (statusCode != "200") {
        return ERR_STATUS;
    }

    delete toSend;

    // The following loop reads out the HTTP header and gets the data size.
    uint64_t fileSize = 0;
    while (true) {
        string httpLine = conn.readLine("\r\n", timeoutMillisec);

        if (httpLine == "" || httpLine == "\r\n") {
            break; // end of header
        }

        string attrName = httpLine.substr(0, 16);

        if (attrName == "Content-Length: ") {
            // store the content length attribute
            // mandatory since it allows to know what
            // size of file is to be downloaded

            long long int i = 0;
            if( sscanf(httpLine.c_str(), "Content-Length: %lld", &i) == 1 ) {
                fileSize = static_cast<uint64_t>(i);
            }
        }
    }

    // no Content-Length header attribute gotten
    if (fileSize == 0)
        return ERR_RESPONSE;

    // make sure we have enough memory to store the
    // data we need to download.
    outContainer.resize(fileSize);

    // download the file
    if (conn.doGet(outContainer.getData(), static_cast<uint32_t>(fileSize), timeoutMillisec) != RET_SUCCESS) {
        return ERR_RESPONSE;
    }

    return RET_SUCCESS;
}

/**
 * doHttpGetRequest
 *
 * execute a HTTP GET request to the server serverHost and path serverPath,
 * the data container resulting is returned
 *
 * \note the baseContainer* returned needs to be destroyed after use
 *
 * \return baseContainer*
 **/
baseContainer* evias::network::doHttpGetRequest(string serverHost, string serverPath, uint64_t timeoutMillisec)
{
    baseContainer* retObj = new baseContainer(0);

    if (RET_SUCCESS != evias::network::doHttpGetRequest(*retObj, serverHost, serverPath, timeoutMillisec))
        releaseMemory(retObj);

    return retObj;
}

/**
 * doHttpDownload
 *
 * execute a GET request for the given server host and path and save
 * the resulting file locally a localFile
 *
 * \note if the file already exists, it is overwritten
 *
 * \return int
 *          - 0 on success
 *          - -1 on empty fileContainer
 *          - -2 on local file opening error
 *          - -3 on local file writing error
 **/
int evias::network::doHttpDownload(string serverHost, string serverPath, string localFile, uint64_t timeoutMillisec)
{
    baseContainer* fileContainer = evias::network::doHttpGetRequest(serverHost, serverPath, timeoutMillisec);
    if(fileContainer == NULL)
        return -1;

    FILE* fileHandle = fopen(localFile.c_str(), "w+b");
    if (! fileHandle) {

        releaseMemory(fileContainer);
        return -2;
    }

    bool writeSuccess = fwrite( fileContainer->getData(), static_cast<size_t>(fileContainer->getSize()), 1, fileHandle ) == 1;

    releaseMemory(fileContainer);

    fclose(fileHandle);
    fileHandle = NULL;

    return writeSuccess ? RET_SUCCESS : -3;
}

