//
// $Id$
//
#include "Hello.h"
#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"

ACE_RCSID(Hello, Hello, "$Id$")

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Upcall in process ..\n"));

  // This is collocated test. If a connection is setup just abort
  // since it shouldt happen at all!
  if (this->orb_->orb_core ()->lane_resources ().transport_cache ().current_size () > 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR: A connection has been setup,"
                  " exiting ..\n"));

      ACE_OS::exit (-1);
    }
  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
