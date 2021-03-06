/*
 * src/Partyman/LogListModel.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef LOGLISTMODEL_HPP
#define LOGLISTMODEL_HPP LogListModel_HPP

/* base class */
#include <QListWidget>

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Innuendo

  @{
*/

/*!
 \brief \todo complete documentation

*/
class LogListWidget : public QListWidget
{
   Q_OBJECT

public:
   LogListWidget( QWidget *parent = 0 );

public slots:

protected:
   virtual QMimeData *mimeData ( const QList<QListWidgetItem *> items ) const;
   virtual QStringList mimeTypes () const;

private:
   Q_DISABLE_COPY( LogListWidget )
};

/*! @} */

#endif
