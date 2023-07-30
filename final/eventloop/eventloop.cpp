/***********************************************************************
* eventloop.cpp - Implements the event loop                            *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2023 Andreas Noe                                           *
*                                                                      *
* FINAL CUT is free software; you can redistribute it and/or modify    *
* it under the terms of the GNU Lesser General Public License as       *
* published by the Free Software Foundation; either version 3 of       *
* the License, or (at your option) any later version.                  *
*                                                                      *
* FINAL CUT is distributed in the hope that it will be useful, but     *
* WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU Lesser General Public License for more details.                  *
*                                                                      *
* You should have received a copy of the GNU Lesser General Public     *
* License along with this program.  If not, see                        *
* <http://www.gnu.org/licenses/>.                                      *
***********************************************************************/

#include <unistd.h>

#include <iostream>

#include "final/eventloop/eventloop.h"

namespace finalcut
{

//----------------------------------------------------------------------
// class EventLoop
//----------------------------------------------------------------------

// public methods of EventLoop
//----------------------------------------------------------------------
auto EventLoop::run() -> int
{
  running = true;

  while ( running )
  {
    processNextEvents();
  }

  return 0;
}

//----------------------------------------------------------------------
void EventLoop::leave()
{
  running = false;
}


// private methods of EventLoop
//----------------------------------------------------------------------
inline void EventLoop::processNextEvents()
{
  nfds_t fd_count = 0;
  monitors_changed = false;

  for (Monitor* monitor : monitors)
  {
    if ( monitor->isActive() )
    {
      fds[fd_count] = { monitor->getFd(), monitor->getEvents(), 0 };
      lookup_table[fd_count] = monitor;
      fd_count++;
    }

    if ( fd_count >= MAX_MONITORS )
      break;
  }

  int poll_result{};

  while ( true )
  {
    poll_result = poll(fds.data(), fd_count, WAIT_INDEFINITELY);

    if ( poll_result != -1 || errno != EINTR )
      break;
  }

  if ( poll_result <= 0 )
    return;

  // Dispatch events waiting in fds
  dispatcher(fd_count, poll_result);
}

//----------------------------------------------------------------------
inline void EventLoop::dispatcher (nfds_t fd_count, int poll_result)
{
  // Dispatching events that are waiting in fds
  nfds_t processed_fds{0};

  for (nfds_t index{0}; index < fd_count; index++)
  {
    const pollfd& current_fd = fds[index];

    if ( current_fd.revents == 0
      || ! (current_fd.revents & current_fd.events) )
      continue;

    // Call the event handler for current_fd
    lookup_table[index]->trigger(current_fd.revents);
    ++processed_fds;

    if ( monitors_changed || ! running
      || int(processed_fds) == poll_result )
      break;
  }
}

//----------------------------------------------------------------------
void EventLoop::addMonitor (Monitor* monitor)
{
  monitors.push_back(monitor);
  monitors_changed = true;
}

//----------------------------------------------------------------------
void EventLoop::removeMonitor (Monitor* monitor)
{
  monitors.remove(monitor);
  monitors_changed = true;
}

}  // namespace finalcut
