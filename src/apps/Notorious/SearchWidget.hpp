/*
 * src/apps/Notorious/SearchWidget.hpp
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
#include <QTime>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QString;

/* forward declaration of local classes */


/*!
  \addtogroup Notorious

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Notorious/SearchWidget_connect.dot" "Connect Graph"
*/
class SearchWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief \todo

    \param parent
    \param flags
   */
   SearchWidget( QWidget *parent = 0, Qt::WindowFlags flags = Qt::WindowFlags() );

   /*!
    \brief \todo

    \return const QString
   */
   const QString &getSQL() { return mSQL; }

public slots:
   /*!
    \brief \todo

   */
   void generateSQL();
   /*!
    \brief \todo

   */
   void checkSubmit();
   /*!
    \brief \todo

    \param r
   */
   void query1Running( bool r );
   /*!
    \brief \todo

    \param r
   */
   void query2Running( bool r );

signals:
   void submit( const QString &sql );

private:
   Q_DISABLE_COPY( SearchWidget )

   /*!
    \brief \todo

    \param time
    \return QString
   */
   QString time2frames( const QString &time );

   const QChar mStar; /*!< \brief \todo TODO */
   const QChar mPercent; /*!< \brief \todo TODO */
   const QChar mQuote; /*!< \brief \todo TODO */
   const QString mDoubleQuote; /*!< \brief \todo TODO */

   QLineEdit   *mpTitle; /*!< \brief \todo TODO */
   QCheckBox   *mpExact; /*!< \brief \todo TODO */
   QComboBox   *mpTrack; /*!< \brief \todo TODO */
   QLineEdit   *mpMinTime; /*!< \brief \todo TODO */
   QLineEdit   *mpMaxTime; /*!< \brief \todo TODO */
   QPushButton *mpSearchButton; /*!< \brief \todo TODO */
//   QPushButton *mpSettingsButton; /*!< \brief \todo TODO */
   QLabel      *mpSQLClause; /*!< \brief \todo TODO */
   QLabel      *mpQuery1Info; /*!< \brief \todo TODO */
   QLabel      *mpQuery2Info; /*!< \brief \todo TODO */
   QTime       mTime1; /*!< \brief \todo TODO */
   QTime       mTime2; /*!< \brief \todo TODO */
   QString     mSQL; /*!< \brief \todo TODO */
};

/*! @} */

#endif
