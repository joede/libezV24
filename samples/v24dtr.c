/* -*- Mode: C -*-
 * --------------------------------------------------------------------------
 * Copyright  (c) 2017  Joerg Desch <github@jdesch.de>
 * -----------------------------------------------------------------------
 * PROJECT: ezV24 -- Sample Application
 * MODULE.: V24DTR.C: setup handshake signals
 * AUTHOR.: Joerg Desch
 * --------------------------------------------------------------------------
 * DESCRIPTION:
 *
 * --------------------------------------------------------------------------
 * COMPILER-FLAGS:
 *
 * --------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#ifdef UNINSTALLED
#include "ezV24.h"
#else
#include <ezV24/ezV24.h>
#endif

#define __V24DTR_C__


/*+=========================================================================+*/
/*|                      CONSTANT AND MACRO DEFINITIONS                     |*/
/*`========================================================================='*/

/*             .-----------------------------------------------.             */
/* ___________/  local macro declaration                        \___________ */
/*            `-------------------------------------------------'            */

/*+=========================================================================+*/
/*|                          LOCAL TYPEDECLARATIONS                         |*/
/*`========================================================================='*/

/*+=========================================================================+*/
/*|                            PUBLIC VARIABLES                             |*/
/*`========================================================================='*/

/*+=========================================================================+*/
/*|                             LOCAL VARIABLES                             |*/
/*`========================================================================='*/

char MyPortName[V24_SZ_PORTNAME]={'\0'};
v24_port_t *UsedPort=NULL;

unsigned int MyOpenFlags=V24_STANDARD;
int MyTimeOut=0;

int WantWait=0;


/*+=========================================================================+*/
/*|                      PROTOTYPES OF LOCAL FUNCTIONS                      |*/
/*`========================================================================='*/

static void usage (void);
static void installSignalhandler (void);
static void mySignalHandler (int reason);
void dumpErrorMessage ( int rc );


/*+=========================================================================+*/
/*|                     IMPLEMENTATION OF THE FUNCTIONS                     |*/
/*`========================================================================='*/



int main( int argc, char* argv[] )
{
    int NewRTS=-1, NewDTR=-1;
    int Done=0;
    int rc;

    fprintf(stderr,"v24dtr -- set handshake signals\ncompiled: "__DATE__"\n\n");
    optind=1;     /* start without prog-name */
    do
    {
        switch( getopt(argc,argv,"lDwhp:d:r:") )
        {
	    case 'p':
		if ( optarg )
		{
		    strncpy(MyPortName,optarg,V24_SZ_PORTNAME);
		    MyPortName[V24_SZ_PORTNAME]='\0';
		    fprintf(stderr,"info: use port `%s'\n",MyPortName);
		}
		else
		{
		    fputs("error: missing parameter for -p\n",stderr);
		    return 2;
		}
		break;
	    case 'd':
		if ( optarg )
		{
		    switch ( optarg[0] )
		    {
			case '0':
			case '-':
			    NewDTR=0;
			    fputs("info: disable DTR\n",stderr);
			    break;
			case '1':
			case '+':
			    NewDTR=1;
			    fputs("info: enable DTR\n",stderr);
			    break;
		    }
		}
		else
		{
		    fputs("error: missing parameter for -d\n",stderr);
		    return 2;
		}
		break;
	    case 'r':
		if ( optarg )
		{
		    switch ( optarg[0] )
		    {
			case '0':
			case '-':
			    NewRTS=0;
			    fputs("info: disable RTS\n",stderr);
			    break;
			case '1':
			case '+':
			    NewRTS=1;
			    fputs("info: enable RTS\n",stderr);
			    break;
		    }
		}
		else
		{
		    fputs("error: missing parameter for -r\n",stderr);
		    return 2;
		}
		break;
	    case 'l':
		MyOpenFlags |= V24_LOCK;
		fputs("info: open with V24_LOCK\n",stderr);
		break;
	    case 'D':
		MyOpenFlags |= V24_DEBUG_ON;
		fputs("info: open with V24_DEBUG_ON\n",stderr);
		break;
	    case 'w':
		WantWait=1;
		fputs("info: wait for keypress.\n",stderr);
		break;
            case 'h':     // user want's help
            case '?':     // getopt3() reports invalid option
                usage();
                exit(1);
            default:
                Done=1;
	}
    } while (!Done);


    if ( *MyPortName=='\0' )
    {
	fputs("error: you have to specify a portname using parm `-p'!\n",stderr);
	return 1;
    }
    installSignalhandler();

    /* First we have to open the port.
     */
    if ( MyPortName[0]=='\0' )
    {
	fputs("error: use option -p to specify a valid port!\n",stderr);
	return 1;
    }
    UsedPort=v24OpenPort(MyPortName,MyOpenFlags);
    if ( UsedPort==NULL )
    {
	fputs("error: sorry, open failed!\n",stderr);
	return 1;
    }
    fputs("info: port opened!\n",stderr);

    /* than we have to configure the port.
     */
    rc=v24SetParameters(UsedPort,V24_B9600,V24_8BIT,V24_NONE);
    if ( rc!=V24_E_OK )
    {
	dumpErrorMessage(rc);
	v24ClosePort(UsedPort);
	return 1;
    }

    v24SetDTR(UsedPort,NewDTR);
    v24SetRTS(UsedPort,NewRTS);
    if ( WantWait )
    {
	puts("\npress RETURN to continue\n");
	getchar();
    }

    /* At the end of all the stuff, we have close the port. ;-)
     */
    rc=v24ClosePort(UsedPort);
    if ( rc!=V24_E_OK )
	dumpErrorMessage(rc);
    else
	fputs("info: port closed!\n",stderr);
    return 0;
}



void dumpErrorMessage ( int rc )
{
    switch ( rc )
    {
	case V24_E_OK: fputs("error-msg: V24_E_OK\n",stderr); break;
	case V24_E_ILLBAUD: fputs("error-msg: V24_E_ILLBAUD\n",stderr); break;
	case V24_E_ILLDATASZ: fputs("error-msg: V24_E_ILLDATASZ\n",stderr); break;
	case V24_E_ILLHANDLE: fputs("error-msg: V24_E_ILLHANDLE\n",stderr); break;
	case V24_E_ILLTIMEOUT: fputs("error-msg: V24_E_ILLTIMEOUT\n",stderr); break;
	case V24_E_OPEN_LOCK: fputs("error-msg: V24_E_OPEN\n",stderr); break;
	case V24_E_CREATE_LOCK: fputs("error-msg: V24_E_CREATE_LOCK\n",stderr); break;
	case V24_E_KILL_LOCK: fputs("error-msg: V24_E_KILL_LOCK\n",stderr); break;
	case V24_E_LOCK_EXIST: fputs("error-msg: V24_E_LOCK_EXIST\n",stderr); break;
	case V24_E_NOMEM: fputs("error-msg: V24_E_NOMEM\n",stderr); break;
	case V24_E_NULL_POINTER: fputs("error-msg: V24_E_NULL_POINTER\n",stderr); break;
	case V24_E_OPEN: fputs("error-msg: V24_E_OPEN\n",stderr); break;
	case V24_E_READ: fputs("error-msg: V24_E_READ\n",stderr); break;
	case V24_E_WRITE: fputs("error-msg: V24_E_WRITE\n",stderr); break;
	case V24_E_NOT_IMPLEMENTED: fputs("error-msg: V24_E_NOT_IMPLEMENTED\n",stderr); break;
	case V24_E_DBG_STALE_LOCK: fputs("debug-msg: V24_E_DBG_STALE_LOCK\n",stderr); break;
	default:  fputs("error-msg undefined?!?!\n",stderr); break;
    }
}



/*+=========================================================================+*/
/*|                    IMPLEMENTATION OF LOCAL FUNCTIONS                    |*/
/*`========================================================================='*/


static void usage (void)
{
    fprintf(stderr,"SYNOPSIS\n");
    fprintf(stderr,"\tv24dtr [options]\n");
    fprintf(stderr,"\nDESCRIPTION\n");
    fprintf(stderr,"\tThis program allows the user to setup the handshake signals\n");
    fprintf(stderr,"\tRTS and CTS.\n");
    fprintf(stderr,"\nOPTIONS\n");
    fprintf(stderr,"-h\tdisplay this help page.\n");
    fprintf(stderr,"-p dev\tuse serial port <dev>.\n");
    fprintf(stderr,"-d {+|-}\tenable (+) or disable (-) DTR.\n");
    fprintf(stderr,"-r {+|-}\tenable (+) or disable (-) RTS.\n");
    fprintf(stderr,"-l\tlock the serial port.\n");
    fprintf(stderr,"-w\twait for RETURN key before closing the port.\n");
}


static void installSignalhandler ( void )
{
    signal(SIGINT,mySignalHandler);
    signal(SIGTERM,mySignalHandler);
}


static void mySignalHandler ( int reason )
{
    // just close the serial port
    v24ClosePort(UsedPort);
    exit(99);
}


/* ==[End of file]========================================================== */
