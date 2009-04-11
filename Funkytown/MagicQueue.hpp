/**
 * MagicQueue.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAGICQUEUE_HPP
#define MAGICQUEUE_HPP MAGICQUEUE_HPP

#include <QList>

class TheMagic;

class QListWidget;


class MagicQueue
{
public:
   MagicQueue();
   virtual ~MagicQueue();
   
   /*  */
   void addUrl( const QString &addurl, QListWidget *listWidget );
   /*  */
   void addMagic( TheMagic *addmagic );
   /*  */
   TheMagic *getMagic();
   
private:
   MagicQueue( const MagicQueue &other );
   MagicQueue &operator=( const MagicQueue &other );

   QList<TheMagic*> mMagicList;
};


#endif
