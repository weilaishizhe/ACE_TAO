// $Id$

#include "SSLIOP_Connection_Handler.h"
#include "SSLIOP_Current.h"
#include "SSLIOP_Endpoint.h"

#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/Base_Connection_Property.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/CDR.h"
#include "tao/Messaging_Policy_i.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/IIOP_Endpoint.h"


#if !defined (__ACE_INLINE__)
# include "SSLIOP_Connection_Handler.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(TAO_SSLIOP, SSLIOP_Connect, "$Id$")



// ****************************************************************

TAO_SSLIOP_Connection_Handler::TAO_SSLIOP_Connection_Handler (
    ACE_Thread_Manager *t)
  : TAO_SSL_SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    current_ (),
    current_impl_ (),
    transport_ (this, 0, 0),
    refcount_ (1),
    tcp_properties_ (0)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (this->orb_core () != 0);
}


TAO_SSLIOP_Connection_Handler::TAO_SSLIOP_Connection_Handler (
    TAO_ORB_Core *orb_core,
    CORBA::Boolean flag,
    void *arg)
  : TAO_SSL_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    current_ (),
    current_impl_ (),
    transport_ (this, orb_core, flag),
    refcount_ (1),
    tcp_properties_ (ACE_static_cast
                     (TAO_IIOP_Properties *, arg))
{
}


TAO_SSLIOP_Connection_Handler::
    ~TAO_SSLIOP_Connection_Handler (void)
{
  // If the socket has not already been closed.
  if (this->get_handle () != ACE_INVALID_HANDLE)
    {
      // Cannot deal with errors, and therefore they are ignored.
      this->transport_.send_buffered_messages ();
    }
  else
    {
      // Dequeue messages and delete message blocks.
      this->transport_.dequeue_all ();
    }
}



int
TAO_SSLIOP_Connection_Handler::open (void*)
{
  if (this->set_socket_option (this->peer (),
                               tcp_properties_->send_buffer_size,
                               tcp_properties_->recv_buffer_size)
      == -1)
    return -1;

#if !defined (ACE_LACKS_TCP_NODELAY)

  if (this->peer ().set_option (ACE_IPPROTO_TCP,
                                TCP_NODELAY,
                                (void *)
                                &this->tcp_properties_->no_delay,
                                sizeof (int)) == -1)
    return -1;
#endif /* ! ACE_LACKS_TCP_NODELAY */

  // Called by the <Strategy_Acceptor> when the handler is
  // completely connected.
  ACE_INET_Addr addr;

  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  char client[MAXHOSTNAMELEN + 16];
  if (addr.addr_to_string (client, sizeof (client)) == -1)
    return -1;



  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) SSLIOP connection from ")
                  ACE_TEXT ("client <%s> on %d\n"),
                  client,
                  this->peer ().get_handle ()));
    }

  return 0;
}


int
TAO_SSLIOP_Connection_Handler::activate (long flags,
                                         int n_threads,
                                         int force_active,
                                         long priority,
                                         int grp_id,
                                         ACE_Task_Base *task,
                                         ACE_hthread_t thread_handles[],
                                         void *stack[],
                                         size_t stack_size[],
                                         ACE_thread_t  thread_names[])
{
  if (TAO_orbdebug)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_TEXT ("TAO (%P|%t) ")
                 ACE_TEXT ("SSLIOP_Connection_Handler::activate %d ")
                 ACE_TEXT ("threads, flags = %d\n"),
                 n_threads,
                 flags,
                 THR_BOUND));

  return TAO_SSL_SVC_HANDLER::activate (flags,
                                        n_threads,
                                        force_active,
                                        priority,
                                        grp_id,
                                        task,
                                        thread_handles,
                                        stack,
                                        stack_size,
                                        thread_names);
}


int
TAO_SSLIOP_Connection_Handler::svc (void)
{
  // This method is called when an instance is "activated", i.e.,
  // turned into an active object.  Presumably, activation spawns a
  // thread with this method as the "worker function".

  // Call the implementation here
  return this->svc_i ();
}


int
TAO_SSLIOP_Connection_Handler::handle_close (ACE_HANDLE handle,
                                             ACE_Reactor_Mask rm)
{
  if (TAO_orbdebug)
    ACE_DEBUG  ((LM_DEBUG,
                 "TAO (%P|%t) SSLIOP_Server_Connection_Handler::handle_close "
                 "(%d, %d)\n",
                 handle,
                 rm));

  --this->refcount_;
  if (this->refcount_ == 0 &&
      this->is_registered ())
    {
      // Set the flag to indicate that it is no longer registered with
      // the reactor, so that it isn't included in the set that is
      // passed to the reactor on ORB destruction.
      this->is_registered (0);

      // Decrement the reference count
      this->decr_ref_count ();
    }

  return 0;
}


ACE_HANDLE
TAO_SSLIOP_Connection_Handler::fetch_handle (void)
{
  return this->get_handle ();
}


int
TAO_SSLIOP_Connection_Handler::handle_timeout (const ACE_Time_Value &,
                                               const void *)
{
  // This method is called when buffering timer expires.
  //
  ACE_Time_Value *max_wait_time = 0;

  TAO_Stub *stub = 0;
  int has_timeout;
  this->orb_core ()->call_timeout_hook (stub,
                                        has_timeout,
                                        *max_wait_time);

  // Cannot deal with errors, and therefore they are ignored.
  this->transport ()->send_buffered_messages (max_wait_time);

  return 0;
}

int
TAO_SSLIOP_Connection_Handler::close (u_long)
{
  this->destroy ();

  return 0;
}


int
TAO_SSLIOP_Connection_Handler::add_handler_to_cache (void)
{
  ACE_INET_Addr addr;

  // Get the peername.
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  // Construct an  IIOP_Endpoint object
  TAO_IIOP_Endpoint tmpoint (addr,
                             0);

  TAO_SSLIOP_Endpoint endpoint (0,
                                &tmpoint);

  // Construct a property object
  TAO_Base_Connection_Property prop (&endpoint);

  // Add the handler to Cache
  return this->orb_core ()->connection_cache ().cache_handler (&prop,
                                                               this);
}


int
TAO_SSLIOP_Connection_Handler::handle_input (ACE_HANDLE h)
{
  return this->handle_input_i (h);
}



int
TAO_SSLIOP_Connection_Handler::handle_input_i (ACE_HANDLE,
                                               ACE_Time_Value *max_wait_time)
{
  int result;

  // Set up the SSLIOP::Current object.
  TAO_SSL_State_Guard ssl_state_guard (this, this->orb_core (),
                                       result);

  if (result == -1)
    return -1;

  this->refcount_++;

  // Call the transport read the message
  result = this->transport_.read_process_message (max_wait_time);

  // Now the message has been read
  if (result == -1 && TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - %p\n"),
                  ACE_TEXT ("IIOP_Connection_Handler::read_message \n")));

    }

  // The upcall is done. Bump down the reference count
  --this->refcount_;
  if (this->refcount_ == 0)
    this->decr_ref_count ();

  if (result == 0 || result == -1)
    {
      return result;
    }

  return 0;
}


int
TAO_SSLIOP_Connection_Handler::setup_ssl_state (TAO_ORB_Core *orb_core)
{
  // Make sure we have a valid reference to the SSLIOP::Current
  // object.
  if (CORBA::is_nil (this->current_.in ()))
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          CORBA::Object_var object =
            orb_core->orb ()->resolve_initial_references (
              "SSLIOPCurrent",
              ACE_TRY_ENV);
          ACE_TRY_CHECK;

          this->current_ = SSLIOP::Current::_narrow (object.in (),
                                                     ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (this->current_.in ()))
            ACE_TRY_THROW (CORBA::INV_OBJREF ());
        }
      ACE_CATCHANY
        {
          if (TAO_debug_level > 0)
            ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                 "Could not resolve "
                                 "\"SSLIOPCurrent\" object");

          return -1;
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (-1);
    }

  TAO_SSLIOP_Current *current =
    ACE_dynamic_cast (TAO_SSLIOP_Current *,
                      this->current_.in ());

  if (current == 0)   // Sanity check
    return -1;

  // Make the SSL session state available to the SSLIOP::Current
  // TSS object.
  this->current_impl_.ssl (this->peer ().ssl ());

  current->setup (&this->current_impl_);

  return 0;
}

void
TAO_SSLIOP_Connection_Handler::teardown_ssl_state (void)
{
  TAO_SSLIOP_Current *current =
    ACE_dynamic_cast (TAO_SSLIOP_Current *,
                      this->current_.in ());

  if (current != 0)
    current->teardown ();
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Handler<ACE_SSL_SOCK_STREAM, ACE_NULL_SYNCH>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Svc_Handler<ACE_SSL_SOCK_STREAM, ACE_NULL_SYNCH>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
