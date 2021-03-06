#ifndef __EVIAS_IRC_HPP__
#define __EVIAS_IRC_HPP__

#ifdef WIN32

	#include <windows.h>
    #include <winsock2.h>

#else

	#include <unistd.h>
	#include <errno.h>
	#include <string.h>
	#include <netdb.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <sys/socket.h>

	#define closesocket( s ) close( s )
	#define SOCKET_ERROR	-1
	#define INVALID_SOCKET	-1

#endif /* WIN32 */

#include <stdio.h>
#include <stdlib.h>

// 2010 refactoring
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "../core/string_utils.hpp"
#include "../core/common_utils.hpp"

#define IRC_DEBUG 1

// IRC Flags
#define IRC_USER_VOICE	1
#define IRC_USER_HALFOP	2
#define IRC_USER_OP		4

namespace evias {

namespace application {

    using std::cout;
    using std::string;
    using std::endl;
    using std::vector;

    using evias::core::in_vector;

    // reply fetch
    struct __ircReplyData
    {
        char *nick;
        char *ident;
        char *host;
        char *target;
    };

    // command catch feature
    struct __ircCmdHook
    {
        char		    *ircCommand;
        __ircCmdHook	*next;
        int ( * function ) ( char *, __ircReplyData *, void * );
    };

    // people ..
    struct __ircChannelUser
    {
        char			*nick;
        char			*channel;
        char			flags;
        __ircChannelUser		*next;
    };

    class Irc
    {
        public :

            static string MSG_ENDMOTD;
            static string MSG_ENDLIST;
            static string MSG_CHANPART;
            static string MSG_NOTONCHAN;
            static string MSG_INVALIDCHAN;

            Irc( );
            ~Irc( );

            // for-data-reply loop, runs to catch messages coming from server
            // if code is caught, stops catching
            int catchIt(string = "");

            int start	( char *pServer, int iPort, char *pNick, char *pUser, char *pName, char *pPass );

            int closeConnection	( char *QuitMsg );

            void closeAll();

            bool isConnected()
                { return connected; }

            void setQuietMode(bool q)
                { _quiet = q; }

            // PUBLIC API

            // communication access
            int privateMessage	( char *target, char *message );

            int notice	( char *target, char *message );

            // channel access
            int channelJoin	( char *channel );
            int channelPart	( char *channel );
            int channelKick	( char *channel, char *nick );
            int channelKick	( char *channel, char *nick, char *message );
            int channelMode	( char *modes );
            int channelMode	( char *channel, char *modes, char *targets );
            int channelIsOp	( char *channel, char *nick );
            int channelIsVoice	( char *channel, char *nick );
            int whoisUser	( char *userList );

            // user access
            int userNick	( char *newnick );
            char* currentNick() { return cur_nick; };

            // allows treatment definition, make sure the command is the IRC protocol
            // call, as such as: "NICK", "LIST", "PART", "JOIN", "PING" ...
            void hookCmdWith( char *cmd_name, int ( * function ) ( char *, __ircReplyData *, void * ) );

            int getConnection() {
                return irc_socket;
            }

            string getLastTreatedCommand()
                { return _lastTreatedMsgCode; }

            void log(string msg)
            {
                if (_quiet) return ;

                cout << msg << endl;
            }

        private :

            // call linked hook for irc command
            void _callHook ( char *irc_command, char *params, __ircReplyData *hostd );

            // parse response ..
            void _parseIrcReply	( char *data );

            // handle data
            void _splitToReplies( char *data );

            void _addIrcCmdHook( __ircCmdHook *hook, char *cmd_name, int ( * function ) ( char *, __ircReplyData *, void * ) );
            void _removeIrcCmdHook( __ircCmdHook *cmd_hook );

            int				irc_socket;
            int             local_socket;
            int             _caughtCall;
            bool            _canExit;
            bool            _quiet;
            bool			connected;
            bool			bSentNick;
            bool			bSentPass;
            bool			bSentUser;
            bool			bNickserv;
            bool            _bCanJoin;
            char*           _server;
            char*           cur_nick;
            char*           InvalServ;
            string          _lastTreatedMsgCode;
            vector<string>  _endMsgCode;
            __ircChannelUser		*chan_users;
            __ircCmdHook		*hooks;

    }; // end class Irc

}; // end namespace application

}; // end namespace evias

#endif

