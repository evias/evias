#include "irc.hpp"

using namespace std;

namespace evias {

namespace application {

Irc::Irc( )
{
	hooks		= 0;
	chan_users	= 0;
	connected	= false;
	bSentNick	= false;
	bSentPass	= false;
	bSentUser	= false;
	cur_nick	= 0;
	InvalServ	= ( char * ) malloc ( strlen( "Impossible de se connecter au serveur random.irc.wanadobe.bizness" ) + 1 );
	_server     = new char [strlen("xxxxxxxxx.xxxxxx.xxxxxxxx.xxxxx")];
	_caughtCall = 0;

	cout    << "data initialized" << endl;
}

Irc::~Irc( )
{
	if ( hooks )
		_removeIrcCmdHook( hooks );

	free( InvalServ );

	delete _server;
}

void Irc::_addIrcCmdHook(	__ircCmdHook *hook, char *cmd_name, int ( * function ) ( char *, __ircReplyData *, void * ) )
{
	if ( hook->function )
	{
		if ( !hook->next )
		{
			hook->next				= new __ircCmdHook;
			hook->next->function	= 0;
			hook->next->ircCommand	= 0;
			hook->next->next		= 0;
		}
		_addIrcCmdHook( hook->next, cmd_name, function );
	}
	else
	{
		hook->function		= function;
		hook->ircCommand	= new char[ strlen( cmd_name ) + 1 ];

		strcpy( hook->ircCommand, cmd_name );
	}
}

void Irc::hookCmdWith(	char *cmd_name, int ( * function ) ( char *, __ircReplyData *, void * ) )
{
	if ( !hooks )
	{
		hooks				= new __ircCmdHook;
		hooks->function		= 0;
		hooks->ircCommand	= 0;
		hooks->next			= 0;

		_addIrcCmdHook( hooks, cmd_name, function );
	}
	else
		_addIrcCmdHook( hooks, cmd_name, function );

}

void Irc::_removeIrcCmdHook( __ircCmdHook *cmd_hook )
{
	if ( cmd_hook->next )
		_removeIrcCmdHook( cmd_hook->next );

	if ( cmd_hook->ircCommand )
		delete cmd_hook->ircCommand;

	delete cmd_hook;
}

int Irc::start( char* ConnectServer, int ServerPort, char *Nick, char *User, char *Name, char *NickservPass )
{
	#ifdef WIN32
	HOSTENT *resolv;
	{
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2,2), &wsa)) {
			cout << "socket init failed : " << (unsigned long) GetLastError() << endl;
		}
	}
	#else
		hostent	*resolv;
	#endif

	sockaddr_in rem;
	char		ConnectBuffer[ 514 ];
	char		*InvalServ;

	if ( connected )
		return 1;

	sprintf( _server, "%s", ConnectServer );

	// @todo : IDENTD
	//  @brief : use second socket (one for tcp_connect:6667, one for tcp_listen:22)
	irc_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	local_socket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP );

	if ( irc_socket == INVALID_SOCKET || local_socket == INVALID_SOCKET )
	{
		cout << "Erreur: Socket Invalide!" << endl;
		return 1;
	}

	resolv = gethostbyname( ConnectServer );

	if ( !resolv )
	{
		closesocket( irc_socket );
		closesocket( local_socket );
		cout << "Could not resolve hostname: " << ConnectServer <<  endl;
		return 1;
	}

	memcpy( &rem.sin_addr, resolv->h_addr, 4 );

	rem.sin_family	= AF_INET;
	rem.sin_port	= htons( ServerPort );

	// CONNECT
	if (
			(connect( irc_socket, ( const sockaddr * ) &rem, sizeof( rem ) ) == SOCKET_ERROR)
	   )
	{

		cout << "########## La connexion ou l'écoute a echouée .." << endl;

		closesocket( irc_socket );
		closesocket( local_socket );

		return 1;
	}

	connected	= true;

	cur_nick	= new char[ strlen( Nick ) + 1 ];
	strcpy( cur_nick, Nick );

	if ( NickservPass )
		bNickserv = true;
	else
		bNickserv = false;


	if ( bNickserv )
	{
		sprintf	( ConnectBuffer, "PASS %s\r\n", NickservPass );
		send	( irc_socket, ConnectBuffer, strlen( ConnectBuffer ), 0 );
	}

	sprintf	( ConnectBuffer, "NICK %s\r\n", Nick );
	send	( irc_socket, ConnectBuffer, strlen( ConnectBuffer ), 0 );

	sprintf	( ConnectBuffer, "USER %s * 0 :%s\r\n", User, Name );
	send	( irc_socket, ConnectBuffer, strlen( ConnectBuffer ), 0 );

	return 0;
}

void Irc::closeAll( )
{
	// Si vous êtes connecté à un serveur IRC
	if ( connected )
	{
		cout << "Disconnected from server." << endl;
		connected	= false;
		// laisse un message de quit
		closeConnection	( (char*) "eVias Documentation Bot" );

		#ifdef WIN32
			shutdown	( irc_socket, 2 );
		#endif
			closesocket	( irc_socket );
	}
}

int Irc::closeConnection( char *QuitMsg )
{
	char buffer[ 1024 ];

	// Si vous êtes connecté à l'IRC
	if ( connected )
	{
		// Si un message de quit à été donné
		if ( QuitMsg )
			// Vous quittez avec ce message de quit
			sprintf( buffer, "QUIT %s\r\n", QuitMsg );
		else
			// Sinon vous quittez tout simplement
			sprintf(buffer, "QUIT\r\n");

		if ( send( irc_socket, buffer, strlen( buffer ), 0 ) == SOCKET_ERROR )
			return 1;
	}

	return 0;
}

int Irc::catchIt( )
{
	char	buffer[ 1024 ];
	int		ret_len;
	sockaddr_in cli;
	int     cli_size;
	int     cli_ret;

#ifdef WIN32
	HOSTENT* resolv;
#else
	hostent* resolv;
#endif

	// Si vous n'êtes pas connecté
	if ( !connected )
	{
		cout << "Not connected!" << endl;
		return 1;
	}

	cout << "-- IDENTD treatment .." << endl;
/*
	int sockfd, newfd;
	struct sockaddr_in my_addr,
					   serv_addr;

#ifdef WIN32
	int addr_size;
#else
	socklen_t addr_size;
#endif

	sockfd = socket (AF_INET, SOCK_STREAM, 0);

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(22);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero (&(my_addr.sin_zero), 8);

	bind (sockfd, (struct sockaddr*) &my_addr, sizeof(struct sockaddr));

	listen (sockfd, SOMAXCONN);

	addr_size = sizeof( struct sockaddr_in );
	newfd = accept (sockfd, (struct sockaddr*)&serv_addr, &addr_size);
*/
	cout << "-- IDENTD treatment done" << endl;

	// Boucle étant éxécutez chaque fois
	while ( 1 )
	{
		ret_len = recv( irc_socket, buffer, 1023, 0 );

		if ( ret_len == SOCKET_ERROR || !ret_len ) {
			return 1;
		}

		buffer[ ret_len ] = '\0';
		_splitToReplies( buffer );
	}

	return 0;
}

void Irc::_splitToReplies( char *data )
{
	char *p;

	while ( p = strstr( data, "\r\n" ) )
	{
		*p = '\0';

		_parseIrcReply( data );

		data = p + 2;
	}
}

int Irc::channelIsOp( char *channel, char *nick )
{
	__ircChannelUser *sChanUser;
	sChanUser = chan_users;

	while ( sChanUser )
	{
		if (	!strcmp( sChanUser->channel, channel ) &&
				!strcmp( sChanUser->nick, nick )
		   )
		{
			return sChanUser->flags&IRC_USER_OP;
		}

		sChanUser = sChanUser->next;
	}

	return 0;
}

int Irc::channelIsVoice( char *channel, char *nick)
{
	__ircChannelUser *sChanUser;
	sChanUser = chan_users;

	while ( sChanUser )
	{
		if ( !strcmp( sChanUser->channel, channel ) && !strcmp( sChanUser->nick, nick ) )
			return sChanUser->flags&IRC_USER_VOICE;

		sChanUser = sChanUser->next;
	}

	return 0;
}

void Irc::_parseIrcReply( char *data )
{
	char			*hostd;
	char			*cmd;
	char			*params;
	char			buffer[514];
	__ircReplyData	hostd_tmp;
	__ircChannelUser* sChannelUser;
	char			*p;
	char			*chan_temp;
	string          message = "";

	hostd_tmp.target = 0;

	if ( data[ 0 ] == ':' )
	{
		hostd	= &data[ 1 ];
		cmd		= strchr( hostd, ' ' );

		if ( !cmd )
			return;

		*cmd = '\0';
		cmd++;

		params = strchr( cmd, ' ' );

		if ( params )
		{
			*params = '\0';
			params++;
		}

		hostd_tmp.nick	= hostd;
		hostd_tmp.ident	= strchr( hostd, '!' );

		if ( hostd_tmp.ident )
		{
			*hostd_tmp.ident	= '\0';
			hostd_tmp.ident++;

			hostd_tmp.host		= strchr( hostd_tmp.ident, '@' );

			if ( hostd_tmp.host )
			{
				*hostd_tmp.host = '\0';
				hostd_tmp.host++;
			}
		}

		if ( !strcmp( cmd, "JOIN" ) )
		{
			sChannelUser = chan_users;

			if ( sChannelUser )
			{
				while ( sChannelUser->nick )
				{
					if ( !sChannelUser->next )
					{
						sChannelUser->next			= new __ircChannelUser;
						sChannelUser->next->channel	= 0;
						sChannelUser->next->flags	= 0;
						sChannelUser->next->next	= 0;
						sChannelUser->next->nick	= 0;
					}

					sChannelUser= sChannelUser->next;
				}

				sChannelUser->channel	= new char[ strlen( params ) + 1 ];
				strcpy( sChannelUser->channel, params );

				sChannelUser->nick		= new char[ strlen( hostd_tmp.nick ) + 1 ];
				strcpy( sChannelUser->nick, hostd_tmp.nick );
			}
			message = "-- JOIN : ";
			message.append (hostd_tmp.nick).append(" joins ").append(params);
		}
		else if ( !strcmp( cmd, "PART" ) )
		{
			__ircChannelUser	*d;
			__ircChannelUser	*prev;

			d				= 0;
			prev			= 0;
			sChannelUser	= chan_users;

			while ( sChannelUser )
			{
				if (	!strcmp( sChannelUser->channel, params )		&&
						!strcmp( sChannelUser->nick, hostd_tmp.nick )
				   )
				{
					d = sChannelUser;
					break;
				}
				else
				{
					prev = sChannelUser;
				}

				sChannelUser= sChannelUser->next;
			}

			if ( d )
			{
				if ( d == chan_users )
				{
					chan_users = d->next;

					if ( d->channel )
						delete [] d->channel;

					if ( d->nick )
						delete [] d->nick;

					delete d;
				}
				else
				{
					if ( prev )
						prev->next = d->next;

					chan_users = d->next;

					if ( d->channel )
						delete [] d->channel;

					if ( d->nick )
						delete [] d->nick;

					delete d;
				}
			}
			message = "-- PART : ";
			message.append (hostd_tmp.nick).append(" parts from ").append(params);
		}
		else if ( !strcmp( cmd, "QUIT" ) )
		{
			__ircChannelUser	*d;
			__ircChannelUser	*prev;

			d				= 0;
			prev			= 0;
			sChannelUser	= chan_users;

			while ( sChannelUser )
			{
				if ( !strcmp( sChannelUser->nick, hostd_tmp.nick ) )
				{
					d = sChannelUser;

					if ( d == chan_users )
					{
						chan_users = d->next;

						if ( d->channel )
							delete [] d->channel;

						if ( d->nick )
							delete [] d->nick;

						delete d;
					}
					else
					{
						if ( prev )
						{
							prev->next = d->next;
						}
						chan_users = d->next;
						if ( d->channel )
							delete [] d->channel;

						if ( d->nick )
							delete [] d->nick;

						delete d;
					}
				}
				else
				{
					prev = sChannelUser;
				}

				sChannelUser = sChannelUser->next;
			}
			message = "-- QUIT : ";
			message.append (hostd_tmp.nick).append(" quits connection ").append(params);
		}
		else if ( !strcmp( cmd, "MODE" ) )
		{
			char		*chan;
			char		*changevars;
			char		*tmp;
			__ircChannelUser	*sChannelUser;
			__ircChannelUser	*d;
			int			i;
			bool		plus;

			chan		= params;
			params		= strchr( chan, ' ' );
			*params		= '\0';
			params++;

			changevars	= params;
			params		= strchr( changevars, ' ' );

			if ( !params )
				return;

			if ( chan[ 0 ] != '#' )
				return;

			*params = '\0';
			params++;

			plus = false;

			for ( i = 0; i < ( signed ) strlen( changevars ); i++ )
			{
				switch ( changevars[ i ] )
				{
					case '+':
						plus = true;
						break;

					case '-':
						plus = false;
						break;

					case 'o':
						tmp = strchr( params, ' ' );

						if ( tmp )
						{
							*tmp = '\0';
							tmp++;
						}

						tmp = params;

						// si un + est devant le 'o' ( /mode +o )
						if ( plus )
						{
							// utilisateur a été oppé (chan, params)
							sChannelUser	= chan_users;
							d				= 0;

							while ( sChannelUser )
							{
								if ( sChannelUser->next	 &&	sChannelUser->channel )
								{
									if (	!strcmp( sChannelUser->channel, chan) &&
											!strcmp( sChannelUser->nick, tmp )
									   )
									{
										d = sChannelUser;
										break;
									}
								}

								sChannelUser = sChannelUser->next;
							}

							if ( d )
								d->flags = d->flags|IRC_USER_OP;
						}
						else // ( /mode -o )
						{
							// utilisateur a été déoppé (chan, params)
							sChannelUser	= chan_users;
							d				= 0;

							while ( sChannelUser )
							{
								if ( !strcmp( sChannelUser->channel, chan) && !strcmp( sChannelUser->nick, tmp ) )
								{
									d = sChannelUser;
									break;
								}

								sChannelUser = sChannelUser->next;
							}

							if ( d )
								d->flags = d->flags^IRC_USER_OP;
						}

						params = tmp;
						break;

					case 'v':
						tmp = strchr( params, ' ' );
						if ( tmp )
						{
							*tmp = '\0';
							tmp++;
						}

						if ( plus )
						{
							// utilisateur voicé
							sChannelUser	= chan_users;
							d				= 0;

							while ( sChannelUser )
							{
								if ( !strcmp( sChannelUser->channel, params) && !strcmp( sChannelUser->nick, hostd_tmp.nick ) )
								{
									d = sChannelUser;
									break;
								}

								sChannelUser = sChannelUser->next;
							}

							if ( d )
								d->flags = d->flags|IRC_USER_VOICE;
						}
						else // ( /mode -v )
						{
							// utilisateur a été dévoicé
							sChannelUser	= chan_users;
							d				= 0;
							while ( sChannelUser )
							{
								if ( !strcmp( sChannelUser->channel, params) && !strcmp( sChannelUser->nick, hostd_tmp.nick ) )
								{
									d = sChannelUser;
									break;
								}

								sChannelUser = sChannelUser->next;
							}

							if ( d )
								d->flags = d->flags^IRC_USER_VOICE;
						}
						params = tmp;
						break;

					default:
						return;
						break;
				}
			}
			message = "-- MODE : ";
			message.append (hostd_tmp.nick).append(" changes mode ").append(params);
		}
		// '353' -> la liste des noms d'un chan..
		else if ( !strcmp( cmd, "353" ) )
		{
			// Percepte la liste des nick sur un chan
			if ( !chan_users )
			{
				chan_users			= new __ircChannelUser;
				chan_users->next	= 0;
				chan_users->nick	= 0;
				chan_users->flags	= 0;
				chan_users->channel	= 0;
			}

			sChannelUser	= chan_users;
			chan_temp		= strchr( params, '#' );

			if ( chan_temp )
			{
				p = strstr( chan_temp, " :" );

				if ( p )
				{
					*p	 = '\0';
					p	+= 2;

					while ( strchr( p, ' ' ) )
					{
						char *tmp;

						tmp	 = strchr( p, ' ' );
						*tmp = '\0';
						tmp++;

						// Tant que le nick est valide
						while ( sChannelUser->nick )
						{
							if ( !sChannelUser->next )
							{
								sChannelUser->next			= new __ircChannelUser;
								sChannelUser->next->channel	= 0;
								sChannelUser->next->flags	= 0;
								sChannelUser->next->next	= 0;
								sChannelUser->next->nick	= 0;
							}

							sChannelUser = sChannelUser->next;
						}

						// Si l'utilisateur est opérateur sur le certain chan
						if ( p[ 0 ] == '@' )
						{
							sChannelUser->flags = sChannelUser->flags|IRC_USER_OP;
							p++;
						}
						// S'il est voice
						else if ( p[ 0 ] == '+' )
						{
							sChannelUser->flags = sChannelUser->flags|IRC_USER_VOICE;
							p++;
						}

						sChannelUser->nick		= new char[ strlen( p ) + 1 ];
						sChannelUser->channel	= new char[ strlen( chan_temp ) + 1 ];

						strcpy( sChannelUser->nick, p);
						strcpy( sChannelUser->channel, chan_temp );
						p = tmp;
					}

					while ( sChannelUser->nick )
					{
						if ( !sChannelUser->next )
						{
							sChannelUser->next			= new __ircChannelUser;
							sChannelUser->next->channel	= 0;
							sChannelUser->next->flags	= 0;
							sChannelUser->next->next	= 0;
							sChannelUser->next->nick	= 0;
						}

						sChannelUser = sChannelUser->next;
					}

					if ( p[ 0 ] == '@' )
					{
						sChannelUser->flags = sChannelUser->flags|IRC_USER_OP;
						p++;
					}
					else if ( p[ 0 ] == '+' )
					{
						sChannelUser->flags = sChannelUser->flags|IRC_USER_VOICE;
						p++;
					}

					sChannelUser->nick		= new char[ strlen( p ) + 1 ];
					sChannelUser->channel	= new char[ strlen( chan_temp ) + 1 ];

					strcpy( sChannelUser->nick, p );
					strcpy( sChannelUser->channel, chan_temp );

					if (message.empty())
						message.append("-- LIST of ").append(chan_temp).append(" : ");
					else
						message.append(", ");

					message.append(sChannelUser->nick);
				}
			}
			else
				// no chan name
				message = "-- FAULT LIST ;";
		}
		// S'il y a une notice
		else if ( !strcmp( cmd, "NOTICE" ) )
		{
			hostd_tmp.target	= params;
			params				= strchr( hostd_tmp.target, ' ' );

			if ( params )
				*params = '\0';

			params++;

			message = "-- NOTICE : ";
			message.append (hostd_tmp.nick).append(" notices ").append(hostd_tmp.target)
				   .append (" that ").append(params);
		}
		else if ( !strcmp( cmd, "PRIVMSG" ) )
		{
			hostd_tmp.target	= params;
			params				= strchr( hostd_tmp.target, ' ' );

			if ( params )
				*params = '\0';

			params++;

			message = "-- PRIVMSG : ";
			message.append (hostd_tmp.nick).append(" talks to ").append(hostd_tmp.target)
				   .append (" and says : ").append(params);
		}
		// S'il y a un changement de pseudo
		else if ( !strcmp( cmd, "NICK" ) )
		{
			if ( !strcmp( hostd_tmp.nick, cur_nick ) )
			{
				delete [] cur_nick;

				cur_nick = new char[ strlen( params ) + 1 ];

				strcpy( cur_nick, params );
			}

			message = "-- NICK : ";
			message.append (hostd_tmp.target).append(" is now known as ").append(hostd_tmp.nick);
		}
		else if (
				 ! strcmp(cmd, "311") ||
				 ! strcmp(cmd, "312") ||
				 ! strcmp(cmd, "318") ||
				 ! strcmp(cmd, "319") ||
				 ! strcmp(cmd, "301")
				) {
			message = "-- WHOIS :";
			message.append(params);
		}
		else {
			// not treated yet / not been needed
			message = "-- SERVER : ";
			message.append("[").append(cmd).append("]");
			message.append (params);
		}

		cout << message << endl;

		_callHook( cmd, params, &hostd_tmp );
	}
	else
	{
		// no ':' in front of the response

		cmd	 = data;
		data = strchr( cmd, ' ' );

		if ( !data )
			return;

		*data  = '\0';
		params = data + 1;

		// Si un ping est reçu
		if ( !strcmp( cmd, "PING" ) )
		{
			if ( !params )
				return;

			sprintf( buffer, "PONG %s\r\n", &params[ 1 ] );

			cout << "PING > PONG :" << params << endl;

			send( irc_socket, buffer, strlen( buffer ), 0 );
		}
		else
		{
			cout << "--- RAW [" << cmd << "] {" << data << "} : " << params << "---" << endl;

			hostd_tmp.host	 = 0;
			hostd_tmp.ident	 = 0;
			hostd_tmp.nick	 = 0;
			hostd_tmp.target = 0;

			_callHook( cmd, params, &hostd_tmp );
		}
	}
}

void Irc::_callHook( char *irc_command, char *params, __ircReplyData *hostd )
{
	__ircCmdHook *p;

	if ( !hooks )
		return;

	p = hooks;

	while ( p )
	{
		// we need to get the right command hook, this is differenciated by the irc_command
		// attribute of the structure
		if ( !strcmp( p->ircCommand, irc_command ) )
		{
			( * ( p->function ) ) ( params, hostd, this );
			p = 0;
		}
		else
		{
			p = p->next;
		}
	}
}

int Irc::notice( char *target, char *message )
{
	char buffer[ 514 ];

	if ( !connected )
		return 1;

	sprintf	( buffer, "NOTICE %s :%s\r\n", target, message );
	send	( irc_socket, buffer, strlen( buffer ), 0 );

	return 0;
}

int Irc::privateMessage( char *target, char *message )
{
	char buffer[ 514 ];

	if ( !connected )
		return 1;

	sprintf	( buffer, "PRIVMSG %s :%s\r\n", target, message );
	send	( irc_socket, buffer, strlen( buffer ), 0 );

	return 0;
}

int Irc::channelJoin( char *channel )
{
	char buffer[ 514 ];

	if ( !connected )
		return 1;

	sprintf	( buffer, "JOIN %s\r\n", channel );
	send	( irc_socket, buffer, strlen( buffer ), 0 );

	return 0;
}

int Irc::channelPart( char *channel )
{
	char buffer[ 514 ];

	if ( !connected )
		return 1;

	sprintf	( buffer, "PART %s\r\n", channel );
	send	( irc_socket, buffer, strlen( buffer ), 0 );

	return 0;
}

int Irc::channelKick( char *channel, char *nick )
{
	char buffer[ 514 ];

	if ( !connected )
		return 1;

	sprintf	( buffer, "KICK %s %s\r\n", channel, nick );
	send	( irc_socket, buffer, strlen( buffer ), 0 );

	return 0;
}

int Irc::channelKick( char *channel, char *nick, char *message)
{
	char buffer[ 514 ];

	if ( !connected )
		return 1;

	sprintf	( buffer, "KICK %s %s :%s\r\n", channel, nick, message );
	send	( irc_socket, buffer, strlen( buffer ), 0 );

	return 0;
}

int Irc::channelMode( char *channel, char *modes, char *targets )
{
	char buffer[ 514 ];

	if ( !connected )
		return 1;

	if ( !targets )
		sprintf( buffer, "MODE %s %s\r\n", channel, modes );
	else
		sprintf( buffer, "MODE %s %s %s\r\n", channel, modes, targets );

	send( irc_socket, buffer, strlen( buffer ), 0 );

	return 0;
}

int Irc::channelMode( char *modes )
{
	if ( !connected )
		return 1;

	channelMode( cur_nick, modes, 0 );

	return 0;
}

int Irc::userNick( char *newnick )
{
	char buffer[ 514 ];

	if ( !connected )
		return 1;

	sprintf	( buffer, "NICK %s\r\n", newnick );
	send	( irc_socket, buffer, strlen( buffer ), 0 );

	return 0;
}

int Irc::whoisUser( char *userList )
{
	char buffer[ 514 ];

	if ( !connected )
		return 1;

	sprintf	( buffer, "WHOIS %s\r\n", userList );
	send	( irc_socket, buffer, strlen( buffer ), 0 );

	return 0;
}

}; // end namespace application

}; // end namespace evias

