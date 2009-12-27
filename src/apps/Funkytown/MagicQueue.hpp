/**
 * src/apps/Funkytown/MagicQueue.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAGICQUEUE_HPP
#define MAGICQUEUE_HPP MAGICQUEUE_HPP

#include <QList>
#include <QListWidget>

#define LIST_NEEDS_FIXING 0

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
   
#if LIST_NEEDS_FIXING
   void updateList();
#endif
};

#endif
