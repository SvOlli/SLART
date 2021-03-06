/*
 * src/apps/Funkytown/MagicQueue.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAGICQUEUE_HPP
#define MAGICQUEUE_HPP MAGICQUEUE_HPP

/* base class */
#include <QListWidget>

/* system headers */

/* Qt headers */
#include <QList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class TheMagic;


#define LIST_NEEDS_FIXING 0


/*!
  \addtogroup Funkytown

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Funkytown/MagicQueue_connect.dot" "Connect Graph"
*/
class MagicQueue : public QListWidget
{
   Q_OBJECT

public:
   MagicQueue( QWidget *parent = 0 );
   virtual ~MagicQueue();

   /*!
    \brief generate a magic item from url and cue it

   */
   void addUrl( const QString &addurl );
   /*!
    \brief cue an anready generated magic item

   */
   void addMagic( TheMagic *addmagic );
   /*!
    \brief get the next magic item from the cue

   */
   TheMagic *getMagic();

public slots:
   /*!
    \brief handler to set selection of an item in the queue

   */
   void handleSelect( QListWidgetItem *item );

private:
   Q_DISABLE_COPY( MagicQueue )

   QList<TheMagic*> mMagicList; /*!< \brief \todo TODO */

#if LIST_NEEDS_FIXING
   void updateList();
#endif
};

/*! @} */

#endif
