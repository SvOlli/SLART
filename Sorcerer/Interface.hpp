/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SORCERERINTERFACE_HPP
#define SORCERERINTERFACE_HPP SORCERERINTERFACE_HPP

class QApplication;

class SorcererInterface
{
public:
   virtual ~SorcererInterface() {}
   virtual int run( QApplication *app ) = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(SorcererInterface,
                    "org.svolli.SLART.SorcererInterface/1.0")
QT_END_NAMESPACE

#endif
