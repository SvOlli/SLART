/**
 * GlobalHandlers.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef _GLOBALHANDLERS_HPP_
#define _GLOBALHANDLERS_HPP_ _GLOBALHANDLERS_HPP_

class DownloadHandler;
class PostDownloadHandler;

extern DownloadHandler     *gpDownloadHandler;
extern PostDownloadHandler *gpPostDownloadHandlerHTML;
extern PostDownloadHandler *gpPostDownloadHandlerXML;
extern PostDownloadHandler *gpPostDownloadHandlerMP3;
extern PostDownloadHandler *gpPostDownloadHandlerFLV;

#endif
