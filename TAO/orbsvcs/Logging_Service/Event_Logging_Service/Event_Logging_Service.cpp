#include "Event_Logging_Service.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/Log/EventLogFactory_i.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "tao/IORTable/IORTable.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID (Event_Logging_Service,
           Event_Logging_Service,
           "$Id$")


Event_Logging_Service::Event_Logging_Service (void)
  : service_name_ ("EventLogFactory"),
    bind_to_naming_service_ (1),
    nthreads_ (0)
{
  // No-Op.
}

Event_Logging_Service::~Event_Logging_Service (void)
{
  // No-Op.
}

void
Event_Logging_Service::init_ORB  (int& argc, char *argv[]
                                  ACE_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                ""
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var poa_object  =
    this->orb_->resolve_initial_references("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

int
Event_Logging_Service::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Arg_Opt<ACE_TCHAR> get_opt (argc, argv, ACE_TEXT("n:o:p:t:x"));
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          service_name_.set (ACE_TEXT_TO_CHAR_IN (get_opt.opt_arg()));
          break;

        case 'o':
          ior_file_name_.set (ACE_TEXT_TO_CHAR_IN (get_opt.opt_arg()));
          break;

        case 'p':
          pid_file_name_.set (ACE_TEXT_TO_CHAR_IN (get_opt.opt_arg()));
          break;

        case 't':
          nthreads_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'x':
          bind_to_naming_service_ = 0;
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "-n service_name "
                      "-o ior_file_name "
                      "-p pid_file_name "
                      "-t threads "
                      "-x [disable naming service bind] "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  return 0;
}

int
Event_Logging_Service::init (int argc, ACE_TCHAR *argv[] ACE_ENV_ARG_DECL)
{
  ACE_Argv_Type_Converter convert (argc, argv);

  // initalize the ORB.
  this->init_ORB (convert.get_argc(), convert.get_ASCII_argv()
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (this->parse_args (convert.get_argc(), convert.get_TCHAR_argv()) == -1)
    return -1;

  // Activate the event log factory
  ACE_NEW_THROW_EX (this->event_log_factory_,
                    TAO_EventLogFactory_i (),
                    CORBA::NO_MEMORY ());

  // CORBA::Object_var obj =
  DsEventLogAdmin::EventLogFactory_var obj =
    this->event_log_factory_->activate (this->orb_.in (),
                                        this->poa_.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  ACE_ASSERT (!CORBA::is_nil (obj.in ()));

  CORBA::String_var ior =
    this->orb_->object_to_string (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (true)
    {
      CORBA::Object_var table_object =
        this->orb_->resolve_initial_references ("IORTable"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in ());
      ACE_CHECK_RETURN (-1);

      adapter->bind("EventLogService", ior.in ());
      ACE_CHECK_RETURN (-1);
    }

  if (ior_file_name_.length() != 0)
    {
      FILE* iorf = ACE_OS::fopen (ior_file_name_.fast_rep(), ACE_TEXT("w"));
      if (iorf == 0) {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_file_name_.c_str()),
                          -1);
      }

      ACE_OS::fprintf (iorf, "%s\n", ior.in ());
      ACE_OS::fclose (iorf);
    }

  if (pid_file_name_.length() != 0)
    {
      FILE* pidf = ACE_OS::fopen (pid_file_name_.fast_rep(), ACE_TEXT("w"));
      if (pidf != 0)
        {
          ACE_OS::fprintf (pidf,
                           "%ld\n",
                           static_cast<long> (ACE_OS::getpid ()));
          ACE_OS::fclose (pidf);
        }
    }

  if (bind_to_naming_service_)
    {
      // Resolve the naming service.
      this->resolve_naming_service (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      // Register the Event Log Factory.
      ACE_ASSERT(!CORBA::is_nil (this->naming_.in ()));

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (this->service_name_.fast_rep());

      this->naming_->rebind (name,
                             obj.in ()
                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  return 0;
}

void
Event_Logging_Service::resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_THROW (CORBA::UNKNOWN ());

  this->naming_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ()
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

int
Event_Logging_Service::run (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->nthreads_ > 0)
    {
      if (this->activate ((THR_NEW_LWP | THR_JOINABLE), this->nthreads_) != 0)
        return -1;

      this->thr_mgr ()->wait ();
      return 0;
    }

  this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Event_Logging_Service::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

void
Event_Logging_Service::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  // @@ JTC - factory object isn't activated on root poa.
#if 0
  // Deactivate.
  PortableServer::ObjectId_var oid =
    this->poa_->servant_to_id (this->event_log_factory_
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // deactivate from the poa.
  this->poa_->deactivate_object (oid.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
#endif

  if (bind_to_naming_service_)
    {
      // Unbind from the naming service.
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (this->service_name_.fast_rep());

      this->naming_->unbind (name
                             ACE_ENV_ARG_PARAMETER);
    }

  // shutdown the ORB.
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown ();
}
