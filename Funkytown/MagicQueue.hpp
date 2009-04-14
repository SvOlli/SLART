/**
 * MagicQueue.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAGICQUEUE_HPP
#define MAGICQUEUE_HPP MAGICQUEUE_HPP

#include <QList>
#include <QListWidget>

class TheMagic;


class MagicQueue : public QListWidget
{
Q_OBJECT
   
public:
   MagicQueue( QWidget *parent = 0 );
   virtual ~MagicQueue();
   
   /* generate a magic item from url and cue it */
   void addUrl( const QString &addurl );
   /* cue an anready generated magic item */
   void addMagic( TheMagic *addmagic );
   /* get the next magic item from the cue */
   TheMagic *getMagic();

public slots:
   /* handler to set selection of an item in the queue */
   void handleSelect( QListWidgetItem *item );
   
private:
   MagicQueue( const MagicQueue &other );
   MagicQueue &operator=( const MagicQueue &other );
   
   QList<TheMagic*> mMagicList;
   
   void updateList();
};

#endif
