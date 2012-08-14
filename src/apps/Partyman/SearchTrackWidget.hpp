/*
 * src/apps/Partyman/SearchTrackWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef SEARCHWIDGET_HPP
#define SEARCHWIDGET_HPP SEARCHWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QLabel;
class QLineEdit;
class QModelIndex;

/* forward declaration of local classes */
class Database;
class PlaylistContentWidget;
class PlaylistControlWidget;


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Partyman/SearchTrackWidget_connect.dot" "Connect Graph"
*/
class SearchTrackWidget : public QWidget
{
   Q_OBJECT

public:
   SearchTrackWidget( Database *database, QWidget *parent );
   virtual ~SearchTrackWidget();

public slots:
   /*!
    \brief execute search

   */
   void search();
   /*!
    \brief do something with selected items

   */
   void selectedEntries( const QModelIndex &index, int key );
   /*!
    \brief handle focus

   */
   void setFocus();
   /*!
    \brief update the number of found entries

   */
   void updateCounter();

signals:
   /*!
    \brief request add for tracks

    \param entries tracks to add
   */
   void requestAdd( const QStringList &entries );

private:
   SearchTrackWidget( const SearchTrackWidget &that );
   SearchTrackWidget &operator=( const SearchTrackWidget &that );

   Database              *mpDatabase;
   QWidget               *mpParent;
   PlaylistContentWidget *mpResults;
   QLineEdit             *mpInput;
   QLabel                *mpFound;
};

/*! @} */

#endif
