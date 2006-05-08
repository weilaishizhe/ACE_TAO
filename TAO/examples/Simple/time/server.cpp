// $Id$
#include "../Simple_util.h"
#include "Time_i.h"
#include "ace/Argv_Type_Converter.h"

// This is the main driver program for the time and date server.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter convert (argc, argv);
  Server<Time_i> server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tTime and Date server\n\n"));

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      if (server.init ("Time",
                       convert.get_argc(),
                       convert.get_ASCII_argv()
                       ACE_ENV_ARG_PARAMETER) == -1)
        return 1;
      else
        {
          server.run (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex,"User Exception in main");
      return -1;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex,"System Exception in main");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

