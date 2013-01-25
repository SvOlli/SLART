/*
 * src/apps/Stripped/StrippedMainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "StrippedMainWindow.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QTimer>

/* local library headers */
#include <GenericSatelliteHandler.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>
#include <WidgetShot.hpp>

/* local headers */
#include "CDDBClient.hpp"
#include "CDEdit.hpp"
#include "CDInfo.hpp"
#include "CDReader.hpp"
#include "ParanoiaStatus.hpp"
#include "StrippedConfigDialog.hpp"


StrippedMainWindow::StrippedMainWindow( QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
, mForbidMove( 50 )
, mpSatellite( Satellite::get() )
, mpGenericSatelliteHandler( 0 )
, mpCDInfo( new CDInfo() )
, mpCDDBClient( new CDDBClient( mpCDInfo, this ) )
, mpCDEdit( new CDEdit( mpCDInfo, mpCDDBClient, this ) )
, mpCDReader( new CDReader( mpCDInfo, mpCDEdit, this ) )
, mpParanoiaStatus( new ParanoiaStatus( this ) )
, mpMessage( new QLabel( this ) )
, mpConfigDialog( new StrippedConfigDialog( mpCDReader, this ) )
, mpActionSettings( new QAction( tr("Settings"), this) )
, mpActionCancel( new QAction( tr("Cancel"), this) )
, mpActionScanCD( new QAction( tr("Scan CD"), this) )
, mpActionRipTracks( new QAction( tr("Rip Tracks"), this) )
, mpActionEject( new QAction( tr("Eject"), this) )
, mpActionToggleRip( new QAction( tr("Toggle Rip"), this) )
, mpActionToggleEnqueue( new QAction( tr("Toggle Enqueue"), this) )
, mpActionCopyArtist( new QAction( tr("Copy Artist"), this) )
, mpActionNormArtist( new QAction( tr("Norm. Artist"), this) )
, mpActionNormTitle( new QAction( tr("Norm. Title"), this) )
, mpActionCopyYear( new QAction( tr("Copy Year"), this) )
{
   setWindowIcon( QIcon( ":/StrippedIcon.png" ) );
   setWindowTitle( QApplication::applicationName() );
   QApplication::setDoubleClickInterval( Settings::value( Settings::GlobalDoubleClickInterval ) );
   setCentralWidget( mpCDEdit );

   QToolBar *toolBarMain = new QToolBar( tr("Main Actions"), this );
   toolBarMain->setObjectName( "Main Actions" );
   toolBarMain->setToolButtonStyle( Qt::ToolButtonTextOnly );
   addToolAction( toolBarMain, mpActionSettings );
   addToolAction( toolBarMain, mpActionCancel );
   addToolAction( toolBarMain, mpActionScanCD );
   addToolAction( toolBarMain, mpActionRipTracks );
   addToolAction( toolBarMain, mpActionEject );

   QToolBar *toolBarEdit = new QToolBar( tr("Edit Tracks"), this );
   toolBarEdit->setObjectName( "Edit Tracks");
   toolBarEdit->setToolButtonStyle( Qt::ToolButtonTextOnly );
   addToolAction( toolBarEdit, mpActionToggleRip );
   addToolAction( toolBarEdit, mpActionToggleEnqueue );
   addToolAction( toolBarEdit, mpActionNormArtist );
   addToolAction( toolBarEdit, mpActionNormTitle );
   addToolAction( toolBarEdit, mpActionCopyArtist );
   addToolAction( toolBarEdit, mpActionCopyYear );

   QToolBar *toolBarMessage = new QToolBar( tr("Message"), this );
   toolBarMessage->setObjectName( "Message" );
   toolBarMessage->addWidget( mpMessage );

   QToolBar *toolBarParanoiaStatus = new QToolBar( tr("Paranoia Status"), this );
   toolBarParanoiaStatus->setObjectName( "Paranoia Status" );
   toolBarParanoiaStatus->addWidget( mpParanoiaStatus );
   mpParanoiaStatus->setToolBar( toolBarParanoiaStatus );
   connect( toolBarParanoiaStatus, SIGNAL(orientationChanged(Qt::Orientation)),
            mpParanoiaStatus, SLOT(changeOrientation(Qt::Orientation)) );

   QToolBar *toolBarCDReader = new QToolBar( tr("CDReader"), this );
   toolBarCDReader->setObjectName( "CDReader" );
   toolBarCDReader->addWidget( mpCDReader );

   QToolBar *toolBarEncoders = new QToolBar( tr("Encoders"), this );
   toolBarEncoders->setObjectName( "Encoders" );
   toolBarEncoders->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
   foreach( QAction *toggleEnableAction, mpConfigDialog->encoderToggleEnableActions() )
   {
      addToolAction( toolBarEncoders, toggleEnableAction, true );
   }

   QToolBar *toolBarCDDBClient = new QToolBar( tr("CDDBClient"), this );
   toolBarCDDBClient->setObjectName( "CDDBClient" );
   toolBarCDDBClient->addWidget( mpCDDBClient );

   addToolBar( Qt::BottomToolBarArea, toolBarMain );
   addToolBar( Qt::BottomToolBarArea, toolBarEdit );
   addToolBarBreak( Qt::BottomToolBarArea );
   addToolBar( Qt::BottomToolBarArea, toolBarMessage );
   addToolBarBreak( Qt::BottomToolBarArea );
   addToolBar( Qt::BottomToolBarArea, toolBarParanoiaStatus );
   addToolBarBreak( Qt::BottomToolBarArea );
   addToolBar( Qt::BottomToolBarArea, toolBarCDReader );
   addToolBarBreak( Qt::BottomToolBarArea );

   addToolBar( Qt::TopToolBarArea, toolBarEncoders );
   addToolBar( Qt::TopToolBarArea, toolBarCDDBClient );
   addToolBarBreak( Qt::TopToolBarArea );

   restoreGeometry( Settings::value( Settings::CommonGeometry ) );
   restoreState( Settings::value( Settings::CommonState ) );
   QList<QDockWidget*> docks( findChildren<QDockWidget*>() );
   foreach( QDockWidget *dock, docks )
   {
      restoreDockWidget( dock );
   }
   toolBarMain->toggleViewAction()->setDisabled( true );
   toolBarMain->show();

   /* buttons */
   connect( mpActionSettings, SIGNAL(triggered()),
            mpConfigDialog, SLOT(exec()) );

   connect( mpActionCancel, SIGNAL(triggered()),
            mpCDReader, SLOT(cancel()) );
   connect( mpActionCancel, SIGNAL(triggered()),
            mpCDDBClient, SIGNAL(cancel()) );

   connect( mpActionScanCD, SIGNAL(triggered()),
            mpCDReader, SLOT(readToc()) );

   connect( mpActionRipTracks, SIGNAL(triggered()),
            mpCDReader, SLOT(readTracks()) );
   connect( mpCDEdit, SIGNAL(containsData(bool)),
            mpActionRipTracks, SLOT(setEnabled(bool)) );

   connect( mpActionEject, SIGNAL(triggered()),
            this, SLOT(eject()) );
   connect( mpActionEject, SIGNAL(triggered()),
            mpCDDBClient, SIGNAL(eject()) );

   connect( mpActionToggleRip, SIGNAL(triggered()),
            mpCDEdit, SLOT(toggleAllRipFlags()) );
   connect( mpActionToggleEnqueue, SIGNAL(triggered()),
            mpCDEdit, SLOT(toggleAllEnqueueFlags()) );
   connect( mpActionCopyArtist, SIGNAL(triggered()),
            mpCDEdit, SLOT(copyTrackArtist()) );
   connect( mpActionNormArtist, SIGNAL(triggered()),
            mpCDEdit, SLOT(normalizeArtist()) );
   connect( mpActionNormTitle, SIGNAL(triggered()),
            mpCDEdit, SLOT(normalizeTitle()) );
   connect( mpActionCopyYear, SIGNAL(triggered()),
            mpCDEdit, SLOT(copyYear()) );

   /* edit sheet interaction */
   connect( mpCDReader, SIGNAL(gotToc()),
            mpCDEdit, SLOT(update()) );
   connect( mpCDDBClient, SIGNAL(gotdata()),
            mpCDEdit, SLOT(update()) );
   connect( mpCDReader, SIGNAL(setTrackDisabled(int,bool)),
            mpCDEdit, SLOT(setTrackDisabled(int,bool)) );
   connect( mpCDReader, SIGNAL(ensureVisible(int)),
            mpCDEdit, SLOT(ensureVisible(int)) );

   /* other interaction */
   connect( mpConfigDialog, SIGNAL(stateNoDrive()),
            this, SLOT(stateNoDrive()) );

   connect( mpCDReader, SIGNAL(gotToc()),
            mpCDDBClient, SIGNAL(cdinsert()) );
   connect( mpCDReader, SIGNAL(gotData()),
            mpCDDBClient, SIGNAL(gotdata()) );
   connect( mpCDReader, SIGNAL(stateNoDisc()),
            this, SLOT(stateNoDisc()) );
   connect( mpCDReader, SIGNAL(stateDisc()),
            this, SLOT(stateDisc()) );
   connect( mpCDReader, SIGNAL(stateScan()),
            this, SLOT(stateScan()) );
   connect( mpCDReader, SIGNAL(stateRip()),
            this, SLOT(stateRip()) );

   connect( mpCDDBClient, SIGNAL(stateNet()),
            this, SLOT(stateNet()) );
   connect( mpCDDBClient, SIGNAL(stateDisc()),
            this, SLOT(stateDisc()) );

   connect( mpCDDBClient, SIGNAL(requestCDText()),
            mpCDReader, SLOT(readCDText()) );
   connect( mpCDDBClient, SIGNAL(message(QString)),
            mpMessage, SLOT(setText(QString)) );
   connect( mpCDReader, SIGNAL(message(QString)),
            mpMessage, SLOT(setText(QString)) );

   connect( mpCDReader, SIGNAL(errors(int,uint,const ulong*)),
            mpParanoiaStatus, SLOT(setTrackData(int,uint,const ulong*)) );

   if( mpSatellite )
   {
      mpGenericSatelliteHandler = new GenericSatelliteHandler( mpSatellite, GenericSatelliteHandler::WithoutPing, this );
      connect( mpGenericSatelliteHandler, SIGNAL(updateConfig()),
               mpConfigDialog, SLOT(readSettings()) );
   }

   WidgetShot::addWidget( "MainWindow", this );
}


StrippedMainWindow::~StrippedMainWindow()
{
}


void StrippedMainWindow::addToolAction( QToolBar *bar, QAction *action, bool checkbox )
{
   bar->addAction( action );
   QToolButton *b = qobject_cast<QToolButton*>( bar->widgetForAction( action ) );
   b->setAutoRaise( false );
   b->setCheckable( true );
   if( checkbox )
   {
      b->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
      connect( action, SIGNAL(toggled(bool)),
               this, SLOT(changeEncoderIcon()) );
      changeEncoderIcon( action );
   }
   bar->addSeparator();
}


void StrippedMainWindow::changeEncoderIcon( QAction *action )
{
   if( !action )
   {
      action = qobject_cast<QAction*>(sender());
      if( !action )
      {
         return;
      }
   }
   QCommonStyle style;
   if( action->isChecked() )
   {
      action->setIcon( style.standardIcon( QStyle::SP_DialogApplyButton ) );
   }
   else
   {
      action->setIcon( style.standardIcon( QStyle::SP_DialogCancelButton ) );
   }
}


/* Evil workaround: in the first fifty events revert all move requests */
bool StrippedMainWindow::event( QEvent *event )
{
   if( mForbidMove > 0 )
   {
      if( event->type() == QEvent::Move )
      {
         restoreGeometry( Settings::value( Settings::CommonGeometry ) );
      }
      mForbidMove--;
   }
   return QMainWindow::event( event );
}


void StrippedMainWindow::changeTitle( const QIcon &icon, const QString &title )
{
   if( !icon.isNull() )
   {
      setWindowIcon( icon );
   }
   if( !title.isNull() )
   {
      setWindowTitle( title );
   }
}


void StrippedMainWindow::closeEvent( QCloseEvent *event )
{
   Settings::setValue( Settings::CommonGeometry, saveGeometry() );
   Settings::setValue( Settings::CommonState, saveState() );
   event->accept();
}


void StrippedMainWindow::eject()
{
   mpCDDBClient->clear();
   mpCDEdit->clear();
   mpCDReader->eject();
}


void StrippedMainWindow::stateNoDrive()
{
   mpCDEdit->clear();
   mpCDDBClient->setDisabled( true );
   mpCDEdit->setDisabled( true );
   mpActionSettings->setDisabled( false );
   mpActionCancel->setDisabled( true );
   mpActionScanCD->setDisabled( true );
   mpActionRipTracks->setDisabled( true );
   mpActionEject->setDisabled( true );
   QMessageBox::critical(this, QApplication::applicationName(),
         tr("No drive found capable of reading audio CDs.") );
}


void StrippedMainWindow::stateNoDisc()
{
   mpCDEdit->clear();
   mpCDDBClient->setDisabled( true );
   mpCDEdit->setDisabled( true );
   mpActionSettings->setDisabled( false );
   mpActionCancel->setDisabled( true );
   mpActionScanCD->setDisabled( false );
   mpActionRipTracks->setDisabled( true );
   mpActionEject->setDisabled( false );
}


void StrippedMainWindow::stateDisc()
{
   mpCDDBClient->setDisabled( false );
   mpCDEdit->setDisabled( false );
   mpActionSettings->setDisabled( false );
   mpActionCancel->setDisabled( true );
   mpActionScanCD->setDisabled( false );
   mpActionRipTracks->setDisabled( false );
   mpActionEject->setDisabled( false );
}


void StrippedMainWindow::stateScan()
{
   mpCDDBClient->setDisabled( true );
   mpCDEdit->setDisabled( false );
   mpActionSettings->setDisabled( true );
   mpActionCancel->setDisabled( false );
   mpActionScanCD->setDisabled( true );
   mpActionRipTracks->setDisabled( true );
   mpActionEject->setDisabled( true );
}


void StrippedMainWindow::stateNet()
{
   mpCDDBClient->setDisabled( true );
   mpCDEdit->setDisabled( false );
   mpActionSettings->setDisabled( true );
   mpActionCancel->setDisabled( false );
   mpActionScanCD->setDisabled( true );
   mpActionRipTracks->setDisabled( true );
   mpActionEject->setDisabled( true );
}


void StrippedMainWindow::stateRip()
{
   mpParanoiaStatus->clear();
   mpCDDBClient->setDisabled( true );
   mpCDEdit->setDisabled( false );
   mpActionSettings->setDisabled( true );
   mpActionCancel->setDisabled( false );
   mpActionScanCD->setDisabled( true );
   mpActionRipTracks->setDisabled( true );
   mpActionEject->setDisabled( true );
}
