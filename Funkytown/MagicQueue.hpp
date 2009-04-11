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
   
   /*  */
   void addUrl( const QString &addurl );
   /*  */
   void addMagic( TheMagic *addmagic );
   /*  */
   TheMagic *getMagic();

public slots:
   /*  */
   void handleSelect( QListWidgetItem *item );
   
private:
   MagicQueue( const MagicQueue &other );
   MagicQueue &operator=( const MagicQueue &other );

   QList<TheMagic*> mMagicList;
   
   void updateList();
};


#endif
