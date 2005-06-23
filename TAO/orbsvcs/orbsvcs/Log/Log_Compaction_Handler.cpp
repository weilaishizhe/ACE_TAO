#include "orbsvcs/Log/Log_Compaction_Handler.h"
#include "orbsvcs/Log/Log_i.h"
#include "ace/Reactor.h"

ACE_RCSID (Log,
           Log_Compaction_Handler,
           "$Id$")

TAO_Log_Compaction_Handler::TAO_Log_Compaction_Handler (ACE_Reactor* reactor,
                                                        TAO_Log_i* log,
                                                        const ACE_Time_Value& interval)
  : timer_id_(-1),
    reactor_(reactor),
    log_(log),
    interval_(interval)
{
}


TAO_Log_Compaction_Handler::~TAO_Log_Compaction_Handler ()
{
  this->cancel ();
}


void
TAO_Log_Compaction_Handler::schedule ()
{
  this->cancel ();
  this->timer_id_ =
    this->reactor_->schedule_timer (this, 0, this->interval_, this->interval_);
}


void
TAO_Log_Compaction_Handler::cancel ()
{
  if (this->timer_id_ != -1)
    {
      this->reactor_->cancel_timer (timer_id_);
      this->timer_id_ = -1;
    }
}


int
TAO_Log_Compaction_Handler::handle_timeout (const ACE_Time_Value&,
                                            const void *)
{
  ACE_TRY_NEW_ENV
    {
      // TODO: can't invoke remove_old_records until access to record
      // store is protected by synchronization primitives.
#if 0
      this->log_->remove_old_records(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
#endif
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  
  return 0;
}
