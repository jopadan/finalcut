// fmenulist.cpp
// class FMenuList

#include "fmenulist.h"

//----------------------------------------------------------------------
// class FMenuList
//----------------------------------------------------------------------

// constructor and destructor
//----------------------------------------------------------------------
FMenuList::FMenuList()
{
}

//----------------------------------------------------------------------
FMenuList::~FMenuList()  // destructor
{
  // delete all items
  if ( ! itemlist.empty() )
  {
    std::vector<FMenuItem*>::iterator iter;
    iter = itemlist.begin();

    while ( iter != itemlist.end() )
    {
      (*iter)->setSuperMenu(0);
      //delAccelerator (*iter);
      iter = itemlist.erase(iter);
    }
  }
}


// private methods of FMenuList
//----------------------------------------------------------------------



// public methods of FMenuList
//----------------------------------------------------------------------
bool FMenuList::hasSelectedItem()
{
  if ( ! itemlist.empty() )
  {
    std::vector<FMenuItem*>::const_iterator iter, end;
    iter = itemlist.begin();
    end = itemlist.end();

    while ( iter != end )
    {
      if ( (*iter)->isSelected() )
        return true;
      ++iter;
    }
  }
  return false;
}

//----------------------------------------------------------------------
void FMenuList::insert (FMenuItem* item)
{
  itemlist.push_back(item);
}

//----------------------------------------------------------------------
void FMenuList::remove (FMenuItem* item)
{
  std::vector<FMenuItem*>::iterator iter;

  //delAccelerator (item);

  if ( itemlist.empty() )
    return;

  iter = itemlist.begin();
  while ( iter != itemlist.end() )
  {
    if ( (*iter) == item )
    {
      iter = itemlist.erase(iter);
      item->setSuperMenu(0);
      break;
    }
    else
      ++iter;
  }
}

//----------------------------------------------------------------------
void FMenuList::remove (int pos)
{
  if ( int(count()) < pos )
    return;

  itemlist.erase (itemlist.begin()+pos-1);
}

//----------------------------------------------------------------------
void FMenuList::clear()
{
  itemlist.clear();
}
